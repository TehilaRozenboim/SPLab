#include <stdio.h>

extern int open_file(const char *file_path, int open_mode);
extern int close_file(int _fd);

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 1;

    int _fd = open_file(argv[1], 0);
    printf("The corresponding file descriptor is: %d\n", _fd);
    
    int ret = close_file(_fd);
    if (ret >= 0)
        printf("CLOSED DONE\n");
    else
        printf("CLOSING FAILED\n");

    return 0;
}