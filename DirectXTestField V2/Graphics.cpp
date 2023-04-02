#include "Graphics.h"

GraphicsClass::~GraphicsClass()
{
	_MIDELETE(swapChain)
	_MIDELETE(device)
	_MIDELETE(deviceContext)
}

bool GraphicsClass::ShadersCompile()
{
	HRESULT hr;
	
	hr = D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &BlobPixelShader, 0);
	hr = D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &BlobVertexShader, 0);

	hr = device->CreateVertexShader(BlobVertexShader->GetBufferPointer(), BlobVertexShader->GetBufferSize(), 0, &vertexShader);
	hr = device->CreatePixelShader(BlobPixelShader->GetBufferPointer(), BlobPixelShader->GetBufferSize(), 0, &pixelShader);

	return true;
}

bool GraphicsClass::SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE resource;
	MatrixType* matrixData;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	hr = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	matrixData = (MatrixType*)resource.pData;
	matrixData->projectionMatrix = projectionMatrix;
	matrixData->viewMatrix = viewMatrix;
	matrixData->worldMatrix = worldMatrix;

	deviceContext->Unmap(matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &shaderTexture[0]);
	return true;
}

bool GraphicsClass::ShadersLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[2];
	inputLayoutDesc[0].AlignedByteOffset = 0;
	inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayoutDesc[0].InputSlot = 0;
	inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[0].InstanceDataStepRate = 0;
	inputLayoutDesc[0].SemanticIndex = 0;
	inputLayoutDesc[0].SemanticName = "POSITION";

	inputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayoutDesc[1].InputSlot = 0;
	inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayoutDesc[1].InstanceDataStepRate = 0;
	inputLayoutDesc[1].SemanticIndex = 0;
	inputLayoutDesc[1].SemanticName = "TEXCOORD";

	device->CreateInputLayout(inputLayoutDesc, sizeof(inputLayoutDesc) / sizeof(inputLayoutDesc[0]), BlobVertexShader->GetBufferPointer(), BlobVertexShader->GetBufferSize(), &inputLayout);
	deviceContext->IASetInputLayout(inputLayout);
	return true;
}

bool GraphicsClass::InitMatrixBuffer()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.ByteWidth = sizeof(MatrixType);
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	device->CreateBuffer(&matrixBufferDesc, 0, &matrixBuffer);
	return true;
}

bool GraphicsClass::InitDeviceSwapChainContext()
{
	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenSizeX;
	swapChainDesc.BufferDesc.Height = screenSizeY;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = *hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Flags = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, 0, &deviceContext);  _HRISOK(hr)
		return true;
}

bool GraphicsClass::InitDepthStencil()
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	

	depthBufferDesc = { 0 };
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Width = screenSizeX;
	depthBufferDesc.Height = screenSizeY;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, 0, &depthStencilBuffer);

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.StencilReadMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	return true;
}

bool GraphicsClass::InitRenderTarget()
{
	HRESULT hr;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer); _HRISOK(hr);
	hr = device->CreateRenderTargetView(backBuffer, 0, &renderTarget);
	deviceContext->OMSetRenderTargets(1, &renderTarget, depthStencilView);
	return true;
}

bool GraphicsClass::InitRasteraizer()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	deviceContext->RSSetState(rasterizerState);
	return true;
}

bool GraphicsClass::InitViewPort()
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = (float)screenSizeX;
	viewport.Height = (float)screenSizeY;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	return true;
}

void GraphicsClass::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MipLODBias = 0.0f;

	device->CreateSamplerState(&samplerDesc, &samplerState);
}

bool GraphicsClass::Initialization()
{
	InitDeviceSwapChainContext();
	InitDepthStencil();
	InitRenderTarget();
	InitRasteraizer();
	InitViewPort();

	projectiveMatrix = XMMatrixPerspectiveFovLH(3.1415f / 4.0f, (float)screenSizeX / (float)screenSizeY, 0.1f, 1000.0f);

	worldMatrix = XMMatrixIdentity();

	orthoMatrix = XMMatrixOrthographicLH((float)screenSizeX, (float)screenSizeY, 0.1f, 1000.0f);

	TestModelLoader();
	TextureLoader((WCHAR*)L"pic.dds");
	ShadersCompile();
	ShadersLayout();
	CreateSamplerState();
	InitMatrixBuffer();
	

	

	return true;
}

void GraphicsClass::SetScreenSize(int x, int y)
{
	screenSizeX = x;
	screenSizeY = y;
}

void GraphicsClass::SetHWND(HWND* HWnd)
{
	hwnd = HWnd;
}

float color[4] = {0.0f, 0.0f, 0.7f, 0.0f};

bool GraphicsClass::FrameProc()
{
	deviceContext->ClearRenderTargetView(renderTarget, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	camera.Render();
	RenderModels();
	SetShaderParameters(worldMatrix, viewMatrix, projectiveMatrix, shaderTexture[0]);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	deviceContext->DrawIndexed(3, 0, 0);
	//deviceContext->Draw(3, 0);
	swapChain->Present(1, 0);
	return true;
}
