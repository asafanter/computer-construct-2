cmake_minimum_required(VERSION 2.8)

project(cacheSim)

set(SOURCE_FILES
    cacheSim.cpp
    DataBlock.h
    DataBlock.cpp
    Cache.h
    Cache.cpp
    Way.h
    Way.cpp
    CacheSimulator.h
    CacheSimulator.cpp
    Entry.h
    Entry.cpp
    Exceptions.h
    VictimCache.h
    VictimCache.cpp
    Memory.h
    Ref.h
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
