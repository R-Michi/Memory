# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# project name
project("HexDump")

# use C++ 17
set(CMAKE_CXX_STANDARD 17)

# use AVX extension for optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx")

# library directories
link_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# compile executable
add_executable(HexDump "main.cpp")

# link libraries to executable
target_link_libraries(HexDump "-lmemory_lib")

# export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
