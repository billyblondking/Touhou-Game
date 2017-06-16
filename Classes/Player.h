#pragma once

#include "cocos2d.h"
#include "resources.h.dir\common.h"

#define MAX_SPEED 400.0
#define ACCELERATE_TIME 1.0

using namespace cocos2d;

class Player:public Node
{
public:
	virtual bool init() override;
	CREATE_FUNC(Player);

public:
		
	void playerRunLeft(float dt);
	void playerRunRight(float dt);
	void playerJump();

	void resetAction(Node * pNode);

	String attackDirection = "right";

private:
	Sprite *playerSprite; //��ɫ����
	Animate *playerAnim;
	Texture2D *jumpTexture;


	Animation* p1Animation;
	Animation* jumpAnimation;

	//int hp = 100;

	
};
