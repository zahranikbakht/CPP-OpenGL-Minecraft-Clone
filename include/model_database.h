//
// Created by Willi on 8/5/2020.
//
#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include "../libs/easylogging++.h"
#include "objloader.h"
#include "model.h"
namespace fs = std::experimental::filesystem::v1;

/// The Model Database loads models in the resources/models/ folder to be available at run time.
class ModelDatabase {
private:
    ModelDatabase();

    ModelDatabase(const ModelDatabase &) = delete;

    ModelDatabase &operator=(const ModelDatabase &) = delete;

    static ModelDatabase instance;
    std::unordered_map<std::string, std::shared_ptr<Model>> models;

public:

    /// Initializes the Model Database. Should only be called once during program execution.
    static void init();

    /** Returns a pointer to a Model based on its name
     *
     * @param modelName the model to look for
     * @return a pointer to the model if it exists, otherwise a nullptr.
     */
    static std::shared_ptr<Model> getModelByName(std::string &modelName);
};

/// Different model names namespace, since enums don't support strings
namespace ModelType {
    const std::string CUBE = "cubeModel";
}