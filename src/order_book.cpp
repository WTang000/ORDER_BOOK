#include "order_book.hpp"
#include <algorithm>

void OrderBook::add_order(uint64_t id, Side side, uint64_t price, uint64_t qty){
    Order* o = pool.allocate();
    o->order_id = id;
    o->side = side;
    o->price = price;
    o->quantity = qty;
    o->next = nullptr;
    o->prev = nullptr;

    if ( side == Side::Buy ){
        bids[price].push_back(o);
    }
    else{
        asks[price].push_back(o);
    }

    order_map[id] = o;
    match();
}

void OrderBook::cancel_order(uint64_t id){
    auto it = order_map.find(id);
    
    if ( it == order_map.end()){
        return;
    }
    
    Order* o = it->second;

    if ( o->side == Side::Buy ){
        bids[o->price].remove(o);
        if ( bids[o->price].quantity == 0 ){
            bids.erase(o->price);
        }
    }
    else{
        asks[o->price].remove(o);
        if ( asks[o->price].quantity == 0 ){
            asks.erase(o->price);
        }
    }

    pool.deallocate(o);
    order_map.erase(it);
}

void OrderBook::match(){
    while ( !(bids.empty()) && !(asks.empty()) ){
        auto& bid_level = bids.begin()->second;
        uint64_t bid_price = bids.begin()->first;

        auto& ask_level = asks.begin()->second;
        uint64_t ask_price = asks.begin()->first;

        if ( bid_price < ask_price ){
            break;
        }

        // get front orders
        Order* bid_order = bid_level.head;
        Order* ask_order = ask_level.head;

        // fill
        uint64_t fill_qty = std::min(bid_order->quantity, ask_order->quantity);

        // reduce qty
        bid_order->quantity -= fill_qty;
        ask_order->quantity -= fill_qty;
        bid_level.quantity -= fill_qty;
        ask_level.quantity -= fill_qty;

        if ( bid_order->quantity == 0 ){
            bid_level.remove(bid_order);
            order_map.erase(bid_order->order_id);
            pool.deallocate(bid_order);
            
            if ( bid_level.quantity == 0 ){
                bids.erase(bid_price);
            }
        }

        if ( ask_order->quantity == 0 ){
            ask_level.remove(ask_order);
            order_map.erase(ask_order->order_id);
            pool.deallocate(ask_order);

            if ( ask_level.quantity == 0 ){
                asks.erase(ask_price);
            }
        }
    }
}