#include "Device.h"

// �ʱ�ȭ ����
// ���α׷� ����� �ѹ��� ����
void InitScene()
{
	// handle�� ���� �޼��� �ڽ��� �ڽ����� ó��, �ڽ� â ó�� �������� ���α׷� ���� ���� ����.
	MessageBox(Hwnd, L"Init Scene", L"Test", MB_OK);
	// Hwnd ����(hwnd�� null��) ����� �޼��� �ڽ��� ���α׷� ������ ������ ����.
	//MessageBox(NULL, L"Init Scene", L"Test", MB_OK);
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

}

// ������Ʈ �� ���� ȭ�鿡 ���
void Render()
{
	// �� ǥ���� ���� Ŭ����, float������ ����, r, g, b, a�� ����. a �� �׻� 1�� ����
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 1, 1);

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

