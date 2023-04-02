#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{

public:

	void Render();

	XMMATRIX viewMatrix;
	XMMATRIX rotationMatrix;

};