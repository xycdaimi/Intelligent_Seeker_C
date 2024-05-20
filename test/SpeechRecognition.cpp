#include "log_system/LoggingSystem.h"
#include "SpeechRecognition/SpeechRecognition.hpp"
#include <string>

int main(int argc, char const *argv[])
{
    std::string data;
    if (SpeechRecognition::working(data) == false)
    {
        Log_ERROR << "SpeechRecognition working error" << Log_END;
    }

    std::cout << data << std::endl;

    return 0;
}
