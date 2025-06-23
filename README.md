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

---

## 📜 License

This project is for educational purposes. See [LICENSE](LICENSE) for details.

---

## 🖼️ Screenshots

*(Add screenshots of your game here!)*

---

Enjoy playing and hacking on **INFOBROS**! 🚀