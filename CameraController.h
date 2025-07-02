#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Matrix.h"

using Key = sf::Keyboard::Key;

class CameraController {
public:
    Vector3 position;
    Vector3 rotation;

    CameraController() : position(0, 0, 0), rotation(0, 0, 0) {}

    void handle_input() {
        const float moveSpeed = 0.1f;
        const float rotateSpeed = 0.02f;

        Vector3 forward = {
            std::sin(rotation.y) * std::cos(rotation.x),
            std::sin(rotation.x),
            std::cos(rotation.y) * std::cos(rotation.x)
        };

        Vector3 right = {
            std::cos(rotation.y),
            0,
            -std::sin(rotation.y)
        };

        forward = forward.normalized();
        right = right.normalized();

        if (isKeyPressed(Key::W)) position = position + forward * moveSpeed;
        if (isKeyPressed(Key::S)) position = position - forward * moveSpeed;
        if (isKeyPressed(Key::A)) position = position + right * moveSpeed;
        if (isKeyPressed(Key::D)) position = position - right * moveSpeed;
        if (isKeyPressed(Key::Q)) position.y += moveSpeed;
        if (isKeyPressed(Key::E)) position.y -= moveSpeed;

        if (isKeyPressed(Key::Left))  rotation.y += rotateSpeed;
        if (isKeyPressed(Key::Right)) rotation.y -= rotateSpeed;
        if (isKeyPressed(Key::Up))    rotation.x += rotateSpeed;
        if (isKeyPressed(Key::Down))  rotation.x -= rotateSpeed;
    }

    Matrix4 getViewMatrix() const {
        Matrix4 rotX = Matrix4::rotate_x(-rotation.x);
        Matrix4 rotY = Matrix4::rotate_y(-rotation.y);
        Matrix4 rotZ = Matrix4::rotate_z(-rotation.z);

        Matrix4 rotationMatrix = rotZ * rotY * rotX;
        Matrix4 translation = Matrix4::translate(-position.x, -position.y, -position.z);
        return rotationMatrix * translation;
    }
};
