#include <cmath>
#include <string>
#include "VictimCache.h"


VictimCache::VictimCache(const uint &block_size_bytes_log2, const uint &num_of_entries,
                         const uint &access_time_cycles) :
    _block_size_bytes_log2(block_size_bytes_log2),
    _num_of_entries(num_of_entries),
    _access_time_cycles(access_time_cycles),
    _entries(num_of_entries)
{

}

bool VictimCache::isContains(const uint &address)
{
    auto tag = calcTag(address);

    for(uint i = 0; i < _num_of_entries; i++)
    {
        if(_entries[i].first == tag && _entries[i].second.getState() != DataBlock::State::INVALID)
        {
            return true;
        }
    }

    return false;
}

VictimCache &VictimCache::insert(const uint &address)
{
    auto tag = calcTag(address);
    auto data_block = DataBlock(address);

    _entries.push_front(std::pair<uint, DataBlock>(tag, data_block));
    if(_entries.size() >= _num_of_entries)
    {
        _entries.pop_back();
    }

    return *this;
}

uint VictimCache::calcTag(const uint &address)
{
    return address >> _block_size_bytes_log2;
}

