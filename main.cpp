#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp> // Include for music
#include <optional>      // Required for std::optional
#include <vector>
#include <string> // For std::string
#include <iostream> // For std::cerr (used for CAFE texture loading error)

using namespace sf; // use this to avoid sf:: prefix

int main()
{
    // 1) Correctly construct the window with SFML 3.0.1
    VideoMode mode({1600, 900}); 
    RenderWindow window(mode, "INFOBROS"); //this create the screen
    window.setFramerateLimit(60);

    // 2) Use openFromFile(...) for Font in SFML 3.0.1
    Font font;
    if (!font.openFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/GAME_glm.ttf")) // this is the font, that is the path in my computer
        return -1; // if the font is not found, return -1, end of the program.

    // --- Music Setup ---
    Music backgroundMusic;
    if (!backgroundMusic.openFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/musica_OOP.mp3"))  //this is a music that i have created.
    {
        return -1; // Handle error: music file not found or couldn't be opened
    }
    backgroundMusic.setLooping(true); // This is how to loop the music
    backgroundMusic.setVolume(05.f); // Set the volume (0-100)
    backgroundMusic.play();        // Start playing the music

    // --- Dialogue Scene Assets ---
    Texture staticMathiTexture;
    if (!staticMathiTexture.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/static_mathi.png")) //load the static mathi for the dialogue
    {
        return -1; // Handle error
    }
    Sprite staticMathiSprite(staticMathiTexture); // This is the sprite for the static mathi
    // Reflect the sprite by using a negative x-scale.
    staticMathiSprite.setScale({-0.5f, 0.5f}); // Reflected horizontally, because the mathi is looking to the left 
    FloatRect bounds = staticMathiSprite.getGlobalBounds();
    // Position it.
    staticMathiSprite.setPosition({100.f + bounds.size.x, window.getSize().y - bounds.size.y - 100.f}); // here we set the position of Mathi

    std::vector<std::string> dialogueLines = { // This is the dialogue that will be shown in the begining of the game
        "Oh, I fell asleep!!!!! ",
        "I need to pass Data structures and OOP...",
        "I will study i must past this course..."};
    int currentLineIndex = 0;
    std::string stringToDisplayForTypewriter = "";
    unsigned int currentCharTypewriterIndex = 0;
    Clock typewriterClock;
    const float TYPEWRITER_CHAR_INTERVAL = 0.05f; // Seconds between characters, this will create the typewriter effect

    enum class DialogueState
    {
        TYPING,
        WAITING_FOR_NEXT,
        FINISHED
    };
    DialogueState currentDialogueState = DialogueState::TYPING;

    Text dialogueText(font, "", 40U); // Start with empty text
    dialogueText.setFillColor(Color::White); //color of the text
    FloatRect dialogueTextBounds = dialogueText.getLocalBounds();
    dialogueText.setOrigin({dialogueTextBounds.size.x / 2.f, dialogueTextBounds.size.y / 2.f});
    dialogueText.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f}); // Adjusted y position slightly from 0.9 to 0.85

    RectangleShape dialogueBox;
    dialogueBox.setSize({window.getSize().x * 0.8f, 150.f});
    dialogueBox.setFillColor({0, 0, 0, 150}); // Using {} for Color
    dialogueBox.setOrigin({dialogueBox.getSize().x / 2.f, dialogueBox.getSize().y / 2.f});
    dialogueBox.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f});

    // --- Dialogue Scene Loop ---
    bool showDialogueScene = true;
    if (dialogueLines.empty())
    {
        showDialogueScene = false;
    }
    else
    {
        typewriterClock.restart();
    }

    while (showDialogueScene && window.isOpen())
    {
        std::optional<Event> evOpt = window.pollEvent();
        while (evOpt.has_value())
        {
            const Event &ev = evOpt.value();
            if (ev.is<Event::Closed>())
            {
                window.close();
                showDialogueScene = false;
            }
            else if (ev.is<Event::KeyPressed>())
            {
                if (currentDialogueState == DialogueState::TYPING)
                {
                    stringToDisplayForTypewriter = dialogueLines[currentLineIndex];
                    currentCharTypewriterIndex = stringToDisplayForTypewriter.length();
                    dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
                    currentDialogueState = DialogueState::WAITING_FOR_NEXT;
                }
                else if (currentDialogueState == DialogueState::WAITING_FOR_NEXT)
                {
                    currentLineIndex++;
                    if (currentLineIndex < dialogueLines.size())
                    {
                        stringToDisplayForTypewriter = "";
                        currentCharTypewriterIndex = 0;
                        dialogueText.setString("");
                        currentDialogueState = DialogueState::TYPING;
                        typewriterClock.restart();
                    }
                    else
                    {
                        showDialogueScene = false;
                    }
                }
            }
            if (showDialogueScene)
                evOpt = window.pollEvent();
            else
                break;
        }

        if (currentDialogueState == DialogueState::TYPING && currentLineIndex < dialogueLines.size())
        {
            if (typewriterClock.getElapsedTime().asSeconds() > TYPEWRITER_CHAR_INTERVAL)
            {
                if (currentCharTypewriterIndex < dialogueLines[currentLineIndex].length())
                {
                    stringToDisplayForTypewriter += dialogueLines[currentLineIndex][currentCharTypewriterIndex];
                    dialogueText.setString(stringToDisplayForTypewriter);
                    currentCharTypewriterIndex++;
                    typewriterClock.restart();
                }
                else
                {
                    dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
                    currentDialogueState = DialogueState::WAITING_FOR_NEXT;
                }
            }
        }

        dialogueTextBounds = dialogueText.getLocalBounds();
        dialogueText.setOrigin({dialogueTextBounds.size.x / 2.f, dialogueTextBounds.size.y / 2.f});
        dialogueText.setPosition({window.getSize().x / 2.f, window.getSize().y * 0.85f});

        window.clear({30, 30, 80}); // Using {} for Color
        window.draw(staticMathiSprite);
        window.draw(dialogueBox);
        window.draw(dialogueText);
        window.display();
    }

    // --- Original Title Screen Assets ---
    Text title_text(font, "INFOBROS", 128U); 
    {
        FloatRect tb = title_text.getLocalBounds();
        title_text.setOrigin({tb.size.x * 0.5f, tb.size.y * 0.5f});
        title_text.setPosition({window.getSize().x * 0.5f, window.getSize().y * 0.5f});
        title_text.setFillColor(Color::Yellow);
    }

    Text prompt(font, "Press any key to start", 32U);
    {
        FloatRect pb_prompt_bounds = prompt.getLocalBounds();
        prompt.setOrigin({pb_prompt_bounds.size.x * 0.5f, pb_prompt_bounds.size.y * 0.5f});
        prompt.setPosition({window.getSize().x * 0.5f, window.getSize().y * 0.7f});
        prompt.setFillColor(Color::White);
    }

    // Intro loop (Title Screen)
    bool showIntro = true;
    if (!window.isOpen())
        showIntro = false;

    Clock introClock;
    while (showIntro && window.isOpen())
    {
        std::optional<Event> evOpt = window.pollEvent();
        while (evOpt.has_value())
        {
            const Event &ev = evOpt.value();
            if (ev.is<Event::Closed>())
            {
                window.close();
                showIntro = false;
            }
            else if (ev.is<Event::KeyPressed>())
            {
                showIntro = false;
            }
            if (showIntro)
                evOpt = window.pollEvent();
            else
                break;
        }

        if (introClock.getElapsedTime().asSeconds() > 5.f && showIntro) 
        {
            showIntro = false;
        }

        if (!showIntro)
            break;

        window.clear({10, 10, 70}); 
        window.draw(title_text);
        window.draw(prompt);
        window.display();
    }

    // --- Gameplay Assets ---
    Texture idleTexture, moveTexture1, moveTexture2;
    if (!idleTexture.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi.png") ||
        !moveTexture1.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento.png") ||
        !moveTexture2.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/mathi_movimiento_complemento.png"))
    {
        return -1;
    }

    Sprite player(idleTexture);
    {
        FloatRect b_player = player.getLocalBounds();
        player.setOrigin({b_player.size.x * 0.5f, b_player.size.y * 0.5f});
    }
    player.setScale({0.1f, 0.1f});
    player.setPosition({800.f, 450.f}); 

    // --- CAFE Assets ---
    Texture cafeTexture;
    if (!cafeTexture.loadFromFile("/Users/nicolasvargas/Desktop/Super-Mario-Bros-Main/Source/FotosMC/CAFE.jpeg")) 
    {
        std::cerr << "Failed to load CAFE.jpeg" << std::endl; 
        return -1; 
    }
    std::vector<Sprite> cafeItems;
    int cafeScore = 0;
    const int CAFE_TO_WIN = 10;
    bool gameWon = false;

    Text cafeScoreText(font, "CAFE: 0", 30U);
    cafeScoreText.setFillColor(Color::Black);

    Text winMessageText(font, "Congratulations! You passed the course!", 60U);
    winMessageText.setFillColor(Color::Green);
    FloatRect winTextBounds = winMessageText.getLocalBounds();
    winMessageText.setOrigin({winTextBounds.size.x / 2.f, winTextBounds.size.y / 2.f});

    View view = window.getDefaultView(); 

    std::vector<RectangleShape> platforms;
    auto makePlat = [&](float x, float y, float w, float h)
    {
        RectangleShape r({w, h});
        r.setPosition({x, y});
        r.setFillColor({100, 100, 200}); 
        platforms.push_back(r);
    };

    auto makeCafe = [&](float x, float y) 
    {
        Sprite cafeSprite(cafeTexture);
        cafeSprite.setScale({1.0f, 1.0f}); 
        cafeSprite.setPosition({x, y});
        cafeItems.push_back(cafeSprite);
    };

    // Create platforms (your existing platforms)
    for (int i = 0; i < 100; ++i)
        makePlat(i * 400.f, 880.f, 400.f, 20.f);
    makePlat(500.f, 780.f, 150.f, 20.f);
    makePlat(750.f, 700.f, 200.f, 20.f);
    makePlat(1000.f, 620.f, 180.f, 20.f);
    makePlat(850.f, 540.f, 100.f, 20.f);
    makePlat(1300.f, 650.f, 250.f, 20.f);
    makePlat(1650.f, 580.f, 150.f, 20.f);
    makePlat(1900.f, 750.f, 100.f, 20.f);
    makePlat(2100.f, 550.f, 200.f, 20.f);
    makePlat(2400.f, 450.f, 80.f, 20.f);
    makePlat(2550.f, 400.f, 80.f, 20.f);
    makePlat(2700.f, 350.f, 80.f, 20.f);
    makePlat(3000.f, 550.f, 300.f, 20.f);
    makePlat(3400.f, 650.f, 200.f, 20.f);
    makePlat(3700.f, 750.f, 150.f, 20.f);
    makePlat(4000.f, 600.f, 100.f, 20.f);
    makePlat(4200.f, 500.f, 70.f, 20.f);
    makePlat(4400.f, 400.f, 100.f, 20.f);
    makePlat(4250.f, 350.f, 50.f, 20.f);
    makePlat(4700.f, 500.f, 250.f, 20.f);
    makePlat(5050.f, 450.f, 200.f, 20.f);
    makePlat(5350.f, 400.f, 150.f, 20.f);
    makePlat(5600.f, 390.f, 300.f, 20.f);
    makePlat(6000.f, 470.f, 150.f, 20.f);
    makePlat(6250.f, 550.f, 150.f, 20.f);
    makePlat(6500.f, 650.f, 150.f, 20.f);
    makePlat(6750.f, 750.f, 150.f, 20.f);
    makePlat(7100.f, 700.f, 200.f, 20.f);
    makePlat(7400.f, 600.f, 180.f, 20.f);
    makePlat(7700.f, 550.f, 150.f, 20.f);
    makePlat(8000.f, 650.f, 250.f, 20.f);
    makePlat(8300.f, 550.f, 100.f, 20.f);
    makePlat(8500.f, 480.f, 80.f, 20.f);
    makePlat(8700.f, 450.f, 60.f, 20.f);
    makePlat(9000.f, 500.f, 300.f, 20.f);
    makePlat(9400.f, 450.f, 250.f, 20.f);
    makePlat(9800.f, 400.f, 200.f, 20.f);
    makePlat(10200.f, 350.f, 150.f, 20.f);
    makePlat(10500.f, 300.f, 100.f, 20.f);
    makePlat(10800.f, 250.f, 50.f, 20.f);
    makePlat(11000.f, 200.f, 20.f, 20.f);
    makePlat(11200.f, 150.f, 10.f, 20.f);
    makePlat(11400.f, 100.f, 5.f, 20.f);
    makePlat(11600.f, 50.f, 2.f, 20.f);
    makePlat(11800.f, 0.f, 1.f, 20.f);
    makePlat(12000.f, -50.f, 0.f, 20.f);
    makePlat(12200.f, -100.f, 0.f, 20.f);

    // Create CAFE items (at least 10, place them strategically)
    makeCafe(600.f, 750.f);
    makeCafe(900.f, 670.f);
    makeCafe(1200.f, 590.f);
    makeCafe(1500.f, 750.f);
    makeCafe(1800.f, 650.f);
    makeCafe(2200.f, 500.f);
    makeCafe(2600.f, 350.f);
    makeCafe(3000.f, 500.f);
    makeCafe(3500.f, 600.f);
    makeCafe(4000.f, 550.f);
    makeCafe(4500.f, 300.f); 


    const float GRAVITY = 1100.f;
    const float MOVE_SPD = 300.f;
    const float JUMP_SPD = 800.f; 

    Vector2f velocity{0.f, 0.f};
    bool onGround = false;

    bool toggleMoveFrame = false;
    float toggleInterval = 0.15f;
    Clock toggleClock;

    Clock dtClock;

    if (!window.isOpen())
        return 0;

    // Main Game Loop
    while (window.isOpen())
    {
        float dt = dtClock.restart().asSeconds();
        if (gameWon) { 
            dt = 0; 
        }

        std::optional<Event> evOpt_main = window.pollEvent();
        while (evOpt_main.has_value())
        {
            const Event &ev = evOpt_main.value();
            if (ev.is<Event::Closed>())
                window.close();
            
            if (gameWon && ev.is<Event::KeyPressed>()) 
            {
                if (ev.getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape)
                    window.close();
            }
            
            evOpt_main = window.pollEvent(); 
        }

        if (!gameWon) 
        {
            bool isMoving = false;
            velocity.x = 0.f;

            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                velocity.x = -MOVE_SPD;
                player.setScale({0.1f, 0.1f}); 
                isMoving = true;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                velocity.x = MOVE_SPD;
                player.setScale({-0.1f, 0.1f}); 
                isMoving = true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Up) && onGround)
            {
                velocity.y = -JUMP_SPD;
                onGround = false;
            }

            if (!isMoving)
            {
                toggleClock.restart(); 
                player.setTexture(idleTexture);
            }
            else
            {
                if (toggleClock.getElapsedTime().asSeconds() >= toggleInterval)
                {
                    toggleMoveFrame = !toggleMoveFrame;
                    toggleClock.restart();
                }
                player.setTexture(toggleMoveFrame ? moveTexture1 : moveTexture2);
            }

            velocity.y += GRAVITY * dt;

            // Horizontal movement and collision
            player.move({velocity.x * dt, 0.f});
            {
                FloatRect pb = player.getGlobalBounds();
                for (auto &plat : platforms)
                {
                    FloatRect pr = plat.getGlobalBounds();
                    if (pb.findIntersection(pr).has_value()) // Corrected: Use findIntersection
                    {
                        if (velocity.x > 0.f) 
                            player.setPosition({pr.position.x - pb.size.x * 0.5f, player.getPosition().y});
                        else if (velocity.x < 0.f) 
                            player.setPosition({pr.position.x + pr.size.x + pb.size.x * 0.5f, player.getPosition().y});
                        velocity.x = 0.f;
                        pb = player.getGlobalBounds(); 
                    }
                }
            }

            // Vertical movement and collision
            player.move({0.f, velocity.y * dt});
            onGround = false; 
            {
                FloatRect pb = player.getGlobalBounds();
                for (auto &plat : platforms)
                {
                    FloatRect pr = plat.getGlobalBounds();
                    if (pb.findIntersection(pr).has_value()) // Corrected: Use findIntersection
                    {
                        if (velocity.y > 0.f) 
                        {
                            player.setPosition({player.getPosition().x, pr.position.y - pb.size.y * 0.5f});
                            onGround = true;
                        }
                        else if (velocity.y < 0.f) 
                        {
                            player.setPosition({player.getPosition().x, pr.position.y + pr.size.y + pb.size.y * 0.5f});
                        }
                        velocity.y = 0.f;
                    }
                }
            }

            // CAFE Collection
            for (size_t i = 0; i < cafeItems.size(); ++i)
            {
                FloatRect playerBounds = player.getGlobalBounds();
                FloatRect cafeBounds = cafeItems[i].getGlobalBounds();
                if (playerBounds.findIntersection(cafeBounds).has_value()) // Corrected: Use findIntersection
                {
                    cafeItems.erase(cafeItems.begin() + i); 
                    cafeScore++;
                    cafeScoreText.setString("CAFE: " + std::to_string(cafeScore));
                    --i; 
                }
            }

            // Check for win condition
            if (cafeScore >= CAFE_TO_WIN)
            {
                gameWon = true;
            }
        } 

        // Update view to follow player
        view.setCenter(player.getPosition());
        window.setView(view);

        // Position UI elements relative to the view
        Vector2f viewTopLeft = view.getCenter() - view.getSize() / 2.f;
        cafeScoreText.setPosition(viewTopLeft + Vector2f({10.f, 10.f})); 
        winMessageText.setPosition(view.getCenter()); 


        // --- Drawing ---
        window.clear({180, 220, 255}); 

        for (auto &plat : platforms) 
            window.draw(plat);
        
        if (!gameWon) 
        {
            for (auto &cafe : cafeItems) 
                window.draw(cafe);
            window.draw(player); 
        }
        
        window.draw(cafeScoreText); 

        if (gameWon) 
        {
            window.draw(winMessageText);
        }
        
        window.display();
    }

    return 0;
}