#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

bool Enemy::init()
{
	if (!Node::init())
		return false;

	Animation *enemyAnimation = Animation::create();
	for (int i = 1; i <= 6; i++)
		enemyAnimation->addSpriteFrameWithFile("gamescene/opossum-" + std::to_string(i) + ".png");

	enemyAnimation->setDelayPerUnit(0.15f);

	enemySprite = Sprite::create("gamescene/opossum-1.png"); //�˴������ʼ��һ�Ž�ɫ������������޷��л�����

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

bool Enemy::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 102)
		{
			ParticleSystem* ps = ParticleExplosion::createWithTotalParticles(5);
			ps->setTexture(Director::getInstance()->getTextureCache()->addImage("gamescene/smallOrb000.png"));
			ps->setPosition(nodeA->getPosition());

			nodeB->getParent()->addChild(ps, 10);//�ӵ��˻�ȡ�������޷���ͨ��BatchNode�����ľ����ȡparent

			nodeA->removeFromParentAndCleanup(true);//�Ƴ��ӵ�

			auto enemy = (Enemy*)nodeB;
			enemy->hp = enemy->hp - 5;
			if (enemy->hp < 0)
			{
				nodeB->removeFromParentAndCleanup(true);
			}
		}
		else if (nodeB->getTag() == 102)
		{

			ParticleSystem* ps = ParticleExplosion::createWithTotalParticles(5);
			ps->setTexture(Director::getInstance()->getTextureCache()->addImage("gamescene/smallOrb000.png"));
			ps->setPosition(nodeB->getPosition());

			nodeA->getParent()->addChild(ps, 10);

			nodeB->removeFromParentAndCleanup(true);//�Ƴ��ӵ�

			auto enemy = (Enemy*)nodeA;
			enemy->hp = enemy->hp - 5;
			if (enemy->hp < 0)
			{
				nodeA->removeFromParentAndCleanup(true);
			}
		}
		else if (nodeA->getTag() == 100)
		{
			auto enemy = (Enemy*)nodeB;
			enemy->hp = enemy->hp - 5;
			enemy->_canJump = true;
		}
		else if (nodeB->getTag() == 100)
		{
			auto enemy = (Enemy*)nodeA;
			enemy->hp = enemy->hp - 5;
			enemy->_canJump = true;
		}

	}
	return true;
}

bool Enemy::onContactSeparate(const PhysicsContact & contact)
{
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
		this->getPhysicsBody()->applyImpulse(Vec2(-10.0f, 0.0f));
		//this->getPhysicsBody()->setVelocity(Vec2(-200.f, 0.0f));
	}
	else {
		this->getPhysicsBody()->applyImpulse(Vec2(10.0f, 0.0f));
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
