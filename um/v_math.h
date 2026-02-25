#pragma once
#include <cmath>

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vector3 operator*(float factor) const { return { x * factor, y * factor, z * factor }; }
};

struct ViewMatrix {
    float matrix[4][4];
};

static bool WorldToScreen(const Vector3& worldPos, Vector2& screenPos, const ViewMatrix& viewMatrix, int width, int height) {
    float clipX = worldPos.x * viewMatrix.matrix[0][0] + worldPos.y * viewMatrix.matrix[0][1] + worldPos.z * viewMatrix.matrix[0][2] + viewMatrix.matrix[0][3];
    float clipY = worldPos.x * viewMatrix.matrix[1][0] + worldPos.y * viewMatrix.matrix[1][1] + worldPos.z * viewMatrix.matrix[1][2] + viewMatrix.matrix[1][3];
    float clipW = worldPos.x * viewMatrix.matrix[3][0] + worldPos.y * viewMatrix.matrix[3][1] + worldPos.z * viewMatrix.matrix[3][2] + viewMatrix.matrix[3][3];

    if (clipW < 0.01f)
        return false;

    Vector3 ndc;
    ndc.x = clipX / clipW;
    ndc.y = clipY / clipW;

    screenPos.x = (width / 2.0f) + (ndc.x * width / 2.0f);
    screenPos.y = (height / 2.0f) - (ndc.y * height / 2.0f);


    return true;
}
