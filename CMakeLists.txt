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
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Set library variables
set(BeastQuest_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "")

# Set include directories
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost/")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/build/lib")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/build/include")
set(OPENSSL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(BeastQuest_INCLUDE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  CACHE
  INTERNAL
  ""
)
include_directories(
    ${BOOST_INCLUDEDIR}
    ${OPENSSL_INCLUDE_DIR}
    ${BeastQuest_INCLUDE}
)

# Build dependencies if specified
if(BeastQuest_BUILD_DEPS)
    add_subdirectory(deps)
endif(BeastQuest_BUILD_DEPS)

# Find dependencies
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)
if(WIN32 OR APPLE)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif(WIN32 OR APPLE)
find_package(OpenSSL REQUIRED)

add_subdirectory(src)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    enable_testing()
    add_subdirectory(test)
endif()