// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ENDSTONE_DISABLE_DEVTOOLS

#include "endstone/detail/gui/dev_tools.h"

#include <imgui.h>
#include <imgui_file_browser.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <cstdio>
#include <filesystem>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

#include "endstone/detail/level/level.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/os.h"
#include "endstone/detail/server.h"
#include "entt/locator/locator.hpp"

namespace fs = std::filesystem;

namespace ImGui {
void Json(const nlohmann::json &json)  // NOLINT(*-no-recursion)
{
    switch (json.type()) {
    case nlohmann::json::value_t::object: {
        for (const auto &el : json.items()) {
            if (el.value().is_primitive()) {
                ImGui::Text("%s: %s", el.key().c_str(), el.value().dump().c_str());
            }
            else if (ImGui::TreeNode(el.key().c_str())) {
                ImGui::Json(el.value());
                ImGui::TreePop();
            }
        }
        break;
    }
    case nlohmann::json::value_t::array: {
        int index = 0;
        for (const auto &el : json) {
            auto key = std::to_string(index);
            if (el.is_primitive()) {
                ImGui::Text("%s: %s", key.c_str(), json.dump().c_str());
            }
            else if (ImGui::TreeNode(key.c_str())) {
                ImGui::Json(el);
                ImGui::TreePop();
            }
            ++index;
        }
        break;
    }
    default: {
        ImGui::Text("%s", json.dump().c_str());
        break;
    }
    }
}
}  // namespace ImGui

namespace endstone::detail {
namespace {
auto &gLogger = endstone::detail::LoggerFactory::getLogger("EndstoneGUI");
GLFWwindow *gWindow;

void onError(int error, const char *description)
{
    gLogger.error("GLFW Error {}: {}", error, description);
}

void onWindowClose(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_FALSE);
    glfwHideWindow(window);
}

nlohmann::json toJson(const Tag &tag)
{
    switch (tag.getId()) {
    case Tag::Type::Byte: {
        const auto &t = static_cast<const ByteTag &>(tag);
        return t.data;
    }
    case Tag::Type::Short: {
        const auto &t = static_cast<const ShortTag &>(tag);
        return t.data;
    }
    case Tag::Type::Int: {
        const auto &t = static_cast<const IntTag &>(tag);
        return t.data;
    }
    case Tag::Type::Int64: {
        const auto &t = static_cast<const Int64Tag &>(tag);
        return t.data;
    }
    case Tag::Type::Float: {
        const auto &t = static_cast<const FloatTag &>(tag);
        return t.data;
    }
    case Tag::Type::Double: {
        const auto &t = static_cast<const DoubleTag &>(tag);
        return t.data;
    }
    case Tag::Type::String: {
        const auto &t = static_cast<const StringTag &>(tag);
        return t.data;
    }
    case Tag::Type::List: {
        nlohmann::json array;
        const auto &t = static_cast<const ListTag &>(tag);
        for (auto i = 0; i < t.size(); i++) {
            array.push_back(toJson(*t.get(i)));
        }
        return array;
    }
    case Tag::Type::Compound: {
        nlohmann::json object;
        const auto &t = static_cast<const CompoundTag &>(tag);
        for (const auto &[key, value] : t) {
            object[key] = toJson(*value.get());
        }
        return object;
    }
    case Tag::Type::IntArray: {
        nlohmann::json array;
        const auto &t = static_cast<const IntArrayTag &>(tag);
        for (const auto &i : t.data) {
            array.push_back(i);
        }
        return array;
    }
    case Tag::Type::ByteArray: {
        nlohmann::json array;
        const auto &t = static_cast<const ByteArrayTag &>(tag);
        for (const auto &i : t.data) {
            array.push_back(i);
        }
        return array;
    }
    case Tag::Type::End:
    default:
        return {};
    }
}

}  // namespace

