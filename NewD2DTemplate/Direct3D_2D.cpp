#include "Direct3D_2D.h"

#include "Direct3DWindow.h"
XMMATRIX Direct3D::m_projectionMatrix = XMMatrixIdentity();
XMMATRIX Direct3D::m_viewMatrix = XMMatrixIdentity();

HRESULT Direct3D::InvokeScreenAdapter()
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;



	HRESULT result = S_OK;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return result;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return result;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return result;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return result;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return E_FAIL;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return result;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (UINT)m_screenWidth)
		{
			if (displayModeList[i].Height == (UINT)m_screenHeight)
			{
				m_numerator = (UINT)displayModeList[i].RefreshRate.Numerator;
				m_denominator = (UINT)displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return result;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.

	int error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return E_FAIL;
	}

	// Release objects.

	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	return result;
}

HRESULT Direct3D::InvokeDevices()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = m_windowHandle;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (m_fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_DESC swapChainDesc2 = {
		{ m_screenWidth, m_screenHeight,{ 60, 1 }, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED },
		{ m_numerator, m_denominator },
		DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
		1,
		m_windowHandle,

		true,
		DXGI_SWAP_EFFECT_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL featureLevelArray[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT featureLevelArraySize = ARRAYSIZE(featureLevelArray);
	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,//,
		featureLevelArray,
		featureLevelArraySize,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		&featureLevel,
		&m_pImmediateContext);
	if (FAILED(result)) { return result; }

	// Get the pointer to the back buffer.
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result)) { return result; }

	// Create the render target view with the back buffer pointer.
	result = m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView);
	if (FAILED(result)) { return result; }

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_screenWidth;
	depthBufferDesc.Height = m_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result)) { return result; }

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) { return result; }

	// Set the depth stencil state.
	m_pImmediateContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_pDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result)) { return result; }

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result)) { return result; }

	// Now set the rasterizer state.
	m_pImmediateContext->RSSetState(m_rasterState.Get());

	// Setup a raster description which turns off back face culling.
	rasterDesc.CullMode = D3D11_CULL_NONE;

	// Create the no culling rasterizer state.
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling);
	if (FAILED(result)) { return result; }

	// Setup a raster description which enables wire frame rendering.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the wire frame rasterizer state.
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateWireframe);
	if (FAILED(result)) { return result; }
	return S_OK;
}

void Direct3D::InvokeViewportMatrices()
{

	m_Viewport.Width = (float)m_screenWidth;
	m_Viewport.Height = (float)m_screenHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)m_screenWidth / (float)m_screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screenNear, m_screenDepth);



	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)m_screenWidth, (float)m_screenHeight, m_screenNear, m_screenDepth);


}

