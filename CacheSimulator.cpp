#include "CacheSimulator.h"

CacheSimulator::CacheSimulator(const uint &num_of_caches,
                               const uint &block_size_bytes_log2,
                               const uint &mem_access_time_cycles,
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
    _L2(L2_size_bytes_log2, block_size_bytes_log2, L2_num_of_ways_log2, L2_access_time_cycles),
    _victim_cache(nullptr),
    _memory(mem_access_time_cycles)
{
    if(_has_victim_cashe)
    {
        _victim_cache = std::make_shared<VictimCache>(block_size_bytes_log2);
    }
}

CacheSimulator &CacheSimulator::read(const unsigned int &address)
{
    if(_L1.read(address))
    {
        return *this;
    }

    if(_L2.read(address))
    {
        bringDataToL1(address);
        return *this;
    }

    if(_has_victim_cashe)
    {
        if(_victim_cache->read(address))
        {
            bringDataToL2(address).bringDataToL1(address);
            return *this;
        }

        bringDataFromMemory(address);
    }
    else
    {
        bringDataFromMemory(address);
    }

    return *this;
//    if(tryReadL1(address))
//    {
//        return *this;
//    }

//    if(tryReadL2(address))
//    {
//        return *this;
//    }

//    if(_has_victim_cashe)
//    {
//        if(tryReadVictimCache(address))
//        {
//            return *this;
//        }
//    }
//    else
//    {
//        bringDataFromMemory(address);
//    }

//    return *this;
}

CacheSimulator &CacheSimulator::write(const unsigned int &address)
{
    if(tryWriteL1(address))
    {
        return *this;
    }

    if(tryWriteL2(address))
    {
        return *this;
    }

    if(_has_victim_cashe)
    {
        tryWriteVictimCache(address);
    }

    bringDataToL2(address).bringDataToL1(address);

    return *this;
}

uint CacheSimulator::getAvgTime()
{

}

bool CacheSimulator::tryReadL1(const uint &address)
{
    if(_L1.read(address))
    {
        return true;
    }

    return false;
}

bool CacheSimulator::tryReadL2(const uint &address)
{
    if(_L2.read(address))
    {
        bringDataToL1(address);
        return true;
    }

    return false;
}

bool CacheSimulator::tryReadVictimCache(const uint &address)
{
    if(_victim_cache->read(address))
    {
        bringDataToL2(address).bringDataToL1(address);
        return true;
    }

    bringDataToVictimCache(address).bringDataToL2(address).bringDataToL1(address);

    return false;
}

bool CacheSimulator::tryWriteL1(const uint &address)
{
    if(_L1.write(address))
    {
        return true;
    }

    return false;
}

bool CacheSimulator::tryWriteL2(const uint &address)
{
    if(_L2.write(address))
    {
        if(_write_policy == WritePolicy::ALLOCATE)
        {
            bringDataToL1(address);
        }

        return true;
    }

    return false;
}

bool CacheSimulator::tryWriteVictimCache(const uint &address)
{
    if(_victim_cache->write(address))
    {
        if(_write_policy == WritePolicy::ALLOCATE)
        {
            bringDataToL2(address).bringDataToL1(address);
        }
        return true;
    }

    if(_write_policy == WritePolicy::ALLOCATE)
    {
        bringDataToVictimCache(address).bringDataToL2(address).bringDataToL1(address);
    }

    return false;
}

CacheSimulator &CacheSimulator::bringDataToL1(const uint &address)
{
    Entry &entry = _L1.getEntryForNewData(address);
    if(entry.getDataBlock().isDirty())
    {
        auto to_update = _L2.findEntry(address);
        _L2.markAsDirty(to_update->getDataBlock());
        to_update->resetTime();
    }

    entry = _L1.makeEntry(address);

    return *this;
}

CacheSimulator &CacheSimulator::bringDataToL2(const uint &address)
{
    Entry &entry = _L2.getEntryForNewData(address);
    if(entry.getDataBlock().isDirty())
    {
        if(_has_victim_cashe)
        {
            _victim_cache->findDataBlock(address)->setState(DataBlock::State::DIRTY);
        }
    }

    entry = _L2.makeEntry(address);

    return *this;
}

CacheSimulator &CacheSimulator::bringDataToVictimCache(const uint &adress)
{
    _victim_cache->insert(adress);

    return *this;
}

CacheSimulator &CacheSimulator::bringDataFromMemory(const uint &address)
{
    _memory.access();
    if(_has_victim_cashe)
    {
        bringDataToVictimCache(address);
    }
    else
    {
        bringDataToL2(address).bringDataToL1(address);
    }

    return *this;
}
