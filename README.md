# GTC (Place holder name!)

A voxel style game with open world and game mechanics of GTA San Andreas.


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
├── voxel/
│   ├── block/
│   ├── chunk/
│   ├── world/
│   ├── meshing/
│   ├── generation/
│   └── streaming/
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




