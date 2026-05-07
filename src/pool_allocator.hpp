#pragma once
#include <cstddef> // std::size_t and std::byte
#include <new> // placement new

template<typename T, std::size_t N>
struct PoolAllocator {
    alignas(T) std::byte storage[N * sizeof(T)];
    T* free_list[N];
    std::size_t size = 0; // slots in free list

    PoolAllocator(){
        for ( int i = 0; i < N; ++i ){
            free_list[i] = reinterpret_cast<T*>(&storage[i * sizeof(T)]);
        }
        size = N;
    }

    T* allocate(){
        T* ptr = free_list[--size];
        new (ptr) T();
        return ptr;
    }

    void deallocate(T* p){
        p->~T();
        free_list[size++] = p;
    }
};