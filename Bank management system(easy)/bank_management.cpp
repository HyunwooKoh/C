// ConsoleApplication140.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS>

typedef struct bank
{
	char number[30];
	char name[20];
	int balance = 0;
}BANK;
void get_infor(BANK b)
{
	printf("계좌번호를 입력 하세요 : ");
	gets_s(b.number);
	printf("예금주 : ");
	gets_s(b.name);
}
int menu()
{
	int result = 0;
	printf("select menu : 1)입금 2)출금 3)잔액조회 4)종료 : ");
	scanf("%d", &result);
	return result;
}
BANK deposit(BANK b)
{
	int amount = 0;
	printf("input money : ");
	scanf("%d", &amount);
	b.balance = b.balance + amount;
	return b;
}
BANK withdraw(BANK b)
{
	int amount = 0;
	printf("output money : ");
	scanf("%d", &amount);
	if (b.balance < amount)
		printf("Lack of money on your acount.\n");
	else
		b.balance -= amount;
	return b;
}
void print(BANK b)
{
	printf("balance : %d \n", b.balance);
	return;
}
int main(void)
{
	BANK acount;
	get_infor(acount);
	while (1)
	{
		int result = menu();
		if (result == 4) break;

		switch (result)
		{
		case 1:
			acount = deposit(acount);
			print(acount);
			break;
		case 2:
			acount = withdraw(acount);
			print(acount);
			break;
		case 3:
			print(acount);
			break;
		default:
			break;
		}
	}
	ret

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
