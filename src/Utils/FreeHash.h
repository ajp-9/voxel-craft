#pragma once

struct FreeHash
{
    template<typename T>
    static inline void freeHash(T& p_container)
    {
        T empty;
        using std::swap;
        swap(p_container, empty);
    }
};