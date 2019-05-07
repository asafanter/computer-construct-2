#include <iostream>
#include "CacheSimulator.h"
#include "VictimCache.h"




int main()
{
//    CacheSimulator c(2, 2, 100, 3, 1, 30, 3, 2, 30, false, CacheSimulator::WritePolicy::ALLOCATE);

//    c.read(4);
//    c.read(20);
//    c.read(52);
//    c.read(21);

//    c.read(5);
//    c.read(7);
//    c.read(1);
//    c.read(4);
//    c.read(36);
//    c.read(8);
//    c.read(100);
//    c.read(6);
//    c.read(4);

    VictimCache c(2);
    c.insert(1);
    c.insert(2);
    c.insert(3);
    c.insert(4);
    c.insert(5);
    c.insert(6);
    c.insert(7);

    return 0;
}
