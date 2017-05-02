#ifndef __CUBE_H__
#define __CUBE_H__

#include "cocos2d.h"

class Cube :public cocos2d::Sprite
{
public:
	Cube(int t) { cubeType = t; }

	static Cube* createCube(int t);
	virtual bool init();

	void setType(int t)
	{
		cubeType = t;
		this->setTexture(cocos2d::__String::createWithFormat("fruit_%02d.png",t)->getCString());
	}

	int getType()
	{
		return cubeType;
	}
private:
	int cubeType;
};

#endif