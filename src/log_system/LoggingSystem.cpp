#include "log_system/LoggingSystem.h"

#ifdef LOG_SYSTEM_SAVE
#include <chrono>
#include <thread>
#include <string>
#include <time.h>
#include <sstream>
#endif // LOG_SYSTEM_SAVE

namespace LoggingSystem
{
    /**
     * @brief save current log level
     *
     */
    Grade nowGrade = Grade::INFO;

    /**
     * @brief log file pointer
     *
     */
    std::ofstream LogFile;

#ifdef LOG_SYSTEM_SAVE
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
        stream << "./log/" << clock->tm_year + 1900 << "_" << clock->tm_mon + 1 << "_" << clock->tm_mday << "_" << clock->tm_hour << ":" << clock->tm_min << ":" << clock->tm_sec << ".log";

        std::string data;
        stream >> data;

        Log_INFO << "the file name of this video is : " << data << Log_END;
        std::cout << "the file name of this video is : " << data << std::endl;
        return data;
    }
#endif // LOG_SYSTEM_SAVE

    /**
     * @brief this function is used to initialize the logging system
     *
     * @param now     : current log level
     * @return true   : log system initialization successful
     * @return false  : log system initialization failed
     */
    bool Initialize(Grade now)
    {
        nowGrade = now;

#ifdef LOG_SYSTEM_SAVE
        LogFile.open(Get_Now_File_Name(), std::ios_base::out);
        return LogFile.is_open();
#endif // LOG_SYSTEM_SAVE

        return true;
    }

    /**
     * @brief this function is used to shut down the logging system and release the file pointer
     *
     */
    void Release()
    {
#ifdef LOG_SYSTEM_SAVE
        if (LogFile.is_open() == true)
        {
            LogFile.close();
        }
#endif // LOG_SYSTEM_SAVE
    }

} // namespace LoggingSystem
