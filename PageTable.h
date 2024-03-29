//
// Created by shane on 3/15/2023.
//

#ifndef AS3_PAGETABLE_H
#define AS3_PAGETABLE_H

#include <vector>
#include "level.h"

class PageTable {
public:
    unsigned int levelCount;
    vector<const unsigned char>bitmaskAry;
    vector<int> bitShift;
    vector<long> entryCount;

    Level *rootNodePtr;
    void insert_vpn2pfn(PageTable *pageTablePtr, unsigned int address, unsigned int frame){
        void insert_vpn2pfn(pageTablePtr->rootNodePtr, address, frame);
    }
};

unsigned int virtualAddressToVPN(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
Map * lookup_vpn2pfn(PageTable *pageTable,unsigned int virtualAddress);
void insert_vpn2pfn(PageTable *pageTable, unsigned int virtualAddress, unsigned int frame){
    pageTable->insert_vpn2pfn(pageTable->rootNodePtr, virtualAddress, frame);
}

#endif //AS3_PAGETABLE_H
