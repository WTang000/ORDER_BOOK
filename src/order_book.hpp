#pragma once
#include <map>
#include <unordered_map>
#include <functional>
#include "order.hpp"
#include "price_level.hpp"
#include "pool_allocator.hpp"

struct OrderBook {
    static constexpr std::size_t MAX_ORDERS = 65536;

    PoolAllocator<Order, MAX_ORDERS> pool;

    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids;
    std::map<uint64_t, PriceLevel> asks;

    std::unordered_map<uint64_t, Order*> order_map;

    void add_order(uint64_t id, Side side, uint64_t price, uint64_t qty);
    void cancel_order(uint64_t id);
    void match();
};