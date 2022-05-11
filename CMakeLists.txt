cmake_minimum_required (VERSION 3.5)
project (SAMPLE)


# Use C++ 17 compiler standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_BIND_GLOBAL_PLACEHOLDERS")

# include support for modern C++ language features across the build
if (WIN32 OR MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4251")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996")
    set(CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}; C:/Program Files (x86)/GnuWin32")
elseif (ARM)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os ")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS_DEBUG="-g -O0 -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os ")
    set(CMAKE_C_FLAGS_DEBUG="${CMAKE_C_FLAGS} -g -O0 ")
endif(WIN32 OR MSVC)

include_directories("${PROJECT_BINARY_DIR}")

# Find python and numpy
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED )
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_Numpy_INCLUDE_DIRS})

# Find pthread
find_library(PTHREAD_LIBRARIES pthread)

# Setup linking shared libs for Windows build
if (WIN32 OR MSVC)
	set(Boost_NO_SYSTEM_PATHS true)
	set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from Boost")
	set(Boost_USE_MULTITHREADED ON)

	add_definitions(-DBOOST_ALL_NO_LIB)
	add_definitions(-DBOOST_ALL_DYN_LINK)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif(WIN32 OR MSVC)

# we need to tell the linker to use the sysroot (although the compiler does this automatically).
if (CMAKE_SYSROOT)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_SYSROOT} -lrt -lpthread -lresolv")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_SYSROOT} -lrt -lpthread -lresolv")
endif()

# tell the installer where to put our output files. DESTDIR will be prefixed to this.
if (WIN32 OR MSVC)
    set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif(WIN32 OR MSVC)

# Find Boost
#Set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
# Set(Boost_NO_BOOST_CMAKE ON) # Enable to disable search for boost-cmake since boost-cmake wasn't used to build boost
if (WIN32 OR MSVC)
    find_package(Boost COMPONENTS unit_test_framework iostreams system serialization thread program_options python numpy filesystem REQUIRED )
elseif (arm)
    find_package(Boost 1.65.1 REQUIRED unit_test_framework iostreams system serialization thread program_options python numpy filesystem)
else ()
    find_package(Boost paths /usr/local COMPONENTS unit_test_framework iostreams system serialization thread program_options python numpy filesystem REQUIRED )
endif(WIN32 OR MSVC)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/code)

# Now we invoke each of our project sub-directories (they must each have a CMakeLists.txt file).
add_subdirectory(code)

