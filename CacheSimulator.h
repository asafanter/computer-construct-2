#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H

#include <memory>
#include <vector>
#include <string>
#include "Cache.h"
#include "VictimCache.h"
#include "Memory.h"

using uint = unsigned int;
using string = std::string;

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
    double getAvgTime() const;
    double getL1MissRate() const {return _L1.getMissRate();}
    double getL2MissRate() const {return _L2.getMissRate();}

private:
    bool tryWriteL2(const uint &address);
    bool tryWriteVictimCache(const uint &address);
//    CacheSimulator &bringDataToL1(const DataBlock &data_block);
    CacheSimulator &bringDataToL1(const uint &address, const DataBlock::State &state);
//    CacheSimulator &bringDataToL2(const DataBlock &data_block);
    CacheSimulator &bringDataToL2(const uint &address, const DataBlock::State &state);
    CacheSimulator &bringDataFromMemory(const uint &address);

private: //members
    uint _num_of_caches;
    uint _mem_access_time_cycles;
    bool _has_victim_cashe;
    uint _access_counter;
    WritePolicy _write_policy;
    Cache _L1;
    Cache _L2;
    std::shared_ptr<VictimCache> _victim_cache;
    Memory _memory;
};

#endif // CACHESIMULATOR_H
