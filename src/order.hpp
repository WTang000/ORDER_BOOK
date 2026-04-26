#pragma once
#include <cstdint>

enum class Side : uint8_t { Buy, Sell };

struct Order{
    uint64_t order_id; 
    uint64_t price;
    uint64_t quantity;
    Side side;
    Order* next;
    Order* prev;
};