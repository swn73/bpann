cmake_minimum_required(VERSION 3.1)
project(bpann)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(bpann ${SOURCE_FILES} bpann/bpann.h bpann/train_set.h comm.h image_training/image.h)