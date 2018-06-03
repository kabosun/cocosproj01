#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ECS.h"
#include "component/Field.h"

enum class Mode
{
	Normal = 0,
	Dig,
};

class HelloWorld : public cocos2d::Layer
{
	EntityRegistry entities;
	ComponentRegistry components;
	Input input;
	Field field;
	Mode mode = Mode::Normal;
	cocos2d::Label* textMode;
	
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
	
	void update(float delta) override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
