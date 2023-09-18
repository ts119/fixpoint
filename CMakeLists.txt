cmake_minimum_required(VERSION 3.16)
project(Test LANGUAGES CXX)

add_executable(q1.out q1.cpp Date_calc.cpp)

target_compile_features(q1.out PUBLIC cxx_std_17)