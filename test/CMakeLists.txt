add_executable(format format.cpp)
target_link_libraries(format ${FORMATSTRING_NAME})

add_custom_target(test python3 "${CMAKE_CURRENT_SOURCE_DIR}/test.py" $<TARGET_FILE:format> DEPENDS format)
