# GTC (Place holder name!)

A 2d pirate, RPG, wold exploration game.


<br>


## BUILD & RUN

For Dev build run:

```

./debug-build.sh

```

it whill compile and run the project automatacly

<br>

For Release build:

```

./release-build.sh

```

It will run the build for release build and with optimisations for the prod version.


<br>


## Project structure
```


# for now 
GTC/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── LICENSE
│
├── cmake/
│   └── windows-mingw.cmake
│
├── assets/
│   ├── textures/
│   │   ├── player/
│   │   │   └── player.png
│   │   ├── world/
│   │   ├── environment/
│   │   └── ui/
│   │
│   ├── audio/
│   │   ├── music/
│   │   └── sounds/
│   │
│   ├── fonts/
│   └── data/
│
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── application/
│   │   │   ├── application.cpp
│   │   │   └── application.hpp
│   │   │
│   │   ├── window/
│   │   │   ├── main_window.cpp
│   │   │   └── main_window.hpp
│   │   │
│   │   ├── time/
│   │   │   ├── time.cpp
│   │   │   └── time.hpp
│   │   │
│   │   └── input/
│   │       ├── input_manager.cpp
│   │       └── input_manager.hpp
│   │
│   ├── game/
│   │   ├── game.cpp
│   │   ├── game.hpp
│   │   │
│   │   ├── player/
│   │   │   ├── player.cpp
│   │   │   └── player.hpp
│   │   │
│   │   └── world/
│   │       ├── world.cpp
│   │       └── world.hpp
│   │
│   ├── scenes/
│   │   ├── scene.hpp
│   │   └── test_scene/
│   │       ├── test_scene.cpp
│   │       └── test_scene.hpp
│   │
│   ├── rendering/
│   │   ├── renderer.cpp
│   │   ├── renderer.hpp
│   │   └── camera/
│   │       ├── camera.cpp
│   │       └── camera.hpp
│   │
│   ├── resources/
│   │   ├── texture_manager.cpp
│   │   └── texture_manager.hpp
│   │
│   └── audio/
│       ├── audio_manager.cpp
│       └── audio_manager.hpp
│
├── tests/
│
├── tools/
│
└── build/



# test structure
src/
│
├── core/
│   ├── application/
│   ├── window/
│   ├── input/
│   ├── time/
│   ├── logging/
│   └── profiling/
│
├── renderer/
│   ├── renderer/
│   ├── camera/
│   ├── mesh/
│   ├── material/
│   └── shader/
│
│
├── world/
│   ├── island/
│   ├── biome/
│   ├── region/
│   ├── settlement/
│   ├── dungeon/
│   └── world_generation/
│
├── gameplay/
│   ├── player/
│   ├── ship/
│   ├── combat/
│   ├── inventory/
│   ├── crafting/
│   ├── trading/
│   └── progression/
│
├── ai/
│   ├── npc/
│   ├── wildlife/
│   ├── pirates/
│   └── navy/
│
├── systems/
│   ├── economy/
│   ├── bounty/
│   ├── weather/
│   └── quests/
│
├── resources/
│
└── game/

```


This is a plan for now not final just a structure that i imagine.




