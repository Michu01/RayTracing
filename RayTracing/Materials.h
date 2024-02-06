#pragma once

#include <variant>

#include "LambertianMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"

using Material = std::variant<LambertianMaterial, MetalMaterial, DielectricMaterial>;