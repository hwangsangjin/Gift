#pragma once

class Vector3;

class Matrix4x4
{
public:
	Matrix4x4();
	~Matrix4x4();

	void SetIdentity();
	void SetTranslation(const Vector3& translation);
	void SetScale(const Vector3& scale);
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);
	void SetOrthographic(float width, float height, float near_plane, float far_plane);
	void SetPerspective(float fov, float aspect, float z_near, float z_far);

	float GetDeterminant();
	void Inverse();

	Vector3 GetXDirection();
	Vector3 GetYDirection();
	Vector3 GetZDirection();
	Vector3 GetTranslation();

	void operator*=(const Matrix4x4& matrix);
	void SetMatrix(const Matrix4x4& matrix);

	float m_matrix[4][4] = {};
};

