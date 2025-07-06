# Game of Life Implementation in C with ncurses

This project implements Conway's Game of Life, a cellular automaton, using the C programming language and the ncurses library for terminal-based visualization. The program reads an initial pattern from a text file, simulates the evolution of cells based on predefined rules, and allows users to control the simulation speed interactively.

## Overview

The Game of Life is a grid-based simulation where each cell can be alive (`1`) or dead (`0`). The state of each cell evolves over generations based on its neighbors, following these rules:

- A live cell with **2 or 3 live neighbors** stays alive.
- A live cell with **fewer than 2 live neighbors** dies (underpopulation).
- A live cell with **more than 3 live neighbors** dies (overpopulation).
- A dead cell with **exactly 3 live neighbors** becomes alive (reproduction).

This implementation uses a **25×80 grid**, reads the initial state from a text file, and displays the simulation in a terminal using **ncurses**. Users can adjust the speed of the simulation or exit the program using keyboard inputs.

## Features

- **File Input**: Loads initial patterns from a text file, where `'*'` represents a live cell and other characters (except newlines) represent dead cells.
- **Terminal Visualization**: Uses the ncurses library to display the grid in the terminal, with live cells shown as `'*'` and a status bar indicating speed and controls.
- **Interactive Controls**:
  - Press `A` or `a` to **increase simulation speed** (up to level 10).
  - Press `Z` or `z` to **decrease simulation speed** (down to level 1).
  - Press `Space` to **exit the program**.
- **Speed Control**: Supports 10 speed levels, with delays ranging from **2000ms (slowest)** to **50ms (fastest)**.
- **Toroidal Grid**: The grid wraps around at the edges, so cells at the top connect to the bottom, and cells at the left connect to the right.

## Code Structure

The program is organized into several key functions, each handling a specific aspect of the simulation.

### 1. `load_file(const char* name, int field[25][80])`

**Purpose**: Reads a text file and converts it into a 25×80 matrix of `0`s (dead) and `1`s (alive).  

**Input**:  
- `name`: Path to the input file (e.g., `pattern.txt`).  
- `field`: A 25×80 integer array to store the grid state.  

**Process**:  
1. Opens the file in read mode (`"r"`) using `fopen`.  
2. Initializes the `field` array to all `0`s to avoid undefined values.  
3. Reads the file character by character using `fgetc`:  
   - `'*'` sets the corresponding cell to `1` (alive).  
   - Newline (`\n`) characters are skipped, adjusting the column index to stay aligned.  
   - Other characters are ignored (treated as dead cells).  
4. If the file ends (`EOF`), the loop terminates early.  
5. Excess characters in a line (beyond 80 columns) are discarded until a newline or `EOF` is reached.  
6. Closes the file with `fclose`.  

**Output**: Returns `1` if the file is loaded successfully, `0` if the file cannot be opened.  

**Key Detail**: The function ensures robust file handling by checking for file existence and properly managing newlines and file boundaries.  

---

### 2. `draw(const int field[25][80], int speed)`

**Purpose**: Visualizes the current state of the grid in the terminal using ncurses.  

**Input**:  
- `field`: The 25×80 grid representing the current state.  
- `speed`: The current speed level (1–10).  

**Process**:  
1. Clears the terminal screen with `clear`.  
2. Iterates through the grid, printing `'*'` at position `(i, j)` for live cells (`field[i][j] == 1`) using `mvprintw`.  
3. Displays a status bar below the grid (at row 26) with the current speed and control instructions using `mvprintw`.  
4. Updates the screen with `refresh` to ensure all changes are visible.  

**Key Detail**: The use of `clear` and `refresh` ensures smooth, flicker-free updates by preparing the entire frame in memory before displaying it.  

---

### 3. `count_neighbors(const int field[25][80], int r, int c)`

**Purpose**: Counts the number of live neighbors for a cell at position `(r, c)`.  

**Input**:  
- `field`: The 25×80 grid.  
- `r, c`: Row and column indices of the cell.  

**Process**:  
1. Uses nested loops to check the 8 neighboring cells (3×3 grid, excluding the center cell).  
2. Implements a toroidal grid using modulo arithmetic (`(r + i + 25) % 25` and `(c + j + 80) % 80`) to wrap around the edges.  
3. Adds the value of each neighboring cell (`0` or `1`) to a counter.  

