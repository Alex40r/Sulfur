#pragma once

#include "Definitions.hpp"

/* ---- ---- ---- ---- */

#include "Generics/List.hpp"

/* ---- ---- ---- ---- */

#include "Objects/Handle.hpp"
#include "Objects/Object.hpp"
#include "Objects/ParentHandle.hpp"

/* ---- ---- ---- ---- */

namespace Utils {
bool ReadFile(const std::string& filename, List<uint8>& output);
}