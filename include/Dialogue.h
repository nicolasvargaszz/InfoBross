#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
using namespace sf;

class Dialogue {
public:
    Dialogue(Font& font,
             const std::vector<std::string>& lines,
             unsigned int charSize,
             const std::vector<std::string>& spritePaths);

    void reset();
    void handleEvent(const Event& ev);
    void update(float dt);
    void draw(RenderWindow& window);
    bool isFinished() const;

private:
    enum class DialogueState { TYPING, WAITING_FOR_NEXT, FINISHED };
    std::vector<std::string> dialogueLines;
    int currentLineIndex;
    std::string stringToDisplayForTypewriter;
    unsigned int currentCharTypewriterIndex;
    Clock typewriterClock;
    float TYPEWRITER_CHAR_INTERVAL;
    DialogueState currentDialogueState;

    size_t currentAnimIndex = 0;
    float animTimer = 0.f;
    float animFrameTime = 0.5f;

    Font& font;
    Text dialogueText;
    FloatRect dialogueTextBounds;
    RectangleShape dialogueBox;

    std::vector<std::shared_ptr<sf::Texture>> animTextures;
    std::vector<sf::Sprite> animSprites;
};