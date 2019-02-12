
#include"DxFontClass.h"

DxFontClass::DxFontClass(){
	m_font = 0;
	m_texture = 0;
}


DxFontClass::DxFontClass(const DxFontClass& other)
{
}


DxFontClass::~DxFontClass()
{
}

bool DxFontClass::Init(ID3D11Device* device, const char* fontFilename, const WCHAR* textureFilename)
{
	bool isSuccess;
	isSuccess = LoadFontData(fontFilename);
	if (!isSuccess)
	{
		return false;
	}

	isSuccess = LoadTexture(device, textureFilename);
	if (!isSuccess)
	{
		return false;
	}

	return true;
}

void DxFontClass::ShutDown()
{
	ReleaseTexture();

	ReleaseFontData();

	return;
}
//载入字型数据
bool DxFontClass::LoadFontData(const char* filename)
{
	ifstream fin;
	int i;
	char temp;

	//存储字
	m_font = new FontType[95];
	if (!m_font)
	{
		return false;
	}
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//读取文件中的字形数据
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	fin.close();

	return true;
}
//释放资源
void DxFontClass::ReleaseFontData()
{
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

	return;
}
bool DxFontClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool isSuccess;

	m_texture = new DxTextureClass();
	if (!m_texture)
	{
		return false;
	}

	isSuccess = m_texture->Init(device, filename);
	if (!isSuccess)
	{
		return false;
	}

	return true;
}

void DxFontClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->ShutDown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* DxFontClass::GetTexture()
{
	return m_texture->GetTexture();
}
//生成定点数组
void DxFontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	//强制转换成vertextype数据类型
	vertexPtr = (VertexType*)vertices;

	//输入句子的长度
	numLetters = (int)strlen(sentence);

	//初始化索引
	index = 0;
	//绘制字符
	for (i = 0; i < numLetters; i++){
		letter = ((int)sentence[i]) - 32;

		//处理空白字符
		if (letter == 0){
			drawX = drawX + 3.0f;
		}
		else{
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			//更新到下一个位置
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
}
