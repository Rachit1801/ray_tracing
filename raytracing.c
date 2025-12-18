#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct Circle {
    double x;
    double y;
    double r;
};

void drawRays(SDL_Renderer* renderer, double cx, double cy, int rayCount,struct Circle circle, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    for (int i = 0; i < rayCount; i++) {
        double angle = (2.0 * M_PI * i) / rayCount;
        double dx = cos(angle);
        double dy = sin(angle);

        /*double t = 10000.0; // large enough to go off screen
        double x2 = cx + dx * t;
        double y2 = cy + dy * t;*/

        bool hit = false;

        double step = 1;
        double x = cx;
        double y = cy;
        while(!hit) {
            x += step * dx;
            y += step * dy;
            if(x < 0 || x > WINDOW_WIDTH)
                hit = true;
            if(y < 0 || y > WINDOW_HEIGHT)
                hit = true;
            double dist_sq = pow(x - circle.x,2) + pow(y - circle.y,2);
            if(dist_sq < pow(circle.r,2))
                hit = true;
            step++;
            //SDL_RenderPoint(renderer,x,y);
            SDL_RenderLine(renderer, cx, cy, x, y);
        }
    }
}

void fillCircle(SDL_Renderer* renderer, struct Circle circle, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    double radius_sq = pow(circle.r,2);
    for(double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
        for(double y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
            double dist_sq = pow(x - circle.x,2) + pow(y - circle.y,2);
            if(dist_sq<=radius_sq) {
                SDL_RenderPoint(renderer,x,y);
                //SDL_FillSurfaceRect(SDL_Surface *dst, pixel, Uint32 color);
            }
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL); 

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    /*
    Rectangle

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect rect = {100.0, 100.0, 100.0, 100.0};
    SDL_RenderFillRect(renderer, &rect);
    */

    struct Circle circle = {320,240,80};
    SDL_Event event;
    bool running = true;
    while(running) {
        while(SDL_PollEvent(&event)) {                  //quit application when corss is clicked
            if(event.type == SDL_EVENT_QUIT)            //without this we would have to go to process and kill the process every time
                running = false;

            if(event.type == SDL_EVENT_MOUSE_MOTION && event.motion.state != 0) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
        SDL_RenderClear(renderer);

        drawRays(renderer,320,240,100,circle,255,160,160,255);

        fillCircle(renderer,circle,255,255,255,255);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}