void DevTools::render()
{
    glfwSetErrorCallback(onError);
    if (!glfwInit()) {
        gLogger.error("glfwInit failed!");
        return;
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    // Create window with graphics context
    gWindow = glfwCreateWindow(1280, 720, "Endstone - DevTools", nullptr, nullptr);
    if (gWindow == nullptr) {
        gLogger.error("glfwCreateWindow failed!");
        return;
    }

    glfwSetWindowCloseCallback(gWindow, onWindowClose);
    glfwMakeContextCurrent(gWindow);
    glfwSwapInterval(1);  // Enable vsync

    if (glewInit() != GLEW_OK) {
        gLogger.error("glewInit failed!");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float prev_scale = 0.0F;
    bool server_ready = false;
    bool first_time = true;
    bool show_about_window = true;
    bool show_block_window = true;
    bool show_demo_window = true;
    auto font_path = fs::path{os::get_module_pathname()}.parent_path() / "fonts" / "JetBrainsMono-Regular.ttf";
    nlohmann::json blocks;
    nlohmann::json *json_to_save = nullptr;

    ImGui::FileBrowser save_dialog(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);

    while (!glfwWindowShouldClose(gWindow)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Handle DPI scaling
        // https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application
        float x_scale, y_scale;
        glfwGetWindowContentScale(gWindow, &x_scale, &y_scale);
        if (x_scale != prev_scale) {
            prev_scale = x_scale;
            io.Fonts->Clear();
            io.Fonts->AddFontFromFileTTF(font_path.string().c_str(), std::round(15 * x_scale));
            io.Fonts->Build();
            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
            auto style = ImGuiStyle();
            style.ScaleAllSizes(x_scale);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto dockspace_id = ImGui::DockSpaceOverViewport();
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::SeparatorText("Export");
                if (ImGui::MenuItem("Save Blocks", nullptr, false, !blocks.empty())) {
                    json_to_save = &blocks;
                    save_dialog.SetTitle("Save Blocks");
                    save_dialog.SetTypeFilters({".json"});
                    save_dialog.SetInputName("blocks.json");
                    save_dialog.Open();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::SeparatorText("Data");
                ImGui::MenuItem("Biomes");
                ImGui::MenuItem("Blocks", nullptr, &show_block_window);
                ImGui::MenuItem("Commands");
                ImGui::MenuItem("Items");
                ImGui::SeparatorText("Tools");
                ImGui::MenuItem("ImGui Demo", nullptr, &show_demo_window);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {
                    show_about_window = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (first_time) {
            first_time = false;
            ImGui::DockBuilderDockWindow("About", dockspace_id);
            ImGui::DockBuilderDockWindow("Blocks", dockspace_id);
            ImGui::DockBuilderFinish(dockspace_id);
        }

        EndstoneServer *server = nullptr;
        if (entt::locator<EndstoneServer>::has_value()) {
            server = &entt::locator<EndstoneServer>::value();
        }

        if (show_about_window) {
            showAboutWindow(&show_about_window);
        }

        if (show_block_window) {
            showBlockWindow(&show_block_window, server, blocks);
        }

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        save_dialog.Display();
        if (save_dialog.HasSelected()) {
            auto path = save_dialog.GetSelected();
            if (json_to_save && !json_to_save->empty()) {
                std::ofstream file(path);
                file << *json_to_save;
            }
            else {
                gLogger.error("Unable to save json to {}: Empty json.", path.string());
            }
            json_to_save = nullptr;
            save_dialog.ClearSelected();
        }

        // Rendering
        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(gWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(gWindow);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(gWindow);
    glfwTerminate();
}

void DevTools::show()
{
    if (gWindow) {
        glfwShowWindow(gWindow);
    }
}

void DevTools::hide()
{
    if (gWindow) {
        glfwHideWindow(gWindow);
    }
}

void DevTools::showAboutWindow(bool *open)
{
    if (!ImGui::Begin("About", open)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Welcome to Endstone DevTools!");
    ImGui::Separator();
    ImGui::Text("By all Endstone contributors.");
    ImGui::Text("Endstone is licensed under the Apache License, see LICENSE for more information.");
    ImGui::Spacing();
    ImGui::Text("Built with Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::End();
}

void DevTools::showBlockWindow(bool *open, EndstoneServer *server, nlohmann::json &json)
{
    if (!ImGui::Begin("Blocks", open)) {
        ImGui::End();
        return;
    }

    if (!server) {
        ImGui::Text("Wait for server to be ready...");
        ImGui::End();
        return;
    }

    if (server->getLevels().empty()) {
        ImGui::Text("Wait for level to be loaded...");
        ImGui::End();
        return;
    }

    if (json.empty()) {
        auto &palette = static_cast<EndstoneLevel *>(server->getLevels()[0])->getHandle().getBlockPalette();
        for (auto i = 0; i < palette.getNumBlockNetworkIds(); i++) {
            const auto &block = palette.getBlock(i);
            json.push_back({
                {"name", block.getSerializationId().get("name")->toString().c_str()},
                {"runtimeId", block.getRuntimeId()},
                {"serializationId", toJson(block.getSerializationId())},
                {"burnOdds", block.getBurnOdds()},
                {"flameOdds", block.getFlameOdds()},
                {"light", block.getLight()},
                {"lightEmission", block.getLightEmission()},
                {"explosionResistance", block.getExplosionResistance()},
                {"friction", block.getFriction()},
                {"destroySpeed", block.getDestroySpeed()},
            });
        }
    }

    ImGui::Text("Num of Blocks: %zu", json.size());

    if (ImGui::TreeNode("Block Palette")) {
        ImGui::Json(json);
        ImGui::TreePop();
    }

    ImGui::End();
}

}  // namespace endstone::detail

#endif
