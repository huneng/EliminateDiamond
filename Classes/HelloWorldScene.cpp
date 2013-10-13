#include "HelloWorldScene.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
USING_NS_CC;

FILE *file = fopen("D:\\log.txt", "w+");

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

int getDiamond(char* str){
	int t = rand()%5;
	switch(t){
	case 0:
		strcpy(str, "green.jpg");
		break;
	case 1:
		strcpy(str, "red.jpg");
		break;
	case 2:
		strcpy(str, "yellow.jpg");
		break;
	case 3:
		strcpy(str, "blue.jpg");
		break;
	case 4:
		strcpy(str, "purple.jpg");
		break;
	default:
		strcpy(str, "yellow.jpg");
		t = 2;
		break;
	}
	return t;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	float visibleW = visibleSize.width;
	float visibleH = visibleSize.height;
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                               "CloseNormal.png",
                               "CloseSelected.png",
                               this,
                               menu_selector(HelloWorld::menuCloseCallback));
    CCMenuItemImage *pPlayItem = CCMenuItemImage::create(
								"btn-play-normal.png",
								"btn-play-selected.png",
								this,
								menu_selector(HelloWorld::menuPlayCallback));
	float closeItemW = pCloseItem->getContentSize().width;
	float closeItemH = pCloseItem->getContentSize().height;
	
	pCloseItem->setPosition(ccp(closeItemW/2 , visibleH+origin.y - closeItemH/2));
	
	float playItemW = pPlayItem->getContentSize().width;
	float playItemH = pPlayItem->getContentSize().height;
	pPlayItem->setScale(0.7f);
	pPlayItem->setPosition(ccp(visibleW-playItemW*0.5, origin.y+playItemH*0.6));
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pPlayItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    pLabelTime = CCLabelTTF::create("Time:0", "Arial", 15);
	pLabelScore = CCLabelTTF::create("Score:0", "Arial", 15);

    // position the label on the center of the screen
    pLabelTime->setPosition(ccp(origin.x + visibleW/2,
		origin.y+ visibleH - pLabelTime->getContentSize().height));
	
	pLabelScore->setPosition(ccp(visibleW-playItemW*0.5,
		origin.y+visibleH/2));

    // add the label as a child to this layer
    this->addChild(pLabelTime, 1);
	this->addChild(pLabelScore, 1);


	int a = visibleW>visibleH?visibleH:visibleW;
	CCSize squreSize(a*0.8, a*0.8);
	
	dw = squreSize.width/8;
	dh = squreSize.height/8;
	float startX = origin.x + visibleW/2 - squreSize.width/2;
	float startY = origin.y + visibleH/2 - squreSize.height/2;
	diamondRect = CCRect(startX, startY, squreSize.width, squreSize.height);
	char str[30];
	
	srand(time(NULL));
	for(int i = 0; i < 64; i ++){
		base[i] = getDiamond(str);
		CCSprite* pSprite = CCSprite::create(str);
		diamonds.push_back(pSprite);
		//fprintf(file, "%s %d\n", str, base[i]);
	}
	//fclose(file);
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 8; j ++){
			CCSprite *pSprite =  diamonds[j*8+i];
			float scaleX = dw/pSprite->getContentSize().width;
			float scaleY = dh/pSprite->getContentSize().height;
			pSprite->setScaleX(scaleX);
			pSprite->setScaleY(scaleY);
			pSprite->setPosition(ccp(startX+i*dw+dw/2, startY+(7-j)*dh+dh/2));
			this->addChild(pSprite, i*8+j);
		}
	}
	m_score = -1;
	this->setTouchEnabled(true);
	
    return true;
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouches, cocos2d::CCEvent *pEvent){
	CCPoint point = pTouches->getLocation();
	return this->diamondRect.containsPoint(point);
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouches, cocos2d::CCEvent *pEvent){
	if(m_score<0){
		return;
	}
	CCPoint  point = pTouches->getLocation();
	char pos[100];

	int y_ = (int)((point.y-diamondRect.origin.y)/dh);
	int y = 7-y_;
	int x  = (int)((point.x-diamondRect.origin.x)/dw);
	int result[64] = {0};
	if(y>=8||x>=8){
		return;
	}
	int count = handlerTouchInSquare(y*8+x, result);
	if(count < 3)
		return;

	for(int i = 0; i < count; i++){
		CCSprite *sprite = diamonds[result[i]];
		CCRotateBy *rotateAct = new CCRotateBy();
		rotateAct->initWithDuration(0.2f, 90.0f);
		sprite->runAction(rotateAct);
		
		char str[20];
		CCImage *image = new CCImage();
		
		int color = getDiamond(str);
		base[result[i]] = color;
		image->initWithImageFile(str, CCImage::kFmtJpg);

		CCTexture2D* texture = new CCTexture2D();
		texture->initWithImage(image);
		sprite->setTexture(texture);
	}
	//delete rotateAct;
	m_score += 5*count;
	char str[20];
	sprintf(str, "Score: %d", m_score);
	pLabelScore->setString(str);
	//fflush(file);
}


void HelloWorld::updateSquare(float tm){
	for(int i = 0; i < 64; i++){
		CCSprite *sprite = diamonds[i];

		char str[20];
		CCImage *image = new CCImage();

		int color = getDiamond(str);
		base[i] = color;
		image->initWithImageFile(str, CCImage::kFmtJpg);

		CCTexture2D* texture = new CCTexture2D();
		texture->initWithImage(image);
		sprite->setTexture(texture);
	}
}

void HelloWorld::timing(float tm){
	m_time --;
	char str[10];
	sprintf(str, "Time: %d", m_time);
	pLabelTime->setString(str);
	if(m_time == 0){
		this->unschedule(schedule_selector(HelloWorld::timing));
		this->unschedule(schedule_selector(HelloWorld::updateSquare));
		m_score = -1;
		return ;
	}
}
void HelloWorld::registerWithTouchDispatcher(void){ 
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN+1, true);
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuPlayCallback(CCObject* pSender){
	m_time = 60;
	m_score = 0;
	pLabelTime->setString("Time: 60");
	pLabelScore->setString("Score: 0");
	this->schedule(schedule_selector(HelloWorld::timing), 1.0f);
	this->schedule(schedule_selector(HelloWorld::updateSquare), 10.0f);
}


int HelloWorld::handlerTouchInSquare(int seed, int result[]){
	int originVal = base[seed];
	int count = 0;
	result[count++] = seed;
	
	int stack[64] = {0};
	stack[0] = seed;
	int top = 0;
	
	while(top>=0){
		seed = stack[top];
		top --;
		//self
		base[seed] = -1;
		//up
		if(seed-8>=0&&originVal == base[seed-8]){
			stack[++top] = seed-8;
			result[count++] = seed-8;
			base[seed-8] = -1;
		}
		//down
		if(seed+8<64&&originVal == base[seed+8]){
			stack[++top] = seed+8;
			result[count++] = seed+8;
			base[seed+8] = -1;
		}
		//left
		if(seed-1>=0 && (seed-1)%8!=7 && originVal == base[seed-1]){
			stack[++top] = seed-1;
			result[count++] = seed-1;
			base[seed-1] = -1;
		}
		//right
		if(seed+1<64 && (seed+1)%8!=0 && originVal == base[seed+1]){
			stack[++top] = seed+1;
			result[count++] = seed+1;
			base[seed+1] = -1;
		}
		
	}
	
	if(count < 3){
		for(int i = 0; i < count; i++){
			base[result[i]] = originVal;
		}
	}
	return count;
}