cmake_minimum_required(VERSION 2.8)
project(xaxpy-test)

add_definitions(-std=c++11)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/xbyak")
add_definitions(-fno-operator-names)

add_definitions(-std=c++11)
set(SOURCE "main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/xbyak/xbyak/xbyak.h" "gemm.cpp"  "gemm.h")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_executable(xaxpy-test ${SOURCE})

#target_link_libraries(openmp_test)
