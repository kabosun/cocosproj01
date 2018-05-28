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
	
	const int maxSize = 1024 * 2;
	
	auto&& transformComponent = components.AddComponent<TransformComponent>();
	transformComponent->Initialize(entities, maxSize);
	components.AddUpdatableComponent<SpawnerComponent>()->Initialize(entities, maxSize);
	auto&& moveComponent = components.AddUpdatableComponent<MoveComponent>();
	moveComponent->Initialize(entities, maxSize);
	moveComponent->SetSharedComponent(transformComponent.get());

	components.AddUpdatableComponent<VisualComponent>()->Initialize(entities, maxSize, this);
	components.AddUpdatableComponent<LifetimeComponent>()->Initialize(entities, maxSize);
	
	// bind event
	entities.AddEventListener(components.GetComponent<VisualComponent>().get());
	
	Archetype archetype = {
		typeid(SpawnerComponent),
	};
	components.CreateEntity(archetype);
	
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
