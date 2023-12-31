 
#include <iostream>
#include "TLB.h"

void tlbCache::addEntry(unsigned int VP, unsigned int PF) {
    /* adding an entry, increase address count */
    this->addressTime++;
    /* cache not full yet, add new entry right away */
    if(this->frames < this->size){
        /* new time and VP slot */
        this->times[VP] = addressTime;
        this->cache[VP] = PF;
        this->frames++;
    }else{
        /* cache full, replace LRU entry */
        /*set min_key to the first VPN in times map */
        int min_key = this->times.begin()->first;
        /*set min_val to the first addressTime in times map */
        int min_value = this->times.begin()->second;
        /* iterate through may and find the smallest(oldest) address time in times */
        for(auto i = this->times.begin(); i != this->times.end(); i++){
            if(i->second < min_value){
                min_value = i->second;
                min_key = i->first;
            }
        }
        /* erase the oldest entry from times and cache map */
        this->cache.erase(min_key);
        this->times.erase(min_key);
        /* create the new entry */
        this->cache[VP] = PF;
        this->times[VP] = addressTime;
    }
}

