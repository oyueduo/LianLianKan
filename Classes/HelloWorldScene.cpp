#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MyUtility.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* NewGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = NewGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool NewGame::init()
{
	if (!Layer::init())
		return false;
	 visibleSize = Director::getInstance()->getVisibleSize();


	auto background = Sprite::create("bg.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);

	initTypeArray();
	initMap();

	currentCube = nullptr;
	currentFingerCube = nullptr;
	canDeleteWithThreeLines = false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(NewGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(NewGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(NewGame::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
void NewGame::initMap() {
	log("initMap");
	for (int i = 0; i < MAP_SIZE_HEIGHT; i++)
		{
		atlas.push_back(Vector<Cube*>(MAP_SIZE_WIDTH));
		}
	for (int i = 0; i < MAP_SIZE_HEIGHT; i++)
		for (int j = 0; j < MAP_SIZE_WIDTH; j++)
		{
			auto sprite = Cube::createCube(typeArray[i+1][j+1]);
			atlas.at(i).pushBack(sprite);
			sprite->setPosition(Vec2(visibleSize.width/2- CUBE_WIDTH*CUBE_SCALE*MAP_SIZE_WIDTH/2+CUBE_WIDTH*CUBE_SCALE/2 +CUBE_WIDTH*CUBE_SCALE*j,
				visibleSize.height / 2 - CUBE_HEIGHT*CUBE_SCALE*MAP_SIZE_HEIGHT / 2 + CUBE_HEIGHT*CUBE_SCALE / 2 +CUBE_HEIGHT*CUBE_SCALE*i));
			sprite->setScale((float)CUBE_SCALE);
			this->addChild(sprite,1);
		}
	
}

void NewGame::initTypeArray()
{
	log("initTypeArray");
	srand((unsigned int)time(NULL));
	for (int i = 1; i <= MAP_SIZE_HEIGHT; ++i)
		for (int j = 1; j+1 <= MAP_SIZE_WIDTH; j+=2)
		{
			typeArray[i][j] = rand() % TYPE_NUM+1;
			typeArray[i][j + 1] = typeArray[i][j];
	  }
	
	//打乱
	for (int i = 1; i <= MAP_SIZE_HEIGHT; ++i)
		for (int j = 1; j <= MAP_SIZE_WIDTH; ++j)
		{
			int temp = typeArray[i][j];
			int x = rand() % MAP_SIZE_HEIGHT+1;
			int y = rand() % MAP_SIZE_WIDTH +1;
			typeArray[i][j] = typeArray[x][y];
			typeArray[x][y] = temp;
		}
	for (int i = 0; i <= MAP_SIZE_HEIGHT + 1; ++i)

	{
		int j = 0;
		log("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d", typeArray[i][j++], typeArray[i][j++], typeArray[i][j++], typeArray[i][j++],
			typeArray[i][j++], typeArray[i][j++], typeArray[i][j++], typeArray[i][j++], typeArray[i][j++],
			typeArray[i][j++]);
	}
	//初始化外圈
	for (int i = 0; i <= MAP_SIZE_HEIGHT + 1; ++i)
	{
		typeArray[i][0] = CubeType::blank;
		typeArray[i][MAP_SIZE_WIDTH + 1] = CubeType::blank;
	}
	for (int i = 0; i <= MAP_SIZE_WIDTH + 1; ++i)
	{
		typeArray[0][i] = CubeType::blank;
		typeArray[MAP_SIZE_HEIGHT + 1][i] = CubeType::blank;
	}
	
}

bool NewGame::onTouchBegan(Touch* touch, Event *unusedEvent)
{
	currentFingerCube = getCubeByPos(touch->getLocation());
	if(currentFingerCube!=nullptr)
	     currentFingerCube->setScale((float)CUBE_TOUCH_SCALE);
	return true;
}

void NewGame::onTouchMoved(Touch* touch, Event *unusedEvent)
{
	Vec2 pos = touch->getLocation();
	auto cube=getCubeByPos(pos);
	if (cube == nullptr)
	{
		if(currentFingerCube!=nullptr)
		currentFingerCube->setScale((float)CUBE_SCALE);
		return;
	}
	if (cube!=currentFingerCube && cube && cube->isVisible())
	{
		cube->setScale((float)CUBE_TOUCH_SCALE);
		if (currentFingerCube != nullptr)
		    currentFingerCube->setScale((float)CUBE_SCALE);
		currentFingerCube = cube;
	}
}

void NewGame::removeLightning(float dt)
{
	while(this->getChildByTag(CubeType::lightning)!=nullptr)
	this->removeChildByTag(CubeType::lightning);
}

void NewGame::menuCallback(cocos2d::Ref * sender)
{
	this->removeAllChildren();
	init();
	
}


void NewGame::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unusedEvent)
{
	if(currentFingerCube!=nullptr)
	currentFingerCube->setScale((float)CUBE_SCALE);
	log("touch pos,%f,%f", touch->getLocation().x, touch->getLocation().y);
	Vec2 pos = touch->getLocation();
	auto  cube = getCubeByPos(pos);
	if (cube&&cube->isVisible())
	{
		if (currentCube == nullptr)
		{
			currentCube = cube;
			auto star = Sprite::createWithSpriteFrameName("fruit_24.png");
			star->setPosition(Vec2(CUBE_WIDTH / 2, CUBE_HEIGHT / 2));
			star->setScale((float)1.0 / 10);
			cube->addChild(star, 1, CubeType::border);
		}
		else
		{
			if (canDelete(currentCube, cube))
			{
				cube->runAction(Sequence::create(DelayTime::create(LIGHTNINGLASTTIME)
					, Hide::create(), NULL));
				auto cubeCoord = changePosToCoord(cube->getPosition());
				typeArray[cubeCoord.second][cubeCoord.first] = CubeType::blank;

				currentCube->runAction(Sequence::create(DelayTime::create(LIGHTNINGLASTTIME)
					, Hide::create(), NULL));
				auto currentCubeCoord = changePosToCoord(currentCube->getPosition());
				typeArray[currentCubeCoord.second][currentCubeCoord.first] = CubeType::blank;
			}
			currentCube->removeChildByTag(CubeType::border);
			currentCube = nullptr;
		}
	}
	if (isWin())
		win();
	//while(noneIsConnect())
		resetCube();
	
}

bool NewGame::canDelete(Cube * cube1, Cube * cube2)
{
	if (cube1 == cube2)                       //同一方块不可消除
		return false;
	if (cube1->getType() == cube2->getType())    //两方块类型相同
	{
		pair<int, int> coord1 = changePosToCoord(cube1->getPosition());
		pair<int, int> coord2 = changePosToCoord(cube2->getPosition());
		canDeleteWithThreeLines = true;
		if (isConnectedWithOneLine(coord1.first, coord1.second , coord2.first , coord2.second ))  //一条线消除
			return true;
		canDeleteWithThreeLines = false;
		if (isConnectedWithTwoLines(coord1.first , coord1.second , coord2.first , coord2.second )) // 两条线消除
			return true;
		
		if (isConnectedWithThreeLines(coord1.first , coord1.second , coord2.first, coord2.second )) //三条线消除
			return true;
	}
	return false;
}

bool NewGame::canDelete(int x1, int y1, int x2, int y2)
{
	//一条线相连
	
	//两条线相连

	return false;
}

bool NewGame::isConnectedWithOneLine(int x1, int y1, int x2, int y2)
{
	bool isConnected = false;
	if (x1 == x2)
	{
		if (y1 < y2)
		{
			for (int i = y1 + 1; i < y2; i++)
			{
				if (typeArray[i][x1] != CubeType::blank)
					return isConnected;
			}
			isConnected = true;
		}
		else
		{
			for (int i = y2 + 1; i < y1; i++)
			{
				if (typeArray[i][x1] != CubeType::blank)
					return isConnected;
			}
			isConnected = true;
		}
	}
	else if (y1 == y2)
	{
		if (x1 < x2)
		{
			for (int i = x1 + 1; i < x2; i++)
			{
				if (typeArray[y1][i] != CubeType::blank)
					return isConnected;
			}
			isConnected = true;
		}
		else
		{
			for (int i = x2 + 1; i < x1; i++)
			{
				if (typeArray[y1][i] != CubeType::blank)
					return isConnected;
			}
			isConnected = true;
		}
	}
	if (isConnected&&canDeleteWithThreeLines)
		addLightning(x1, y1, x2, y2);
	scheduleOnce(schedule_selector(NewGame::removeLightning), LIGHTNINGLASTTIME);
	return isConnected;
}

bool NewGame::isConnectedWithTwoLines(int x1, int y1, int x2, int y2)
{
	if (typeArray[y1][x2] == CubeType::blank && isConnectedWithOneLine(x1, y1, x2, y1) && isConnectedWithOneLine(x2, y1, x2, y2))
	{
		canDeleteWithThreeLines = true;
		isConnectedWithOneLine(x1, y1, x2, y1);
		isConnectedWithOneLine(x2, y1, x2, y2);
		return true;
	}
	if (typeArray[y2][x1] == CubeType::blank && isConnectedWithOneLine(x1, y1, x1, y2) && isConnectedWithOneLine(x1, y2, x2, y2))
	{
		canDeleteWithThreeLines = true;
		isConnectedWithOneLine(x1, y1, x1, y2);
		isConnectedWithOneLine(x1, y2, x2, y2);
		return true;
	}
	return false;
}

bool NewGame::isConnectedWithThreeLines(int x1, int y1, int x2, int y2)
{
	if (x1 == x2) //一条线上
	{
		for (int i = x1 + 1,j=x1-1; i <= MAP_SIZE_WIDTH + 1 || j>=0; i++,j--)
		{
			if (i <= MAP_SIZE_WIDTH+1)
			{
				if (typeArray[y1][i] == CubeType::blank && isConnectedWithOneLine(x1, y1, i, y1) && isConnectedWithTwoLines(i, y1, x2, y2))
				{
					canDeleteWithThreeLines = true;
					isConnectedWithOneLine(x1, y1, i, y1);
					isConnectedWithTwoLines(i, y1, x2, y2);
					return true;
				}
			}
			if (j >= 0)
			{
				if (typeArray[y1][j] == CubeType::blank && isConnectedWithOneLine(x1, y1, j, y1) && isConnectedWithTwoLines(j, y1, x2, y2))
				{
					canDeleteWithThreeLines = true;
					isConnectedWithOneLine(x1, y1, j, y1);
					isConnectedWithTwoLines(j, y1, x2, y2);
					return true;
				}
			}
		}
	}
	else if (y1 == y2)
	{
		for (int i = y1 +1, j= y1 - 1; i <= MAP_SIZE_HEIGHT + 1 || j>=0; ++i,--j)
		{
			if (i <= MAP_SIZE_HEIGHT + 1)
			{
				if (typeArray[i][x1] == CubeType::blank && isConnectedWithOneLine(x1, y1, x1, i) && isConnectedWithTwoLines(x1, i, x2, y2))
				{
					canDeleteWithThreeLines = true;
					isConnectedWithOneLine(x1, y1, x1, i);
					isConnectedWithTwoLines(x1, i, x2, y2);
					return true;
				}
			}
			if (j >= 0)
			{
				if (typeArray[j][x1] == CubeType::blank && isConnectedWithOneLine(x1, y1, x1, j) && isConnectedWithTwoLines(x1, j, x2, y2))
				{
					canDeleteWithThreeLines = true;
					isConnectedWithOneLine(x1, y1, x1, j);
					isConnectedWithTwoLines(x1, j, x2, y2);
					return true;
				}
			}
		}
	}
	else                                  //不在一条线上
	{                                                                         
			for (int i = x1 + 1, j = x1-1; i <= MAP_SIZE_WIDTH + 1 || j>= 0; ++i ,--j)
			{
				if (i <= MAP_SIZE_WIDTH + 1)
				{
					if (typeArray[y1][i] == CubeType::blank && isConnectedWithOneLine(x1, y1, i, y1) && isConnectedWithTwoLines(i, y1, x2, y2))
					{
						canDeleteWithThreeLines = true;
						isConnectedWithOneLine(x1, y1, i, y1);
						isConnectedWithTwoLines(i, y1, x2, y2);
						return true;
					}
				}
				if (j >= 0)
				{
					if (typeArray[y1][j] == CubeType::blank && isConnectedWithOneLine(x1, y1, j, y1) && isConnectedWithTwoLines(j, y1, x2, y2))
					{
						canDeleteWithThreeLines = true;
						isConnectedWithOneLine(x1, y1, j, y1);
						isConnectedWithTwoLines(j, y1, x2, y2);
						return true;
					}
				}
			}
			for (int i = y1 + 1, j =y1 - 1; i <= MAP_SIZE_HEIGHT + 1 || j >= 0; ++i, --j)
			{
				if (i <= MAP_SIZE_HEIGHT + 1)
				{
					if (typeArray[i][x1] == CubeType::blank && isConnectedWithOneLine(x1, y1, x1, i) && isConnectedWithTwoLines(x1, i, x2, y2))
					{
						canDeleteWithThreeLines = true;
						isConnectedWithOneLine(x1, y1, x1, i); 
						isConnectedWithTwoLines(x1, i, x2, y2);
						return true;
					}
				}
				if (j >= 0)
				{
					if (typeArray[j][x1] == CubeType::blank && isConnectedWithOneLine(x1, y1, x1, j) && isConnectedWithTwoLines(x1, j, x2, y2))
					{
						canDeleteWithThreeLines = true;
						isConnectedWithOneLine(x1, y1, x1, j);
						isConnectedWithTwoLines(x1, j, x2, y2);
						return true;
					}
				}
			}
	}
	return false;
}

bool NewGame::isPosBlank(Vec2 pos)
{
	pair<int,int> coord= changePosToCoord(pos);
	if (coord.first == -1)
		return true;
	if (atlas.at(coord.second).at(coord.first)->isVisible() == false)
		return true;
	return false;
}

bool NewGame::noneIsConnect()
{
	for(int i =0;i<MAP_SIZE_HEIGHT;++i)
		for (int j = 0; j < MAP_SIZE_HEIGHT; ++j)
		{
			
			int type = atlas.at(i).at(j)->getType();
			auto cube1 = atlas.at(i).at(j);
			for(int k =i;k<MAP_SIZE_HEIGHT;++k)
				for (int h = j; h < MAP_SIZE_WIDTH; ++h)
				{
					if (k == i&&j == h)
						continue;
					if (atlas.at(k).at(h)->getType() == type)
					{
						auto cube2 = atlas.at(k).at(h);
						if (canDelete(cube1, cube2))
							return true;
					}
				}
		}
	return false;
}

void NewGame::resetCube()
{
	resetVector.clear();
	for (int i = 1; i <= MAP_SIZE_HEIGHT ; ++i)
		for (int j = 1; j <= MAP_SIZE_WIDTH ; ++j)
		{
			if (typeArray[i][j] != CubeType::blank)
				resetVector.push_back(typeArray[i][j]);
		}
	random_shuffle(resetVector.begin(), resetVector.end());
	vector<int>::iterator it = resetVector.begin(); 
		for (int i = 1; i <= MAP_SIZE_HEIGHT; ++i)
			for (int j = 1; j <= MAP_SIZE_WIDTH; ++j)
			{
				if (typeArray[i][j] != CubeType::blank)
				{
					typeArray[i][j] = *it;
					++it;
					auto cube = atlas.at(i - 1).at(j - 1);
					auto texture = Sprite::createWithSpriteFrameName(StringUtils::format("fruit_%02d.jpg", typeArray[i][j]).c_str())->getTexture();
					cube->setTexture(texture);
				}
			}
}

bool NewGame::isWin()
{
	for (int i = 0; i <= MAP_SIZE_HEIGHT + 1; ++i)
		for (int j = 0; j <= MAP_SIZE_WIDTH + 1; ++j)
		{
			if (typeArray[i][j] != CubeType::blank)
				return false;
		}
	return true;
}

void NewGame::win()
{
	if (isWin())
	{	
		auto label = Label::createWithSystemFont("YOU WIN ", "Arial", 70);
		label->setColor(Color3B::BLACK);
		this->addChild(label);
		label->setPosition(visibleSize.width / 2, visibleSize.height / 3 * 2);

		auto restart = Label::createWithSystemFont("Restart ", "Arial", 70);
		restart->setColor(Color3B::BLACK);

		auto menuItem = MenuItemLabel::create(restart, CC_CALLBACK_1(NewGame::menuCallback, this));
		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(visibleSize.width / 2, visibleSize.height / 3 );
		this->addChild(menu);
	}
	
}

void NewGame::createLightningByPos(int x1, int y1,LightningType type,Direction dir)
{
	auto lightning = Sprite::createWithSpriteFrameName(__String::createWithFormat("lightning_%d.png", type)->getCString());
	lightning->setScale((CUBE_HEIGHT*CUBE_SCALE)/ lightning->getContentSize().height);
	auto pos =changeCoordToPos( x1, y1 );
	switch (dir)
	{
	case Direction::left :
		lightning->setPosition(pos + Vec2(-CUBE_WIDTH*CUBE_SCALE / 2, 0));
		break;
	case Direction::right:
		lightning->setPosition(pos + Vec2(CUBE_WIDTH*CUBE_SCALE / 2, 0));
		break;
	case Direction::up:
		lightning->setPosition(pos + Vec2(0, CUBE_HEIGHT*CUBE_SCALE / 2));
		break;
	case Direction::down:
		lightning->setPosition(pos + Vec2(0, CUBE_HEIGHT*CUBE_SCALE / 2));
		break;
	default:
		log("direction error");
	}
	this->addChild(lightning, 1, CubeType::lightning);
	
}

void NewGame::addLightning(int x1,int y1, int x2,int y2)
{
	if (x1 == x2)
	{
		if (y1 < y2)
		{
			for (int i = y1 ; i < y2; i++)
			{
				createLightningByPos( x1, i,LightningType::vertical,Direction::up);
			}
		}
		else
		{
			for (int i = y2 ; i < y1; i++)
			{
				createLightningByPos( x1,  i,LightningType::vertical,Direction::up);
			}
		}
	}
	else if (y1 == y2)
	{
		if (x1 < x2)
		{
			for (int i = x1 ; i < x2; i++)
			{
				createLightningByPos(i, y1, LightningType::horizon,Direction::right);
			}
			
		}
		else
		{
			for (int i = x2 ; i < x1; i++)
			{
				createLightningByPos(i, y1, LightningType::horizon,Direction::right);
			}
		}
	}
}

Cube * NewGame::getCubeByPos(Vec2 pos)
{
	pair<int, int> coord = changePosToCoord(pos);
	if (coord.first == -1 || coord.first == 0 || coord.second == 0 || coord.first == MAP_SIZE_WIDTH + 1 || coord.second == MAP_SIZE_HEIGHT + 1)
		return nullptr;
	return atlas.at(coord.second-1).at(coord.first-1);
}
