# VoxelEngine

I've been inspired to recreate Minecraft in C++.

I am using a library for Perlin noise generation, and I would like to know if I need to rewrite one for this project.
My current goals are to optimize how chunks are loaded and reduce RAM impact.

## Currently
- 
- Multithreaded chunk generation

## Goals
- Lower memory impact
  - Make use of a "Chunk Palette" technique instead of chunks, allocating 16384 bytes in memory (16^3 blocks, each with a 16-bit integer allowing 2^16 block types)
- Multiplayer
- API for the creation of game modes. Uses Lua
  - Allow players to make a game they choose. They can create bosses, materials, custom game progressions, etc.
- There are two modes: edit and play.
  - Play allows you to play game modes you've already created
  - Edit allows for an easy way to edit the game mode and see what the game would look like.
    - Users are also free to edit or upload script game files
