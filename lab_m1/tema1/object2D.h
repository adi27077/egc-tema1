#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    // Create triangle with given bottom left corner, length and color
    Mesh* CreateTriangle(const ::std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);

    // Create circle with given center, radius and color
    Mesh* CreateCircle(const ::std::string& name, glm::vec3 center, float radius, glm::vec3 color, int triangles = 360);

    // Create rectangle with given bottom left corner, length, width and color
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);
}
