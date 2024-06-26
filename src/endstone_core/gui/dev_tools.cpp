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

#include "endstone/detail/gui/dev_tools.h"

#include <imgui.h>

#include <cstdio>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "endstone/detail/gui/imgui_impl_glfw.h"
#include "endstone/detail/gui/imgui_impl_opengl3.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/os.h"

namespace endstone::detail {

namespace {

auto &gLogger = endstone::detail::LoggerFactory::getLogger("EndstoneGUI");
bool gShow = false;

void onError(int error, const char *description)
{
    gLogger.error("GLFW Error {}: {}", error, description);
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
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(100, 100, "Endstone - DevTools", nullptr, nullptr);
    if (window == nullptr) {
        gLogger.error("glfwCreateWindow failed!");
        return;
    }

    glfwMakeContextCurrent(window);
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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Load Fonts
    auto font_path = std::filesystem::path{os::get_module_pathname()}.parent_path() / "fonts" / "DroidSans.ttf";
    io.Fonts->AddFontFromFileTTF(font_path.string().c_str(), std::round(13 * 1.5F));
    ImGui::GetStyle().ScaleAllSizes(1.5F);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (gShow) {
            ImGui::ShowDemoWindow(&gShow);
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void DevTools::show()
{
    gShow = true;
}

void DevTools::hide()
{
    gShow = false;
}

}  // namespace endstone::detail
