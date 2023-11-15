############### LIB ########################################################

# set(LIB_NAME intelitrader_feed_sbe)
# set(LIB_INTERFACE_NAME intelitrader-umdf-feed-sbe)

find_package(Threads REQUIRED)

add_library(asio INTERFACE)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../submodules/asio/asio/include)
target_link_libraries(asio INTERFACE Threads::Threads)

add_library(b3_umdf INTERFACE)
target_include_directories(b3_umdf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_features(b3_umdf INTERFACE  cxx_std_20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("--Debug Build")
    target_compile_options(b3_umdf INTERFACE -fprofile-arcs -ftest-coverage -fPIC -O0)
    target_link_libraries(b3_umdf  INTERFACE pcap asio Threads::Threads gcov)
else ()
	#	target_compile_options(b3_umdf INTERT-fPIC)
	target_link_libraries(b3_umdf INTERFACE pcap asio Threads::Threads)
endif()

add_executable(backtest_sample back_test.cpp)
target_link_libraries(backtest_sample b3_umdf)
