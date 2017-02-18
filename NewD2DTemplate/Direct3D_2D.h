
#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <assert.h>
using namespace Microsoft;
using namespace WRL;
using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")



#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


class Direct3D
{
private:
	HRESULT InvokeScreenAdapter();
	HRESULT InvokeDevices();
	void InvokeViewportMatrices();
	HRESULT InvokeDepthStencilAndBlendStates();


	HWND& m_windowHandle;
	UINT m_numerator = 0;
	UINT m_denominator = 0;
	float m_screenNear = 0.1f;
	float m_screenDepth = 1000.0f;
	bool m_fullscreen = false;
	D3D11_VIEWPORT m_Viewport;

protected:
	static XMMATRIX m_projectionMatrix;
	static XMMATRIX m_viewMatrix;
	XMMATRIX m_orthoMatrix;

	int m_videoCardMemory;
	char m_videoCardDescription[128];
	bool m_vsync_enabled;
	UINT m_screenWidth;
	UINT m_screenHeight;

	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ComPtr<ID3D11RasterizerState> m_rasterState;
	ComPtr<ID3D11RasterizerState> m_rasterStateNoCulling;
	ComPtr<ID3D11RasterizerState> m_rasterStateWireframe;
	ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	ComPtr<ID3D11BlendState> m_alphaEnableBlendingState;
	ComPtr<ID3D11BlendState> m_alphaDisableBlendingState;
	ComPtr<ID3D11BlendState> m_alphaEnableBlendingState2;

protected:
	//////////////////////////////////
	// Direct 2D sharing swap chain buffer with Direct 3D 
	///////////////////////////////////
	ComPtr<ID2D1Factory> m_pD2DFactory;
	ID2D1SolidColorBrush* m_pD2DWhiteBrush = nullptr;
	ID2D1RenderTarget* m_pD2DRenderTarget = nullptr;
	HRESULT InvokeD2DDeviceResources();
	void ReleaseD2DDeviceResources();
private:
	HRESULT InvokeD2DDeviceIndependentResources();
public:
	__inline static void SetViewMatrix(XMMATRIX mat)
	{
		m_viewMatrix = mat;
	}
	__inline static XMMATRIX ViewMatrix() { return m_viewMatrix; }
public:
	Direct3D(UINT scrnWidth, UINT scrnHeight, HWND& hwnd, bool vsync, bool fullscreen, float scrnDepth, float scrnNear);
	~Direct3D();

	
	
	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	D3D11_VIEWPORT GetViewport();
	XMMATRIX GetProjectionMatrix();
	void GetWorldMatrix(XMMATRIX& worldMatrix);
	void GetOrthoMatrix(XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardName, int& memory);

	void EnableZBuffer();
	void DisableZBuffer();

	void EnableCulling();
	void DisableCulling();

	void EnableAlphaBlending();
	void EnableAlphaToCoverageBlending();
	void DisableAlphaBlending();

	void EnableWireframe();
	void DisableWireframe();

};
