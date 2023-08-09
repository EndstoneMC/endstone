//
// Created by Vincent on 07/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_H
#define ENDSTONE_ENDSTONE_H

#include <bitset>
#include <codecvt>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#if defined(_WIN32)
#define BEDROCK_API __declspec(dllexport)
#else
#define BEDROCK_API
#endif

#endif // ENDSTONE_ENDSTONE_H
