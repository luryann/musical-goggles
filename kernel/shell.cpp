#include "shell.h"
#include "memory/mem.h"
#include "fs/ramfs.h"
#include "../tools/picsofbread.h"
#include "video/window.h"

char NataluzzVer[] = "7120";

Shell newShell;
bool xRunning = false;

void Shell::PrintPrompt()
{
    if (shouldPrint)
    {
        //printf("C:/> ");
        printf("%sroot@NatalieTextMyAssBack%s / %s# ", Red, Blue, White);
    }    
}

char ver[]      = "ver";
char help[]     = "help";
char clear[]    = "clear";
char neofetch[] = "neofetch";
char rm[]       = "rm";
char panic[]    = "panic";
char exitx[]    = "exitx";
char startx[]   = "startx";
char pob[]      = "pob";
char tree[]     = "tree";
char cat[]      = "cat";
char lsStr[]    = "ls";
char shutdown[] = "shutdown";
char blank[] = "";

void Shell::TestCMD(char* input)
{
    TwoStrings twoStrings;
    for (int i = 0; input[i] != '\0'; i ++) {
        twoStrings.a[i] = '\0';
    }
    for (int i = 0; input[i] != '\0'; i ++) {
        twoStrings.b[i] = '\0';
    }

    twoStrings = strsplit(input, ' ');
    
    if (mystrcmp(input, "ver"))
    {
        printf("Nataluzz version %s\n", NataluzzVer);
    }
    else if (mystrcmp(input, help))
    {
        printf("List of available commands:\n");
        printf("    ver:         shows the version of Nataluzz\n");
        printf("    help:        shows this menu\n");
        printf("    clear:       clears the screen\n");
        printf("    neofetch:    only a Linux user would understand\n");
    }
    else if (mystrcmp(input, "clear"))
    {
        Clear(0);
    }
    else if (mystrcmp(input, "neofetch"))
    {
        uint64_t totMem = 0;
        uint64_t useMem = 0;
        uint64_t freMem = 0;
        for (uint64_t i = 0; i < mmap.response->entry_count; i++)
        {
            uint64_t type = mmap.response->entries[i]->type;
            if (type == LIMINE_MEMMAP_USABLE || type == LIMINE_MEMMAP_ACPI_RECLAIMABLE || type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            {
                freMem += mmap.response->entries[i]->length;
            }
            else
            {
                useMem += mmap.response->entries[i]->length;
            }
        }
        totMem = useMem + freMem;       
        printf("\n");
        printf("              _        _                      root@Nataluzz\n");
        printf("  _ __   __ _| |_ __ _| |_   _ ________       --------------\n");
        printf(" | '_ \\ / _` | __/ _` | | | | |_  /_  /      OS: Nataluzz %s\n", NataluzzVer);
        printf(" | | | | (_| | || (_| | | |_| |/ / / /        Kernel: %s\n", NataluzzVer);
        printf(" |_| |_|\\__,_|\\__\\__,_|_|\\__,_/___/___|   Shell: NatShell (Ykthelore)\n");
        printf("                                              Memory: %i KB/%i KB\n", useMem / 1024, totMem / 1024);
        printf("                                              Resolution: %ix%i\n", buffer.width, buffer.height);
        printf("              _        _               \n");
        printf("  _ __   __ _| |_ __ _| |_   _ ________\n");
        printf(" | '_ \\ / _` | __/ _` | | | | |_  /_  /\n");
        printf(" | | | | (_| | || (_| | | |_| |/ / / / \n");
        printf(" |_| |_|\\__,_|\\__\\__,_|_|\\__,_/___/___|\n");
        printf("\n");
    }
    else if (mystrcmp(input, "")){}
    else if (mystrcmp(input, "rm"))
    {
        printf("your 'rm' program is bad, sorry.\n");
    }
    else if (mystrcmp(input, "panic"))
    {
        Panic("user init panic");
    }
    else if (mystrcmp(input, "exitx"))
    {
        xRunning = false;
        Clear(0);
        newShell.shouldPrint = true;
    }
    else if (mystrcmp(input, "startx"))
    {
        xRunning = true;
        drawRect(0, 0, buffer->width, buffer->height, 0xD97F1956);
        Window yes = Window(10, 10, 200, 200, "Hello World        ", 0x12345678);
        Window no = Window(100, 100, 200, 200, "1234567890!@#$%^&*()", 0x12345678);

        yes.DrawWindow();
        no.DrawWindow();

        drawRect(0, buffer->height - 40, buffer->width, 40, 0xff888888);        
    
        newShell.shouldPrint = false;
    }
    else if (mystrcmp(input, "pob"))
    {
        drawImage(g_picsofbread_data, buffer->width / 2 - 36,  buffer->height / 2 - 36);
    }
    else if (mystrcmp(input, "tree"))
    {
        for (int i = 0; headers[i]->filename != "\0"; i++)
        {
            printf("%s\n", headers[i]->filename);
        }
        
    }
    else if (mystrcmp(twoStrings.a, "cat"))
    {
        printf(readFile(twoStrings.b));
    }
    else if (mystrcmp(input, "ls"))
    {
        ls();
    }
    else if (mystrcmp(input, "shutdown"))
    {
        Clear(0);
        printf("shutting down now on some nonchalant type shit");
        sleep(2);
        outw(0xB004, 0x2000);
        outw(0x604, 0x2000);
        outw(0x4004, 0x3400);
    }
    else
    {
        printf("Unknown command: \"%s\"\n", input);
    }
}