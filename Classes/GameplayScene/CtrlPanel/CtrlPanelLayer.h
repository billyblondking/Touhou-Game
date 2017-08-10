﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef CTRL_PANEL_LAYER_H
#define CTRL_PANEL_LAYER_H

#include "cocos2d.h"

USING_NS_CC;

class GameData;

class CtrlPanelLayer : public Layer
{
public:
    CREATE_FUNC(CtrlPanelLayer)

    bool init() override;

public:
    // `onEventXXX' 的前缀表明这个组件会接收和处理哪些事件
    void onEventIncreasePlayerHP(EventCustom*);
    void onEventDecreasePlayerHP(EventCustom*);
    void onEventIncreasePlayerMana(EventCustom*);
    void onEventDecreasePlayerMana(EventCustom*);

private:
    CtrlPanelLayer() = default;

    void initCharacterPanelUIAndListener();
    void initTouchListener();
    void initKeyboardListener();
    void initCustomEventListener();

private:
    GameData* _gamedata;
    Size _visibleSize;

    Layer* _characterCtrlPanel[2];
    int _currCharacterIdx;
};

#endif // CTRL_PANEL_LAYER_H