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
#include <fstream>
#include <mutex>
#include <utility>
#include <zstr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "bedrock/util/string_byte_output.h"
#include "bedrock/nbt/nbt_io.h"
#include "endstone/color_format.h"
#include "endstone/detail/devtools/imgui/imgui_json.h"
#include "endstone/detail/devtools/vanilla_data.h"
#include "endstone/detail/logger_factory.h"
#include "endstone/detail/os.h"
#include "endstone/endstone.h"

namespace fs = std::filesystem;

namespace endstone::detail::devtools {

namespace {
auto &gLogger = LoggerFactory::getLogger("DevTools");
GLFWwindow *gWindow = nullptr;
ImGui::FileBrowser *gFileBrowser = nullptr;

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
void showAboutWindow(bool *open);
void showBlockWindow(bool *open);
void showItemWindow(bool *open);
void showRecipeWindow(bool *open);
void showBiomeWindow(bool *open);
void openFileBrowser(std::string title, const std::string &input_name);

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
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create window with graphics context
    {
        float x_scale, y_scale;
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(monitor, &x_scale, &y_scale);
        gWindow = glfwCreateWindow(x_scale * 640, y_scale * 360, "Endstone - DevTools", nullptr, nullptr);
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

    // File browser
    gFileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
    auto data_dir = fs::current_path() / "data";
    if (!exists(data_dir)) {
        create_directories(data_dir);
    }
    gFileBrowser->SetPwd(data_dir);

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
            ImGui::DockBuilderDockWindow("Blocks", dockspace_id);
            ImGui::DockBuilderDockWindow("Items", dockspace_id);
            ImGui::DockBuilderDockWindow("Recipes", dockspace_id);
            ImGui::DockBuilderDockWindow("Biomes", dockspace_id);
            ImGui::DockBuilderFinish(dockspace_id);
        });

