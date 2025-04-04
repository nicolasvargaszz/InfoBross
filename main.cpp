/***********************************************************
 * Simple Mario-like Platformer (SDL2 Skeleton in C++)
 * 
 * NOTE: 
 * 1) Make sure you have SDL2 and SDL2_image installed.
 * 2) Compile with something like:
 *    g++ -o platformer main.cpp -lSDL2 -lSDL2_image
 ***********************************************************/

 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 #include <iostream>
 #include <vector>
 
 // Screen dimensions
 const int SCREEN_WIDTH  = 800;
 const int SCREEN_HEIGHT = 600;
 
 // Player constants
 const float PLAYER_SPEED       = 3.0f;   // Horizontal speed
 const float JUMP_VELOCITY      = 12.0f;  // Upward jump speed
 const float GRAVITY            = 0.5f;   // Downward acceleration
 const int   PLAYER_WIDTH       = 32;     // Player sprite width
 const int   PLAYER_HEIGHT      = 32;     // Player sprite height
 
 // A struct for platform data
 struct Platform {
     SDL_Rect rect; // x, y, w, h
 };
 
 // A struct to represent the player
 struct Player {
     float x, y;      // Position
     float vx, vy;    // Velocity
     bool onGround;
 
     SDL_Texture* texture; // The player sprite
     SDL_Rect srcRect;     // The portion of the sprite sheet to draw
     SDL_Rect dstRect;     // Destination on screen
 
     Player() : x(0), y(0), vx(0), vy(0), onGround(false), texture(nullptr) {
         srcRect = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
         dstRect = { (int)x, (int)y, PLAYER_WIDTH, PLAYER_HEIGHT };
     }
 };
 
 // Forward declarations
 bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
 bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
 SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer);
 
 int main(int argc, char* argv[])
 {
     SDL_Window* window = nullptr;
     SDL_Renderer* renderer = nullptr;
 
     if (!initSDL(window, renderer)) {
         std::cerr << "Failed to initialize SDL!\n";
         return 1;
     }
 
     // Load a player texture (replace with your own image)
     // For demonstration, we load a 32x32 placeholder sprite from a local file.
     // Make sure the file path is correct or replace with your own.
     SDL_Texture* playerTexture = loadTexture("mario.png", renderer);
     if (!playerTexture) {
         std::cerr << "Failed to load player texture.\n";
         return 1;
     }
 
     // Create player
     Player player;
     player.x = 100.0f;
     player.y = 100.0f;
     player.texture = playerTexture;
 
     // Sample platforms
     // (In a real game, you might load this from a file or tile map)
     std::vector<Platform> platforms;
     platforms.push_back({ SDL_Rect{0, 550, 800, 50}   }); // Floor
     platforms.push_back({ SDL_Rect{200, 450, 150, 25} });
     platforms.push_back({ SDL_Rect{400, 350, 150, 25} });
     platforms.push_back({ SDL_Rect{600, 250, 150, 25} });
 
     // Main loop flag
     bool quit = false;
     // Event handler
     SDL_Event e;
 
     // Frame timing
     Uint32 lastTime = SDL_GetTicks();
     
     while (!quit) {
         // Handle events
         while (SDL_PollEvent(&e) != 0) {
             if (e.type == SDL_QUIT) {
                 quit = true;
             } else if (e.type == SDL_KEYDOWN) {
                 if (e.key.keysym.sym == SDLK_ESCAPE) {
                     quit = true;
                 }
                 // Jump
                 if (e.key.keysym.sym == SDLK_SPACE && player.onGround) {
                     player.vy = -JUMP_VELOCITY;
                     player.onGround = false;
                 }
             }
         }
 
         // Get keyboard state for left/right movement
         const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
         player.vx = 0; // reset horizontal velocity each frame
         if (currentKeyStates[SDL_SCANCODE_LEFT]) {
             player.vx = -PLAYER_SPEED;
         } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
             player.vx = PLAYER_SPEED;
         }
 
         // Calculate delta time
         Uint32 currentTime = SDL_GetTicks();
         float deltaTime = (currentTime - lastTime) / 16.666f; // Approx ~60 FPS
         lastTime = currentTime;
 
         // APPLY GRAVITY
         player.vy += GRAVITY * deltaTime;
 
         // UPDATE PLAYER POSITION
         player.x += player.vx * deltaTime;
         player.y += player.vy * deltaTime;
 
         // COLLISION CHECK
         // We'll do a simplified bounding-box check for each platform
         player.onGround = false;
         SDL_Rect playerRect = {
             (int)player.x,
             (int)player.y,
             PLAYER_WIDTH,
             PLAYER_HEIGHT
         };
 
         for (auto& platform : platforms) {
             // If there's a collision
             if (checkCollision(playerRect, platform.rect)) {
                 // We collided from above (floor)
                 if (player.vy > 0) {
                     player.y = platform.rect.y - PLAYER_HEIGHT;
                     player.vy = 0;
                     player.onGround = true;
                 }
                 // Otherwise, you could add detection for hitting your head or from the side
             }
         }
 
         // KEEP PLAYER IN SCREEN (optional simple clamp)
         if (player.x < 0) player.x = 0;
         if (player.x > SCREEN_WIDTH - PLAYER_WIDTH) {
             player.x = SCREEN_WIDTH - PLAYER_WIDTH;
         }
         if (player.y > SCREEN_HEIGHT - PLAYER_HEIGHT) {
             // If you fall below screen, reset
             player.x = 100.0f;
             player.y = 100.0f;
             player.vy = 0;
         }
 
         // RENDER
         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
         SDL_RenderClear(renderer);
 
         // Draw platforms
         SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
         for (auto& platform : platforms) {
             SDL_RenderFillRect(renderer, &platform.rect);
         }
 
         // Draw player
         player.dstRect.x = (int)player.x;
         player.dstRect.y = (int)player.y;
         SDL_RenderCopy(renderer, player.texture, &player.srcRect, &player.dstRect);
 
         SDL_RenderPresent(renderer);
     }
 
     // Clean up
     SDL_DestroyTexture(playerTexture);
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     SDL_Quit();
 
     return 0;
 }
 
 // ----------------------------------------------------------
 // checkCollision: Basic AABB collision check
 // ----------------------------------------------------------
 bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
     if (a.x + a.w <= b.x) return false;
     if (a.x >= b.x + b.w) return false;
     if (a.y + a.h <= b.y) return false;
     if (a.y >= b.y + b.h) return false;
     return true;
 }
 
 // ----------------------------------------------------------
 // initSDL: Initialize SDL and create a window/renderer
 // ----------------------------------------------------------
 bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
         std::cerr << "SDL could not initialize! SDL Error: " 
                   << SDL_GetError() << std::endl;
         return false;
     }
 
     window = SDL_CreateWindow("Simple Mario-like Platformer",
                               SDL_WINDOWPOS_CENTERED, 
                               SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);
 
     if (!window) {
         std::cerr << "Window could not be created! SDL Error: " 
                   << SDL_GetError() << std::endl;
         return false;
     }
 
     renderer = SDL_CreateRenderer(window, -1, 
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     if (!renderer) {
         std::cerr << "Renderer could not be created! SDL Error: " 
                   << SDL_GetError() << std::endl;
         return false;
     }
 
     // Initialize SDL_Image for PNG loading (optional but recommended)
     if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
         std::cerr << "SDL_image could not initialize! SDL_image Error: "
                   << IMG_GetError() << std::endl;
         return false;
     }
 
     return true;
 }
 
 // ----------------------------------------------------------
 // loadTexture: Helper to load PNG textures using SDL_image
 // ----------------------------------------------------------
 SDL_Texture* loadTexture(const std::string &path, SDL_Renderer* renderer) {
     SDL_Texture* newTexture = nullptr;
     SDL_Surface* loadedSurface = IMG_Load(path.c_str());
     if (loadedSurface == nullptr) {
         std::cerr << "Unable to load image " << path 
                   << "! SDL_image Error: " << IMG_GetError() << std::endl;
         return nullptr;
     }
     // Create texture from surface
     newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
     SDL_FreeSurface(loadedSurface);
 
     if (newTexture == nullptr) {
         std::cerr << "Unable to create texture from " << path 
                   << "! SDL Error: " << SDL_GetError() << std::endl;
     }
     return newTexture;
 }
 