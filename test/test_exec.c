#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/debugr.h"

#define BUF_SIZE 2048

/*
    Description: Duplicate file descriptor in order to capture STDOUT

    Input:
        - (int) file descriptor

    Output:
        - (int) file descriptor
*/
int capture_fd(int fd)
{
    char name[] = "test_exec_XXXXXX";
    int captured = mkstemp(name);
    if(dup2(captured, fd) == -1) perror("dup2");

    unlink(name);

    return captured;
}

/*
    Description: compare output of a file descriptor with a char *

    Input:
        - (int) file descriptor
        - (char *) string

    Output:
        - null
        - assert on error

*/
void verify_text(int fd, char *text)
{
    char buf[BUF_SIZE] = { 0 };
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, BUF_SIZE-1);
    
    fprintf(stderr, "Compare against: %s", buf);
    if(strcmp(buf, text) == 0) return;

    assert(!"verify_text could not find the specified text");
}

int main()
{
    char *argv[1] = { 0 };

    int captured = capture_fd(STDOUT_FILENO);

    dbg_inferior_exec("./inferiors/hello", argv);
    
    verify_text(captured, "Hello, world!\n");

    return 0;
}