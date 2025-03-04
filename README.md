# Super Mega Bruster

Super Mega Bruster is a 2D platformer game built using C++ and the SFML library. The game combines elements from classic games like Super Mario and Megaman, providing an engaging and nostalgic experience.


## Key Components

- **GameEngine**: The main class that initializes the game, loads assets, and manages the game loop.
- **Scene**: An abstract class representing different scenes in the game (e.g., menu, level one).
- **SceneMenu**: A class representing the main menu scene.
- **SceneLevelOne**: A class representing the first level of the game.
- **Entity**: A class representing game entities with various components.
- **EntityManager**: A class that manages all game entities.
- **Components**: Various components that can be attached to entities (e.g., transform, animation, bounding box).
- **Assets**: A class that loads and manages game assets like textures, animations, sounds, and fonts.
- **Physics**: A class that handles collision detection and resolution.
- **Vec2**: A utility class for 2D vector operations.

## Dependencies

- [SFML](https://www.sfml-dev.org/): Simple and Fast Multimedia Library used for graphics, windowing, and input handling.

## Building the Project

To build the project, compile the source files with your favorite C++ compiler. Ensure that you have SFML installed and properly configured in your development environment.

## Running the Game

After building the project, run the executable. The game will start with the main menu, where you can select a level to play.

## Controls

- **Arrow Keys**: Navigate the menu
- **Enter**: Select menu option
- **A**: Move left
- **D**: Move right
- **W**: Jump
- **Space**: Shoot
- **Escape**: Pause
- **P**: Toggle debug mode

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.