**Output**: Returns the total number of live neighbors.  

**Key Detail**: The modulo operation ensures that cells at the grid's edges correctly account for neighbors on the opposite side, creating a seamless wrap-around effect.  

---

### 4. `update_field(int field[25][80], int new_field[25][80])`

**Purpose**: Applies the Game of Life rules to compute the next generation of the grid.  

**Input**:  
- `field`: The current grid state.  
- `new_field`: A temporary grid to store the next state.  

**Process**:  
1. Iterates through each cell in `field`.  
2. Calls `count_neighbors` to determine the number of live neighbors (`n`) for each cell.  
3. Applies the Game of Life rules:  
   - **For a live cell (`field[i][j] == 1`)**:
     - Stays alive (`new_field[i][j] = 1`) if it has **2 or 3 live neighbors**.  
     - Dies (`new_field[i][j] = 0`) otherwise.  
   - **For a dead cell (`field[i][j] == 0`)**:
     - Becomes alive (`new_field[i][j] = 1`) if it has **exactly 3 live neighbors**.  
     - Stays dead (`new_field[i][j] = 0`) otherwise.  
4. Copies the `new_field` back to `field` to update the grid.  

**Key Detail**: Using a separate `new_field` array ensures that all cells are updated based on the current state, avoiding inconsistencies during the update process.  

---

### 5. `get_delay(int speed)`

**Purpose**: Converts a speed level (1–10) to a delay in milliseconds.  

**Input**: `speed`: The current speed level.  

**Process**: Uses a predefined array of delays (`[2000, 1500, 1000, 750, 500, 350, 250, 150, 100, 50]`) and returns the delay corresponding to `speed - 1`.  

**Output**: The delay in milliseconds.  

**Key Detail**: Higher speed levels correspond to shorter delays, making the simulation run faster.  

---

### 6. `check_input(int* speed)`

**Purpose**: Handles keyboard input to control the simulation.  

**Input**: `speed`: A pointer to the current speed level, allowing modification.  

**Process**:  
1. Uses `getch` to read a keypress without waiting for `Enter`.  
2. If `Space` is pressed, returns `0` to signal program termination.  
3. If `A` or `a` is pressed and `speed` is less than `10`, increments `speed`.  
4. If `Z` or `z` is pressed and `speed` is greater than `1`, decrements `speed`.  

**Output**: Returns `1` to continue running, `0` to exit.  

**Key Detail**: The use of a pointer allows direct modification of the `speed` variable, ensuring changes persist in the main loop.  

---

### 7. `main(int argc, char* argv[])`

**Purpose**: The entry point of the program, coordinating the simulation.  

**Input**:  
- `argc`: Number of command-line arguments.  
- `argv`: Array of argument strings (expects the program name and a file path).  

**Process**:  
1. Checks if exactly one file path is provided (`argc != 2` triggers an error).  
2. Loads the initial grid from the file using `load_file`. Exits with an error if the file cannot be loaded.  
3. Initializes ncurses with:  
   - `initscr`: Starts the ncurses environment.  
   - `cbreak`: Enables immediate keypress processing.  
   - `noecho`: Prevents keypresses from appearing on the screen.  
   - `nodelay(stdscr, TRUE)`: Makes `getch` non-blocking.  
   - `curs_set(0)`: Hides the cursor.  
4. Runs the main loop while `running` is `1`:  
   - Calls `draw` to display the grid.  
   - Checks for input with `check_input`. If it returns `0`, sets `running` to `0` to exit.  
   - Updates the grid with `update_field`.  
   - Implements the delay using `napms(10)` in a loop, checking for input every `10ms` to allow responsive speed changes or exit.  
5. Terminates ncurses with `endwin` and exits.  

**Key Detail**: The main loop balances responsiveness (checking input frequently) with controlled pacing (using delays based on speed).  

---

## Usage

### Compile the Program
Ensure you have the ncurses library installed (e.g., `libncurses-dev` on Debian/Ubuntu). Compile with:
```bash
gcc -o life game.c -lncurses