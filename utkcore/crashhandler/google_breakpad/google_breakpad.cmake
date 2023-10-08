# Source files.
SET(SOURCES
    ${SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/common/string_conversion.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/convert_UTF.c
        ${CMAKE_CURRENT_LIST_DIR}/common/md5.cc
)
#IF(UNIX)
#	SET(SOURCES
#		${SOURCES}
#		${BREAKPAD_SOURCE_DIR}/src/client/minidump_file_writer.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/log/log.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/handler/exception_handler.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/handler/minidump_descriptor.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/guid_creator.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/dump_writer_common/thread_info.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/dump_writer_common/ucontext_reader.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/linux_dumper.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/minidump_writer.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/microdump_writer/microdump_writer.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/file_id.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/elfutils.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/linux_libc_support.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/memory_mapped_file.cc
#		${BREAKPAD_SOURCE_DIR}/src/common/linux/safe_readlink.cc
#		${BREAKPAD_SOURCE_DIR}/src/client/linux/crash_generation/crash_generation_client.cc
#    )
#ENDIF()

IF(WIN32)
        SET(SOURCES
                ${SOURCES}
                ${CMAKE_CURRENT_LIST_DIR}/common/windows/guid_string.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/windows/handler/exception_handler.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/windows/crash_generation/minidump_generator.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/windows/crash_generation/client_info.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/windows/crash_generation/crash_generation_client.cc
    )
ENDIF()

IF(APPLE)
        SET(SOURCES
                ${SOURCES}
                ${CMAKE_CURRENT_LIST_DIR}/client/minidump_file_writer.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/crash_generation/crash_generation_client.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/handler/exception_handler.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/handler/minidump_generator.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/handler/breakpad_nlist_64.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/handler/dynamic_images.cc
                ${CMAKE_CURRENT_LIST_DIR}/client/mac/handler/protected_memory_allocator.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/bootstrap_compat.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/file_id.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/macho_id.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/macho_reader.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/macho_utilities.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/macho_walker.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/string_utilities.cc
                ${CMAKE_CURRENT_LIST_DIR}/common/mac/MachIPC.mm
    )
ENDIF()

SET(BREAKPAD_RESOURCES
    ${SOURCES}
)

