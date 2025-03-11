cmake_minimum_required(VERSION 3.10)
project(aerospace_encryptor_with_qr)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(mbedtls)

add_executable(aerospace_encryptor main.cpp qrcodegen.cpp)
target_link_libraries(aerospace_encryptor mbedcrypto)
set_target_properties(aerospace_encryptor PROPERTIES LINK_FLAGS "-static")
