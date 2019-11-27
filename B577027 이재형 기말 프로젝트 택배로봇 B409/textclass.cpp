///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"

TextClass::TextClass() {
	m_Font = 0;
	m_FontShader = 0;
	for (int i = 0; i < 10; i++)
	{
		m_sentence[i] = 0;
	}
}
TextClass::TextClass(const TextClass& other)
{
}
TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int
	screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, "../Engine/data/fontdata.txt", L"../Engine/data/textures/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		result = InitializeSentence(&m_sentence[i], 64, device);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

void TextClass::Shutdown()
{
	for (int i = 0; i < 10; i++)
	{
		ReleaseSentence(&m_sentence[i]);
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX
	orthoMatrix)
{
	bool result;

	for (int i = 0; i < 10; i++)
	{
		result = RenderSentence(deviceContext, m_sentence[i], worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	return true;
}

bool TextClass::SetScreen(ID3D11DeviceContext* deviceContext)
{
	char tempString[128];
	char ScreenSizeString[128];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s(m_screenWidth, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(ScreenSizeString, "Screen Size Width: ");
	strcat_s(ScreenSizeString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[0], ScreenSizeString, m_screenWidth / 2, 40, 0.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(m_screenHeight, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(ScreenSizeString, "Screen Size Height: ");
	strcat_s(ScreenSizeString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[1], ScreenSizeString, m_screenWidth / 2, 70, 0.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[64];
	char mouseString[64];
	bool result;

	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcat_s(mouseString, " Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[2], mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetCameraPosition(float cameraX, float cameraY, float cameraZ, ID3D11DeviceContext* deviceContext)
{
	char tempString[128];
	char cameraString[128];
	bool result;

	_itoa_s((int)cameraX, tempString, 10);
	strcpy_s(cameraString, "Camera X: ");
	strcat_s(cameraString, tempString);

	_itoa_s((int)cameraY, tempString, 10);
	strcat_s(cameraString, " Camera Y: ");
	strcat_s(cameraString, tempString);

	_itoa_s((int)cameraZ, tempString, 10);
	strcat_s(cameraString, " Camera Z: ");
	strcat_s(cameraString, tempString);

	result = UpdateSentence(m_sentence[3], cameraString, 20, 70, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[4], fpsString, 20, 100, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[5], cpuString, 20, 130, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetObjs(int objs, ID3D11DeviceContext* deviceContext)
{
	char tempString[128];
	char objsString[128];
	bool result;

	// Convert the fps integer to string format.
	_itoa_s(objs, tempString, 10);

	// Setup the fps string.
	strcpy_s(objsString, "Objects: ");
	strcat_s(objsString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[6], objsString, m_screenWidth / 2, 100, 0.0f, 1.0f, 0.3f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetPolys(int polys, ID3D11DeviceContext* deviceContext)
{
	char tempString[128];
	char polysString[128];
	bool result;

	// Convert the cpu integer to string format.
	_itoa_s(polys, tempString, 10);

	// Setup the cpu string.
	strcpy_s(polysString, "Polygons: ");
	strcat_s(polysString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence[7], polysString, m_screenWidth / 2, 130, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScore(int pScore, int cScore, ID3D11DeviceContext* deviceContext)
{
	char tempString[128];
	char polysString[128];
	bool result;

	_itoa_s(pScore, tempString, 10);

	strcpy_s(polysString, "Player : ");
	strcat_s(polysString, tempString);

	result = UpdateSentence(m_sentence[8], polysString, 300, 400, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	_itoa_s(cScore, tempString, 10);

	strcpy_s(polysString, "Computer : ");
	strcat_s(polysString, tempString);

	result = UpdateSentence(m_sentence[9], polysString, 500 + m_screenWidth / 2, 400, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.delete [] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, const char* text, int positionX, int positionY, float red, float
	green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX
	worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix,
		orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		false;
	}

	return true;
}