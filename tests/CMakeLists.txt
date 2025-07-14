cmake_minimum_required(VERSION 3.15)
project(endstone_test LANGUAGES CXX)

endstone_add_plugin(test_plugin endstone/core/test_plugin.cpp)
set_target_properties(test_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "plugins")
set_target_properties(test_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "plugins")

add_executable(endstone_test
        bedrock/test_binary_stream.cpp
        bedrock/test_block_pos.cpp
        bedrock/test_check.cpp
        bedrock/test_function_ref.cpp
        bedrock/test_hashed_string.cpp
        bedrock/test_sem_version.cpp
        bedrock/test_spin_lock.cpp
        bedrock/test_static_optimized_string.cpp
        bedrock/test_uuid.cpp
        endstone/core/test_base64.cpp
        endstone/core/test_command_lexer.cpp
        endstone/core/test_command_usage_parser.cpp
        endstone/core/test_cpp_plugin_loader.cpp
        endstone/core/test_logger_factory.cpp
        endstone/core/test_player_ban_list.cpp
        endstone/core/test_scheduler.cpp
        endstone/core/test_service_manager.cpp
        endstone/core/test_thread_pool_executor.cpp
        endstone/core/test_uuid.cpp
        endstone/core/test_vector.cpp
)
add_dependencies(endstone_test test_plugin)
target_link_libraries(endstone_test PRIVATE endstone::core GTest::gtest_main GTest::gmock_main)
gtest_discover_tests(endstone_test)