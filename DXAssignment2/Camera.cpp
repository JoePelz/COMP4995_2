#include "Camera.h"



Camera::Camera() :
	direction{ 0.0f, -1.7f, 5.0f },
	lookAt{ 0.0f, 0.0f, 1.0f },
	right{ 1.0f, 0.0f, 0.0f },
	up{ 0.0f, 1.0f, 0.0f },
	nearClip{ 0.1f },
	farClip{ 100.0f },
	horizontalAngle{ 0.0f },
	verticalAngle{ -0.2f },
	aspect{ 4.0f / 3.0f },
	initialFoV{ D3DX_PI / 4 },
	mouseSpeed{ 1.0f } 
{
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, initialFoV, aspect, nearClip, farClip);
	addRotation(0, 0);
}

Camera::~Camera() {
}

void Camera::addRotation(float horizontal, float vertical) {
	horizontalAngle -= horizontal * mouseSpeed;
	verticalAngle += vertical * mouseSpeed;

	//No flipping upside down! I mean you!
	if (verticalAngle > 1.570f)
		verticalAngle = 1.570f;
	else if (verticalAngle < -1.570f)
		verticalAngle = -1.570f;

	direction = D3DXVECTOR3(
		cosf(verticalAngle) * sinf(horizontalAngle),
		sinf(verticalAngle),
		cosf(verticalAngle) * cosf(horizontalAngle)
		);

	right = D3DXVECTOR3(
		sinf(horizontalAngle - D3DX_PI / 2.0f),
		0.0f,
		cosf(horizontalAngle - D3DX_PI / 2.0f)
		);
	D3DXVec3Cross(&up, &right, &direction);

	lookAt = position_ + direction;
	D3DXMatrixLookAtLH(&ViewMatrix, &position_, &lookAt, &up);
}

void Camera::setFOV(float newFOV) {
	initialFoV = newFOV;
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, initialFoV, aspect, nearClip, farClip);
}

void Camera::setPosition(const D3DXVECTOR3& newPosition) {
	ITransform::setPosition(newPosition);
	lookAt = position_ + direction;
	D3DXMatrixLookAtLH(&ViewMatrix, &position_, &lookAt, &up);
}

void Camera::setAspect(float ratio) {
	aspect = ratio;
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, initialFoV, aspect, nearClip, farClip);
}