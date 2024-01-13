#ifndef ROVECOMM_BYTE_BUFFER_H
#define ROVECOMM_BYTE_BUFFER_H

#include <memory>
#include <stdlib.h>

/******************************************************************************
 * @brief Hacky replacement for a Java ByteBuffer. Host byte order, non-resizable.
 * Note: this is only meant to be used internally! Do not trust my code!
 *
 * @author OcelotEmpire (hobbz.pi@gmail.com)
 * @date 2023-12-22
 ******************************************************************************/
class RoveCommByteBuffer
{
    public:
        RoveCommByteBuffer() : RoveCommByteBuffer(0) {}

        RoveCommByteBuffer(size_t size) : m_pData(size > 0 ? std::make_unique<char>(size) : nullptr), m_size(size), m_position(0) {}

        RoveCommByteBuffer(std::unique_ptr<char>&& pData, size_t size) : m_pData(std::move(pData)), m_size(size), m_position(0) {}

        // RoveCommByteBuffer(RoveCommByteBuffer&& other) : m_size(other.m_size), m_position(other.m_position), m_pdata(std::move(other.pData)) {}

        template<typename T>
        size_t Get(T& out)
        {
            if (m_position + sizeof(T) > m_size)    // out of range
                return 0;

            union
            {
                    T value;
                    char data[sizeof(T)];
            } uConvert;

            for (int i = 0; i < sizeof(T); i++)
            {
                uConvert.data[i] = m_data[m_position];
                m_position++;
            }
            out = c.value;
            return sizeof(T);
        }

        template<typename T>
        size_t Put(T&& in)
        {
            if (m_position + sizeof(T) > m_size)    // out of range
                return 0;

            union
            {
                    T value;
                    char data[sizeof(T)];
            } uConvert;

            uConvert.value = in;
            for (int i = 0; i < sizeof(T); i++)
            {
                m_pData[m_position] = uConvert.data[i];
                m_position++;
            }
            return sizeof(T);
        }

        // Bulk movement
        // void Get(char* out, size_t count);
        // void Put(char* in, size_t count);

        // Reset position to 0.
        inline void Flip() { m_position = 0; }

        // Set internal pointer and size
        inline void Wrap(std::unique_ptr<char>&& pData, size_t size)
        {
            m_size     = size;
            m_position = 0;
            m_pData    = std::move(pData);
        }

        inline size_t GetSize() const { return m_size; }

        inline size_t GetPosition() const { return m_position; }

        inline bool SetPosition(size_t position) { m_position = position; }

        /******************************************************************************
         * @brief Release ownership of the internal pointer, resetting the buffer to be empty.
         *
         * @return char* - Internal pointer to the data.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        inline char* Release()
        {
            m_size     = 0;
            m_position = 0;
            return m_pData.release();
        }

        /******************************************************************************
         * @brief Get underlying raw pointer. The pointer will be deleted if the buffer goes out of scope.
         * If you need to read data, use RoveCommByteBuffer::Get() instead.
         *
         * @return char* - The raw pointer of this buffer.
         *
         * @author OcelotEmpire (hobbz.pi@gmail.com)
         * @date 2023-12-23
         ******************************************************************************/
        inline char* GetRawData() { return m_pData.get(); }

    private:
        std::unique_ptr<char> m_pData;
        size_t m_size;
        // position to get() and put(). if m_position >= m_size, then get() and put() will return 0.
        size_t m_position;
};

#endif
