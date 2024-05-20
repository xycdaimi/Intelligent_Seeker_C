#include "Reader/Reader.hpp"
#include <time.h>

using namespace ekho;

int main(int argc, char **argv)
{
    struct timespec sleepTime;
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 3000;

    Ekho wong("Mandarin");
    wong.setRate(30);
    wong.setPitch(-30);
    wong.blockSpeak("改革春风");
    nanosleep(&sleepTime, nullptr);

    wong.setRate(30);
    wong.setPitch(-30);
    wong.blockSpeak("吹满地");

    usleep(1);

    wong.setRate(30);
    wong.setPitch(-30);
    wong.blockSpeak("中国人民");
    nanosleep(&sleepTime, nullptr);

    wong.setRate(30);
    wong.setPitch(-30);
    wong.blockSpeak("真争气");

    return 0;
}
