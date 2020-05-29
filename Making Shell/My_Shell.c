#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 512
#define DELIMS " \t\r\n"

#define BACKGROUND "&"
#define REDIRECT ">"

size_t parse(char* CmdLine, char* args[]);
void help();
void handle(int SIGN);
int Execution(char* CmdLine);

int main(int argc, char* argv[])
{
	char line[MAX_SIZE] = "\0";
	while (1)
	{
		printf("[%s]$",get_current_dir_name());
		if (!fgets(line, sizeof(line) - 1, stdin)) break;
		fflush(stdin);
		if (strcmp(line, "\n") == 0) continue;
		if (Execution(line)) break;
	}
	return EXIT_SUCCESS;
}

size_t parse(char* CmdLine, char*args[])
{
	size_t count = 0;
	char* arg = strtok(CmdLine, DELIMS);
	while (arg != NULL)
	{
		args[count++] = arg;
		arg = strtok(NULL, DELIMS);
	}
	args[count] = NULL;
	return count;
}

void help()
{
	printf("==========================================\n");
	printf("%-7s : SHOW FILES AND DIRECTORY IN THIS DIRECTORY\n", "ls");
	printf("%-7s : CHANGE DIRECTORY\n", "cd");
	printf("%-7s : COMPILER\n", "gcc");
	printf("%-7s : MAKE FILE\n", "vi");
	printf("%-7s : SHOW TEXT IN FILE\n", "cat");
	printf("...............\n");
	printf("==========================================\n");
	return;
}
void handle(int SIGN)
{
	int status;
	pid_t pid;
	if ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		fprintf(stderr, "killed process: %d\n", pid);
	return;
}

int Execution(char* CmdLine)
{
	int arg_count = 0;
	char* argv[MAX_SIZE];
	char* args[MAX_SIZE];
	int i, status,fd = -1;
	int flag = 0;
	int redirect_index;
	pid_t pid;
	size_t n_args = parse(CmdLine, args);
	if (n_args == 0) return 0;

	for (redirect_index = 0; redirect_index < n_args; redirect_index++)
	{
		if (strcmp(args[redirect_index], REDIRECT) == 0)
		{
			flag = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		}
	}
	if (strcmp(args[0], "help") == 0)
	{
		help();
		return 0;
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		if (n_args == 1)
			chdir(getenv("HOME"));
		else if (n_args == 2)
		{
			if (chdir(args[1]))
				printf("Directory is not exist.\n");
		}
		return 0;
	}
	else if ((strcmp(args[0], "exit") == 0) || (strcmp(args[0], "quit") == 0))
		return 1;
	else
	{
		signal(SIGCLD, (void*)handle);
		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			return 0;
		}
		else if (pid == 0)
		{
			for (i = 0; i < redirect_index; i++)
				argv[i] = args[i];
			if (flag > 0)
			{
				if ((fd = open(args[redirect_index + 1], flag, 0644)) == -1)
				{
					perror("open");
					return 0;
				}
				if (close(STDOUT_FILENO) == -1)
				{
					perror("close");
					return 0;
				}
				if (dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("dup2");
					return 0;
				}
				execvp(argv[0], argv);
				printf("Can't execution\n");
				return 0;
			}
			else
			{
				execvp(args[0], args);
				printf("%s: Can't execution\n", args[0]);
				return 0;
			}

		}
		else if (strcmp(args[n_args - 1], BACKGROUND))
			waitpid(pid, &status, 0);
		return 0;
	}
}
		
