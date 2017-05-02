#include "Cube.h"

USING_NS_CC;

Cube * Cube::createCube(int t)
{
	auto sprite = new Cube(t);
	if (sprite && sprite->init() && t<23 && t>0 )
	{
		sprite->autorelease();
		sprite->initWithSpriteFrameName(StringUtils::format("fruit_%02d.jpg",t).c_str());
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Cube::init()
{
	if (!Sprite::init())
		return false;
	return true;
}

