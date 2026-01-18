cmake_minimum_required(VERSION 3.15)
project(endstone_test LANGUAGES CXX)

endstone_add_plugin(test_plugin test_plugin.cpp)
set_target_properties(test_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "plugins")
set_target_properties(test_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "plugins")

add_executable(endstone_test
        bedrock/test_actor_definition_identifier.cpp
        bedrock/test_binary_stream.cpp
        bedrock/test_block_pos.cpp
        bedrock/test_check.cpp
        bedrock/test_function_ref.cpp
        bedrock/test_hashed_string.cpp
        bedrock/test_sem_version.cpp
        bedrock/test_spin_lock.cpp
        bedrock/test_static_optimized_string.cpp
        bedrock/test_uuid.cpp
        test_identifier.cpp
        test_nbt.cpp
        test_base64.cpp
        test_command_lexer.cpp
        test_command_usage_parser.cpp
        test_cpp_plugin_loader.cpp
        test_logger_factory.cpp
        test_player_ban_list.cpp
        test_scheduler.cpp
        test_service_manager.cpp
        test_thread_pool_executor.cpp
        test_uuid.cpp
        test_vector.cpp
)
add_dependencies(endstone_test test_plugin)
target_link_libraries(endstone_test PRIVATE endstone::core GTest::gtest_main GTest::gmock_main)
gtest_discover_tests(endstone_test)