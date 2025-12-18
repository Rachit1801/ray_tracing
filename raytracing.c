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

void drawRays(SDL_Renderer* renderer, float cx, float cy, int rayCount,struct Circle circle, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    float ox = cx, oy = cy;  // ray origin

    for (int i = 0; i < rayCount; ++i) {
        float angle = (2.0f * M_PI * i) / rayCount;
        float dx = cosf(angle);
        float dy = sinf(angle);

        // Compute intersection with circle using line equation
        float cx_ = circle.x, cy_ = circle.y, radius = circle.r;

        // Vector from ray origin to circle center
        float fx = ox - cx_;
        float fy = oy - cy_;

        float a_term = dx * dx + dy * dy;
        float b_term = 2 * (fx * dx + fy * dy);
        float c_term = fx * fx + fy * fy - radius * radius;

        float discriminant = b_term * b_term - 4 * a_term * c_term;

        if (discriminant >= 0) {
            // Ray hits the circle
            discriminant = sqrtf(discriminant);

            float t1 = (-b_term - discriminant) / (2 * a_term);
            float t2 = (-b_term + discriminant) / (2 * a_term);

            float t = fminf(t1, t2);  // smallest positive intersection

            if (t > 0) {
                float x2 = ox + dx * t;
                float y2 = oy + dy * t;
                SDL_RenderLine(renderer, ox, oy, x2, y2);
            }
        } else {
            // No hit: draw full ray
            SDL_RenderLine(renderer, ox, oy, ox + dx * 1000, oy + dy * 1000);
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
    SDL_FRect rect = {100.0f, 100.0f, 100.0f, 100.0f};
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

        drawRays(renderer, 320, 240, 100, circle, 255, 160, 160, 255);

        fillCircle(renderer,circle,255,255,255,255);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
