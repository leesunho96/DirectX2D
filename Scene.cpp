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
	// �� ǥ���� ���� Ŭ����, float������ ����, r, g, b, a�� ����. a �� �׻� 1�� ����
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 1, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{

	}
	SwapChain->Present(0, 0);
}
