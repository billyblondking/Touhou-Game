#include "GameScene.h"
#include "Enemy.h"
#include "SettingsLayer.h"

#include "resources.h.dir\common.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define PTM_RATIO 1

void GameScene::onEnter()
{
	Scene::onEnter();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameScene/bgm001.mp3", true);//����ѭ��
}

void GameScene::onExit()
{
	Scene::onExit();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	this->initWithPhysics();//��ʼ����������	
	Vect gravity(0, -1000.0f);	//��Ϸ����������
	this->getPhysicsWorld()->setGravity(gravity); //��������
	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //����ģʽ����Χ��
	
	//��ʼ����ͼ����
	initBackGround();

	// ��ʼ����ͼ
	initMap();

	// ��ʼ���������
	initCtrlPanel();

	// ���ؽ�ɫ
	initCharacter();

	// ���������
	initCamera();

	// ���ط�����
	initLauncher();

	// ���ص���
	initEnemy();

	// ���ؼ�����
	initListener();

	// ����֡��ʱ��
	scheduleUpdate();

	return true;
}


void GameScene::initBackGround()
{
	auto backGroundLayer = Layer::create();

	Sprite* bg = Sprite::create("gamescene/gbg.png");
	bg->setAnchorPoint(Point::ZERO);
	bg->setPosition(Point::ZERO);
	bg->setScale(1.5);

	backGroundLayer->addChild(bg);

	this->addChild(backGroundLayer, -100);
}


void GameScene::initMap()
{
	mapLayer = Layer::create();
	_map = TMXTiledMap::create("gamescene/test.tmx");
	_map->setScale(1.0f);
	mapLayer->addChild(_map);
	this->addChild(mapLayer,0);

	//������̬����ǽ  
	createPhysical(1);

}

//������̬���壬���ܲ������ø����С����
bool GameScene::createPhysical(float scale)
{
	// �ҳ��赲�������ڵĲ�
	TMXObjectGroup* group = _map->getObjectGroup("physics");
	auto objects = group->getObjects();
	//Value objectsVal = Value(objects);
	//log("%s", objectsVal.getDescription().c_str());

	for (auto& v : objects)
	{
		auto dict = v.asValueMap();

		if (dict.size() == 0)
			continue;

		// ��ȡ������״����ʼ��  
		float x = dict["x"].asFloat() * scale;
		float y = dict["y"].asFloat() * scale;
		//log("x:%f\ny:%f", x, y);

		//�����polygonPoints
		if (dict.find("points") != dict.end()) {
			auto polygon_points = dict["points"].asValueVector();

			Vec2 points[20];
			int shapeVecAmount = 0;					//ÿ��shape�Ķ������

													// ���뽫���ж�ȡ�Ķ���������Ϊ��תy֮�������ζ����˳���Ѿ������ˣ�����b2PolygonShape��crash  
			int c = polygon_points.size();
			polygon_points.resize(c);
			c--;

			for (auto obj : polygon_points)
			{
				// �������ʼ���ƫ��
				float offx = obj.asValueMap()["x"].asFloat() * scale;
				float offy = obj.asValueMap()["y"].asFloat() * scale;

				points[c] = Vec2((x + offx) / PTM_RATIO, (y - offy) / PTM_RATIO);
				c--;
				shapeVecAmount++;
			}

			PhysicsBody* _pBody = PhysicsBody::createPolygon(points, shapeVecAmount);
			_pBody->getFirstShape()->setDensity(0);
			_pBody->getFirstShape()->setFriction(1.0);
			_pBody->getFirstShape()->setRestitution(0);
			_pBody->setDynamic(false);

			_pBody->setCategoryBitmask(groundCategory);		//������ε����������룬Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setCollisionBitmask(playerCategory | enemyCategory);				//Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setContactTestBitmask(playerCategory);			//Ĭ��ֵΪ0


			auto sprite = Sprite::create();
			sprite->setTag(98);
			sprite->setPhysicsBody(_pBody);
			mapLayer->addChild(sprite);
		}
		else if (dict.find("polylinePoints") != dict.end()) {
			auto polyline_points = dict["polylinePoints"].asValueVector();

			int shapeVecAmount = 0;					//ÿ��shape�Ķ������
			Vec2 points[20];

			int i = 0;
			for (auto obj : polyline_points)
			{
				float offx = obj.asValueMap()["x"].asFloat() * scale;
				float offy = obj.asValueMap()["y"].asFloat() * scale;

				points[i] = Vec2((x + offx) / PTM_RATIO, (y - offy) / PTM_RATIO);
				i++;
				shapeVecAmount++;
			}

			PhysicsBody* _pBody = PhysicsBody::createEdgeChain(points, shapeVecAmount);
			_pBody->getFirstShape()->setDensity(0);
			_pBody->getFirstShape()->setFriction(1.0);
			_pBody->getFirstShape()->setRestitution(0);
			_pBody->setDynamic(false);

			_pBody->setCategoryBitmask(groundCategory);		//�����ߵ����������룬Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setCollisionBitmask(playerCategory | enemyCategory);	//Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setContactTestBitmask(playerCategory | enemyCategory);	//Ĭ��ֵΪ0


			auto sprite = Sprite::create();
			sprite->setPhysicsBody(_pBody);
			sprite->setTag(99);

			mapLayer->addChild(sprite);
		}
		else {
			PhysicsBody* _pBody;

			float width = dict["width"].asFloat() * scale;
			float height = dict["height"].asFloat() * scale;

			_pBody = PhysicsBody::createBox(Size(width, height));
			_pBody->getFirstShape()->setDensity(0);
			_pBody->getFirstShape()->setFriction(1.0);
			_pBody->getFirstShape()->setRestitution(0);
			_pBody->setDynamic(false);

			//_pBody->setTag(100);
			_pBody->setCategoryBitmask(groundCategory);		//�����ε����������룬Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setCollisionBitmask(playerCategory | enemyCategory);				//Ĭ��ֵΪ0xFFFFFFFF
			_pBody->setContactTestBitmask(playerCategory | enemyCategory);			//Ĭ��ֵΪ0

			auto sprite = Sprite::create();
			sprite->setTag(100);
			sprite->setPosition(x + width / 2.0f, y + height / 2.0f);
			sprite->setPhysicsBody(_pBody);
			mapLayer->addChild(sprite);
		}
	}
	return true;
}

