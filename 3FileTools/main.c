#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

int fd;
int opened = 0;
char fileName[100];
char* pargv[4] = { "ls", "-l", NULL, NULL};

void createFile()
{
	printf("PLEASE INPUT THE FILENAME:\n");
	scanf("%s", fileName);
	fd = open(fileName, O_RDWR | O_CREAT, 0751);

	if (fd < 0)
	{
		printf("OPEN FAILED.\n");
		printf("--------------------\n");
		return;
	}

	opened = 1;
	pargv[2] = (char*)malloc(100);
	strcpy(pargv[2], fileName);

	printf("OPEN SUCCEED.\n");
	printf("--------------------\n");
}

void writeFile()
{
	if (opened == 0)
	{
		printf("PLEASE OPEN A FILE FIRST.\n");
		printf("--------------------\n");
		return;
	}

	char buffer[1024];
	memset(buffer, 0, 1024);
	lseek(fd, 0, SEEK_END);
	printf("WRITE SOMETHING:\n");
	scanf("%s", buffer);
	buffer[strlen(buffer)] = '\n';
	int result = (int)write(fd, buffer, strlen(buffer));

	if (result == -1)
	{
		printf("WRITE FAILED.\n");
		printf("--------------------\n");
		return;
	}

	printf("WRITE SUCCEED.\n");
	printf("--------------------\n");
}

void readFile()
{
	if (opened == 0)
	{
		printf("PLEASE OPEN A FILE FIRST.\n");
		printf("--------------------\n");
		return;
	}

	char buffer[1024];
	memset(buffer, 0, 1024);
	lseek(fd, 0, SEEK_SET);
	int result = (int)read(fd, buffer, 1024);

	if (result == -1)
	{
		printf("READ FAILED.\n");
		printf("--------------------\n");
		return;
	}

	printf("READ SUCCEED. FILE CONTENT IS:\n");
	printf("--------------------\n");
	printf("%s", buffer);
	printf("--------------------\n");
}

void changeMode()
{
	if (opened == 0)
	{
		printf("PLEASE OPEN A FILE FIRST.\n");
		printf("--------------------\n");
		return;
	}

	int mode;
	printf("NEW OCTAL NUMERIC NOTATION IS:\n");
	scanf("%o", &mode);
	int result = chmod(fileName, (__mode_t)mode);

	if (result == -1)
	{
		printf("CHANGE MODE FAILED.\n");
		printf("--------------------\n");
		return;
	}

	printf("CHANGE MODE SUCCEED.\n");
	printf("--------------------\n");
}

void viewMode()
{
	if (opened == 0)
	{
		printf("PLEASE OPEN A FILE FIRST.\n");
		printf("--------------------\n");
		return;
	}

	printf("FILE INFORMATION IS:\n");
	int result = execv("/bin/ls", pargv);

	if (result == -1)
	{
		printf("VIEW MODE FAILED.\n");
		printf("--------------------\n");
		return;
	}

	printf("VIEW MODE SUCCEED.\n");
	printf("--------------------\n");
}

int main()
{
	int choice = 1;
	while (choice != 0)
	{
		printf("PLEASE CHOOSE:\n");
		printf("1.CREATE OR OPEN FILE\n");
		printf("2.WRITE FILE\n");
		printf("3.READ FILE\n");
		printf("4.CHANGE FILE MODE\n");
		printf("5.VIEW FILE MODE\n");
		printf("0.EXIT\n");
		printf("--------------------\n");

		scanf("%d", &choice);
		switch (choice)
		{
			case 1:
				createFile();
				break;
			case 2:
				writeFile();
				break;
			case 3:
				readFile();
				break;
			case 4:
				changeMode();
				break;
			case 5:
				viewMode();
				break;
			case 0:
				close(fd);
				return 0;
			default:
				printf("ERROR!\n");
				break;
		}
	}

	return 0;
}
