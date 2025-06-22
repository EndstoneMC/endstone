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

class PyMapRenderer : public MapRenderer {
public:
    using MapRenderer::MapRenderer;

    void initialize(MapView &map) override
    {
        PYBIND11_OVERRIDE(void, MapRenderer, initialize, map);
    }

    void render(MapView &map, MapCanvas &canvas, Player &player) override
    {
        PYBIND11_OVERRIDE_PURE(void, MapRenderer, render, map, canvas, player);
    }
};

void init_map(py::module_ &m)
{
    auto view = py::class_<MapView>(m, "MapView", "Represents a map item.");

    py::class_<MapCanvas>(m, "MapCanvas",
                          "Represents a canvas for drawing to a map. Each canvas is associated with a specific "
                          "MapRenderer and represents that renderer's layer on the map.")
        .def_property_readonly("map_view", &MapCanvas::getMapView, "Get the map this canvas is attached to.",
                               py::return_value_policy::reference)
        .def("set_pixel_color", &MapCanvas::setPixelColor, py::arg("x"), py::arg("y"), py::arg("color"),
             "Draw a pixel to the canvas.\n\nIf None is used as color, then the color returned by "
             "get_base_pixel_color() is shown on the map.")
        .def("get_pixel_color", &MapCanvas::getPixelColor, py::arg("x"), py::arg("y"),
             "Get a pixel from the canvas.\n\nIf no color is set at the given position for this canvas, then None is "
             "returned and the color returned by get_base_pixel_color() is shown on the map")
        .def("get_base_pixel_color", &MapCanvas::getBasePixelColor, py::arg("x"), py::arg("y"),
             "Get a pixel from the layers below this canvas.")
        .def("draw_image", &MapCanvas::drawImage, py::arg("x"), py::arg("y"), py::arg("image"),
             "Draw an image to the map. The image will be clipped if necessary.");

    py::class_<MapRenderer, PyMapRenderer, std::shared_ptr<MapRenderer>>(m, "MapRenderer",
                                                                         "Represents a renderer for a map.")
        .def(py::init<bool>(), py::arg("is_contextual") = false,
             "Initialize the map renderer base with the given contextual status.")
        .def("initialize", &MapRenderer::initialize, py::arg("view"), "Initialize this MapRenderer for the given map.")
        .def("render", &MapRenderer::render, py::arg("map"), py::arg("canvas"), py::arg("player"),
             "Render to the given map.");

    py::enum_<MapView::Scale>(view, "Scale", "An enum representing all possible scales a map can be set to.")
        .value("CLOSEST", MapView::Scale::Closest)
        .value("CLOSE", MapView::Scale::Close)
        .value("NORMAL", MapView::Scale::Normal)
        .value("FAR", MapView::Scale::Far)
        .value("FARTHEST", MapView::Scale::Farthest);

    view.def_property_readonly("id", &MapView::getId, "Get the ID of this map item for use with MapMeta.")
        .def_property_readonly("is_virtual", &MapView::isVirtual, "Check whether this map is virtual.")
        .def_property("scale", &MapView::getScale, &MapView::setScale, "Get or set the scale of this map.")
        .def_property("center_x", &MapView::getCenterX, &MapView::setCenterX,
                      "Get or set the center X position of this map.")
        .def_property("center_z", &MapView::getCenterZ, &MapView::setCenterZ,
                      "Get or set the center Z position of this map.")
        .def_property("dimension", &MapView::getDimension, &MapView::setDimension,
                      "Get or set the dimension that this map is associated with.", py::return_value_policy::reference)
        .def_property_readonly("renderers", &MapView::getRenderers,
                               "Get a copied list of MapRenderers currently in effect.",
                               py::return_value_policy::reference_internal)
        .def("add_renderer", &MapView::addRenderer, py::arg("renderer"), "Add a renderer to this map.")
        .def("remove_renderer", &MapView::removeRenderer, py::arg("renderer"), "Remove a renderer from this map.")
        .def_property("is_unlimited_tracking", &MapView::isUnlimitedTracking, &MapView::setUnlimitedTracking,
                      "Whether the map will show a smaller position cursor (true), or no position cursor (false) when "
                      "cursor is outside of map's range.")
        .def_property("locked", &MapView::isLocked, &MapView::setLocked,
                      "Whether the map is locked or not. A locked map may not be explored further.");
}

}  // namespace endstone::python
