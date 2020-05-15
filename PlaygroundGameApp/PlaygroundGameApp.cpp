#include "PlaygroundGameApp.h"

PlaygroundGameApp::PlaygroundGameApp(HINSTANCE AppInstance)
	: GameBase(AppInstance)
{
}

PlaygroundGameApp::~PlaygroundGameApp()
{
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(showCmd);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	PlaygroundGameApp playgroundApp(hInstance);

	if (!playgroundApp.Initialise())
	{
		::MessageBox(0, L"App initialisation failed", 0, 0);
		return 0;
	}

	return playgroundApp.Run();
}

bool PlaygroundGameApp::Initialise()
{
	if (!GameBase::Initialise())
	{
		return false;
	}
	
	// TODO: Additional Init logic here.

	return true;
}

void PlaygroundGameApp::OnResize()
{
	GameBase::OnResize();
}

void PlaygroundGameApp::Update(const GameTimer& gameTimer)
{
	// TODO: Update logic here.
}

void PlaygroundGameApp::Draw(const GameTimer& gameTimer)
{
	//TODO: Draw logic here.
}
