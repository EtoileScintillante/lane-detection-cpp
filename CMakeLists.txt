cmake_minimum_required(VERSION 3.5)
project(lane-detection)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# OpenCV
find_package( OpenCV REQUIRED )

# Source files
file(GLOB CPP_SOURCES src/*.cpp)

# Add executables
add_executable(video video.cpp ${CPP_SOURCES})
add_executable(image image.cpp ${CPP_SOURCES})

# Specify include dir
include_directories(src/include)

# Link libraries
target_link_libraries(video ${OpenCV_LIBS})
target_link_libraries(image ${OpenCV_LIBS})