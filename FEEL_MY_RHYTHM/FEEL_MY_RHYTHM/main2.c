#define MAX_NOTE 6
#define ROW_SIZE 160
#define COLUMN_SIZE 50
#include "input.h"
#include "main.h"
#include "audio.h"



///
/// 04. 10
/// 커서 받기 gotoxy, initConsole 등
/// 창 사이즈 설정
/// ***더블 버퍼링 찾아보기***
/// 






// 화면 버퍼값 받아올 구조체 (사각형마냥 값을 저장한다고 함)
SMALL_RECT consoleScreenSize;
// UI 최대 사이즈 설정값
SMALL_RECT _UImaxSize;
// 버퍼(화면) 2개 설정
HANDLE hScreen[2];
bool bScreenIndex;


/// <summary>
/// 초기화
/// 버퍼 받기 -> 나중에 콘솔창 크기 따로 설정하기
/// </summary>
void initConsole()
{
	//system("mode con cols=160 lines=50 | title FEEL_THE_RHYTHM");
	// 더블 버퍼링.. 버퍼 두개 만들기
	hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// 커서 숨겨버리기 (안깜빡임! 안보임!)
	HANDLE consonleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO consoleCursor = { 0, };
	consoleCursor.bVisible = 0;											// 0 이면 커서를 감춤, 0 이 아닐 경우에 보인다
	consoleCursor.dwSize = 1;											// 값에 따라 커서의 크기를 조절한다

	// 커서에대한 설정값을 각 화면에 입력
	SetConsoleCursorInfo(consonleHandle, &consoleCursor);
	SetConsoleCursorInfo(hScreen[0], &consoleCursor);
	SetConsoleCursorInfo(hScreen[1], &consoleCursor);


	// 화면 사이즈 받아오기 (버퍼값 받아서 ScreenSize 에 대입)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	GetConsoleScreenBufferInfo(hScreen[0], &csbi);
	GetConsoleScreenBufferInfo(hScreen[1], &csbi);
	consoleScreenSize.Left = csbi.srWindow.Left;
	consoleScreenSize.Right = csbi.srWindow.Right;
	consoleScreenSize.Bottom = csbi.srWindow.Bottom;
	consoleScreenSize.Top = csbi.srWindow.Top;

	_UImaxSize.Left = consoleScreenSize.Left + 2;
	_UImaxSize.Right = consoleScreenSize.Right - 2;
	_UImaxSize.Bottom = consoleScreenSize.Bottom - 2;
	_UImaxSize.Top = consoleScreenSize.Top + 2;


}

HANDLE GetScreenHandle()
{
	int index = (bScreenIndex ? 1 : 0);

	return hScreen[bScreenIndex];
}


void SetStdScreen()
{
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

void SetBackScreen()
{
	SetConsoleActiveScreenBuffer(GetScreenHandle());
}


/// 버퍼 두개를 전환해주는 함수
void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(GetScreenHandle());
	bScreenIndex = !bScreenIndex;
}



/// 화면을 비우는 함수. 다음 버퍼로 전환할 때 이전 출력내용을 비워줘야한다
void ScreenClear()
{
	COORD coordScreen = { 0, 0 };
	DWORD dwConSize;


	// 콘솔 창을 공백으로 채우기
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwConSize);

	// 색상 속성을 지정하여 콘솔 창을 지우기
	WORD wColors = ((WORD)0 << 4) | (WORD)15; // 흰색 글자색, 검정 배경색
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwConSize);
}

void ScreenClear1(int text_color, int back_color)
{
	COORD coordScreen = { 0, 0 };
	DWORD dwConSize;


	// 콘솔 창을 공백으로 채우기
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwConSize);

	// 색상 속성을 지정하여 콘솔 창을 지우기
	WORD wColors = ((WORD)back_color << 4) | (WORD)text_color; // 흰색 글자색, 검정 배경색
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwConSize);
}

//void ScreenClear()
//{
//	COORD coor = { 0,0 };
//	DWORD dw;
//
//	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dw, coor, &dw);
//	WORD wColors = ((WORD)0 << 4) | (WORD)15; // 흰색 글자색, 검정 배경색
//	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dw, coor, &dw);
//		//문자 수가 화면 버퍼에서 지정된 행의 끝 이상으로 확장되면 다음행에 기록된다(자동으로 다음행으로 넘어가는듯)
//		//버퍼보다 문자 수가 큰 경우는 버퍼의 끝 까지만 기록된다
//		//작성된 위치의 특성 값은 변경되지 않는다(? ? ) 색 변경이 안되는건가
//	FillConsoleOutputCharacter(GetScreenHandle(), 'a', 180 * 60, coor, &dw);
//
//
//	FillConsoleOutputCharacter(GetScreenHandle(), ' ', 180 * 60, coor, &dw);
//
//	 ////[가로줄에 전부 공백 채우기] 를 세로줄 개수만큼 돌림
//		//for (int i = 0; i < COLUMN_SIZE; i++)
//		//{
//		//	coor.Y = i;
//		//	//공백을 화면크기만큼 입력해줘야함;;
//		//	for (int j = 0; j < ROW_SIZE; j++)
//		//	{
//		//		setColor(color_yellow, color_blue);
//		//		ScreenPrint(j, i, ' ', 1);
//		//	}
//
//		//	FillConsoleOutputCharacter(GetScreenHandle(), 'a', _UImaxSize.Right - _UImaxSize.Left + 5, coor, &dw);
//		//}
//
//}

