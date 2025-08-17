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

	// 후면 버퍼를 만들고 → 그 버퍼를 그림 대상(View)으로 지정하고 → 화면 크기에 맞게 뷰포트를 설정한다
	CreateDeviceAndSwapChain(); // 버퍼 준비
	CreateRenderTargetView();   // 그림 대상 지정
	SetViewport();				// 화면에 맞는 그리기 영역 설정
}

void Game::Update()
{
}

void Game::Render()
{
	RenderBegin();

	// TODO. 렌더링 파이프라인
	// IA → VS → RS → PS → OM
	{

	}

	RenderEnd();
}

void Game::RenderBegin()
{
	// 후면 버퍼를 RTV에 그리라고 요청
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 윈도우 창 배경색 초기화
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 화면 정보 기입
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// 다 그린 것을 제출
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	// 후면 버퍼를 생성해서 교체(스왑)할 수 있도록 초기화하는 과정
	// 
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
	//assert(SUCCEEDED(hr));
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	// Render Target View(RTV) 생성. 이는 "GPU야, 이 후면 버퍼에 그림을 그려라" 하고 알려주는 역할
	// 
	// 스왑체인에서 후면 버퍼를 꺼내와, 그 버퍼를 GPU가 실제로 그림을 그릴 대상(View)인 RTV로 연결

	HRESULT hr;

	// 1) 스왑체인에서 후면 버퍼(그림을 그릴 실제 화면 메모리) 꺼내오기
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 2) 꺼낸 후면 버퍼를 "그릴 수 있는 대상(Render Target)"으로 묘사하는 뷰 생성
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}
