# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 2.8.7)

project ("redis_demo")

# 包含子项目。
add_subdirectory ("redis_demo")
