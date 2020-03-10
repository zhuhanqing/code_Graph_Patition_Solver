cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROJECT_NAME "hanqingNB")
project(${PROJECT_NAME})

#The project version number
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   1   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   0   CACHE STRING "Project patch version number.")

# output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(DOCUMENT_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/docs)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Check if intall prefix flag is found, if not, set a default path
if (CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_INSTALL_PREFIX})
else()
    set(CMAKE_INSTALL_PREFIX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()
unset(CMAKE_INSTALL_PREFIX)
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")




#Google Test
if (GTEST_DIR)
    set(GTEST_ROOT_DIR ${GTEST_DIR})
    message(STATUS "Use google test: ${GTEST_ROOT_DIR}")
else()
    set(GTEST_ROOT_DIR $ENV{GTEST_DIR})
    message(STATUS "Use system google test: ${GTEST_ROOT_DIR}")
endif()
unset(GTEST_DIR CACHE)


#Print out the used build type
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Using build type DEBUG")
    #ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Using build type RELEASE")
    #ENDIF(CMAKE_BUILD_TYPE MATCHES Release)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Profile)
    message(STATUS "Using build type PROFILE")
    #ENDIF(CMAKE_BUILD_TYPE MATCHES Profile)
ELSE()
    message(STATUS "Using build type DEFAULT: using Release flag")
    set(CMAKE_BUILD_TYPE Release)
ENDIF()
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-inline ")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fno-inline") 
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -frename-registers -fprofile-use -fprofile-correction") 
set(CMAKE_CXX_FLAGS_PROFILE "-Ofast -pg -Winline")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/unittest
    ${GTEST_ROOT_DIR}/googletest/include
)


file (GLOB GTEST_MAIN_LIB ${GTEST_ROOT_DIR}/lib/libgtest_main.a)
file (GLOB GTEST_LIB ${GTEST_ROOT_DIR}/lib/libgtest.a)
link_libraries (
    ${GTEST_MAIN_LIB}
)

file(GLOB SOURCES src/*.h        src/*.cpp
                  )

file(GLOB EXE_SOURCES src/main/*.cpp ${SOURCES})
file(GLOB UNITTEST_SOURCES 
    unittest/main/*.cpp
    unittest/db/*.cpp
    unittest/parser/*.cpp
    ${SOURCES})

#set_target_properties (${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static")

# Add modules to pybind
add_executable(${PROJECT_NAME} ${SOURCES} ${EXE_SOURCES})

#Unit Tests using GoogleTest
enable_testing()
add_executable(unittest ${UNITTEST_SOURCES})
target_link_libraries(unittest ${GTEST_LIB} )
add_test(NAME unittest COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/unittest ${CMAKE_CURRENT_SOURCE_DIR}/unittest)
