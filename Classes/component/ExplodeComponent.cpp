#include "ExplodeComponent.h"
#include "TransformComponent.h"
#include "../ecs/ComponentRegistry.h"
#include <iostream>


void ExplodeComponent::Initialize(EntityRegistry& registry, int maxSize)
{
	Component::Initialize(registry, maxSize);
	
	m_Data.ExplodePerticileId.resize(maxSize);
	
	// コンポーネントをエンティティイベントに登録する
	registry.AddEventListener(this);
}

void ExplodeComponent::OnCreateEntity(Entity entity)
{}

void ExplodeComponent::OnRemoveEntity(Entity entity)
{
	auto m_TransformComponent = ComponentRegistry()->GetComponent<TransformComponent>();

	auto handle = m_TransformComponent->GetHandle(entity);
	auto position = m_TransformComponent->GetPosition(handle);

	// entityがなくなったら爆発エフェクトを発生させる
	std::cout << "ExplodeComponentSystem. Emit Explode Perticle. ID:" << entity.Index() << "(" << entity.Generation() << ") X:" << position.X << " Y:" << position.Y << " Z:" << position.Z << std::endl;
}

