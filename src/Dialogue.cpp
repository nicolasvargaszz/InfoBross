#include "../include/Dialogue.h"
#include <memory>
#include <iostream>

// Constructor: ahora acepta un vector de rutas de sprites
Dialogue::Dialogue(Font& fontRef,
                   const std::vector<std::string>& lines,
                   unsigned int charSize,
                   const std::vector<std::string>& spritePaths)
    : dialogueLines(lines)
    , currentLineIndex(0)
    , stringToDisplayForTypewriter("")
    , TYPEWRITER_CHAR_INTERVAL(0.05f)
    , currentCharTypewriterIndex(0)
    , currentDialogueState(DialogueState::TYPING)
    , font(fontRef)
    , dialogueText(fontRef, "", charSize)
    , animTextures()
    , animSprites()
    , currentAnimIndex(0)
    , animTimer(0.f)
    , animFrameTime(0.5f)
{
    typewriterClock.restart();
    dialogueText.setFillColor(Color::White);
    dialogueBox.setFillColor(Color(0, 0, 0, 150));

    // Cargar sprites animados según el vector recibido
    for (const auto& path : spritePaths) {
    auto tex = std::make_shared<sf::Texture>();
    if (tex->loadFromFile(path)) {
        animTextures.push_back(tex);
        animSprites.emplace_back(*tex);
        animSprites.back().setScale({4.f, 4.f});
    } else {
        std::cerr << "No se pudo cargar el sprite: " << path << std::endl;
    }
    }
}

void Dialogue::reset() {
    currentLineIndex = 0;
    stringToDisplayForTypewriter.clear();
    currentCharTypewriterIndex = 0;
    currentDialogueState = DialogueState::TYPING;
    dialogueText.setString("");
    typewriterClock.restart();
    animTimer = 0.f;
    currentAnimIndex = 0;
}

void Dialogue::handleEvent(const Event& ev) {
    if (ev.is<sf::Event::KeyPressed>()) {
        if (currentDialogueState == DialogueState::TYPING) {
            stringToDisplayForTypewriter = dialogueLines[currentLineIndex];
            currentCharTypewriterIndex = static_cast<unsigned int>(stringToDisplayForTypewriter.length());
            dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
            currentDialogueState = DialogueState::WAITING_FOR_NEXT;
        } else if (currentDialogueState == DialogueState::WAITING_FOR_NEXT) {
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

void Dialogue::update(float dt) {
    if (currentDialogueState == DialogueState::TYPING) {
        if (typewriterClock.getElapsedTime().asSeconds() >= TYPEWRITER_CHAR_INTERVAL) {
            typewriterClock.restart();
            const std::string& line = dialogueLines[currentLineIndex];
            if (currentCharTypewriterIndex < line.size()) {
                stringToDisplayForTypewriter += line[currentCharTypewriterIndex++];
                dialogueText.setString(stringToDisplayForTypewriter);
            } else {
                dialogueText.setString(stringToDisplayForTypewriter + "\n(Press any key to continue)");
                currentDialogueState = DialogueState::WAITING_FOR_NEXT;
            }
        }
    }

    // Animación de sprites
    if (!animSprites.empty()) {
        animTimer += dt;
        if (animTimer >= animFrameTime) {
            animTimer = 0.f;
            currentAnimIndex = (currentAnimIndex + 1) % animSprites.size();
        }
    }
}

void Dialogue::draw(RenderWindow& window) {
    // Dibuja el sprite animado (si hay)
    if (!animSprites.empty()) {
        auto& sprite = animSprites[currentAnimIndex];
        sprite.setPosition({50.f, window.getSize().y - sprite.getGlobalBounds().size.y - 50.f});
        window.draw(sprite);
    }

    // Dibuja el cuadro de diálogo
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

    // Dibuja el texto centrado
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

bool Dialogue::isFinished() const {
    return currentDialogueState == DialogueState::FINISHED;
}