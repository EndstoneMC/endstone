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

#include "endstone/detail/devtools/devtools.h"

#include <imgui.h>
#include <imgui_file_browser.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "endstone/color_format.h"
#include "endstone/detail/devtools/imgui/imgui_json.h"
#include "endstone/detail/devtools/vanilla_data.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/os.h"

namespace fs = std::filesystem;

namespace endstone::detail::devtools {

namespace {
auto &gLogger = LoggerFactory::getLogger("DevTools");
GLFWwindow *gWindow;

void onError(int error, const char *description)
{
    gLogger.error("GLFW Error {}: {}", error, description);
}

void onWindowClose(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_FALSE);
    hide();
}
}  // namespace

// Forward declaration
void showBlockWindow(bool *open);
void showItemWindow(bool *open);

void render()
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
    {
        float x_scale, y_scale;
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(monitor, &x_scale, &y_scale);
        gWindow = glfwCreateWindow(x_scale * 720, y_scale * 480, "Endstone - DevTools", nullptr, nullptr);
    }

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

    while (!glfwWindowShouldClose(gWindow)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Handle DPI scaling
        // https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application
        static float prev_scale = 0.0F;
        float x_scale, y_scale;
        glfwGetWindowContentScale(gWindow, &x_scale, &y_scale);
        if (x_scale != prev_scale) {
            prev_scale = x_scale;
            auto font_path = fs::path{os::get_module_pathname()}.parent_path() / "fonts" / "JetBrainsMono-Regular.ttf";
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
        static std::once_flag first_time;
        std::call_once(first_time, [&] {
            ImGui::DockBuilderDockWindow("About", dockspace_id);
            ImGui::DockBuilderDockWindow("Blocks", dockspace_id);
            ImGui::DockBuilderDockWindow("Items", dockspace_id);
            ImGui::DockBuilderFinish(dockspace_id);
        });

        static bool show_block_window = true;
        static bool show_item_window = true;

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::SeparatorText("Vanilla Data");
                ImGui::MenuItem("Blocks", nullptr, &show_block_window);
                ImGui::MenuItem("Items", nullptr, &show_item_window);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_block_window) {
            showBlockWindow(&show_block_window);
        }
        if (show_item_window) {
            showItemWindow(&show_item_window);
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

void show()
{
    if (gWindow) {
        glfwShowWindow(gWindow);
    }
}

void hide()
{
    if (gWindow) {
        glfwHideWindow(gWindow);
        gLogger.info(ColorFormat::DarkAqua +
                     "DevTools has been hidden. Use the command /devtools to display it again.");
    }
}

void showBlockWindow(bool *open)
{
    if (!ImGui::Begin("Blocks", open)) {
        ImGui::End();
        return;
    }

    auto *data = VanillaData::get();
    if (!data) {
        ImGui::Text("Wait for server to be ready...");
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Block Types", data->block_types.size()).c_str())) {
        ImGui::Json(data->block_types);
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Block States", data->block_states.size()).c_str())) {
        ImGui::Json(data->block_states);
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Block Tags", data->block_tags.size()).c_str())) {
        ImGui::Json(data->block_tags);
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Materials", data->materials.size()).c_str())) {
        ImGui::Json(data->materials);
    }
    ImGui::End();
}

void showItemWindow(bool *open)
{
    if (!ImGui::Begin("Items", open)) {
        ImGui::End();
        return;
    }

    auto *data = VanillaData::get();
    if (!data) {
        ImGui::Text("Wait for server to be ready...");
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Items", data->items.size()).c_str())) {
        ImGui::Json(data->items);
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Item Tags", data->item_tags.size()).c_str())) {
        ImGui::Json(data->item_tags);
    }
    ImGui::End();
}

}  // namespace endstone::detail::devtools
