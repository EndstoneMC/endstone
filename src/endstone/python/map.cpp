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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

class PyMapRenderer : public MapRenderer, public py::trampoline_self_life_support {
public:
    using MapRenderer::MapRenderer;

    void initialize(MapView &map) override
    {
        PYBIND11_OVERRIDE(void, MapRenderer, initialize, std::ref(map));
    }

    void render(MapView &map, MapCanvas &canvas, Player &player) override
    {
        PYBIND11_OVERRIDE_PURE(void, MapRenderer, render, std::ref(map), std::ref(canvas), std::ref(player));
    }
};

void init_map(py::module_ &m)
{
    auto cursor = py::class_<MapCursor>(m, "MapCursor", "Represents a cursor on a map.");

    py::native_enum<MapCursor::Type>(cursor, "Type", "enum.Enum", "Represents the standard types of map cursors.")
        .value("PLAYER", MapCursor::Type::Player)
        .value("FRAME", MapCursor::Type::Frame)
        .value("RED_MARKER", MapCursor::Type::RedMarker)
        .value("BLUE_MARKER", MapCursor::Type::BlueMarker)
        .value("TARGET_X", MapCursor::Type::TargetX)
        .value("TARGET_POINT", MapCursor::Type::TargetPoint)
        .value("PLAYER_OFF_MAP", MapCursor::Type::PlayerOffMap)
        .value("SIGN_MARKER", MapCursor::Type::SignMarker)
        .value("PINK_MARKER", MapCursor::Type::PinkMarker)
        .value("ORANGE_MARKER", MapCursor::Type::OrangeMarker)
        .value("YELLOW_MARKER", MapCursor::Type::YellowMarker)
        .value("CYAN_MARKER", MapCursor::Type::CyanMarker)
        .value("GREEN_POINT", MapCursor::Type::GreenPoint)
        .value("PLAYER_OFF_LIMITS", MapCursor::Type::PlayerOffLimits)
        .value("MANSION", MapCursor::Type::Mansion)
        .value("MONUMENT", MapCursor::Type::Monument)
        .value("VILLAGE_DESERT", MapCursor::Type::VillageDesert)
        .value("VILLAGE_PLAINS", MapCursor::Type::VillagePlains)
        .value("VILLAGE_SAVANNA", MapCursor::Type::VillageSavanna)
        .value("VILLAGE_SNOWY", MapCursor::Type::VillageSnowy)
        .value("VILLAGE_TAIGA", MapCursor::Type::VillageTaiga)
        .value("JUNGLE_TEMPLE", MapCursor::Type::JungleTemple)
        .value("SWAMP_HUT", MapCursor::Type::SwampHut)
        .value("TRIAL_CHAMBERS", MapCursor::Type::TrialChambers)
        .finalize();

    cursor
        .def(py::init<std::int8_t, std::int8_t, std::int8_t, MapCursor::Type, bool, std::string>(), py::arg("x"),
             py::arg("y"), py::arg("direction"), py::arg("type"), py::arg("visible"), py::arg("caption") = "")
        .def_property("x", &MapCursor::getX, &MapCursor::setX, "The X position of this cursor.")
        .def_property("y", &MapCursor::getY, &MapCursor::setY, "The Y position of this cursor.")
        .def_property("direction", &MapCursor::getDirection, &MapCursor::setDirection,
                      "The facing of the cursor, from 0 to 15.")
        .def_property("type", &MapCursor::getType, &MapCursor::setType,
                      "The type (color/style) of this map cursor.")
        .def_property("is_visible", &MapCursor::isVisible, &MapCursor::setVisible,
                      "The visibility status of this cursor.")
        .def_property("caption", &MapCursor::getCaption, &MapCursor::setCaption, "The caption on this cursor.");

    auto view = py::class_<MapView>(m, "MapView", "Represents a map item.");

    py::class_<MapCanvas>(m, "MapCanvas", R"doc(
    Represents a canvas for drawing to a map.

    Each canvas is associated with a specific MapRenderer and represents that
    renderer's layer on the map.
)doc")
        .def_property_readonly("map_view", &MapCanvas::getMapView, "The MapView this canvas is attached to.",
                               py::return_value_policy::reference)
        .def_property("cursors", &MapCanvas::getCursors, &MapCanvas::setCursors,
                      "The cursors associated with this canvas.")
        .def("set_pixel_color", &MapCanvas::setPixelColor, py::arg("x"), py::arg("y"), py::arg("color"), R"doc(
    Draw a pixel to the canvas.

    Args:
        x: The x coordinate, from 0 to 127.
        y: The y coordinate, from 0 to 127.
        color: The color.
)doc")
        .def("get_pixel_color", &MapCanvas::getPixelColor, py::arg("x"), py::arg("y"), R"doc(
    Get a pixel from the canvas.

    Args:
        x: The x coordinate, from 0 to 127.
        y: The y coordinate, from 0 to 127.

    Returns:
        The color.
)doc")
        .def("set_pixel", &MapCanvas::setPixel, py::arg("x"), py::arg("y"), py::arg("color"), R"doc(
    Draw a pixel to the canvas.

    Args:
        x: The x coordinate, from 0 to 127.
        y: The y coordinate, from 0 to 127.
        color: The color.
)doc")
        .def("get_pixel", &MapCanvas::getPixel, py::arg("x"), py::arg("y"), R"doc(
    Get a pixel from the canvas.

    Args:
        x: The x coordinate, from 0 to 127.
        y: The y coordinate, from 0 to 127.

    Returns:
        The color.
)doc")
        .def("draw_image", &MapCanvas::drawImage, py::arg("x"), py::arg("y"), py::arg("image"), R"doc(
    Draw an image to the map. The image will be clipped if necessary.

    Args:
        x: The x coordinate of the image.
        y: The y coordinate of the image.
        image: The Image to draw.
)doc");

    py::class_<MapRenderer, PyMapRenderer, py::smart_holder>(m, "MapRenderer", "Represents a renderer for a map.")
        .def(py::init<bool>(), py::arg("is_contextual") = false,
             "Initialize the map renderer base with the given contextual status.")
        .def("initialize", &MapRenderer::initialize, py::arg("view"), R"doc(
    Initialize this MapRenderer for the given map.

    Args:
        view: The MapView being initialized.
)doc")
        .def("render", &MapRenderer::render, py::arg("view"), py::arg("canvas"), py::arg("player"), R"doc(
    Render to the given map.

    Args:
        view: The MapView being rendered to.
        canvas: The canvas to use for rendering.
        player: The player who triggered the rendering.
)doc");

    py::native_enum<MapView::Scale>(view, "Scale", "enum.IntEnum",
                                    "An enum representing all possible scales a map can be set to.")
        .value("CLOSEST", MapView::Scale::Closest)
        .value("CLOSE", MapView::Scale::Close)
        .value("NORMAL", MapView::Scale::Normal)
        .value("FAR", MapView::Scale::Far)
        .value("FARTHEST", MapView::Scale::Farthest)
        .export_values()
        .finalize();

    view.def_property_readonly("id", &MapView::getId, "The unique ID of this map item for use with MapMeta.")
        .def_property_readonly("is_virtual", &MapView::isVirtual, R"doc(
    Whether this map is virtual.

    A map is virtual if its lowermost MapRenderer is plugin-provided.
)doc")
        .def_property("scale", &MapView::getScale, &MapView::setScale, "The scale of this map.")
        .def_property("center_x", &MapView::getCenterX, &MapView::setCenterX, "The center X position of this map.")
        .def_property("center_z", &MapView::getCenterZ, &MapView::setCenterZ, "The center Z position of this map.")
        .def_property("dimension", &MapView::getDimension, &MapView::setDimension,
                      "The dimension that this map is associated with.", py::return_value_policy::reference)
        .def_property_readonly("renderers", &MapView::getRenderers, "A list of MapRenderers currently in effect.",
                               py::return_value_policy::reference_internal)
        .def("add_renderer", &MapView::addRenderer, py::arg("renderer"), R"doc(
    Add a renderer to this map.

    Args:
        renderer: The MapRenderer to add.
)doc")
        .def("remove_renderer", &MapView::removeRenderer, py::arg("renderer"), R"doc(
    Remove a renderer from this map.

    Args:
        renderer: The MapRenderer to remove.

    Returns:
        True if the renderer was successfully removed.
)doc")
        .def_property("is_unlimited_tracking", &MapView::isUnlimitedTracking, &MapView::setUnlimitedTracking, R"doc(
    Whether the map will show a smaller position cursor (True), or no position cursor (False) when cursor is
    outside of map's range.
)doc")
        .def_property("locked", &MapView::isLocked, &MapView::setLocked,
                      "Whether the map is locked or not. A locked map may not be explored further.");
}

}  // namespace endstone::python