bool GameScene::onContactBegin(const PhysicsContact& contact) {

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 99)
		{
			return contact.getContactData()->normal.y > 0;
		}
		else if (nodeB->getTag() == 99)
		{
			return contact.getContactData()->normal.y > 0;
		}
	}
	return true;
}


void GameScene::initCtrlPanel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

	auto controlPanel = Layer::create();
	this->addChild(controlPanel);

	auto set_button = Button::create("CloseNormal.png");
	set_button->setPosition(Vec2(visibleSize.width * 0.080, visibleSize.height * 0.950));
	set_button->setScale(1.5);
	set_button->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto layer = SettingsLayer::create("HomeScene");
			this->addChild(layer, 1000);//������ȼ�
		}
	});	
	controlPanel->addChild(set_button);
	
	//auto jumpBtn = Sprite::create("jumpButton.png");
	//jumpBtn->setScale(0.5);
	////jumpBtn->setPosition(Point(visibleOrigin.x + visibleSize.width - 100, visibleOrigin.y + 50));
	//jumpBtn->setPosition(Point(visibleSize.width/2, visibleOrigin.y/2 + 50));
	//this->addChild(jumpBtn, 1000);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

}

bool GameScene::onTouchBegan(Touch* touch, Event *event) {
	auto location = touch->getLocation();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//Player* p = (Player *)this->getParent()->getChildByName("player");
	//auto body = p->getPhysicsBody();

	auto body = _player->getPhysicsBody();

	if (location.x > visibleSize.width / 2 && location.y < visibleSize.height / 2) {
		//����������
		//_player->unschedule(CC_SCHEDULE_SELECTOR(Player::playerRunRight));
		_player->setScaleX(1);//���﷭ת
		_player->attackDirection = "right";
		_player->schedule(CC_SCHEDULE_SELECTOR(Player::playerRunRight));
	}

	else if (location.x < visibleSize.width / 2 && location.y < visibleSize.height / 2) {
		//����������  
		//_player->unschedule(CC_SCHEDULE_SELECTOR(Player::playerRunLeft));
		_player->setScaleX(-1);//���﷭ת
		_player->attackDirection = "left";
		_player->schedule(CC_SCHEDULE_SELECTOR(Player::playerRunLeft));
	}

	else if (location.y >= visibleSize.height / 2) {
		//��������
		_player->playerJump();
	}
	return true;
}

