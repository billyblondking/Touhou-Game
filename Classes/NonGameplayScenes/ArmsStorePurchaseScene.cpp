#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include "ArmsStorePurchaseScene.h"
#include "NonGameplayScenesCache.h"
#include "PlaceHolder.h"
#include "ui/CocosGUI.h"
#include <string>

// ��̬���ݳ�Ա�������ඨ�� *��* ���г�ʼ��
// Ϊ��֤����ʱ��̬���ݳ�Ա���ֻ������һ��Ŀ���ļ���
// �������Ҳ����д�� .h �ļ��У����ڶ�Ӧ�� .cpp �ļ��Ŀ�ͷ�����ѡ��
const std::string ArmsStorePurchaseScene::TAG{ "ArmsStoreScene" };

ArmsStorePurchaseScene::ArmsStorePurchaseScene()
{
    _visibleSize = _director->getVisibleSize();
}

bool
ArmsStorePurchaseScene::init()
{
    /*  1. super init first */

    if (!Scene::init()) {

        return false;
    }

/*  2. debug info */

#ifndef NDEBUG
    auto sceneTag = Label::createWithTTF("ArmsStorePurchaseScene", "fonts/arial.ttf", 16);
    sceneTag->setAnchorPoint(Vec2(0, 1));
    sceneTag->setPosition(Vec2(0, _visibleSize.height));
    sceneTag->setColor(Color3B::WHITE);
    this->addChild(sceneTag);
#endif

    ///*  3. PlaceHolder */

    // auto p = PlaceHolder::createCircle(100, "ArmsStorePurchaseScene");
    // p->setPosition(_visibleSize / 2);
    // this->addChild(p);

    /*  4. schedule */

    this->scheduleUpdate();

    return true;

    /*background*/
    auto bg = Sprite::create("InventoryScene/bg.png");
    bg->setContentSize(Size(_visibleSize.width, _visibleSize.height));
    bg->setPosition(Vec2(_visibleSize.width * 0.5, _visibleSize.height * 0.5));
    addChild(bg, 0, 1);
    /*background2*/
    auto bg_2 =
        PlaceHolder::createRect(Size(_visibleSize.width * 0.859, _visibleSize.height * 0.78), "",
                                16, Color3B(91, 155, 213));
    bg_2->setPosition(_visibleSize / 2);
    addChild(bg_2, 0, 2);
    /*background3*/
    auto bg_3 = PlaceHolder::createRect(
        Size(_visibleSize.width * 0.595, _visibleSize.height * 0.707), "", 16, Color3B::WHITE);
    bg_3->setAnchorPoint(Vec2(0, 0));
    bg_3->setPosition(Vec2(_visibleSize.width * 0.29, _visibleSize.height * 0.141));
    addChild(bg_3, 0, 3);

    /*backButton*/
    auto backButton = Button::create("InventoryScene/p1.png", "", "");
    backButton->setPosition(Vec2(_visibleSize.width * 0.2, _visibleSize.height * 0.2));
    backButton->setTitleText("����");
    backButton->setTitleColor(Color3B::WHITE);
    backButton->setContentSize(Size(_visibleSize.width * 0.15, _visibleSize.height * 0.15));
    backButton->addTouchEventListener(
        [](Ref* pSender, Widget::TouchEventType type) { Director::getInstance()->popScene(); });
    addChild(backButton, 0, 4);

    /*normalProps*/
    auto normalProps = Button::create("InventoryScene/p2.png", "", "");
    normalProps->setPosition(Vec2(_visibleSize.width * 0.2, _visibleSize.height * 0.45));
    normalProps->setTitleText("��ͨ����");
    normalProps->setTitleFontSize(15);
    normalProps->setTitleColor(Color3B(91, 155, 213));
    normalProps->setContentSize(Size(_visibleSize.width * 0.2, _visibleSize.height * 0.25));
    addChild(normalProps, 0, 5);
    normalProps->setTouchEnabled(true);

    /*strongProps*/
    auto strongProps = Button::create("InventoryScene/p2.png", "", "");
    strongProps->setPosition(Vec2(_visibleSize.width * 0.2, _visibleSize.height * 0.6));
    strongProps->setTitleText("ǿ������");
    strongProps->setTitleFontSize(15);
    strongProps->setTitleColor(Color3B(91, 155, 213));
    strongProps->setContentSize(Size(_visibleSize.width * 0.2, _visibleSize.height * 0.2));
    addChild(strongProps, 0, 6);
    strongProps->setTouchEnabled(true);

    /*specialProps*/
    auto specialProps = Button::create("InventoryScene/p2.png", "", "");
    specialProps->setPosition(Vec2(_visibleSize.width * 0.2, _visibleSize.height * 0.75));
    specialProps->setTitleText("�������");
    specialProps->setTitleFontSize(15);
    specialProps->setTitleColor(Color3B(91, 155, 213));
    specialProps->setContentSize(Size(_visibleSize.width * 0.2, _visibleSize.height * 0.2));
    addChild(specialProps, 0, 7);
    specialProps->setTouchEnabled(true);
    return true;
}

void
ArmsStorePurchaseScene::update(float dt)
{
}
