#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include "rectangle.h"
#include <time.h>
//define window dimensions
constexpr int WINDOW_WIDTH{700};
constexpr int WINDOW_HEIGHT{800};

// Vertical scrolling grid related variables
constexpr float VERTICAL_SCROLLING_SPEED{ 100 };
constexpr int VERTICAL_SCROLLING_GRID_SQUARE_SIZE{ 20 };
std::vector<Rectangle> scrollingGrid;
// Animation related variables
float DT{ 0.01f };
Uint64 startTime{ 0 };
Uint64 endTime{ 0 };

//define SDL Window related variables
SDL_Window *window{nullptr};
SDL_Renderer *renderer{nullptr};
SDL_Event currentEvent;
SDL_Color backgroundColor{255, 255, 255, 255};

bool quit{false};

float mouseX{-1.0f}, mouseY{-1.0f};

float displayScale{1.0f};


void initScollingRectangles() {
    int numGridRectangles = (WINDOW_HEIGHT / VERTICAL_SCROLLING_GRID_SQUARE_SIZE) / 2 + 2;

    for (int i = 0; i < numGridRectangles; i++) {
        Rectangle newGridRectangleLeft, newGridRectangleRight;

        newGridRectangleLeft.width = newGridRectangleLeft.height = VERTICAL_SCROLLING_GRID_SQUARE_SIZE;
        newGridRectangleLeft.pos.x = newGridRectangleLeft.width / 2.0f;
        newGridRectangleLeft.pos.y = WINDOW_HEIGHT - newGridRectangleLeft.height / 2.0f - 2.0f * i * newGridRectangleLeft.height;
        newGridRectangleLeft.vel = glm::vec2(0.0f, VERTICAL_SCROLLING_SPEED);
        newGridRectangleLeft.color = SDL_Color{ 235, 235, 235, 255 };

        newGridRectangleRight.width = newGridRectangleRight.height = VERTICAL_SCROLLING_GRID_SQUARE_SIZE;
        newGridRectangleRight.pos.x = WINDOW_WIDTH - newGridRectangleRight.width / 2.0f;
        newGridRectangleRight.pos.y = WINDOW_HEIGHT - newGridRectangleRight.height / 2.0f - 2.0f * i * newGridRectangleRight.height;
        newGridRectangleRight.vel = glm::vec2(0.0f, VERTICAL_SCROLLING_SPEED);
        newGridRectangleRight.color = SDL_Color{ 235, 235, 235, 255 };

        scrollingGrid.push_back(newGridRectangleLeft);
        scrollingGrid.push_back(newGridRectangleRight);
    }
}
int player_x = WINDOW_WIDTH/2;
int player_y = WINDOW_HEIGHT-25;
int x1;
int x2;
int x3;
int ye1, ye2, ye3;
void setEnemyPositions()
{
    srand(time(NULL));
    x1 = rand() % WINDOW_WIDTH / 3 + 50;
    x2 = rand() % WINDOW_WIDTH / 3;
    x3 = rand() % WINDOW_WIDTH / 3 - 50;
    ye1 = rand() % WINDOW_HEIGHT / 3;
    ye2 = rand() % WINDOW_HEIGHT / 3;
    ye3 = rand() % WINDOW_HEIGHT / 3;
}
void initCharacters() {
    Rectangle player=Rectangle(50,50, glm::vec2(player_x,player_y),SDL_Color(0,255,0,0));
    DrawRectangle(renderer,player);
    FillRectangle(renderer,player);


    Rectangle enemy1=Rectangle(50,50, glm::vec2( x1,WINDOW_HEIGHT - WINDOW_HEIGHT/2 - ye1),SDL_Color(255,165,0,0));

    Rectangle enemy2=Rectangle(50,50, glm::vec2(WINDOW_WIDTH/3 + x2,WINDOW_HEIGHT - WINDOW_HEIGHT/2 - ye2),SDL_Color(255,165,0,0));
    Rectangle enemy3=Rectangle(50,50, glm::vec2(WINDOW_WIDTH/3 * 2 + x3,WINDOW_HEIGHT - WINDOW_HEIGHT/2 - ye3),SDL_Color(255,165,0,0));
    DrawRectangle(renderer,enemy1);
    DrawRectangle(renderer,enemy2);
    DrawRectangle(renderer,enemy3);
    FillRectangle(renderer,enemy1);
    FillRectangle(renderer,enemy2);
    FillRectangle(renderer,enemy3);


}
bool initWindow() {
    bool success{true};

    //Try to initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL initialization failed: %s\n", SDL_GetError());
        success = false;
    } else {
        //Try to create the window and renderer
        displayScale = SDL_GetDisplayContentScale(1);

        if (!SDL_CreateWindowAndRenderer(
            "SDL Hello World Example",
            static_cast<int>(displayScale * WINDOW_WIDTH),
            static_cast<int>(displayScale * WINDOW_HEIGHT),
            0,
            &window, &renderer)) {
            SDL_Log("Failed to create window and renderer: %s\n", SDL_GetError());
            success = false;
        } else {
            //Apply global display scaling to renderer
            SDL_SetRenderScale(renderer, displayScale, displayScale);

            //Set background color
            SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b,
                                   backgroundColor.a);

            //Apply background color
            SDL_RenderClear(renderer);

        }
    }

    return success;
}

