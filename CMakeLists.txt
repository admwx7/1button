cmake_minimum_required(VERSION 3.22)
project(1button VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Do we still need this?
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BUILD_TESTING ON)
else()
  set(BUILD_TESTING OFF)
endif()

# Collect sources
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.cc)
if(NOT PROJECT_SOURCES)
  message(FATAL_ERROR "No sources found in src/. Add source files or adjust CMakeLists.txt.")
endif()

# Main executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link common libs installed via vcpkg (if present)
find_package(SDL3 CONFIG REQUIRED)
if(SDL3_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif()
find_package(SDL3_image CONFIG REQUIRED)
if(SDL3_image_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)
endif()
find_package(SDL3_ttf CONFIG REQUIRED)
if(SDL3_ttf_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)
endif()

# SDL_image (used for loading various image formats)
# set(SDLIMAGE_VENDORED OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_PNG ON)
set(SDLIMAGE_SVG OFF)
# add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# Clang-tidy integration (apply to targets)
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
  message(FATAL_ERROR "clang-tidy requested but not found on PATH.")
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# Tests: add_subdirectory(test) if a test CMakeLists exists, otherwise try simple GTest discovery
if(BUILD_TESTING)
  enable_testing()
  if(EXISTS ${CMAKE_SOURCE_DIR}/test/CMakeLists.txt)
    add_subdirectory(test)
  else()
    find_package(GTest QUIET)
    if(GTest_FOUND)
      file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS test/*.cpp)
      if(TEST_SOURCES)
        add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
        target_link_libraries(${PROJECT_NAME}_tests PRIVATE GTest::gtest_main)
        add_test(NAME unit_tests COMMAND ${PROJECT_NAME}_tests)
      endif()
    endif()
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
endif()

# Basic packaging metadata (optional)
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)