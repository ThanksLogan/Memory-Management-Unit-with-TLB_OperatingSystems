 

#ifndef AS32_PAGETABLE_H
#define AS32_PAGETABLE_H
#include <map>
#include <vector>


class pageTable;

class Map{
public:
    /* used in an array of 2^(level) entries, as a hash map */
    /* PFN mapping from index */
    unsigned int PFN;
    Map(unsigned int PN){
        PFN = PN;
    }
};

class level{
public:
    unsigned int depth;
    /* pointer to an array of level pointers */
    level **nextLevelPtr = NULL;
    /* pointer back to the pageTablePtr to access shared data */
    pageTable *pageTablePtr;
    /* pointer to an array of map pointers */
    Map **mappings = NULL;
    level(int dep){
        depth = dep;
    }
    /* inserts address->frame mapping into pagetable, uses levels specifically */
    void insert_vpn2pfn(unsigned int address, unsigned int frame);
    /* attempts to find address->frame mapping in pagetable, using levels specifically */
    Map* lookup_vpn2pfn(unsigned int virtualAddress);
    unsigned int virtualAddressToVPN(unsigned int virtualAddress, unsigned int mask, unsigned int shift);
};
class pageTable{
public:
    /* total amount of levels in tree */
    unsigned int levelCount;
    /* byte count used by pagetable */
    unsigned int byteCount;
    /* dynamic array of shifts needed at each level of tree */
    std::vector<unsigned int> shiftAry;
    /* dynamic array of entries needed at each level */
    std::vector<unsigned int> entryCount;
    /* bitmasks needed at each level */
    std::vector<unsigned int> bitmaskAry;
    /* points to the root level pointer, depth 0 */
    level *rootNodePtr;
    /* inserts mapping to PT */
    void insert_vpn2pfn(unsigned int address, unsigned int frame);
    /* find mapping in PT */
    Map* lookup_vpn2pfn(unsigned int virtualAddress);
    /* converts VA to a VPN based on masks and shifts */
    unsigned int virtualAddressToVPN(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

};
#endif //AS32_PAGETABLE_H
