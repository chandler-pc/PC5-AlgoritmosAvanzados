#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Matrix.h"

using Key = sf::Keyboard::Key;

class CameraController {
public:
    Vector3 position;
    float yaw;
    float pitch; 

    CameraController() : position(0, 0, 0), yaw(0), pitch(0) {}

    void handle_input() {
        const float moveSpeed = 0.1f;
        const float rotateSpeed = 0.02f;

        Vector3 forward = {
            std::sin(yaw) * std::cos(pitch),
            std::sin(pitch),
            std::cos(yaw) * std::cos(pitch)
        };

        Vector3 right = {
            std::cos(yaw),
            0,
            -std::sin(yaw)
        };

        forward = forward.normalized();
        right = right.normalized();

        if (isKeyPressed(Key::W)) position = position + forward * moveSpeed;
        if (isKeyPressed(Key::S)) position = position - forward * moveSpeed;
        if (isKeyPressed(Key::A)) position = position - right * moveSpeed;
        if (isKeyPressed(Key::D)) position = position + right * moveSpeed;
        if (isKeyPressed(Key::Q)) position.y += moveSpeed;
        if (isKeyPressed(Key::E)) position.y -= moveSpeed;

        if (isKeyPressed(Key::Left)) yaw -= rotateSpeed;
        if (isKeyPressed(Key::Right)) yaw += rotateSpeed;
        if (isKeyPressed(Key::Up)) pitch += rotateSpeed;
        if (isKeyPressed(Key::Down)) pitch -= rotateSpeed;
    }


    Matrix4 getViewMatrix() const {
        Matrix4 rotY = Matrix4::rotate_y(-yaw);
        Matrix4 rotX = Matrix4::rotate_x(-pitch);
        Matrix4 rotation = rotY * rotX;

        Matrix4 translation = Matrix4::translate(-position.x, -position.y, -position.z);
        return rotation * translation ;
    }
};