        static bool show_about_window = false;
        static bool show_block_window = true;
        static bool show_item_window = true;
        static bool show_recipe_window = true;
        static bool show_biome_window = true;
        static std::variant<std::monostate, nlohmann::json, CompoundTag> file_to_save;

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::SeparatorText("Export");
                auto *data = VanillaData::get();
                if (ImGui::BeginMenu("JSON files", data != nullptr)) {
                    if (ImGui::MenuItem("Block Types")) {
                        file_to_save = data->block_types;
                        openFileBrowser("Save Block Types", "block_types.json");
                    }
                    if (ImGui::MenuItem("Block States")) {
                        file_to_save = data->block_states;
                        openFileBrowser("Save Block States", "block_states.json");
                    }
                    if (ImGui::MenuItem("Block Tags")) {
                        file_to_save = data->block_tags;
                        openFileBrowser("Save Block Tags", "block_tags.json");
                    }
                    if (ImGui::MenuItem("Materials")) {
                        file_to_save = data->materials;
                        openFileBrowser("Save Materials", "materials.json");
                    }
                    if (ImGui::MenuItem("Items")) {
                        file_to_save = data->items;
                        openFileBrowser("Save Items", "items.json");
                    }
                    if (ImGui::MenuItem("Item Tags")) {
                        file_to_save = data->item_tags;
                        openFileBrowser("Save Item Tags", "item_tags.json");
                    }
                    if (ImGui::MenuItem("Recipes")) {
                        file_to_save = nlohmann::json{
                            {"shapeless", data->recipes.shapeless},
                            {"shaped", data->recipes.shaped},
                            {"furnace", data->recipes.furnace},
                            {"furnaceAux", data->recipes.furnace_aux},
                            {"multi", data->recipes.multi},
                            {"shulkerBox", data->recipes.shulker_box},
                            {"shapelessChemistry", data->recipes.shapeless_chemistry},
                            {"shapedChemistry", data->recipes.shaped_chemistry},
                            {"smithingTransform", data->recipes.smithing_transform},
                            {"smithingTrim", data->recipes.smithing_trim},
                            {"potionMixes", data->recipes.potion_mixes},
                            {"containerMixes", data->recipes.container_mixes},
                            {"materialReducer", data->recipes.material_reducer},
                        };
                        openFileBrowser("Save Recipes", "recipes.json");
                    }if (ImGui::MenuItem("Biomes")) {
                        file_to_save = data->biomes;
                        openFileBrowser("Save Biomes", "biomes.json");
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("NBT files")) {
                    if (ImGui::MenuItem("Block Palette", nullptr, false, data != nullptr)) {
                        file_to_save = CompoundTag();
                        std::get<CompoundTag>(file_to_save).put("blocks", data->block_palette.copy());
                        openFileBrowser("Save Block Palette", "block_palette.nbt");
                    }
                    if (ImGui::MenuItem("Creative Items")) {
                        file_to_save = CompoundTag();
                        std::get<CompoundTag>(file_to_save).put("items", data->creative_items.copy());
                        openFileBrowser("Save Creative Items", "creative_items.nbt");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::SeparatorText("Vanilla Data");
                ImGui::MenuItem("Blocks", nullptr, &show_block_window);
                ImGui::MenuItem("Items", nullptr, &show_item_window);
                ImGui::MenuItem("Recipes", nullptr, &show_recipe_window);
                ImGui::MenuItem("Biomes", nullptr, &show_biome_window);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("About", nullptr, &show_about_window);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_about_window) {
            showAboutWindow(&show_about_window);
        }
        if (show_block_window) {
            showBlockWindow(&show_block_window);
        }
        if (show_item_window) {
            showItemWindow(&show_item_window);
        }
        if (show_recipe_window) {
            showRecipeWindow(&show_recipe_window);
        }
        if (show_biome_window) {
            showBiomeWindow(&show_biome_window);
        }

        // Handle file browser
        gFileBrowser->Display();
        if (gFileBrowser->HasSelected()) {
            auto path = gFileBrowser->GetSelected();
            std::visit(overloaded{[&](std::monostate) { gLogger.error("Unable to save to {}: Empty.", path.string()); },
                                  [&](const nlohmann::json &arg) {
                                      std::ofstream file(path);
                                      file << arg;
                                  },
                                  [&](const CompoundTag &arg) {
                                      BigEndianStringByteOutput output;
                                      NbtIo::writeNamedTag("", arg, output);
                                      zstr::ofstream file(path.string(), std::ios::out | std::ios::binary);
                                      file << output.buffer;
                                  }},
                       file_to_save);
            file_to_save = std::monostate();
            gFileBrowser->ClearSelected();
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
    delete gFileBrowser;
    gFileBrowser = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(gWindow);
    gWindow = nullptr;
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

void showAboutWindow(bool *open)
{
    if (!ImGui::Begin("About", open)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Endstone DevTools");
    ImGui::Text("v%s", ENDSTONE_VERSION);
    ImGui::Separator();
    ImGui::Text("By all Endstone contributors.");
    ImGui::Text("Endstone is licensed under the Apache License, see LICENSE for more information.");
    ImGui::Spacing();
    ImGui::Text("Built with Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::End();
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

    if (ImGui::CollapsingHeader("Block Palette")) {
        ImGui::Json(NbtIo::toJson(data->block_palette));
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

    if (ImGui::CollapsingHeader(fmt::format("{} Creative Items", data->creative_items.size()).c_str())) {
        ImGui::Json(NbtIo::toJson(data->creative_items));
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Item Tags", data->item_tags.size()).c_str())) {
        ImGui::Json(data->item_tags);
    }
    ImGui::End();
}

void showRecipeWindow(bool *open)
{
    if (!ImGui::Begin("Recipes", open)) {
        ImGui::End();
        return;
    }

    auto *data = VanillaData::get();
    if (!data) {
        ImGui::Text("Wait for server to be ready...");
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Shapeless Recipes", data->recipes.shapeless.size()).c_str())) {
        ImGui::Json(data->recipes.shapeless);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Shaped Recipes", data->recipes.shaped.size()).c_str())) {
        ImGui::Json(data->recipes.shaped);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Furnace Recipes", data->recipes.furnace.size()).c_str())) {
        ImGui::Json(data->recipes.furnace);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Furnace Aux Recipes", data->recipes.furnace_aux.size()).c_str())) {
        ImGui::Json(data->recipes.furnace_aux);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Multi Recipes", data->recipes.multi.size()).c_str())) {
        ImGui::Json(data->recipes.multi);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Shulker Box Recipes", data->recipes.shulker_box.size()).c_str())) {
        ImGui::Json(data->recipes.shulker_box);
    }
    if (ImGui::CollapsingHeader(
            fmt::format("{} Shapeless Chemistry Recipes", data->recipes.shapeless_chemistry.size()).c_str())) {
        ImGui::Json(data->recipes.shapeless_chemistry);
    }
    if (ImGui::CollapsingHeader(
            fmt::format("{} Shaped Chemistry Recipes", data->recipes.shaped_chemistry.size()).c_str())) {
        ImGui::Json(data->recipes.shaped_chemistry);
    }
    if (ImGui::CollapsingHeader(
            fmt::format("{} Smithing Transform Recipes", data->recipes.smithing_transform.size()).c_str())) {
        ImGui::Json(data->recipes.smithing_transform);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Smithing Trim Recipes", data->recipes.smithing_trim.size()).c_str())) {
        ImGui::Json(data->recipes.smithing_trim);
    }
    if (ImGui::CollapsingHeader(fmt::format("{} Potion Mix Recipes", data->recipes.potion_mixes.size()).c_str())) {
        ImGui::Json(data->recipes.potion_mixes);
    }
    if (ImGui::CollapsingHeader(
            fmt::format("{} Container Mix Recipes", data->recipes.container_mixes.size()).c_str())) {
        ImGui::Json(data->recipes.container_mixes);
    }
    if (ImGui::CollapsingHeader(
            fmt::format("{} Material Reducer Recipes", data->recipes.material_reducer.size()).c_str())) {
        ImGui::Json(data->recipes.material_reducer);
    }

    ImGui::End();
}

void showBiomeWindow(bool *open)
{
    if (!ImGui::Begin("Biomes", open)) {
        ImGui::End();
        return;
    }

    auto *data = VanillaData::get();
    if (!data) {
        ImGui::Text("Wait for server to be ready...");
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(fmt::format("{} Biomes", data->biomes.size()).c_str())) {
        ImGui::Json(data->biomes);
    }

    ImGui::End();
}

void openFileBrowser(std::string title, const std::string &input_name)
{
    if (!gFileBrowser) {
        return;
    }

    gFileBrowser->Open();
    gFileBrowser->SetTitle(std::move(title));
    gFileBrowser->SetTypeFilters({fs::path(input_name).extension().string()});
    gFileBrowser->SetInputName(input_name);
}

}  // namespace endstone::detail::devtools
