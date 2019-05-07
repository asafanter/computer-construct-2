#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H

#include <memory>
#include <vector>
#include "Cache.h"
#include "VictimCache.h"
#include "Memory.h"

using uint = unsigned int;

class CacheSimulator
{
public:

    enum WritePolicy
    {
        ALLOCATE,
        NO_ALLOCATE
    };

    CacheSimulator(const uint &num_of_caches,
                   const uint &block_size_bytes_log2,
                   const uint &mem_access_time_cycles,
                   const uint &L1_size_bytes_log2,
                   const uint &L1_num_of_ways_log2,
                   const uint &L1_access_time_cycles,
                   const uint &L2_size_bytes_log2,
                   const uint &L2_num_of_ways_log2,
                   const uint &L2_access_time_cycles,
                   const bool &has_victim_cache,
                   const WritePolicy &write_policy);

    CacheSimulator &read(const unsigned int &address);
    CacheSimulator &write(const unsigned int &address);

private:
    bool tryReadL1(const uint &address);
    bool tryReadL2(const uint &address);
    bool tryReadVictimCache(const uint &address);
    bool tryWriteL1(const uint &address);
    bool tryWriteL2(const uint &address);
    bool tryWriteVictimCache(const uint &address);
    CacheSimulator &bringDataToL1(const uint &address);
    CacheSimulator &bringDataToL2(const uint &address);
    CacheSimulator &bringDataToVictimCache(const uint &adress);
    CacheSimulator &bringDataFromMemory(const uint &address);

private: //members
    uint _num_of_caches;
    uint _mem_access_time_cycles;
    bool _has_victim_cashe;
    WritePolicy _write_policy;
    Cache _L1;
    Cache _L2;
    std::shared_ptr<VictimCache> _victim_cache;
    Memory _memory;
};

#endif // CACHESIMULATOR_H
