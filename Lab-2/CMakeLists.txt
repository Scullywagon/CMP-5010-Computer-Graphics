# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.16)

# Set the project name
project(Lab-2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages for GLEW, GLFW3, and OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# Include directories (e.g., your header files)
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Find all source files in the src/ directory
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add the executable target (replace "execute" with your desired executable name)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the required libraries (GLEW, GLFW3, OpenGL)
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw GLUT::GLUT)

