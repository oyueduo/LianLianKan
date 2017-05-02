#ifndef __MY_UTILITY_H__
#define __MY_UTILITY_H__

#include"cocos2d.h"



std::pair<int,int> changePosToCoord(cocos2d::Vec2 pos);

cocos2d::Vec2 changeCoordToPos(int x, int y);
#endif
