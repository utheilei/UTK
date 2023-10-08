INCLUDE(${CMAKE_CURRENT_LIST_DIR}/google_breakpad/google_breakpad.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/google_breakpad)

SET(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/crashhandler.cpp
)

SET(HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/crashhandler.h
)

SET(RESOURCES
    ${HEADERS}
    ${SOURCES}
    ${BREAKPAD_RESOURCES}
)

SET(OTHER_FILES
    ${OTHER_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/crashhandler.cmake
)
