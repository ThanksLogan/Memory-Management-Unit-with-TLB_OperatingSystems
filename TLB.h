/* Names: Shane Wechsler, Logan Foreman
 * REDIDS: 823526399,
 * CS 480 Assignment 3
 * TLB header file */
#ifndef AS32_TLB_H
#define AS32_TLB_H

#define MAX_CAP 8
#include <string.h>
#include <map>
#include <algorithm>
#include <unordered_map>

class tlbCache {
public:
    /* size of cache */
    unsigned int size;
    /* address time to implement an LRU algorithm, updated with every insert and access */
    unsigned int addressTime;
    /* frame counter to track fill */
    unsigned int frames;
    /* where actual address->frame mappings stored */
    std::unordered_map<unsigned int, unsigned int> cache;
    /* where last access times stored, same keys */
    std::unordered_map<unsigned int, unsigned int> times;
    tlbCache(unsigned int s){
        size = s;
        addressTime = 0;
        frames = 0;
    }
    /* adds new mapping to cache, replaces entries if needed */
    void addEntry(unsigned int VP, unsigned int PF);
};




#endif //AS32_TLB_H
