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
    
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
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
	
	auto moveItem = MenuItemImage::create(
										   "CloseNormal.png",
										   "CloseSelected.png",
										  [this](Ref* pSender)
	{
		switch (mode)
		{
			case Mode::Dig:
				mode = Mode::Normal;
				textMode->setString("Mov");
				break;
			default:
				mode = Mode::Dig;
				textMode->setString("Dig");
				break;
		}
	});
	
	moveItem->setPosition(Vec2(origin.x + visibleSize.width - moveItem->getContentSize().width/2,
								origin.y + moveItem->getContentSize().height*2));
	
	menu->addChild(moveItem);
	
	
	mode = Mode::Dig;
	textMode = Label::createWithSystemFont("ini", "HiraKakuProN-W6", 12);
	textMode->setPosition(cocos2d::Point(origin.x + visibleSize.width - textMode->getContentSize().width,
										 origin.y + visibleSize.height - textMode->getContentSize().height/2));
	textMode->setAnchorPoint({0.5, 1});
	this->addChild(textMode);

    /////////////////////////////
    // 3. add your codes below...
	
	newecsdemo.Init();
	
	input.Reset();
	
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
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				input.KeyPress(3);
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				input.KeyPress(4);
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
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				input.KeyRelease(3);
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				input.KeyRelease(4);
				break;
			default:
				break;
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener , this);
	
	// タッチイベント
	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		
		switch (mode)
		{
			case Mode::Dig:
				field.Dig(location.x, location.y);
				break;
			default:
				input.TouchBegin({location.x, location.y});
				break;
		}
		
		return true;
	};
	
	touch->onTouchMoved = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		
		switch (mode)
		{
			case Mode::Dig:
				field.Dig(location.x, location.y);
				break;
			default:
				input.TouchMove({location.x, location.y});
				break;
		}
		
		return true;
	};
	
	touch->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		auto location = touch->getLocation();
		
		switch (mode)
		{
			case Mode::Dig:
				field.SelectTile(location.x, location.y);
				break;
			default:
				input.TouchEnd({location.x, location.y});
				break;
		}
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);
	
	// 物理エンジン
	physics.Initialize();
	
	// フィールド
	//field.Initialize(this);
	
	// コンポーネント
	components.Initialize(&entities);
	
	const int maxSize = 1024 * 5;
	
	auto&& transformComponent = components.AddComponent<TransformComponent>();
	transformComponent->Initialize(entities, maxSize);
	physics.SetSharedComponent(transformComponent.get());
	
	auto&& rigidbodyComponent = components.AddComponent<RigidBodyComponent>();
	rigidbodyComponent->Initialize(entities, maxSize);
	rigidbodyComponent->SetSharedComponent(transformComponent.get());
	rigidbodyComponent->SetPhysics(&physics);
	
	auto&& spawnComponent = components.AddUpdatableComponent<SpawnerComponent>();
	spawnComponent->Initialize(entities, maxSize);
	
	auto&& playerControlComponent = components.AddUpdatableComponent<PlayerControlComponent>();
	playerControlComponent->Initialize(entities, maxSize);
	playerControlComponent->SetSharedComponent(rigidbodyComponent.get());
	playerControlComponent->SetInput(&input);
	
	auto&& projectileComponent = components.AddUpdatableComponent<ProjectileComponent>();
	projectileComponent->Initialize(entities, maxSize);
	projectileComponent->SetSharedComponent(transformComponent.get());
	entities.AddEventListener(projectileComponent.get());
	
	auto&& lifetimeComponent = components.AddUpdatableComponent<LifetimeComponent>();
	lifetimeComponent->Initialize(entities, maxSize);

	auto&& visualComponent = components.AddPostUpdatableComponent<VisualComponent>();
	visualComponent->Initialize(entities, maxSize, this);
	visualComponent->SetSharedComponent(transformComponent.get());
	
	auto&& touchSpawnComponent = components.AddUpdatableComponent<TouchPointSpawnerComponent>();
	touchSpawnComponent->Initialize(entities, maxSize);
	touchSpawnComponent->SetSharedComponent(transformComponent.get());
	touchSpawnComponent->SetSharedComponent(lifetimeComponent.get());
	touchSpawnComponent->SetSharedComponent(visualComponent.get());
	touchSpawnComponent->SetInput(&input);
	
	auto&& walkComponent = components.AddUpdatableComponent<WalkComponent>();
	walkComponent->Initialize(entities, maxSize);
	walkComponent->SetSharedComponent(transformComponent.get());
	walkComponent->SetField(&field, this);
	
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
#if 0
	// プレイヤーキャラ
	{
		Archetype archetype = {
//			typeid(SpawnerComponent),
//			typeid(MoveComponent),
//			typeid(WalkComponent),
			typeid(TransformComponent),
			typeid(VisualComponent),
		};
		Entity entity = components.CreateEntity(archetype);
//		{
//			auto handle = spawnComponent->GetHandle(entity);
//			Archetype archetype = {
//				typeid(TransformComponent),
//				typeid(LifetimeComponent),
//				typeid(LeftComponent),
//				typeid(VisualComponent),
//			};
//			spawnComponent->SetArchetype(handle, archetype);
//		}
		{
			auto handle = visualComponent->GetHandle(entity);
			visualComponent->SetTextureName(handle, "mon_002.png");
			handle = transformComponent->GetHandle(entity);
			transformComponent->SetPosition(handle, {100, 96-16});
		}
	}
