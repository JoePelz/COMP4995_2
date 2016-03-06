#pragma once

#include <d3d9.h>
#include "Defines.h"
#include "Drawable2D.h"
#include "Drawable3D.h"
#include "Camera.h"
#include "Errors.h"
#include "Light.h"
#include <vector>
#include <memory>

typedef std::shared_ptr<Drawable3D> pDrawable3D;
typedef std::shared_ptr<Light> pLight;

/* 
This class represents the current state of the program. It stores all game state information 
and provides accessors and mutators as needed. 
*/
class Model {
	/* Collection of renderable items to be layered behind the 3d scene. */
	std::vector<std::shared_ptr<Drawable2D>> bgLayers_;
	/* Collection of 3d renderable items to be rendered in the scene. */
	std::vector<pDrawable3D> polyLayers_;
	/* Collection of renderable items to be layered ahead of the 3d scene. */
	std::vector<std::shared_ptr<Drawable2D>> fgLayers_;
	/* Collection of D3D9 Lights to illuminate the scene. */
	std::vector<pLight> lights_;
	//The camera representing the players view
	Camera camera_;

	/* The current frame rate, in frames per second. */
	UINT frameRate_;
	/* The render time in ticks per frame. (1 / fps) */
	INT64 frameTime_;
	/* The number of timing ticks per second. */
	INT64 ticksFrequency_;

	/* The device context width. */
	int width_;
	/* The device context height. */
	int height_;
	/* Whether to run in fullscreen mode. True for fullscreen. */
	bool fullscreen_;

	/* The currently selected object. */
	Transform3D* selected_;
public:
	Model::Model() : frameRate_{ 1000 }, width_{ DEFAULT_WIDTH }, height_{ DEFAULT_HEIGHT }, fullscreen_{ USE_FULLSCREEN }, selected_{ &camera_ } { }
	
	/* Getter for frame rate. */
	UINT getFrameRate() const;
	/* Getter for frame time in milliseconds. */
	float getFrameTime() const;
	/* Mark that a frame has occured to update the frame rate counter.  Must be run every frame. */
	void setFrameTick();
	/* Initalize the frame counter, and the ticksFrequency_ variable. */
	int initFrameTimer();
	/* Getter for screen width. */
	inline int getWidth() const { return width_; }
	/* Getter for screen height. */
	inline int getHeight() const { return height_; }
	/* Getter for fullscreen mode. */
	inline bool getFullscreen() const { return fullscreen_; }
	void setDisplayMode(int width, int height, bool fullscreen);

	/* Add a drawable element to the background layer. */
	inline void addBG(std::shared_ptr<Drawable2D> drawable) { bgLayers_.push_back(drawable); }
	/* Get the list of background elements. */
	inline const std::vector<std::shared_ptr<Drawable2D>>& getBG() const { return bgLayers_; }
	/* Empty the list of background elements. */
	inline void clearBG() { bgLayers_.clear(); }
	/* Add a drawable element to the foreground layer. */
	inline void addFG(std::shared_ptr<Drawable2D> drawable) { fgLayers_.push_back(drawable); }
	/* Get the list of foreground elements. */
	inline const std::vector<std::shared_ptr<Drawable2D>>& getFG() const { return fgLayers_; }
	/* Empty the list of foreground elements. */
	inline void clearFG() { bgLayers_.clear(); }

	inline void add3D(pDrawable3D drawable) { polyLayers_.push_back(drawable); }
	inline const auto& get3D() const { return polyLayers_; }
	inline void clear3D() { polyLayers_.clear(); }

	inline void addLight(pLight light) { lights_.push_back(light); }
	inline const auto& getLights() const { return lights_; }
	inline void clearLights() { lights_.clear(); }

	/* Get the camera object. */
	inline Camera& Model::getCamera() { return camera_; }
	/* Sets the currently selected object's transform controls. */
	inline void setSelection(Transform3D* sel) { selected_ = sel; }
	/* Get the currently selected object's transform controls. */
	inline Transform3D* getSelection() { return selected_; }
};

