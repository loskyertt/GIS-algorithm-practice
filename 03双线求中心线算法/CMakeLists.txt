cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置默认构建类型为Debug/Release
set(CMAKE_BUILD_TYPE Debug)
project(03双线求中心线算法)
aux_source_directory(src SRC_SUB)
aux_source_directory(. SRC_CUR)
add_executable(test.out ${SRC_SUB} ${SRC_CUR})
include_directories(include)
