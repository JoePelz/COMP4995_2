#pragma once

#include <d3d9.h>
#include "Defines.h"
#include "Drawable2D.h"
#include "Camera.h"
#include "Errors.h"
#include <vector>
#include <memory>

/* This class represents the current state of the program. It stores all game state information 
	and provides accessors and mutators as needed. 
*/
class Model {
	/* Collection of renderable items to be layered behind the 3d scene. */
	std::vector<std::shared_ptr<Drawable2D>> bgLayers_;
	/* Collection of renderable items to be layered ahead of the 3d scene. */
	std::vector<std::shared_ptr<Drawable2D>> fgLayers_;
	//The camera representing the players view
	Camera camera_;

	/* The current frame rate, in frames per second. */
	UINT frameRate_;
	/* The render time in ticks per frame. (1 / fps) */
	INT64 frameTime_;

	/* The device context width. */
	int width_;
	/* The device context height. */
	int height_;
public:
	/* The number of timing ticks per second. */
	INT64 ticksFrequency_;
	Model();
	~Model();

	/* Getter for frame rate. */
	UINT getFrameRate() const;
	/* Getter for frame time in ticks. There are ticksFrequency_ ticks per second. */
	INT64 getFrameTime() const;
	/* Mark that a frame has occured to update the frame rate counter.  Must be run every frame. */
	void setFrameTick();
	/* Initalize the frame counter, and the ticksFrequency_ variable. */
	int initFrameTimer();
	/* Getter for screen width. */
	inline int getWidth() const { return width_; }
	/* Getter for screen height. */
	inline int getHeight() const { return height_; }

	/* Add a drawable element to the background layer. */
	void addBG(std::shared_ptr<Drawable2D> drawable);
	/* Get the list of background elements. */
	const std::vector<std::shared_ptr<Drawable2D>>& getBG() const;
	/* Empty the list of background elements. */
	void clearBG();
	/* Add a drawable element to the foreground layer. */
	void addFG(std::shared_ptr<Drawable2D> drawable);
	/* Get the list of foreground elements. */
	const std::vector<std::shared_ptr<Drawable2D>>& getFG() const;
	/* Empty the list of foreground elements. */
	void clearFG();
	/* Get the camera object. */
	inline Camera& Model::getCamera() { return camera_; }
};

