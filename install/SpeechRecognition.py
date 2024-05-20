from aip import AipSpeech
import speech_recognition

#! ======   AUDITORY_PARAMETERS   ======

APP_ID = ''
API_KEY = ''
SECRET_KEY = ''

#! ======   THIS_IS_THE_IDENTIFICATION_ZONE   ======

class SpeechMachine():

    machine = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
    reader = speech_recognition.Recognizer()
    microPhone = speech_recognition.Microphone()

    def Get_String(self,audio):

        result = self.machine.asr(audio,'wav',16000, {'dev_pid': 1537,})
        try:
            answer = result['result'][0]
        except Exception as e:
            print(e)
            answer = ""

        return answer

    def Listening(self):

        print("Please speaking...")
        
        with self.microPhone as source:
            self.reader.adjust_for_ambient_noise(source)
            audio = self.reader.listen(source)
        
        data = audio.get_wav_data(convert_rate=16000)
        
        print("Analyzing...")

        answer = self.Get_String(data)

        return answer

#! ======   THIS_IS_THE_MAIN_WORK_SPACE   ======

speechMachine = SpeechMachine()
SpeechRecognitionBuffer = open("./SpeechRecognitionBuffer","w")

str_answer = speechMachine.Listening()

print(str_answer,file=SpeechRecognitionBuffer)
SpeechRecognitionBuffer.close()
