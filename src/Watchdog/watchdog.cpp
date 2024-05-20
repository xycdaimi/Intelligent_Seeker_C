// example/C/demo.c

// Copyright 2017  KITT.AI (author: Guoguo Chen)

#include "WakeWord/Watchdog.h"
#include "log_system/LoggingSystem.h"

namespace Watchdog
{
  // Pointer to the ring buffer memory.
  char *g_ringbuffer;
  // Ring buffer wrapper used in PortAudio.
  PaUtilRingBuffer g_pa_ringbuffer;
  // Pointer to PortAudio stream.
  PaStream *g_pa_stream;
  // Number of lost samples at each LoadAudioData() due to ring buffer overflow.
  int g_num_lost_samples;
  // Wait for this number of samples in each LoadAudioData() call.
  int g_min_read_samples;
  // Pointer to the audio data.
  int16_t *g_data;

  int PortAudioCallback(const void *input,
                        void *output,
                        unsigned long frame_count,
                        const PaStreamCallbackTimeInfo *time_info,
                        PaStreamCallbackFlags status_flags,
                        void *user_data)
  {
    ring_buffer_size_t num_written_samples =
        PaUtil_WriteRingBuffer(&g_pa_ringbuffer, input, frame_count);
    g_num_lost_samples += frame_count - num_written_samples;
    return paContinue;
  }

  void StartAudioCapturing(int sample_rate,
                           int num_channels, int bits_per_sample)
  {
    g_data = NULL;
    g_num_lost_samples = 0;
    g_min_read_samples = sample_rate * 0.1;

    // Allocates ring buffer memory.
    int ringbuffer_size = 16384;
    g_ringbuffer = (char *)(PaUtil_AllocateMemory(bits_per_sample / 8 * ringbuffer_size));
    if (g_ringbuffer == NULL)
    {
      Log_ERROR << "Fail to allocate memory for ring buffer." << Log_END;
      exit(1);
    }

    // Initializes PortAudio ring buffer.
    ring_buffer_size_t rb_init_ans =
        PaUtil_InitializeRingBuffer(&g_pa_ringbuffer, bits_per_sample / 8,
                                    ringbuffer_size, g_ringbuffer);
    if (rb_init_ans == -1)
    {
      Log_ERROR << "Ring buffer size is not power of 2." << Log_END;
      exit(1);
    }

    // Initializes PortAudio.
    PaError pa_init_ans = Pa_Initialize();
    if (pa_init_ans != paNoError)
    {
      Log_ERROR << "Fail to initialize PortAudio, error message is " << Pa_GetErrorText(pa_init_ans) << Log_END;
      exit(1);
    }

    PaError pa_open_ans;
    if (bits_per_sample == 8)
    {
      pa_open_ans = Pa_OpenDefaultStream(
          &g_pa_stream, num_channels, 0, paUInt8, sample_rate,
          paFramesPerBufferUnspecified, PortAudioCallback, NULL);
    }
    else if (bits_per_sample == 16)
    {
      pa_open_ans = Pa_OpenDefaultStream(
          &g_pa_stream, num_channels, 0, paInt16, sample_rate,
          paFramesPerBufferUnspecified, PortAudioCallback, NULL);
    }
    else if (bits_per_sample == 32)
    {
      pa_open_ans = Pa_OpenDefaultStream(
          &g_pa_stream, num_channels, 0, paInt32, sample_rate,
          paFramesPerBufferUnspecified, PortAudioCallback, NULL);
    }
    else
    {
      Log_ERROR << "Unsupported BitsPerSample: " << bits_per_sample << Log_END;
      exit(1);
    }
    if (pa_open_ans != paNoError)
    {
      Log_ERROR << "Fail to open PortAudio stream, error message is " << Pa_GetErrorText(pa_open_ans) << Log_END;
      exit(1);
    }

    PaError pa_stream_start_ans = Pa_StartStream(g_pa_stream);
    if (pa_stream_start_ans != paNoError)
    {
      Log_ERROR << "Fail to start PortAudio stream, error message is " << Pa_GetErrorText(pa_stream_start_ans) << Log_END;
      exit(1);
    }
  }

  void StopAudioCapturing()
  {
    if (g_data != NULL)
    {
      free(g_data);
      g_data = NULL;
    }
    Pa_StopStream(g_pa_stream);
    Pa_CloseStream(g_pa_stream);
    Pa_Terminate();
    PaUtil_FreeMemory(g_ringbuffer);
  }

  int LoadAudioData()
  {
    if (g_data != NULL)
    {
      free(g_data);
      g_data = NULL;
    }

    // Checks ring buffer overflow.
    if (g_num_lost_samples > 0)
    {
      // Log_ERROR << "Lost " << g_num_lost_samples << " samples due to ring buffer overflow. " << Log_END;
      g_num_lost_samples = 0;
    }

    ring_buffer_size_t num_available_samples = 0;
    while (true)
    {
      num_available_samples =
          PaUtil_GetRingBufferReadAvailable(&g_pa_ringbuffer);
      if (num_available_samples >= g_min_read_samples)
      {
        break;
      }
      Pa_Sleep(5);
    }

    // Reads data.
    num_available_samples = PaUtil_GetRingBufferReadAvailable(&g_pa_ringbuffer);
    g_data = (int16_t *)malloc(num_available_samples * sizeof(int16_t));
    ring_buffer_size_t num_read_samples = PaUtil_ReadRingBuffer(
        &g_pa_ringbuffer, g_data, num_available_samples);
    if (num_read_samples != num_available_samples)
    {
      Log_ERROR << num_available_samples << " samples were available, but only " << num_read_samples << " samples were read" << Log_END;
    }
    return num_read_samples;
  }

  void working(std::string resource, std::string model, std::string sensitivity_str, bool *is_exit, FUNCTION fun, void *path)
  {
    float audio_gain = 1;
    bool apply_frontend = false;

    // Initializes Snowboy detector.
    SnowboyDetect *detector = SnowboyDetectConstructor(resource.c_str(), model.c_str());
    SnowboyDetectSetSensitivity(detector, sensitivity_str.c_str());
    SnowboyDetectSetAudioGain(detector, audio_gain);
    SnowboyDetectApplyFrontend(detector, apply_frontend);

    StartAudioCapturing(SnowboyDetectSampleRate(detector),
                        SnowboyDetectNumChannels(detector),
                        SnowboyDetectBitsPerSample(detector));

    // Runs the detection.
    std::cout << "Listening..." << std::endl;
    while ((*is_exit) == false)
    {
      int array_length = LoadAudioData();
      if (array_length != 0)
      {
        int result = SnowboyDetectRunDetection(detector,
                                               g_data, array_length, false);
        if (result > 0)
        {
          fun(path);
        }
      }
    }

    StopAudioCapturing();
    SnowboyDetectDestructor(detector);
  }

} // namespace Watchdog
