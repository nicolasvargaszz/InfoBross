# 🎮 Super Mario Bros - SFML Clone

A platformer game inspired by Super Mario Bros, built with C++ and [SFML 3.x](https://www.sfml-dev.org/).  
Collect cafés ☕, avoid enemies 👾, and reach the exit 🚪 to win!

---

## ✨ Features

- 🗺️ **Multiple Levels:** Load maps from JSON files.
- 🏃‍♂️ **Animated Player & Enemies:** With walking, jumping, and idle animations.
- 👾 **Enemy AI:** Enemies patrol and react to collisions.
- ☕ **Collectibles:** Gather cafés to win.
- 🔊 **Sound Effects:**  
  - 💥 Enemy kill  
  - 💀 Player death  
  - ☕ Café collected  
  - 🏆 Victory
- 🕹️ **Game States:** Menu, Playing, Dialogue, Game Over, Win.
- 🌗 **Transitions:** Fade effects between scenes.
- 🎨 **Custom Assets:** Sprites, sounds, and fonts.

---

## 🎮 Controls

| Action         | Key                |
|----------------|--------------------|
| Move Left      | ← / A              |
| Move Right     | → / D              |
| Jump           | Space / W / ↑      |
| Start Game     | Any key (on menu)  |
| Exit           | Exit button        |

---

## 🛠️ Build Instructions

### Prerequisites

- C++17 or newer
- [SFML 3.x](https://www.sfml-dev.org/download.php)  
- CMake (recommended)

### Build (macOS/Linux)

```sh
git clone https://github.com/yourusername/Super-Mario-Bros-Main.git
cd Super-Mario-Bros-Main
mkdir build && cd build
cmake ..
make
./SuperMarioBros
```

### Build (Windows)

1. Install [SFML 3.x](https://www.sfml-dev.org/download.php) and set up your compiler.
2. Use CMake GUI or Visual Studio to configure and build the project.

---

## 📁 Project Structure

```
assets/         # Sprites, sounds, fonts, maps
include/        # Header files
src/            # Source files
library/        # External libraries (SFML, nlohmann/json)
```

---

## ➕ Adding Levels & Assets

- **Maps:** Place new `.json` maps in `assets/maps/` and add their paths to `main.cpp`.
- **Sprites/Sounds:** Add to `assets/sprites/` or `assets/sounds/` and reference in code.

---

## 🙏 Credits

- [SFML](https://www.sfml-dev.org/)
- [nlohmann/json](https://github.com/nlohmann/json)
- Sprites, sounds, and fonts: see `assets/` for attributions.
- Pixel arts: Diego Corvalan
- Music: Nicolás Vargas
- Assets: https://pixelfrog-assets.itch.io/kings-and-pigs (It's a free Assets this is in the official website: These assets are released under a Creative Commons Zero (CC0) license.
You can distribute, remix, adapt, and build upon the material in any medium or format, even for commercial purposes.

Attribution is not required.

If you want to have a closer look at the license, check here. Have fun! :))

- Maps: Cesar Oyola Using TiledMap And kings & pig assets.
- Code, physics & logic: Alfredo Vargas, Mathias Drews, Sebastian Benegas.
- Sounds: "Oof" from the 2002 game Messiah game.
- For any complain there's the comments session.
---


## 🤧 General Notes:
- If you lose the game, please just close the window and re run the game. 
- If you win the game, and you want to play the game again please also re-run the game, and enjoy.

## 📜 License

This project is for educational purposes. See [LICENSE](LICENSE) for details.

---

## 🖼️ Screenshots

*(Add screenshots of your game here!)*

---

Enjoy playing and hacking on **INFOBROS**! 🚀
