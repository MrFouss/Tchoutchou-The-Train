cmake_minimum_required(VERSION 3.5)
project(TchouTchou-The-Train)

set(SOURCE_FILES
        src/main.c
        src/manager.c
        src/network.c
        src/train.c
)

add_executable(run ${SOURCE_FILES})