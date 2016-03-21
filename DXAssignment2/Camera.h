#pragma once

#include "d3dx9.h"
#include "Transform3D.h"

/*
The camera class represents the view of the user, 
and is modeled after a camera in a position looking at a target.
*/

class Camera : public Transform3D {
	D3DXMATRIXA16 ViewMatrix; //viewing matrix transformation (for orientation of the scene)
	D3DXMATRIXA16 ReflectedViewMatrix; //reflected viewing matrix transformation (for mirrors)
	D3DXMATRIXA16 ProjectionMatrix;  //projection matrix transformation (for perspective)
	D3DXVECTOR3 direction; //Direction the camera is looking.
	D3DXVECTOR3 lookAt; //position camera is looking at. Derived from camera position and direction
	D3DXVECTOR3 right; //Right (N) vector
	D3DXVECTOR3 up; //Up (V) vector
	float nearClip; //Near clipping plane. Objects between here and camera won't render.
	float farClip; //Far clipping plane. Objects further than this won't render.
	float horizontalAngle; //Current horizontal rotation of the camera.
	float verticalAngle; //Current vertical rotation of the camera.
	float aspect; //Aspect ratio (for creating the projection matrix)
	float fieldOfView; //camera field of view. Default is PI/4 radians. (45 degrees)
	float mouseSpeed; //mouse sensitivity for rotation purposes.
public:
	Camera();
	inline float getFOV() const { return fieldOfView; }
	void setFOV(float newFOV);
	void setPosition(const D3DXVECTOR3& newPosition) override;
	void setAspect(float ratio);
	void addRotation(float horizontal, float vertical);
	const D3DXMATRIXA16& getReflectedView(const D3DXMATRIX& reflection);
	inline void addRotation(const POINTFLOAT& delta) { addRotation(delta.x, delta.y); }
	inline const D3DXVECTOR3& getDirection() const { return direction; }
	inline const D3DXVECTOR3& getRight() const { return right; }
	inline const D3DXVECTOR3& getUp() const { return up; }
	inline float getTurnSpeed() const { return mouseSpeed; }
	inline const D3DXMATRIXA16& getViewMatrix() const { return ViewMatrix; }
	inline const D3DXMATRIXA16& getProjectionMatrix() const { return ProjectionMatrix; }
};

