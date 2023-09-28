set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_definitions(-DUTKCORE_LIBRARY)

find_package(Qt5Core CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS})

FILE(GLOB HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

FILE(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(PROJECT_SOURCES
    ${HEADERS}
    ${SOURCES}
)

add_library(utkcore ${PROJECT_SOURCES})
target_link_libraries(utkcore PUBLIC
        Qt5::Core PRIVATE
        Qt5::CorePrivate
)
