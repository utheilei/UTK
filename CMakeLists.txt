cmake_minimum_required(VERSION 3.10)
project(utkwidget)

set(PROJECT_NAME "UtkWidget")
set(PROJECT_DESCRIPTION "Union Tool Kit for Qt applications")
set(PROJECT_AUTHOR "uthelei")
set(PROJECT_URL "https://gitee.com/uthelei/utkwidget")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# Build a shared library by default
option(BUILD_SHARED_LIBS "Build UtkWidget as a shared library" ON)
if(BUILD_SHARED_LIBS)
    add_subdirectory(src)
endif()

set(CMAKE_AUTOMOC ON)

option(BUILD_DOC "Build Doxygen documentation" OFF)
if(BUILD_DOC)
    find_package(Doxygen REQUIRED)
    add_subdirectory(doc)
endif()

option(BUILD_EXAMPLES "Build the example applications" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
