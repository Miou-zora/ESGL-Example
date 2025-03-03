#pragma once

#include <Windows.h>
#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>

#include "Loader.hpp"
#include "Material.hpp"
#include "utils/Mesh/Mesh.hpp"

struct Model {
    std::string shaderName;
    std::string materialName;
    Mesh mesh;
};