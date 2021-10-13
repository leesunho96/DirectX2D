#include "Device.h"

void InitScene()
{

}

void DestroyScene()
{
}

void Update()
{
}

void Render()
{
	// 색 표현을 위한 클래스, float형으로 구성, r, g, b, a로 구성. a 는 항상 1로 구성
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 1, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{

	}
	SwapChain->Present(0, 0);
}
