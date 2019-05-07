#include "CacheSimulator.h"

CacheSimulator::CacheSimulator(const uint &num_of_caches,
                               const uint &block_size_bytes_log2,
                               const uint& mem_access_time_cycles,
                               const uint &L1_size_bytes_log2,
                               const uint &L1_num_of_ways_log2,
                               const uint &L1_access_time_cycles,
                               const uint &L2_size_bytes_log2,
                               const uint &L2_num_of_ways_log2,
                               const uint &L2_access_time_cycles,
                               const bool &has_victim_cache,
                               const WritePolicy &write_policy) :
    _num_of_caches(num_of_caches),
    _mem_access_time_cycles(mem_access_time_cycles),
    _has_victim_cashe(has_victim_cache),
    _write_policy(write_policy),
    _L1(L1_size_bytes_log2, block_size_bytes_log2, L1_num_of_ways_log2, L1_access_time_cycles),
    _L2(L2_size_bytes_log2, block_size_bytes_log2, L2_num_of_ways_log2, L2_access_time_cycles)
{

}

CacheSimulator &CacheSimulator::read(const unsigned int &address)
{
    if(tryReadL1(address))
    {
        return *this;
    }

    tryReadL2(address);

    return *this;
}

CacheSimulator &CacheSimulator::write(const unsigned int &address)
{
//    for(auto &cache : _caches)
//    {
//        if(cache.write(address))
//        {
//            return *this;
//        }
//    }

    //    return *this;
}

bool CacheSimulator::tryReadL1(const uint &address)
{
    if(_L1.read(address))
    {
        return true;
    }

    bringDataToL1(address);

    return false;
}

bool CacheSimulator::tryReadL2(const uint &address)
{
    if(_L2.read(address))
    {
        return true;
    }

    bringDataToL2(address);

    return false;
}

CacheSimulator &CacheSimulator::bringDataToL1(const uint &address)
{
    Entry &entry = _L1.getEntryForNewData(address);
    if(entry.getDataBlock().isDirty())
    {
        _L2.markAsDirty(entry.getDataBlock());
    }

    entry = _L1.makeEntry(address);

    return *this;
}

CacheSimulator &CacheSimulator::bringDataToL2(const uint &address)
{
    Entry &entry = _L2.getEntryForNewData(address);
    if(entry.getDataBlock().isDirty())
    {
        //victim mark as dirty
    }

    entry = _L2.makeEntry(address);

    return *this;
}