#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define VIM_PATH "/usr/bin/vim"

void printUsage(char *programName)
{
    fprintf(stderr, "Usage: %s <file_name> [-i header1 header2 ...]\n", programName);
}

int main(int argc, char *argv[])
{
    int interactive = 0;
    int headerIndex = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Error: Insufficient arguments.\n");
        printUsage(argv[0]);
        return 1;
    }

    // Check if "-i" option is provided
    if (argc > 2 && strcmp(argv[2], "-i") == 0)
    {
        interactive = 1;
        headerIndex = 3; // Adjust the index to the headers
    }

    // Calculate the length of the input filename
    size_t filename_len = strlen(argv[1]) + strlen(".c") + 1;

    // Allocate memory for the filename
    char *filename = malloc(filename_len);
    if (filename == NULL)
    {
        perror("Error allocating memory for filename");
        return 1;
    }

    // Construct the filename
    snprintf(filename, filename_len, "%s.c", argv[1]);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        free(filename);
        return 1;
    }

    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <unistd.h>\n");

    // Check if additional headers are provided
    if (interactive && argc > headerIndex)
    {
        for (int i = headerIndex; i < argc; i++)
        {
            // 14 accounts for "#include <", the header, and ">\n\0"
            char *header = malloc(strlen(argv[i]) + 14);
            if (!header)
            {
                perror("Error allocating memory");
                fclose(file);
                free(filename);
                return 1;
            }
            snprintf(header, strlen(argv[i]) + 14, "#include <%s>\n", argv[i]);
            fprintf(file, "%s", header);
            free(header);
        }
    }

    fprintf(file, "\nint main(int argc, char *argv[], char *envp[])\n");
    fprintf(file, "{\n\n");
    fprintf(file, "\treturn 0;\n");
    fprintf(file, "}");

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        free(filename);
        return 1;
    }

    char *vim_args[] = {VIM_PATH, filename, NULL};
    if (execvp(VIM_PATH, vim_args) == -1)
    {
        perror("execvp");
        free(filename);
        return 1;
    }

    free(filename);
    return 0;
}
