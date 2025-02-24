# 2048 AI Game

## Overview
This is a **2048 game** implemented in C with an **AI mode**. Players can choose to play manually or enable the AI to make moves automatically. The game is played on a **4x4 board**, where numbers slide and merge to reach 2048.

## Features
- **Play Mode**: Users can play manually by pressing `W`, `A`, `S`, or `D` to move tiles.
- **AI Mode**: Set `_2048AI = 1` to let the AI play automatically.
- **Console-Based**: Uses a simple console-based interface to display the board.
- **Board Evaluation**: The AI evaluates moves based on heuristics such as smoothness, monotonicity, empty cells, and max value.

## How to Compile and Run
### Prerequisites
- A **C compiler** (e.g., `gcc` for Linux/Mac, `MinGW` for Windows)

### Compilation (Linux/Mac)
```sh
gcc -o 2048 game.c
```

### Compilation (Windows with MinGW)
```sh
gcc -o 2048.exe game.c
```

### Running the Game
```sh
./2048
```
On Windows:
```sh
2048.exe
```

## Game Controls
| Key | Action |
|-----|--------|
| `W` | Move Up |
| `A` | Move Left |
| `S` | Move Down |
| `D` | Move Right |

## AI Mode
To enable AI mode, change the following line in the code:
```c
int _2048AI = 1;
```
If `_2048AI` is set to `0`, the game runs in manual mode.

## Code Structure
- **Board Operations**: Functions to initialize, update, and print the board.
- **AI Logic**: Implements move evaluation using heuristics such as smoothness, monotonicity, empty cells, and max value.
- **Game Loop**: Continually updates the board until the game is over.

## Future Improvements
- Add a graphical user interface (GUI) for better visualization.
- Optimize the AI algorithm for better decision-making.

## License
This project is for educational purposes and free to use.

