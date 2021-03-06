﻿#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#ifndef ENEMY_UDONGE_H
#define ENEMY_UDONGE_H

#include "GameplayScene/Enemy/Enemy.h"

class Udonge : public Enemy
{
public:
    virtual bool init(const std::string& tag);

public:
    void horizontallyAccelerate(float dt);
    void jump();
    void dash();
    void decreaseHp(int damage);
    void autoChangeDirection(float dt) override;

private:
    float timeAccumulation = 0;

    class StandAndChooseAction : public State<Enemy>
    {
    public:
        static StandAndChooseAction* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Walk : public State<Enemy>
    {
    public:
        static Walk* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Jump : public State<Enemy>
    {
    public:
        static Jump* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Fall : public State<Enemy>
    {
    public:
        static Fall* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Dash : public State<Enemy>
    {
    public:
        static Dash* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Shoot : public State<Enemy>
    {
    public:
        static Shoot* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class ShootFromAir : public State<Enemy>
    {
    public:
        static ShootFromAir* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class UseSpellCard : public State<Enemy>
    {
    public:
        static UseSpellCard* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Hit : public State<Enemy>
    {
    public:
        static Hit* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };

    class Knockdown : public State<Enemy>
    {
    public:
        static Knockdown* getInstance();
        void Enter(Enemy*);
        void Exit(Enemy*);
        void defaultChangeState(Enemy*);
    };
};

#endif