void processEvents() {
    //Check for events in queue
    while (SDL_PollEvent(&currentEvent)) {
        //User requests quit
        if (currentEvent.type == SDL_EVENT_QUIT) {
            quit = true;
        }

        //Mouse event -> pressed button
        if (currentEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (currentEvent.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX /= displayScale;
                mouseY /= displayScale;

                std::cout << "Left mouse click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
            }
        }
       // const SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(nullptr, nullptr); mouseButtons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)
        //Mouse event -> mouse movement
        if (currentEvent.type == SDL_EVENT_MOUSE_MOTION) {
            if (SDL_SetWindowRelativeMouseMode(window, true)) {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX /= displayScale;
                mouseY /= displayScale;
                player_x = mouseX;
                player_y = mouseY;
                std::cout << "Mouse move while left clicking => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
            }
        }

        //Keyboard event
        if (currentEvent.type == SDL_EVENT_KEY_DOWN) {
            switch (currentEvent.key.key) {
                case SDLK_UP:
                    std::cout << "UP arrow key was pressed" << std::endl;
                    break;
                case SDLK_DOWN:
                    std::cout << "DOWN arrow key was pressed" << std::endl;
                    break;
                case SDLK_LEFT:
                    std::cout << "LEFT arrow key was pressed" << std::endl;
                    break;
                case SDLK_RIGHT:
                    std::cout << "RIGHT arrow key was pressed" << std::endl;
                    break;
                case SDLK_R:
                    std::cout << "R key was pressed" << std::endl;
                    break;
                case SDLK_S:
                    std::cout << "S key was pressed" << std::endl;
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }
}

void animate() {

    // Scrolling grid
    for (int i = 0; i < scrollingGrid.size(); i++) {
        Rectangle& gridRectangle = scrollingGrid.at(i);

        gridRectangle.pos += gridRectangle.vel * DT;

        if (gridRectangle.pos.y > (WINDOW_HEIGHT + gridRectangle.height / 2.0f)) {
            gridRectangle.pos.y -= gridRectangle.height * scrollingGrid.size();
        }
    }
}

void drawFrame() {
    //Clear the background
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    //Draw scene
    initCharacters();
    //Draw scrolling grid

    for (int i = 0; i < scrollingGrid.size(); i++) {
        Rectangle& gridRectangle = scrollingGrid.at(i);
        gridRectangle.draw(renderer);
    }

    //Update window
    SDL_RenderPresent(renderer);
}

void cleanup() {
    //Destroy renderer
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    //Destroy window
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    //Quit SDL
    SDL_Quit();
}

int main() {
    //Initialize window
    setEnemyPositions();
    if (!initWindow()) {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    SDL_zero(currentEvent);

	initScollingRectangles();

    //Game loop
    while (!quit) {
        startTime = SDL_GetTicks();

        processEvents();
        animate();
        drawFrame();

        endTime = SDL_GetTicks();
        DT = (endTime - startTime) / 1000.0f;
    }

    cleanup();
    return 0;
}
