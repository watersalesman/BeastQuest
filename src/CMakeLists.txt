# Set library output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Generate code coverage
if(BeastQuest_TEST_COVERAGE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage --coverage")
    set(GCOV gcov)
endif(BeastQuest_TEST_COVERAGE)

# Compile and link
include_directories(${BeastQuest_INCLUDE})
add_library(
    ${PROJECT_NAME}
    auth.cc
    multipart.cc
    session.cc
    parameters.cc
    simpleform.cc
    url.cc
    util.cc
)
target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    pthread
    dl
    stdc++
    ${GCOV}
)
