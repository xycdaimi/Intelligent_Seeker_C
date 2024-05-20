#include "log_system/LoggingSystem.h"
#include <iostream>
#include <fstream>
#include <string>

/**
 * @file Reader.hpp
 * @author jiyilin 2474803745@qq.com
 * @brief this is a mandarin speech recognition module based on baidu ai
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */

namespace SpeechRecognition
{
    /**
     * @brief this is a speech recognition interface
     * 
     * @param data_str this variable serves as the return value of speech recognition results
     * @return true    successfully identified
     * @return false   recognition failed
     */
    bool working(std::string &data_str)
    {
        Log_INFO << "start SpeechRecognition..." << Log_END;

        system("python3 ./SpeechRecognition.py");

        std::ifstream SpeechRecognitionDrive;
        SpeechRecognitionDrive.open("./SpeechRecognitionBuffer", std::ios_base::in);
        if (SpeechRecognitionDrive.is_open() == false)
        {
            return false;
        }

        std::getline(SpeechRecognitionDrive, data_str);
        SpeechRecognitionDrive.close();

        return true;
    }

} // namespace SpeechRecognition
