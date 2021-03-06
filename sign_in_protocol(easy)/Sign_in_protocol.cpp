
// DKU software Engineering Hyunwoo koh, trghyunwoo@gmail.com
// for practice making sign in protocol

#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

char id[100][100] = { 0, };
char pw[100][100] = { 0, };
char str1[30] = { 0 };
char str2[30] = { 0 };
int id_count = 0;

int menu()
{
	int result = 0;
	printf("원하시는 작업을 고르세요. \n1)회원가입.\n2)로그인\n선택: ");
	scanf("%d", &result);
	if (result == 1) return 1;
	else if (result == 2) return 2;
	else
	{
		printf("존재하는 항목만 선택 하시오.\n");
		return 0;
	}
}
void get_id()
{
	printf("아이디를 입력 하십시오 : ");
	scanf("%s", str1);
	return;
}
void get_pw()
{
	printf("비밀번호를 입력 하십시오 : ");
	scanf("%s", str2);
	return;
}
int sign_up(char *d, char *p)
{
	if (id_count == 100)
	{
		printf("가입가능한 최대 회원수 입니다. 죄송합니다. \n");
		return 0;
	}
	else
	{
		for (int i = 0; id[i][0] != NULL; i++)
		{
			int k = 0;
			for (int j = 0; ; j++)
			{
				if (id[i][j] != d[j]) break;
				else k++;
			}
			if (id[i][k] == d[k])
			{
				printf("동일한 아이디가 존재합니다 .다른 아이디를 선택해 주세요. \n");
				return 0;
			}
		}

		for (int i = 0; d[i] != NULL; i++)
			id[id_count][i] = d[i];
		for (int i = 0; p[i] != NULL; i++)
			pw[id_count][i] = p[i];
		id_count++;

		printf("회원등록이 완료되었습니다. 다시 로그인 해주세요. 감사합니다. \n");

		return 1;
	}
}
int log_in(char *d, char *p)
{
	int k = 0, q = 0;

	for (int i = 0; i < id_count; i++)
	{
		k = 0;
		for (int j = 0; id[i][j] != 0; j++)
		{
			if (id[i][j] == d[j] == NULL) break;
			if (id[i][j] != d[j]) break;
			else k++;

		}
		if (id[i][k] == d[k])
		{
			k = i;
			break;
		}
	}

	for (int i = 0; pw[k][i] != 0; i++)
	{
		if (pw[k][i] != p[i]) break;
		q++;
	}
	if (pw[k][q] == p[q])
	{
		printf("로그인에 성공하였습니다.\n");
		return 1;
	}
	else
	{
		printf("비밀번호를 확인 하세요. \n");
		return 0;
	}
}
int main(void)
{
	while (1)
	{

		int result = menu();
		while (1)
		{
			if (result == 0) break;
			else if (result == 1)
			{
				get_id();
				get_pw();
				sign_up(str1, str2);
				break;
			}
			else
			{
				get_id();
				get_pw();
				log_in(str1, str2);
				break;
			}

		}
	}
	return 0;
}
