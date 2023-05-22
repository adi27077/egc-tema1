#include "object2D.h"

#include <vector>

#include "components/camera.h"
#include "core/engine.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    const glm::vec3 leftBottomCorner,
    const float length,
    const glm::vec3 color,
    const bool fill)
{
	const glm::vec3 corner = leftBottomCorner;

	const std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

	const auto square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(
	const ::std::string& name,
	const glm::vec3 leftBottomCorner,
	const float length,
	const glm::vec3 color)
{
	const glm::vec3 corner = leftBottomCorner;

	const std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length * 2, length / 2, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    const auto triangle = new Mesh(name);
    const std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateCircle(
	const ::std::string& name,
	const glm::vec3 center,
	const float radius,
	const glm::vec3 color,
	const int triangles)
{
    constexpr float PI = 3.1415926f;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    const float increment = 2.0f * PI / static_cast<float>(triangles);
    unsigned int index = 0;

    vertices.emplace_back(center, color);
    indices.push_back(index++);

    for (float currentAngle = 0.0f; currentAngle <= 2.0f * PI; currentAngle += increment) {
        vertices.emplace_back(center + glm::vec3(radius * cos(currentAngle), radius * sin(currentAngle), 0), color);
        indices.push_back(index++);
    }

    const auto circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    const glm::vec3 leftBottomCorner,
    const float length,
    const float width,
    const glm::vec3 color,
    const bool fill)
{
	const glm::vec3 corner = leftBottomCorner;

	const std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

	const auto square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
