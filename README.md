Tetris Game using C++ and OpenGL
================================

The game was coded using just C++, OpenGL (with GLFW and GLAD), and 2D rendering, making it a simple classic Tetris version. It presents the workings of a game, handles user controls, and uses OpenGL to show things in real time.

----------------------------------------
Demonstration Video 
====
https://drive.google.com/file/d/1ncYeuxarPSRRQxrQCeNg2ciEnrDBO28X/view?usp=sharing

---------------------------------------
Tetromino Pieces Explained
==========================

There are 7 special shapes called Tetrominoes in classic Tetris. Every piece consists of 4 blocks and can be placed inside the central 4x4 area.


    I - Straight line
    O - Square block
    T - T-shaped
    S - S-shaped
    Z - Z-shaped
    J - J-shaped
    L - L-shaped

These are stored as 16-element (4x4) arrays for rotation and placement.

---------------------------------------------
Features
========

1. 7 Tetromino types
2. Real-time movement and rotation
3. Line clearing with score tracking
4. Game over detection
5. Color rendering with OpenGL
6. Smooth keyboard controls
7. Gravity-based falling logic

--------------------------------------------------
How It was Built
================


A. OpenGL Setup
===============
   - GLFW: Manages the window and keyboard input.
   - GLAD: Loads OpenGL function pointers.
   - Window resolution: 600x800 pixels.
     
B. Game Logic Overview
==================
   
1. `bool doesPieceFit(int tetromino, int rotation, int posX, int posY); `

Checks if a piece can fit on the board at the given rotation and position.

- Each loop goes through every square of the tetromino’s 4x4 arrangement.
- The function uses rotate(x, y, rotation) to find the right index after rotating the view.
- If any part of the given Path goes into a filled area or outside its limits, returns false.



2. `int rotate(int x, int y, int rotation);`

Rotates a tetromino (0°, 90°, 180°, 270°).

Rotation logic using precomputed formulas:
  - 0° → index = y * 4 + x
  - 90° → index = 12 + y - (x * 4)
  - 180° → index = 15 - (y * 4) - x
  - 270° → index = 3 - y + (x * 4)

3. `void lockPiece();`

Locks the current Tetromino onto the game board.
  - Fills the board array with the tetromino index.
  - Marks position as occupied.

4. `void clearLines();`

Clears completed lines and shifts remaining blocks down.
  - Iterates each row from bottom to top.
  - If a full row is found (all cells filled), remove and shift above rows down.
  - Increments score (totalClearedLines).

5. `void renderBoard();`
   
Draws the current board state with OpenGL.
  - Uses glBegin(GL_QUADS) for filled blocks.
  - Draws colored squares per cell type.
  - Optionally outlines with GL_LINE_LOOP.

6. `void drawTetromino();`

Renders the active falling piece.
  - Loops over 4x4 tetromino grid.
  - Translates its current x, y position.
  - Applies color based on Tetromino type.

7. `void gameLoop();`

Main game loop: updates, input handling, and rendering.
  - Processes GLFW events (keyboard input).
  - Handles movement, rotation, falling speed.
  - Calls render functions and updates window.

8. `void handleInput();`

Processes player controls via keyboard.
  - LEFT → Move left
  - RIGHT → Move right
  - DOWN → Speed up fall
  - UP → Rotate piece

C. Game Board Details
==========
Grid size: 15 (width) x 20 (height)
Each cell stores:
  - 0: empty
  - 1–7: Tetromino type
------------------------------

How to Run
==========

1. Clone the Repository
  - `git clone https://github.com/Meseret1G/TetrisGame-Using-C-And-OpenGL.git`
  - `cd TetrisGame-Using-C-And-OpenGL`

2. Build the Project
`.\build_cmake.bat`

3. Run 
`.\build\main.exe`

------------------------------------

Requirements
============
C++ Compiler
OpenGL libraries:
  - GLFW
  - GLAD
  - CMake
    
Ensure you have GLAD and GLFW installed properly!!!
==============
