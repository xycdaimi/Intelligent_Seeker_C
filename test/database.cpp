#include "log_system/LoggingSystem.h"
#include "databases/database.hpp"
#include <string>

int main(int argc, char const *argv[])
{
    LoggingSystem::Initialize(LoggingSystem::Grade::INFO);

    DBMS::DataBase<int, std::string> data;

    data.Insert(0, "a");
    data.Insert(1, "b");
    data.Insert(2, "c");
    data.Insert(3, "d");
    data.Insert(4, "e");
    data.Insert(5, "f");

    data.Insert(0, "error");

    std::string value;
    if (data.Find(3, value) == true)
    {
        std::cout << value << std::endl;
    }

    data.Delete(3);
    data.Delete(3);

    if (data.Find(3, value) == true)
    {
        std::cout << value << std::endl;
    }

    data.Change(0, "hello world");

    if (data.is_empty() == false)
    {
        for (size_t i = 0; i <= data.size(); i++)
        {
            if (data.Find(i, value) == true)
            {
                std::cout << i << " : " << value << std::endl;
            }
            
        }
    }

    return 0;
}
