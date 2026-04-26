#pragma once
#include "order.hpp"

// implement a queue using a doubly linked list
struct PriceLevel {
    uint64_t price = 0;
    uint64_t quantity = 0;
    Order* head = nullptr;
    Order* tail = nullptr;

    void push_back(Order* o){
        o->prev = tail;
        o->next = nullptr;
        if (tail){
            tail->next = o;
        }
        else{
            head = o;
        }
        tail = o;
        quantity += o->quantity;
    }

    void remove(Order* o){
        if ( o->prev ){
            o->prev->next = o->next;
        }
        else {
            head = o->next;
        }

        if ( o->next ){
            o->next->prev = o->prev;
        }
        else {
            tail = o->prev;
        }

        quantity -= o->quantity;
    }
};
