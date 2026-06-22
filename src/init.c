#include "init.h"

// -1 = continue
// >=0 = stop and exit with returned value
int arg_parse(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        if (arg[0] == '-') {
     
            if (arg[1] == '-') {
                // --theme=./theme.conf
                if (strncmp(arg, "--theme=", 8) == 0) {
                    const char *themePath = arg + 8;

                    #ifdef _WIN32
                    printf(stderr, "Error: Custom themes are not supported on Windows\n");
                    return 1;
                    #endif
                    if (themePath && *themePath) {
                        if(load_theme(themePath)){
                            fprintf(stderr, "Error: Failed to load theme %s\n",themePath);
                            return 1;
                        }
                    } else {
                        fprintf(stderr, "Error: --theme requires a path\n");
                        return 1;
                    }
                }
                else {
                    fprintf(stderr, "Unknown option: %s\n", arg);
                    return 1;
                }
            }
          
            else {
                for (int j = 1; arg[j]; j++) {
                    switch (arg[j]) {
                        case 'v':
                            printf("szci v1.0.0 - By SakizciAdam\n"
                                   "If you encounter any issues or have a suggestion, "
                                   "please open an issue on GitHub.\n"
                                   "Contributions are always welcome!\n"
                                   "Thanks for using szci!\n");
                            return 0;
                            break;
                        case 'r':
                            readOnly = true;
                            break;
                        default:
                            fprintf(stderr, "Unknown option: -%c\n", arg[j]);
                            return 1;
                    }
                }
            }
        }
        else if (!fileSet) {
            FILE *file = fopen(arg, "rb");
            if (!file) {
                fileName = malloc(strlen(arg) + 1);
                strcpy(fileName, arg);
                fileSet = true;
            } else {
                fileName = malloc(strlen(arg) + 1);
                strcpy(fileName, arg);
                fileSet = true;

                fseek(file, 0, SEEK_END);
                long len = ftell(file);
                fseek(file, 0, SEEK_SET);

                if (len >= 3) {
                    unsigned char bom[3];
                    fread(bom, 1, 3, file);
                    if (!(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)) {
                        fseek(file, 0, SEEK_SET);
                    } else {
                        len -= 3;
                    }
                }

                char *new_text = malloc(len + 1);
                if (new_text) {
                    fread(new_text, 1, len, file);
                    new_text[len] = '\0';
                    int j = 0;
                    for (int k = 0; k < len; k++) {
                        if (new_text[k] == '\r' && k + 1 < len && new_text[k + 1] == '\n')
                            continue;
                        new_text[j++] = new_text[k];
                    }
                    new_text[j] = '\0';
                    length = j;
                    free(text);
                    text = new_text;
                    saved=1;
                }
                fclose(file);
            }
        }
        else {
            fprintf(stderr, "Ignoring extra argument: %s\n", arg);
        }
    }

    return -1;
}
