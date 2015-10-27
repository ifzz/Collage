Collage
=======

Written by Luke Martin (Entropy Machine LLC.)
All rights reserved.

Twitter: @CodeEntropy
Email: EntropyMachineGames@gmail.com

Have fun!

Collage is an advanced version of an ECS I wrote to make RoboCorps.
Ideally, this re-write will address a number of issues I encountered while working on it.

Problem 1: One World
--------------------

RoboCorps maintained only one active entity pool for managing the "world". Menus, input, and
a number of other systems used their own hand-crafted approach to item management. This
resulted in a lot of cross-talk and explicit hacks to work around the ECS rather than with it.

A related issue was mode-shifting. RoboCorps handled pausing by turning off systems related to
item and actor logic. If the menus were to use the ECS during this time, work-arounds would need
to be put in place to whitelist entityIds that ignore certain system-wide pauses.

The solution:

* Worlds are named based on their purpose (e.g.: menu, pause, gameplay).
* Worlds consist of entities and systems that share a common goal.
* getWorld() remains the same - returns the active World.
* setWorld(worldName) changes the current active World to `worldName`.

Problem 2: Systems and Entities
-------------------------------

By the time RoboCorps was approaching release, gameplay programming largely began to avoid the
ECS due to the rigidness of translating context-sensitive Event information to all Systems in the World.


The solution:

	init:
		createEvent(&EVENT_DAMAGE)
		createSystem(EVENT_DAMAGE, COMPONENT_STATS, &damageCallback);
	
	triggerEvent(entityId, EVENT_DAMAGE, &eventData)

* `triggerEvent` accepts a void pointer 

Use Cases 
=========

Game Loop
---------

The problem with 



ToDo
----

Component IDs are not reused after delete.
	The constant would also need to be returned to 0.
