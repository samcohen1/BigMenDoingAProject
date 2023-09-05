message("Processing CMakeLists.txt")

cmake_minimum_required(VERSION 3.27.1) # the currently stable version of CMake is required
project(elen3009-project)

# ====================== Declare Variables ======================

set(SRC_PATH "${CMAKE_SOURCE_DIR}/game-source-code") # path to game source code folder
set(MAIN_CPP "main.cpp") # your cpp file that runs your game and contains the entry point main() function
set(GAME_EXE_NAME "game") # name of the game executable
set(TESTS_EXE_NAME "tests") # name of the test executable
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # the output directory for the executables
set(WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # working directory for exe's so relative paths are correct when running from within VS Code
get_filename_component(COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY) # extract the path to the C++ compiler being used

# ====================== Select Files for Compilation ======================

# CONFIGURE_DEPENDS is used to make sure that the globbing is re-run when a new source or test file is added
file(GLOB GAME_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/game-source-code/*.cpp)
file(GLOB TESTS_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/game-source-code/*.cpp ${CMAKE_SOURCE_DIR}/test-source-code/*.cpp) # compile/link all cpp files in game-source-code and test-source-code for the test executable
list(REMOVE_ITEM TESTS_SRC "${CMAKE_SOURCE_DIR}/game-source-code/${MAIN_CPP}") # remove MAIN_CPP from the test source files - doctest provides its own main function

# ====================== Download Dependencies ======================

include(FetchContent)

# doctest
FetchContent_Declare(  # supersedes ExternalProject_Add for downloading dependencies
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    # corresponds to doctest v2.4.11 - good practice to specify the commit hash as tags can be moved
    GIT_TAG "ae7a13539fb71f270b87eb2e874fbac80bc8dda2" )

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    # corresponds to SFML 2.6.0 - good practice to specify a commit hash, tags can be moved
    GIT_TAG "11b73743c42cf7ecd7c596ba83fdbf1150ffa96c" )

# make the dependencies available to the build system and populate dependency variables like doctest_SOURCE_DIR
FetchContent_MakeAvailable(doctest SFML)

# ====================== Setup Targets ======================

# Game executable target
add_executable(${GAME_EXE_NAME} ${GAME_SRC})
target_compile_features(${GAME_EXE_NAME} PRIVATE cxx_std_17) # enable C++17 features for the target
target_link_libraries(${GAME_EXE_NAME} PRIVATE sfml-audio sfml-graphics) # link privately to hide SFML internal headers

# Test executable target
add_executable(${TESTS_EXE_NAME} ${TESTS_SRC})
target_include_directories(${TESTS_EXE_NAME} PRIVATE ${SRC_PATH}) # include game source code
target_include_directories(${TESTS_EXE_NAME} PRIVATE "${doctest_SOURCE_DIR}/doctest") # include doctest header
target_compile_features(${TESTS_EXE_NAME} PRIVATE cxx_std_17) # enable C++17 features for the target
target_link_libraries(${TESTS_EXE_NAME} PRIVATE sfml-audio sfml-graphics) # link privately to hide SFML internal headers

# ====================== Post-Build  ======================

# ----- platform/compiler-specific post-build commands -------

function(copy_dlls TARGET)
    # alternative to the code below; https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake
    # copy SFML DLLs (from the SFML bin directory) to the executables' directory
    install(FILES $<TARGET_RUNTIME_DLLS:${TARGET}> TYPE BIN)

    # These MinGW DLLs are required for the game to run on Windows. They will not be present if the computer that the game is running on
    # does not have MinGW installed.
    add_custom_command(TARGET "${TARGET}" POST_BUILD
        # copy MinGW DLLs from compiler's bin directory to executables' directory
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${COMPILER_PATH}/libstdc++-6.dll $<TARGET_FILE_DIR:${TARGET}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${COMPILER_PATH}/libgcc_s_seh-1.dll $<TARGET_FILE_DIR:${TARGET}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${COMPILER_PATH}/libwinpthread-1.dll $<TARGET_FILE_DIR:${TARGET}>
    )
    add_custom_command(TARGET "${TARGET}" POST_BUILD
        # SFML audio requires openal32.dll to be in the same directory (bin) as the game executable
        # this is not copied automatically by SFML's install target
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FETCHCONTENT_BASE_DIR}/sfml-src/extlibs/bin/x64/openal32.dll $<TARGET_FILE_DIR:${GAME_EXE_NAME}>
    )
endfunction()

# if the MinGW GCC compiler is used on windows and SFML shared libraries are built, then copy all the required DLLs to the executables' directory
if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND BUILD_SHARED_LIBS)
    copy_dlls(${GAME_EXE_NAME})
    copy_dlls(${TESTS_EXE_NAME})
else()
    message("Unknown platform and compiler combination. Library dependencies not copied to output directory.")
endif()

# -------- platform/compiler-independent post-build commands ---------

function(copy_game_resources TARGET)
    add_custom_command(TARGET "${TARGET}" POST_BUILD
        # copy the game resources folder to the executable's directory
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_CURRENT_SOURCE_DIR}/game-source-code/resources $<TARGET_FILE_DIR:${GAME_EXE_NAME}>/resources COMMAND_EXPAND_LISTS
        )
endfunction()

copy_game_resources(${GAME_EXE_NAME})
copy_game_resources(${TESTS_EXE_NAME})

# ====================== CTest ======================

include(CTest) # include otherwise Site is missing
enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
# automatically add doctest tests to CTest; specify WORKING_DIRECTORY to ensure that relative paths are correct for CTest
doctest_discover_tests(${TESTS_EXE_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
