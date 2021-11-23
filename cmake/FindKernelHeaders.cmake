# Find the kernel release
execute_process(
        COMMAND uname -r
        OUTPUT_VARIABLE KERNEL_RELEASE
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
        COMMAND find /usr/src -type d -name *${KERNEL_RELEASE}*
        OUTPUT_VARIABLE KERNELHEADERS_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

#set(KERNELHEADERS_DIR /usr/src/linux-headers-${KERNEL_RELEASE})

message(STATUS "Kernel release: ${KERNEL_RELEASE}")
message(STATUS "Kernel headers: ${KERNELHEADERS_DIR}")

if (KERNELHEADERS_DIR)
    set(KERNELHEADERS_INCLUDE_DIRS
            ${KERNELHEADERS_DIR}/include
            ${KERNELHEADERS_DIR}/arch/x86/include)
endif (KERNELHEADERS_DIR)
message(STATUS "PATH: ${KERNELHEADERS_INCLUDE_DIRS}")
