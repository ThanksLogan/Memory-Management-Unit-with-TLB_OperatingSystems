#include <iostream>
#include <string.h>
#include <cstdio>
#include <getopt.h>
#include <fstream>
#include "TLB.h"
#include "pagetable.h"
#include <cwchar>
extern "C" {
#include "vaddr_tracereader.h"
}

#define BADFLAG 2
#define BADPATH 3

int main(int argc, char **argv) {
    /* process first N number of addresses given in trace file */
    int processTil = -1;
    /* TLB cache capacity */
    int cacheCap = 0;
    /* print mode, int for easy switch statement */
    int printMode = 0;
    int readAddressCount = 0;
    int frameNumber = 0;
    unsigned int cacheHits = 0;
    unsigned int pageHits = 0;
    unsigned int addresses = 0;
    tlbCache *cache = new tlbCache(8);


    /* opens and error checks file */
    FILE *trace = fopen(argv[1], "r");
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
                }else if(!strcmp(mode, "vpn2pfn")){
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

    p2AddrTr mtrace;

    unsigned int vaddr;
    unsigned int offMask = 0x00000FFF;
    unsigned int virtualMask = 0xFFFFF000;
    unsigned int offset;
    unsigned int VPN;
    unsigned int PA;
    Map *currentMapping;
    pageTable *rootPT = new pageTable();
    unsigned int bitmasks[] = {0xFF000, 0x00F00, 0x000FF};
    unsigned int shifts[] = {16, 12, 8};
    unsigned int entryC[] = {256, 16, 256};
    unsigned int levelC = 3;
    rootPT->bitmaskAry = bitmasks;
    rootPT->shiftAry = shifts;
    rootPT->entryCount = entryC;
    rootPT->levelCount = levelC;
    rootPT->rootNodePtr = new level(0);
    rootPT->rootNodePtr->pageTablePtr = rootPT;
    if(processTil == -1) {
        while(NextAddress(trace, &mtrace)) {
            addresses++;
            frameNumber++;
            vaddr = mtrace.addr;
            offset = vaddr & offMask;
            VPN = vaddr >> 12;
            if(cache->cache.find(VPN) != cache->cache.end()){
                cache->addressTime++;
                cacheHits++;
                cache->times[VPN] = cache->addressTime;
                PA = cache->cache[VPN] + offset;
                std::cout << "PA cache: " << std::hex << PA << std::endl;
            }else{
                currentMapping = rootPT->lookup_vpn2pfn(VPN);
                if(currentMapping != NULL){
                    pageHits++;
                    cache->addEntry(VPN, currentMapping->PFN);
                    std::cout << "PA page: " << std::hex << currentMapping->PFN + offset << std::endl;
                }else{
                    rootPT->insert_vpn2pfn(VPN, frameNumber);
                    std::cout << "PA inserted: " << std::hex << frameNumber + offset << std::endl;
                }
            }
        }
    }else{
        while(NextAddress(trace, &mtrace) && readAddressCount < processTil) {
            vaddr = mtrace.addr;
            readAddressCount++;
        }
    }

}