//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "libs/easylogging++.h"
#include "include/engine.h"

INITIALIZE_EASYLOGGINGPP

void initLogging();

/// Main
int main(int argc, char *argv[]) {

    initLogging();

    auto engine = Engine({});

    LOG(INFO) << "Initializing resource databases.";

    TextureDatabase::init();
    ModelDatabase::init();

    LOG(INFO) << "Primary initialization done.";

    //TODO: Would it be possible to move this to the engine constructor?
    glfwSetCursorPosCallback(engine.getWindow(), [](GLFWwindow *window, double x, double y) {
        auto *engine = static_cast<Engine *>( glfwGetWindowUserPointer(window));
        engine->mouseCallbackFunc(window, x, y);
    });

//<<<<<<< HEAD
//=======
//
//
//
//    LOG(INFO) << "ADDING SOME ENTITIES";
//
//    for (int x = 0; x < 40; x++) {
//        for (int z = 0; z < 40; z++) {
//            int rd = rand() % 2;
//            if (rd) {
//                engine.addEntity(
//                        Entity(ModelType::CUBE, BlockID::DIRT_GRASS, Transform({x, 0, z}, {1, 1, 1}, {0, 0, 0})));
//            } else {
//                engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({x, 0, z}, {1, 1, 1}, {0, 0, 0})));
//            }
//        }
//    }
//
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({12.0f, 1, 12.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({12.0f, 2, 12.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({12.0f, 3, 12.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({12.0f, 3, 11.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({11.0f, 3, 11.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({13.0f, 3, 11.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({11.0f, 3, 10.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({13.0f, 3, 10.0f}, {1, 1, 1}, {0, 0, 0})));
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({5.0f, 1, 5.0f}, {1, 1, 1}, {0, 0, 0})));
//
//
//
//    engine.addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({22.0f, 1, 15.0f}, {1, 1, 1}, {0, 0, 0})));
//>>>>>>> feature/removeBlocks
    engine.runLoop();
}

/// Sets up some logging attributes from config file and flags
void initLogging() {

    // Load logging config from file
    el::Configurations conf("easylogging.config");

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    //Reconfigure all loggers
    el::Loggers::reconfigureAllLoggers(conf);
}