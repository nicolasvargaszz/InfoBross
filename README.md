# Para runnear el main en macos hay que hacer:
```
g++ -std=c++17 main.cpp -o out -I/opt/homebrew/Cellar/sfml/3.0.1/include/ -L/opt/homebrew/Cellar/sfml/3.0.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio 
```
# Para runnear el main en windows hay que:

```
g++ -std=c++17 main.cpp -o INFOBROS.exe -IC:\SFML\include -LC:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main
```
