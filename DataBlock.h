#ifndef DATABLOCK_H
#define DATABLOCK_H

class DataBlock
{
public:

    enum class State
    {
        VALID,
        INVALID,
        DIRTY
    };

    DataBlock();
    explicit DataBlock(const unsigned int &address);
    bool operator == (const DataBlock &data_block);
    State getState() const {return _state;}
    unsigned int getAdress() const {return _address;}
    DataBlock &setState(const State &new_State) {_state = new_State;}
    bool isDirty() const {return _state == State::DIRTY;}

private: //members
    unsigned int _address;
    State _state;
};

#endif // DATABLOCK_H
