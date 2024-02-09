####################################################################################################################
##											    RoveComm C++												   	  ##
##											 v3.00.00 Build 000												 	  ##
##										  Mars Rover Design Team												  ##
##								   Copyright 2023 - All Rights Reserved										 	  ##
####################################################################################################################


## Set CMake Minimum Version
cmake_minimum_required(VERSION 3.24.3)

## C++ Version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Project Name and Software Version Number
project(RoveComm_CPP VERSION 3.00.00)

## Include Required CMake Packages
if(NOT CPack_CMake_INCLUDED)
    include(CPack)
endif()
if(NOT CTest_CMake_INCLUDED)
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

    ## Find Eigen3.
    find_package(Eigen3 3.3 REQUIRED NO_MODULE)

    ## Enable to CTest and Google Test Frameworks
    enable_testing()
else()
    message(STATUS "RoveComm_CPP -- LIBRARY MODE")

    ## Set 'Library Mode' Flag
    add_compile_definitions(__ROVECOMM_LIBRARY_MODE__)
endif()

## Define Project Name and Version Number for CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Mars Rover Design Team")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/data/CPACK/logo.ico")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_GENERATOR "STGZ")

## Search Project Directories for Header and Source Files
file(GLOB RoveComm_INCLUDE  CONFIGURE_DEPENDS  "src/RoveComm/*.h")
file(GLOB RoveComm_SRC      CONFIGURE_DEPENDS  "src/RoveComm/*.cpp")

## Declare the library target.
add_library(${PROJECT_NAME} STATIC ${RoveComm_INCLUDE} ${RoveComm_SRC})

## Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE src/RoveComm)

## Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

## Set the language property.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

## Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "src/RoveComm/RoveComm.h")

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
if(PROJECT_IS_TOP_LEVEL)
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
else()
    install(FILES ${CMAKE_BINARY_DIR}/build/_deps/rovecomm_cpp-src/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()

if(PROJECT_IS_TOP_LEVEL)
    ## Find all main source files
    file(GLOB Main_SRC  CONFIGURE_DEPENDS  "src/*.cpp")
    
    ## Create Executable
    add_executable(${PROJECT_NAME}_App ${Main_SRC})

    ## Set Compile Options for Autonomy Software.
    if(MSVC) # True when compiler is Microsoft Visual C++/simulation of Visual C++ CL.
        target_compile_options(${PROJECT_NAME}_App PRIVATE /W4 /WX)
    else()
        target_compile_options(${PROJECT_NAME}_App PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    ## Attach libraries to Executable
    target_link_libraries(${PROJECT_NAME}_App PRIVATE quill::quill RoveComm_CPP)

    ## Find all test source files
    file(GLOB UnitTests_SRC             CONFIGURE_DEPENDS  "tests/Unit/**/*.cc")
    file(GLOB IntegrationTests_SRC      CONFIGURE_DEPENDS  "tests/Integration/**/*.cc")

    ## Count the number of tests found for each type
    list(LENGTH UnitTests_SRC           UnitTests_LEN)
    list(LENGTH IntegrationTests_SRC    IntegrationTests_LEN)

    ## Create Unit Tests if found
    if (UnitTests_LEN GREATER 0)
        add_executable(${PROJECT_NAME}_UnitTests "tests/Unit/main.cc" "tests/TestUtils.cc" ${UnitTests_SRC})
        target_link_libraries(${PROJECT_NAME}_UnitTests GTest::gtest GTest::gtest_main RoveComm_CPP)
        add_test(Unit_Tests ${PROJECT_NAME}_UnitTests)
    endif()

    ## Create Integration Tests if found
    if (IntegrationTests_LEN GREATER 0)
        add_executable(${PROJECT_NAME}_IntegrationTests "tests/Unit/main.cc" "tests/TestUtils.cc" ${IntegrationTests_SRC})
        target_link_libraries(${PROJECT_NAME}_IntegrationTests GTest::gtest GTest::gtest_main RoveComm_CPP)
        add_test(Integration_Tests ${PROJECT_NAME}_IntegrationTests)
    endif()
else()
    message(STATUS "RoveComm_CPP -- LIBRARY MODE")

    ## Set 'Library Mode' Flag
    add_compile_definitions(__ROVECOMM_LIBRARY_MODE__)
endif()