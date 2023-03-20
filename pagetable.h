//
// Created by shane on 3/19/2023.
//

#ifndef AS32_PAGETABLE_H
#define AS32_PAGETABLE_H
#include <map>

#define NUMBEROF_LEVELS 256
#define MAPPINGS 4096

class pageTable;

class Map{
public:
    unsigned int PFN;
    Map(unsigned int PN){
        PFN = PN;
    }
};

class level{
public:
    unsigned int depth;
    level *nextLevelPtr[NUMBEROF_LEVELS] = {};
    pageTable *pageTablePtr;
    Map *mappings[MAPPINGS] = {};
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
