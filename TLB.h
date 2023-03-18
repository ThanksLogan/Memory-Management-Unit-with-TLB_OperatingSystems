//
// Created by shane on 3/16/2023.
//

#ifndef AS32_TLB_H
#define AS32_TLB_H

#define MAX_CAP 8
#include <string.h>
#include <map>
#include <algorithm>
#include <unordered_map>

class tlbCache {
public:
    unsigned int size;
    unsigned int addressTime;
    unsigned int frames;
    std::unordered_map<unsigned int, unsigned int> cache;
    std::unordered_map<unsigned int, unsigned int> times;
    tlbCache(unsigned int s){
        size = s;
        addressTime = 0;
        frames = 0;
    }
    void addEntry(unsigned int VP, unsigned int PF);
};




#endif //AS32_TLB_H
