#pragma once

namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(const float translateX, const float translateY)
    {
        // TODO(student): Implement the translation matrix
        return transpose(
            glm::mat3(1, 0, translateX,
                      0, 1, translateY,
                      0, 0, 1)
        );

    }

    // Scale matrix
    inline glm::mat3 Scale(const float scaleX, const float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return transpose(
            glm::mat3(scaleX, 0, 0,
                      0, scaleY, 0,
                      0, 0, 1)
        );

    }

    // Rotate matrix
    inline glm::mat3 Rotate(const float radians)
    {
        // TODO(student): Implement the rotation matrix
        return transpose(
            glm::mat3(cos(radians), -sin(radians), 0,
                      sin(radians), cos(radians), 0,
                      0, 0 , 1)
        );

    }
}   // namespace transform2D
