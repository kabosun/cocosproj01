#include "BallBehaviorComponent.h"
#include "../ecs/ComponentRegistry.h"

void BallBehaviorComponent::Initialize(EntityRegistry& registry, int maxSize)
{
	Component::Initialize(registry, maxSize);
	
	// コンポーネントをエンティティイベントに登録する
	registry.AddEventListener(this);
}

void BallBehaviorComponent::Update(EntityRegistry& registry, float dt)
{}

void BallBehaviorComponent::OnCreateEntity(Entity entity)
{
	// TODO 物理エンジンに登録する
}

void BallBehaviorComponent::OnRemoveEntity(Entity entity)
{
	// TODO 物理エンジンから解除する
}

void BallBehaviorComponent::OnContact(Entity body1, Entity body2)
{
	// TODO ブロックと衝突したらブロックのライフを減らす
	// body1が自分
	// そもそもボールでもブロックでもない？
}

