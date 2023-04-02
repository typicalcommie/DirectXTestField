#include "Camera.h"

float pitch = 0;

void Camera::Render()
{

	XMFLOAT3 up, pos, at;

	up = { 0, 1.0f, 0 };

	pos = { 0, 0, -10.0f };

	at = { 0, 0, 1.0f };

	XMVECTOR Pos = XMLoadFloat3(&pos);

	XMVECTOR At = XMLoadFloat3(&at);

	XMVECTOR Up = XMLoadFloat3(&up);


	pitch += 3;

	rotationMatrix = XMMatrixRotationRollPitchYaw( 0, pitch * 0.0174532925f, 0);

	At = XMVector3TransformCoord(At, rotationMatrix);
	Up = XMVector3TransformCoord(Up, rotationMatrix);

	At = XMVectorAdd(Pos, At);

	viewMatrix = XMMatrixLookAtLH(Pos, At, Up);

}