void GameScene::onTouchEnded(Touch * touch, Event * event)
{
	auto location = touch->getLocation();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	if (location.x > visibleSize.width / 2 && location.y < visibleSize.height / 2) {
		//�������·��������������״̬ 
		_player->unschedule(CC_SCHEDULE_SELECTOR(Player::playerRunRight));
	}
	else if (location.x < visibleSize.width / 2 && location.y < visibleSize.height / 2) {
		//�������·��������������״̬
		_player->unschedule(CC_SCHEDULE_SELECTOR(Player::playerRunLeft));
	}
	else if (location.y >= visibleSize.height / 2) {
		//�벻Ҫ�϶����
	}
	//�Զ����ٻ�û����
}

void GameScene::initCharacter()
{
	TMXObjectGroup* temp = _map->getObjectGroup("player");
	auto ts = temp->getObject("birthPoint");
	//log("%s", Value(ts).getDescription().c_str());

	float a = ts["x"].asFloat();
	float b = ts["y"].asFloat();

	_player = Player::create();
	_player->setTag(101);
	_player->setName("player");
	_player->setPosition(a, b);

	mapLayer->addChild(_player);
}

void GameScene::initCamera()
{
	auto mapSize = _map->getMapSize();
	auto mapTileSize = _map->getTileSize();

	camera = Sprite::create();
	camera->setPosition(_player->getPosition());
	this->addChild(camera);

	auto follow = Follow::create(camera, Rect(0, 0, mapSize.width * mapTileSize.width * 3, mapSize.height * mapTileSize.height - 50));
	mapLayer->runAction(follow);

	// camera->schedule(CC_SCHEDULE_SELECTOR(GameScene::moveCamera));
	//this->scheduleUpdate();
}

//void GameScene::moveCamera(float dt)
//{
//	//	Player* p = (Player *)this->getChildByName("player");
//
//	Vec2 poi = _player->getPosition();
//	//Vec2 poi = p->getPosition();
//	camera->setPosition(poi.x + 100, poi.y + 70);
//}

void GameScene::initLauncher()
{
	auto s = Director::getInstance()->getWinSize();

	//����BatchNode�ڵ㣬������Ⱦ�ӵ�
	bulletBatchNode = SpriteBatchNode::create("gamescene/bullet1.png");
	mapLayer->addChild(bulletBatchNode);

	//ÿ��0.2S����һ�η����ӵ�����  
	//this->schedule(CC_SCHEDULE_SELECTOR(GameScene::ShootBullet), 0.2f);
	//ÿ��0.4S����һ�η����ӵ�����  
	this->schedule(CC_SCHEDULE_SELECTOR(GameScene::ShootBullet), 0.4f);
}


