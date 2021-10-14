#include "Device.h"


// I : Interface, �������� ���� ����ü ����. 
ID3D11Buffer* pVertexBuffer = nullptr;
ID3D11InputLayout* pInputLayout = nullptr;
D3D11_INPUT_ELEMENT_DESC InputElementDesc[] = 
{
	{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


struct Vertex
{
	D3DXVECTOR3 Potision;
};


// �ʱ�ȭ ����
// ���α׷� ����� �ѹ��� ����
void InitScene()
{
	// handle�� ���� �޼��� �ڽ��� �ڽ����� ó��, �ڽ� â ó�� �������� ���α׷� ���� ���� ����.
	// MessageBox(Hwnd, L"Init Scene to Test MessageBox", L"TestMB", MB_OK);
	// Hwnd ����(hwnd�� null��) ����� �޼��� �ڽ��� ���α׷� ������ ������ ����.
	// MessageBox(NULL, L"Init Scene", L"Test", MB_OK);
	// *bgColor = D3DXCOLOR(0, 0, 1, 1);


	Vertex vertices[6];
	vertices[0].Potision = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Potision = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Potision = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[3].Potision = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[4].Potision = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[5].Potision = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	// {} : ���� ������ ����, ���� �������� ���� ����, ������ �޸� ����, ���������� ����.

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		// Window���� ���� ó�� �Ǿ����� �����(��ȯ��) ����, ����ó���Ǿ����� Ȯ��
		HRESULT hr = Device->CreateBuffer(&desc, &data, &pVertexBuffer);
		// assert : ���� ����
		assert(SUCCEEDED(hr));
	}


	// Create InputLayout
	{
		HRESULT hr = Device->CreateInputLayout
		(
			InputElementDesc,
			ARRAYSIZE(InputElementDesc),
			VsBlob->GetBufferPointer(),
			VsBlob->GetBufferSize(),
			&pInputLayout
		);
		assert(SUCCEEDED(hr));
	}


}




// ���α׷� ����� ����
// �ѹ��� �����
void DestroyScene()
{
	pInputLayout->Release();
	pVertexBuffer->Release();
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

	bgColor = D3DXCOLOR(0.1f, 0.3f, 0.5f, 1);

	// Device : CPU

	// DeviceContext : GPU, RenderTarvetView : RTV,
	// ȭ���� �ѹ� ����� �� ��, ������ ����.
	// �Ϲ������� �츮�� �׸��� ȭ���� RTV, ClearRenderTargetView : ������ ȭ���� ����.
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		// stride : �ѹ��� �ѱ� �������� ũ��, Vertex �ϳ� �ѱ沨�ϱ� Sizeof(Vertex)
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		DeviceContext->IASetInputLayout(pInputLayout);
		DeviceContext->Draw(6, 0);

		//D3DXCOLOR triangleColor = D3DXCOLOR(0, 0, 0, 1);
		// �ش� ���� �� �����ϴ� �޼ҵ�.
		//DeviceContext->ClearRenderTargetView(RTV, (float*)triangleColor);
	}


	// systemBuffer�� ������ �����.
	// ������ �� ������ ���� ȭ������ �����.
	SwapChain->Present(0, 0);
}

/*
����(vertex) : �⺻ ������ ������.
*/