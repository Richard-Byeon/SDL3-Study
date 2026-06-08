constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

#include <SDL3/SDL.h>
#include <vector>

struct Entry
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;

};

int main(int argc, char* argv[]) // program entry point.
{
    // big question is, how do i access to each entry(piexls)..
    // and each entry in framebuffer looks like this: 
    // framebuffer[0] : [R][G][B][A]
    // framebuffer[1] : [R][G][B][A]
    // (x, y) -> x is col, y is row. and SDL uses row-major system. 
    std::vector<Entry> framebuffer(WIDTH * HEIGHT);
    int x = 0, y = 0;
    // the indexing. ( ROW MAJOR )
    // 
    // int idx = y * WIDTH + x;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            framebuffer[y * WIDTH + x] = { 255, 0, 0, 255 };
        }

    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Rasterizer", 800, 600, 0);

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888, // this format has to be same with the pixelformat that my framebuffer is using.
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    bool running = true;
    SDL_Event event; // what consists of SDL_Event? 
    // 그냥 어떤 이벤트가 있다고 생각해놓자.
    // event는 언제 업데이트하는거임?

    while (running)
    {
        // 이벤트의 주소를 전달해서 PollEvent에서 참인지 아닌지 판단한다. 그럼 그 판단하는 기준은 이벤트가 있을 시?
        while (SDL_PollEvent(&event))
        {
            // so, in the first iteration, update hasn't been done to the event. so the event must be contains true
            // and, event type isn't SDL_EVENT_QUIT. so this loop ended. 

            /*  ANSWERS TO ALL OF THE QUESTIONS

                1. PollEvent checks if there is event in the Queue. if there is, then it returns true.
                2. SDL_Event is union, and the event is stored in that structure.
                3. event is updated in every loop.
                4. Rough data flow is like this:
                       _______         _________           _________
                       | O.S |   <->   |  SDL  |     <->   |  Code |
                       ------_         ---------           ---------

                       OS creates events, and store those events in SDL queue (yes, THE QUEUE, FIFO.) and then the code checks if that queue is empty or not.
            */

            if (event.type == SDL_EVENT_QUIT) { // when user pressed the quit button, set 'running' as false. plain and simple, right? 
                running = false;
            }
        }
        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Entry));
        //SDL_Log("UpdateTexture error: %s", SDL_GetError());

        //SDL_RenderClear(renderer);
        //SDL_Log("RenderClear error: %s", SDL_GetError());

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        //SDL_Log("RenderTexture error: %s", SDL_GetError());

        //SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Entry));

        SDL_RenderClear(renderer); // what does this function do? the name contains clear. so it might clear the window in every iteration.
        //
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        //
        SDL_RenderPresent(renderer); // but, present? what is this? the present state of renderer? what? |||| NO! present to the user that is in the renderer. 
        //// in this case, it'll be just black window.

        // unless user closes the window, << this is when the event variable is updated. this loop iterates.
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}