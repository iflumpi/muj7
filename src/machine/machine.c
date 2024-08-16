
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "cpu/muj7cpu.h"
#include "helper/muj7helper.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 200;

const int VIDEO_MEMORY_START = 16384;
const int VIDEO_MEMORY_SIZE = (SCREEN_WIDTH*SCREEN_HEIGHT)/8;

int refresh_screen(struct CPU* cpu, SDL_Window* main_window)
{
    SDL_Surface* surface = SDL_GetWindowSurface(main_window);
    uint32_t* pixels = (uint32_t*)surface->pixels;

    for(ssize_t i=0; i<VIDEO_MEMORY_SIZE; i++)
    {
        uint8_t byte_memory = cpu->ram[VIDEO_MEMORY_START + i];

        for(int j=7; j>=0; j--)
        {
            pixels[i*8 + j] = byte_memory & (int)pow(2, j) ? 0xFFFFFFFF : 0;
        }
    }
    
    SDL_UpdateWindowSurface(main_window);

    return 0;
}

int main(int argc, char** argv)
{ 
    if(argc <= 1)
    {
        printf("Usage: %s binary_file\n\n", argv[0]);
        return 1;
    }

    struct CPU* cpu = create_cpu(65536);
    load_binary(cpu, argv[1]);

    for(int i=0; i<VIDEO_MEMORY_SIZE; i++)
        cpu->ram[i + VIDEO_MEMORY_START] = 0;

    SDL_Window* main_window = SDL_CreateWindow("muj7 machine", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Event event;

    int p_status = 1;
    uint64_t ticks = SDL_GetTicks64();
    uint64_t cycles = 0;
    int value = 0;
    while(p_status)
    {
        execute(cpu);

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case 'q':
                        p_status = 0;
                        break;

                    case 'd':
                        printf("A: %d\n", cpu->registers->a);
                        break;

                    default:
                        break;
                }
            }
        }

        if(SDL_GetTicks64() - ticks >= 20)
        {
            refresh_screen(cpu, main_window);

            ticks = SDL_GetTicks64();
        }
    }

    return 0;
}


