#include "DataBlock.h"

DataBlock::DataBlock() :
    _address(0),
    _state(DataBlock::State::INVALID)
{

}

DataBlock::DataBlock(const unsigned int &address, const State &state) :
    _address(address),
    _state(state)
{

}

bool DataBlock::operator ==(const DataBlock &data_block)
{
    return _address == data_block._address;
}
