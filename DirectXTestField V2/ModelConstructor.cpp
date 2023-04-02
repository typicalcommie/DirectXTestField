#include "Graphics.h"
#include <fstream>
#include <vector>

void GraphicsClass::TestModelLoader()
{
	/*VertexType vertexArray[6]{};

	vertexArray[0].vertex = { -1.0f, -1.0f, 0.0f };
	vertexArray[1].vertex = { -1.0f, 1.0f, 0.0f };
	vertexArray[2].vertex = { 1.0f, -1.0f, 0.0f };
	vertexArray[3].vertex = { 1.0f, 1.0f, 0.0f };
	vertexArray[4].vertex = { 1.4f, 1.0f, 0.0f };
	vertexArray[5].vertex = { -1.4f, -1.0f, 0.0f };

	vertexArray[0].texture = { 0.0f, 1.0f };
	vertexArray[1].texture = { 0.0f, 0.0f };
	vertexArray[2].texture = { 1.0f, 1.0f };
	vertexArray[3].texture = { 1.0f, 0.0f };
	vertexArray[4].texture = { 0.0f, 0.0f };
	vertexArray[5].texture = { 0.0f, 1.0f };


	uint32_t indices[12] = {0, 1, 2, 1, 2, 3, 3, 2, 5, 3, 5, 4};

	D3D11_BUFFER_DESC vertexDesc{}, indexDesc{};
	D3D11_SUBRESOURCE_DATA vertexData{}, indexData{};

	vertexDesc.ByteWidth = sizeof(VertexType) * 6;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertexArray;

	indexDesc.ByteWidth = sizeof(uint32_t) * 12;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;

	device->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer);

	device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);*/

	VertexType vertexArray[3];

	vertexArray[0].vertex = { -1.0f, -1.0f, 0.0f };
	vertexArray[1].vertex = { -1.0f, 1.0f, 0.0f };
	vertexArray[2].vertex = { 1.0f, 1.0f, 0.0f };


	vertexArray[0].texture = { 0.0f, 1.0f };
	vertexArray[1].texture = { 0.5f, 1.0f };
	vertexArray[2].texture = { 1.0f, 0.0f };

	unsigned int indices[3];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexDesc = {}, indexDesc = {};
	D3D11_SUBRESOURCE_DATA vertexData = {}, indexData = {};

	indexData.pSysMem = indices;

	indexDesc.ByteWidth = sizeof(unsigned int) * 3;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);


	vertexData.pSysMem = vertexArray;

	vertexDesc.ByteWidth = sizeof(VertexType) * 3;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;

	device->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer);

}

void GraphicsClass::RenderModels()
{
	const UINT stride = sizeof(VertexType);
	const UINT offset = 0;


	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

UINT GraphicsClass::ModelLoader()
{
	 
	std::fstream file;

	file.open("model_data.mdt", std::ios::binary | std::ios::in);

	file.seekg(0, std::ios::end);

	unsigned int file_size = file.tellg();

	std::vector<char> model_data(file_size);

	file.read(model_data.data(), file_size);

	
	return 0u;
}

bool GraphicsClass::TextureLoader(WCHAR* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector <char> buffer(size);
	file.read(buffer.data(), size);

	const char* header = buffer.data();
	const char* well = "DDS ";

	if (std::memcmp(header, well, 4))
		return false;

	const uint32_t* header32 = reinterpret_cast<const uint32_t*>(header + 4);
	uint16_t height = header32[2];
	uint16_t width = header32[3];
	const uint32_t mipLevels = header32[6];
	const uint32_t format = header32[20];

	D3D11_TEXTURE2D_DESC desc{};

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = mipLevels;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_BC3_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	uint32_t offset = 128;

	switch (format)
	{
	case MAKEFOURCC('D', 'X', '1', '0'):
		desc.Format = DXGI_FORMAT_BC7_UNORM;
		offset += 20;
		break;
	}

	ID3D11Texture2D* texture = nullptr;

	 

	std::vector<D3D11_SUBRESOURCE_DATA> initData(mipLevels);

	for (uint32_t i = 0; i < mipLevels; ++i)
	{
		uint32_t size = max(4u, width) * max(4u, height);

		initData[i].pSysMem = buffer.data() + offset;
		initData[i].SysMemPitch = width * 4;
		initData[i].SysMemSlicePitch = size;

		offset += size;

		width /= 2;
		height /= 2;
	}

	device->CreateTexture2D(&desc, initData.data(), &texture);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;

	device->CreateShaderResourceView(texture, &srvDesc, &shaderTexture[0]);

	return true;
}