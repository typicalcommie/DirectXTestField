#pragma once

#include "General.h"
#include <d3d11.h>
#include <d3d10.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include "Camera.h"


class GraphicsClass
{
public:
	~GraphicsClass();

	bool ShadersCompile();
	bool SetShaderParameters(XMMATRIX worldMatrrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	bool ShadersLayout();
	bool InitMatrixBuffer();
	bool InitDeviceSwapChainContext();
	bool InitDepthStencil();
	bool InitRenderTarget();
	bool InitRasteraizer();
	bool InitViewPort();
	bool Initialization();
	void SetScreenSize(int x, int y);
	void SetHWND(HWND* HWnd);
	bool FrameProc();
	void CreateSamplerState();
	UINT ModelLoader();
	void TestModelLoader();
	void RenderModels();
	bool TextureLoader(WCHAR* filename);

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11RasterizerState* rasterizerState;
	ID3D11RenderTargetView* renderTarget;

	ID3D11Texture2D* backBuffer;
	ID3D11ShaderResourceView* shaderTexture[32];

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	ID3D11Buffer* matrixBuffer;

	ID3DBlob *BlobPixelShader, *BlobVertexShader;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11SamplerState* samplerState;
	ID3D11InputLayout* inputLayout;

	Camera camera;

private:
	HWND* hwnd;
	int screenSizeX, screenSizeY;
	XMMATRIX projectiveMatrix, worldMatrix, orthoMatrix, viewMatrix;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	struct VertexType
	{
		XMFLOAT3 vertex;
		XMFLOAT2 texture;
	};

	struct MatrixType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

};