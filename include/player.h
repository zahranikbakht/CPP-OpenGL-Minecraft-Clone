//
// Created by Ewan on 8/8/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "camera.h"
#include "entity.h"

class Engine;

enum VelocityComponent {
    X,
    Y,
    Z
};

class Player : public Entity {
public:
    Player();

    /// Returns the player's current view matrix
    glm::mat4 getPlayerView() { return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up); }

    /// Updates the player (movement physics and collision physics)
    void update(Engine *engine, float dt);

    /// Processes the player's inputs
    void processInput(GLFWwindow *window, float dt);

    /// Manipulates the camera's view based on the player's mouse input
    void look(GLFWwindow *window, double xpos, double ypos);

private:
    Camera camera;
    bool onGround = true;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};

    /// Jumps
    void jump();

    /** Checks the player's collision based on a given axis
     *
     * @param engine the engine
     * @param comp the velocity component/axis to verify collisions
     */
    void collide(Engine *engine, VelocityComponent comp);

    /** Checks if the player is on the ground (gravity)
     *
     * @param engine
     */
    void checkOnGround(Engine *engine);
};