#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/types.h>
#define SIZE 512
#define SVKEY 75
#define REQ 1

struct msgp
{
	long type;
	char text[SIZE];
};

void pipeComm()
{
	pid_t pid;
	int fd[2];
	char buf[SIZE];
	char msg[SIZE];
	memset(buf, 0, SIZE);
	memset(msg, 0, SIZE);
	if (pipe(fd) < 0)
	{
		perror("pipe");
		exit(1);
	}

	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);

		sprintf(msg, "%d is sending a message to parent %d by PIPE\n", getpid(), getppid());
		write(fd[1], msg, strlen(msg));

		exit(0);
	}
	else if (pid > 0)
	{
		close(fd[1]);

		sleep(0);

		read(fd[0], buf, SIZE);
		printf("%s", buf);
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

void msgQueueComm()
{
	int msqid = msgget(SVKEY, IPC_CREAT | 0664);
	if (msqid < 0)
	{
		perror("msgget");
		exit(1);
	}

	int pid = fork();
	if (pid == 0)
	{
		struct msgp msg;
		memset(&msg, 0, sizeof(struct msgp));
		msg.type = REQ;
		sprintf(msg.text, "%d", getpid());
		if (msgsnd(msqid, &msg, SIZE, 0) == -1)
		{
			perror("msgsnd");
			exit(1);
		}

		struct msgp buf;
		memset(&buf, 0, sizeof(struct msgp));
		if (msgrcv(msqid, &buf, SIZE, getpid(), 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}
		printf("receive reply from %s by MESSAGE QUEUE\n", buf.text);

		exit(0);
	}
	else if (pid > 0)
	{
		struct msgp buf;
		memset(&buf, 0, sizeof(struct msgp));
		if (msgrcv(msqid, &buf, SIZE, REQ, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}
		printf("serving for client %s by MESSAGE QUEUE\n", buf.text);

		sleep(0);

		struct msgp msg;
		memset(&msg, 0, sizeof(struct msgp));
		msg.type = atol(buf.text);
		sprintf(msg.text, "%d", getpid());
		if (msgsnd(msqid, &msg, SIZE, 0) == -1)
		{
			perror("msgsnd");
			exit(1);
		}

		if (msgctl(msqid, IPC_RMID, NULL) == -1)
		{
			perror("msgctl");
			exit(1);
		}
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

void sharedMemComm()
{
	char* msg;
	int shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0664);
	if (shmid == -1)
	{
		perror("shmget");
		exit(1);
	}

	int pid = fork();
	if (pid == 0)
	{
		msg = shmat(shmid, NULL, 0);
		if ((int)msg == -1)
		{
			perror("shmat");
			exit(1);
		}

		sprintf(msg, "%d is sending a message to parent %d by SHARED MEMORY\n", getpid(), getppid());

		if (shmdt(msg) == -1)
		{
			perror("shmdt");
			exit(1);
		}

		exit(0);
	}
	else if (pid > 0)
	{
		msg = shmat(shmid, NULL, 0);
		if ((int)msg == -1)
		{
			perror("shmat");
			exit(1);
		}

		sleep(0);

		printf("%s", msg);

		if (shmdt(msg) == -1)
		{
			perror("shmdt");
			exit(1);
		}

		if (shmctl(shmid, IPC_RMID, NULL) == -1)
		{
			perror("shmctl");
			exit(1);
		}
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

int main()
{
	int choice = 1;
	while (choice != 0)
	{
		printf("PLEASE CHOOSE:\n");
		printf("1.PIPE\n");
		printf("2.MESSAGE QUEUE\n");
		printf("3.SHARED MEMORY\n");
		printf("0.EXIT\n");
		printf("--------------------\n");

		scanf("%d", &choice);
		switch (choice)
		{
			case 1:
				pipeComm();
				break;
			case 2:
				msgQueueComm();
				sleep(0);
				break;
			case 3:
				sharedMemComm();
				break;
			case 0:
				return 0;
			default:
				printf("ERROR!\n");
				break;
		}

		printf("--------------------\n");
	}

	return 0;
}
