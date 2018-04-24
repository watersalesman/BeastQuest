* [Introduction](https://watersalesman.github.io/index.md)
* [Build](https://watersalesman.github.io/build.md)

# Build

BeastQuest currently only supports Linux and OS X. This will change as I figure
out a convenient way of handling dependencies on Windows.

## CMake Build

This project uses CMake, so it is as simple as cloning the repo, adding a new
subdirectory to your CMake project, and using the provided variables:
```cmake
# This will download and configure needed dependencies
set(BeastQuest_BUILDS_DEPS ON)

add_subdirectory(beastquest)

include_directories(${BeastQuest_INCLUDES})
target_link_libraries(your_target_here ${BeastQuest_LIBRARIES})
```

The `BeastQuest_BUILD_DEPS` variable will build both Boost and OpenSSL and
handle their include files and libraries. You can also specify which
dependency to handle with `BeastQuest_BUILD_BOOST` or
`BeastQuest_BUILD_OPENSSL`. When BeastQuest handles dependencies, static linking
is the default for OpenSSL on Windows and Mac OS X, and for Boost on all
platforms. You can change this default by setting `Boost_USE_STATIC_LIBS` and
`OPENSSL_USE_STATIC_LIBS` to `OFF`.

If depedencies are not handled, CMake will search default locations for an installed version of Boost or OpenSSL and link dynamically by default.


### Tests

Tests use Google Test, which can be downloaded by running the following:
```
git submodule update --init
```
Tests can be built by setting the CMake option `BeastQuest_BUILD_TESTS` to `ON`
either on the command line or in your `CMakeLists.txt`.
