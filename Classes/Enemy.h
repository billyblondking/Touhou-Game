#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Enemy :public Node
{
public:	
	CREATE_FUNC(Enemy);

	virtual bool init() override;
	
public:
	void run(); //�����ƶ�
	void jump(); //������Ծ

	void AI(float dt);
	
	int hp = 200;
	bool _canJump = false;

private:
	Sprite *enemySprite; //���˾���

	Animate *enemyAnim;
	Texture2D *jumpTexture;

	//bool _isAir = true;
	//bool _isLand = false;

};


#endif