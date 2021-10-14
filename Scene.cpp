#include "Device.h"


// I : Interface, 생성값에 대한 구조체 존재. 
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


// 초기화 구간
// 프로그램 실행시 한번만 실행
void InitScene()
{
	// handle이 들어가면 메세지 박스가 자식으로 처리, 자식 창 처리 이전에는 프로그램 진행 되지 않음.
	// MessageBox(Hwnd, L"Init Scene to Test MessageBox", L"TestMB", MB_OK);
	// Hwnd 없이(hwnd에 null시) 진행시 메세지 박스와 프로그램 진행이 별개로 진행.
	// MessageBox(NULL, L"Init Scene", L"Test", MB_OK);
	// *bgColor = D3DXCOLOR(0, 0, 1, 1);


	Vertex vertices[6];
	vertices[0].Potision = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Potision = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Potision = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[3].Potision = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[4].Potision = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[5].Potision = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	// {} : 스택 프레임 생성, 스택 프레임은 지역 변수, 나가면 메모리 해제, 스택프레임 깨짐.

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		// Window에서 정상 처리 되었는지 결과값(반환값) 저장, 정상처리되었는지 확인
		HRESULT hr = Device->CreateBuffer(&desc, &data, &pVertexBuffer);
		// assert : 오류 검출
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




// 프로그램 종료시 실행
// 한번만 실행됨
void DestroyScene()
{
	pInputLayout->Release();
	pVertexBuffer->Release();
}

/*
더블 버퍼링
일반적으로 그리는 과정이 느림, 화면이 깜빡이는 현상 일어남.
=> 더블 버퍼링 이용해서 깜빡이는 현상 삭제. 모든 화면을 렌더링 한 후, 한번에 출력
=> Update(), Render()반복
*/

// 프로그램 내용 업데이트
void Update()
{
	

}

// 업데이트 한 내용 화면에 띄움
void Render()
{
	// 색 표현을 위한 클래스, float형으로 구성, r, g, b, a로 구성. a 는 항상 1로 구성

	bgColor = D3DXCOLOR(0.1f, 0.3f, 0.5f, 1);

	// Device : CPU

	// DeviceContext : GPU, RenderTarvetView : RTV,
	// 화면을 한번 지우고 난 후, 렌더링 실행.
	// 일반적으로 우리가 그리는 화면이 RTV, ClearRenderTargetView : 기존의 화면을 지움.
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		// stride : 한번에 넘길 데이터의 크기, Vertex 하나 넘길꺼니까 Sizeof(Vertex)
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		DeviceContext->IASetInputLayout(pInputLayout);
		DeviceContext->Draw(6, 0);

		//D3DXCOLOR triangleColor = D3DXCOLOR(0, 0, 0, 1);
		// 해당 도형 색 결정하는 메소드.
		//DeviceContext->ClearRenderTargetView(RTV, (float*)triangleColor);
	}


	// systemBuffer의 내용을 출력함.
	// 렌더링 한 내용을 실제 화면으로 띄워줌.
	SwapChain->Present(0, 0);
}

/*
정점(vertex) : 기본 도형의 꼭지점.
*/