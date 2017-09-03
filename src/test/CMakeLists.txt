aux_source_directory(. TEST_FILES)
add_executable(arsenal_test ${TEST_FILES})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include)
target_link_libraries(arsenal_test arsenal_static gtest gtest_main)
