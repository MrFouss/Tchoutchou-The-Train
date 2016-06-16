cmake_minimum_required(VERSION 3.5)
project(TchouTchou-The-Train)

set(SOURCE_FILES
        src/main.c
        src/manager.c
        src/parser.c
        src/train.c
        src/communication.c
        src/header_master.h
)

set(GCC_COVERAGE_COMPILE_FLAGS "-W -Wall -ansi -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(GCC_COVERAGE_LINK_FLAGS "-pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(run ${SOURCE_FILES})

target_link_libraries(run rt)