#pragma once

#include "d3dx9.h"
#include "ITransform.h"

class Camera : public ITransform {
	D3DXMATRIXA16 ViewMatrix;
	D3DXMATRIXA16 ProjectionMatrix;
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
	inline float getFOV() const { return initialFoV; }
	void setFOV(float newFOV);
	void setPosition(const D3DXVECTOR3& newPosition) override;
	void setAspect(float ratio);
	void addRotation(float horizontal, float vertical);
	inline void addRotation(const POINTFLOAT& delta) { addRotation(delta.x, delta.y); }
	inline const D3DXVECTOR3& getDirection() const { return direction; }
	inline const D3DXVECTOR3& getRight() const { return right; }
	inline const D3DXVECTOR3& getUp() const { return up; }
	inline float getTurnSpeed() const { return mouseSpeed; }
	inline const D3DXMATRIXA16& getViewMatrix() const { return ViewMatrix; }
	inline const D3DXMATRIXA16& getProjectionMatrix() const { return ProjectionMatrix; }
};

