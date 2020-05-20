#include "GameBase.h"

#include <windowsx.h>

GameBase::GameBase(HINSTANCE hAppInstance)
	: m_hAppInstance(hAppInstance)
{
	assert(GameBase::m_pGameBase == nullptr);

	GameBase::m_pGameBase = this;
}

GameBase::~GameBase()
{
}

GameBase* GameBase::GetGameBase()
{
	return GameBase::m_pGameBase;
}

HINSTANCE GameBase::GetAppInstance() const
{
	return m_hAppInstance;
}

HWND GameBase::GetMainWindowHandle() const
{
	return m_hMainWindowHandle;
}

int GameBase::Run()
{
	MSG msg = { 0 };

	m_GameTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			m_GameTimer.Tick();

			if (!m_bIsAppPaused)
			{
				Update(m_GameTimer);
				Draw(m_GameTimer);
			}
			else
			{
				::Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

bool GameBase::Initialise()
{
	if (!InitMainWindow())
	{
		return false;
	}

	if (!InitGraphics())
	{
		return false;
	}

	OnResize();

	return true;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return GameBase::GetGameBase()->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT GameBase::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_bIsAppPaused = true;
				m_GameTimer.Stop();
			}
			else
			{
				m_bIsAppPaused = false;
				m_GameTimer.Start();
			}
			return 0;
		}
		case WM_SIZE:
		{
			m_ClientWindowWidth = LOWORD(lParam);
			m_ClientWindowHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				m_bIsAppPaused = true;
				m_bIsMinimized = true;

				m_bIsMaximised = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_bIsAppPaused = false;
				m_bIsMinimized = false;

				m_bIsMaximised = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_bIsMinimized)
				{
					m_bIsAppPaused = false;
					m_bIsMinimized = false;

					OnResize();
				}
				else if (m_bIsMaximised)
				{
					m_bIsAppPaused = false;
					m_bIsMaximised = false;

					OnResize();
				}
				else if (m_bIsResizing)
				{
					// Do Nothing. Wait for the user to finish resizing the window, at which point WM_EXITSIZEMOVE is sent and we can correctly handle the new Window size.
					// Whilst I could combine this with the below to be: 'if (!m_bIsReiszing) { OnResize(); }' It's slightly more readable to know we're deliberately not doing anything (Also looks odd to call 'OnResize()' right after checking '!Resizing').
				}
				else
				{
					OnResize();
				}
			}
			return 0;
		}
		case WM_ENTERSIZEMOVE:
		{
			m_bIsAppPaused = true;
			m_bIsResizing = true;

			m_GameTimer.Stop();

			return 0;
		}
		case WM_EXITSIZEMOVE:
		{
			m_bIsAppPaused = false;
			m_bIsResizing = false;

			m_GameTimer.Start();

			OnResize();

			return 0;
		}
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		case WM_MENUCHAR:
		{
			return MAKELRESULT(0, MNC_CLOSE);
		}
		case WM_GETMINMAXINFO:
		{
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;

			return 0;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}		
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		case WM_KEYUP:
		{
			if (wParam == VK_ESCAPE)
			{
				::PostQuitMessage(0);
			}

			return 0;
		}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void GameBase::OnResize()
{
	if (m_VulkanInstance.IsInitialised())
	{
		m_VulkanInstance.RecreateSwapChain();
	}
}

bool GameBase::InitMainWindow()
{
	WNDCLASS windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = m_hAppInstance;
	windowClass.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	windowClass.hCursor = ::LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = L"PlayGroundGameWindow";

	if (!::RegisterClass(&windowClass))
	{
		::MessageBox(0, L"Failed to register WindowClass", 0, MB_ICONERROR);
		return false;
	}

	RECT windowRect = { 0, 0, m_ClientWindowWidth, m_ClientWindowHeight };

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;

	m_hMainWindowHandle = ::CreateWindow(L"PlayGroundGameWindow", m_MainWndCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInstance, 0);

	if (!m_hMainWindowHandle)
	{
		::MessageBox(0, L"Failed to create main window", 0, MB_ICONERROR);
		return false;
	}

	::ShowWindow(m_hMainWindowHandle, SW_SHOW);
	::UpdateWindow(m_hMainWindowHandle);

	return true;
}

bool GameBase::InitGraphics()
{
	assert(!m_VulkanInstance.IsInitialised());

	if (!m_VulkanInstance.InitVulkan(m_hMainWindowHandle))
	{
		::MessageBox(0, L"Failed to create Vulkan instance!", 0, MB_ICONERROR);
		return false;
	}

	return true;
}
