#ifndef ENTRY_H
#define ENTRY_H

#include "DataBlock.h"

class Entry
{
public:
    Entry();
    Entry(const unsigned int &tag, const unsigned int &set, const DataBlock &data_block);
    unsigned int getTag() const {return _tag;}
    unsigned int getSet() const {return _set;}
    unsigned int getTime() const {return _time;}
    DataBlock &getDataBlock() {return _data_block;}
    const DataBlock &getDataBlock() const {return _data_block;}
    Entry &incrementTime() {_time++;}
    Entry &setDataBlock(const DataBlock &new_data_block) {_data_block = new_data_block;}
    Entry &removeDataBlock() {_data_block.setState(DataBlock::State::INVALID);}
    Entry &resetTime() {_time = 0;}

private:
    unsigned int _tag;
    unsigned int _set;
    DataBlock _data_block;
    unsigned int _time;
};

#endif // ENTRY_H
