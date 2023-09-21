cmake_minimum_required(VERSION 3.10)

project(utkwidget)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_PREFIX_PATH /Users/linwei/qt/5.11.1/clang_64/lib/cmake)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

if (WIN32)
    set(CMAKE_CXX_FLAGS "-mwindows")
endif()

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Svg CONFIG REQUIRED)
find_package(Qt5Multimedia CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})

FILE(GLOB HEADERS
    ${CMAKE_SOURCE_DIR}/*.h
    ${CMAKE_SOURCE_DIR}/widgets/*.h
    ${CMAKE_SOURCE_DIR}/style/*.h
    ${CMAKE_SOURCE_DIR}/effect/*.h
    ${CMAKE_SOURCE_DIR}/singleapplication/*.h
    ${CMAKE_SOURCE_DIR}/logger/*.h
)

FILE(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/widgets/*.cpp
    ${CMAKE_SOURCE_DIR}/style/*.cpp
    ${CMAKE_SOURCE_DIR}/effect/*.cpp
    ${CMAKE_SOURCE_DIR}/singleapplication/*.cpp
    ${CMAKE_SOURCE_DIR}/logger/*.cpp
)


qt5_add_resources(qrc_FILES ${CMAKE_SOURCE_DIR}/resouces.qrc)

set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
)

add_executable(utkwidget ${PROJECT_SOURCES} ${qrc_FILES})
target_link_libraries(utkwidget PUBLIC
        Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Multimedia Qt5::Network PRIVATE
        Qt5::GuiPrivate
        Qt5::WidgetsPrivate
)
