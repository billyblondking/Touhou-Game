#include "Enemy.h"
#include "Player.h"
#include "GameplayScene.h"

bool Enemy::init()
{
	if (!Node::init())
		return false;

	Animation *enemyAnimation = Animation::create();
	for (int i = 1; i <= 6; i++)
		enemyAnimation->addSpriteFrameWithFile("gameplayscene/opossum-" + std::to_string(i) + ".png");

	enemyAnimation->setDelayPerUnit(0.15f);

	enemySprite = Sprite::create("gameplayscene/opossum-1.png"); //�˴������ʼ��һ�Ž�ɫ������������޷��л�����

	enemyAnim = Animate::create(enemyAnimation);
	enemySprite->runAction(RepeatForever::create(enemyAnim)); //��ʼʱ���ڱ���
	this->addChild(enemySprite);

	auto enemyBody = PhysicsBody::createBox(enemySprite->getContentSize()); //����Ҫ�ð�Χ�У������Բ�εĻ��ᵼ�¹���	
	enemyBody->setDynamic(true);
	enemyBody->setMass(1);
	enemyBody->setGravityEnable(true);
	enemyBody->setRotationEnable(false);

	enemyBody->getFirstShape()->setDensity(0);
	enemyBody->getFirstShape()->setFriction(0.2);
	enemyBody->getFirstShape()->setRestitution(0);//���ø���ص���

	enemyBody->setCategoryBitmask(enemyCategory);
	enemyBody->setCollisionBitmask(groundCategory | bulletCategory | playerCategory);
	enemyBody->setContactTestBitmask(groundCategory | bulletCategory);

	this->setPhysicsBody(enemyBody);
	//enemySprite->setPhysicsBody(enemyBody);//����ʾ����Ӧ�ø�ʵ���������ø��壬������һ��������ʾͼƬ�ľ���

	this->setTag(103);

	return true;
}


void Enemy::run()
{
	//������ƶ�
	Player* p = (Player *)this->getParent()->getChildByName("player");
	Vec2 poi = p->getPosition();

	Point enemyPos = this->getPosition();

	if (enemyPos.x - poi.x > 500) {
		return;
	}
	if (enemyPos.x - poi.x > 0)
	{
		this->getPhysicsBody()->applyImpulse(Vec2(-3.0f, 0.0f));
		//this->getPhysicsBody()->setVelocity(Vec2(-200.f, 0.0f));
	}
	else {
		this->getPhysicsBody()->applyImpulse(Vec2(3.0f, 0.0f));
		//this->getPhysicsBody()->setVelocity(Vec2(200.f, 0.0f));
	}
}

void Enemy::jump()
{
	auto body = this->getPhysicsBody();
	//auto curVelocity = body->getVelocity();
	//body->setVelocity(Vec2(curVelocity.x, 0));//�ٴ���Ծʱ������Y���ٶ�Ϊ0
	Vec2 impluse = Vec2(0.0f, 400.0f);
	body->applyImpulse(impluse);
}

void Enemy::AI(float dt)
{
	int weight = random(0, 1000);//Ȩ��
	if (weight >= 20)
	{
		run();
	}
	else if (weight < 20)
	{
		if (_canJump == false) return;	//�������ڿ��е�ʱ�򲻿�������Ծ
		if (_canJump)
		{
			jump();
			this->_canJump = false;
		}
	}
}
