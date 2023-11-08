add_subdirectory("${PROJECT_SOURCE_DIR}/submodules/googletest" "submodules/googletest")

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test_with_libraries TESTNAME FILES LIBRARIES TEST_WORKING_DIRECTORY)
    add_executable(${TESTNAME} ${FILES})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main "${LIBRARIES}")
    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${TEST_WORKING_DIRECTORY}"
    )
    target_include_directories(${TESTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../auto_gen 
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/../intelitrader_umdf_sbe
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)

    
    target_compile_features(${TESTNAME} PUBLIC cxx_std_20)

endmacro()

set(LIBS_DEPENDS  b3_umdf asio)

package_add_test_with_libraries(socket_allocator_test allocator_test.cpp "${LIBS_DEPENDS}" "")
package_add_test_with_libraries(message_b3_protocol_sbe msg_b3_protocol_sbe.cpp "${LIBS_DEPENDS}" "")
package_add_test_with_libraries(udp_socket_receiver_test udp_socket_receiver_test.cpp "${LIBS_DEPENDS}" "")
package_add_test_with_libraries(st_cache_test st_cache_test.cpp "${LIBS_DEPENDS}" "")
package_add_test_with_libraries(channel_test channel_test.cpp "${LIBS_DEPENDS}" "")
