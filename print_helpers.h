#ifndef PRINT_HELPERS_H
#define PRINT_HELPERS_H

/*
 * Compilation notes:
 * C compilers:
 *    uses bool, must compile with -std=c99 or later (bool was introduced
 *    in the 1999 C standard.
 *
 * C++ compilers
 *    uses uint32_t, unsigned 32 bit integer type, introduced in C++11,
 *    The defaults in the g++ compiler on edoras should be fine with this
 */

/* C and C++ define some of their types in different places.
 * Check and see if we are using C or C++ and include appropriately
 * so that this will compile under C and C++
 */
#ifdef __cplusplus
/* C++ includes */
#include <stdint.h>
#else
/* C includes */
#include <inttypes.h>
#include <stdbool.h>
#endif 

/*
 * structure that can be used to maintain which output types are enabled.
 * Note that this uses the bool keyword. 
 *
 * If compiled with a C compiler, make sure that the C99 dialect or later is used.
 * (-std=c99 with a GNU C compiler)
 */
typedef struct {
  bool levelbitmasks;  /* display bitmaks */
  bool va2pa;  /* show virtual to physical address translation */
  bool va2pa_tlb_ptwalk; /* show virtual to physical address translation 
                            using TLB then Pagetable walk (if TLB miss)*/
  bool vpn2pfn;  /* show mapping between page numbers and frame */
  bool offset; /* show the offset of each address */
  bool summary; /* summary statistics */
} OutputOptionsType;

/**
 * @brief Print out a number in hex, one per line
 * @param number 
 */
void hexnum(uint32_t number);

/**
 * @brief Print out bitmasks for all page table levels.
 * 
 * @param levels - Number of levels
 * @param masks - Pointer to array of bitmasks
 */
void report_levelbitmasks(int levels, uint32_t *masks);

/**
 * @brief Given a pair of numbers, output a line: 
 *        src -> dest  
 * Example usages:
 * Map between virtual and physical addresses:  mapping(virtual, physical)
 * Map between page number and frame: mapping(page, frame)
 * 
 * @param src 
 * @param dest 
 */
void report_virtualAddr2physicalAddr(uint32_t src, uint32_t dest);

/**
 * @brief report_va2pa_TLB_PTwalk(src, dest, tlbhit, pthit)
 *        Given a pair of addresses, hit or miss along the tlb 
 *        then pagetable walk, output a line: 
 *          src -> dest, tlb hit 
 *          src -> dest, tlb miss, pagetable hit
 *          src -> dest, tlb miss, pagetable miss  
 * Example usages:
 * Report translation/mapping from logical to physical address: 
 *        e.g., report_va2pa_TLB_PTwalk(logical, physical, false, true)
 *              tlb miss, pagetable hit
 * Report translation/mapping from virtual page number to physical frame: 
 *        e.g., report_va2pa_TLB_PTwalk(page, frame, false, true)
 *              tlb miss, pagetable hit
 * 
 * @param src 
 * @param dest 
 * @param tlbhit 
 * @param pthit 
 */
void report_va2pa_TLB_PTwalk(uint32_t src, uint32_t dest, bool tlbhit, bool pthit);

/**
 * @brief Write out page numbers and frame number
 * 
 * @param levels - specified number of levels in page table
 * @param pages - pages[idx] is the page number associated with 
 *	              level idx (0 < idx < levels)
 * @param frame - page is mapped to specified frame
 */
void report_pagetable_map(int levels, uint32_t *pages, uint32_t frame);

/**
 * @brief Write out a message that indicates summary information for the page table.
 * 
 * @param page_size - Number of bytes per page
 * @param cacheHits - Number of vpn->pfn mapping found in the TLB
 * @param pageTableHits - Number of times a page was mapped
 * @param addresses - Number of addresses processed
 * @param frames_used - Number of frames allocated
 * @param bytes - Total number of bytes needed for page table data structure.  
 *                Should include all levels, allocated arrays, etc.
 */
void report_summary(unsigned int page_size, 
                    unsigned int cacheHits,
                    unsigned int pageTableHits, 
                    unsigned int addresses, unsigned int frames_used,
                    unsigned int bytes);


#endif
