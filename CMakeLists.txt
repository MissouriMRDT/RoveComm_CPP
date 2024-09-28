####################################################################################################################
##											    RoveComm C++												   	  ##
##											 v3.00.00 Build 000												 	  ##
##										  Mars Rover Design Team												  ##
##								   Copyright 2023 - All Rights Reserved										 	  ##
####################################################################################################################


## Set CMake Minimum Version
cmake_minimum_required(VERSION 3.24.3)

## C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## C++ Compiler Flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

## Project Name and Software Version Number
project(RoveComm_CPP VERSION 3.00.00)

## Include Required CMake Packages
if(NOT CPack_CMake_INCLUDED)
    include(CPack)
endif()
if(NOT CTest_CMake_INCLUDED)
    include(CTest)
endif()

####################################################################################################################
##                                                    Options                                                     ##
####################################################################################################################

## Enable or Disable cross-compile for windows on linux.
option(BUILD_WIN "Windows Cross-Compile Mode" OFF)

## Enable or Disable Tests Mode
option(BUILD_TESTS_MODE "Enable Tests Mode" OFF)

####################################################################################################################
##                                         Configuration Based on Options                                         ##
####################################################################################################################

## Check if the project is standalone or within another project.
if(NOT DEFINED __ROVECOMM_LIBRARY_MODE__)
    if(PROJECT_IS_TOP_LEVEL)
        set(__ROVECOMM_LIBRARY_MODE__ 0)
    else()
        set(__ROVECOMM_LIBRARY_MODE__ 1)
    endif()
endif()

## Check if we are on Windows or Linux or for windows mode if cross-compiling.
if(NOT DEFINED __ROVECOMM_WINDOWS_MODE__)
    if(WIN32 OR BUILD_WIN)
        set(__ROVECOMM_WINDOWS_MODE__ 1)
        include(ExternalProject)
    else()
        set(__ROVECOMM_WINDOWS_MODE__ 0)
    endif()
endif()

## Build Unit and Integration Tests
if (BUILD_TESTS_MODE)
    enable_testing()
endif()

message("-- RoveComm Build Mode: ${__ROVECOMM_LIBRARY_MODE__}")
add_definitions(-D__ROVECOMM_LIBRARY_MODE__=${__ROVECOMM_LIBRARY_MODE__})
message("-- RoveComm Windows Mode: ${__ROVECOMM_WINDOWS_MODE__}")
add_definitions(-D__ROVECOMM_WINDOWS_MODE__=${__ROVECOMM_WINDOWS_MODE__})

####################################################################################################################
##                                              Cross-Compile Mode                                                ##
####################################################################################################################

## Detect if we are cross-compiling or should compile for windows.
if(__ROVECOMM_WINDOWS_MODE__)
    message("-- RoveComm_CPP -- Cross-compiling for Windows on Linux")
    
    ## Set the cross-compilation toolchain
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

    ## Set necessary paths for MinGW
    set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    
    ## Check if we are using this as a library or just testing within the repo scope
    if(PROJECT_IS_TOP_LEVEL)
        message("-- RoveComm_CPP -- STAND ALONE MODE")

        ## Force CMAKE to build a completely static binary.
        set(CMAKE_EXE_LINKER_FLAGS "-static")

        ## Find Eigen3.
        find_package(Eigen3 3.3 REQUIRED NO_MODULE)

        if (BUILD_TESTS_MODE)
            ## ExternalProject configuration for pulling and building GTest
            ExternalProject_Add(GTest
                GIT_REPOSITORY  https://github.com/google/googletest.git
                GIT_TAG         v1.15.2  # Specify the GTest version tag or commit
                PREFIX          ${CMAKE_BINARY_DIR}/gtest
                CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest_install
                                -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix
                                -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix
                                -DCMAKE_SYSTEM_NAME=Windows
            )
            
            ## Create an imported target for GTest
            add_library(GTest::gtest IMPORTED INTERFACE)
            add_library(GTest::gtest_main IMPORTED INTERFACE)
            
            ## Specify where to find GTest once they're built
            ExternalProject_Get_Property(GTest install_dir)
            set(GTEST_INCLUDE_DIR ${install_dir}_install/include)
            set(GTEST_LIB_DIR ${install_dir}_install/lib)

            ## Must create empty install dirs.
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gtest_install/include)
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gtest_install/lib)

            ## Set the imported target properties for GTest
            set_target_properties(GTest::gtest PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
                INTERFACE_LINK_LIBRARIES ${GTEST_LIB_DIR}/libgtest.a
            )
            set_target_properties(GTest::gtest_main PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
                INTERFACE_LINK_LIBRARIES ${GTEST_LIB_DIR}/libgtest_main.a
            )
        endif()
    else()
        message("-- RoveComm_CPP -- LIBRARY MODE")
    endif()
else()
    message(STATUS "RoveComm_CPP -- Configuring for native platform: ${CMAKE_SYSTEM_NAME}")
    ## Check if we are using this as a library or just testing within the repo scope
    if(PROJECT_IS_TOP_LEVEL)
        message("-- RoveComm_CPP -- STAND ALONE MODE")

        if (BUILD_TESTS_MODE)
            ## Find Google Test
            find_package(GTest CONFIG REQUIRED)
            include(GoogleTest)
            add_library(GTest::GTest INTERFACE IMPORTED)
            target_link_libraries(GTest::GTest INTERFACE gtest_main)
        endif()

        ## Find Eigen3.
        find_package(Eigen3 3.3 REQUIRED NO_MODULE)
    else()
        message("-- RoveComm_CPP -- LIBRARY MODE")
    endif()
