set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_PREFIX_PATH /Users/linwei/qt/5.11.1/clang_64/lib/cmake)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_definitions(-DUTKWIDGET_LIBRARY)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Svg CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})

FILE(GLOB HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/style/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effect/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/singleapplication/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zip/*.h
)

FILE(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/style/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effect/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/singleapplication/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zip/*.cpp
)

qt5_add_resources(qrc_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resouces.qrc)

set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
)

add_library(utkwidget ${PROJECT_SOURCES} ${qrc_FILES})
target_link_libraries(utkwidget PUBLIC
        Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Network PRIVATE
        Qt5::GuiPrivate
        Qt5::WidgetsPrivate
)