#endif
#if 0
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
#if 0
	auto sp = Sprite::create("chip02d_dungeon.png");
	sp->getTexture()->setAliasTexParameters();
	sp->setTextureRect(Rect(0, 0, 16, 16));
	sp->setContentSize(Size(32, 32));
	sp->setPosition(Vec2(0, 0));
	sp->setAnchorPoint(Vec2(0, 0));
	sp->setScale(8);
	addChild(sp);
#endif
	
	physics.SetOnCollideFunc([this](Entity entity1, Entity entity2)
	{
		entities.SendCollideEvent(entity1, entity2);
	});
	
	field.SetCreateTileFunc([this](int id, int index, int x, int y) {
		
		if (m_TileEntities.count(index) > 0)
		{
			Entity entity = m_TileEntities.at(index);
			entities.Remove(entity);
			
			m_TileEntities.erase(index);
		}
		
		switch(id)
		{
			case 1:
			{
				// wall
				Archetype archetype = {
					typeid(TransformComponent),
					typeid(RigidBodyComponent),
				};
				auto&& transformComponent = components.GetComponent<TransformComponent>();
				auto&& rigidbodyComponent = components.GetComponent<RigidBodyComponent>();
				
				Entity entity = components.CreateEntity(archetype);
				m_TileEntities[index] = entity;
				{
					auto handle = rigidbodyComponent->GetHandle(entity);
					rigidbodyComponent->SetGroup(handle, 1000);
				}
				{
					auto handle = transformComponent->GetHandle(entity);
					transformComponent->SetPosition(handle, Vector2f(x-8, y-8));
				}
				break;
			}
			case 3:
			{
				// player
				Archetype archetype = {
					typeid(TransformComponent),
					typeid(RigidBodyComponent),
					typeid(PlayerControlComponent),
					typeid(VisualComponent),
				};
				auto&& transformComponent = components.GetComponent<TransformComponent>();
				auto&& rigidbodyComponent = components.GetComponent<RigidBodyComponent>();
				auto&& visualComponent = components.GetComponent<VisualComponent>();
				
				Entity entity = components.CreateEntity(archetype);
				m_TileEntities[index] = entity;
				{
					auto handle = rigidbodyComponent->GetHandle(entity);
					rigidbodyComponent->SetGroup(handle, 1);
				}
				{
					auto handle = transformComponent->GetHandle(entity);
					transformComponent->SetPosition(handle, Vector2f(x-8, y-8));
				}
				{
					auto handle = visualComponent->GetHandle(entity);
					visualComponent->SetRect(handle, id % 8 * 16, id / 8 * 16, 16, 16);
				}
				
				break;
			}
			case 5:
			{
				// trap
				Archetype archetype = {
					typeid(TransformComponent),
					typeid(RigidBodyComponent),
					typeid(SpawnerComponent),
					typeid(VisualComponent),
				};
				{
					auto&& transformComponent = components.GetComponent<TransformComponent>();
					auto&& rigidbodyComponent = components.GetComponent<RigidBodyComponent>();
					auto&& spawnComponent = components.GetComponent<SpawnerComponent>();
					auto&& visualComponent = components.GetComponent<VisualComponent>();
					
					Entity entity = components.CreateEntity(archetype);
					m_TileEntities[index] = entity;
					{
						auto handle = rigidbodyComponent->GetHandle(entity);
						rigidbodyComponent->SetGroup(handle, 2);
					}
					{
						auto handle = transformComponent->GetHandle(entity);
						transformComponent->SetPosition(handle, Vector2f(x-8, y-8));
					}
					{
						auto handle = visualComponent->GetHandle(entity);
						visualComponent->SetRect(handle, id % 8 * 16, id / 8 * 16, 16, 16);
					}
					{
						Archetype archetype = {
							typeid(TransformComponent),
							typeid(RigidBodyComponent),
							typeid(LifetimeComponent),
							typeid(ProjectileComponent),
							typeid(VisualComponent),
						};
						auto handle = spawnComponent->GetHandle(entity);
						spawnComponent->SetArchetype(handle, archetype);
					}
				}
				break;
			}
		}
	});
	
	
	scheduleUpdate();
	
    return true;
}

void HelloWorld::update(float delta)
{
	newecsdemo.Update(delta);
	
	components.Update(delta);
	physics.Update(delta);
	components.PostUpdate(delta);
	components.GC();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
