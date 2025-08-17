// DirectX ���̺귯�� ��ũ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ����� ����� ���, DirectXTex ���̺귯���� ����� ���� ���
// ������ ����� ���, �Ϲ� DirectXTex ���̺귯�� ���
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif