add_executable(example example.cpp)
target_link_libraries(example ${FORMATSTRING_NAME})

add_executable(format format.cpp)
target_link_libraries(format ${FORMATSTRING_NAME})
