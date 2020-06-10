#include "CruptEnginePCH.h"
#include "PhysicsSystem.h"
#include "Components.h"
using namespace crupt;

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Init()
{
	m_pCoordinator = &ECSCoordinator::GetInstance();
}

void PhysicsSystem::Update(float dt)
{
	dt;
	for (const Entity& entity : m_Entities)
	{	
		//Get the transform & Renderable component
		const TransformComponent& transform = m_pCoordinator->GetComponent<TransformComponent>(entity);
		const GravityComponent& gravity = m_pCoordinator->GetComponent<GravityComponent>(entity);
		const VelocityComponent& velocity = m_pCoordinator->GetComponent<VelocityComponent>(entity);

		glm::vec3 pos = transform.position;

		pos;
		transform;
		gravity;
		velocity;
	}
}
