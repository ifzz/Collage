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

Problem 3: Saving/Loading
-------------------------

Components should be easy enough to dump directly to disk, then just as easily be loaded back in.
I have a feeling there will be a few cases where some components just won't dump correctly:

	* Components with callbacks of their own.
	* Use of reference by Pointer.

This may be a design choice we make early on.

Problem 4: The component.h Problem
----------------------------------

When the line is drawn between framework and project-specific code, something to keep in mind is
how these things can be linked together logically. Currently, `component.h` describes the structures
in use by the project to the framework so it can allocate memory. However, this isn't so much a
problem as it is, again, something to keep in mind moving forward. KISS.

Tech
====

Debugging
---------

It should be relatively easy to pause the entity world and spin up another that can both access
and modify the main entity world. This would open up a lot of possibilties- mostly because objects
could be changed in the editor, then spit back out to their data file.

Object Blobs
------------

http://troydhanson.github.io/tpl/index.html

Entities are data-driven- kind of important to remember since we want to spend some time outside
the framework code itself. Basically, projects built in this framework would either need to parse these
files and describe it to the framework, or some sort of "compiler" could look over the files and produce
something more readable by the framework. Either way, we need to make this easy for people to understand,
so perhaps we could pay more attention to files like `component.h` (SEE: `The component.h Problem`)

Rendering
=========

Scenes
------

Process:
	* Sprites are added to a Scene.
	* Sprites are ordered on-the-fly via `UPDATE_SPRITE`.
	* The scene is rendered out.

Scenes track:
	* Sprite orders.
	* Camera offsets.


Use Cases 
=========

Game Loop
---------

The problem with 



ToDo
----

Component IDs are not reused after delete.
	The constant would also need to be returned to 0.
