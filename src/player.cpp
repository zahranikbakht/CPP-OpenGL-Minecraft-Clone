//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::CUBE, BlockID::DIRT,
                          Transform({20.0, 30.0, 20.0}, {1, 1, 1}, {0, 0, 0})) {
    this->camera = Camera{};
}

Player::Player(glm::vec3 spawnPoint) : Entity(ModelType::CUBE, BlockID::DIRT,
                                              Transform(spawnPoint, {1, 1, 1}, {0, 0, 0})) {
    this->camera = Camera{};
}

void Player::jump() {
    if (onGround) {
        onGround = false;
        this->acceleration.y = 25.0f;
    }
}

void Player::look(GLFWwindow *window, double xpos, double ypos) {

    static double lastXpos = 0.0, lastYpos = 0.0;
    double changeX = xpos - lastXpos, changeY = ypos - lastYpos;

    //TODO: Player can rotate on Y axis
    //this->getTransform().rotate({0.0, 1.0, 0.0}, glm::radians(changeX * 0.05f));

    this->camera.Pitch -= static_cast<float>(changeY) * 0.5f;
    if (this->camera.Pitch > 89.0f)
        this->camera.Pitch = 89.0f;
    if (this->camera.Pitch < -89.0f)
        this->camera.Pitch = -89.0f;

    this->camera.Yaw += static_cast<float>(changeX) * 0.5f;

    this->camera.updateCameraVectors();
    glfwGetCursorPos(window, &lastXpos, &lastYpos);
}

/// Should do any collision/physics here
void Player::update(Engine *engine, float dt) {

    auto currentChunk = engine->chunkManager->getChunkByXZ(
            {getTransform().getPosition().x, getTransform().getPosition().z});

    velocity += acceleration;
    velocity *= dt;
    acceleration = {0, velocity.y, 0};

    if (!onGround) {
        acceleration.y -= 50 * dt;
    }

    if (currentChunk.has_value()) {
        collide((*currentChunk));
        checkOnGround((*currentChunk));
    } else {
        // Out of bounds!
        LOG(INFO) << "Player is out of bounds at " << this->getTransform().getPosition().x << " "
                  << this->getTransform().getPosition().y << " " << this->getTransform().getPosition().z;
    }

    this->transform.translate(velocity);

    if (this->transform.getPosition().y < 0) {
        LOG(DEBUG) << "Player fell through the world!.";
        this->transform.position.y = 30.0f;
    }

    this->camera.Position = this->getTransform().getPosition() + glm::vec3(0.5f, 1.5f, 0.5f);
}

static bool pressedLMB = false;
static bool pressedRMB = false;

void Player::processInput(Engine *engine) {

    GLFWwindow *window = engine->getWindow();
    float speed = 10;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::DIRT;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::DIRT_GRASS;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::BEDROCK;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::STONE;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::OAK_LOG;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::OAK_LEAVES;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        this->selectedBlockID = BlockID::WATER;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(camera.Front + camera.Right);
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(camera.Front + -(camera.Right));
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(-(camera.Front) + -(camera.Right));
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(-(camera.Front) + camera.Right);
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            this->acceleration.x += camera.Front.x * speed;
            this->acceleration.z += camera.Front.z * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            this->acceleration.x += camera.Front.x * -speed;
            this->acceleration.z += camera.Front.z * -speed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            this->acceleration.x += camera.Right.x * -speed;
            this->acceleration.z += camera.Right.z * -speed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            this->acceleration.x += camera.Right.x * speed;
            this->acceleration.z += camera.Right.z * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            this->jump();
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pressedLMB) {
        pressedLMB = true;
        this->removeEntity(engine);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !pressedRMB) {
        pressedRMB = true;
        this->placeBlock(engine);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        pressedLMB = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        pressedRMB = false;
}

void Player::collide(const std::shared_ptr<Chunk> &currentChunk) {

    std::optional<Entity *> optEnt;

    optEnt = (*currentChunk).getEntityByBoxCollision(
            this->getTransform().getPosition() + velocity,
            this->box);

    if (optEnt.has_value()) {
        if (velocity.y > 0.0f) {
            if (this->getTransform().getPosition().y + this->box.dimensions.y <
                (*optEnt)->getTransform().getPosition().y) {
                this->transform.position.y = glm::ceil((*optEnt)->getTransform().getPosition().y - 1.0f);
                velocity.y = 0.0f;
            }
        } else if (velocity.y < 0.0f) {
            if (this->getTransform().getPosition().y >
                (*optEnt)->getTransform().getPosition().y + (*optEnt)->box.dimensions.y) {
                this->transform.position.y = glm::floor((*optEnt)->getTransform().getPosition().y + 1.0f);
                onGround = true;
                velocity.y = 0.0f;
            }
        }
        velocity.x = 0.0f;
        velocity.z = 0.0f;
    }

}

void Player::checkOnGround(const std::shared_ptr<Chunk> &currentChunk) {
    std::optional<Entity *> optEntity = (*currentChunk).getEntityByBoxCollision(
            this->getTransform().getPosition() + glm::vec3(0.0f, -0.2f, 0.0f), this->box);

    onGround = optEntity.has_value();
}

void Player::removeEntity(Engine *engine) const {

    std::optional<std::shared_ptr<Chunk>> chunk;
    std::optional<Entity *> closestEnt;
    glm::vec3 endPoint;

    for (unsigned int i = 0; i < 500; i++) {
        endPoint = this->camera.Position + ((static_cast<float>(i) / 100.0f) * camera.Front);
        chunk = engine->chunkManager->getChunkByXZ({endPoint.x, endPoint.z});
        closestEnt = chunk.value()->getEntityByWorldPos(endPoint);

        if (closestEnt.has_value()) {
            engine->chunkManager->removeEntityFromChunk(*closestEnt.value());
            return;
        }
    }

}

void Player::placeBlock(Engine *engine) {

    std::optional<std::shared_ptr<Chunk>> chunk;
    std::optional<Entity *> closestEnt;
    glm::vec3 endPoint;

    for (unsigned int i = 0; i < 500; i++) {
        endPoint = this->camera.Position + ((static_cast<float>(i) / 100.0f) * camera.Front);
        chunk = engine->chunkManager->getChunkByXZ({endPoint.x, endPoint.z});
        closestEnt = chunk.value()->getEntityByWorldPos(endPoint);

        if (closestEnt.has_value()) {
            glm::vec3 newBlockPlacement = glm::vec3(
                    this->camera.Position + ((static_cast<float>(i - 1) / 100.0f) * camera.Front));
            newBlockPlacement = glm::vec3(glm::floor(newBlockPlacement.x), glm::floor(newBlockPlacement.y),
                                          glm::floor(newBlockPlacement.z));
            (*chunk)->addEntity(Entity(ModelType::CUBE, selectedBlockID,
                                       Transform(newBlockPlacement, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0))));
            return;
        }
    }
}