
#include "Direct3DWindow.h"

#include <windows.h>
#include <cmath>
#include <algorithm>

Direct3DWindow::Direct3DWindow()
	:
	m_input(mouse, kbd)
{
	
}

Direct3DWindow::Direct3DWindow(int scrnWidth, int scrnHeight)
	:m_screenWidth(scrnWidth),m_screenHeight(scrnHeight),
	m_input(mouse,kbd)
{
	InitializeWindows();
	
}


Direct3DWindow::Direct3DWindow(const Direct3DWindow& other)
	:m_input(mouse, kbd)
{
}


Direct3DWindow::~Direct3DWindow()
{
	
}





void Direct3DWindow::Shutdown()
{
	// Release the application wrapper object.
	

	// Shutdown the window.
	ShutdownWindows();

	return;
}


_GameState Direct3DWindow::ProcessMessage()
{
	MSG msg;
	bool ok;

	
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	ok = true;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			state  = _GameState::exit;
		}
	}
	
	return state;
}

int Direct3DWindow::ScreenWidth()
{
	return m_screenWidth;
}

int Direct3DWindow::ScreenHeight()
{
	return m_screenHeight;
}

void Direct3DWindow::ScreenHalfExtents(float & x, float & y)
{
	
	x = (float)( m_screenWidth / 2);
	y = (float)( m_screenHeight / 2);
	
}

HWND & Direct3DWindow::WindowHandle()
{
	return m_hwnd;
}


bool Direct3DWindow::Frame()
{
	return true;
}


LRESULT CALLBACK Direct3DWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{


	switch (umsg)
	{
	case WM_KEYDOWN:
		if (!(lparam & 0x40000000) || kbd.AutorepeatIsEnabled()) // no thank you on the autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wparam));
		}
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wparam));
		break;

	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lparam);
		if (pt.x > 0 && pt.x < m_screenWidth && pt.y > 0 && pt.y < m_screenHeight)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hwnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				pt.x = std::max<short>(short(0), pt.x);
				pt.x = std::min<short>(short(m_screenWidth - 1), pt.x);
				pt.y = std::max<short>(short(0), pt.y);
				pt.y = std::min<short>(short(m_screenHeight - 1), pt.y);
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
				mouse.OnLeftReleased(pt.x, pt.y);
				mouse.OnRightReleased(pt.x, pt.y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	}
	
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void Direct3DWindow::InitializeWindows()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = APPLICATION_NAME;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	int screenWidth2 = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight2 = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
		m_screenWidth = screenWidth2;
		m_screenHeight = screenHeight2;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
	}


	RECT wr;
	wr.left = (long)posX;
	wr.right = m_screenWidth + wr.left;
	wr.top = (long)posY;
	wr.bottom = m_screenHeight + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	m_hwnd = CreateWindow(m_applicationName, m_applicationName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, m_hinstance, this);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	
	ShowCursor(true);

	return;
}


void Direct3DWindow::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	
	
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
	return 0;
}
