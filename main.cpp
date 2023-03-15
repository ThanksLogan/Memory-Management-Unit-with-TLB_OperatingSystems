#include <iostream>
#include <string.h>
#include <getopt.h>
#include <fstream>

#define BADFLAG 2
#define BADPATH 3

void

int main(int argc, char **argv) {
    /* process first N number of addresses given in trace file */
    int processTil;
    /* TLB cache capacity */
    int cacheCap = 0;
    /* print mode, int for easy switch statement */
    int printMode = 0;

    /* opens and error checks file */
    std::ifstream trace(argv[1]);
    if(trace.fail()){
        std::cout << "unable to open<<" << argv[1] << std::endl;
        exit(BADPATH);
    }
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

    
}
