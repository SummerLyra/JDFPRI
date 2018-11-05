#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

pid_t pid1, pid2;
int fd[2];

void handler1(int sg)
{
	if (sg == SIGINT)
	{
		kill(pid1, SIGUSR1);
		kill(pid2, SIGUSR1);
	}
}

void handler2(int sg)
{
	close(fd[0]);
	close(fd[1]);

	if (pid1 == 0 && sg == SIGUSR1)
	{
		printf("Child Process 1 is killed by Parent!\n");
		exit(0);
	}
	if (pid2 == 0 && sg == SIGUSR1)
	{
		printf("Child Process 2 is killed by Parent!\n");
		exit(0);
	}
}

int main()
{
	int myclock = 1;
	char buf[64];
	char msg[64];
	memset(buf, 0, 64);
	memset(msg, 0, 64);

	if (pipe(fd) < 0)
	{
		perror("pipe");
		exit(1);
	}

	signal(SIGINT, handler1);

	pid1 = fork();

	if (pid1 == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, handler2);

		while (1)
		{
			close(fd[0]);
			sprintf(msg, "I send message %d times.\n", myclock);
			write(fd[1], msg, strlen(msg));
			myclock++;
			sleep(1);
		}
	}

	else if (pid1 > 0)
	{
		pid2 = fork();

		if (pid2 == 0)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGUSR1, handler2);

			while (1)
			{
				close(fd[1]);
				read(fd[0], buf, 64);
				printf("%s", buf);
			}
		}

		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
		close(fd[0]);
		close(fd[1]);
		printf("Parent Process is killed!\n");
	}

	else
	{
		perror("fork");
		exit(1);
	}

	return 0;
}
