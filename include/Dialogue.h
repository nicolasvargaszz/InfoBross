// Dialogue.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using namespace sf;

class Dialogue {
public:
    enum class DialogueState { TYPING, WAITING_FOR_NEXT, FINISHED };
    Dialogue(sf::Font& font,
             const std::vector<std::string>& lines,
             unsigned int charSize);
    void reset();
    void handleEvent(const Event& ev);
    void update(float dt);
    void draw(RenderWindow& window);
    bool isFinished() const;


private:
    std::vector<std::string> dialogueLines;
    int currentLineIndex;
    std::string stringToDisplayForTypewriter;
    unsigned int currentCharTypewriterIndex;
    Clock typewriterClock;
    float TYPEWRITER_CHAR_INTERVAL;
    DialogueState currentDialogueState;

    Font& font;
    Sprite* sprite;
    Text dialogueText;
    FloatRect dialogueTextBounds;
    RectangleShape dialogueBox;
};
