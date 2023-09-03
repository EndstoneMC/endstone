//
// Created by Vincent on 30/08/2023.
//
#include "endstone/chat_color.h"
#include "pybind.h"

void def_chat_color(py::module &m)
{
    py::enum_<ChatColor>(m, "ChatColor")
        .value("BLACK", ChatColor::Black)
        .value("DARK_BLUE", ChatColor::DarkBlue)
        .value("DARK_GREEN", ChatColor::DarkGreen)
        .value("DARK_AQUA", ChatColor::DarkAqua)
        .value("DARK_RED", ChatColor::DarkRed)
        .value("DARK_PURPLE", ChatColor::DarkPurple)
        .value("GOLD", ChatColor::Gold)
        .value("GRAY", ChatColor::Gray)
        .value("DARK_GRAY", ChatColor::DarkGray)
        .value("BLUE", ChatColor::Blue)
        .value("GREEN", ChatColor::Green)
        .value("AQUA", ChatColor::Aqua)
        .value("RED", ChatColor::Red)
        .value("LIGHT_PURPLE", ChatColor::LightPurple)
        .value("YELLOW", ChatColor::Yellow)
        .value("WHITE", ChatColor::White)
        .value("MINECOIN_GOLD", ChatColor::MinecoinGold)
        .value("MATERIAL_QUARTZ", ChatColor::MaterialQuartz)
        .value("MATERIAL_IRON", ChatColor::MaterialIron)
        .value("MATERIAL_NETHERITE", ChatColor::MaterialNetherite)
        .value("MAGIC", ChatColor::Magic)
        .value("BOLD", ChatColor::Bold)
        .value("MATERIAL_REDSTONE", ChatColor::MaterialRedstone)
        .value("MATERIAL_COPPER", ChatColor::MaterialCopper)
        .value("ITALIC", ChatColor::Italic)
        .value("MATERIAL_GOLD", ChatColor::MaterialGold)
        .value("MATERIAL_EMERALD", ChatColor::MaterialEmerald)
        .value("RESET", ChatColor::Reset)
        .value("MATERIAL_DIAMOND", ChatColor::MaterialDiamond)
        .value("MATERIAL_LAPIS", ChatColor::MaterialLapis)
        .value("MATERIAL_AMETHYST", ChatColor::MaterialAmethyst)
        .export_values()
        .def(py::self + py::str())
        .def(py::str() + py::self)
        .def("__str__", &ChatColors::toString, py::prepend{});
}
