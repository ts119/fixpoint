add_executable(q1.out q1.cpp Date_calc.cpp)
add_executable(q2.out q2.cpp Date_calc.cpp)
add_executable(q3.out q3.cpp Date_calc.cpp)
add_executable(q4.out q4.cpp Date_calc.cpp)

target_compile_features(q1.out PUBLIC cxx_std_17)
target_compile_features(q2.out PUBLIC cxx_std_17)
target_compile_features(q3.out PUBLIC cxx_std_17)
target_compile_features(q4.out PUBLIC cxx_std_17)