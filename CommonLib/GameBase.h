#pragma once

#include <Windows.h>
#include <string>
#include <assert.h>

#include "GameTimer.h"

class GameBase
{
public:
	GameBase(HINSTANCE hAppInstance);
	virtual ~GameBase();

	GameBase(const GameBase& rhs) = delete;
	GameBase& operator=(const GameBase& rhs) = delete;

public:
	static GameBase* GetGameBase();

	HINSTANCE GetAppInstance() const;

	HWND GetMainWindowHandle() const;

	int Run();

	virtual bool Initialise();

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnResize() { }

	virtual void Update(const GameTimer& gameTimer) = 0;
	virtual void Draw(const GameTimer& gameTimer) = 0;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:
	bool InitMainWindow();
	bool InitVulkan();

protected:
	static GameBase* m_pGameBase;

	HINSTANCE m_hAppInstance = nullptr;

	HWND m_hMainWindowHandle = nullptr;

	bool m_bIsAppPaused = false;

	bool m_bIsMinimized = false;
	bool m_bIsMaximised = false;

	bool m_bIsResizing = false;

	GameTimer m_GameTimer;

	int m_ClientWindowWidth = 800;
	int m_ClientWindowHeight = 600;

	std::wstring m_MainWndCaption = L"GameBase MainWindow";
};

