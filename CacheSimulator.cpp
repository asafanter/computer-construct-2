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
        bringDataToL1(address, _L2.findDataBlock(address).getState());
        return *this;
    }

    if(_has_victim_cashe)
    {
        if(_victim_cache->read(address))
        {
            bringDataToL2(address, _victim_cache->findDataBlock(address)->getState());
            bringDataToL1(address, _L2.findDataBlock(address).getState());
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

    if(_L1.write(address))
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
           bringDataToL2(address, data_block.getState());
           bringDataToL1(address, _L2.findDataBlock(address).getState());
           _L1.findDataBlock(address).setState(DataBlock::State::DIRTY);
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

bool CacheSimulator::tryWriteL2(const uint &address)
{
    if(_L2.write(address))
    {
        if(_write_policy == WritePolicy::ALLOCATE)
        {
            bringDataToL1(address, _L2.findDataBlock(address).getState());
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
            bringDataToL2(address, data_block.getState());
            bringDataToL1(address, _L2.findDataBlock(address).getState());
            _victim_cache->remove(address);
        }
        return true;
    }
    else
    {
        _memory.access();
        if(_write_policy == WritePolicy::ALLOCATE)
        {
            DataBlock data_block(address, DataBlock::State::DIRTY);
            bringDataToL2(address, data_block.getState());
            bringDataToL1(address, _L2.findDataBlock(address).getState());
        }
    }

    return false;
}

//CacheSimulator &CacheSimulator::bringDataToL1(const DataBlock &data_block)
//{
//    auto address = data_block.getAdress();
//    Entry &entry = _L1.getEntryForNewData(address);

//    if(entry.getDataBlock().isDirty())
//    {
//        auto to_update = _L2.findEntry(entry.getDataBlock().getAdress());
//        _L2.markAsDirty(to_update->getDataBlock());
//        _L2.incrementTime();
//        to_update->resetTime();
//    }

//    entry = _L1.makeEntry(address);

//    if(data_block.isDirty())
//    {
//        entry.markAsDirty();
//        _L2.findDataBlock(entry.getDataBlock().getAdress()).markAsValid();
//    }

//    return *this;
//}

CacheSimulator &CacheSimulator::bringDataToL1(const uint &address, const DataBlock::State &state)
{
    Entry &entry = _L1.getEntryForNewData(address);

    if(entry.getDataBlock().isDirty())
    {
        auto to_update = _L2.findEntry(entry.getDataBlock().getAdress());
        _L2.markAsDirty(to_update->getDataBlock());
        _L2.incrementTime();
        to_update->resetTime();
    }

    entry = _L1.makeEntry(address);

    if(state == DataBlock::State::DIRTY)
    {
        entry.markAsDirty();
        _L2.findDataBlock(address).markAsValid();
    }

    return *this;
}

//CacheSimulator &CacheSimulator::bringDataToL2(const DataBlock &data_block)
//{
//    //find entry to insert
//    auto address = data_block.getAdress();
//    Entry &entry = _L2.getEntryForNewData(address);

//    auto e = _L1.findEntry(address);
//    if(!e.isNull())
//    {
//        e->getDataBlock().setState(DataBlock::State::INVALID);
//    }

//    //evacuate entry's data block
//    if(_has_victim_cashe && entry.getDataBlock().getState() != DataBlock::State::INVALID)
//    {
//        _victim_cache->insert(entry.getDataBlock());
//    }


//    entry = _L2.makeEntry(address);

//    if(data_block.isDirty())
//    {
//        entry.getDataBlock().markAsDirty();
//    }

//    return *this;
//}

CacheSimulator &CacheSimulator::bringDataToL2(const uint &address, const DataBlock::State &state)
{
    //find entry to insert
    Entry &entry = _L2.getEntryForNewData(address);

    //snoop
    auto e = _L1.findEntry(address);
    if(!e.isNull())
    {
        e->getDataBlock().setState(DataBlock::State::INVALID);
    }

    //evacuate entry's data block
    if(_has_victim_cashe && entry.getDataBlock().getState() != DataBlock::State::INVALID)
    {
        _victim_cache->insert(entry.getDataBlock());
    }


    entry = _L2.makeEntry(address);

    if(state == DataBlock::State::DIRTY)
    {
        entry.markAsDirty();
    }

    return *this;
}


CacheSimulator &CacheSimulator::bringDataFromMemory(const uint &address)
{
    _memory.access();
    DataBlock data_block(address);

    bringDataToL2(address, data_block.getState());
    bringDataToL1(address, _L2.findDataBlock(address).getState());

    return *this;
}
