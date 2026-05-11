#pragma once

#include "graphics/core/camera.hpp"
#include "graphics/core/window.hpp"

#include <glm/glm.hpp>
#include <string>

/**
 * @enum InputState
 * @brief It tells which state is the mouse.
 *
 * If the mouse in passive state means that the camera will move following
 * the mouse cursor, if the state is active the camera will not follow
 * the cursor.
 */
enum InputState {
	MOUSE_PASSIVE,
	MOUSE_ACTIVE
};

/**
 * @brief Data structure to store mouse position data and allow fluent movement.
 */
struct Mouse {
	/// mouse current position
	glm::vec2 pos{};
	/// first move of the cursor
	bool first = true;
	/// if true start executing trackball movement
	bool trackState = false;
	/// skips the current position if the cursor has been moved to the opposite side
	/// of the window
	bool skipCursorPos = false;
};

/**
 * @brief Register callbacks for the WorldCamera movement with collisions.
 *
 * @param world the world camera
 */
void defaultCameraMovement(ogl::WorldCamera &world);

/**
 * @brief Utility method that manages mouse behaviour in active or passive mode.
 * It changes the GLFW callbacks.
 *
 * @param w the window
 * @param world the world camera
 * @param state the current input state (ACTIVE - PASSIVE)
 * @param mouse the mouse data
 */
void changeInputState(ogl::Window *w, ogl::WorldCamera &world, const InputState &state, Mouse &mouse);

/**
 * @brief Defines default callbacks for a standard Window.
 *
 * @param w the window
 * @param world the world camera
 * @param mouse the mouse data
 */
void defaultKeyCallback(ogl::Window *w, ogl::WorldCamera &world, Mouse &mouse);

/**
 * @brief Simple data structure to store mesh information: position,
 * scale, rotation and if it needs to be rendered.
 */
struct BasicInfo {
	/// mesh position
	glm::vec3 position{};
	/// mesh scale
	glm::vec3 scale{1, 1, 1};
	/// mesh rotation
	glm::vec3 rotation{};
	/// render mesh flag
	bool render = true;
};

/**
 * @brief Read data from the given image.
 *
 * @param path the image path
 * @param width the image width
 * @param height the image height
 * @param nrChannels the image channels number
 * @param desiredChannels the number of desired channels
 *
 * @return pointer to the data read from image
 */
unsigned char *readImageData(const std::string &path, int &width, int &height, int &nrChannels, int desiredChannels = 0);

/**
 * @brief Tells to the stbi library to flip images vertically.
 *
 * @param val the flag to flip
 */
void flipImagesVertically(const bool &val);

/**
 * @brief Frees the data read from an image.
 *
 * @param data the pointer to image data
 */
void freeImageData(void *data);
