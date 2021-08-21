include_directories(${PROJECT_SOURCE_DIR}/src)


add_executable(testValue testValue.cpp ${PROJECT_SOURCE_DIR}/src/value.cpp)
add_executable(testShell testShell.cpp ${PROJECT_SOURCE_DIR}/src/shell.hpp)
target_link_libraries(testValue gtest)
target_link_libraries(testShell gtest)