cmake_minimum_required(VERSION 3.0.0)#minm required virsion






#project name and project version (optional)
project(GFX_lab VERSION 0.1.0)

#second 2 line for the automted testing 
#include(CTest)
#enable_testing()


#DONT BUILD DOCS
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#DONT BUILD TESTS
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#DONT BUILD EXAMPLES
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#DONT BUILD  INSTALLATION INFORMATION
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#ADD VARIABLES TO USE HIGH PERFORMANCE GRAPHCS CARD IF AVALIABLE
set(GLFW_USE_HYBRID_HPG OFF CACHE BOOL "" FORCE)


#connect custome library to a prject
add_subdirectory(vendor/glfw-3.3.8) 


#get the header file from the libarary
include_directories(
    vendor/glfw-3.3.8/include
    vendor/glad/include
    vendor/glm
)

#add exe file name and the required main file and source files in your projec
add_executable(GFX_lab main.cpp vendor/glad/src/gl.c)


target_link_libraries(GFX_lab glfw)  #to make it able it access the library throught its real name


#those following lines for packing 
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
