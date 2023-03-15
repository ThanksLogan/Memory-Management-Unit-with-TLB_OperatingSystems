//
// Created by shane on 3/15/2023.
//

#ifndef AS3_LEVEL_H
#define AS3_LEVEL_H

#define NUMBEROF_NEXTLEVELS
#include "PageTable.h"
#include <map>

class Level {
public:
    int currentDepth;
    PageTable * PageTablePtr;
    union MvL {
        Level * NextLevelPtr[NUMBEROF_NEXTLEVELS] = {};
        map<unsigned int, unsigned int> map[NUMBEROF_NEXTLEVELS] = {};
    };
    void insert_vpn2pfn(Level *levelPtr, unsigned int address, unsigned int frame){

    }
};
#endif //AS3_LEVEL_H
