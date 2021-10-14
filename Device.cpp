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
//Device :  CPU, 연산처리
ID3D11Device* Device;
//DeviceContext : GPU, 화면에 그리는 연산.
ID3D11DeviceContext* DeviceContext;
ID3D11RenderTargetView* RTV;

/*
	DX10부터 완벽하게 CPU, GPU 분리.
	그 통신은 RAM - VRAM을 이용
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
	D3D11CreateDevice 매개변수
		  pAdapter : 해당 함수로 생성할 장치를 나타내는 디스플레이 어댑터
		  DriverType : 일반적으로는 렌더링에 3차원 그래픽 가속 적용되도록 D3D_Driver_Type_Hardware 적용, Hardware 외에 Refference, software, warp 존재
		    D3D_DRIVER_TYPE_HARDWARE : 일반적인 3차원 그래픽 가속 적용
		    D3D_DRIVER_TYPE_REFERENCE : 소위 표준장치(refernce device) 적용, 정확성을 목표로 하는 directx 3d 소프트웨어 구현이며, 그에 따라 극도로 느림.
									  DirectX SDK와 함께 설치되며 개발자 전용으로 만들어짐 -> 실제 배포시 해당 옵션 적용하면 안됨.
									  일반적으로 하드웨어가 지원하지 않는 기능 테스트 용도 혹은 구동기의 버그 테스트 용으로 사용
		    D3D_DRIVER_TYPE_SOFTWARE : 3차원 하드웨어를 흉내내는 소프트웨어 구동시 생성. 개발자가 해당 소프트웨어를 직접 구현하거나 서드파티에 존재하는 
									  소프트웨어 구동기가 존재하는 경우 사용 가능, DirectX 3D 가 제공하는 구동기는 WARP 하나만 존재
		    D3D_DRIVER_TYPE_WARP : Direct3D 10.1 소프트웨어 구동기 생성, WARP : Windows Advanced Rasterization Platform 의 약자.
								   10.1버전이기 때문에 directX11은 지원하지 않음=> 사용 할 수 없다.
		  Software : 소프트웨어 구동기를 지정한다. 하드웨어로 렌더링시 해당 옵션은 NULL.
		  Flags : 추가적인 장치 생성 플래그이며, OR로 결합이 가능하다.
			-D3D11_CREATE_DEVICE_DEBUG: 디버그 모드에서 디버그 계층 활성화시 해당 옵션이 필요하다.
			-D3D11_CREATE_DEVICE_SINGLETHREAD : Direct3D가 여러개의 스레드에서 호출되지 않는다고 보장되는 경우 해당 플래그 생성시 성능 향상된다. 
				해당 플래그 선언시 ID3D11DEVICE::CreateDefferedContext 호출은 실패한다.
		  pFeatureLevels : D3D_FEATURE_LEVEL 형식 원소 배열로 원소들의 순서가 기능 수준을 점검하는 순서이다. NULL 선언시 지원되는 최고 기능 선택된다.
		  FeatureLevels : pFeatureLevel의 원소의 개수이다 NULL시 0
		  SDKVersion : 항상 D3D11_SDK_VERSION 선언
		  ppDevice : 함수가 생성한 장치 반환 (c++의 반환값은 항상 하나이기 떄문에 해당 값을 참조해서 원하는 값 반환 : 출력매개변수)
		  pFeatureLevel : pFeatureLevels 배열에서 처음으로 지원되는 기능(pFeatureLevels가 NULL인 경우 지원되는 가장 높은 기능 수준 반환, 출력 매개변수)
		  ppImmediateContext : 생성된 장치 문맥 반환

	CreateSwapChain 매개변수
		  pDevice : ID3D11Device를 가리키는 포인터
		  pDesc(pSwapChainDesc) : 교환사슬 서술 구조체를 가리키는 포인터
		  ppSwapChain : 생성된 교환사슬 인터페이스 반환

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


