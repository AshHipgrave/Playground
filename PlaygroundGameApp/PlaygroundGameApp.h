#pragma once

#include "..\CommonLib\GameBase.h"

GameBase* GameBase::m_pGameBase = nullptr;

class PlaygroundGameApp : public GameBase
{
public:
    PlaygroundGameApp(HINSTANCE AppInstance);
    ~PlaygroundGameApp();

    virtual bool Initialise() override;

private:
    virtual void OnResize() override;

    virtual void Update(const GameTimer& gameTimer) override;
    virtual void Draw(const GameTimer& gameTimer) override;
};

