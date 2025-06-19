#include "../include/Dialogue.h"
#include <iostream>
// Constructor: initializes dialogue lines, text, and box
Dialogue::Dialogue(Font& fontRef,
                   const std::vector<std::string>& lines,
                   unsigned int charSize,
                   const sf::Texture& imagePath)  // Interval is fixed by TYPEWRITER_CHAR_INTERVAL
    : dialogueLines(lines)
    , currentLineIndex(0)
    , stringToDisplayForTypewriter("")
    , TYPEWRITER_CHAR_INTERVAL (0.05f)
    , currentCharTypewriterIndex(0)
    , currentDialogueState(DialogueState::TYPING)
    , font(fontRef)
    , dialogueText(fontRef, "", charSize)
    , staticSprite(imagePath)
    , staticTexture(imagePath)
{
    
    // Initialize typewriter clock
    typewriterClock.restart();

    // Configure text appearance
    dialogueText.setFillColor(Color::White);

    // Configure dialogue box (size will be set in draw)
    dialogueBox.setFillColor(Color(0, 0, 0, 150));

    // Manage the Size of Sprite
    staticSprite.setScale({-10.f, 10.f});
}

// Reset dialogue to start over
void Dialogue::reset() {
    currentLineIndex = 0;
    stringToDisplayForTypewriter.clear();
    currentCharTypewriterIndex = 0;
    currentDialogueState = DialogueState::TYPING;
    dialogueText.setString("");
    typewriterClock.restart();
}

void Dialogue::handleEvent(const Event& ev) {
    // Use SFML-3 event API to check for key presses
    if (ev.is<sf::Event::KeyPressed>()) {
        if (currentDialogueState == DialogueState::TYPING) {
            // Complete current line immediately
            stringToDisplayForTypewriter = dialogueLines[currentLineIndex];
            currentCharTypewriterIndex = static_cast<unsigned int>(stringToDisplayForTypewriter.length());
            dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
            currentDialogueState = DialogueState::WAITING_FOR_NEXT;
        } else if (currentDialogueState == DialogueState::WAITING_FOR_NEXT) {
            // Move to next line or finish
            ++currentLineIndex;
            if (currentLineIndex < static_cast<int>(dialogueLines.size())) {
                stringToDisplayForTypewriter.clear();
                currentCharTypewriterIndex = 0;
                dialogueText.setString("");
                currentDialogueState = DialogueState::TYPING;
                typewriterClock.restart();
            } else {
                currentDialogueState = DialogueState::FINISHED;
            }
        }
    }
}
// Update typewriter effect based on elapsed time
void Dialogue::update(float dt) {

    if (currentDialogueState != DialogueState::TYPING)
        return;

    // Use the clock to ensure consistent intervals
    if (typewriterClock.getElapsedTime().asSeconds() >= TYPEWRITER_CHAR_INTERVAL) {
        typewriterClock.restart();
        const std::string& line = dialogueLines[currentLineIndex];
        if (currentCharTypewriterIndex < line.size()) {
            // Append next character
            stringToDisplayForTypewriter += line[currentCharTypewriterIndex++];
            dialogueText.setString(stringToDisplayForTypewriter);
        } else {
            // Line complete: prompt user to press key
            dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
            currentDialogueState = DialogueState::WAITING_FOR_NEXT;
        }
    }
}

// Render the dialogue box and text
void Dialogue::draw(RenderWindow& window) {
    // Draw the sprite if loaded
    sf::FloatRect bounds = staticSprite.getGlobalBounds();
    staticSprite.setPosition({
        50.f + bounds.size.x,
        window.getSize().y - bounds.size.y - 50.f
    });
    if (staticTexture.getSize().x > 0 && staticTexture.getSize().y > 0) {
        window.draw(staticSprite);
    }

    // Compute box size and position
    dialogueBox.setSize({window.getSize().x * 0.8f, 150.f});
    dialogueBox.setOrigin({
        dialogueBox.getSize().x / 2.f,
        dialogueBox.getSize().y / 2.f
    });
    dialogueBox.setPosition({
        window.getSize().x / 2.f,
        window.getSize().y * 0.85f
    });

    window.draw(dialogueBox);

    // Center text within the box
    dialogueText.setString(stringToDisplayForTypewriter);
    dialogueTextBounds = dialogueText.getLocalBounds();
    dialogueText.setOrigin({
        dialogueTextBounds.size.x / 2.f,
        dialogueTextBounds.size.y / 2.f
    });
    dialogueText.setPosition({
        window.getSize().x / 2.f,
        window.getSize().y * 0.85f
    });
    window.draw(dialogueText);
}

// Check if dialogue has fully finished
bool Dialogue::isFinished() const {
    return currentDialogueState == DialogueState::FINISHED;
}
