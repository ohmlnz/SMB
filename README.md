# Super Mega Bruster

---

A retro-inspired 2D platformer built in C++ with SFML, blending the worlds of Super Mario and Megaman! Jump, run, and shoot your way through creative levels, nostalgic visuals, and classic platforming action.

## Features

- **Classic Platforming**: Tight controls, challenging jumps, and enemy encounters.
- **Hybrid Aesthetics**: Sprites and assets inspired by both Mario and Megaman franchises.
- **Multiple Levels**: Play through at least one complete level, with more to come.
- **Custom Assets**: Unique pixel art, animations, and sound effects.
- **Debug Mode**: Toggle debug info for development or curiosity.
- **Keyboard Controls**: Intuitive, responsive input for smooth gameplay.

## Getting Started

### Prerequisites
- C++ compiler (supporting C++11 or later)
- [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)

### Build Instructions
1. **Install SFML** on your system and ensure your compiler can find the SFML headers and libraries.
2. **Clone this repository**:
   ```sh
   git clone <repo-url>
   cd SMB
   ```
3. **Compile the source files** (example using g++):
   ```sh
   g++ -std=c++11 -I<path-to-sfml-include> -L<path-to-sfml-lib> *.cpp -lsfml-graphics -lsfml-window -lsfml-system -o SuperMegaBruster
   ```
   Adjust the include/library paths as needed for your setup.
4. **Run the game**:
   ```sh
   ./SuperMegaBruster
   ```

## Controls

- **Arrow Keys**: Navigate menu
- **Enter**: Select menu option
- **A**: Move left
- **D**: Move right
- **W**: Jump
- **Space**: Shoot
- **Escape**: Pause
- **P**: Toggle debug mode
