#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>



class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    
	void menuCloseCallback(CCObject* pSender);
	void menuPlayCallback(CCObject* pSender);
	bool ccTouchBegan(cocos2d::CCTouch *pTouches, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouches, cocos2d::CCEvent *pEvent); 
	void registerWithTouchDispatcher(void);// implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
private:
	int isInDiamondsRect();
	int handlerTouchInSquare(int seed, int result[]);
	void timing(float tm);
	void updateSquare(float tm);
private:
	int base[64];
	cocos2d::CCLabelTTF* pLabelScore;
	cocos2d::CCLabelTTF* pLabelTime;
	std::vector<cocos2d::CCSprite*> diamonds;
	cocos2d::CCRect exitRect;
	cocos2d::CCRect playRect;
	cocos2d::CCRect diamondRect;
	
	float dw, dh; //diamond  width & height
	int m_score;
	int m_time;
};

#endif // __HELLOWORLD_SCENE_H__
