# Boost
set(BOOST_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/boost")
if(NOT EXISTS ${BOOST_FOLDER})
    file(DOWNLOAD "https://dl.bintray.com/boostorg/beta/1.67.0.beta.1/source/boost_1_67_0_b1.tar.bz2" "${CMAKE_CURRENT_SOURCE_DIR}/boost.tar.bz2" SHOW_PROGRESS)

    # Decompress archive and rename folder
    message("Decompressing Boost archive...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf boost.tar.bz2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND rm boost.tar.bz2
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND mv boost_1_67_0 boost
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # Configure and build needed Boost components
    execute_process(COMMAND ./bootstrap.sh
    WORKING_DIRECTORY ${BOOST_FOLDER})
    execute_process(COMMAND ./b2 stage cxxflags=-fPIC variant=release link=static --stagedir=./build --with-system
    WORKING_DIRECTORY ${BOOST_FOLDER})
else()
    message("Boost folder exists. Skipping")
endif(NOT EXISTS ${BOOST_FOLDER})

# OpenSSL
set(OPENSSL_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/openssl")
set(OPENSSL_TAG "OpenSSL_1_1_1-pre2")
if(NOT EXISTS ${OPENSSL_FOLDER})
    file(DOWNLOAD "https://github.com/openssl/openssl/archive/${OPENSSL_TAG}.tar.gz" "${CMAKE_CURRENT_SOURCE_DIR}/openssl.tar.gz" SHOW_PROGRESS)

    # Decompress archive and rename folder
    message("Decompressing OpenSSL archive...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf openssl.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND rm openssl.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(COMMAND mv "openssl-${OPENSSL_TAG}" openssl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # Configure and build needed OpenSSL components
    execute_process(COMMAND ./config
    WORKING_DIRECTORY ${OPENSSL_FOLDER})
    execute_process(COMMAND make -j4
    WORKING_DIRECTORY ${OPENSSL_FOLDER})
else()
    message("OpenSSL folder exists. Skipping")
endif(NOT EXISTS ${OPENSSL_FOLDER})