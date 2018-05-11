#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	components.Initialize(&entities);
	
	components.AddComponent<TransformComponent>()->Initialize(entities, 1024);
	components.AddUpdatableComponent<LifetimeComponent>()->Initialize(entities, 1024);
	components.AddUpdatableComponent<SpawnerComponent>()->Initialize(entities, 1024);
	components.AddUpdatableComponent<VisualComponent>()->Initialize(entities, 1024, this);
	
	// bind event
	entities.AddEventListener(components.GetComponent<VisualComponent>().get());
	
#if 0
	Archetype archetype = {
		typeid(TransformComponent),
		typeid(LifetimeComponent),
		typeid(VisualComponent),
	};
	Entity entity = components.CreateEntity(archetype);
	auto lifetimeComponent = components.GetComponent<LifetimeComponent>();
	auto handle = lifetimeComponent->GetHandle(entity);
	lifetimeComponent->SetLifetime(handle, {0, 240});
#else
	Archetype archetype = {
		typeid(SpawnerComponent),
	};
	components.CreateEntity(archetype);
#endif
	
	scheduleUpdate();
	
    return true;
}

void HelloWorld::update(float delta)
{
	components.Update(delta);
	components.GC();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
