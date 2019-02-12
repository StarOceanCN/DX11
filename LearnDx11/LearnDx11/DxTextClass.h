
#ifndef DXTEXTCLASS
#define DXTEXTCLASS

#include"DxFontClass.h"
#include"DxShaderClass.h"

class DxTextClass
{
private:

	//SentenceType is the structure that holds the rendering information for each text sentence.
	struct SentenceType{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	//The VertexType must match the one in the FontClass.
	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	DxTextClass();
	DxTextClass(const DxTextClass& other);
	~DxTextClass();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
		D3DXMATRIX baseViewMatrix);
	void ShutDown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device);
	bool UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
		ID3D11DeviceContext* deviceContext);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix);

private:
	DxFontClass* m_font;
	DxTextureShaderClass* m_fontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};

#endif //DXTEXTCLASS
