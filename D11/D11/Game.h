#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	// 윈도우 핸들(포인터). 어떤 윈도우(창)를 가리키는 식별자 같은 역할 
	HWND _hwnd;
	// 화면 크기
	uint32 _width = 0;
	uint32 _height = 0;

private:
	// DX
};

