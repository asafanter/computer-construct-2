#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include "Way.h"
#include "Ref.h"

using uint = unsigned int;

class Cache
{
public:
    Cache(const uint & size_bytes_log2, const uint &block_size_bytes,
          const uint & num_of_ways,
          const uint &access_time_cycles);

    Entry &findLRU(const uint &set);
    bool read(const uint &address);
    bool write(const unsigned int &address);
    DataBlock &findDataBlock(const uint &address);
    bool isContains(const uint &address) const;
    Entry makeEntry(const uint &address);
    Entry &getEntryForNewData(const uint &address);
    Cache &markAsDirty(const DataBlock &data_block);
    Cache &updateLRU(const uint &set);
    Ref<Entry> findEntry(const uint &address);
    uint getTotalTime() const {return _access_time_cycles * _access_counter;}
    uint getNumOfAccess() const {return _access_counter;}
    uint getAccessTimeCycles() const {return _access_time_cycles;}
    double getMissRate() const {return static_cast<double>(_miss_counter) / static_cast<double>(_access_counter);}
    Cache &incrementTime();

public: //methods
    void initWays();
    uint calcTag(const uint &address) const;
    uint calcSet(const uint &address) const;
    Cache &insert(Way &way, const DataBlock &data_block);
    int findAvailalbeWay(const uint &set);

private: //members
    uint _size_bytes_log2;
    uint _block_size_bytes;
    uint _access_time_cycles;
    uint _num_of_ways;
    uint _hits_counter;
    uint _access_counter;
    uint _miss_counter;

    std::vector<Way> _ways;
};

#endif // CACHE_H
