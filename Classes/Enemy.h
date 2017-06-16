#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Enemy :public Node
{
public:	
	CREATE_FUNC(Enemy);

	virtual bool init() override;
	
	bool onContactBegin(PhysicsContact& contact);
	bool onContactSeparate(const PhysicsContact& contact);

public:
	void run(); //�����ƶ�
	void jump(); //������Ծ

	void AI(float dt);
		
private:
	Sprite *enemySprite; //���˾���

	Animate *enemyAnim;
	Texture2D *jumpTexture;

	int hp = 200;

	//bool _isAir = true;
	//bool _isLand = false;
	bool _canJump = false;

};


#endif