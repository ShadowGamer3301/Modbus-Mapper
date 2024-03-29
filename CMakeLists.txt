#Require cmake version 3.12 or higher
cmake_minimum_required(VERSION 3.12)

#Require C++ 20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#Generate compile flags to make clangd happy
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#Create project
project(MbMapper VERSION 1.0)

#Look for all C++ files in src folder
file(GLOB MbSrc "src/*.cxx" "src/*.cpp" "src/*.cc")

#Create executable
add_executable(MbMapper ${MbSrc})

#Set linking for Linux
if(UNIX)
  target_link_libraries(MbMapper PRIVATE "libmodbus.so" "libglfw.so" "libGL.so")

  #Link libmodbus for Arch Linux (I use Arch BTW)
  target_link_directories(MbMapper PRIVATE "/usr/local/lib64")

  #Link libmodbus for OpenSUSE Linux
  target_link_directories(MbMapper PRIVATE "/usr/local/lib")
endif()
