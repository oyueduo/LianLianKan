#ifndef __HelloWorld_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include"iostream"
using namespace std;
#include "cocos2d.h"
#include "Cube.h"

#define MAP_SIZE_WIDTH (8)
#define MAP_SIZE_HEIGHT (8)
#define TYPE_NUM (22)
#define CUBE_HEIGHT (44)
#define CUBE_WIDTH (41)
#define CUBE_SCALE (1.1)
#define CUBE_TOUCH_SCALE (1.3)
#define LIGHTNINGLASTTIME (0.2)
enum CubeType
{
	blank=0,
	fruit,
	star,
	border,
	lightning
};

enum LightningType
{
	horizon=1,
	vertical,
	cross
};

enum Direction
{
	left,
	right,
	up,
	down
};

class NewGame : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	// implement the "static create()" method manually
    CREATE_FUNC(NewGame);

	void initMap();
	void initTypeArray();
	
	bool canDelete(Cube* cube1, Cube* cube2);
	bool canDelete(int x1,int y1,int x2,int y2);
	bool isConnectedWithOneLine(int x1, int y1, int x2, int y2);
	bool isConnectedWithTwoLines(int x1, int y1, int x2, int y2);
	bool isConnectedWithThreeLines(int x1, int y1, int x2, int y2);
	bool isPosBlank(cocos2d::Vec2 pos);

	bool noneIsConnect();
    //重新排列已有的方块
	void resetCube();
	bool isWin();
	void win();
	void createLightningByPos(int x1, int y1,LightningType type,Direction dir);
	void addLightning(int x1,int y1,int x2,int y2);
	Cube* getCubeByPos(cocos2d::Vec2 pos);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *unusedEvent);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *unusedEvent);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event *unusedEvent);

	void removeLightning(float dt);
	void menuCallback(cocos2d::Ref* sender);//restart 的回调函数

private:
	vector<cocos2d::Vector<Cube*> > atlas; 
	int typeArray[MAP_SIZE_HEIGHT+2][MAP_SIZE_WIDTH+2]; //地图元素编号，代表相应图案,0代表空白
	//int cubeTypeArray[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH];
	cocos2d::Size visibleSize;
	Cube* currentCube;//选中的cube
	Cube* currentFingerCube; //当前手指下的cube
	bool canDeleteWithThreeLines;
	vector<int> resetVector;

};

#endif // __HELLOWORLD_SCENE_H__
