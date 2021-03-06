﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef LOGOANDDISCLAIMERSCENE_H
#define LOGOANDDISCLAIMERSCENE_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class LogoAndDisclaimerScene : public Scene
{
public:
    CREATE_FUNC(LogoAndDisclaimerScene);

    virtual bool init();

private:
    LogoAndDisclaimerScene();

    void switchToJumpTableScene();
    void switchToBackgroundIntroScene();

private:
    // intorspection
    Size _visibleSize;

    // game logic data
    float _logoLast = 1.0f;
    float _disclaimerLast = 1.0f;
};

#endif // LOGOANDDISCLAIMERSCENE_H
