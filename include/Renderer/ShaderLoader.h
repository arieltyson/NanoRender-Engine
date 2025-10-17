#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Renderer/Shader.h"

namespace nre
{
// Descriptor binds a shader stage to a filesystem path containing the source.
using ShaderFileDescriptor = std::pair<ShaderStage, std::string>;

std::vector<ShaderSource> loadShaderSources(const std::vector<ShaderFileDescriptor>& descriptors);
}
