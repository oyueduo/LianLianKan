#include "MyUtility.h"
#include "HelloWorldScene.h"
USING_NS_CC;

pair<int,int> changePosToCoord(cocos2d::Vec2 pos)
{
	pair<int, int> coord(-1, -1);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	pos -= Vec2(visibleSize.width / 2 - CUBE_WIDTH*CUBE_SCALE*(MAP_SIZE_WIDTH / 2+1),
		visibleSize.height / 2 - CUBE_HEIGHT*CUBE_SCALE*(MAP_SIZE_HEIGHT / 2+1));
	log("pos.%f.%f", pos.x, pos.y);
	if (pos.x<0 || pos.x>CUBE_WIDTH*CUBE_SCALE*(MAP_SIZE_WIDTH+2) || pos.y<0 || pos.y> CUBE_HEIGHT*CUBE_SCALE*(MAP_SIZE_HEIGHT+2))
		return coord;
	coord.first = (int)(pos.x / CUBE_WIDTH/CUBE_SCALE);
	coord.second =(int) (pos.y / CUBE_HEIGHT/CUBE_SCALE);
	return coord;
}
cocos2d::Vec2 changeCoordToPos(int x, int y)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin(visibleSize.width / 2 - CUBE_WIDTH*CUBE_SCALE*(MAP_SIZE_WIDTH / 2 + 1)+ CUBE_WIDTH*CUBE_SCALE/2,
		visibleSize.height / 2 - CUBE_HEIGHT*CUBE_SCALE*(MAP_SIZE_HEIGHT / 2 + 1)+ CUBE_HEIGHT*CUBE_SCALE/2);
	origin.x += CUBE_WIDTH*CUBE_SCALE*x;
	origin.y += CUBE_HEIGHT*CUBE_SCALE*y;
	return origin;
}


