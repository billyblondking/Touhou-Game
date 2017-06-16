#include "Player.h"
#include "GameScene.h"

bool Player::init()
{
	if (!Node::init())
		return false;

	//��������ɫ������֡����,ע��˴�Ҫ�����л��������Բ�����AnimationCache
	p1Animation = Animation::create();
	for (int i = 0; i <= 7; i++)
		p1Animation->addSpriteFrameWithFile("gamescene/walkFront00" + std::to_string(i) + ".png");
	p1Animation->setDelayPerUnit(0.15f);

	jumpAnimation = Animation::create();
	for (int i = 0; i <= 9; i++)
		jumpAnimation->addSpriteFrameWithFile("gamescene/jumpFront00" + std::to_string(i) + ".png");
	jumpAnimation->setDelayPerUnit(0.1f);

	//Animation *p2Animation = Animation::create();
	//for (int i = 1; i <= 12; i++)
	//	p2Animation->addSpriteFrameWithFile("");
	//p2Animation->setDelayPerUnit(0.1f);

	std::string playerTextureName = "gamescene/walkFront001.png";

	playerSprite = Sprite::create(playerTextureName); //�˴������ʼ��һ�Ž�ɫ������������޷��л�����	
	this->addChild(playerSprite);

	Animation *playerAnimation;
	playerAnimation = p1Animation;

	playerAnim = Animate::create(playerAnimation);

	playerAnim->setTag(1);

	AnimationCache::getInstance()->addAnimation(jumpAnimation, "jumpAnimation");
	AnimationCache::getInstance()->addAnimation(p1Animation, "p1Animation");

	playerSprite->runAction(RepeatForever::create(playerAnim));//��ʼʱ�̽�ɫ�ڱ���
	//playerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("p1Animation"))));//��ʼʱ�̽�ɫ�ڱ���

	auto bo = PhysicsBody::createBox(Size(50, 75));
	bo->setDynamic(true);
	bo->setMass(1);
	bo->setGravityEnable(true);
	bo->setRotationEnable(false);
	bo->getFirstShape()->setDensity(0);
	bo->getFirstShape()->setFriction(0.2);
	bo->getFirstShape()->setRestitution(0);

	//bo->setTag(101)
	bo->setCategoryBitmask(playerCategory);
	bo->setCollisionBitmask(groundCategory | enemyCategory);
	bo->setContactTestBitmask(groundCategory | enemyCategory);

	this->setPhysicsBody(bo); //�滻������

	//this->setScale(0.8);//���ô�С�Ŀ̶�

	return true;
}

void Player::playerRunRight(float dt)
{
	auto body = this->getPhysicsBody();
	auto velocity = body->getVelocity();

	Vec2 impluse = Vec2(0, 0);
	//Vec2 impluse = Vec2(20.0f, 0.0f);
	//body->applyForce(Vec2(100.0f,0.0f));

	if (velocity.x < MAX_SPEED) {
		impluse.x = std::min(MAX_SPEED / ACCELERATE_TIME * dt, MAX_SPEED - velocity.x);
	}
	body->applyImpulse(impluse);
}

void Player::playerRunLeft(float dt)
{
	auto body = this->getPhysicsBody();
	auto velocity = body->getVelocity();

	Vec2 impluse = Vec2(0, 0);
	//Vec2 impluse = Vec2(-20.0f, 0.0f);
	//body->applyForce(Vec2(-100.0f, 0.0f));

	if (velocity.x > -MAX_SPEED) {
		impluse.x = -std::min(MAX_SPEED / ACCELERATE_TIME * dt, MAX_SPEED + velocity.x);
	}
	body->applyImpulse(impluse);
}

void Player::playerJump()
{
	auto body = this->getPhysicsBody();
	auto curVelocity = body->getVelocity();
	body->setVelocity(Vec2(curVelocity.x, 0));//�ٴ���Ծʱ������Y���ٶ�Ϊ0

	Vec2 impluse = Vec2(0.0f, 500.0f);
	body->applyImpulse(impluse);
	//body->setVelocity(Vec2(0, 700.0f));
	
	auto playerAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("jumpAnimation"));
	playerSprite->stopAllActions();
//	playerSprite->stopActionByTag(1);//��ֹͣ����
	

	auto actionDone = CallFuncN::create(CC_CALLBACK_1(Player::resetAction, this));
	auto sequence = Sequence::create(Repeat::create(playerAnimate, 1) ,actionDone, NULL);
	playerSprite->runAction(sequence);
}

void Player::resetAction(Node* pNode) 
{
	playerSprite->stopAllActions(); 
	//playerSprite->stopActionByTag(1);

	playerSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("p1Animation"))));

}