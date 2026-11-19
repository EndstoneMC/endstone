cmake_minimum_required(VERSION 3.15)
project(endstone_test LANGUAGES CXX)

endstone_add_plugin(test_plugin test_plugin.cpp)
set_target_properties(test_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "plugins")
set_target_properties(test_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "plugins")

endstone_add_plugin(brigadier_probe brigadier/brigadier_probe.cpp)
set_target_properties(brigadier_probe PROPERTIES LIBRARY_OUTPUT_DIRECTORY "probes")
set_target_properties(brigadier_probe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "probes")

add_executable(endstone_test
        bedrock/test_actor_definition_identifier.cpp
        bedrock/test_binary_stream.cpp
        bedrock/test_block_pos.cpp
        bedrock/test_check.cpp
        bedrock/test_command_registry.cpp
        bedrock/test_command_selector.cpp
        bedrock/test_function_ref.cpp
        bedrock/test_hashed_string.cpp
        bedrock/test_minecraft_commands.cpp
        bedrock/test_sem_version.cpp
        bedrock/test_spin_lock.cpp
        bedrock/test_static_optimized_string.cpp
        bedrock/test_uuid.cpp
        brigadier/test_arguments.cpp
        brigadier/test_builder.cpp
        brigadier/test_completion.cpp
        brigadier/test_context.cpp
        brigadier/test_cross_module.cpp
        brigadier/test_dispatcher.cpp
        brigadier/test_exceptions.cpp
        brigadier/test_reader.cpp
        brigadier/test_redirect.cpp
        brigadier/test_suggestion.cpp
        brigadier/test_tree.cpp
        brigadier/test_usages.cpp
        test_identifier.cpp
        test_nbt.cpp
        test_base64.cpp
        test_command_compiler.cpp
        test_command_lexer.cpp
        test_command_tree.cpp
        test_command_usage_parser.cpp
        test_cpp_plugin_loader.cpp
        test_effect.cpp
        test_http_signaling_server.cpp
        test_enchantment_offer.cpp
        test_logger_factory.cpp
        test_player_ban_list.cpp
        test_permissions.cpp
        test_pointers.cpp
        test_plugin_manager.cpp
        test_scheduler.cpp
        test_service_manager.cpp
        test_text_formatter.cpp
        test_thread_pool_executor.cpp
        test_uuid.cpp
        test_object.cpp
        test_vector.cpp
)
add_dependencies(endstone_test test_plugin brigadier_probe)
find_package(asio REQUIRED)
target_link_libraries(endstone_test PRIVATE endstone::core asio::asio GTest::gtest_main GTest::gmock_main)
# CppPluginLoaderTest cases share the plugins/.local shadow-copy dir, whose
# cleanup races when ctest runs them in parallel; serialize just those.
gtest_discover_tests(endstone_test
        TEST_FILTER "CppPluginLoaderTest.*"
        PROPERTIES RESOURCE_LOCK cpp_plugin_loader_local)
gtest_discover_tests(endstone_test
        TEST_FILTER "-CppPluginLoaderTest.*")
