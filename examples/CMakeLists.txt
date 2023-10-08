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

#生成ts文件的文件夹
set(TS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/translations")

set(TS_FILES
    "${TS_DIR}/Demo_zh_CN.ts"
    "${TS_DIR}/Demo_en.ts"
)

#模拟更新和发布的命令代码
find_program(LUPDATE_EXECUTABLE lupdate)
find_program(LRELEASE_EXECUTABLE lrelease)
find_program(LCONVERT_EXECUTABLE lconvert)

#生成ts和qm文件
foreach(_ts_file ${TS_FILES})
    execute_process(COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_CURRENT_SOURCE_DIR} -ts ${_ts_file})
endforeach()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/translations")

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

set(ALLTS_FILES
    "${CMAKE_SOURCE_DIR}/utkwidget/translations/utkwidget_zh_CN.ts"
    "${CMAKE_SOURCE_DIR}/utkwidget/translations/utkwidget_en.ts"
    "${TS_DIR}/Demo_zh_CN.ts"
    "${TS_DIR}/Demo_en.ts"
)

foreach(ts_file ${ALLTS_FILES})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${ts_file} "${CMAKE_CURRENT_BINARY_DIR}/translations")
endforeach()

FILE(GLOB ENTS
    ${CMAKE_CURRENT_BINARY_DIR}/translations/*_en.ts
)
string(JOIN " " entslist ${ENTS})
string(APPEND execonvert ${LCONVERT_EXECUTABLE})
string(APPEND execonvert " -i ${entslist} -o ${CMAKE_CURRENT_BINARY_DIR}/translations/example_en.ts")
message("list ents: ${execonvert}")

if(WIN32)
    execute_process(COMMAND powershell -Command ${execonvert})
else()
    execute_process(COMMAND bash -c ${execonvert})
endif()

FILE(GLOB ZHCNTS
    ${CMAKE_CURRENT_BINARY_DIR}/translations/*_zh_CN.ts
)
string(JOIN " " zhcnlist ${ZHCNTS})
string(APPEND exezhconvert ${LCONVERT_EXECUTABLE})
string(APPEND exezhconvert " -i ${zhcnlist} -o ${CMAKE_CURRENT_BINARY_DIR}/translations/example_zh_CN.ts")
message("list ents: ${exezhconvert}")

if(WIN32)
    execute_process(COMMAND powershell -Command ${exezhconvert})
else()
    execute_process(COMMAND bash -c ${exezhconvert})
endif()

set(EXAMPLE_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/translations/example_en.ts"
    "${CMAKE_CURRENT_BINARY_DIR}/translations/example_zh_CN.ts"
)

foreach(_ts_file ${EXAMPLE_FILES})
    add_custom_command(TARGET Demo PRE_BUILD COMMAND ${LRELEASE_EXECUTABLE} ${_ts_file})
endforeach()
