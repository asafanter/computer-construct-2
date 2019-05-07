#include "DataBlock.h"

DataBlock::DataBlock() :
    _address(0),
    _state(DataBlock::State::INVALID)
{

}

DataBlock::DataBlock(const unsigned int &address) :
    _address(address),
    _state(DataBlock::State::VALID)
{

}

bool DataBlock::operator ==(const DataBlock &data_block)
{
    return _address == data_block._address;
}
