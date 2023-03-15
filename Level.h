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
    Level * NextLevelPtr[NUMBEROF_NEXTLEVELS] = {};
};
#endif //AS3_LEVEL_H
