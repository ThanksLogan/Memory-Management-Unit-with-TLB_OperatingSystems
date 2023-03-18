#include <iostream>
#include <string.h>
#include <cstdio>
#include <getopt.h>
#include <fstream>
#include "TLB.h"
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
    unsigned int fakePFN = 0;
    unsigned int offset;
    unsigned int lookup;
    if(processTil == -1) {
        while(NextAddress(trace, &mtrace)) {
            vaddr = mtrace.addr;
            offset = vaddr & offMask;
            lookup = (vaddr & virtualMask) >> 12;
            std::cout << "offset: " << std::hex << offset << std::endl;
            std::cout << "lookup/VA: " << std::hex << lookup << std::endl;
            if (cache->cache.find(lookup) != cache->cache.end()) {
                cache->addressTime++;
                std::cout << "hit" << std::endl;
                std::cout << cache->cache[lookup] << std::endl;
                cache->times[lookup] = cache->addressTime;
            }else{
                cache->addEntry(lookup, fakePFN);
            }
            fakePFN++;
            std::cout << fakePFN << std::endl;
        }
    }else{
        while(NextAddress(trace, &mtrace) && readAddressCount < processTil) {
            vaddr = mtrace.addr;
            readAddressCount++;
        }
    }
    std::cout << cache->cache[0x1704] << std::endl;

}