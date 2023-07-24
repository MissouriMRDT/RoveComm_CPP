####################################################################################################################
##											    RoveComm C++												   	  ##
##											v24.00.00 Build 000												 	  ##
##										  Mars Rover Design Team												  ##
##								   Copyright 2023 - All Rights Reserved										 	  ##
####################################################################################################################


## Set CMake Minimum Version
cmake_minimum_required(VERSION 3.24.3)

## C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Project Name and Software Version Number
project(RoveComm_CPP VERSION 24.00.00)

## Include Required CMake Packages
include(CPack)
include(CTest)

## Define Project Name and Version Number for CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Mars Rover Design Team")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR "SH")

## Find Quill
find_package(quill REQUIRED)

## Find Google Test
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

## Enable to CTest and Google Test Frameworks
enable_testing()

## Declare the library target.
add_library(${PROJECT_NAME} STATIC
    src/convert.cpp
    src/convert.h
)

## Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE src)

## Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

## Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

## Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/convert.h)

## For access to standard installation directory variables (CMAKE_INSTALL_xDIR).
include(GNUInstallDirs)

## Set library shared object and API header file to install.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

## Create the pkg-config file from the template.
configure_file(src/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

## Set pkg-config file to install.
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

## Unit/Integration Tests
file(GLOB UnitTests_SRC             CONFIGURE_DEPENDS  "tests/Unit/**/*.cc")
file(GLOB IntegrationTests_SRC      CONFIGURE_DEPENDS  "tests/Integration/**/*.cc")

list(LENGTH UnitTests_SRC           UnitTests_LEN)
list(LENGTH IntegrationTests_SRC    IntegrationTests_LEN)

if (UnitTests_LEN GREATER 0)
    add_executable(${PROJECT_NAME}_UnitTests "tests/Unit/main.cc" ${UnitTests_SRC})
    target_link_libraries(${PROJECT_NAME}_UnitTests GTest::gtest GTest::gtest_main)
    add_test(Unit_Tests ${PROJECT_NAME}_UnitTests)
endif()

if (IntegrationTests_LEN GREATER 0)
    add_executable(${PROJECT_NAME}_IntegrationTests "tests/Unit/main.cc" ${IntegrationTests_SRC})
    target_link_libraries(${PROJECT_NAME}_IntegrationTests GTest::gtest GTest::gtest_main)
    add_test(Integration_Tests ${PROJECT_NAME}_IntegrationTests)
endif()