#include "Device.h"

// �ʱ�ȭ ����
// ���α׷� ����� �ѹ��� ����
void InitScene()
{
	// handle�� ���� �޼��� �ڽ��� �ڽ����� ó��, �ڽ� â ó�� �������� ���α׷� ���� ���� ����.
	MessageBox(Hwnd, L"Init Scene", L"Test", MB_OK);
	// Hwnd ����(hwnd�� null��) ����� �޼��� �ڽ��� ���α׷� ������ ������ ����.
	MessageBox(NULL, L"Init Scene", L"Test", MB_OK);
	//*bgColor = D3DXCOLOR(0, 0, 1, 1);
}




// ���α׷� ����� ����
// �ѹ��� �����
void DestroyScene()
{
}

/*
���� ���۸�
�Ϲ������� �׸��� ������ ����, ȭ���� �����̴� ���� �Ͼ.
=> ���� ���۸� �̿��ؼ� �����̴� ���� ����. ��� ȭ���� ������ �� ��, �ѹ��� ���
=> Update(), Render()�ݺ�
*/

// ���α׷� ���� ������Ʈ
void Update()
{
	//bgColor = bgColor == D3DXCOLOR(0, 0, 1, 1) ? D3DXCOLOR(255, 255, 255, 1) : D3DXCOLOR(0, 0, 1, 1);

}

// ������Ʈ �� ���� ȭ�鿡 ���
void Render()
{
	// �� ǥ���� ���� Ŭ����, float������ ����, r, g, b, a�� ����. a �� �׻� 1�� ����

	bgColor = D3DXCOLOR(0.1, 0.3, 0.5, 1);

	// Device : CPU

	// DeviceContext : GPU, RenderTarvetView : RTV,
	// ȭ���� �ѹ� ����� �� ��, ������ ����.
	// �Ϲ������� �츮�� �׸��� ȭ���� RTV, ClearRenderTargetView : ������ ȭ���� ����.
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{

	}


	// systemBuffer�� ������ �����.
	// ������ �� ������ ���� ȭ������ �����.
	SwapChain->Present(0, 0);
}

