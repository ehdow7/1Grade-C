#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
 

 
#define ESC 27
#define MAXENEMY 10
 
//전역 변수
int fx;	//플레이어 좌표 
int bx, by; //총알 좌표 
int score;	//점수	 
int stage=1;	//스테이지 
double tim=30; //시간 
 
 
void gotoxy(int x, int y)	//커서 위치 조정하는 함수 
{
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
 
 
void CursorView(char show)	//커서 숨기는 함수 
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
 
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
 
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
 
  
struct tag_Enemy	//적 구조체 
{
    BOOL exist; //존재 여부 
    int type;	//적 종류 
    int x, y;	//적 좌표 
    int way;	//이동 방향 
    int frame;	//프레임 수 
    int stopframe;	//정지상태 프레임 수 
}
Enemy[MAXENEMY];
 
const char *enemylist[] = { " (:^:) "," <^W^> ", " (OxO) ", " <T=T> " };	//적 종류 포인터 
 
BOOL IsKeyDown(int Key)	//키 입력 판단 함수 
{
    return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}
 
 
main()
{	
	int a; 
    int ch,ch1,ch2;	//키보드 입력 변수 
    int i, j, wall;	//다양한 곳에서 사용되는 변수, 벽만드는 변수 
    BOOL found;	//참 또는 거짓을 가지는 불리언 자료형 
    int count;
	int fever=1; //피버 변수 
	int stack=0;	//천장 변수 
	char name[100];	//이름 변수 배열 
    srand((unsigned)time(NULL));	//난수 
    system("cls");	//화면 초기화 
    CursorView(0);	//커서 숨기기 함수 불러오기 
    fx = 60;	//기본 플레이어 좌표 
    bx = -1;	//기본 총알 좌표 
    score = 0;	//기본 점수 설정 
    FILE * log;	//파일 포인터 
    log = fopen("log.txt","a");
    
    gotoxy(30,10);
    printf("정말 재밌는 FFF급 슈팅게임");
    gotoxy(35,20);
	printf("게임 시작(1)");
	gotoxy(35,22);
	printf("게임 설명(2)");
	gotoxy(35,24);
	printf("게임 종료(3)\n");
	while(1){
	gotoxy(50,22);
	scanf("%d",&a);
	if(a==1){
	system("cls");
	gotoxy(30,10);
	printf("사용자의 이름을 입력해주세요.(기록저장)\n");
	gotoxy(45,18);
	scanf("%s",&name);		
    goto wall;	//wall 레이블 이동
	}
	else if(a==2){
		goto help;	//help 레이블 이동
	}
	else if(a==3)
	{
		goto end; //end 레이블 이동
 }
 else
 {
 	gotoxy(55,22);
 	printf("잘못된 입력");
 	continue; 
 }
}
help:
	system("cls");
	gotoxy(20,10);
	printf ("총 3라운드를 가지고 있으며 1,2 라운드는 30초, 3라운드는 60초를 가집니다.");
	gotoxy(20,13);
	printf ("2라운드는 점수 50점이상, 최종 라운드는 점수 150점이상 시에 갈 수 있습니다.");
	gotoxy(20,16);
	printf("좌우 방향키로 이동, space를 누르면 총알을 발사하고 총알이 적에 닿으면 점수를 얻습니다.");
	gotoxy(20,19);
	printf ("적은 랜덤하게 나오며, 속도에 따라 얻는 점수가 다릅니다.");
	gotoxy(20,22);
	printf ("시간이 10초 남았을땐 얻는 점수가 2배가 되니 이때를 노려보세요.");
	gotoxy(20,25);
	printf("space를 눌러 게임을 시작하세요.");
	while(1){
    a = getch();
    switch (a) 
                {
                case ' '://space 입력시 
                	system("cls");
					gotoxy(30,10);
					printf("사용자의 이름을 입력해주세요.(기록저장)\n");
					gotoxy(45,18);
					scanf("%s",&name);
                    goto wall;	//wall 레이블 이동
                    break;
                default:	//space외 입력은 반응안하기 
                	continue;
                }
            }
wall:
	system("cls");	//화면 초기화 
	 for(wall=0; wall<26; wall++)	//세로 벽 생성 
 	{
 		gotoxy(80,wall);
 		printf("|");
	 }
	for(wall=0; wall<80; wall++)	//가로 벽 생성 
 	{
 		gotoxy(wall,25);
 		printf("ㅡ");
	 }
	 goto Start;
    
Start: 
    for(count=0; ; count++) {
        // 좌우 이동 처리
        if (count % 2 == 0) 	//이동속도 제한 (2프레임 당 한칸) 
        {
            if (IsKeyDown(VK_LEFT))	//왼쪽 화살표 누르면 
            {
                if (fx > 6) //x좌표 6초과면 
                    fx--;	//왼쪽으로 이동 
            }
            if (IsKeyDown(VK_RIGHT))	//오른쪽 화살표 누르면 
            {
                if (fx < 75)	//x좌표 75미만이면 
                    fx++;	//오른쪽으로 이동 
            }
        }
 
        // 키 입력 처리
        if (kbhit()) 
        {
            ch = getch();	//ch에 입력한 키 받기 
            
            switch (ch) 
                {
                case ' '://space 일때 
                    if (bx == -1) //총알이 발사 안됐으면 
                    {
                        bx = fx;	//총알 x좌표는 플레이어 좌표 
                        by = 20;	//총알 y좌표 설정 
                    }
                    break;
                case ESC:	//ESC일때 
                    goto end;	//end 레이블 이동 
                }
            }
                
        
 
        // 적군 생성
        if (rand() % 30 == 0) 
        {
            for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; } //Enemy배열에서 i높이며 존재하지 않는 적 찾기 
            if (i != MAXENEMY) 
            {
                if ((rand() % 2) + 1 == 1)	//50퍼센트 확률로 
                {
                    Enemy[i].x = 5;	 
                    Enemy[i].way = 1;	//왼쪽에서 나오기
                }
 
                else
                {
                    Enemy[i].x = 75;
                    Enemy[i].way = -1;	//오른쪽에서 나오기 
                }
 
                for (;;) 
                {
                    Enemy[i].y = rand() % 10 + 1;	//적 y좌표 랜덤 생성 
                    for (found = FALSE, j = 0; j < MAXENEMY; j++)	//y좌표 중복 검사 반복문
                    {
                        if (Enemy[j].exist == TRUE && Enemy[j].y == Enemy[i].y) 	//이미 존재하는 적과 y가 같으면 
                        {
                            found = TRUE;	//found TRUE로 설정 
                            break;	//y좌표 중복 검사 반복문 나가기 
                        }
                    }
                    if (found == FALSE)	//y좌표 중복된 적이 없으면 
                    {
                        break;	//반복문 나가기 
                    }
                }
                Enemy[i].frame = Enemy[i].stopframe = rand() % 6 + 1; //적 속도 랜덤 결정  
                Enemy[i].type = rand() % (sizeof(enemylist) / sizeof(enemylist[0])); //enemylist배열 중 무작위 선택
                Enemy[i].exist = TRUE;	//적 존재 TRUE 
            }
        }
        //적 천장 시스템 
        //프레임마다 적을 소환하지 못하면 
        else{
        	stack++; //스택 1 증가 
        	if(stack==50) //스택이 60이면 
        	{
        		//적 생성 위랑 동일 
                for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; }
            if (i != MAXENEMY) 
            {
                if ((rand() % 2) + 1 == 1)
                {
                    Enemy[i].x = 5;
                    Enemy[i].way = 1;
                }
 
                else
                {
                    Enemy[i].x = 75;
                    Enemy[i].way = -1;
                }
 
                for (;;) 
                {
                    Enemy[i].y = rand() % 10 + 1;
                    for (found = FALSE, j = 0; j < MAXENEMY; j++) 
                    {
                        if (Enemy[j].exist == TRUE && Enemy[j].y == Enemy[i].y) 
                        {
                            found = TRUE;
                            break;
                        }
                    }
                    if (found == FALSE)
                    {
                        break;
                    }
                }
                Enemy[i].frame = Enemy[i].stopframe = rand() % 6 + 2;  //위보다 속도 1 높은 적 
                Enemy[i].type = rand() % (sizeof(enemylist) / sizeof(enemylist[0])); 
                Enemy[i].exist = TRUE;
                stack=0; //스택 초기화 
            }
        		
			}
		}
 
        // 아군 총알 이동 및 출력
        if (bx != -1)
        {
            gotoxy(bx, by); //총알 위치로 이동 
            putch(' ');	//총알 초기화 
            if (by == 0) 	//총알 못맞출 때  
            {	//총알 초기화 
            	gotoxy(bx, 0);
            	putch(' ');
            	gotoxy(bx, 1);
            	putch(' ');
            	gotoxy(bx, -1);
            	putch(' ');
                bx = -1; //총알 좌표 초기화 
            }
            else
            {
                by--;	//총알 위로 한칸 이동 
                gotoxy(bx, by);	//총알 위치 조정 
                putch('|');	//총알 출력 
            }
        }
 
        // 적군과 아군 총알의 충돌 판정
        for (i = 0; i < MAXENEMY; i++)	//적 최대 갯수만큼 반복 
        {
            if (Enemy[i].exist == FALSE) //적 존재하지않으면 넘기기 
                continue;
 
            if (Enemy[i].y == by && abs(Enemy[i].x - bx) <= 2) //적의 y 좌표가 총알의 y 좌표와 같고, 적과 총알의 x 좌표 차이가 2 이하면 
            {
                bx = -1;	//총알 좌표 재설정 
                Enemy[i].exist = FALSE;	//적 존재 여부 X 
                gotoxy(Enemy[i].x - 3, Enemy[i].y);	//적 위치로 이동 
                puts("       ");	//적 위치에 공백 
                score += (7 - Enemy[i].frame )* fever;	//점수 속도비례 증가, 피버시 두배 
                break;
            }
        }
 
 		if(stage==2&&tim<=0&&score>=150) //시간이 0초고 스테이지가 2, 점수가 150이상이면 
 		{
 			system("cls");	//화면 초기화 
 			gotoxy(30,10);
			printf("TIME OVER!");
			Sleep(2000);
 			goto final;	//final 레이블 이동 
		 }
		else if (stage==1&&tim<=0&&score>=50) //시간이 0초고 스테이지가 1, 점수가 50이상이면
		{
			system("cls");	//화면 초기화 
			gotoxy(30,10);
			printf("TIME OVER!");
			Sleep(2000);
			
			goto clear;	//clear 레이블 이동
		}
		else if (tim<=0)	//앞에 조건이 충족안되고 시간이 0이면 
		{
			system("cls");	//화면 초기화 
			gotoxy(30,10);
			printf("TIME OVER!");
			Sleep(2000);
			
			goto end;	//end 레이블 이동
		}
 
        // 적군 이동 및 출력
        for (i = 0; i < MAXENEMY; i++) {	//적 최대 수만큼 반복 
            if (Enemy[i].exist == FALSE) 	//적 존재하지 않으면 넘기기 
			continue;	
            if (--Enemy[i].stopframe == 0) {	//stopframe을 줄이고 stopframe이 0이면 
                Enemy[i].stopframe = Enemy[i].frame;	//stopframe 재설정 
                if (Enemy[i].x >= 76 || Enemy[i].x <= 4) {	//적이 화면 밖에 나가면 
                    Enemy[i].exist = FALSE;	//적 비활성화 
                    gotoxy(Enemy[i].x - 3, Enemy[i].y);	//적의 위치로 이동 
                    puts("       ");	//적 삭제 
                }
                else
                {
                    Enemy[i].x += Enemy[i].way;	//way에 따라 x좌표 이동 
                    gotoxy(Enemy[i].x - 3, Enemy[i].y);	//적의 위치로 이동 
                    puts(enemylist[Enemy[i].type]);	//적 출력 
                

                    }
                }
            }
        
 
        gotoxy(fx - 3, 20); //플레이어 위치 조정 
        puts(" <<A>> "); //플레이어 출력 
        Sleep(40);    // 초당 25 프레임 
        printf("%s\n",name); //플레이어 이름 출력 
        printf("점수=%d\n", score);	//점수 현황 출력 
        printf("시간:%.2f\n",tim);	//시간 현황 출력 
        if(tim<=10)	//시간이 10초이면 
        {
        	fever=2; //피버 활성화 
        	printf("FEVER 점수 2배");	//피버 표시 
		}
 		tim=tim-0.04;	//프레임 따른 시간 감소 
    }
