#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VIM_PATH "/usr/bin/vim"
#define MAX_HEADERS 20

void printUsage(char *programName)
{
    fprintf(stderr, "Usage: %s <file_name> [header1 header2 ... headerN]\n", programName);
}

int main(int argc, char *argv[], char *envp[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error: Insufficient arguments.\n");
        printUsage(argv[0]);
        return 1;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "%s.c", argv[1]);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <unistd.h>\n");

    // Check if additional headers are provided
    if (argc > 2)
    {
        for (int i = 2; i < argc && i < MAX_HEADERS + 2; i++)
        {
            fprintf(file, "#include <%s>\n", argv[i]);
        }
    }

    fprintf(file, "\nint main(int argc, char** argv, char** envp)\n");
    fprintf(file, "{\n\n");
    fprintf(file, "\treturn 0;\n");
    fprintf(file, "}");

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        return 1;
    }

    char *vim_args[] = {VIM_PATH, filename, NULL};
    if (execvp(VIM_PATH, vim_args) == -1)
    {
        perror("execvp");
        return 1;
    }

    return 0;
}
