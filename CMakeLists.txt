cmake_minimum_required(VERSION 3.9)
set (CMAKE_GENERATOR_PLATFORM "x64")
project(hashmaps C CXX)

file(GLOB_RECURSE hashmaps_SRC src/*.h src/*.cpp)
include_directories(hashmaps PUBLIC include)

add_executable(hashmaps ${hashmaps_SRC})

# set_target_properties(hashmaps PROPERTIES PREFIX "")
# set_target_properties(hashmaps PROPERTIES SUFFIX ".pyd")
set(CMAKE_CXX_FLAGS "/Ox /EHsc /Zi ")