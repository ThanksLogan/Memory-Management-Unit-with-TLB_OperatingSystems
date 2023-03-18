//
// Created by shane on 3/16/2023.
//
#include <iostream>
#include "TLB.h"

void tlbCache::addEntry(unsigned int VP, unsigned int PF) {
    this->addressTime++;
    if(this->frames < this->size){
        this->times[VP] = addressTime;
        this->cache[VP] = PF;
        this->frames++;
    }else{
        int min_key = this->times.begin()->first;
        int min_value = this->times.begin()->second;
        for(auto i = this->times.begin(); i != this->times.end(); i++){
            if(i->second < min_value){
                min_value = i->second;
                min_key = i->first;
            }
        }
        this->cache.erase(min_key);
        this->times.erase(min_key);
        this->cache[VP] = PF;
        this->times[VP] = addressTime;
    }
}

