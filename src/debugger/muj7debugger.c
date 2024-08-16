/**
 * @file muj7debugger.c
 *
 * @brief Implementation of a debugger for muj7 programs
 *
 * Provides a vi-like TUI to load and debug binary files containing muj7 programs 
 * It uses ncurses library to implement the tui
 *
 * @see LICENSE for licensing details.
 */

#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "helper/muj7helper.h"

struct TuiContext
{
    WINDOW* commands_window;
    WINDOW* memory_window;
    WINDOW* registers_window;
    uint16_t initial_address; 
    uint8_t mode;
};

typedef char** command_tokens;

const uint8_t COMMAND_SPLIT_MAX = 10;

struct TuiContext initialize_layouts()
{
    initscr();
    noecho();
    curs_set(0);

    start_color();
    use_default_colors();

    clear();

    WINDOW* memory_window = newwin(LINES - 2, COLS, 1, 2);
    wrefresh(memory_window);

    WINDOW* registers_window = newwin(LINES - 2, 20, 1, 20);
    wrefresh(registers_window);

    WINDOW* commands_window = newwin(1, COLS, LINES - 1, 0);
    keypad(commands_window, TRUE);
    wrefresh(commands_window);

    struct TuiContext attr;
    attr.memory_window = memory_window;
    attr.registers_window = registers_window;
    attr.commands_window = commands_window;
    attr.initial_address = 0x00;


    return attr;
}

void clear_commands_window(WINDOW* commands_window)
{
    int x, y;
    wbkgd(commands_window, COLOR_PAIR(0));
    getyx(commands_window, y, x);
    wmove(commands_window, y, 0);
    wclrtoeol(commands_window);
}

void display_memory(struct CPU* cpu, struct TuiContext* tui)
{
    wmove(tui->memory_window, 0, 1);
    for (ssize_t i = 0; i < LINES - 4; i++)
    {
        int i_addr = i + tui->initial_address;
        if (i_addr == cpu->registers->pc)
            wattron(tui->memory_window, A_BOLD);
        
        mvwprintw(tui->memory_window, i, 0, "0x%04x: 0x%02x", i_addr, cpu->ram[i_addr]);
        wattroff(tui->memory_window, A_BOLD);
    }

    wrefresh(tui->memory_window);
}

void display_registers(struct CPU* cpu, struct TuiContext* tui)
{
    mvwprintw(tui->registers_window, 0, 2, "PC: 0x%04x", cpu->registers->pc);
    mvwprintw(tui->registers_window, 1, 2, "SP: 0x%04x", cpu->registers->sp);
    mvwprintw(tui->registers_window, 3, 2, "A: 0x%02x", cpu->registers->a);
    mvwprintw(tui->registers_window, 4, 2, "B: 0x%02x", cpu->registers->b);
    mvwprintw(tui->registers_window, 5, 2, "C: 0x%02x", cpu->registers->c);
    mvwprintw(tui->registers_window, 6, 2, "F: 0x%02x", cpu->registers->f);
    mvwprintw(tui->registers_window, 8, 2, "Cycles: %d", cpu->clock_cycles);

    wrefresh(tui->registers_window);
}

void update_info_cpu(struct CPU* cpu, struct TuiContext* tui)
{
    display_memory(cpu, tui);
    display_registers(cpu, tui);
}

void process_command(struct CPU* cpu, struct TuiContext* tui, const command_tokens tokens, uint8_t tokens_size)
{    
    if(!tokens_size)
        return;

    if(strcmp(tokens[0], "q")==0 || strcmp(tokens[0], "quit")==0)
    {
        endwin();
        exit(0);
    }

    if(strcmp(tokens[0], "s")==0 || strcmp(tokens[0], "step")==0)
    {
        execute(cpu);
        update_info_cpu(cpu, tui);
    }

    if(tokens_size == 2 && strcmp(tokens[0], "load")==0)
    {
        char* filename = tokens[1];
        load_binary(cpu, filename);

        cpu->registers->pc = 0;
        tui->initial_address = 0;
        update_info_cpu(cpu, tui);
    }

    if(tokens_size == 2 && strcmp(tokens[0], "set")==0)
    {
        tui->initial_address = atoi(tokens[1]); 
        display_memory(cpu, tui);
    }
}

void process_normal_mode(struct CPU* cpu, struct TuiContext* tui)
{
    int ch;
    while(tui->mode == 0)
    {
        ch = wgetch(tui->commands_window);

        switch(ch)
        {
            case ':':
                tui->mode = 1;
                wprintw(tui->commands_window, ":");
                break;

            case 's':
                execute(cpu);
                update_info_cpu(cpu, tui);

            default:
                break;
        }
    }
}

void process_command_mode(struct CPU* cpu, struct TuiContext* tui)
{
    int ch;
    int cursor_command = 0;
    char command[40];

    while(tui->mode == 1)
    {
        ch = wgetch(tui->commands_window);

        switch(ch)
        {
            case 10:    // enter key
                command[cursor_command] = '\0';
                command_tokens tokens = (command_tokens)malloc(sizeof(char*)*COMMAND_SPLIT_MAX);
                uint8_t tokens_size = 0;

                char* token = strtok(command, " ");
                while(token != NULL)                    
                {
                    if(tokens_size >= COMMAND_SPLIT_MAX)
                        break;

                    tokens[tokens_size++] = token;
                    token = strtok(NULL, " ");
                }

                process_command(cpu, tui, tokens, tokens_size);
               
                command[0] = '\0';
                cursor_command = 0;
                clear_commands_window(tui->commands_window);
                tui->mode = 0;

                break;

            default:
                if (!isprint(ch))
                    break;

                command[cursor_command++] = ch;
                wprintw(tui->commands_window, "%c", ch);
                break;
        }
    }
}

int start_tui(struct CPU* cpu, struct TuiContext* tui)
{
    int ch;

    while (true)
    {
        
        if(tui->mode == 0)
        {
            process_normal_mode(cpu, tui);
        }

        else if(tui->mode == 1)
        {
            process_command_mode(cpu, tui);
        }
    }

    endwin();

    return 0;
}

int main(int argc, char** argv)
{
    struct TuiContext tui = initialize_layouts();
    struct CPU* cpu = create_cpu(512);

    if(argc >= 2)
    {
        load_binary(cpu, argv[1]);
    }

    update_info_cpu(cpu, &tui);
    start_tui(cpu, &tui);
}

