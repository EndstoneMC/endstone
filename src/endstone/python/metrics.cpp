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

namespace {
class PyCustomChart : public CustomChart, public py::trampoline_self_life_support {
public:
    using CustomChart::CustomChart;

    std::optional<JsonObject> getChartData() override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<JsonObject>, CustomChart, "get_chart_data", getChartData);
    }
};
}  // namespace

void init_metrics(py::module_ &m)
{
    py::classh<CustomChart, PyCustomChart>(m, "CustomChart", R"doc(
    Represents a custom chart.

    Subclass this to report a chart shape bStats supports but Endstone does not wrap.
)doc")
        .def(py::init<std::string>(), py::arg("chart_id"), R"doc(
    Creates a chart with the given bStats chart id.

    Args:
        chart_id: The id of the chart.

    Raises:
        ValueError: If `chart_id` is empty.
)doc")
        .def_property_readonly("chart_id", &CustomChart::getChartId, "The id of this chart.")
        .def("get_chart_data", &CustomChart::getChartData, R"doc(
    Gets the data for this chart.

    This is called on the primary server thread. Returning `None` omits the chart from the submission.

    Returns:
        The chart data, or `None` to skip this chart.
)doc");

    py::classh<SimplePie, CustomChart>(m, "SimplePie", "A bStats pie chart with a single value.")
        .def(py::init<std::string, SimplePie::Callback>(), py::arg("chart_id"), py::arg("get_value"), R"doc(
    Creates a simple pie chart.

    Args:
        chart_id: The id of the chart.
        get_value: The callable which is used to request the chart data.
)doc");

    py::classh<AdvancedPie, CustomChart>(m, "AdvancedPie", "A bStats pie chart with named values.")
        .def(py::init<std::string, AdvancedPie::Callback>(), py::arg("chart_id"), py::arg("get_values"), R"doc(
    Creates an advanced pie chart.

    Args:
        chart_id: The id of the chart.
        get_values: The callable which is used to request the chart data.
)doc");

    py::classh<DrilldownPie, CustomChart>(m, "DrilldownPie", "A bStats pie chart with grouped named values.")
        .def(py::init<std::string, DrilldownPie::Callback>(), py::arg("chart_id"), py::arg("get_values"), R"doc(
    Creates a drilldown pie chart.

    Args:
        chart_id: The id of the chart.
        get_values: The callable which is used to request the chart data.
)doc");

    py::classh<SimpleBarChart, CustomChart>(m, "SimpleBarChart", "A bStats bar chart with a single bar per category.")
        .def(py::init<std::string, SimpleBarChart::Callback>(), py::arg("chart_id"), py::arg("get_values"), R"doc(
    Creates a simple bar chart.

    Args:
        chart_id: The id of the chart.
        get_values: The callable which is used to request the chart data.
)doc");

    py::classh<AdvancedBarChart, CustomChart>(m, "AdvancedBarChart",
                                              "A bStats bar chart with several bars per category.")
        .def(py::init<std::string, AdvancedBarChart::Callback>(), py::arg("chart_id"), py::arg("get_values"), R"doc(
    Creates an advanced bar chart.

    Args:
        chart_id: The id of the chart.
        get_values: The callable which is used to request the chart data.
)doc");

    py::classh<SingleLineChart, CustomChart>(m, "SingleLineChart", "A bStats line chart with a single line.")
        .def(py::init<std::string, SingleLineChart::Callback>(), py::arg("chart_id"), py::arg("get_value"), R"doc(
    Creates a single line chart.

    Args:
        chart_id: The id of the chart.
        get_value: The callable which is used to request the chart data.
)doc");

    py::classh<MultiLineChart, CustomChart>(m, "MultiLineChart", "A bStats line chart with a line per named value.")
        .def(py::init<std::string, MultiLineChart::Callback>(), py::arg("chart_id"), py::arg("get_values"), R"doc(
    Creates a multi line chart.

    Args:
        chart_id: The id of the chart.
        get_values: The callable which is used to request the chart data.
)doc");
}
}  // namespace endstone::python