/// ScreenPrint(10, 2, '@', 1)
/// 버퍼에 그림그리기 (공백 두 칸 찍기) (문자 하나만 찍을 수 있음)
void ScreenPrint(int x, int y, const char* str, int length)
{
	DWORD dw;	// unsigned long 구조체
	COORD CursorPosition = { x, y };
	


	SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
	// 2 는 문자열 길이

	



	WriteFile(GetScreenHandle(), &str, length, &dw, NULL);
}


///
/// 큰 따옴표 안에 출력할 문자열을 넣는다 "문자열 어쩌구~~~"
/// 다시 만듬
/// 긴 문자열 출력용..gg
void ScreenDraw(int x, int y, const char* str)
{
	DWORD dw;
	COORD Cur = { x, y };

	SetConsoleCursorPosition(GetScreenHandle(), Cur);

	// 문자열의 길이를 for 루프를 사용하여 계산
	int strLen = 0;
	while (str[strLen] != '\0')
	{
		strLen++;
	}

	

	WriteFile(GetScreenHandle(), str, strLen, &dw, NULL);
}








/// 버퍼 닫기
void ScreenRelease()
{
	CloseHandle(hScreen[0]);
	CloseHandle(hScreen[1]);
}







/// <summary>
/// 커서 위치 이동하는 함수
/// y 좌표는 아래로 갈수록 증가, 원점은 좌측 상단
/// </summary>
/// <param name="x">이동할 x좌표</param>
/// <param name="y">이동할 y좌표</param>
void gotoXY(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}





///
/// 노트 카운트, 노트 초기위치 초기화
/// 

COORD NotecurPos_l[17];
COORD NotecurPos_d[17];
COORD NotecurPos_u[17];
COORD NotecurPos_r[17];




void SetNotePosition(int y)
{
	noteCount = sizeof(l_note) / sizeof(int);
	for (int i = 0; i < noteCount; i++)
	{
		NotecurPos_l[i].X = 8;
		NotecurPos_d[i].X = 24;
		NotecurPos_u[i].X = 40;
		NotecurPos_r[i].X = 56;

		NotecurPos_l[i].Y = y;
		NotecurPos_d[i].Y = y;
		NotecurPos_u[i].Y = y;
		NotecurPos_r[i].Y = y;
	}
}









///
/// 04. 10
/// 인풋 값을 받으면
/// 색칠이 되는 UI
/// 거의 전체화면
/// 







// 배경 색, 글꼴 색 지정
void setColor(int backColor, int textColor)
{
	//textColor = color_white;
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backColor << 4) + textColor);
	SetConsoleTextAttribute(GetScreenHandle(), (backColor << 4) + textColor);
}




/// <summary>
/// 화살표 출력 원본 (UP KEY)
/// </summary>
const int arr[5][5] =
{
	{0,0,1,0,0},
	{0,1,1,1,0},
	{1,0,1,0,1},
	{0,0,1,0,0},
	{0,0,1,0,0}
};


/// <summary>
/// 너무 지저분하고 다양한 사용(나중에 위에 올라올 노트 출력에도 써야하니)
/// 따로 빼줘볼까
/// 상하좌우 화살표 출력 함수이다
/// </summary>
/// <param name="pos">좌표 정보를 담고있는 구조체</param>
/// <param name="color">무슨 색으로 출력할깡?</param>
void DrawUpArrow(COORD pos, int color)
{

	// arr[i][j] up
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			// j 는 가로 출력 (x), i 는 세로 출력을 담당 (y)
			// 가로는 세로보다 두배 출력해야함.. -> j*2
			gotoXY(pos.X + 2 * j, pos.Y + i);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}



void ScreenDrawUpArrow(COORD pos, int color)
{
	//const char* c = '  ';
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//ScreenPrint(pos.X + 2 * j, pos.Y + i, NULL);
			////gotoXY(pos.X + 2 * j, pos.Y + i);
			//if (arr[i][j] == 0)
			//{
			//	setColor(color_black, 15);
			//}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * j, pos.Y + i, '  ', 2);
				//printf("  ");
			}
		}
	}
}

void DrawLeftArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * i, pos.Y + j);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawLeftArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * i, pos.Y + j, '  ', 2);
			}
		}
	}
}

void DrawRightArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 8 - 2 * i, pos.Y + j);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawRightArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 8 - 2 * i, pos.Y + j, '  ', 2);
			}
		}
	}
}

void DrawDownArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * j, pos.Y + 4 - i);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawDownArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			setColor(color_black, 15);
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * j, pos.Y + 4 - i, '  ', 2);
			}
		}
	}
}





