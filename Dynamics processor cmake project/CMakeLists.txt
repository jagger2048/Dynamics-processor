# CMakeList.txt: Dynamics processor test 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。

aux_source_directory(. DIR_SRCS)

add_subdirectory(wavfile)
add_subdirectory(nonlinear_processor)

add_executable(noiseGateUnitTest ${DIR_SRCS})	

target_link_libraries(noiseGateUnitTest wavfileLib)
target_link_libraries(noiseGateUnitTest nonlinearProcessorLib)