//�û���ķ��������ӵ�������ʼ���ӵ����˶����˶�����¼�
void GameScene::ShootBullet(float dt) {
	Size winSize = Director::getInstance()->getWinSize();
	auto playerPos = _player->getPosition();
	//auto playerPos = _launcher->getPosition();//��Ϊ����������
	//�ӻ����д����ӵ�  
	auto spritebullet = Sprite::createWithTexture(bulletBatchNode->getTexture());
	spritebullet->setTag(102);

	auto spritebullet2 = Sprite::createWithTexture(bulletBatchNode->getTexture());
	spritebullet2->setTag(102);

	//�������õ��ӵ���ӵ�BatchNode�н���������Ⱦ
	bulletBatchNode->addChild(spritebullet);
	bulletBatchNode->addChild(spritebullet2);

	//�������õ��ӵ���Ӹ���
	do {
		auto _body = PhysicsBody::createBox(spritebullet->getContentSize());
		_body->setRotationEnable(false);
		_body->setGravityEnable(false);
		//_body->setTag(102);

		_body->setContactTestBitmask(bulletCategory);
		_body->setCollisionBitmask(enemyCategory);
		_body->setContactTestBitmask(enemyCategory);
		spritebullet->setPhysicsBody(_body);
	} while (0);

	do {
		auto _body = PhysicsBody::createBox(spritebullet->getContentSize());
		_body->setRotationEnable(false);
		_body->setGravityEnable(false);
		//_body->setTag(102);

		_body->setCategoryBitmask(bulletCategory);
		_body->setCollisionBitmask(enemyCategory);
		_body->setContactTestBitmask(enemyCategory);

		spritebullet2->setPhysicsBody(_body);
	} while (0);

	//�������õ��ӵ���ӵ�����  
	vecBullet.pushBack(spritebullet);
	vecBullet.pushBack(spritebullet2);

	Point bulletPos = (Point(playerPos.x, playerPos.y));

	//spritebullet->setScale(1.3f);

	spritebullet->setPosition(bulletPos);
	//spritebullet2->setPosition(bulletPos);
	spritebullet2->setPosition(Point(playerPos.x, playerPos.y - 30));

	float realFlyDuration = 1.0;
	//�ӵ����еľ����ʱ��
	//auto actionMove = MoveBy::create(realFlyDuration, Point(0,winSize.height));
	//auto actionMove2 = MoveBy::create(realFlyDuration, Point(0,-winSize.height));
	auto actionMove = MoveBy::create(realFlyDuration, Point(winSize.width, 0));
	auto fire1 = actionMove;
	auto actionMove2 = MoveBy::create(realFlyDuration, Point(winSize.width, 0));
	auto fire2 = actionMove2;

	if (_player->attackDirection.compare("right"))
	{
		fire1 = actionMove->reverse();
		fire2 = actionMove2->reverse();
	}

	//�ӵ�ִ���궯������к����ص��������Ƴ��ӵ�����  
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(GameScene::removeBullet, this));

	//�ӵ���ʼ�ܶ�
	Sequence* sequence = Sequence::create(fire1, actionDone, NULL);
	Sequence* sequence2 = Sequence::create(fire2, actionDone, NULL);

	spritebullet->runAction(sequence);
	spritebullet2->runAction(sequence2);
}

//�Ƴ��ӵ������ӵ����������Ƴ���ͬʱҲ��SpriteBatchNode���Ƴ�
void GameScene::removeBullet(Node* pNode) {
	if (NULL == pNode) {
		return;
	}
	Sprite* bullet = (Sprite*)pNode;
	this->bulletBatchNode->removeChild(bullet, true);
	vecBullet.eraseObject(bullet);
}

void GameScene::initEnemy()
{
	TMXObjectGroup* group = _map->getObjectGroup("enemy");
	auto objects = group->getObjects();
	//Value objectsVal = Value(objects);
	//log("%s", objectsVal.getDescription().c_str());

	for (auto v : objects)
	{
		auto dict = v.asValueMap();
		if (dict.size() == 0)
			continue;

		float x = dict["x"].asFloat();
		float y = dict["y"].asFloat();

		Enemy* _enemy = Enemy::create();
		_enemy->setPosition(x, y);
		mapLayer->addChild(_enemy);
		_enemy->schedule(CC_SCHEDULE_SELECTOR(Enemy::AI));
	}
}

//��ʼ�����������ֶ�ָ�����ȼ�
void GameScene::initListener()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);

	//dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	dispatcher->addEventListenerWithFixedPriority(listener, 20);

	auto e = Enemy::create();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Enemy::onContactBegin, e);
	contactListener->onContactSeparate = CC_CALLBACK_1(Enemy::onContactSeparate, e);
	//dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	dispatcher->addEventListenerWithFixedPriority(contactListener, 10);

}


void GameScene::update(float dt)
{
	//Player* p = (Player *)this->getChildByName("player");
	//Vec2 poi = p->getPosition();
	Vec2 poi = _player->getPosition();
	camera->setPosition(poi.x + 100, poi.y + 70);//�ƶ������

	//�򵥵�������Ч
	//ps->setPosition(poi);
}