clear:
	system("cls");	//화면 초기화
    gotoxy(15, 10);
    printf("   #      ######                                  #####                                 ### ");
    gotoxy(15, 11);
    printf("  ##      #     #  ####  #    # #    # #####     #     # #      ######   ##   #####     ### ");
    gotoxy(15, 12);
    printf(" # #      #     # #    # #    # ##   # #    #    #       #      #       #  #  #    #    ### ");
    gotoxy(15, 13);
    printf("   #      ######  #    # #    # # #  # #    #    #       #      #####  #    # #    #     #  ");
    gotoxy(15, 14);
    printf("   #      #   #   #    # #    # #  # # #    #    #       #      #      ###### #####         ");
    gotoxy(15, 15);
    printf("   #      #    #  #    # #    # #   ## #    #    #     # #      #      #    # #   #     ### ");
    gotoxy(15, 16);
    printf(" #####    #     #  ####   ####  #    # #####      #####  ###### ###### #    # #    #    ### ");
    gotoxy(58, 22);
    printf("점수=%d\n\n", score);
    stage++;
    gotoxy(40, 21);
    printf("최종 라운드 150점 이상 시 진입 가능\n");
    Sleep(1000);
    gotoxy(40, 22);
    printf("space를 눌러 2 라운드 시작");
    tim=30; //시간 재설정 
    fever=1; //피버 재설정 
    while(1){
    ch1 = getch();
    switch (ch1) 
                {
                case ' ':	//space 입력시 
                	system("cls");	//화면 초기화
                    goto wall;
                    break;
                default:	//space외 입력은 반응안하기 
                	continue;
                }
            }
	
