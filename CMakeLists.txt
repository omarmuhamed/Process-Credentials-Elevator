cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(Elevator VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find kernel headers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(KernelHeaders REQUIRED)
if (UNIX)
    include_directories(${KERNELHEADERS_INCLUDE_DIRS})
else ()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/include;${CMAKE_CURRENT_SOURCE_DIR}headers/arch/x86/include)
endif(UNIX)

set(DRIVER_FILE main.ko)
set(KBUILD_CMD $(MAKE) -C ${KERNELHEADERS_DIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR})

# Generate the Kbuild file through cmake.
FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "obj-m := main.o")

add_custom_command(OUTPUT ${DRIVER_FILE}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS main.c VERBATIM)

if (UNIX)
    add_custom_target(Elevator ALL DEPENDS ${DRIVER_FILE})
else ()
    add_executable(Elevator main.c)
endif(UNIX)
