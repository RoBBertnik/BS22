cmake_minimum_required(VERSION 3.7.2)
project(Test3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Test3 main.c sub.c sub.h KeyValueStore.c KeyValueStore.h KeyAndValue.h)