HRESULT Direct3D::InvokeDepthStencilAndBlendStates()
{
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;
	HRESULT result;
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result)) { return result; }

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.AlphaToCoverageEnable = FALSE;
	blendStateDescription.IndependentBlendEnable = false;
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result)) { return result; }


	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = false;
	blendStateDescription.AlphaToCoverageEnable = false;

	// Create the blend state using the description.
	result = m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result)) { return result; }

	// Create a blend state description for the alpha-to-coverage blending mode.
	blendStateDescription.AlphaToCoverageEnable = true;
	blendStateDescription.IndependentBlendEnable = false;
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState2);
	if (FAILED(result))
	{
		return false;
	}
	return true;
	return E_NOTIMPL;
}
/////////////////////////////////////////////////////////
// Direct 2D
//////////////////////////////////////////////////////////
HRESULT Direct3D::InvokeD2DDeviceIndependentResources()
{
	auto options = D2D1_FACTORY_OPTIONS();
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	return D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, m_pD2DFactory.GetAddressOf());

	//return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2DFactory.GetAddressOf());
}
HRESULT Direct3D::InvokeD2DDeviceResources()
{
	HRESULT result = S_OK;
	if (!m_pD2DRenderTarget)
	{
		Microsoft::WRL::ComPtr<IDXGISurface> backBufferSurface;

		// Get a DXGI surface for D2D use.
		result = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferSurface));

		// DXGI_FORMAT_UNKNOWN will cause it to use the same format as the back buffer (R8G8B8A8_UNORM)
		D2D1_RENDER_TARGET_PROPERTIES d2dRTProps = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 0, 0);

		// Wraps up our DXGI surface in a D2D render target.
		result = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(backBufferSurface.Get(), &d2dRTProps, &m_pD2DRenderTarget);
		if (FAILED(result)) {
			return result;
		}


		result = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LimeGreen), &m_pD2DWhiteBrush);
		if (FAILED(result)) {
			return result;
		}
	}

	return result;
}
void Direct3D::ReleaseD2DDeviceResources()
{
	SAFE_RELEASE(m_pD2DRenderTarget);
	SAFE_RELEASE(m_pD2DWhiteBrush);

}
// public functions
Direct3D::Direct3D(UINT scrnWidth, UINT scrnHeight, HWND& hwnd, bool vsync, bool fullscreen, float scrnDepth, float scrnNear)
	:
	m_screenWidth(scrnWidth),
	m_screenHeight(scrnHeight),
	m_vsync_enabled(vsync),
	m_fullscreen(fullscreen),
	m_screenNear(scrnNear),
	m_screenDepth(scrnDepth),
	m_windowHandle(hwnd)
{
	HRESULT hr;
	if (FAILED(hr = InvokeScreenAdapter()))
	{
		assert(hr == S_OK);
		//throw CHILI_GFX_EXCEPTION(hr, L"Failed to set up Screen adapter");
	}
	if (FAILED(hr = InvokeDevices()))
	{
		assert(hr == S_OK);
		//throw CHILI_GFX_EXCEPTION(hr, L"Failed to create one of the following :\nDevice : swap chain : raster states");
	}
	InvokeViewportMatrices();
	if (FAILED(hr = InvokeDepthStencilAndBlendStates()))
	{
		assert(hr == S_OK);
		//throw CHILI_GFX_EXCEPTION(hr, L"Failed to create Depth stencil or raster states");
	}

	////////////////////////
	// Direct 2D
	//////////////////////////
	if (FAILED(hr = InvokeD2DDeviceIndependentResources()))
	{
		assert(hr == S_OK);
		//throw CHILI_GFX_EXCEPTION(hr, L"Failed to create Direct2D Factory ");
	}
	if (FAILED(hr = InvokeD2DDeviceResources()))
	{
		assert(hr == S_OK);
		//throw CHILI_GFX_EXCEPTION(hr, L"Failed to create Direct2D Resources ");
	}



}
Direct3D::~Direct3D()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_depthStencilView);
	ReleaseD2DDeviceResources();
}


XMMATRIX Direct3D::GetProjectionMatrix()
{
	return  m_projectionMatrix;

}
void Direct3D::GetWorldMatrix(XMMATRIX& worldMatrix)
{

}
void Direct3D::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}
void Direct3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}
void Direct3D::EnableZBuffer()
{
	m_pImmediateContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	return;
}
void Direct3D::DisableZBuffer()
{
	m_pImmediateContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
	return;
}
void Direct3D::EnableAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_pImmediateContext->OMSetBlendState(m_alphaEnableBlendingState.Get(), blendFactor, 0xffffffff);

	return;
}
void Direct3D::DisableAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_pImmediateContext->OMSetBlendState(m_alphaDisableBlendingState.Get(), blendFactor, 0xffffffff);

	return;
}
void Direct3D::EnableCulling()
{
	// Set the culling rasterizer state.
	m_pImmediateContext->RSSetState(m_rasterState.Get());

	return;
}
void Direct3D::DisableCulling()
{
	// Set the no back face culling rasterizer state.
	m_pImmediateContext->RSSetState(m_rasterStateNoCulling.Get());

	return;
}
void Direct3D::EnableAlphaToCoverageBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_pImmediateContext->OMSetBlendState(m_alphaEnableBlendingState2.Get(), blendFactor, 0xffffffff);

	return;
}
D3D11_VIEWPORT Direct3D::GetViewport()
{
	return m_Viewport;
}
void Direct3D::EnableWireframe()
{
	// Set the wire frame rasterizer state.
	m_pImmediateContext->RSSetState(m_rasterStateWireframe.Get());

	return;
}
void Direct3D::DisableWireframe()
{
	// Set the solid fill rasterizer state.
	m_pImmediateContext->RSSetState(m_rasterState.Get());

	return;
}
