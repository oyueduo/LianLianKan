#ifndef __WELCOME_SCENE__
#define __WELCOME_SCENE__

#include"cocos2d.h"

class Welcome : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual	bool init();
	CREATE_FUNC(Welcome);
	void menuCallback(cocos2d::Ref* sender);
};
#endif