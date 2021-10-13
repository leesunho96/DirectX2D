#include "Device.h"

// 초기화 구간
// 프로그램 실행시 한번만 실행
void InitScene()
{
	// handle이 들어가면 메세지 박스가 자식으로 처리, 자식 창 처리 이전에는 프로그램 진행 되지 않음.
	MessageBox(Hwnd, L"Init Scene", L"Test", MB_OK);
	// Hwnd 없이(hwnd에 null시) 진행시 메세지 박스와 프로그램 진행이 별개로 진행.
	//MessageBox(NULL, L"Init Scene", L"Test", MB_OK);
}




// 프로그램 종료시 실행
// 한번만 실행됨
void DestroyScene()
{
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
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 1, 1);

	// Device : CPU

	// DeviceContext : GPU, RenderTarvetView : RTV,
	// 화면을 한번 지우고 난 후, 렌더링 실행.
	// 일반적으로 우리가 그리는 화면이 RTV, ClearRenderTargetView : 기존의 화면을 지움.
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{

	}


	// systemBuffer의 내용을 출력함.
	// 렌더링 한 내용을 실제 화면으로 띄워줌.
	SwapChain->Present(0, 0);
}

