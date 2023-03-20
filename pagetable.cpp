//
// Created by shane on 3/19/2023.
//

#include <map>
#include "pagetable.h"

void level::insert_vpn2pfn(unsigned int address, unsigned int frame) {
    unsigned int index = (address & this->pageTablePtr->bitmaskAry[this->depth]) >> this->pageTablePtr->shiftAry[this->depth];
    if(this->depth == this->pageTablePtr->levelCount-1){
        this->mappings[index] = new Map(frame);
    }else{
        if(this->nextLevelPtr[index] == NULL) {
            this->nextLevelPtr[index] = new level(this->depth + 1);
            this->nextLevelPtr[index]->pageTablePtr = this->pageTablePtr;
        }
        this->nextLevelPtr[index]->insert_vpn2pfn(address, frame);
    }
}
Map* level::lookup_vpn2pfn(unsigned int virtualAddress){
    unsigned int index = (virtualAddress & this->pageTablePtr->bitmaskAry[this->depth]) >> this->pageTablePtr->shiftAry[this->depth];
    if(this->depth == this->pageTablePtr->levelCount-1){
        return this->mappings[index];
    }else{
        if(this->nextLevelPtr[index] != NULL){
            this->nextLevelPtr[index]->lookup_vpn2pfn(virtualAddress);
        }else{
            return NULL;
        }
    }
}

void pageTable::insert_vpn2pfn(unsigned int address, unsigned int frame) {
    this->rootNodePtr->insert_vpn2pfn(address, frame);
}

Map* pageTable::lookup_vpn2pfn(unsigned int virtualAddress) {
    return this->rootNodePtr->lookup_vpn2pfn(virtualAddress);
}