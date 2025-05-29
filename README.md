# Para runnear el main en macos hay que hacer:
```
g++ -std=c++17 main.cpp -o out -I/opt/homebrew/Cellar/sfml/3.0.1/include/ -L/opt/homebrew/Cellar/sfml/3.0.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio 
```
# Para runnear el main en windows hay que:

```
g++ -std=c++17 main.cpp -o INFOBROS.exe -IC:\SFML\include -LC:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main
```

# En realidad lo que hay que hacer para runnear el codigo en Windows es:

Copy all the .dll files from this SFML 3.0.1 bin directory (e.g., sfml-graphics-3.dll, sfml-window-3.dll, sfml-system-3.dll, sfml-audio-3.dll, openal32.dll (or libopenal-1.dll), freetype.dll (or libfreetype-6.dll), etc.) into the same directory where your game.exe is located.