/// 
/// 배열로 출력하자
/// 0 이면 넘김, 1이면 공백을 출력하고 배경 색상 변경
/// 인터페이스 상 키를 그린다! 키가 입력되면 색상이 활성화 됨
void ScreenDrawKeyInterface()
{
	// 인터페이스 출력 좌표 지정 
	// 5x5 크기의 화살표. 출력의 시작은 좌측 상단 끝
	const int x = 8;
	const int y = 3;

	const int padding = 16;

	const COORD posLeft = { x + padding * 0, y };	// 8
	const COORD posDown = { x + padding * 1, y };	// 24
	const COORD posUp = { x + padding * 2, y };		// 40
	const COORD posRight = { x + padding * 3, y };	// 56

	/// 안 누른 기본 상태는 흰색
	// Up
	int colorUp = color_white;
	// Down
	int colorDown = color_white;
	// Left
	int colorLeft = color_white;
	// Right
	int colorRight = color_white;


	// 키가 눌리면?
	if (GetKeyTable(UP))
	{
		// 한번만 입력받기 위해 false 처리
		//SetKeyTable(UP, false);
		// 색상을 빨강으로 바꾼다
		colorUp = color_gray;
	}

	if (GetKeyTable(DOWN))
	{
		//SetKeyTable(DOWN, false);
		// 색상을 green으로 바꾼다
		colorDown = color_gray;
	}

	if (GetKeyTable(LEFT))
	{
		//SetKeyTable(LEFT, false);
		// 색상을 skyblue으로 바꾼다
		colorLeft = color_gray;
	}

	if (GetKeyTable(RIGHT))
	{
		//SetKeyTable(RIGHT, false);
		// 색상을 yellow으로 바꾼다
		colorRight = color_gray;
	}

	ScreenDrawUpArrow(posUp, colorUp);
	ScreenDrawRightArrow(posRight, colorRight);
	ScreenDrawDownArrow(posDown, colorDown);
	ScreenDrawLeftArrow(posLeft, colorLeft);

	/// 다른 함수로 정리함
	{
		// 이차원 배열을 설정하고 화살표 모양을 만듦
	//int arr[5][5] =
	//{
	//	{0,0,1,0,0},
	//	{0,1,1,1,0},
	//	{1,0,1,0,1},
	//	{0,0,1,0,0},
	//	{0,0,1,0,0}
	//};

	// arr[i][j] up
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		// j는 가로 출력 (x), i는 세로 출력을 담당 (y)
	//		// 가로는 세로보다 두배 출력해야함.. -> j*2
	//		gotoXY(x + 2 * j, y + i);
	//		//if (arr[i][j] == 0)
	//		//{
	//		//	setColor(color_black);
	//		//	//printf("  ");
	//		//}	 없어도 무관
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}

	// arr[i][j] left
	// y=x 대칭 (윈도우의 y축은 아래쪽 증가)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x +  2*i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	// arr[i][j] right
	// left의 x= 5 대칭 (가로줄은 두칸단위임!!!!!) -> 그래서 (5-1)가 아니라 (10-2)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 8 - 2 * i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}
	//

	// arr[i][j] 
	// 배열은 항상 0번째부터 출력됨을 명심하자!! (5 - 1) = 4
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 2 * j, y + 4 - i);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	/*setColor(color_black);
	printf("    ");
	setColor(color, 15);
	printf("  \n");

	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  \n");

	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("      ");
	setColor(color, 15);
	printf("  ");

	setColor(color_black);*/
	}
}





///
/// 노트 출력, 이동

///
/// 일정한 시간마다 업데이트를 하도록 하자
/// deltaTime 
///

// 시작 시간
ULONGLONG startTime;
// 이전 시간
ULONGLONG previousTime;
// 현재 시간
ULONGLONG currentTime;
// 이전시간 - 현재시간
ULONGLONG deltaTime;


const ULONGLONG runningSpeed = 50;		// 0.01 초
const ULONGLONG BPM = 120;				// 대충 120 bpm 기준이면?
const ULONGLONG noteSpeed = 100;


/// <summary>
/// 시간 초기화
/// </summary>
void InitTime()
{
	// 델타타임 초기화 = 0 (누적시간 0)
	startTime = currentTime = previousTime = GetTickCount64();
	// 1ms 단위로 반환
}

void UpdateTime()
{
	currentTime = GetTickCount64();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;
	
}

//
//ULONGLONG GetDeltaTime()
//{
//	return deltaTime;								// 단위 : 1/1000 초
//}




