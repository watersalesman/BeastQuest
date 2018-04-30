# Set library output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

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
)
