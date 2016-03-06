#include "Camera.h"

/*
Summary:
	Constructor. Create a camera at a -Z and +Y position, looking at the origin.
Params: -
Return: -
*/
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
	fieldOfView{ D3DX_PI / 4 },
	mouseSpeed{ 1.0f } 
{
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, fieldOfView, aspect, nearClip, farClip);
	setPosition({ 0.0f, 1.7f, -5.0f });
	addRotation(0, 0);
}

/*
Summary:
	rotate the camera in 3D based on 2D screen-space motion. 
Params: 
	horizontal: The desired horizontal (screen-space) movement, divided by the screen resolution (normalized).
	vertical: The desired vertical (screen-space) movement, divided by the screen resolution (normalized).
Return: -
*/
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

/*
Summary:
	Adjust the field of view (FOV) of the camera. Initial value is PI/4 radians.
Params: 
	newFOV: the new field of view for the camera, in radians.
Return: -
*/
void Camera::setFOV(float newFOV) {
	fieldOfView = newFOV;
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, fieldOfView, aspect, nearClip, farClip);
}

/*
Summary:
	Moves the camera to the given position, maintaining orientation.
Params: 
	newPosition: The coordinates to move to.
Return: -
*/
void Camera::setPosition(const D3DXVECTOR3& newPosition) {
	Transform3D::setPosition(newPosition);
	lookAt = position_ + direction;
	D3DXMatrixLookAtLH(&ViewMatrix, &position_, &lookAt, &up);
}

/*
Summary:
	Set the camera projection aspect ratio. (Should match resolution aspect ratio)
Params: 
	ratio: the aspect ratio to project (width / height)
Return: -
*/
void Camera::setAspect(float ratio) {
	aspect = ratio;
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, fieldOfView, aspect, nearClip, farClip);
}