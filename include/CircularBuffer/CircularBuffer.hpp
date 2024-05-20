#pragma once

/**
 * @file CircularBuffer.hpp
 * @author jiyilin 2474803745@qq.com
 * @brief this is a program for creating and using multithreaded circular buffer
 * @version 0.1
 * @date 2023-07-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "log_system/LoggingSystem.h"
#include <condition_variable>
#include <mutex>
#include <vector>
#include <chrono>
#include <thread>

namespace Memory
{
    /**
     * @brief this class is used to use ring buffers
     *
     * @tparam DATA data
     *
     * @memberof data              : ring buffer for saving data
     * @memberof m_read_lock       : read lock
     * @memberof m_write_lock      : write lock
     *
     * @memberof m_cv              : condition variable to determine whether reading and writing can continue
     *
     * @memberof max_size          : maximum value
     * @memberof count             : existing data volume
     * @memberof head              : buffer read header
     * @memberof end               : buffer write header
     * @memberof is_working        : identification of whether the buffer works normally
     */
    template <typename DATA>
    class CircularBuffer
    {
    private:
        std::vector<DATA> data;

        std::mutex m_read_lock;
        std::mutex m_write_lock;

        std::condition_variable m_cv;

        int max_size;

        int count;

        int head;

        int end;

        bool is_working = false;

    private:
        /**
         * @brief determine whether the buffer is empty
         *
         * @return true  : no data in buffer
         * @return false : buffer has data
         */
        bool is_empty()
        {
            if (this->count == 0)
            {
                return true;
            }
            return false;
        }

        /**
         * @brief is the buffer full
         *
         * @return true  : full
         * @return false : dissatisfaction
         */
        bool is_full()
        {
            if (this->count == this->max_size)
            {
                return true;
            }
            return false;
        }

    public:
        /**
         * @brief Construct a new Circular Buffer object
         *
         * @param size : buffer size
         */
        CircularBuffer(int size)
        {
            std::unique_lock<std::mutex> lock_w(this->m_write_lock);
            std::unique_lock<std::mutex> lock_r(this->m_read_lock);

            if (size <= 0)
            {
                Log_ERROR << "Buffer size must be greater than 0" << Log_END;
            }
            else
            {
                this->data.reserve(size);
                for (int i = 0; i < size; i++)
                {
                    DATA process;
                    this->data.emplace_back(process);
                }

                this->max_size = size;
                this->count = 0;
                this->head = 0;
                this->end = 0;
                this->is_working = true;
            }
        }

        /**
         * @brief Get the Data Size object
         * 
         * @return int 
         */
        int getDataSize()
        {
            std::unique_lock<std::mutex> lock_w(this->m_write_lock);
            std::unique_lock<std::mutex> lock_r(this->m_read_lock);

            if (this->is_working == false)
            {
                return -1;
            }

            return this->count;
        }

        /**
         * @brief clear buffer
         * 
         * @return true  : clear successfully
         * @return false : empty failed
         */
        bool clearBuffer()
        {
            std::unique_lock<std::mutex> lock_w(this->m_write_lock);
            std::unique_lock<std::mutex> lock_r(this->m_read_lock);

            if (this->is_working == false)
            {
                Log_ERROR << "the buffer has not started working" << Log_END;
                return false;
            }

            this->count = 0;
            this->head = 0;
            this->end = 0;

            return true;
        }

        /**
         * @brief Get the Max size object
         * 
         * @return int 
         */
        int getMax_size()
        {
            return this->max_size;
        }

        /**
         * @brief insert data into the buffer
         * 
         * @param data   : data to be added
         * @return true  : successfully added
         * @return false : add failed
         */
        bool insert(DATA data)
        {
            std::unique_lock<std::mutex> lock(this->m_write_lock);

            if (this->is_working == false)
            {
                return false;
            }

            this->m_cv.wait(lock, [this]
                            { return this->is_full() == false; });

            try
            {
                this->data[this->end] = data;
                this->end = (this->end + 1) % this->max_size;
                ++this->count;
            }
            catch (const std::exception &e)
            {
                Log_ERROR << e.what() << Log_END;
            }

            lock.unlock();
            this->m_cv.notify_one();

            return true;
        }

        /**
         * @brief eject data from buffer
         * 
         * @param data   : container for receiving pop up data
         * @return true  : eject successfully
         * @return false : eject failed
         */
        bool eject(DATA &data)
        {
            std::unique_lock<std::mutex> lock(this->m_read_lock);

            if (this->is_working == false)
            {
                return false;
            }

            this->m_cv.wait(lock, [this]
                            { return this->is_empty() == false; });

            try
            {
                data = this->data[this->head];
                this->head = (this->head + 1) % this->max_size;
                --this->count;
            }
            catch (const std::exception &e)
            {
                Log_ERROR << e.what() << Log_END;
            }

            lock.unlock();
            this->m_cv.notify_one();
            return true;
        }

        /**
         * @brief Destroy the Circular Buffer object
         * 
         */
        ~CircularBuffer(){};
    };
} // namespace Memory
