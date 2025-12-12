
find_package(GTest CONFIG QUIET)
if(NOT GTest_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG release-1.12.1
	)
	FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

# Auto-discover all test source files under test/
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/test/*.cpp)
if(NOT TEST_SOURCES)
	message(FATAL_ERROR "No test sources found in test/; skipping test target setup.")
else()
	add_executable(tests ${TEST_SOURCES})
	target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

	# Link against GoogleTest and the main library target if available
	if(TARGET GTest::gtest_main)
		target_link_libraries(tests PRIVATE GTest::gtest_main)
	elseif(TARGET gtest_main)
		target_link_libraries(tests PRIVATE gtest_main)
	else()
		message(FATAL_ERROR "GoogleTest targets not found (expected GTest::gtest_main or gtest_main)")
	endif()

	include(GoogleTest)
	gtest_discover_tests(tests)
endif()
