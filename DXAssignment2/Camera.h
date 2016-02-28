#pragma once

#include "d3dx9.h"

class Camera {
	D3DXMATRIXA16 ViewMatrix;
	D3DXMATRIXA16 ProjectionMatrix;
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	float nearClip;
	float farClip;
	float horizontalAngle;
	float verticalAngle;
	float aspect;
	float initialFoV;
	float mouseSpeed;
public:
	Camera();
	~Camera();
	void setPos(const D3DXVECTOR3& newPosition);
	void setAspect(float ratio);
	void addRotation(float horizontal, float vertical);
	inline void addRotation(POINTFLOAT delta) { addRotation(delta.x, delta.y); }
	inline const D3DXVECTOR3& Camera::getPos() const { return position; }
	inline const D3DXVECTOR3& Camera::getDirection() const { return direction; }
	inline const D3DXVECTOR3& Camera::getRight() const { return right; }
	inline float getTurnSpeed() const { return mouseSpeed; }
	inline const D3DXMATRIXA16& getViewMatrix() const { return ViewMatrix; }
	inline const D3DXMATRIXA16& getProjectionMatrix() const { return ProjectionMatrix; }
};

