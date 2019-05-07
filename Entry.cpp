#include "Entry.h"

Entry::Entry() :
    _tag(0),
    _set(0),
    _data_block(),
    _time(0)
{

}

Entry::Entry(const unsigned int &tag, const unsigned int &set, const DataBlock &data_block) :
    _tag(tag),
    _set(set),
    _data_block(data_block),
    _time(0)
{

}
