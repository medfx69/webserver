#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>


void    exec(char *filename)
{
    char *cmd = "/usr/bin/python";
    char* argv[] = {cmd, filename, NULL};
    execve(argv[0], argv, NULL);
    perror("execve");
}

int    read_output(int *pipefd)
{
    int output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd == -1)
        return 1;
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0)
    {
        if (write(output_fd, buffer, bytes_read) == -1)
            return 1;
    }
    if (bytes_read == -1)
        return 1;
    close(output_fd);
    return 0;
}

int    exec_outfile(char *filename)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return 1;
    pid_t pid = fork();
    if(pid == -1)
        return 1;
    if(pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        exec(filename);
        return 1;
    }
    else
    {
        close(pipefd[1]);
        read_output(pipefd);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
int main()
{
    exec_outfile("t.py");
}