// DirectX ���̺귯�� ��ũ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// 디버그 빌드일 경우, DirectXTex 라이브러리의 디버그 버전 사용
// 릴리즈 빌드일 경우, 일반 DirectXTex 라이브러리 사용
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif