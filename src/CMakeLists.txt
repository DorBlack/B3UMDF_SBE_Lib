############### LIB ########################################################

# set(LIB_NAME intelitrader_feed_sbe)
# set(LIB_INTERFACE_NAME intelitrader-umdf-feed-sbe)

find_package(Threads REQUIRED)

add_library(asio INTERFACE)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../submodules/asio/asio/include)
target_link_libraries(asio INTERFACE Threads::Threads)

add_library(b3_umdf)
target_sources(b3_umdf PRIVATE umdf/impl_channel.cpp)
target_include_directories(b3_umdf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_features(b3_umdf PUBLIC cxx_std_20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("--Debug Build")
    target_compile_options(b3_umdf PUBLIC -fprofile-arcs -ftest-coverage -fPIC -O0)
    target_link_libraries(b3_umdf pcap asio Threads::Threads gcov)
else ()
    target_link_libraries(b3_umdf PRIVATE pcap asio Threads::Threads)
endif()

add_executable(backtest_sample back_test.cpp)
target_link_libraries(backtest_sample b3_umdf)

# add_library(${LIB_NAME} SHARED
#        ../include/io/udp_socket_receiver.hpp)

# target_sources(${LIB_NAME} PRIVATE  sbe_message.cpp
#                                b3_message.cpp
#                                umdf_channel_sbe.cpp
#                                sbe_message_security_def.cpp
#                                sbe_message_book_orders.cpp
#                                sbe_message_statistics.cpp
#                                sbe_message_news.cpp
#                                umdf_instrument.cpp
#                                instrument_proxy.cpp
#                                channel_by_instrument.cpp
#                                channel_stream.cpp "instrument_proxy.cpp" "channel_by_instrument.cpp")

# target_include_directories(${LIB_NAME} PUBLIC
#                                                ${CMAKE_CURRENT_SOURCE_DIR}/../intelitrader_umdf_sbe
#                                        PRIVATE
#                                                ${CMAKE_CURRENT_SOURCE_DIR}/../include
#                                                ${CMAKE_CURRENT_SOURCE_DIR}/../submodules/HdrHistogram_c/include/hdr)
# target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)

# target_compile_options(${LIB_NAME} PUBLIC PUBLIC -Wall -Wextra -Wpedantic -Werror -Wno-language-extension-token )

# target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads asio hdr_histogram_static)

# add_library(${LIB_INTERFACE_NAME} INTERFACE)
# target_include_directories(${LIB_INTERFACE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../intelitrader_umdf_sbe)
# target_link_libraries(${LIB_INTERFACE_NAME} INTERFACE ${LIB_NAME})

# set_target_properties(${LIB_INTERFACE_NAME} PROPERTIES
#        VERSION ${PROJECT_VERSION}
#        SOVERSION ${PROJECT_VERSION_MAJOR}
#        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# install(TARGETS ${LIB_INTERFACE_NAME}
#            PUBLIC_HEADER
#            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} )

# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../intelitrader_umdf_sbe
#         DESTINATION include
#         FILES_MATCHING PATTERN "*.hpp")


