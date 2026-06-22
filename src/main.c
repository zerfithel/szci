
#include "init.h"
#include "input.h"


int main(int argc, char **argv) {
    // Initialize global variables
    mode=WRITING_MODE;
    selStart=-1;
    selEnd=-1;
    text=malloc(sizeof(char));
    if (!text) {
        fprintf(stderr, "Error: failed to allocate memory on heap for text\n");
        return 1;
    }
    length=0;
    statusText=malloc(sizeof(char)*200);
    if (!statusText) {
        fprintf(stderr, "Error: failed to allocate memory on heap for status text\n");
        return 1;
    }
    fileName = NULL;
    fileSet = false;
    set_status_text(":)");

    int exit_code = arg_parse(argc, argv);
    if (exit_code >= 0) {
      return exit_code;
    }
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_colors();

    render();
    while(true){
        if(kbhit()){

            char c;

            #ifdef _WIN32
            c=_getch();
            #else
            c=getch();
            #endif
            
            /*
            printf("%d %c\n",c,c);

            if(true){
                continue;
            }*/
            on_key(c,mode);
            render();
          
        }
    }

    endwin();

    return 0;
}
