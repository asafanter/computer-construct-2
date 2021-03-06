#ifndef MEMORY_H
#define MEMORY_H

using uint = unsigned int;

class Memory
{
public:
    Memory(const uint &access_time_cycles) :
        _access_counter(0),
        _access_time_cycles(access_time_cycles) {}

    Memory &access() {_access_counter++; return *this;}
    uint getAccessTimeCycles() const {return _access_time_cycles;}
    uint getTotalTime() const {return _access_time_cycles * _access_counter;}
    uint getNumOfAccess() const {return _access_counter;}

private:
    uint _access_counter;
    uint _access_time_cycles;
};

#endif // MEMORY_H
