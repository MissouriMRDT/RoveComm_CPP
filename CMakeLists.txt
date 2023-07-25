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
if(CPack_CMake_INCLUDED)
    include(CPack)
endif()
if(CTest_CMake_INCLUDED)
    include(CTest)
endif()

## Check if we are using this as a library or just testing within the repo scope
if(PROJECT_IS_TOP_LEVEL)
    message(STATUS "RoveComm_CPP -- STAND ALONE MODE")

    ## Find Quill
    find_package(quill REQUIRED)

    ## Find Google Test
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)
    add_library(GTest::GTest INTERFACE IMPORTED)
    target_link_libraries(GTest::GTest INTERFACE gtest_main)

    ## Enable to CTest and Google Test Frameworks
    enable_testing()
else()
    message(STATUS "RoveComm_CPP -- LIBRARY MODE")

    ## Set 'Library Mode' Flag
    add_compile_definitions(__LIBRARY_MODE__)
endif()

## Define Project Name and Version Number for CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Mars Rover Design Team")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR "SH")

## Search Project Directories for Header and Source Files
file(GLOB RoveComm_INCLUDE  CONFIGURE_DEPENDS  "src/RoveComm/*.h")
file(GLOB RoveComm_SRC      CONFIGURE_DEPENDS  "src/RoveComm/*.cpp")

## Declare the library target.
add_library(${PROJECT_NAME} STATIC ${RoveComm_INCLUDE} ${RoveComm_SRC})

## Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE src/RoveComm)

## Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

## Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

## Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "src/RoveComm/RoveCommGlobals.h")

## For access to standard installation directory variables (CMAKE_INSTALL_xDIR).
include(GNUInstallDirs)

## Set library shared object and API header file to install.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

## Create the pkg-config file from the template.
configure_file(src/RoveComm/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

## Set pkg-config file to install.
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

if(PROJECT_IS_TOP_LEVEL)
    ## Create Executable
    add_executable(${PROJECT_NAME}_App "src/main.cpp")
    target_link_libraries(${PROJECT_NAME}_App quill::quill RoveComm_CPP)

    ## Find all test source files
    file(GLOB UnitTests_SRC             CONFIGURE_DEPENDS  "tests/Unit/**/*.cc")
    file(GLOB IntegrationTests_SRC      CONFIGURE_DEPENDS  "tests/Integration/**/*.cc")

    ## Count the number of tests found for each type
    list(LENGTH UnitTests_SRC           UnitTests_LEN)
    list(LENGTH IntegrationTests_SRC    IntegrationTests_LEN)

    ## Create Unit Tests if found
    if (UnitTests_LEN GREATER 0)
        add_executable(${PROJECT_NAME}_UnitTests "tests/Unit/main.cc" ${UnitTests_SRC})
        target_link_libraries(${PROJECT_NAME}_UnitTests GTest::gtest GTest::gtest_main)
        add_test(Unit_Tests ${PROJECT_NAME}_UnitTests)
    endif()

    ## Create Integration Tests if found
    if (IntegrationTests_LEN GREATER 0)
        add_executable(${PROJECT_NAME}_IntegrationTests "tests/Unit/main.cc" ${IntegrationTests_SRC})
        target_link_libraries(${PROJECT_NAME}_IntegrationTests GTest::gtest GTest::gtest_main)
        add_test(Integration_Tests ${PROJECT_NAME}_IntegrationTests)
    endif()
else()
    message(STATUS "RoveComm_CPP -- LIBRARY MODE")

    ## Set 'Library Mode' Flag
    add_compile_definitions(__LIBRARY_MODE__)
endif()