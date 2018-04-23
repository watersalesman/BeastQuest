set(gtest_force_shared_crt ON CACHE INTERNAL "")
add_subdirectory(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/unit-tests")

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json.hpp")
    file(DOWNLOAD "https://github.com/nlohmann/json/releases/download/v3.1.2/json.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/json.hpp" SHOW_PROGRESS)
endif(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/json.hpp")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BeastQuest_INCLUDEDIR}
)

set(UNIT_TESTS
    auth-test
    url-test
    util-test
    parameters-test
    simpleform-test
    multipart-test
    session-test
    api-test
)

# Don't run SSL tests on local httpbin server
if(NOT BeastQuest_LOCAL_HTTPBIN)
    set(UNIT_TESTS
        ${UNIT_TESTS}
        ssl-test
    )
else()
    add_definitions(-DBeastQuest_LOCAL_HTTPBIN=ON)
endif(NOT BeastQuest_LOCAL_HTTPBIN)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cc)
    target_link_libraries(
        ${TEST}
        gtest_main
        ${BeastQuest_LIBRARIES}
        ${OPENSSL_LIBRARIES}
    )
    add_test(NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}>)
endforeach(TEST in UNIT_TESTS)
