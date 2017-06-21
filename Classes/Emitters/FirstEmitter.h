#include "EmitterStyle.h"

/*ɢ���ͷ�����*/

class FirstEmitter :public EmitterStyle {
public:
	FirstEmitter(Node* character);
	APP_CREATE_EMITTER(FirstEmitter);
	virtual void createBullet(float dt);
	virtual void shootBullet1(float dt);
	virtual void shootBullet2(float dt);
	virtual void shootBullet3(float dt);

private:
	int number;        //����
	int count;         
	int mode;
	float startAngle;  //��ʼ�Ƕ�
	float endAngle;    //��ֹ�Ƕ�
	float varAngle;    //�仯�Ƕ�
};
