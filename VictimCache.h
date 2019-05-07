#ifndef VICTIMCACHE_H
#define VICTIMCACHE_H

#include <deque>
#include "DataBlock.h"
#include "Ref.h"

using uint = unsigned int;


class VictimCache
{
public:
    VictimCache();
    VictimCache(const uint &block_size_bytes_log2, const uint &num_of_entries = 4,
                const uint &access_time_cycles = 1);

    bool isContains(const uint &address);
    uint getAccessTimeCycles() const {return _access_time_cycles;}
    VictimCache &insert(const uint &address);
    bool read(const uint &address);
    bool write(const uint &address);
    Ref<DataBlock> findDataBlock(const uint &address);

private: //methods
    uint calcTag(const uint &address);

private: //members
    uint _block_size_bytes_log2;
    uint _num_of_entries;
    uint _access_time_cycles;
    uint _hits_counter;
    uint _access_counter;
    uint _miss_counter;
    std::deque<std::pair<uint, DataBlock>> _entries;
};

#endif // VICTIMCACHE_H