final:
	system("cls");	//화면 초기화
    gotoxy(15, 10);
    printf(" #####     ######                                  #####                                 ### ");
    gotoxy(15, 11);
    printf("#     #    #     #  ####  #    # #    # #####     #     # #      ######   ##   #####     ### ");
    gotoxy(15, 12);
    printf("      #    #     # #    # #    # ##   # #    #    #       #      #       #  #  #    #    ### ");
    gotoxy(15, 13);
    printf(" #####     ######  #    # #    # # #  # #    #    #       #      #####  #    # #    #     #  ");
    gotoxy(15, 14);
    printf("#          #   #   #    # #    # #  # # #    #    #       #      #      ###### #####         ");
    gotoxy(15, 15);
    printf("#          #    #  #    # #    # #   ## #    #    #     # #      #      #    # #   #     ### ");
    gotoxy(15, 16);
    printf("#######    #     #  ####   ####  #    # #####      #####  ###### ###### #    # #    #    ### ");

    gotoxy(58, 22);
    printf("점수=%d\n\n", score);
    stage++;
    gotoxy(40, 21);
    printf("최종 라운드 제한시간 60초\n");
    Sleep(1000);
    gotoxy(40, 22);
    printf("space를 눌러 최종 라운드 시작");
    tim=60;	//시간 재설정 
    fever=1; //피버 재설정  
    while(1){
    ch2 = getch();
    switch (ch2) 
                {
                case ' ':	//space 입력시 
                	system("cls");	//화면 초기화
                    goto wall;
                    break;
                default:	//space외 입력은 반응안하기 
                	continue;
                }
            }
end:
    system("cls");	//화면 초기화
    gotoxy(30, 10);
    printf(" #####     #    #     # #######    ####### #     # ####### ######  ");
    gotoxy(30, 11);
    printf("#     #   # #   ##   ## #          #     # #     # #       #     # ");
    gotoxy(30, 12);
    printf("#        #   #  # # # # #          #     # #     # #       #     # ");
    gotoxy(30, 13);
    printf("#  #### #     # #  #  # #####      #     # #     # #####   ######  ");
    gotoxy(30, 14);
    printf("#     # ####### #     # #          #     #  #   #  #       #   #   ");
    gotoxy(30, 15);
    printf("#     # #     # #     # #          #     #   # #   #       #    #  ");
    gotoxy(30, 16);
    printf(" #####  #     # #     # #######    #######    #    ####### #     # ");

    gotoxy(54, 22);
    printf("%d라운드 점수=%d",stage, score);
    fprintf(log,"%s : %d라운드 점수 = %d\n",name,stage, score);	//log.txt에 출력 
    fclose(log); 
	getch();	//플레이어 입력 전엔 화면 그대로  


}