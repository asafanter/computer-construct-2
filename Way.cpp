#include "Way.h"
#include "Exceptions.h"

Way::Way(const unsigned int &num_of_entries) :
    _num_of_entries(num_of_entries),
    _entries()
{
    initEntries();
}

bool Way::isAvailable(const unsigned int &set)
{
    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    return _entries[set].getDataBlock().getState() == DataBlock::State::INVALID;
}

Way &Way::insert(const Entry &entry)
{
    auto set = entry.getSet();

    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    _entries[set] = entry;

    return *this;
}

Way &Way::remove(const unsigned int &set)
{
    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    _entries[set].removeDataBlock();
    return *this;
}

Way &Way::incrementTime()
{
    for(auto &entry : _entries)
    {
        if(entry.getDataBlock().getState() != DataBlock::State::INVALID)
        {
            entry.incrementTime();
        }
    }

    return *this;
}

bool Way::isContains(const unsigned int &set, const unsigned int &tag) const
{
    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    return _entries[set].getTag() == tag &&
            _entries[set].getDataBlock().getState() != DataBlock::State::INVALID;
}

Entry &Way::getEntry(const unsigned int &set)
{
    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    return _entries[set];
}

const Entry &Way::getEntry(const unsigned int &set) const
{
    if(set >= _num_of_entries)
    {
        throw WayInvalidSetNumber(set);
    }

    return _entries[set];
}

void Way::initEntries()
{
    for(unsigned int i = 0; i < _num_of_entries; i++)
    {
        Entry entry(0, i, DataBlock());
        _entries.push_back(entry);
    }
}
