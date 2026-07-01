constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

#define WHITE       255
#define BLACK       255

#include <SDL3/SDL.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>

// Initialize every pixel as white.
struct Entry
{
    uint8_t R = 255;
    uint8_t G = 255;
    uint8_t B = 255;
    uint8_t A = 255;

};

struct Vec3
{
    int x, y;
    float zValue;

};
struct Z
{
    float z = 1.0f;
};

// void Bresenhem(x0, y0, x1, y1, dest)
void Bresenhem(int x0, int y0, int x1, int y1, std::vector<Entry>& dest)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int sx = (dx > 0) ? 1 : -1;
    int sy = (dy > 0) ? 1 : -1;

    /*dx = (dx > 0) ? dx : -dx;
    dy = (dy > 0) ? dy : -dy;*/

    dx = sx * dx;
    dy = sy * dy;

    int x = x0;
    int y = y0;

    // |m| <= 1
    if (dx > dy)
    {
        int D = 2 * dy - dx;

        for (int i = 0; i < dx; i++)
        {
            if (D > 0)
            {
                y += sy;
                D += 2 * (dy - dx);
            }

            else
                D += 2 * dy;
            
            x += sx;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                dest[y * WIDTH + x] = { 0, 0, 0, 255 };
        }

    }

    // |m| > 1 -> swap x and y.
    else
    {
        int D = 2 * dx - dy;
        for (int i = 0; i < dy; i++)
        {
            if (D > 0)
            {
                x += sx;
                D += 2 * (dx - dy);
            }

            else
                D += 2 * dx;

            y += sy;

            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                dest[y * WIDTH + x] = { 0, 0, 0, 255 };
        }
    }


}
struct Vec2
{
    unsigned int x;
    unsigned int y;
    uint8_t color = 255; // WHITE

};

struct Vertex
{
    Vec2 v;
    Entry color;
};
/*
    edge function
    check if the target point P is within the edge. 
    int edge(P, v0, v1)
*/

int edge(Vec3 P, Vec3 v0, Vec3 v1)
{
    return (P.x - v0.x) * (v1.y - v0.y) - (P.y - v0.y) * (v1.x - v0.x);
}

// now what we need to think from now is to integrate this into SDL thingy. 
// what is required for this? 
// 1- the data is required. (but what kind of data?) -> (we're gonna render the triangle just by using this algorithm)
// 2- all points of a triangle.
// 

