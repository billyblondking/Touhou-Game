﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include "NonGameplayScenes/KnowledgeBaseScene.h"
#include "NonGameplayScenes/HomeScene.h"
#include "NonGameplayScenesCache.h"
#include "PlaceHolder.h"
// #include "resources.h.dir/knowledge_base.h"

#include "AudioController.h"
#include "ui/CocosGUI.h"
using namespace ui;

// 静态数据成员必须在类定义 *外* 进行初始化
// 为保证编译时静态数据成员最后只存在于一个目标文件中
// 这个定义也不能写入 .h 文件中，放在对应的 .cpp 文件的开头是最好选择
const std::string KnowledgeBaseScene::TAG{ "KnowledgeBaseScene" };

KnowledgeBaseScene::KnowledgeBaseScene()
{
    _visibleSize = _director->getVisibleSize();
}

bool
KnowledgeBaseScene::init()
{
    /*  1. super init first */

    if (!Scene::init()) {
        return false;
    }

/*  2. debug info */

#ifndef NDEBUG
    auto sceneTag = Label::createWithTTF("KnowledgeBaseScene", "fonts/arial.ttf", 16);
    sceneTag->setAnchorPoint(Vec2(0, 1));
    sceneTag->setPosition(Vec2(0, _visibleSize.height));
    sceneTag->setColor(Color3B::WHITE);
    this->addChild(sceneTag);
#endif

    /*  3. PlaceHolder */

    auto p = PlaceHolder::createCircle(100, "KnowledgeBaseScene");
    p->setPosition(_visibleSize / 2);
    this->addChild(p);

    auto BackButton = Button::create();
    BackButton->setTitleText("返回");
    BackButton->setTitleFontName("fonts/dengxian.ttf");
    BackButton->setTitleColor(Color3B(194, 134, 11));
    BackButton->setTitleFontSize(50);
    BackButton->setAnchorPoint(Vec2(0, 0));
    BackButton->setPosition(Vec2(_visibleSize.width * 0.8, _visibleSize.height * 0.43));
    BackButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            Director::getInstance()->popScene();
        }
    });
    addChild(BackButton);

    return true;
}
