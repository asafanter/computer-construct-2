#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include "Way.h"
#include "Ref.h"

using uint = unsigned int;

class Cache
{
public:
    Cache(const uint & size_bytes, const uint &block_size_bytes,
          const uint & num_of_ways,
          const uint &access_time_cycles);

    Entry &findLRU(const uint &set);
    bool read(const uint &address);
    bool write(const unsigned int &address);
    DataBlock &findDataBlock(const uint &address);
    bool isContains(const uint &address) const;
    Entry makeEntry(const uint &address);
    Entry &getEntryForNewData(const uint &address);
    Cache &markAsDirty(const DataBlock &data_block);
    uint getNumOfAccess() const {return _access_counter;}
    uint getAccessTimeCycles() const {return _access_time_cycles;}

private: //methods
    void initWays();
    uint calcTag(const uint &address) const;
    uint calcSet(const uint &address) const;
    Cache &incrementTime();
    Cache &insert(Way &way, const DataBlock &data_block);
    int findAvailalbeWay(const uint &set);
    Ref<Entry> findEntry(const uint &address);


private: //members
    uint _size_bytes;
    uint _block_size_bytes;
    uint _access_time_cycles;
    uint _num_of_ways;
    uint _hits_counter;
    uint _access_counter;
    uint _miss_counter;

    std::vector<Way> _ways;
};

#endif // CACHE_H
