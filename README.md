# CruptEngine -  By: Eloy De Cort 2DAE01
Digital Arts And Entertainment Engine + Game Project

GITHUB Link: https://github.com/EloyDeCort/CruptEngine

BIN/RELEASE: https://github.com/EloyDeCort/CruptEngine/releases/tag/V1.0

-------------------------------------------------------------

#Design Choices

#ECS - Entity Component System

- CruptEngine uses an Entity Component System based on Austin Morlan's (https://austinmorlan.com/posts/entity_component_system/).

This entity component system uses templating to link together entities, components and systems. 

It uses a variety of techniques such as bitsets to create signatures. These signatures can then be used to match components with their systems.

Be aware that all systems & components need to be registered, either on the game or engine side, depending on the funcitonality.

-------------------------------------------------------------

#Observer Pattern using a signal handler.

- Using the observer pattern we can easily give a list of delegates in this engine. This list of delegates will receive a signal component. Containing certain data needed.

The observer pattern allows for systems to not run every frame, rather be reactive based on a delegate that is called on them.

-------------------------------------------------------------

#Command Pattern for rebinding.

- Using a command pattern for inputs, we can easily make new functionality and bind it to certain keyboard & controller buttons.

CruptEngine uses a very simple system in which it can easily register and rebind certain bindings. These bindings are connected to commands, that execute a certain behavior (ex. Jumping).

CruptEngine uses this pattern in combination with the observer pattern to notify systems to perform a jump, saving us the cost of running a system each frame.

-------------------------------------------------------------

#Following Libraries were used:

- Graphics: SDL (OpenGL)

- Audio: FMOD Core API

- Tile Parser: TmxLite (https://github.com/fallahn/tmxlite)


[Demo Game]: Bubble Bobble.

This is a small demo game showcasing the usage of the engine.
It goes through the most important factors: Initializing systems, components, registering them,...

It also goes through a more in-depth view on how to use the collision systems to create entity specific callbacks.