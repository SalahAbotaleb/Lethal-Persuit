#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collision.hpp>
#include <systems/bullets.hpp>

#include "systems/player.hpp"
#include "systems/shooting.hpp"

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::Shooting shooting;
    our::bulletsSystem bulletsSystem;
    our::PlayerSystem playerSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        collisionSystem.enter(getApp());
        bulletsSystem.enter(getApp());
        playerSystem.enter(getApp());
        shooting.enter(getApp(), config["world"]);
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(const double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem. update(&world, static_cast<float>(deltaTime));
        cameraController.update(&world, static_cast<float>(deltaTime));
        shooting.update(&world, static_cast<float>(deltaTime), &bulletsSystem);
        bulletsSystem.update(&world, static_cast<float>(deltaTime));
        playerSystem.update(&world, static_cast<float>(deltaTime));
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object

        if(const auto& keyboard = getApp()->getKeyboard(); keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }

        world.deleteMarkedEntities();
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        bulletsSystem.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};