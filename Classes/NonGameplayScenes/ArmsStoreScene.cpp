﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include "NonGameplayScenes/ArmsStoreScene.h"
#include "Layers/SettingsLayer.h"
#include "NonGameplayScenes/ArmsStorePurchaseScene.h"
#include "NonGameplayScenes/EquipScene.h"
#include "NonGameplayScenes/InventoryScene.h"
#include "NonGameplayScenes/KnowledgeBaseScene.h"
#include "NonGameplayScenes/LocationSelectScene.h"
#include "NonGameplayScenesCache.h"
#include "PlaceHolder.h"

#include "GameData/Conversation.h"
#include "GameData/GameData.h"
#include "GameData/Location.h"

#include "resources.h.dir/arms_store.h"

#include "AudioController.h"

#include "ui/CocosGUI.h"
using namespace ui;

// 静态数据成员必须在类定义 *外* 进行初始化
// 为保证编译时静态数据成员最后只存在于一个目标文件中
// 这个定义也不能写入 .h 文件中，放在对应的 .cpp 文件的开头是最好选择
const std::string ArmsStoreScene::TAG{ "ArmsStoreScene" };

ArmsStoreScene::ArmsStoreScene()
{
    _visibleSize = _director->getVisibleSize();
}

bool
ArmsStoreScene::init()
{
    /*  1. super init first */

    if (!Scene::init()) {
        return false;
    }

/*  2. debug info */

#ifndef NDEBUG
    auto sceneTag = Label::createWithTTF("ArmsStoreScene", "fonts/arial.ttf", 16);
    sceneTag->setAnchorPoint(Vec2(0, 1));
    sceneTag->setPosition(Vec2(0, _visibleSize.height));
    sceneTag->setColor(Color3B::WHITE);
    this->addChild(sceneTag);
#endif

    auto location = GameData::getInstance()->getLocationByTag("ArmsStore");
    /*背景*/
    auto background = Sprite::create(location.backgroundPicture);
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setContentSize(_visibleSize);
    background->setPosition(_visibleSize / 2);
    this->addChild(background);

    /*购买按钮*/
    auto button_start = Button::create(IMG_ARMS_STORE_BIG_BUTTON);
    button_start->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    button_start->setPosition(Vec2(_visibleSize.width * 0.743, _visibleSize.height * 0.705));
    button_start->setTitleText("购买");
    button_start->setTitleFontSize(25);
    button_start->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            TransitionScene* transition =
                TransitionFade::create(0.5f, ArmsStorePurchaseScene::create());
            Director::getInstance()->pushScene(transition);
        }
    });
    addChild(button_start);

    /*整备按钮*/
    auto button_equip = Button::create(IMG_ARMS_STORE_BIG_BUTTON);
    button_equip->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    button_equip->setPosition(Vec2(_visibleSize.width * 0.743, _visibleSize.height * 0.525));
    button_equip->setTitleText("整备");
    button_equip->setTitleFontSize(25);
    button_equip->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            TransitionScene* transition = TransitionFade::create(0.5f, EquipScene::create());
            Director::getInstance()->pushScene(transition);
        }
    });
    addChild(button_equip);

    /*道具库按钮*/
    auto button_inventory = Button::create(IMG_ARMS_STORE_BIG_BUTTON);
    button_inventory->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    button_inventory->setPosition(Vec2(_visibleSize.width * 0.743, _visibleSize.height * 0.345));
    button_inventory->setTitleText("道具库");
    button_inventory->setTitleFontSize(25);
    button_inventory->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            TransitionScene* transition = TransitionFade::create(0.5f, InventoryScene::create());
            Director::getInstance()->pushScene(transition);
        }
    });
    addChild(button_inventory);

    /*其他地图按钮*/
    auto button_map = Button::create(IMG_ARMS_STORE_BIG_BUTTON);
    button_map->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    button_map->setPosition(Vec2(_visibleSize.width * 0.743, _visibleSize.height * 0.165));
    button_map->setTitleText("前往其他地图");
    button_map->setTitleFontSize(25);
    button_map->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            TransitionScene* transition =
                TransitionFade::create(0.5f, LocationSelectScene::create());
            Director::getInstance()->pushScene(transition);
        }
    });
    addChild(button_map);

    /*地点,进度...等背景*/
    auto info_bg = Sprite::create(IMG_ARMS_STORE_LITTLE_BUTTON_LAYOUT);
    info_bg->setAnchorPoint(Vec2(1, 1));
    info_bg->setContentSize(Size(480, 120));
    info_bg->setOpacity(220);
    info_bg->setPosition(Vec2(_visibleSize.width * 0.98, _visibleSize.height * 0.98));
    this->addChild(info_bg);

    /*钱币*/
    auto coin = Sprite::create("item/coin.png");
    coin->setAnchorPoint(Vec2(0.5, 0.5));
    coin->setPosition(Vec2(50, 60));
    info_bg->addChild(coin);

    money_text = Label::create("", "fonts/NotoSansCJKsc-Black.otf", 20);
    money_text->setAnchorPoint(Vec2(0.5, 0.5));
    money_text->setColor(Color3B::WHITE);
    money_text->setPosition(Vec2(100, 60));
    info_bg->addChild(money_text);

    /*系统商店*/
    auto store_button = Button::create(IMG_ARMS_STORE_LITTLE_BUTTON);
    store_button->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    store_button->setAnchorPoint(Vec2(0.5, 0.5));
    store_button->setPosition(Vec2(200, 60));
    store_button->setTitleText("商城");
    store_button->setTitleFontSize(25);
    store_button->setTitleColor(Color3B::WHITE);
    store_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
        }
    });
    info_bg->addChild(store_button);

    /*资料库按钮*/
    auto know_button = Button::create(IMG_ARMS_STORE_LITTLE_BUTTON);
    know_button->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    know_button->setAnchorPoint(Vec2(0.5, 0.5));
    know_button->setPosition(Vec2(300, 60));
    know_button->setTitleText("资料库");
    know_button->setTitleFontSize(25);
    know_button->setTitleColor(Color3B::WHITE);
    know_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            TransitionScene* transition =
                TransitionFade::create(0.5f, KnowledgeBaseScene::create());
            Director::getInstance()->pushScene(transition);
        }
    });
    info_bg->addChild(know_button);

    /*设置按钮*/
    auto set_button = Button::create(IMG_ARMS_STORE_LITTLE_BUTTON);
    set_button->setTitleFontName("fonts/NotoSansCJKsc-Black.otf");
    set_button->setAnchorPoint(Vec2(0.5, 0.5));
    set_button->setPosition(Vec2(400, 60));
    set_button->setTitleText("设置");
    set_button->setTitleColor(Color3B::WHITE);
    set_button->setTitleFontSize(25);
    set_button->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            AudioController::getInstance()->playClickButtonEffect();
            auto lay = SettingsLayer::create("HomeScene");
            this->addChild(lay, 5);
        }
    });
    info_bg->addChild(set_button);

    /*  init particle touch listener */

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = [this](const std::vector<Touch*>& touches, Event* event) { return; };
    listener->onTouchesEnded = [this](const std::vector<Touch*>& touches, Event* event) {
        auto touch = touches[0];
        auto _emitter = ParticleFlower::createWithTotalParticles(15);
        _emitter->setTexture(
            Director::getInstance()->getTextureCache()->addImage("particle/stars.png"));
        this->addChild(_emitter, 10);
        _emitter->setPosition(touch->getLocation());
        _emitter->setDuration(0.5);
        _emitter->setEmissionRate(30);
        _emitter->setLife(0.4);
        _emitter->setLifeVar(0.1);
        _emitter->setAutoRemoveOnFinish(true);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void
ArmsStoreScene::onEnter()
{
    Scene::onEnter();

    auto location = GameData::getInstance()->getLocationByTag("ArmsStore");

    /*背景音乐*/
    AudioController::getInstance()->playMusic(location.backgroundMusic, true);

    /*钱币*/
    auto money = GameData::getInstance()->getMoneyNum();
    stringstream ss;
    string number;
    ss << money;
    ss >> number;
    money_text->setString(number);

    /*对话图标*/
    double dis = 0.34;
    auto conversations = GameData::getInstance()->getConversationIndicators("ArmsStore");
    for (int i = 0; i < conversations.size(); i++) {
        auto indicatorButton = ConversationIndicatorButton::create(conversations[i]);
        this->addChild(indicatorButton);
        vector_indicator.pushBack(indicatorButton);
        indicatorButton->setPosition(Vec2(_visibleSize.width * dis, _visibleSize.height * 0.13));
        dis += 0.08;
    }
}

void
ArmsStoreScene::onExit()
{
    Scene::onExit();
    for (auto i : vector_indicator) {
        i->removeFromParentAndCleanup(false);
    }
    vector_indicator.clear();
}
