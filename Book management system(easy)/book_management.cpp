// ConsoleApplication141.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS>


typedef struct book {
	char title[50];
	char aurthor[30];
	char publisher[30];
	int is_rented;
}BOOK;
void menu()
{
	int result;
	printf("Welcome to my library!!\n");
	printf("==================================\n");
	printf("1. book add: 도서 추가\n2. book rental: 도서 대여 \n3.return book: 도서 반납\n4.system exit: 시스템 종료\n");
	printf("==================================\n");
}
int select_menu()
{
	int result = 0;
	printf("select menu : ");
	scanf("%d", &result);
	return result;
}
BOOK* book_save(int num)
{
	BOOK *ex;
	ex = (BOOK *)malloc(sizeof(BOOK) * num);
	for (int i = 0; i < num; i++) {
		printf("title:");
		getchar();
		gets_s(ex[i].title);
		printf("author:");
		gets_s(ex[i].aurthor);
		printf("publisher:");
		gets_s(ex[i].publisher);
		printf("is_rented:");
		scanf("%d", &ex[i].is_rented);
		printf("==================================\n");
	}
	return ex;
}
int find_book(char *str1, char *str2)
{
	for (int i = 0; ; i++)
	{
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}
void book_print(BOOK b)
{
	printf("==================================\n");
	printf("title: %s\n", b.title);
	printf("author: %s\n", b.aurthor);
	printf("publisher: %s\n", b.publisher);
	printf("is_rented: %d\n", b.is_rented);
}
int main(void)
{
	BOOK bk[100];
	BOOK *p;
	int bk_count = 0;
	menu();
	p = &bk[0];
	while (1)
	{
		int result = select_menu();
		if (result == 1)
		{
			int num = 0;
			printf("Number of books to sava : ");
			scanf("%d", &num);
			BOOK *ex_bk = book_save(num);

			for (int i = 0; i < num; i++)
			{
				bk[bk_count + i] = ex_bk[i];
			}
			bk_count += num;

			for (int i = 0; i < bk_count; i++)
				book_print(bk[i]);
			printf("==================================\n");
		}
		else if (result == 2)
		{
			char book_name[50];
			int k = 0, same_num = 0;
			printf("빌리고 싶은 책의 이름을 입력하시오.");
			getchar();
			gets_s(book_name);
			for (int i = 0; i < bk_count; i++)
			{
				k = find_book(bk[i].title, book_name);
				if (k == 1) {
					same_num = i;
					break;
				}
			}
			if (bk[same_num].is_rented == 0)
			{
				bk[same_num].is_rented = 1;
				printf("이용해 주셔서 감사합니다.\n", bk[same_num]);
			}
			else
				printf("지금은 대여가 불가능합니다. 죄송합니다.\n");
		}
		else if (result == 3)
		{
			char book_name[50];
			int k = 0, same_num = 0;
			printf("반납하실 책의 이름을 입력하시오.");
			getchar();
			gets_s(book_name);
			for (int i = 0; i < bk_count; i++)
			{
				k = find_book(bk[i].title, book_name);
				if (k == 1)
				{
					same_num = i;
					break;
				}
			}
			bk[same_num].is_rented = 0;
			printf("이용해 주셔서 감사합니다.\n", bk[same_num].is_rented);
		}
		else
			break;
	}
	printf("시스템이 종료됩니다.\n");
	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
