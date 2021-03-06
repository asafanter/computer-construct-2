#include <cmath>
#include "Cache.h"

Cache::Cache(const uint &size_bytes_log2, const uint &block_size_bytes,
             const uint &num_of_ways,
             const uint &access_time_cycles) :
    _size_bytes_log2(size_bytes_log2),
    _block_size_bytes(block_size_bytes),
    _access_time_cycles(access_time_cycles),
    _num_of_ways(static_cast<uint>(std::pow(2, num_of_ways))),
    _hits_counter(0),
    _access_counter(0),
    _miss_counter(0),
    _ways()
{
    initWays();
}

int Cache::findAvailalbeWay(const uint &set)
{
    for(unsigned int i = 0; i < _num_of_ways; i++)
    {
        if(_ways[i].isAvailable(set))
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

Ref<Entry> Cache::findEntry(const uint &address)
{
    auto tag = calcTag(address);
    auto set = calcSet(address);

    for(auto &way : _ways)
    {
        Entry &entry = way.getEntry(set);

        if(entry.getTag() == tag && entry.getDataBlock().getState() != DataBlock::State::INVALID)
        {
            return Ref<Entry>(entry);
        }
    }

    return nullptr;
}

bool Cache::isContains(const uint &address) const
{
    auto tag = calcTag(address);
    auto set = calcSet(address);

    for(auto &way : _ways)
    {
        if(way.isContains(set, tag))
        {
            return true;
        }
    }

    return false;
}

Entry Cache::makeEntry(const uint &address)
{
    auto tag = calcTag(address);
    auto set = calcSet(address);

    return Entry(tag, set, DataBlock(address));
}

Entry &Cache::getEntryForNewData(const uint &address)
{
    auto tag = calcTag(address);
    auto set = calcSet(address);
    Entry entry(tag, set, DataBlock(address));

    int available_way_id = findAvailalbeWay(set);

    if(available_way_id != -1)
    {
        return _ways[static_cast<unsigned int>(available_way_id)].getEntry(set);
    }

    return findLRU(set);
}

Cache &Cache::markAsDirty(const DataBlock &data_block)
{
    auto set = calcSet(data_block.getAdress());

    for(auto &way : _ways)
    {
        DataBlock &to_change = way.getEntry(set).getDataBlock();
        if(to_change.getAdress() == data_block.getAdress())
        {
            to_change.setState(DataBlock::State::DIRTY);
        }
    }

    return *this;
}

Cache &Cache::updateLRU(const uint &set)
{
    findLRU(set).resetTime();

    return *this;
}

Entry &Cache::findLRU(const uint &set)
{
    unsigned int way_contains_lru_id = 0;

    for(unsigned int i = 1; i < _num_of_ways; i++)
    {
        if(_ways[i].getEntry(set).getTime() > _ways[way_contains_lru_id].getEntry(set).getTime())
        {
            way_contains_lru_id = i;
        }
    }

    return _ways[way_contains_lru_id].getEntry(set);
}

bool Cache::read(const uint &address)
{
    _access_counter++;
    incrementTime();

    Ref<Entry> entry = findEntry(address);
    bool is_contains = !entry.isNull();

    if(is_contains)
    {
        _hits_counter++;
        entry->resetTime();
        return true;
    }
    else
    {
        _miss_counter++;
    }

    return is_contains;
}

bool Cache::write(const unsigned int &address)
{
    _access_counter++;
    incrementTime();

    Ref<Entry> entry = findEntry(address);
    bool is_contains = !entry.isNull();

    if(is_contains)
    {
        _hits_counter++;
        entry->getDataBlock().setState(DataBlock::State::DIRTY);
        entry->resetTime();
        return true;
    }
    else
    {
        _miss_counter++;
    }

    return is_contains;
}

DataBlock &Cache::findDataBlock(const uint &address)
{
    return findEntry(address)->getDataBlock();
}

void Cache::initWays()
{
    unsigned int way_size = static_cast<unsigned int>(std::pow(2, _size_bytes_log2 - _block_size_bytes) /
            _num_of_ways);

    for(unsigned int i = 0; i < _num_of_ways; i++)
    {
        Way way(way_size);
        _ways.push_back(way);
    }
}

uint Cache::calcTag(const uint &address) const
{
    uint entries_per_way= static_cast<unsigned int>(std::pow(2, _size_bytes_log2 - _block_size_bytes) /
                                                          _num_of_ways);
    uint entries_per_way_log2 = static_cast<uint>(std::log2(entries_per_way));

    return address >> (entries_per_way_log2 + _block_size_bytes);
}

uint Cache::calcSet(const uint &address) const
{
    auto tmp = address >> _block_size_bytes;
    uint entries_per_way = static_cast<unsigned int>(std::pow(2, _size_bytes_log2 - _block_size_bytes) /
                                                          _num_of_ways);
    uint entries_per_way_log2 = static_cast<uint>(std::log2(entries_per_way));

    unsigned int mask = static_cast<unsigned int>(std::pow(2, entries_per_way_log2)) - 1;

    return tmp & mask;
}

Cache &Cache::incrementTime()
{
    for(auto &way : _ways)
    {
        way.incrementTime();
    }

    return *this;
}
