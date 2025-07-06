# 🎮 Conway's Game of Life in C

A terminal-based implementation of Conway's Game of Life using C and the ncurses library. This cellular automaton simulates the evolution of cells on a grid with interactive speed controls and file-based pattern loading.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Game Rules](#game-rules)
- [Controls](#controls)
- [File Format](#file-format)
- [Code Structure](#code-structure)
- [Examples](#examples)
- [Contributing](#contributing)

## 🌟 Overview

The Game of Life is a zero-player game developed by mathematician John Conway. It's a cellular automaton where each cell on a grid can be either alive or dead, and the population evolves over time based on simple rules about neighboring cells.

This implementation features:
- **25×80 grid** with toroidal topology (wrapping edges)
- **Terminal-based visualization** using ncurses
- **Interactive speed control** with 10 different levels
- **File-based pattern loading** for custom starting configurations

## ✨ Features

- 🔄 **Real-time simulation** with smooth terminal graphics
- 📁 **File input support** for loading custom patterns
- ⚡ **Speed control** with 10 levels (50ms to 2000ms delay)
- 🎮 **Interactive controls** for speed adjustment and exit
- 🔄 **Toroidal grid** - edges wrap around seamlessly
- 💻 **Cross-platform** terminal compatibility

## 🛠️ Installation

### Prerequisites

Make sure you have the ncurses library installed:

**Ubuntu/Debian:**
```bash
sudo apt-get install libncurses-dev
```

**CentOS/RHEL:**
```bash
sudo yum install ncurses-devel
```

**macOS:**
```bash
brew install ncurses
```

### Compilation

```bash
gcc -o life game.c -lncurses
```

## 🚀 Usage

```bash
./life pattern.txt
```

Where `pattern.txt` is a text file containing your initial pattern.

## 🎯 Game Rules

The Game of Life follows these simple rules:

1. **Survival**: A live cell with 2 or 3 live neighbors stays alive
2. **Death by underpopulation**: A live cell with fewer than 2 live neighbors dies
3. **Death by overpopulation**: A live cell with more than 3 live neighbors dies
4. **Birth**: A dead cell with exactly 3 live neighbors becomes alive

## 🎮 Controls

| Key | Action |
|-----|--------|
| `A` or `a` | **Increase speed** (up to level 10) |
| `Z` or `z` | **Decrease speed** (down to level 1) |
| `Space` | **Exit program** |

### Speed Levels

| Level | Delay |
|-------|-------|
| 1 | 2000ms (slowest) |
| 2 | 1500ms |
| 3 | 1000ms |
| 4 | 750ms |
| 5 | 500ms |
| 6 | 350ms |
| 7 | 250ms |
| 8 | 150ms |
| 9 | 100ms |
| 10 | 50ms (fastest) |

## 📄 File Format

Create pattern files using simple text format:
- `*` represents a **live cell**
- Any other character (except newline) represents a **dead cell**
- Maximum 25 rows × 80 columns

### Example Pattern File
```
**********
*        *
*   **   *
*   **   *
*        *
**********
```

## 🏗️ Code Structure

The program is organized into modular functions:

### Core Functions

| Function | Purpose |
|----------|---------|
| `load_file()` | Reads pattern from file into grid |
| `draw()` | Renders grid to terminal using ncurses |
| `count_neighbors()` | Counts live neighbors for a cell |
| `update_field()` | Applies Game of Life rules to generate next state |
| `get_delay()` | Converts speed level to milliseconds |
| `check_input()` | Handles keyboard input for controls |
| `main()` | Coordinates the simulation loop |

### Key Implementation Details

#### 🔄 Toroidal Grid
The grid wraps around at edges using modulo arithmetic:
```c
(r + i + 25) % 25  // Row wrapping
(c + j + 80) % 80  // Column wrapping
```

#### 🎨 Double Buffering
Uses separate arrays for current and next states to ensure consistent updates:
```c
update_field(field, new_field);  // Calculate next state
// Copy new_field back to field
```

#### ⚡ Non-blocking Input
Responsive controls using ncurses configuration:
```c
nodelay(stdscr, TRUE);  // Non-blocking getch()
cbreak();               // Immediate input processing
noecho();               // Hide keypresses
```

## 🌈 Examples

### Famous Patterns

Try these classic Game of Life patterns:

**Glider:**
```
 * 
  *
***
```

**Blinker:**
```
***
```

**Block:**
```
**
**
```

**Toad:**
```
 ***
***
```

## 🤝 Contributing

Contributions are welcome! Here are some ideas for improvements:

- [ ] Add more pattern examples
- [ ] Implement pattern saving functionality
- [ ] Add color support for different cell ages
- [ ] Create a pattern editor mode
- [ ] Add statistics display (generation count, population)
- [ ] Support for larger grid sizes
- [ ] Add pause/resume functionality

### Development Setup

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Make your changes and test thoroughly
4. Commit your changes: `git commit -am 'Add feature-name'`
5. Push to the branch: `git push origin feature-name`
6. Create a Pull Request

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 🙏 Acknowledgments

- John Conway for creating the Game of Life
- The ncurses library developers
- Classic pattern contributors from the Conway's Game of Life community

---

**Enjoy exploring the fascinating world of cellular automata! 🎉**