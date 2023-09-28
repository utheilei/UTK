project(Demo)
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

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_definitions(-DBUILD_TYPE_DEBUG)
endif()

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Svg CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/utkwidget)
include_directories(${CMAKE_SOURCE_DIR}/utkcore)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})

FILE(GLOB HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

FILE(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

qt5_add_resources(qrc_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resouces.qrc)

set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
)

add_executable(Demo ${PROJECT_SOURCES} ${qrc_FILES})
target_link_libraries(Demo PUBLIC
        Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Network utkwidget utkcore
)
