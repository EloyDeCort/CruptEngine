#pragma once
//Source & Inspiration: https://austinmorlan.com/posts/entity_component_system
#include <cstdint>
#include <bitset>
namespace crupt
{
	//ECS type Initialisation 

	//Certain size for entitity (type alias)
	using Entity = size_t;
	//Max amount of entities allowed
	const Entity Max_Entities = 5000;
	//Set size for components
	using ComponentType = size_t;
	//Max amount of components
	const ComponentType Max_Components = 32;

	//We need a signature to determine what Components an Entity has.
	//This can be done using a Bitfield. In modern c++ std::bitset is used.
	//Example: Transform = type 0, Rigidbody = type 1, Gravity = type 2
	//If an entitity has these three components, the signature would be 0b111 -> bits 0 1 2 are set.

	using Signature = std::bitset<Max_Components>;

	//initializes the signature with max_components amount of bits.

	//NOTE: Each Entity will have a signature.
}


