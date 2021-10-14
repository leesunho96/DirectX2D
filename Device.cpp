#include "Device.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	InitWindow(hInstance, nShowCmd);
	InitDirect3D(hInstance);

	Running();

	Destroy();

	return 0;
}


HWND Hwnd = NULL;
wstring Title = L"2108DirectX2d";

IDXGISwapChain* SwapChain;
//Device :  CPU, ����ó��
ID3D11Device* Device;
//DeviceContext : GPU, ȭ�鿡 �׸��� ����.
ID3D11DeviceContext* DeviceContext;
ID3D11RenderTargetView* RTV;

/*
	DX10���� �Ϻ��ϰ� CPU, GPU �и�.
	�� ����� RAM - VRAM�� �̿�
*/



ID3D11VertexShader* VertexShader;
ID3D11PixelShader* PixelShader;
ID3D10Blob* VsBlob;
ID3D10Blob* PsBlob;

void InitWindow(HINSTANCE hInstance, int ShowWnd)
{
	//Register Class
	{
		WNDCLASSEX wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = NULL;
		wndClass.cbWndExtra = NULL;
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = Title.c_str();
		wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		WORD check = RegisterClassEx(&wndClass);
		assert(check != 0);
	}

	//Create Handle
	{
		Hwnd = CreateWindowEx
		(
			NULL,
			Title.c_str(),
			Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			Width, Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);
		assert(Hwnd != NULL);
	}

	ShowWindow(Hwnd, ShowWnd);
	UpdateWindow(Hwnd);
}

void InitDirect3D(HINSTANCE hInstance)
{
	//Create Device and DeviceContext, SwapChain
	{
		DXGI_MODE_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferDesc = bufferDesc;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = Hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
/*
	D3D11CreateDevice �Ű�����
		  pAdapter : �ش� �Լ��� ������ ��ġ�� ��Ÿ���� ���÷��� �����
		  DriverType : �Ϲ������δ� �������� 3���� �׷��� ���� ����ǵ��� D3D_Driver_Type_Hardware ����, Hardware �ܿ� Refference, software, warp ����
		    D3D_DRIVER_TYPE_HARDWARE : �Ϲ����� 3���� �׷��� ���� ����
		    D3D_DRIVER_TYPE_REFERENCE : ���� ǥ����ġ(refernce device) ����, ��Ȯ���� ��ǥ�� �ϴ� directx 3d ����Ʈ���� �����̸�, �׿� ���� �ص��� ����.
									  DirectX SDK�� �Բ� ��ġ�Ǹ� ������ �������� ������� -> ���� ������ �ش� �ɼ� �����ϸ� �ȵ�.
									  �Ϲ������� �ϵ��� �������� �ʴ� ��� �׽�Ʈ �뵵 Ȥ�� �������� ���� �׽�Ʈ ������ ���
		    D3D_DRIVER_TYPE_SOFTWARE : 3���� �ϵ��� �䳻���� ����Ʈ���� ������ ����. �����ڰ� �ش� ����Ʈ��� ���� �����ϰų� ������Ƽ�� �����ϴ� 
									  ����Ʈ���� �����Ⱑ �����ϴ� ��� ��� ����, DirectX 3D �� �����ϴ� ������� WARP �ϳ��� ����
		    D3D_DRIVER_TYPE_WARP : Direct3D 10.1 ����Ʈ���� ������ ����, WARP : Windows Advanced Rasterization Platform �� ����.
								   10.1�����̱� ������ directX11�� �������� ����=> ��� �� �� ����.
		  Software : ����Ʈ���� �����⸦ �����Ѵ�. �ϵ����� �������� �ش� �ɼ��� NULL.
		  Flags : �߰����� ��ġ ���� �÷����̸�, OR�� ������ �����ϴ�.
			-D3D11_CREATE_DEVICE_DEBUG: ����� ��忡�� ����� ���� Ȱ��ȭ�� �ش� �ɼ��� �ʿ��ϴ�.
			-D3D11_CREATE_DEVICE_SINGLETHREAD : Direct3D�� �������� �����忡�� ȣ����� �ʴ´ٰ� ����Ǵ� ��� �ش� �÷��� ������ ���� ���ȴ�. 
				�ش� �÷��� ����� ID3D11DEVICE::CreateDefferedContext ȣ���� �����Ѵ�.
		  pFeatureLevels : D3D_FEATURE_LEVEL ���� ���� �迭�� ���ҵ��� ������ ��� ������ �����ϴ� �����̴�. NULL ����� �����Ǵ� �ְ� ��� ���õȴ�.
		  FeatureLevels : pFeatureLevel�� ������ �����̴� NULL�� 0
		  SDKVersion : �׻� D3D11_SDK_VERSION ����
		  ppDevice : �Լ��� ������ ��ġ ��ȯ (c++�� ��ȯ���� �׻� �ϳ��̱� ������ �ش� ���� �����ؼ� ���ϴ� �� ��ȯ : ��¸Ű�����)
		  pFeatureLevel : pFeatureLevels �迭���� ó������ �����Ǵ� ���(pFeatureLevels�� NULL�� ��� �����Ǵ� ���� ���� ��� ���� ��ȯ, ��� �Ű�����)
		  ppImmediateContext : ������ ��ġ ���� ��ȯ

	CreateSwapChain �Ű�����
		  pDevice : ID3D11Device�� ����Ű�� ������
		  pDesc(pSwapChainDesc) : ��ȯ�罽 ���� ����ü�� ����Ű�� ������
		  ppSwapChain : ������ ��ȯ�罽 �������̽� ��ȯ

*/

		HRESULT hr = D3D11CreateDeviceAndSwapChain
	/*  (
			pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, 
			SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext
		)
	*/
		(
			NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &desc, &SwapChain, &Device, NULL, &DeviceContext
		);
		assert(SUCCEEDED(hr));
	}

	//Create BackBuffer
	{
		HRESULT hr;

		ID3D11Texture2D* BackBuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
		assert(SUCCEEDED(hr));

		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
		assert(SUCCEEDED(hr));
		BackBuffer->Release();

		DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	}

	//Create Shader
	{
		HRESULT hr;

		hr = D3DX11CompileFromFile(L"Effect.hlsl", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VsBlob, 0, 0);
		assert(SUCCEEDED(hr));

		hr = D3DX11CompileFromFile(L"Effect.hlsl", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PsBlob, 0, 0);
		assert(SUCCEEDED(hr));


		hr = Device->CreateVertexShader(VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(), NULL, &VertexShader);
		assert(SUCCEEDED(hr));

		hr = Device->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(), NULL, &PixelShader);
		assert(SUCCEEDED(hr));


		DeviceContext->VSSetShader(VertexShader, 0, 0);
		DeviceContext->PSSetShader(PixelShader, 0, 0);
	}

	//Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Width;
		viewport.Height = Height;

		DeviceContext->RSSetViewports(1, &viewport);
	}
}

void Destroy()
{
	VertexShader->Release();
	PixelShader->Release();
	VsBlob->Release();
	PsBlob->Release();

	SwapChain->Release();
	Device->Release();
	DeviceContext->Release();
	RTV->Release();
}

WPARAM Running()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	InitScene();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			Update();
			Render();
		}
	}
	DestroyScene();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				DestroyWindow(hwnd);
			}

			
		return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;


	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