///
/// 올라오는 노트 생성 및 출력 관련 함수
/// y 축 한줄 씩 -1 시키기
/// _UIMaxSize.Bottom -1
/// L(8) D(24) U(40) R(56)
void UpdateNotePosition_left(int i)
{
	if (l_note[i] == 1)
	{
		
		NotecurPos_l[i].Y --;
		//HitBox(NotecurPos_l[i].Y, i, LEFT);



		// 만약 화살표가 화면을 벗어나면 그만 출력시킨다
		// 판정 기능 구현 후
		// 판정 안에 타격하면 UImaxSize 에서 리턴, 미스는 콘솔사이즈에서 리턴
		if ( HitBox(NotecurPos_l[i].Y, i, LEFT) || NotecurPos_l[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawLeftArrow(NotecurPos_l[i], color_blue);
	}

	if (l_note[i] == 0)
	{
		/*if (GetKeyTable(LEFT))
		{
			SetKeyTable(LEFT, false);
		}*/
		return;
	}

	// 마지막 인덱스에 2를 넣어서 키 입력을 막아버린다
	else 
	{
		SetKeyTable(LEFT, false);
	}
	
		
}

void UpdateNotePosition_down(int i)
{
	if (d_note[i] == 1)
	{
		NotecurPos_d[i].Y--;
		
		if ( HitBox(NotecurPos_d[i].Y, i, DOWN) || NotecurPos_d[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawDownArrow(NotecurPos_d[i], color_green);
	}

	if (d_note[i] == 0)
		return;

	else
	{
		SetKeyTable(DOWN, false);
	}

}

void UpdateNotePosition_up(int i)
{
	if (u_note[i] == 1)
	{
		NotecurPos_u[i].Y--;


		if ( HitBox(NotecurPos_u[i].Y, i, UP) || NotecurPos_u[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawUpArrow(NotecurPos_u[i], color_red);
	}

	if (u_note[i] == 0)
		return;


	else
	{
		SetKeyTable(UP, false);
	}

}


void UpdateNotePosition_right(int i)
{
	if (r_note[i] == 1)
	{
		NotecurPos_r[i].Y--;

		
		if ( HitBox(NotecurPos_r[i].Y, i, RIGHT) || NotecurPos_r[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawRightArrow(NotecurPos_r[i], color_dark_yellow);
	}

	if (r_note[i] == 0)
		return;


	else
	{
		SetKeyTable(RIGHT, false);
	}


}




/// <summary>
///  ㅋㅋ 노트 인덱스 전역변수 때리기
/// </summary>
int note_Index = 0;


void GenerateNote()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	ULONGLONG barTime = 60000 / BPM * 4;
	ULONGLONG noteInterval = barTime / 16;

	
	

	for (note_Index = 0; note_Index < noteCount; note_Index++)
	{
		
		if (elapsedTime >= noteInterval * note_Index)
		{
			UpdateNotePosition_left(note_Index);
			UpdateNotePosition_down(note_Index);
			UpdateNotePosition_up(note_Index);
			UpdateNotePosition_right(note_Index);
			
		}
		
	}
	
}


//void UpdateNote()
//{
//	static ULONGLONG elapsedTime;
//	elapsedTime += deltaTime;
//
//	if (elapsedTime >= noteSpeed)
//	{
//		GenerateNote();
//		elapsedTime -= noteSpeed;
//	}
//
//}



// 아스키 아트
char** asciiArt[3];
//char** copyAnim[5];




/// 렌더 (직접적인 출력 부분) 뒷 버퍼 기준
void UpdateRender()
{
	int anim1_frame = 10;
	int anim2_frame = 10;
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;
	
	static int i = 0;
	static int j = 0;
	
	if (elapsedTime >= runningSpeed)
	{
		// 이전 출력 내용을 지운다
		ScreenClear();

		// 새로 출력할 내용을 작성한다
		ScreenDrawKeyInterface();
		GenerateNote();
		//UpdateNote();
		FindAsciiArt(asciiArtFilePath1, 10, 0);
		FindAsciiArt(asciiArtFilePath2, 10, 2);
		PrintAsciiArt(asciiArt[2], i, anim1_frame, 120, 15, color_dark_white);
		PrintAsciiArt(asciiArt[0], j, anim2_frame, 90, 13, color_dark_white);

		//CopyPrintAsciiArt(1, i, anim1_frame, 120, 15, color_dark_white);
		//CopyPrintAsciiArt(0, j, anim2_frame, 90, 13, color_dark_white);

		// 앞 뒤 버퍼를 뒤집는다
		ScreenFlipping();

		elapsedTime -= runningSpeed;
		i++;
		j++;
	}
	if (i == anim1_frame)
	{
		i = 0;
	}
	if (j == anim2_frame)
	{
		j = 0;
	}
	
}

/// 렌더 (직접적인 출력 부분) 뒷 버퍼 기준
void UpdateRender_Song1()
{
	int anim1_frame = 4;
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	static int i = 0;

	if (elapsedTime >= runningSpeed)
	{
		// 이전 출력 내용을 지운다
		ScreenClear();

		// 새로 출력할 내용을 작성한다
		ScreenDrawKeyInterface();
		GenerateNote();
		//UpdateNote();
		FindAsciiArt("bunny_", anim1_frame, 3);
		PrintAsciiArt(asciiArt[3], i, anim1_frame, 100, 10, color_white);

		//CopyPrintAsciiArt(1, i, anim1_frame, 120, 15, color_dark_white);
		//CopyPrintAsciiArt(0, j, anim2_frame, 90, 13, color_dark_white);

		// 앞 뒤 버퍼를 뒤집는다
		ScreenFlipping();

		elapsedTime -= runningSpeed;
		i++;

	}
	if (i == anim1_frame)
	{
		i = 0;
	}


}


///
/// 애니메이션 출력
/// 

// 파일 경로, 해당 애니메이션을 출력하는데에 필요한 파일의 개수, 몇번째 아스키 아트인지
void FindAsciiArt(const char* asciiArtFilePath, int fileNum, int asciiNum)
{
	FILE* fp;
	char buffer[256];
	int line = 0;
	int index = 0;
	asciiArt[asciiNum] = (char**)malloc(sizeof(char*) * fileNum);


	// n 개의 아스키 아트 파일 읽기
	for (int i = 0; i < fileNum; i++)
	{
		asciiArt[asciiNum][i] = (char*)malloc(sizeof(char) * 5000);
		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
		errno_t err = fopen_s(&fp, buffer, "r");
		if (err != 0) {
			//printf("아스키 아트 파일을 찾을 수 없습니다.");
			exit(1);
		}
		int readSize = fread(asciiArt[asciiNum][i], sizeof(char), 5000, fp);
		asciiArt[asciiNum][i][readSize] = '\0'; // 마지막에 널 문자 추가 > 이거 걍 파일 하나 당의 아스키 아트 문자 개수
		fclose(fp);
	}

}

size_t starlen(const char* str)
{
	const char* s;
	for (s = str; *s; ++s);
	return(s - str);
}


// 파일 경로, 해당 애니메이션을 출력하는데에 필요한 파일의 개수, 몇번째 아스키 아트인지
//void CopyFindAsciiArt(const char* asciiArtFilePath, int fileNum, int asciiNum)
//{
//	FILE* fp;
//	char buffer[256];
//	//int line = 0;
//	//int index = 0;
//	char** asciiArt;
//	asciiArt= (char**)malloc(sizeof(char*) * fileNum);
//	copyAnim[asciiNum] = (char**)malloc(sizeof(char*) * fileNum);
//
//
//	// n 개의 아스키 아트 파일 읽기
//	for (int i = 0; i < fileNum; i++)
//	{
//		asciiArt[i] = (char*)malloc(sizeof(char) * 3000);
//		copyAnim[asciiNum][i] = (char*)malloc(sizeof(char) * 3000);
//		
//
//		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
//		// fopen 함수는 파일 읽기를 성공하면 0을 반환한다. 때문에 0 이 아니면 파일을 읽지 못한 경우임
//		errno_t err = fopen_s(&fp, buffer, "r");
//		if (err != 0) {
//			//printf("아스키 아트 파일을 찾을 수 없습니다.");
//			exit(1);
//		}
//		
//
//		
//	}
//
//	// 동적할당 해제
//	for (int i = 0; i < fileNum; i++)
//	{
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//}






/// 파일 경로, 해당 애니메이션을 출력하는데에 필요한 파일의 개수, 몇번째 아스키 아트인지
//void CopyFindAsciiArt1(const char* asciiArtFilePath, int fileNum, int asciiNum, int posx, int posy)
//{
//	FILE* fp;
//	char buffer[256];
//	//int line = 0;
//	//int index = 0;
//	char** asciiArt;
//	asciiArt = (char**)malloc(sizeof(char*) * fileNum);
//	copyAnim[asciiNum] = (char**)malloc(sizeof(char*) * fileNum);
//
//
//	char line[3000];
//	int readSize = 0;
//
//	// n 개의 아스키 아트 파일 읽기
//	for (int i = 0; i < fileNum; i++)
//	{
//		int index = 0;
//		asciiArt[i] = (char*)malloc(sizeof(char) * 3000);
//		copyAnim[asciiNum][i] = (char*)malloc(sizeof(char) * 3000);
//
//
//		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
//		// fopen 함수는 파일 읽기를 성공하면 0을 반환한다. 때문에 0 이 아니면 파일을 읽지 못한 경우임
//		errno_t err = fopen_s(&fp, buffer, "r");
//		if (err != 0) {
//			//printf("아스키 아트 파일을 찾을 수 없습니다.");
//			exit(1);
//		}
//
//
//		if (fp != NULL)
//		{
//			while (fgets(line, sizeof(line), fp) != NULL)
//			{
//				strcat_s(asciiArt[i], 3000, line);
//			}
//
//		}
//
//		//int readSize = fread(asciiArt[i], sizeof(char), 3000, fp);
//		//// 마지막에 널 문자 추가 > 이거 걍 파일 하나 당의 아스키 아트 문자 개수
//		//asciiArt[i][readSize] = '\0'; 
//		//int length = starlen(asciiArt[i]);
//		//fclose(fp);
//
//		//memcpy(copyAnim[asciiNum][i], asciiArt[i], sizeof(char) * (length + 1));
//		
//		
//		
//
//
//		// 마지막에 널 문자 추가 > 이거 걍 파일 하나 당의 아스키 아트 문자 개수
//		asciiArt[i][starlen(asciiArt[i])] = '\0';
//		int length = starlen(asciiArt[i]);
//
//		memcpy(copyAnim[asciiNum][i], asciiArt[i], sizeof(char) * (length + 1));
//		fclose(fp);
//
//	}
//
//	// 동적할당 해제
//	for (int i = 0; i < fileNum; i++)
//	{
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//	/*int x = posx;
//	int y = posy;*/   // 현재 출력 위치 저장
//
//
//	// i = 0 부터 i = 3 은 파일의 인덱스를 의미
//	//for (int i = 0; i < fileNum; i++)
//	//{
//	//	int line = 0;
//	//	int index = 0;
//
//
//	//	// while 문은 readSize 로 읽어온 아스키 문자수의 마지막에 추가한 널문자를 만나기 전까지 돌린다
//	//	while (copyAnim[asciiNum][index] != '\0')
//	//	{
//	//		setColor(color_black, color_white);
//	//		ScreenPrint(x, y, (char)copyAnim[asciiNum][index++], 1);
//	//		x++;
//	//		if (copyAnim[asciiNum][index] == '\n')
//	//		{
//	//			x = posx;
//	//			index++;
//	//			y++;   // 다음 줄로 이동할 때마다 증가
//	//			ScreenPrint(x, y, NULL, 0);
//
//	//		}
//	//	}
//	//}
//
//
//}


// 버퍼에 저장된 아스키 아트를 출력한다 , 아스키 아트의 종류, 파일 개수 세는 변수, 파일 개수, x,y 좌표
void PrintAsciiArt(char** asciiArt, int i, int n, int posx, int posy, int textcolor)
{

	int x = posx;
	int y = posy;   // 현재 출력 위치 저장


	// i = 0 부터 i = 3 은 파일의 인덱스를 의미
	//for (int i = 0; i < 1; i++)
	{
		int line = 0;
		int index = 0;
		
		
		// while 문은 readSize 로 읽어온 아스키 문자수의 마지막에 추가한 널문자를 만나기 전까지 돌린다
		while (asciiArt[i][index] != '\0')
		{
			setColor(color_black, textcolor);
			ScreenPrint(x , y, asciiArt[i][index++], 1);
			x++;
			if (asciiArt[i][index] == '\n')
			{
				x = posx;
				index++;
				y++;   // 다음 줄로 이동할 때마다 증가
				ScreenPrint(x, y, NULL, 0);
				
			}	
		}
	}

	// 동적할당 해제
	for (int i = 0; i < n; i++)
	{
		free(asciiArt[i]);
	}
	free(asciiArt);

}


// 버퍼에 저장된 아스키 아트를 출력한다 , 아스키 아트의 종류, 파일 개수 세는 변수, 파일 개수, x,y 좌표
//void CopyPrintAsciiArt( int i, int n, int posx, int posy, int textcolor)
//{
//
//	int x = posx;
//	int y = posy;   // 현재 출력 위치 저장
//
//
//	// i = 0 부터 i = 3 은 파일의 인덱스를 의미
//	//for (int i = 0; i < 1; i++)
//	{
//		int line = 0;
//		static int index = 0;
//		int c = 0;
//
//		// while 문은 readSize 로 읽어온 아스키 문자수의 마지막에 추가한 널문자를 만나기 전까지 돌린다
//		//while (copyAnim[i][index][c] != '\0')
//		while (true)
//		{
//			if (c == 1) break;
//			setColor(color_black, textcolor);
//			ScreenPrint(x, y, copyAnim[i][index][c], 1);
//			x++;
//			if (copyAnim[i][index][c] == '\n')
//			{
//				x = posx;
//				c++;
//				y++;   // 다음 줄로 이동할 때마다 증가
//				ScreenPrint(x, y, NULL, 0);
//
//			}
//		}
//
//		index = (index + 1) % n;
//	}
//
//
//}

















char* copy_temp[4];

/// <summary>
/// 동적할당 메모리 복사하는 함수
/// 혹시나 버벅버벅에 도움이 되나
/// 왜이럴까
/// </summary>
/// <param name="str"></param>
/// <param name="index"></param>
void Copy(const char* str, int index)
{
	char* print_temp;
	FILE* rfp;
	errno_t err = fopen_s(&rfp, str, "rt");
	print_temp = (char*)malloc(sizeof(char) * 3000);
	copy_temp[index] = (char*)malloc(sizeof(char) * 3000);


	// a 에 있는 값을 b로 복사한다
	//memcpy(b, a, n * sizeof(int));
	memcpy(copy_temp[index], print_temp, sizeof(print_temp));

	if (err != 0)
	{
		//printf("파일 불러오기에 실패했습니다.\n");
		exit(1);
	}

	// 마지막 문자에 널문자 추가
	int readSize = fread(copy_temp[index], sizeof(char), 3000, rfp);
	copy_temp[index][readSize] = '\0';
	fclose(rfp);

	free(print_temp);

}






///
/// 
/// 타이틀 메뉴 출력 함수
/// 좌표는 대충 (10,2)

// 타이틀 아스키를 출력하는 함수입니다.
void DrawTitle(int posx, int posy)
{
	//char* print_temp;
	//FILE* rfp;
	//errno_t err = fopen_s(&rfp, "FMR_title.txt", "rt");
	//print_temp = (char*)malloc(sizeof(char) * 3000);

	//
	//if (err != 0)
	//{
	//	//printf("파일 불러오기에 실패했습니다.\n");
	//	exit(1);
	//}
	//// 마지막 문자에 널문자 추가
	//int readSize = fread(print_temp, sizeof(char), 3000, rfp);
	//print_temp[readSize] = '\0';
	//fclose(rfp);

	


	int x = posx;
	int y = posy;
	static int color = color_blue;
	int index = 0;


	
	color = (color + 1) % 12 + 1;
	while (copy_temp[0][index] != '\0')
	{
		color = (color + 1) % 12 + 1;
		setColor(color_black, color);
		
		ScreenPrint(x, y, copy_temp[0][index++], 1);
		x++;
		if (copy_temp[0][index] == '\n')
		{
			x = posx;
			//index++;
			y++;   // 다음 줄로 이동할 때마다 증가
			//setColor(color_black, color);
			//ScreenPrint(x, y, NULL, 0);
		}
	}

	//free(print_temp);
	
}




/// 
/// y=32 ~ 40, 출력 컬러 지정
/// 아무키나 눌러 진행.. 같은걸 애니메이션으로 만들고 싶다
void DrawSubtitle(int posx, int posy, int printcolor, int backcolor)
{
	// 바탕용 변수임
	int x = posx;
	int y = posy;

	// 글씨용임
	int x2 = posx + 15;
	int y2 = posy + 2;


	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;


	/*char* print_temp;
	FILE* rfp;
	errno_t err = fopen_s(&rfp, "subTitle2.txt", "rt");
	print_temp = (char*)malloc(sizeof(char) * 5000);*/


	int index = 0;


	//if (err != 0)
	//{
	//	//printf("파일 불러오기에 실패했습니다.\n");
	//	exit(1);
	//}

	//// 마지막 문자에 널문자 추가
	//int readSize = fread(print_temp, sizeof(char), 5000, rfp);
	//print_temp[readSize] = '\0';
	//fclose(rfp);



	// 회색 바탕 출력 코드
	for (int j = 0; j <11; j++)
	{
		// 일단 화면 크기 확정된거 아니니까 화면 버퍼사이즈에 맞게 출력되도록 설정해놓음
		for (int i = 0; i < consoleScreenSize.Right - consoleScreenSize.Left + 1; i++)
		{
			setColor(backcolor, color_white);
			ScreenPrint(x, y, ' ', 1);
			x++;
		}
		y++;
	}



	// 글자를 깜박거리게 하고싶다

	// 글자 상태 변수
	static bool setPrint = true;
	
	if (setPrint == true)
	{
		while (copy_temp[1][index] != '\0')
		{
			// # 문자를 공백으로 출력하는게 이쁜거같다
			char c = copy_temp[1][index];

			// # 인경우
			if (c == '#')
			{
				setColor(printcolor, color_black);
				ScreenPrint(x2, y2, ' ', 1);
				x2++;
			}
			//setColor(color_gray, color_black);

			// 줄바꿈이면 y 축 변경, x 축은 원래 포지션으로 이동
			if (c == '\n')
			{
				x2 = posx + 12;
				y2++;   // 다음 줄로 이동할 때마다 증가
				setColor(backcolor, color_black);
				ScreenPrint(x2, y2, ' ', 1);
			}

			// 기존 파일의 공백을 만나면 색바꿔서 아무것도 안보이게
			if (c == ' ')
			{
				setColor(backcolor, color_black);
				ScreenPrint(x2, y2, c, 1);
				x2++;
			}
			index++;
		}


	}
	// 대충 1 초 간격으로 안보임 ㅎㅎ
	if (elapsedTime >= 1000)
	{
		setPrint = false;

		elapsedTime -= 1000;
	}
	else
	{
		setPrint = true;
	}
	


	//free(print_temp);

}


/// <summary>
/// 타이틀에 출력될 모든 함수를 더블버퍼링 렌더
/// </summary>
void TitleRender(bool isPlaying)
{
	if (isPlaying == false)
	{
		return;
	}


	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	if (elapsedTime >= runningSpeed)
	{
		ScreenClear();




		DrawTitle(20, 3);
		DrawSubtitle(0, 35, color_dark_yellow, color_black);


		ScreenFlipping();

		elapsedTime -= runningSpeed;
	}

	

}



///
/// 곡 선택 메뉴를 그리자
/// 
/// 해당 곡으로 커서가 이동하면
/// 1. 멈춰있던 그 곡에 대한 아스키 아트 애니메이션이 움직이고
/// 2. 해당 곡이 플레이되게..!
/// 하입보이 팬클럽 하드모드 하나 더
/// 


int GetSelectedMenu()
{
	// 1~3 번 곡중 어떤 곡인지 확인하는 변수
	static int menunumber = 1;

	// 입력 값을 처리하는 부분..
	{

		if (menunumber < 1)
		{
			menunumber = 3;
		}
		if (menunumber > 3)
		{
			menunumber = 1;
		}


		if (GetKeyTable(RIGHT))
		{
			SetKeyTable(RIGHT, false);
			menunumber++;
		}
		if (GetKeyTable(LEFT))
		{
			SetKeyTable(LEFT, false);
			menunumber--;
		}
		if (GetKeyTable(SPACE))
		{
			//SetKeyTable(SPACE, false);
			return menunumber;
		}


	}

	// 여기서 몇번곡을 선택했는가를 받아간다
	return menunumber;
}







/// 만약 해당 메뉴바에 커서를 갖다대면 글자색이 바뀜 + (노래나올 예정)
void DrawMenuList(int menunumber)
{

	static int menucolor1 = color_white;
	static int menucolor2 = color_white;
	static int menucolor3 = color_white;


	if (menunumber == 1)
	{
		menucolor1 = color_dark_yellow;
	}
	else
	{
		menucolor1 = color_white;

	}
	if (menunumber == 2)
	{
		menucolor2 = color_dark_yellow;
	}
	else
	{
		menucolor2 = color_white;

	}
	if (menunumber == 3)
	{
		menucolor3 = color_dark_yellow;

	}
	else
	{
		menucolor3 = color_white;

	}
	

	setColor(color_black, menucolor1);
	ScreenDraw(15, 38, "< H Y P E  B O Y > - N E W  J E A N S");

	setColor(color_black, menucolor2);
	ScreenDraw(65, 38, "< 이 게 바 로  사 랑 일 까 ? > - 리 듬 세 상");

	setColor(color_black, menucolor3);
	ScreenDraw(125, 38, " 준 비 중 . . .");


}










///
/// 어떤 곡 선택했는지 리턴값으로 반환
void DrawMenu()
{

	// 메뉴를 활성화하는 변수
	static bool isPlaying = false;

	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;
	
	int menunumber = GetSelectedMenu();
	
	//isPlaying = true;

	

	if (elapsedTime >= runningSpeed)
	{

		ScreenClear();

		GetSelectedMenu();
		PlayAnim1(menunumber, 10, 3);
		PlayAnim2(menunumber, 75, 3);
		PlayAnim3(menunumber, 120, 10);
		DrawMenuList(menunumber);


		int combo = GetSelectedMenu();
		char convertCombo[20] = { 0 };
		snprintf(convertCombo, sizeof(convertCombo), "%d", combo);
		const char* constCombo = convertCombo;


		setColor(color_yellow, color_blue);
		ScreenDraw(130, 5, constCombo);


		ScreenFlipping();


		elapsedTime -= runningSpeed;
	}








}





















///
/// 팬클럽 애니 재생
/// bool 변수는 애니를 재생하는가에 대한 변수, x y 는 좌표
void PlayAnim2(int menunumber, int posx, int posy)
{
	int anim_frame = 10;

	static int i = 0;

	FindAsciiArt("ascii_art_", anim_frame, 0);
	

	if (menunumber == 2)
	{
		PrintAsciiArt(asciiArt[0], i, anim_frame, posx, posy, color_yellow);
		//CopyPrintAsciiArt(0, anim_frame, posx, posy, color_yellow);
		i++;
	}
	else
	{
		//CopyPrintAsciiArt(0, anim_frame, posx, posy, color_dark_white);
		PrintAsciiArt(asciiArt[0], i, anim_frame, posx, posy, color_dark_white);
	}

	if (i == anim_frame)
	{
		i = 0;
	}

}


/// 버니
void PlayAnim1(int menunumber, int posx, int posy)
{
	int anim_frame = 4;

	static int i = 0;

	FindAsciiArt("bunny_", anim_frame, 1);


	if (menunumber == 1)
	{
		PrintAsciiArt(asciiArt[1], i, anim_frame, posx, posy, color_yellow);
		//CopyPrintAsciiArt(2, anim_frame, posx, posy, color_yellow);
		i++;
	}
	else
	{
		//CopyPrintAsciiArt(2, anim_frame, posx, posy, color_white);
		PrintAsciiArt(asciiArt[1], i, anim_frame, posx, posy, color_white);
	}

	if (i == anim_frame)
	{
		i = 0;
	}

}


void PlayAnim3(int menunumber, int posx, int posy)
{
	int x = posx;
	int y = posy;
	//const char* str = "                                 ";


	if (menunumber == 3)
	{
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "     개 발 팀 : 레 드 벨 벳    ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "          게  임  명           ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, " < F E E L  M Y  R H Y T H M > ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");
	}
	


}

















///
/// 04.20
/// 판정 함수
/// y 값의 특정 구간에 입력을 받았는가,, 를 판정한다
/// 판정 구간 내에 있는 내내 콤보가 올라가고..
/// 판정 구간 밖에 있는 내내 미스 카운트가 올라간다..
/// 따로 bool 변수로 제어할 필요가 있다
/// is Good, is Miss
/// 



/// <summary>
/// 판정
/// </summary>
/// <param name="y">판정의 기준이 되는 y값의 위치</param>
/// <param name="i">노트 배열의 index</param>
/// <param name="key">상하좌우 중 무슨 키인지</param>
/// <returns>isGood 에 대한 반환값</returns>
bool HitBox(int y, int i, int key)
{
	//LEFT = 1;
	//RIGHT = 2;
	//UP = 3;
	//DOWN = 4;

	/// 맞춤 판정 변수 나중에 노트 개수랑 맞춰주기
	// 4 는 좌우상하 키 개수, 100 은 노트 개수
	static bool isGood[4][100];
	static bool isMiss[4][100];	
	
	
	if (y >= 2 && y <= 5)
	{
		if (GetKeyTable(key)&&!isGood[key][i])
		{
			SetKeyTable(key, false);
			isGood[key][i] = true;
			setColor(color_green, color_red);
			ScreenPrint(100, 2, 'DOOG', 4);
			combo++;
		}
		/*if (isGood)
		{
			SetKeyTable(LEFT, false);
			setColor(color_green, color_red);
			ScreenPrint(100, 2, 'GOOD', 4);
		}*/

		else
		{
			SetKeyTable(key, false);
		}
	}

	else if (y < 2 && !isGood[key][i] && !isMiss[key][i])
	{
		// 미스이펙트 넣고싶당...
		//ScreenClear1(color_white, color_dark_purple);
		//ScreenClear1(color_white, color_dark_purple);
		{
			missCount++;

			setColor(color_green, color_red);
			ScreenPrint(100, 3, 'SSiM', 4);
			isMiss[key][i] = true;
			isGood[key][i] = false;
		}
		
	}
	
	// 콤보, 미스 개수 확인 출력용 코드
	{
		char convertMiss[20] = { 0 };
		char convertCombo[20] = { 0 };

		snprintf(convertCombo, sizeof(convertCombo), "%d", combo);
		const char* constCombo = convertCombo;


		snprintf(convertMiss, sizeof(convertMiss), "%d", missCount);
		const char* constMiss = convertMiss;
		setColor(color_yellow, color_blue);

		ScreenPrint(90, 4, 'MOC', 6);
		ScreenPrint(90, 6, 'SSIM', 4);
		for (int i = 0; i < 5; i++)
		{
			ScreenPrint(100 + i, 4, convertCombo[i], 1);
			ScreenPrint(100 + i, 6, convertMiss[i], 1);
		}
	}
	
	
	// 판정 값 반환해서 노트 포지션 함수에서 노트 출력 지우는데에 쓰자
	return isGood[key][i];
}






///
/// 04.21
/// hp 게이지 & 점수 계산
/// 


void gLoop2()
{
	UpdateTime();
	UpdateInput();
	UpdateRender();

}



void gLoop1()
{
	UpdateTime();
	UpdateInput();
	UpdateRender_Song1();

}


void TitleLoop()
{
	UpdateTime();
	UpdateMenuInput();
	TitleRender(true);
}


void MenuLoop()
{
	UpdateTime();
	UpdateMenuInput();
	DrawMenu();
}


/// 번호에 따른 씬을 출력한다
void BG_Draw(int selectedNum)
{
	// define 으로
	// TITLE_SCENE 0
	// MENU_SCENE 1
	// INGAME_1_SCENE 2
	// INGAME_1_SCENE 3


	switch (selectedNum)
	{
		case 0:
			TitleLoop();
			break;

		case 1:
			MenuLoop();
			break;

		case 2:
			gLoop1();
			break;

		case 3:
			gLoop2();


		default:
			break;
	}

}





int main()
{
	/// 초기화
	// 콘솔 세팅
	initConsole();
	// 시간 초기화
	InitTime();

	// 오디오 시스템
	AudioSystem();
	// 오디오 생성
	AudioCreate("Assets/fanclub.mp3", &fanclub);
	AudioCreate("Assets/hihat.mp3", &sound2);
	AudioPlay(fanclub, channel);
	

	// 노트 위치 초기화
	SetNotePosition(45);
	// 타이틀에 출력할 아스키 아트 복사
	Copy("FMR_title.txt", 0);
	Copy("subTitle2.txt", 1);


	// 타이틀 출력 상태 조절 변수
	bool bTitle = true;

	// 선택지 저장 변수
	int selectedNum = GetSelectedMenu();


	// title & menu
	while (1)
	{
		// title & title sound
		UpdateTime();
		UpdateMenuInput();
		TitleRender(bTitle);

		if (GetKeyTable(SPACE))
		{
			SetKeyTable(SPACE, false);
			// 동적할당 해제
			free(copy_temp[0]);
			free(copy_temp[1]);
			break;
		}

	}

	// menu
	while (1)
	{

		UpdateTime();
		UpdateMenuInput();

		DrawMenu();
		if (GetKeyTable(SPACE))
		{
			selectedNum = GetSelectedMenu();
			SetKeyTable(SPACE, false);
			break;
		}

	}

	switch (selectedNum)
	{
	case 1:
		while (1)
		{
			
			gLoop1();

		}

		break;
	case 2:

		


		while (1)
		{
			
			gLoop2();

		}

		break;

	}
	
	
}