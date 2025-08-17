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
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain(); 
	void CreateRenderTargetView();
	void SetViewport();

private:
	// 윈도우 핸들(포인터). 어떤 윈도우(창)를 가리키는 식별자 같은 역할 
	HWND _hwnd;
	// 화면 크기
	uint32 _width = 0;
	uint32 _height = 0;

private:
	// DX
	// Device & Swap Chain
	ComPtr<ID3D11Device> _device;				// GPU에 리소스를 생성하는 객체.
	ComPtr<ID3D11DeviceContext> _deviceContext; // 렌더링 파이프라인에 이미 만들어진 리소스를 연결하고 그리기 명령을 내리는 객체.
	/* 메모리 관리 방법
	* 1. new와 delete로 관리하는 것이 아님
	* 2. 수동 관리
	* 2-1. _device->AddRef();
	* 2-2. _device->Release();
	* 3. 자동 관리로, 가장 안전
	* 3-1. ComPtr<>
	*/

	ComPtr<IDXGISwapChain> _swapChain = nullptr; // 화면 출력용 객체. GPU가 그린 결과물을 화면에 보여주기 위한 객체.
	/* 렌더링된 프레임을 디스플레이에 표시하는 데 사용함.
	*  GPU는 렌더링 결과를 후면 버퍼(back buffer)에 그리는데, 
	   이때 '스왑 체인'을 통해 전면 버퍼와 후면 버퍼를 교체(swap)해서 실제 모니터에 출력함.

	  → 이 과정을 더블 버퍼링이라고 부름.
	*/

	// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // 후면 버퍼를 묘사하는 객체

	// Misc
	D3D11_VIEWPORT _viewport = {0};					   // 화면 위치·크기를 묘사하는 구조체
	float _clearColor[4] = { 0.7f, 1.0f, 0.7f, 1.0f }; // 밝고 연한 연두색
};

