#pragma once
#include "cocos2d.h"
#include "Bullet.h"
USING_NS_CC;

class EmitterStyle :public Node
{
public :
	bool init();
	//virtual void createBullet();
	//virtual void shootBullet(float dt);
	void removeBullet(Node* pNode);
	void removeAllObject();
	
protected:
	Vector<Sprite*> bullets;  
	Node* character;   
};

#define APP_CREATE_EMITTER(_TYPE_)\
static _TYPE_* create(Node* character)\
{\
 _TYPE_ *pRet=new _TYPE_(character);\
if(pRet&&pRet->init()){\
pRet->autorelease();\
return pRet;\
}\
else{\
delete pRet;\
pRet = NULL;\
return NULL;\
}\
}