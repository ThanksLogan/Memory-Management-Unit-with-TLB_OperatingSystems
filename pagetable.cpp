//
// Created by shane on 3/19/2023.
//

#include <map>
#include "pagetable.h"

void level::insert_vpn2pfn(unsigned int address, unsigned int frame) {
    /* finds index for nextLevelPtr/mappings, uses the bitmask at given level, then shifts to the right with the needed shift */
    unsigned int index = (address & this->pageTablePtr->bitmaskAry[this->depth]) >> this->pageTablePtr->shiftAry[this->depth];
    /* depth is at last level, leaf node */
    if(this->depth == this->pageTablePtr->levelCount-1){
        /* check if mappings has been initialized, init if not */
        if(this->mappings == NULL){
            this->mappings = new Map*[this->pageTablePtr->entryCount[this->depth]]{};
        }
        /* assign index to frame number */
        this->mappings[index] = new Map(frame);
    }else{
        /* check if nextLevelPointer instantiated */
        if(this->nextLevelPtr == NULL){
            this->nextLevelPtr = new level*[this->pageTablePtr->entryCount[this->depth]]{};
        }
        /* sees if given entry has been assigned already, if not, make a new level there, with current depth + 1 */
        if(this->nextLevelPtr[index] == NULL) {
            this->nextLevelPtr[index] = new level(this->depth+1);
            this->nextLevelPtr[index]->pageTablePtr = this->pageTablePtr;
        }
        /* recursive call till we find leaf node */
        this->nextLevelPtr[index]->insert_vpn2pfn(address, frame);
    }
}
Map* level::lookup_vpn2pfn(unsigned int virtualAddress){
    /* finds index for nextLevelPtr/mappings, uses the bitmask at given level, then shifts to the right with the needed shift */
    unsigned int index = (virtualAddress & this->pageTablePtr->bitmaskAry[this->depth]) >> this->pageTablePtr->shiftAry[this->depth];
    /* leaf node depth */
    if(this->depth == this->pageTablePtr->levelCount-1){
        /* if mappings hasn't been instantiated, no mapping found, miss */
        if(this->mappings == NULL){
            return NULL;
        }
        /* return mapping even if it's NULL, will be handled in main flow */
        return this->mappings[index];
    }else{
        /* checks if nextLevelPtr even exists, miss if not */
        if(this->nextLevelPtr == NULL){
            return NULL;
        }
        /* traverse nextLevelPtr if there's a level entry there */
        if(this->nextLevelPtr[index] != NULL){
            return this->nextLevelPtr[index]->lookup_vpn2pfn(virtualAddress);
        }else{
            return NULL;
        }
    }
}

/* immediately just calls its level ptr to handle lookups/inserts */
void pageTable::insert_vpn2pfn(unsigned int address, unsigned int frame) {
    this->rootNodePtr->insert_vpn2pfn(address, frame);
}

Map* pageTable::lookup_vpn2pfn(unsigned int virtualAddress) {
    return this->rootNodePtr->lookup_vpn2pfn(virtualAddress);
}