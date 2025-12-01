#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int ft_popen(const char *file, char *const argv[], char type)
{
    if(!file || !argv || (type != 'r' && type != 'w'))
        return (-1);

    int pipefd[2];
    if (pipe(pipefd) == -1)
        return (-1);
    
    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }

    if (pid == 0)
    {
        if (type == 'r')
        {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
        }
        execvp(file, argv);
    }

    if (type == 'r')
    {
        close(pipefd[1]);
        return (pipefd[0]);
    }
    else
    {
        close(pipefd[0]);
        return (pipefd[1]);
    }
}

int main()
{
    printf("=== テスト1: ls の出力を読む ('r') ===\n");
    int fd = ft_popen("ls", (char *[]){"ls", "-l", NULL}, 'r');
    if (fd == -1)
    {
        printf("エラー\n");
        return 1;
    }
    
    char buf[1000];
    int n = read(fd, buf, 999);
    buf[n] = '\0';
    printf("読めた内容:\n%s\n", buf);
    close(fd);
    wait(NULL);

    printf("\n=== テスト2: cat に書き込む ('w') ===\n");
    fd = ft_popen("cat", (char *[]){"cat", NULL}, 'w');
    if (fd == -1)
    {
        printf("エラー\n");
        return 1;
    }
    
    write(fd, "Hello from ft_popen!\n", 21);
    write(fd, "This is line 2\n", 15);
    close(fd);
    wait(NULL);
    
    printf("\n=== テスト3: grep でフィルタ ('w') ===\n");
    fd = ft_popen("grep", (char *[]){"grep", "hello", NULL}, 'w');
    if (fd == -1)
    {
        printf("エラー\n");
        return 1;
    }
    
    write(fd, "hello world\n", 12);
    write(fd, "goodbye\n", 8);
    write(fd, "say hello again\n", 16);
    close(fd);
    wait(NULL);

    sleep(1);  // 子プロセスの出力を待つ
    
    return 0;
}

