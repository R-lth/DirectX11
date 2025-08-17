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

void Game::CreateGeometry()
{
	// VertexData
	// 
	{
		_vertices.resize(3);

		// 반 시계 방향 (후면 버퍼)
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f); // 왼쪽 아래
		_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);

		_vertices[1].position = Vec3(0.f, 0.5f, 0.f);    // 위쪽 중앙
		_vertices[1].color = Color(0.0f, 1.f, 0.f, 1.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);  // 오른쪽 아래
		_vertices[2].color = Color(0.0f, 0.f, 1.f, 1.f);
	}

	// CPU 메모리를 GPU에게 넘김
	// [CPU ↔ RAM] [GPU ↔ RAM]

	// VertexBuffer
	{
		// 버퍼 묘사
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;		   // ★ Cpu 접근 불가. Gpu만 읽을 수 있음.
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점 버퍼 용으로 지정
		desc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * _vertices.size());

		//
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = &_vertices[0]; // 시작 주소 반환

		// 정점 버퍼 생성
		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
	}
}

void Game::CreateInputLayout()
{
	// CreateGeometry()로 만든 기하 데이터를 어떻게 끊어서 읽을 것인지
	// Direct3D 11에서 정점(Vertex) 데이터가 어떤 구조로 되어 있는지를 GPU(파이프라인)에 알려주는 역할
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// POSITION : float3 (x, y, z) → 3 * 4바이트 = 12바이트
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		/* COLOR    : POSITION이 12바이트 차지하므로 COLOR는 그 뒤(오프셋 12)에서 시작*/
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, 
		_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// Default.hlsl을 _vsBlob이라는 형태로 갖게 됨.
	LoadShaderFromFile(L"Default.hlsl", "VS"/*메인 함수*/, "vs_5_0", _vsBlob);

	// _vsBlob 정보로 _vertexShader 생성
	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());

	//
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3D10Blob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // 디버그 모드이며, 최적화 건너띔.

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	CHECK(hr);
}
