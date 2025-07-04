#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "Matrix.h"

using Key = sf::Keyboard::Key;

class CameraController {
public:
    Vector3<float> position;
    Vector3<float> rotation;

    CameraController() : position(0, 0, 5), rotation(0, 0, 0) {}

    void handle_input() {
        const float moveSpeed = 0.1f;
        const float rotateSpeed = 0.02f;

        Matrix4 rot = Matrix4::rotate_y(rotation.y) * Matrix4::rotate_x(rotation.x);

        Vector4<float> localForward(0, 0, 1, 1);
        Vector4<float> localRight(1, 0, 0, 1);

        Vector4<float> forward_h = (rot * localForward);
        Vector4<float> right_h = (rot * localRight);

        Vector3<float> forward = Vector3<float>(forward_h.x, forward_h.y, forward_h.z).normalized();
        Vector3<float> right = Vector3<float>(right_h.x, right_h.y, right_h.z).normalized();
        Vector3<float> up = forward.cross(right);

        if (isKeyPressed(Key::W)) position = position - forward * moveSpeed;
        if (isKeyPressed(Key::S)) position = position + forward * moveSpeed;
        if (isKeyPressed(Key::A)) position = position - right * moveSpeed;
        if (isKeyPressed(Key::D)) position = position + right * moveSpeed;
        if (isKeyPressed(Key::E)) position = position - up * moveSpeed;
        if (isKeyPressed(Key::Q)) position = position + up * moveSpeed;

        if (isKeyPressed(Key::Left))  rotation.y += rotateSpeed;
        if (isKeyPressed(Key::Right)) rotation.y -= rotateSpeed;
        if (isKeyPressed(Key::Up))    rotation.x += rotateSpeed;
        if (isKeyPressed(Key::Down))  rotation.x -= rotateSpeed;
    }

    Matrix4 getViewMatrix() const {
        Matrix4 rotX = Matrix4::rotate_x(-rotation.x);
        Matrix4 rotY = Matrix4::rotate_y(-rotation.y);
        Matrix4 rotZ = Matrix4::rotate_z(-rotation.z);

        Matrix4 rotationMatrix = rotX * rotY;
        Matrix4 translation = Matrix4::translate(-position.x, -position.y, -position.z);
        return rotationMatrix * translation;
    }
};
