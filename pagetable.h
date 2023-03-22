//
// Created by shane on 3/19/2023.
//

#ifndef AS32_PAGETABLE_H
#define AS32_PAGETABLE_H
#include <map>


class pageTable;

class Map{
public:
    unsigned int PFN;
    Map();
    Map(unsigned int PN){
        PFN = PN;
    }
};

class level{
public:
    unsigned int depth;
    level **nextLevelPtr = NULL;
    pageTable *pageTablePtr;
    Map **mappings = NULL;
    level();
    level(int dep){
        depth = dep;
    }
    void insert_vpn2pfn(unsigned int address, unsigned int frame);
    Map* lookup_vpn2pfn(unsigned int virtualAddress);

};
class pageTable{
public:
    unsigned int levelCount;
    unsigned int *shiftAry;
    unsigned int *entryCount;
    unsigned int *bitmaskAry;
    level *rootNodePtr;
    void insert_vpn2pfn(unsigned int address, unsigned int frame);
    Map* lookup_vpn2pfn(unsigned int virtualAddress);
    unsigned int virtualAddressToVPN(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

};
#endif //AS32_PAGETABLE_H
