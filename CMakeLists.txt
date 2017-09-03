cmake_minimum_required(VERSION 3.8)
project(cpp_arsenal)
set(CMAKE_CXX_STANDARD 11)

#Customize build directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)
