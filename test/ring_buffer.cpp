# include "log_system/LoggingSystem.h"
# include "CircularBuffer/CircularBuffer.hpp"

void WriteThread(Memory::CircularBuffer<int *> *buffer)
{
    for (int i = 0; i < 100; i++)
    {
        int *process = new int;
        *process = i;
        buffer->insert(process);
    }
}

void TestThread(Memory::CircularBuffer<int *> *buffer)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (buffer->getDataSize() != 0)
        {
            std::cout << "buffer size = " << buffer->getDataSize() << std::endl;
        }
        
    }
}

int main()
{
    Memory::CircularBuffer<int *> buffer(10);

    std::thread writeThreader(WriteThread, &buffer);

    std::thread testThreader(TestThread, &buffer);

    int *output;
    while (true)
    {
        if (buffer.eject(output) == true)
        {
            std::cout << *output << std::endl;
        }
    }

    return 0;
}
