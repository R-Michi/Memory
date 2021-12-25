# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project("Memory")

# use C++ 17
set(CMAKE_CXX_STANDARD 17)

# use AVX extension for optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfma -mavx")

# add subdirectories
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/hexdump")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/live_memory")

# compile library of files in src/memory directory to be reused
# can also be used as external static library
add_library(memory_lib STATIC   "src/memory/buffer.cpp"
                                "src/memory/command_decoder.cpp"
                                "src/memory/command_input.cpp"
                                "src/memory/process_handler.cpp"
                                "src/memory/table.cpp"
                                "src/memory/hexdump.cpp"
                                "src/memory/utility.cpp"
                                "src/memory/command_str_list.cpp"
                                "src/memory/process.cpp")

# compile and link executable
add_executable(memory   "main.cpp"
                        "src/application/config.cpp"
                        "src/application/app_commands.cpp"
                        "src/application/app_util.cpp")

# link libraries to executable
target_link_libraries(memory PRIVATE "memory_lib")

# export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
