find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(testsuite testsuite.cpp)
set_target_properties(testsuite PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK)
target_link_libraries(testsuite
	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	${FORMATSTRING_NAME})

add_custom_target(test $<TARGET_FILE:testsuite> --log_level=success DEPENDS testsuite)
