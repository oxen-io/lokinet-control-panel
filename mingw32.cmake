set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX i686-w64-mingw32)
set(WOW64_CROSS_COMPILE ON)
set(LIBSODIUM_CROSS_TARGET i686-w64-mingw32)

set(TOOLCHAIN_PATHS
  /usr/${TOOLCHAIN_PREFIX}
  /usr/local/opt/mingw-w64/toolchain-i686
  /usr/local/opt/mingw-w64/toolchain-i686/i686-w64-mingw32
  /opt/mingw32
  /home/$ENV{USER}/mingw32
  /home/$ENV{USER}/mingw32/${TOOLCHAIN_PREFIX}
)

set(CMAKE_SYSTEM_VERSION 6.0)

# target environment on the build host system
# second one is for non-root installs
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATHS})

add_definitions("-DWINNT_CROSS_COMPILE")

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# cross compilers to use
if($ENV{COMPILER} MATCHES "clang")
    set(USING_CLANG ON)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-clang)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-clang++)
else()
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc${TOOLCHAIN_SUFFIX})
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++${TOOLCHAIN_SUFFIX})
    add_compile_options("-Wa,-mbig-obj")
endif()

set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