int main(int argc, char* argv[]) // program entry point.
{
    
    // 800 * 600 framebuffer. 
    // Same size as the window SDL3 created. 
    std::vector<Entry>  framebuffer(WIDTH * HEIGHT);
    std::vector<Z>      ZBuffer(WIDTH * HEIGHT);
    /*for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++) 
            framebuffer[y * WIDTH + x] = { 255, 255, 255, 255 };
    }*/
    Vec3        v0, v1, v2;
    Vec3        v3, v4, v5;

    v0 = { 100, 100 };
    v1 = { 700, 200 };
    v2 = { 400, 500 };

    v3 = { 300, 80 };
    v4 = { 600, 400 };
    v5 = { 150, 350 };
    /*
    A = (100, 100)
    B = (700, 200)
    C = (400, 500)
    */

    /*
        (300, 150)
    */

    Bresenhem(v0.x, v0.y, v1.x, v1.y, framebuffer); // x0, y0, x1, y1, dest
    Bresenhem(v1.x, v1.y, v2.x, v2.y, framebuffer);
    Bresenhem(v2.x, v2.y, v0.x, v0.y, framebuffer);

    Bresenhem(v3.x, v3.y, v4.x, v4.y, framebuffer); 
    Bresenhem(v4.x, v4.y, v5.x, v5.y, framebuffer);
    Bresenhem(v5.x, v5.y, v3.x, v3.y, framebuffer);
    // every pixel has 1.0 as a z value.
    // we need to update the value as we update to fragment.
    // Z value test should be done in the 
    /*
        Now, we have to implement barycentric coordinate system. 
        What does barycentric coordinate system does?
            
        We need a sample point. 
        Sample point P = ( 411, 275 ) { inside the triangle }
        
        what we need is v0, v1, v2. in this caes, 
        
        v0 = (100, 100)
        v1 = (700, 200)
        v2 = (400, 500)

        E(P, v0, v1) = (P.x-v0.x)(v1.y-v0.y) - (P.y-v0.y)(v1.x-v0.x); -> cross product of vector P-v0 (A), v1-v0 (B)
        in other words, A cross B.

        we need to do this for 3 times. 
        1- E(P, v0, v1)  ==  a
        2- E(P, v1, v2)  ==  b
        3- E(P, v2, v0)  ==  c
        
            Area T0, T1, T2

            T0 = 0.5 * b (since edge function returns the area of parellogram
            T1 = 0.5 * c 
            T2 = 0.5 * a

        if those function all returns the negative value, then it's within the edge. 
        continue to set the weighted mean

            weighted mean

            E(v2, v0, v1) == W  ---> 
            l0, l1, l2
            l0 + l1 + l2 = 1

            l0 should be float (8 Byte)
            l0 = b / W 
            l1 = c / W
            l2 = a / W

                                                          ##
        i wanna add a feature that when I hover on the screen, I can see what coordinate is my mouse on
                                                          ##
    */
    //int a, b, c, 
    //    W;
    //// winding -> a, b, c's sign has to equal to W.

    //// COLLINEAR TRIANGLE CASE : W == 0.

    //float l0, l1, l2, invW;
    //
    //P1      =       { 393, 274 };
    //P2      =       { 678, 405 };
    

    //a = edge(P1, v0, v1);
    //b = edge(P1, v1, v2);
    //c = edge(P1, v2, v0);
    //
    //W = edge(v2, v0, v1);
    //invW = 1.0f / W;
    
    /*if (!W)
    {
        SDL_Log("W is 0");
        return 0;
    }*/

    // we need bounding box: the smallest square that includes the pixel
    
    int         W1, W2;
    float       invW1, invW2;

  
    W1 = edge(v2, v0, v1);
    invW1 = 1.0f / W1;

    W2 = edge(v5, v3, v4);
    invW2 = 1.0f / W2;

    if (!W1)
    {
        SDL_Log("Triangle is either collinear, or it is not a triangle");
        return 1;
    }

    if (!W2)
    {
        SDL_Log("Triangle is either collinear, or it is not a triangle");
        return 1;
    }


    int xmin1, xmax1,
        ymin1, ymax1;

    int xmin2, xmax2,
        ymin2, ymax2;

    // bounding box.
    xmin1 = std::min({ v0.x, v1.x, v2.x }); xmax1 = std::max({ v0.x, v1.x, v2.x });
    xmin2 = std::min({ v3.x, v4.x, v5.x }); xmax2 = std::max({ v3.x, v4.x, v5.x });

    ymin1 = std::min({ v0.y, v1.y, v2.y }); ymax1 = std::max({ v0.y, v1.y, v2.y });
    ymin2 = std::min({ v3.y, v4.y, v5.y }); ymax2 = std::max({ v3.y, v4.y, v5.y });

    
    float l0 = 0.0f, l1 = 0.0f, l2 = 0.0f;
    int     a, b, c;

    for (int y = ymin1; y <= ymax1; y++)
    {
        for (int x = xmin1; x <= xmax1; x++)
        {
            float z = 0.5f;
            Vec3    P = { x, y };
            uint8_t R, G, B;

            a = edge(P, v0, v1);
            b = edge(P, v1, v2);
            c = edge(P, v2, v0);
            // 근데 이렇게하면 덮어씌우게 됨. -> 일단 이걸 여기서 처음 
            
            // Winding & test
            bool inside = (W1 > 0) ? (a >= 0 && b >= 0 && c >= 0)
                                   : (a <= 0 && b <= 0 && c <= 0);

            if (!inside)
                continue;

            P.zValue = z;

            ZBuffer[y * WIDTH + x] = { P.zValue };
            
            // 일단 저장 ㄱㄱ.
            

            l0 = b * invW1;
            l1 = c * invW1;
            l2 = a * invW1;
            // each constant represents C_v0, C_v1, C_v2

            
        }
    }

    // 이렇게 되면 색을 채우는게 총 두 번이 되고, 그냥 비교 한번 하고 한번에 전부 그릴 수는 없나? 
    float l3 = 0.0f, l4 = 0.0f, l5 = 0.0f;
    int d, e, f;
    d = e = f = 0;
    // hello!
    int Hello;

    for (int y = ymin2; y <= ymax2; y++)
    {
        for (int x = xmin2; x <= xmax2; x++)
        {
            float   z = 0.2f;
            Vec3    P = { x, y };
            uint8_t R, G, B;

            d = edge(P, v3, v4);
            e = edge(P, v4, v5);
            f = edge(P, v5, v3);

            // Winding & test
            bool inside = (W2 > 0) ? (d >= 0 && e >= 0 && f >= 0)
                : (d <= 0 && e <= 0 && f <= 0);

            if (!inside)
                continue;

            P.zValue = z;
            

            l3 = e * invW2;
            l4 = f * invW2;
            l5 = d * invW2;

            if (P.zValue < ZBuffer[y * WIDTH + x].z)
            {
                // 만약에 fragment의 z value가 ZBuffer에 저장되어있는 z value보다 작을 때, Zbuffer에 있는 놈을 바꾸고, 그 zBuffer에 있는 놈만을!
                 // 이 다음에 어떻게 해야 하지; 아니 애초에 바꿀 필요가 있나? 
                R = (uint8_t)(l3 * 0 + l4 * 0 + l5 * 255);
                G = (uint8_t)(l3 * 0 + l4 * 255 + l5 * 0);
                B = (uint8_t)(l3 * 255 + l4 * 0 + l5 * 255);
                framebuffer[y * WIDTH + x] = { R, G, B, 255 };
            }
            // there is no same value. 
            /*else 
            {
                R = (uint8_t)(l0 * 120 + l1 * 30 + l2 * 180);
                G = (uint8_t)(l0 * 210 + l1 * 68 + l2 * 90);
                B = (uint8_t)(l0 * 148 + l1 * 91 + l2 * 255);
                framebuffer[y * WIDTH + x] = { R, G, B, 255 };
            }*/

            // 바꾼 다음 z버퍼에 있는 값을 토대로 test한 후 그 이후가 되서야 그린다?
            // 일단 한 번 그린 다음에, 덮어씌운다?
            // each constant represents C_v0, C_v1, C_v2
         
          
            
        }
    }

  

   /* bool inside = (a <= 0 && b <= 0 && c <= 0);
    if (inside)
        SDL_Log("inside");

    SDL_Log("l0 + l1 + l1 = %.1f", (l0 + l1 + l2));*/

    // now what?
    // 

    /*a = edge(P2, v0, v1);
    b = edge(P2, v1, v2);
    c = edge(P2, v2, v0);*/

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Rasterizer", WIDTH, HEIGHT, 0);

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,       
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
    SDL_Event event; 

    float mx, my;
    int ww, wh;

    // program loop
    while (running)
    {
        // For getting the pixel's coordinate.
        SDL_GetMouseState(&mx, &my);

        SDL_GetWindowSize(window, &ww, &wh);

        int fx = (int)(mx * WIDTH / ww);
        int fy = (int)(my * HEIGHT / wh);
        

        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                SDL_Log("framebuffer pixel = (%d, %d)", fx, fy);

            //if (event.type == SDL_EVENT_WINDOW_MAXIMIZED)
            //{
            //    // if this is maximized, we need to resize the window to the user's resolution.
            //}
        }
        
        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Entry));

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        
        SDL_RenderPresent(renderer); 
        SDL_RenderClear(renderer); 
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}