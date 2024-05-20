#pragma once

/**
 * @file Monitor.h
 * @author jiyilin 2474803745@qq.com
 * @brief this is a video recording program based on open cv
 * @version 0.1
 * @date 2023-08-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <opencv2/opencv.hpp>
#include <log_system/LoggingSystem.h>
#include <sys/statvfs.h>
#include <chrono>
#include <thread>
#include <string>
#include <time.h>

#define MINIMUM_MEMORY_SAVED_FOR_RUNNING 200

/**
 * @brief this class is used for recording videos
 *
 * @memberof writer  : video file handle
 * @memberof state   : class working status
 *
 */
class Monitor
{
private:
    cv::VideoWriter *writer;

    bool state = false;

private:
    /**
     * @brief Get the Free Space object
     *
     * @return long
     */
    long getFreeSpace()
    {
        std::string path = "/";
        struct statvfs stat;
        if (statvfs(path.c_str(), &stat) != 0)
        {
            Log_ERROR << "getting information failure" << Log_END;
            return -1;
        }
        // 计算可用空间(单位：MB)
        long freeSpace = stat.f_frsize * stat.f_bfree / 1024 / 1024;
        return freeSpace;
    }

    /**
     * @brief this function is used to encode a video file name based on the current time
     *
     * @return std::string
     */
    std::string Get_Now_File_Name()
    {
        auto now = std::chrono::system_clock::now();
        time_t time = std::chrono::system_clock::to_time_t(now);

        auto clock = localtime(&time);

        std::stringstream stream;
        stream << "./videos/" << clock->tm_year + 1900 << "_" << clock->tm_mon + 1 << "_" << clock->tm_mday << "_" << clock->tm_hour << ":" << clock->tm_min << ":" << clock->tm_sec << ".log";

        std::string data;
        stream >> data;

        Log_INFO << "the file name of this video is : " << data << Log_END;
        return data;
    }

public:
    /**
     * @brief Construct a new Monitor object
     *
     * @param height  : video frame width
     * @param width   : video frame height
     * @param save    : save video
     */
    Monitor(int height, int width, bool save)
    {
        if (save == true)
        {
            if (getFreeSpace() < MINIMUM_MEMORY_SAVED_FOR_RUNNING)
            {
                Log_ERROR << "insufficient memory space" << Log_END;
            }
            else
            {
                this->writer = new cv::VideoWriter(Get_Now_File_Name(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(width, height));
                if (this->writer->isOpened() == true)
                {
                    Log_INFO << "VideoWriter created successfully" << Log_END;
                    this->state = true;
                }
                else
                {
                    Log_ERROR << "VideoWriter Creation Failed" << Log_END;
                }
            }
        }
        else
        {
            Log_INFO << "monitor has been set to not save video streams" << Log_END;
        }
    }

    /**
     * @brief this function is used to write a frame to a video file
     *
     * @param image   frame data to be written
     * @return true   write successful
     * @return false  write failed
     */
    bool Read(cv::Mat image)
    {
        if (this->is_working() == false)
        {
            Log_ERROR << "VideoWriter not started working" << Log_END;
            return false;
        }

        if (image.empty() == true)
        {
            Log_ERROR << "image is NULL" << Log_END;
            return false;
        }

        this->writer->write(image);
        return true;
    }

    /**
     * @brief this function is used to check the working status of the device
     *
     * @return true   the device is in working condition
     * @return false  the device is in an inactive state
     */
    bool is_working()
    {
        return this->state;
    }

    /**
     * @brief release Monitor
     *
     */
    void release()
    {
        this->writer->release();
        Log_INFO << "VideoWriter ~Monitor finish" << Log_END;
        this->state = false;
    }

    /**
     * @brief Destroy the Monitor object
     *
     */
    ~Monitor()
    {
        this->release();
    }
};