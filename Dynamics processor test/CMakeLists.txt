# CMakeList.txt: Dynamics processor test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (CMakeTarget "Dynamics processor test.cpp" "Dynamics processor test.h")

add_library(audio_file STATIC "dr_wav.h","wavfile.h")
link_libraries(audio_file)