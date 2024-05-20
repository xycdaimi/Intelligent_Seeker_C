#pragma once

/**
 * @file database.hpp
 * @author jiyilin 2474803745@qq.com
 * @brief this is a database system
 * @version 0.1
 * @date 2023-07-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <unordered_map>
#include <mutex>

namespace DBMS
{
    /**
     * @brief this is a hashi bucket based database
     *
     * @tparam    INDEX             : hashi bucket key value
     * @tparam    DATA              : data

     * @memberof  Data              : hashi bucket for saving data
     * @memberof  m_read_write_lock : readers writer lock
     */
    template <typename INDEX, typename DATA>
    class DataBase
    {
    private:
        std::unordered_map<INDEX, DATA> Data;

        std::mutex m_read_write_lock;

    public:
        /**
         * @brief Construct a new Data Base object
         *
         */
        DataBase() : m_read_write_lock(){};

        /**
         * @brief this function is used to add a key value pair to the database
         *
         * @param index  : key
         * @param value  : data
         * @return true  : successfully added
         * @return false : add failed
         */
        bool Insert(INDEX index, DATA value)
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            if (this->Data.count(index) != 0)
            {
                Log_WARM << index << "already exists , unable to add the data" << Log_END;
                return false;
            }

            try
            {
                this->Data.insert(std::pair(index, value));
            }
            catch (const std::exception &e)
            {
                Log_ERROR << e.what() << Log_END;
                return false;
            }

            return true;
        }

        /**
         * @brief this function is used to search for data in the database
         *
         * @param index  : key for the value to be searched for
         * @param value  : return search results
         * @return true  : the data to be searched exists
         * @return false : the data to be searched does not exist
         */
        bool Find(INDEX index, DATA &value)
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            auto target = this->Data.find(index);
            if (target == this->Data.end())
            {
                return false;
            }

            value = (*target).second;
            return true;
        }

        /**
         * @brief this function is used to delete a record in the database
         *
         * @param index  : key to be deleted
         * @return true  : successfully deleted
         * @return false : delete failed
         */
        bool Delete(INDEX index)
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            auto target = this->Data.find(index);
            if (target == this->Data.end())
            {
                Log_WARM << index << "not present" << Log_END;
                return false;
            }

            this->Data.erase((*target).first);
            return true;
        }

        /**
         * @brief this function is used to modify database data
         *
         * @param index  : the key of the key value pair to be modified
         * @param value  : new data to be written
         * @return true  : successfully modified
         * @return false : modification failed
         */
        bool Change(INDEX index, DATA value)
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            auto target = this->Data.find(index);
            if (target == this->Data.end())
            {
                Log_WARM << index << "not present" << Log_END;
                return false;
            }

            (*target).second = value;
            return true;
        }

        /**
         * @brief determine if the database is empty
         *
         * @return true  : database has no data
         * @return false : database has data
         */
        bool is_empty()
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            return this->Data.empty();
        }

        /**
         * @brief get database size
         *
         * @return int
         */
        int size()
        {
            std::unique_lock<std::mutex> lock(this->m_read_write_lock);

            return this->Data.size();
        }

        /**
         * @brief Destroy the Data Base object
         *
         */
        ~DataBase(){};
    };

} // namespace DBMS
