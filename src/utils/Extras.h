#pragma once

enum MouseInputMode {
    WINDOW_MODE,
    CAMERA_MODE
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

constexpr float PI = 3.14159265358979323846f;
constexpr static std::vector<float> CreateSphere(const float radius, const int PointAmount) {
    const int totalelements = 3 * PointAmount * PointAmount; // Adjusted for more points

    std::vector<float> points;
    points.reserve(totalelements); // Reserve memory for efficiency

    for (int i = 0; i < PointAmount; i++) {
        float phi = PI * i / (PointAmount - 1); // Ranges from 0 to PI

        for (int j = 0; j < PointAmount; j++) {
            float theta = 2 * PI * j / (PointAmount - 1); // Ranges from 0 to 2*PI

            points.push_back(radius * sin(phi) * cos(theta));
            points.push_back(radius * sin(phi) * sin(theta));
            points.push_back(radius * cos(phi));
        }
    }
    return points;
};

constexpr static std::vector<unsigned int> CreateSphereIndices(const int PointAmount) {
    const int totalIndices = 2 * (PointAmount - 1) * (PointAmount - 1) * 3;

    std::vector<unsigned int> indices;
    indices.reserve(totalIndices); // Reserve memory for efficiency

    for (int i = 0; i < PointAmount - 1; i++) {
        for (int j = 0; j < PointAmount - 1; j++) {
            int current = i * PointAmount + j;
            int next = current + PointAmount;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
    return indices;
};

static std::vector<float> CreateSphereNormals(const std::vector<float>& points, const int PointAmount) {
    std::vector<float> normals;
    normals.reserve(3 * PointAmount * PointAmount); // 3 components per normal (x, y, z)

    for (int i = 0; i < PointAmount; i++) {
        for (int j = 0; j < PointAmount; j++) {
            // Fetch the point position
            float x = points[3 * (i * PointAmount + j)];
            float y = points[3 * (i * PointAmount + j) + 1];
            float z = points[3 * (i * PointAmount + j) + 2];

            // Normalize the normal (it’s the same as the position for a unit sphere)
            float length = sqrt(x * x + y * y + z * z);
            normals.push_back(x / length); // Normalized x
            normals.push_back(y / length); // Normalized y
            normals.push_back(z / length); // Normalized z
        }
    }
    return normals;
};