endif()


####################################################################################################################
##                                                 CPack Setup                                                    ##
####################################################################################################################

## Define Project Name and Version Number for CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Mars Rover Design Team")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/data/CPACK/logo.ico")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
set(CPACK_GENERATOR "STGZ")

####################################################################################################################
##                                           Dependencies and Libraries                                           ##
####################################################################################################################

## Set ThreadPool Variables
add_compile_definitions(BS_THREAD_POOL_ENABLE_PAUSE=1)

## Search Project Directories for Header and Source Files
file(GLOB_RECURSE RoveComm_INCLUDE      CONFIGURE_DEPENDS  "src/RoveComm/*.h")
file(GLOB_RECURSE RoveComm_SRC          CONFIGURE_DEPENDS  "src/RoveComm/*.cpp")

## Declare the library target.
add_library(${PROJECT_NAME} STATIC ${RoveComm_INCLUDE} ${RoveComm_SRC})

## Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PUBLIC src/RoveComm)

## Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

## Set the language property.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

## Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

## For access to standard installation directory variables (CMAKE_INSTALL_xDIR).
include(GNUInstallDirs)

####################################################################################################################
##                                             Installation                                                       ##
####################################################################################################################
## Set library shared object and API header file to install.
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

## Install headers and sources into corresponding folders
install(DIRECTORY src/RoveComm DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY external/ DESTINATION external FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY src/interfaces DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY src/util DESTINATION include FILES_MATCHING PATTERN "*.hpp")

## Create the pkg-config file from the template.
configure_file(src/RoveComm/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

## Set pkg-config file to install.
if(PROJECT_IS_TOP_LEVEL)
    install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
else()
    install(FILES ${CMAKE_BINARY_DIR}/external/rovecomm/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()



if(PROJECT_IS_TOP_LEVEL)
    ####################################################################################################################
    ##                                                Build Executable                                                ##
    ####################################################################################################################

    ## Find all main source files
    file(GLOB Main_SRC  CONFIGURE_DEPENDS  "src/*.cpp")

    ## Create Executable
    add_executable(${PROJECT_NAME}_App ${Main_SRC})

    ## Set Compile Options for RoveComm_CPP.
    if(MSVC) # True when compiler is Microsoft Visual C++/simulation of Visual C++ CL.
        target_compile_options(${PROJECT_NAME}_App PRIVATE /W4 /WX)
    else()
        target_compile_options(${PROJECT_NAME}_App PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    ## Attach libraries to Executable
    if (__ROVECOMM_WINDOWS_MODE__)
        message("-- RoveComm_CPP -- USING WINDOWS SOCKETS")
        # Set link libraries.
        target_link_libraries(${PROJECT_NAME}_App PRIVATE RoveComm_CPP ws2_32)
    else()
        message("-- RoveComm_CPP -- USING UNIX SOCKETS")
        target_link_libraries(${PROJECT_NAME}_App PRIVATE RoveComm_CPP)
    endif()

    ####################################################################################################################
    ##                                             Tests                                                              ##
    ####################################################################################################################

    if (BUILD_TESTS_MODE)
        ## Find all test source files
        file(GLOB UnitTests_SRC             CONFIGURE_DEPENDS  "tests/Unit/**/*.cc")
        file(GLOB IntegrationTests_SRC      CONFIGURE_DEPENDS  "tests/Integration/**/*.cc")

        ## Count the number of tests found for each type
        list(LENGTH UnitTests_SRC           UnitTests_LEN)
        list(LENGTH IntegrationTests_SRC    IntegrationTests_LEN)

        ## Create Unit Tests if found
        if (UnitTests_LEN GREATER 0)
            add_executable(${PROJECT_NAME}_UnitTests "tests/Unit/main.cc" "tests/TestUtils.cc" ${UnitTests_SRC})
            if (__ROVECOMM_WINDOWS_MODE__)
                ## Ensure GTest are built before project
                add_dependencies(${PROJECT_NAME}_UnitTests GTest)
                target_link_libraries(${PROJECT_NAME}_UnitTests RoveComm_CPP ws2_32 GTest::gtest GTest::gtest_main)
            else()
                target_link_libraries(${PROJECT_NAME}_UnitTests GTest::gtest GTest::gtest_main RoveComm_CPP)
            endif()
            add_test(Unit_Tests ${PROJECT_NAME}_UnitTests)
        endif()

        ## Create Integration Tests if found
        if (IntegrationTests_LEN GREATER 0)
            add_executable(${PROJECT_NAME}_IntegrationTests "tests/Unit/main.cc" "tests/TestUtils.cc" ${IntegrationTests_SRC})
            if (__ROVECOMM_WINDOWS_MODE__)
                ## Ensure GTest are built before project
                add_dependencies(${PROJECT_NAME}_IntegrationTests GTest)
                target_link_libraries(${PROJECT_NAME}_IntegrationTests RoveComm_CPP ws2_32 GTest::gtest GTest::gtest_main)
            else()
                target_link_libraries(${PROJECT_NAME}_IntegrationTests GTest::gtest GTest::gtest_main RoveComm_CPP)
            endif()
            add_test(Integration_Tests ${PROJECT_NAME}_IntegrationTests)
        endif()
    endif()
endif()
