#include "PlayScene.hpp"

PlayScene::PlayScene(kit2d::Stage& stage)
    : Scene(stage), board(0, 0, stage.bounds().width, stage.bounds().height) {
}

void PlayScene::prepare() {
    board.load("assets/level.txt");
    board.prepare(assetManager);
    SDL_GetMouseState(&lastMouseX, nullptr);
}

void PlayScene::update(float deltaTime) {
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    int mouseX = 0;
    const Uint32 buttons = SDL_GetMouseState(&mouseX, nullptr);
    const bool space = keys[SDL_SCANCODE_SPACE];
    const bool click = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    const bool left = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
    const bool right = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];
    const bool pause = keys[SDL_SCANCODE_P];
    const bool restart = keys[SDL_SCANCODE_R];
    const bool quit = keys[SDL_SCANCODE_ESCAPE];
    Board::Input input;
    input.movement = static_cast<float>(right) - static_cast<float>(left);
    if (mouseX != lastMouseX)
        mouseControl = true;
    if (left || right)
        mouseControl = false;
    input.usePointer = mouseControl;
    input.pointerX = static_cast<float>(mouseX);
    input.launch = (space && !spaceHeld) || (click && !clickHeld);
    input.pause = pause && !pauseHeld;
    input.restart = restart && !restartHeld;
    board.setInput(input);
    board.act(deltaTime);
    if (quit && !quitHeld) {
        SDL_Event event {};
        event.type = SDL_QUIT;
        if (SDL_PushEvent(&event) < 0)
            throw kit2d::Err {};
    }
    lastMouseX = mouseX;
    spaceHeld = space;
    clickHeld = click;
    pauseHeld = pause;
    restartHeld = restart;
    quitHeld = quit;
}

void PlayScene::render(kit2d::SpriteBatch& spriteBatch) {
    board.draw(spriteBatch);
}
