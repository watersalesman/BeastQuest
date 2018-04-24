cmake_minimum_required (VERSION 3.7.2)
project (beastquest)

# Set compiler if CXX is defined
if($ENV{CXX})
    set(CMAKE_CXX_COMPILER $ENV{CXX})
endif($ENV{CXX})

# Set standard
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to release by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Set library variables
set(BeastQuest_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "")

# Build dependencies if specified
if(BeastQuest_BUILD_DEPS)
    set(BeastQuest_BUILD_BOOST ON)
    set(BeastQuest_BUILD_OPENSSL ON)
endif(BeastQuest_BUILD_DEPS)
if(BeastQuest_BUILD_BOOST OR BeastQuest_BUILD_OPENSSL)
    add_subdirectory(deps)
endif(BeastQuest_BUILD_BOOST OR BeastQuest_BUILD_OPENSSL)

# Set Boost options if handling Boost
if(BeastQuest_BUILD_BOOST)
    # Use static libs by default when handling Boost
    if(NOT DEFINED Boost_USE_STATIC_LIBS)
        set(Boost_USE_STATIC_LIBS ON)
    endif(NOT DEFINED Boost_USE_STATIC_LIBS)
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/build/lib")
endif(BeastQuest_BUILD_BOOST)
find_package(Boost COMPONENTS system REQUIRED)

# Set OpenSSL options if handling OpenSSL
if(BeastQuest_BUILD_OPENSSL)
    # Use static libs by default on Windows and Apple when handling OpenSSL
    if(WIN32 OR APPLE AND NOT DEFINED OPENSSL_USE_STATIC_LIBS)
        set(OPENSSL_USE_STATIC_LIBS TRUE)
    endif(WIN32 OR APPLE AND NOT DEFINED OPENSSL_USE_STATIC_LIBS)
    set(OPENSSL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
endif(BeastQuest_BUILD_OPENSSL)
find_package(OpenSSL REQUIRED)

# Set include directories
set(BeastQuest_INCLUDE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  CACHE
  INTERNAL
  ""
)
include_directories(
    ${BeastQuest_INCLUDE}
    ${BOOST_INCLUDEDIR}
    ${OPENSSL_INCLUDE_DIR}
)

# Build BeastQuest lib
add_subdirectory(src)

# Build tests if enabled
if(BeastQuest_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif(BeastQuest_BUILD_TESTS)
