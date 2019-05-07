#ifndef WAY_H
#define WAY_H

#include <vector>
#include "Entry.h"

class Way
{
public:
    Way(const unsigned int &num_of_entries);
    Way &insert(const Entry &entry);
    bool isAvailable(const unsigned int &set);
    Way &remove(const unsigned int &set);
    Way &incrementTime();
    bool isContains(const unsigned int &set, const unsigned int &tag) const;
    Entry &getEntry(const unsigned int &set);
    const Entry &getEntry(const unsigned int &set) const;

private: //methods
    void initEntries();

private: //members
    unsigned int _num_of_entries;
    std::vector<Entry> _entries;
};

#endif // WAY_H
