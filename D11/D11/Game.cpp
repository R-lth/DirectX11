#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd   = hwnd;
	_width  = GWindSizeX;
	_height = GWindSizeY;

	// TODO.
	CreateDeviceAndSwapChain();
}

void Game::Update()
{
}

void Game::Render()
{
}

void Game::CreateDeviceAndSwapChain()
{
	//1. 스왑 체인(구조체 메모리) 초기화
	DXGI_SWAP_CHAIN_DESC desc;  
	ZeroMemory(&desc, sizeof(desc)); // 0으로 초기화. 단, 필요한 부분만 추가 설정. ≒ memset
	{
		desc.BufferDesc.Width = _width;						// 버퍼 크기와 화면 크기 맞춤
		desc.BufferDesc.Height = _height;					// 버퍼 크기와 화면 크기 맞춤
		desc.BufferDesc.RefreshRate.Numerator = 60; 
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;							// 멀티 샘플링 사용 안 함. 계단 현상(Aliasing) 고려하지 않음.
		desc.SampleDesc.Quality = 0; 
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 해당 버퍼는 최종 결과를 그려주는 역할로 사용 
		desc.BufferCount = 1;								// 후면 버퍼는 1개
		desc.OutputWindow = _hwnd;							// 스왑 체인에서 렌더링 결과를 출력할 윈도우의 핸들을 지정
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	};

	// 2. DirectX 객체 생성 (= 실질적인 초기화)
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, /*그래픽 카드를 사용하겠다. 사양 고려 필요*/
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	// 3. 초기화 여부. 실패 시 크러쉬
	assert(SUCCEEDED(hr));
}
