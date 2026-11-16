cmake_minimum_required(VERSION 3.24)

project(metal_compute VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT APPLE)
    message(FATAL_ERROR "Metal requires an Apple platform")
endif()

# 1. Load FetchContent module
include(FetchContent)

FetchContent_Declare(
    metal_cpp
    GIT_REPOSITORY https://github.com/bkaradzic/metal-cpp.git
    GIT_TAG        metal-cpp_27
)

# 3. Download and populate the headers at configure time
FetchContent_MakeAvailable(metal_cpp)

add_executable(metal_compute src/main.cpp)


# 4. Include the downloaded metal-cpp directories
# ${metal_cpp_SOURCE_DIR} points to the extracted location in your build folder
target_include_directories(metal_compute PRIVATE ${metal_cpp_SOURCE_DIR})

# 5. Find and link the required Apple frameworks
find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
find_library(METAL_FRAMEWORK Metal REQUIRED)
find_library(QUARTZCORE_FRAMEWORK QuartzCore REQUIRED)

target_link_libraries(metal_compute PRIVATE
    ${FOUNDATION_FRAMEWORK}
    ${METAL_FRAMEWORK}
    ${QUARTZCORE_FRAMEWORK}
)
