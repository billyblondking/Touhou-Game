﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include "GameplayScene/Enemy/Opossum.h"
#include "GameData/EnemyData.h"
#include "GameData/GameData.h"

#define CREATE_AND_ADD_ANIMATION_CACHE(animation, enemy, frames, delayPerUnit, key)                \
    animation = Animation::create();                                                               \
    for (auto v : enemy.frames) {                                                                  \
        animation->addSpriteFrameWithFile(v);                                                      \
    }                                                                                              \
    animation->setDelayPerUnit(enemy.delayPerUnit);                                                \
    AnimationCache::getInstance()->addAnimation(animation, key);

bool
Opossum::init(std::string tag)
{
    if (!Node::init())
        return false;

    this->enemyTag = tag;
    this->setTag(enemyCategoryTag);
    this->setName(tag);

    EnemyData _enemyData = GameData::getInstance()->getEnemyByTag(tag);
    enemySprite = Sprite::create(_enemyData.defaultTexture);
    this->addChild(enemySprite);

    //设置属性值
    this->hp = _enemyData.healthPoint;

    //设置刚体
    body = PhysicsBody::createBox(
        enemySprite->getContentSize()); //这里要用包围盒，如果用圆形的话会导致滚动
    body->setDynamic(true);
    body->setMass(1);
    body->setGravityEnable(true);
    body->setRotationEnable(false);
    body->getFirstShape()->setDensity(0);
    body->getFirstShape()->setFriction(0.2);
    body->getFirstShape()->setRestitution(0); //设置刚体回弹力
    body->getFirstShape()->setCategoryBitmask(enemyCategory);
    body->getFirstShape()->setCollisionBitmask(groundCategory | bulletCategory | elevatorCategory);
    body->getFirstShape()->setContactTestBitmask(groundCategory | bulletCategory |
                                                 elevatorCategory | playerCategory);
    this->setPhysicsBody(body);

    //索敌检测框
    auto rect = PhysicsShapeBox::create(Size(150, 200));
    rect->setTag(lockCategoryTag);
    rect->setCategoryBitmask(lockCategory);
    rect->setCollisionBitmask(0);
    rect->setContactTestBitmask(playerCategory);
    body->addShape(rect);

    //设置速度上限
    body->setVelocityLimit(500);

    //设置动画
    CREATE_AND_ADD_ANIMATION_CACHE(runAnimation, _enemyData, runFrame, runFrameDelay,
                                   "OpossumRunAnimation");
    runAnimation->retain();

    //行为模式状态机
    modeStateMachine = new StateMachine<Enemy>(this);
    modeStateMachine->changeState(OpossumPatrolState::getInstance());
    //动画状态机
    animateStateMachine = new StateMachine<Enemy>(this);
    animateStateMachine->changeState(Opossum::RunAnimation::getInstance());

    return true;
}

void
Opossum::run(float dt)
{
    timeAccumulation += dt;
    if (timeAccumulation >= 0.1) {
        if (this->enemyDirection == Direction::LEFT) {
            this->getPhysicsBody()->applyImpulse(Vec2(-30.0f, 0.0f));
        } else {
            this->getPhysicsBody()->applyImpulse(Vec2(30.0f, 0.0f));
        }
        timeAccumulation = 0;
    }
}

void
Opossum::decreaseHp(int damage)
{
    this->hp = this->hp - damage;
    if (this->hp < 0) {
        this->removeFromParentAndCleanup(true);
    }
}

void
Opossum::alertMode(float dt)
{
    this->schedule(CC_SCHEDULE_SELECTOR(Opossum::run));
}

void
Opossum::patrolMode(float dt)
{
    int weight = random(0, 100);
    if (weight >= 60) {
        if (this->isScheduled(CC_SCHEDULE_SELECTOR(Opossum::run))) {
            this->unschedule(CC_SCHEDULE_SELECTOR(Opossum::run));
        }
    } else {
        if (weight >= 30) {
            enemySprite->setScaleX(-1);
            this->enemyDirection = Direction::RIGHT;
        } else {
            enemySprite->setScaleX(1);
            this->enemyDirection = Direction::LEFT;
        }
        if (!(this->isScheduled(CC_SCHEDULE_SELECTOR(Opossum::run)))) {
            this->schedule(CC_SCHEDULE_SELECTOR(Opossum::run));
        }
    }
}

void
Opossum::autoChangeDirection(float dt)
{
    Point enemyPos = this->getPosition();
    Vec2 playerPos = (*curTarget)->getPosition();
    if (enemyPos.x > playerPos.x) {
        this->enemyDirection = Direction::LEFT;
        enemySprite->setScaleX(1);
    } else {
        this->enemyDirection = Direction::RIGHT;
        enemySprite->setScaleX(-1);
    }
}

OpossumAlertState*
OpossumAlertState::getInstance()
{
    static OpossumAlertState instance;
    return &instance;
}

void
OpossumAlertState::Enter(Enemy* entity)
{
    entity->schedule(CC_SCHEDULE_SELECTOR(Opossum::alertMode), 0.50);
    entity->schedule(CC_SCHEDULE_SELECTOR(Opossum::autoChangeDirection), 0.50);
}

void
OpossumAlertState::Exit(Enemy* entity)
{
    entity->unschedule(CC_SCHEDULE_SELECTOR(Opossum::alertMode));
    entity->unschedule(CC_SCHEDULE_SELECTOR(Opossum::autoChangeDirection));
}

void
OpossumAlertState::changeToState(Enemy* entity)
{
}

OpossumPatrolState*
OpossumPatrolState::getInstance()
{
    static OpossumPatrolState instance;
    return &instance;
}

void
OpossumPatrolState::Enter(Enemy* entity)
{

    entity->schedule(CC_SCHEDULE_SELECTOR(Opossum::patrolMode), 1.0);
}

void
OpossumPatrolState::Exit(Enemy* entity)
{
    entity->unschedule(CC_SCHEDULE_SELECTOR(Opossum::patrolMode));
}

void
OpossumPatrolState::changeToState(Enemy* entity)
{
    entity->modeStateMachine->changeState(OpossumAlertState::getInstance());
}

Opossum::RunAnimation*
Opossum::RunAnimation::getInstance()
{
    static Opossum::RunAnimation instance;
    return &instance;
}

void
Opossum::RunAnimation::Enter(Enemy* enemy)
{
    auto opossum = (Opossum*)enemy;
    opossum->currentAnimateAction = RepeatForever::create(Animate::create(opossum->runAnimation));
    opossum->enemySprite->runAction(opossum->currentAnimateAction);
}

void
Opossum::RunAnimation::Exit(Enemy* enemy)
{
    auto opossum = (Opossum*)enemy;
    opossum->enemySprite->stopAction(opossum->currentAnimateAction);
    opossum->enemySprite->unschedule("RunAnimationUpdate");
}

void
Opossum::RunAnimation::changeToState(Enemy* enemy)
{
}