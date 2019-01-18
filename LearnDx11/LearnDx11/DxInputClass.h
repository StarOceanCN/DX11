#ifndef DXINPUTCLASS_H
#define DXINPUTCLASS_H


class DxInputClass
{
public:
	DxInputClass();
	DxInputClass(const DxInputClass& other);
	~DxInputClass();

	void Init();
	bool IsKeyDown(unsigned int key);
	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);

private:
	bool m_keys[256];
};


#endif //DXINPUTCLASS_H
