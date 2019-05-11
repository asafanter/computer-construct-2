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
    DataBlock(const unsigned int &address, const State &state = State::VALID);
    bool operator == (const DataBlock &data_block);
    State getState() const {return _state;}
    unsigned int getAdress() const {return _address;}
    DataBlock &setState(const State &new_State) {_state = new_State;}
    bool isDirty() const {return _state == State::DIRTY;}
    DataBlock &markAsDirty() {_state = State::DIRTY; return *this;}
    DataBlock &markAsValid() {_state = State::VALID; return *this;}

private: //members
    unsigned int _address;
    State _state;
};

#endif // DATABLOCK_H
