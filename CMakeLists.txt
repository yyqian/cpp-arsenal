cmake_minimum_required(VERSION 3.8)
project(cpp_arsenal)
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(src/arsenal)
include_directories(src)

add_subdirectory(src/arsenal)

add_executable(bom-check src/bom-check.cpp)
add_executable(java_util src/java_util.cpp)
add_executable(t0 src/playground/t0.c)
#add_executable(producer_consumer src/playground/producer_consumer.c)
add_executable(play src/playground/play.cpp)
#add_executable(http src/playground/http.cpp)
add_executable(apue_file src/apue/file.cpp)
add_executable(io src/apue/io.cpp)
add_executable(simple_tcp_daytime_server src/unp/simple_tcp_daytime_server.cpp)
add_executable(simple_tcp_daytime_client src/unp/simple_tcp_daytime_client.cpp)
add_executable(byte_order src/unp/byte_order.cpp)
add_executable(tcp_echo_server src/unp/tcp_echo_server.c)
add_executable(tcp_echo_client src/unp/tcp_echo_client.c)
add_executable(addrinfo src/unp/addrinfo.cpp)
add_executable(tcp_echo_client_select src/unp/tcp_echo_client_select.c)
add_executable(tcp_echo_server_select src/unp/tcp_echo_server_select.c)
add_executable(tcp_echo_server_epoll src/unp/tcp_echo_server_epoll.cpp)
add_executable(thread src/apue/thread.cpp)
add_executable(cond_var src/apue/cond_var.cpp)
add_executable(thread_safe src/apue/thread_safe.cpp)
add_executable(consum_prod_sem src/apue/consum_prod_sem.cpp)
add_executable(rwlock_sem src/apue/rwlock_sem.c)
add_executable(problem src/playground/problem.cpp)
add_executable(mem src/playground/mem.cpp)
add_executable(text_query src/playground/text_query.cpp)
add_executable(std_thread src/playground/std_thread.cpp)
add_executable(consum_prod_std src/apue/consum_prod_std.cpp)
add_executable(read_write_lock_std src/apue/read_write_lock_std.cpp)
add_executable(mp src/playground/mp.cpp)
target_link_libraries(bom-check arsenal_static)
target_link_libraries(t0 pthread)
#target_link_libraries(producer_consumer pthread)
target_link_libraries(consum_prod_sem pthread)
target_link_libraries(rwlock_sem pthread)
#target_link_libraries(http curl)
target_link_libraries(thread pthread)
target_link_libraries(cond_var pthread)
target_link_libraries(thread_safe pthread)