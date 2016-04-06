#pragma once

#include "d3dx9.h"
#include "MathUtilities.h"
#include <iomanip>

/*
The Transform3D class represents a transformation in 3D space. 
This includes position, rotation and scale, and excludes shearing.
*/

class Transform3D {
protected:
	D3DXMATRIXA16 xform_; //The transformation matrix of this object in world space
	D3DXVECTOR3 scale_; //This object's scale.
	D3DXVECTOR3 position_; //This object's position.
	D3DXQUATERNION  rotation_; //This object's rotation.
	D3DXMATRIXA16 tnet_; //the final transform to render with.
	BoundingSphere bsphere_; //the bounding sphere for selection.
public:
	//Construct a default transform at the origin with a scale of 1.0 and no rotation.
	Transform3D() : scale_{ 1.0f, 1.0f, 1.0f }, rotation_{ 0, 1, 0, 0 }, bsphere_{ } {}

	inline const D3DXVECTOR3& getPosition() const { return position_; }
	inline const D3DXVECTOR3& getScale() const { return scale_; }

	//Update the position of an object.
	virtual void setPosition(const D3DXVECTOR3& offset) { position_ = offset; bsphere_.center_ = offset; }
	virtual void translate(const D3DXVECTOR3& offset) { setPosition(position_ + offset); }

	//Update the scale of an object.
	virtual void setScale(const D3DXVECTOR3& s) { scale_ = s; }
	virtual void scale(const D3DXVECTOR3& s) { setScale({ s.x * scale_.x, s.y * scale_.y, s.z * scale_.z }); }
	virtual void scale(float s) { setScale(scale_ * s); }

	//Update the rotation of an object. Note: rotations are expressed as quaternions, not matrices.
	virtual void setRotation(D3DXQUATERNION rotationQuat) { rotation_ = rotationQuat; }
	virtual void setRotation(const D3DXVECTOR3& axis, float angle) { D3DXQuaternionRotationAxis(&rotation_, &axis, angle); }
	virtual void rotate(const D3DXVECTOR3& axis, float angle) { 
		D3DXQUATERNION temp;
		D3DXQuaternionRotationAxis(&temp, &axis, angle);
		setRotation(rotation_ * temp);
	}

	//Get the net transformation matrix.
	virtual const D3DXMATRIXA16& getTransform() { 
		D3DXMatrixTransformation(&xform_, NULL, NULL, &scale_, NULL, &rotation_ , &position_);
		return xform_;
	}

	virtual const D3DXMATRIXA16& getReflection(const D3DXMATRIX& reflection) {
		D3DXMatrixMultiply(&tnet_, &getTransform(), &reflection);
		return tnet_;
	}

	virtual const BoundingSphere& getBoundingSphere() const {
		return bsphere_;
	}

	virtual void setBoundingSphereRadius(float radius) {
		bsphere_._radius = radius;
	}
};

