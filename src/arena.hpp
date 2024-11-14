#pragma once
#include <cstdlib>


class ArenaAllocator {
public:
    explicit ArenaAllocator(size_t bytesToAllocate) : m_size(bytesToAllocate) {
        m_buffer = static_cast<unsigned char *>(malloc(m_size));
        m_offset = m_buffer;
    }

    template<typename T>
    inline T *allocate() {
        void *offset = m_offset;
        m_offset += sizeof(T);
        return static_cast<T *>(offset);
    }

    inline ArenaAllocator(const ArenaAllocator &other) = delete;

    inline ArenaAllocator &operator=(const ArenaAllocator &other) = delete;

    ~ArenaAllocator() {
        free(m_buffer);
    }

private:
    size_t m_size;
    unsigned char *m_buffer;
    unsigned char *m_offset;
};
