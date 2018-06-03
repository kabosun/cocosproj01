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
	input.Reset();
	
	// タッチイベント
	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		input.TouchBegin({location.x, location.y});
		return true;
	};
	
	touch->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		input.TouchMove({location.x, location.y});
		return true;
	};
	
	touch->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		input.TouchEnd({location.x, location.y});
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);
	
	// コンポーネント
	components.Initialize(&entities);
	
	const int maxSize = 1024 * 5;
	
	auto&& transformComponent = components.AddComponent<TransformComponent>();
	transformComponent->Initialize(entities, maxSize);
	
	auto&& spawnComponent = components.AddUpdatableComponent<SpawnerComponent>();
	spawnComponent->Initialize(entities, maxSize);
	
	auto&& moveComponent = components.AddUpdatableComponent<MoveComponent>();
	moveComponent->Initialize(entities, maxSize);
	moveComponent->SetSharedComponent(transformComponent.get());
	moveComponent->SetInput(&input);
	
	auto&& leftComponent = components.AddUpdatableComponent<LeftComponent>();
	leftComponent->Initialize(entities, maxSize);
	leftComponent->SetSharedComponent(transformComponent.get());
	
	auto&& lifetimeComponent = components.AddUpdatableComponent<LifetimeComponent>();
	lifetimeComponent->Initialize(entities, maxSize);

	auto&& visualComponent = components.AddUpdatableComponent<VisualComponent>();
	visualComponent->Initialize(entities, maxSize, this);
	visualComponent->SetSharedComponent(transformComponent.get());
	
	auto&& touchSpawnComponent = components.AddUpdatableComponent<TouchPointSpawnerComponent>();
	touchSpawnComponent->Initialize(entities, maxSize);
	touchSpawnComponent->SetSharedComponent(transformComponent.get());
	touchSpawnComponent->SetSharedComponent(lifetimeComponent.get());
	touchSpawnComponent->SetSharedComponent(visualComponent.get());
	touchSpawnComponent->SetInput(&input);
	
	// bind event
	entities.AddEventListener(visualComponent.get());
	
#if 0
	Archetype archetype = {
		typeid(SpawnerComponent),
	};
	{
		Entity entity = components.CreateEntity(archetype);
		auto handle = spawnComponent->GetHandle(entity);
		Archetype archetype = {
			typeid(TransformComponent),
			typeid(LifetimeComponent),
			typeid(LeftComponent),
			typeid(VisualComponent),
		};
		spawnComponent->SetArchetype(handle, archetype);
	}
#endif
#if 1
	// プレイヤーキャラ
	{
		Archetype archetype = {
			typeid(SpawnerComponent),
			typeid(MoveComponent),
			typeid(TransformComponent),
			typeid(VisualComponent),
		};
		Entity entity = components.CreateEntity(archetype);
		{
			auto handle = spawnComponent->GetHandle(entity);
			Archetype archetype = {
				typeid(TransformComponent),
				typeid(LifetimeComponent),
				typeid(LeftComponent),
				typeid(VisualComponent),
			};
			spawnComponent->SetArchetype(handle, archetype);
		}
		{
			auto handle = visualComponent->GetHandle(entity);
			visualComponent->SetTextureName(handle, "mon_002.png");
			handle = transformComponent->GetHandle(entity);
			transformComponent->SetPosition(handle, {100, 100});
		}
	}
#endif
#if 1
	// タッチの軌跡
	{
		Archetype archetype = {
			typeid(TouchPointSpawnerComponent),
			typeid(TransformComponent),
		};
		Entity entity = components.CreateEntity(archetype);
		{
			auto handle = touchSpawnComponent->GetHandle(entity);
			Archetype archetype = {
				typeid(TransformComponent),
				typeid(LifetimeComponent),
				typeid(VisualComponent),
			};
			touchSpawnComponent->SetArchetype(handle, archetype);
		}
	}
#endif
	
	// キーイベント
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this]( cocos2d::EventKeyboard::KeyCode keyCode , cocos2d::Event* keyEvent ) {
		switch (keyCode)
		{
			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				input.KeyPress(1);
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				input.KeyPress(2);
				break;
			default:
				break;
		}
	};
	listener->onKeyReleased= [this]( cocos2d::EventKeyboard::KeyCode keyCode , cocos2d::Event* keyEvent ) {
		switch (keyCode)
		{
			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				input.KeyRelease(1);
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				input.KeyRelease(2);
				break;
			default:
				break;
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener , this);
	
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
