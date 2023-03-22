#include <iostream>
#include <string.h>
#include <cstdio>
#include <getopt.h>
#include <fstream>
#include <vector>
#include <numeric>
#include "TLB.h"
#include "pagetable.h"
#include <cwchar>
#include <math.h>

extern "C" {
#include "vaddr_tracereader.h"
#include "print_helpers.h"
}

#define BADFLAG 2
#define BADPATH 3

#define TOTALBITS 32
int main(int argc, char **argv) {
    /* process first N number of addresses given in trace file */
    int processTil = -1;
    /* TLB cache capacity */
    int cacheCap = 4;
    /* print mode, int for easy switch statement */
    int printMode = 4;
    int readAddressCount = 0;
    int frameNumber = 0;
    unsigned int cacheHits = 0;
    unsigned int pageHits = 0;
    unsigned int addresses = 0;
    int idx;
    /* records the amount of bits in each level, based on arguments */
    std::vector<int> levelVec = {};


    /* opens and error checks file */
    FILE *trace;

    int option;
    while ( (option = getopt(argc, argv, ":n:c:p:")) != -1) {

        switch (option) {
            case 'n': {
                processTil = atoi(optarg);
                if(processTil < 0){
                    std::cout << "Number of memory accesses must be a number, greater than or equal to 0" << std::endl;
                    exit(BADFLAG);
                }
                break; }
            case 'c': {
                cacheCap = atoi(optarg);
                if(cacheCap < 0){
                    std::cout << "Cache capacity must be a number, greater than or equal to 0" << std::endl;
                    exit(BADFLAG);
                }
                break; }
            case 'p': {
                const char *mode = optarg;
                if(!strcmp(mode, "levelbitmasks")){
                    printMode = 1;
                }else if(!strcmp(mode, "va2pa")){
                    printMode = 2;
                }else if(!strcmp(mode, "va2pa_tlb_ptwalk")){
                    printMode = 3;
                }else if(!strcmp(mode, "virtualA2pfn")){
                    printMode = 4;
                }else if(!strcmp(mode, "offset")){
                    printMode = 5;
                }else if(!strcmp(mode, "summary")){
                    printMode = 0;
                }else{
                    std::cout << "Incorrect print mode argument, look at list!" << std::endl;
                    exit(BADFLAG);
                }
                break; }
            default:
                std::cout << "flag unrecognized, exiting..." << std::endl;
                exit(BADFLAG);
        }
    }
    /* sets index to argument after switches */
    idx = optind;
    /* reads in trace file */
    trace = fopen(argv[idx], "r");
    idx++;
    while(idx < argc){
        /* pushes bit counts to vector */
        levelVec.push_back(atoi(argv[idx]));
        idx++;
    }
    std::vector<unsigned int> bitmasks;
    std::vector<unsigned int> shifts;
    std::vector<unsigned int> entries;
    /* test vector */
    levelVec = {4, 6, 8};
    /* sums up vector for use in shifting, (32-sum) is length of offset */
    int sum = std::accumulate(levelVec.begin(), levelVec.end(), 0);
    int off = TOTALBITS-sum;
    for(int i = 0; i < levelVec.size(); i++){
        /* takes sum and subtracts current level for shifting for bit masks */
        sum -= levelVec[i];
        /* shifts 2^levelVec[i]-1 to the left by remaining sum+off to get mask, FF -> FF000 for level 1 */
        bitmasks.push_back(int(pow(2,levelVec[i]))-1 << (sum+off));
        /* adds remaining sum+off to shifts, to shift back when traversing page table */
        shifts.push_back(sum+off);
        /* adds 2^(level[i]) for allocation of data structure at each level */
        entries.push_back(int(pow(2, levelVec[i])));
    }
    /* creates a new TLB cache given capacity */
    tlbCache *cache = new tlbCache(cacheCap);

    p2AddrTr mtrace;

    /* stores entire address given from trace reader */
    unsigned int vaddr;
    /*makes offset mask, 2^(length of offset)-1 example: length 3 FFF */
    unsigned int offMask = int(pow(2, off))-1;
    /*makes vta masks, 2^(length of VA) shifted to the left by offset */
    unsigned int virtualMask = (int(pow(2, TOTALBITS-off))-1) << off;

    unsigned int offset;
    unsigned int virtualA;
    unsigned int PA;
    unsigned int currentFrame;
    bool tlbhit = false;
    bool pghit = false;
    Map *currentMapping;
    pageTable *rootPT = new pageTable();
    /* level count is the length of the bit arguments */
    unsigned int levelC = levelVec.size();
    rootPT->bitmaskAry = bitmasks;
    rootPT->shiftAry = shifts;
    rootPT->entryCount = entries;
    rootPT->levelCount = levelC;
    /*initializes the rootNodePtr to a level with depth 0 */
    rootPT->rootNodePtr = new level(0);
    /* sets the level's pgtpointer back to rootPT */
    rootPT->rootNodePtr->pageTablePtr = rootPT;
    if(printMode == 1) {
        unsigned int *masks = &bitmasks[0];
        report_levelbitmasks(levelC, masks);
    }
    /* processTil not triggered, process till the end of file */
    if(processTil == -1) {
        while(NextAddress(trace, &mtrace)) {
            addresses++;
            vaddr = mtrace.addr;
            offset = vaddr & offMask;
            virtualA = (vaddr & virtualMask);
            /* checks if virtualA in cache's map */
            if(cache->cache.find(virtualA) != cache->cache.end()){
                /* new hit, increment addressTime to track */
                cache->addressTime++;
                cacheHits++;
                /*update entry's time */
                cache->times[virtualA] = cache->addressTime;
                tlbhit = true;
                /* find PA based on cache's return */
                currentFrame = cache->cache[virtualA];
                PA = (cache->cache[virtualA] << off) | offset;
            }else{
                currentMapping = rootPT->lookup_vpn2pfn(virtualA);
                /* if found in PG */
                if(currentMapping != NULL) {
                    pageHits++;
                    pghit = true;
                    /* add to cache */
                    if (cacheCap > 0) {
                        cache->addEntry(virtualA, currentMapping->PFN);
                    }
                    currentFrame = currentMapping->PFN;
                    PA = (currentMapping->PFN << off) | offset;
                }else{
                    /* not found anywhere, insert into page table */
                    rootPT->insert_vpn2pfn(virtualA, frameNumber);
                    cache->addEntry(virtualA, frameNumber);
                    currentFrame = frameNumber;
                    PA = (frameNumber << off) | offset;
                    /* new frame needed */
                    frameNumber++;
                }
            }
            switch(printMode){
                case 2:
                    report_virtualAddr2physicalAddr(vaddr, PA);
                    break;
                case 3:
                    report_va2pa_TLB_PTwalk(vaddr, PA, tlbhit, pghit);
                    break;
                case 4: {
                    std::vector<unsigned int> pages;
                    for (int i = 0; i < levelVec.size(); i++) {
                        pages.push_back((virtualA & bitmasks[i]) >> shifts[i]);
                    }
                    unsigned int *passedP = &pages[0];
                    report_pagetable_map(levelC, passedP, currentFrame);
                    break;
                }
                default:
                    break;
            }
            tlbhit = false;
            pghit = false;
        }
    }else{
        while(NextAddress(trace, &mtrace) && readAddressCount < processTil) {
            vaddr = mtrace.addr;
            readAddressCount++;
        }
    }

}