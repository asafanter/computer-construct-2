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
    _access_counter(0),
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
    _access_counter++;

    if(_L1.read(address))
    {
        return *this;
    }

    if(_L2.read(address))
    {
        bringDataToL1(_L2.findDataBlock(address));
        return *this;
    }

    if(_has_victim_cashe)
    {
        if(_victim_cache->read(address))
        {
            bringDataToL2(_victim_cache->findDataBlock(address).get());
            bringDataToL1(_L2.findDataBlock(address));
            _victim_cache->remove(address);
            return *this;
        }

        bringDataFromMemory(address);
    }
    else
    {
        bringDataFromMemory(address);
    }

    return *this;
}

CacheSimulator &CacheSimulator::write(const unsigned int &address)
{
    _access_counter++;

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
    else
    {
        _memory.access();
       if(_write_policy == WritePolicy::ALLOCATE)
       {
           DataBlock data_block(address);
           bringDataToL2(data_block);
           bringDataToL1(_L2.findDataBlock(address));
       }
    }

    return *this;
}

double CacheSimulator::getAvgTime() const
{
    uint tot_time = _L1.getTotalTime() + _L2.getTotalTime() + _memory.getTotalTime();

    if(_has_victim_cashe)
    {
        tot_time += _victim_cache->getTotalTime();
    }


    return static_cast<double>(tot_time) / static_cast<double>(_access_counter);
}

bool CacheSimulator::tryReadVictimCache(const uint &address)
{
    if(_victim_cache->read(address))
    {       
        auto data_block = _victim_cache->findDataBlock(address).get();

        bringDataToL2(data_block).bringDataToL1(data_block);
        return true;
    }

    DataBlock data_block(address);

    bringDataToVictimCache(data_block).bringDataToL2(data_block).bringDataToL1(data_block);

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
            bringDataToL1(_L2.findDataBlock(address));
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
            auto data_block = _victim_cache->findDataBlock(address).get();
            bringDataToL2(data_block).bringDataToL1(data_block);
        }
        return true;
    }

    if(_write_policy == WritePolicy::ALLOCATE)
    {
        DataBlock data_block(address);
        bringDataToVictimCache(data_block).bringDataToL2(data_block).bringDataToL1(data_block);
    }

    return false;
}

CacheSimulator &CacheSimulator::bringDataToL1(const DataBlock &data_block)
{
    auto address = data_block.getAdress();
    Entry &entry = _L1.getEntryForNewData(address);

    if(entry.getDataBlock().isDirty())
    {
        auto to_update = _L2.findEntry(address);
        _L2.markAsDirty(to_update->getDataBlock());
        _L2.incrementTime();
        to_update->resetTime();
    }

    entry = _L1.makeEntry(address);

    if(data_block.isDirty())
    {
        entry.getDataBlock().setState(DataBlock::State::DIRTY);
        _L2.findDataBlock(data_block.getAdress()).setState(DataBlock::State::VALID);
    }

    return *this;
}

CacheSimulator &CacheSimulator::bringDataToL2(const DataBlock &data_block)
{
    auto address = data_block.getAdress();
    Entry &entry = _L2.getEntryForNewData(address);

    if(entry.getDataBlock().isDirty())
    {
        if(_has_victim_cashe)
        {
            _victim_cache->findDataBlock(address)->setState(DataBlock::State::DIRTY);
        }
    }

    entry = _L2.makeEntry(address);

    if(data_block.isDirty())
    {
        entry.getDataBlock().setState(DataBlock::State::DIRTY);
        _victim_cache->findDataBlock(address)->setState(DataBlock::State::VALID);
    }

    return *this;
}

CacheSimulator &CacheSimulator::bringDataToVictimCache(const DataBlock &data_block)
{
    _victim_cache->insert(data_block.getAdress());

    return *this;
}

CacheSimulator &CacheSimulator::bringDataFromMemory(const uint &address)
{
    _memory.access();
    DataBlock data_block(address);

    if(_has_victim_cashe)
    {     
        bringDataToVictimCache(data_block);
        bringDataToL2(_victim_cache->findDataBlock(address).get());
        bringDataToL1(_L2.findDataBlock(address));
    }
    else
    {
        bringDataToL2(data_block);
        bringDataToL1(_L2.findDataBlock(address));
    }


    return *this;
}
