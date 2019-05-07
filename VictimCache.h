#ifndef VICTIMCACHE_H
#define VICTIMCACHE_H

#include <deque>
#include "DataBlock.h"

using uint = unsigned int;


class VictimCache
{
public:
    VictimCache(const uint &block_size_bytes_log2, const uint &num_of_entries = 4,
                const uint &access_time_cycles = 1);

    bool isContains(const uint &address);
    uint getAccessTimeCycles() const {return _access_time_cycles;}
    VictimCache &insert(const uint &address);

private: //methods
    uint calcTag(const uint &address);

private: //members
    uint _block_size_bytes_log2;
    uint _num_of_entries;
    uint _access_time_cycles;
    std::deque<std::pair<uint, DataBlock>> _entries;
};

#endif // VICTIMCACHE_H
