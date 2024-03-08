#define _CRT_SECURE_NO_WARNINGS

#define MAX_NOTE 6
#define ROW_SIZE 160
#define COLUMN_SIZE 50
#define MAX_LINES 5000


#include "input.h"
#include "main.h"
#include "audio.h"




///������� �ε��ְ� ���� ����ü�� �Լ�
typedef struct
{
	int x;
	double y;
	int time;
	int type;
	int hit_sound;
} HitObject;

// ���
int scrollSpeed = 1500;
int judgeLineY = 2;
int yHeight = 50;


// ����
HitObject hitobjects[3][MAX_LINES];
int num_hitobjects = 0;
int num_hitobjects2 = 0;
int num_hitobjects3 = 0;

int startNote = 0;


char* filename_fanclub = "fanclub.osu";
char* filename_hypeboy = "Hype Boy.osu";
char* filename_kamui = "Kamui.osu";
char* filename_delarue = "DeLaRue.osu";

FILE* fp1;
FILE* fp2;
FILE* fp3;


char buffer[1024];
char buffer2[1024];
char buffer3[1024];

char* token;








// ����
// 
// Ÿ��Ʋ ��� ���� ���� ����
bool bTitle = true;
bool bMenu = true;
bool bPlaySound = true;
int selectedNum = 0;






///
/// 04. 10
/// Ŀ�� �ޱ� gotoxy, initConsole ��
/// â ������ ����
/// ***���� ���۸� ã�ƺ���***
/// 






// ȭ�� ���۰� �޾ƿ� ����ü (�簢������ ���� �����Ѵٰ� ��)
SMALL_RECT consoleScreenSize;
// UI �ִ� ������ ������
SMALL_RECT _UImaxSize;
// ����(ȭ��) 2�� ����
HANDLE hScreen[2];
bool bScreenIndex;

// ����׿� ��ġ ��� ����
int g_ms_ForDebugA[255] = { 0, };
int g_ms_ForDebugAd[255] = { 0, };

int g_ms_ForDebugB[255] = { 0, };
int g_ms_ForDebugBd[255] = { 0, };

int g_ms_ForDebug2[255] = { 0, };
int g_ms_ForDebug2d[255] = { 0, };

ULONGLONG g_ms_ForDebug_OriginalElaspedTime[10000] = { 0, };



// song1 �� elapsed Time
double g_elapsedTimeA = 0.f;
double g_elapsedTimeB = 0.f;

double g_elapsedTime_Note = 0.f;

double g_deltaTime = 0.f;


/// <summary>
/// �ʱ�ȭ
/// ���� �ޱ� -> ���߿� �ܼ�â ũ�� ���� �����ϱ�
/// </summary>
void initConsole()
{
	system("mode con cols=160 lines=50 | title FEEL_THE_RHYTHM");
	// ���� ���۸�.. ���� �ΰ� �����
	hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ�� ���ܹ����� (�ȱ�����! �Ⱥ���!)
	HANDLE consonleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO consoleCursor = { 0, };
	consoleCursor.bVisible = 0;											// 0 �̸� Ŀ���� ����, 0 �� �ƴ� ��쿡 ���δ�
	consoleCursor.dwSize = 1;											// ���� ���� Ŀ���� ũ�⸦ �����Ѵ�

	// Ŀ�������� �������� �� ȭ�鿡 �Է�
	SetConsoleCursorInfo(consonleHandle, &consoleCursor);
	SetConsoleCursorInfo(hScreen[0], &consoleCursor);
	SetConsoleCursorInfo(hScreen[1], &consoleCursor);


	// ȭ�� ������ �޾ƿ��� (���۰� �޾Ƽ� ScreenSize �� ����)
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


	// ������ �ʱ�ȭ
	g_elapsedTimeA = 0.f;
	g_elapsedTimeB = 0.f;

	g_elapsedTime_Note = 0.f;
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


/// ���� �ΰ��� ��ȯ���ִ� �Լ�
void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(GetScreenHandle());
	bScreenIndex = !bScreenIndex;
}



/// ȭ���� ���� �Լ�. ���� ���۷� ��ȯ�� �� ���� ��³����� �������Ѵ�
void ScreenClear()
{
	COORD coordScreen = { 0, 0 };
	DWORD dwConSize = 8000;
	DWORD dwWrittenCount = 0;

	// �ܼ� â�� �������� ä���
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwWrittenCount);

	// ���� �Ӽ��� �����Ͽ� �ܼ� â�� �����
	WORD wColors = ((WORD)0 << 4) | (WORD)15; // ��� ���ڻ�, ���� ����
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwWrittenCount);
}

void ScreenClear1(int text_color, int back_color)
{
	COORD coordScreen = { 0, 0 };
	DWORD dwConSize;


	// �ܼ� â�� �������� ä���
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwConSize);

	// ���� �Ӽ��� �����Ͽ� �ܼ� â�� �����
	WORD wColors = ((WORD)back_color << 4) | (WORD)text_color; // ��� ���ڻ�, ���� ����
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwConSize);
}

//void ScreenClear()
//{
//	COORD coor = { 0,0 };
//	DWORD dw;
//
//	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dw, coor, &dw);
//	WORD wColors = ((WORD)0 << 4) | (WORD)15; // ��� ���ڻ�, ���� ����
//	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dw, coor, &dw);
//		//���� ���� ȭ�� ���ۿ��� ������ ���� �� �̻����� Ȯ��Ǹ� �����࿡ ��ϵȴ�(�ڵ����� ���������� �Ѿ�µ�)
//		//���ۺ��� ���� ���� ū ���� ������ �� ������ ��ϵȴ�
//		//�ۼ��� ��ġ�� Ư�� ���� ������� �ʴ´�(? ? ) �� ������ �ȵǴ°ǰ�
//	FillConsoleOutputCharacter(GetScreenHandle(), 'a', 180 * 60, coor, &dw);
//
//
//	FillConsoleOutputCharacter(GetScreenHandle(), ' ', 180 * 60, coor, &dw);
//
//	 ////[�����ٿ� ���� ���� ä���] �� ������ ������ŭ ����
//		//for (int i = 0; i < COLUMN_SIZE; i++)
//		//{
//		//	coor.Y = i;
//		//	//������ ȭ��ũ�⸸ŭ �Է��������;;
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


/// ���ۿ� �׸��׸��� (���� �� ĭ ���) 
void ScreenPrint(int x, int y, const char* str, int length)
{
	DWORD dw;	// unsigned long ����ü
	COORD CursorPosition = { x, y };
	


	SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
	// 2 �� ���ڿ� ����

	



	WriteFile(GetScreenHandle(), &str, length, &dw, NULL);
}


///
/// �ٽ� ����
/// �� ���ڿ� ��¿�..gg
void ScreenDraw(int x, int y, const char* str)
{
	DWORD dw;
	COORD Cur = { x, y };

	SetConsoleCursorPosition(GetScreenHandle(), Cur);

	// ���ڿ��� ���̸� for ������ ����Ͽ� ���
	int strLen = 0;
	while (str[strLen] != '\0')
	{
		strLen++;
	}

	

	WriteFile(GetScreenHandle(), str, strLen, &dw, NULL);
}








/// ���� �ݱ�
void ScreenRelease()
{
	CloseHandle(hScreen[0]);
	CloseHandle(hScreen[1]);
}







/// <summary>
/// Ŀ�� ��ġ �̵��ϴ� �Լ�
/// y ��ǥ�� �Ʒ��� ������ ����, ������ ���� ���
/// </summary>
/// <param name="x">�̵��� x��ǥ</param>
/// <param name="y">�̵��� y��ǥ</param>
void gotoXY(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}





///
/// ��Ʈ ī��Ʈ, ��Ʈ �ʱ���ġ �ʱ�ȭ
/// 

COORD NotecurPos_l[43];
COORD NotecurPos_d[43];
COORD NotecurPos_u[43];
COORD NotecurPos_r[43];
COORD NotePos[4][43];



void SetNotePosition(int y)
{
	noteCount = sizeof(l_note) / sizeof(int);
	for (int i = 0; i < noteCount; i++)
	{
		// test
		noteCheck[LEFT][i] = true;
		noteCheck[RIGHT][i] = true;
		noteCheck[UP][i] = true;
		noteCheck[DOWN][i] = true;

		NotecurPos_l[i].X = 8;
		NotecurPos_d[i].X = 24;
		NotecurPos_u[i].X = 40;
		NotecurPos_r[i].X = 56;

		NotecurPos_l[i].Y = y;
		NotecurPos_d[i].Y = y;
		NotecurPos_u[i].Y = y;
		NotecurPos_r[i].Y = y;

		NotePos[LEFT][i].X = 8;
		NotePos[RIGHT][i].X = 56;
		NotePos[UP][i].X = 40;
		NotePos[DOWN][i].X = 24;

		NotePos[LEFT][i].Y = y;
		NotePos[RIGHT][i].Y = y;
		NotePos[UP][i].Y = y;
		NotePos[DOWN][i].Y = y;
	}
}









///
/// 04. 10
/// ��ǲ ���� ������
/// ��ĥ�� �Ǵ� UI
/// ���� ��üȭ��
/// 







// ��� ��, �۲� �� ����
void setColor(int backColor, int textColor)
{
	//textColor = color_white;
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backColor << 4) + textColor);
	SetConsoleTextAttribute(GetScreenHandle(), (backColor << 4) + textColor);
}




/// <summary>
/// ȭ��ǥ ��� ���� (UP KEY)
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
/// �ʹ� �������ϰ� �پ��� ���(���߿� ���� �ö�� ��Ʈ ��¿��� ����ϴ�)
/// ���� ���ຼ��
/// �����¿� ȭ��ǥ ��� �Լ��̴�
/// </summary>
/// <param name="pos">��ǥ ������ ����ִ� ����ü</param>
/// <param name="color">���� ������ ����ұ�?</param>
void DrawUpArrow(COORD pos, int color)
{

	// arr[i][j] up
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			// j �� ���� ��� (x), i �� ���� ����� ��� (y)
			// ���δ� ���κ��� �ι� ����ؾ���.. -> j*2
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


void ScreenDrawArrow(int keyNum, COORD pos)
{
	if (keyNum == LEFT)
	{
		ScreenDrawLeftArrow(pos, color_blue);
	}
	if (keyNum == RIGHT)
	{
		ScreenDrawRightArrow(pos, color_dark_yellow);
	}
	if (keyNum == UP)
	{
		ScreenDrawUpArrow(pos, color_red);
	}
	if (keyNum == DOWN)
	{
		ScreenDrawDownArrow(pos, color_green);
	}
}


/// 
/// �迭�� �������
/// 0 �̸� �ѱ�, 1�̸� ������ ����ϰ� ��� ���� ����
/// �������̽� �� Ű�� �׸���! Ű�� �ԷµǸ� ������ Ȱ��ȭ ��
void ScreenDrawKeyInterface()
{
	// �������̽� ��� ��ǥ ���� 
	// 5x5 ũ���� ȭ��ǥ. ����� ������ ���� ��� ��
	const int x = 8;
	const int y = 3;

	const int padding = 16;

	const COORD posLeft = { x + padding * 0, y };	// 8
	const COORD posDown = { x + padding * 1, y };	// 24
	const COORD posUp = { x + padding * 2, y };		// 40
	const COORD posRight = { x + padding * 3, y };	// 56

	/// �� ���� �⺻ ���´� ���
	// Up
	int colorUp = color_white;
	// Down
	int colorDown = color_white;
	// Left
	int colorLeft = color_white;
	// Right
	int colorRight = color_white;


	// Ű�� ������?
	if (GetKeyTable(UP))
	{
		// �ѹ��� �Է¹ޱ� ���� false ó��
		//SetKeyTable(UP, false);
		// ������ �������� �ٲ۴�
		colorUp = color_gray;
	}
	else
	{
		colorUp = color_white;
	}

	if (GetKeyTable(DOWN))
	{
		//SetKeyTable(DOWN, false);
		// ������ green���� �ٲ۴�
		colorDown = color_gray;
	}

	if (GetKeyTable(LEFT))
	{
		//SetKeyTable(LEFT, false);
		// ������ skyblue���� �ٲ۴�
		colorLeft = color_gray;
	}

	if (GetKeyTable(RIGHT))
	{
		//SetKeyTable(RIGHT, false);
		// ������ yellow���� �ٲ۴�
		colorRight = color_gray;
	}

	ScreenDrawUpArrow(posUp, colorUp);
	ScreenDrawRightArrow(posRight, colorRight);
	ScreenDrawDownArrow(posDown, colorDown);
	ScreenDrawLeftArrow(posLeft, colorLeft);

	/// �ٸ� �Լ��� ������
	{
		// ������ �迭�� �����ϰ� ȭ��ǥ ����� ����
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
	//		// j�� ���� ��� (x), i�� ���� ����� ��� (y)
	//		// ���δ� ���κ��� �ι� ����ؾ���.. -> j*2
	//		gotoXY(x + 2 * j, y + i);
	//		//if (arr[i][j] == 0)
	//		//{
	//		//	setColor(color_black);
	//		//	//printf("  ");
	//		//}	 ��� ����
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}

	// arr[i][j] left
	// y=x ��Ī (�������� y���� �Ʒ��� ����)
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
	// left�� x= 5 ��Ī (�������� ��ĭ������!!!!!) -> �׷��� (5-1)�� �ƴ϶� (10-2)
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
	// �迭�� �׻� 0��°���� ��µ��� �������!! (5 - 1) = 4
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
/// ��Ʈ ���, �̵�

///
/// ������ �ð����� ������Ʈ�� �ϵ��� ����
/// deltaTime 
///


// ���� �ð�
double startTime;
// ���� �ð�
double previousTime;
// ���� �ð�
double currentTime;
// �����ð� - ����ð�
double deltaTime;


const double runningSpeed = 75.f;		// 0.01 ��
const ULONGLONG BPM = 120;				// ���� 120 bpm �����̸�?
const ULONGLONG noteSpeed = 100;

int g_countU = 0;


/// <summary>
/// �ð� �ʱ�ȭ
/// </summary>
void InitTime()
{
	// ��ŸŸ�� �ʱ�ȭ = 0 (�����ð� 0)
	//startTime = currentTime = previousTime = GetTickCount64();
	// 1ms ������ ��ȯ

	startTime = currentTime = previousTime = (double)timeGetTime();
}

void UpdateTime()
{
	//currentTime = GetTickCount64();

	currentTime = (double)timeGetTime();

	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	//g_ms_ForDebug_OriginalElaspedTime[g_countU] = deltaTime;
	//g_countU++;
}

//
//ULONGLONG GetDeltaTime()
//{
//	return deltaTime;								// ���� : 1/1000 ��
//}


// �̹� ���ϵ� �迭�� ���Ͽ� ������� �����ϰ� �ʹ�
bool noteCheck[4][43];


///
/// �ö���� ��Ʈ ���� �� ��� ���� �Լ�
/// y �� ���� �� -1 ��Ű��
/// _UIMaxSize.Bottom -1
/// L(8) D(24) U(40) R(56)
void UpdateNotePosition_left(int i)
{
	if (l_note[i] == 1)
	{
		
		NotecurPos_l[i].Y --;
		//HitBox(NotecurPos_l[i].Y, i, LEFT);



		// ���� ȭ��ǥ�� ȭ���� ����� �׸� ��½�Ų��
		// ���� ��� ���� ��
		// ���� �ȿ� Ÿ���ϸ� UImaxSize ���� ����, �̽��� �ֻܼ������ ����
		if ( HitBox(NotecurPos_l[i].Y, i, LEFT) || NotecurPos_l[i].Y <= consoleScreenSize.Top)
		{
			noteCheck[LEFT][i] = false;
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
	// ������ �ε����� 2�� �־ Ű �Է��� ���ƹ�����
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
			noteCheck[DOWN][i] = false;
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
			noteCheck[UP][i] = false;
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
			noteCheck[RIGHT][i] = false;
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

void UpdateNotePosition(int keyNum, int i)
{
	if (note[keyNum][i] == 1)
	{
		NotePos[keyNum][i].Y--;


		if (HitBox(NotePos[keyNum][i].Y, i, keyNum) || NotePos[keyNum][i].Y <= consoleScreenSize.Top)
		{
			noteCheck[keyNum][i] = false;
			return;
		}
		ScreenDrawArrow(keyNum, NotePos[keyNum][i]);
	}

	if (note[keyNum][i] == 0)
		return;

	else
	{
		SetKeyTable(keyNum, false);
	}

}




/// <summary>
///  ��Ʈ ���� �κ�
/// </summary>


// 80 �� 
void GenerateNote()
{
	g_elapsedTime_Note += deltaTime;

	/// ��ü ���� ���� �־��� ���ΰ�?
	// ī���ÿ� ����ƽ
	static int _count_Note = 0;

	//g_ms_ForDebug2[_count_Note] = g_elapsedTime_Note;
	//g_ms_ForDebug2d[_count_Note] = deltaTime;
	//_count_Note++;
	
	ULONGLONG barTime = 60000 / BPM * 4;			// bpm = 120
	ULONGLONG noteInterval = barTime / 16;			// interval = 125

	int note_Index;

	// ������ ��Ʈ�� ���ؼ��� �˻縦 ���߰� ���� -> noteCheck
	for (note_Index = 0; note_Index < noteCount; note_Index++)
	{
		if ((g_elapsedTime_Note >= (double)noteInterval * note_Index / 10.f))
		{
			// ��Ʈ ������ �Լ� ���� & ��Ʈ �迭 ������ �迭�� �ٲٱ�
			/*UpdateNotePosition_left(note_Index);
			UpdateNotePosition_down(note_Index);
			UpdateNotePosition_up(note_Index);
			UpdateNotePosition_right(note_Index);*/

			if (noteCheck[LEFT][note_Index] == true)
			{
				UpdateNotePosition(LEFT, note_Index);
			}
			if (noteCheck[RIGHT][note_Index] == true)
			{
				UpdateNotePosition(RIGHT, note_Index);
			}
			if (noteCheck[UP][note_Index] == true)
			{
				UpdateNotePosition(UP, note_Index);
			}
			if (noteCheck[DOWN][note_Index] == true)
			{
				UpdateNotePosition(DOWN, note_Index);
			}
		}
		
	}
}


// �ƽ�Ű ��Ʈ ���� 3
char** asciiArt[5];
//char** copyAnim[5];


/// ���� - ��Ŭ�� (�������� ��� �κ�) �� ���� ����
void UpdateRender_Song2()
{
	int anim1_frame = 7;
	int anim2_frame = 10;

	g_elapsedTimeB += deltaTime;
	
	// �ִϸ��̼� ������ ���� ����
	static int i = 0;
	static int j = 0;

	// ī���ÿ� ����ƽ
	static int _countB = 0;
	
	if (g_elapsedTimeB >= runningSpeed)
	{
		/// ��ü ���� ���� �־��� ���ΰ�?
		//g_ms_ForDebugB[_countB] = g_elapsedTimeB;
		//g_ms_ForDebugBd[_countB] = deltaTime;

		//_countB++;

		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GameLogic(1, 61000, num_hitobjects2);

		//GenerateNote();
		//UpdateNote();
		FindAsciiArt(asciiArtFilePath1, 10, 0);
		FindAsciiArt(asciiArtFilePath2, 7, 2);
		PrintAsciiArt(asciiArt[2], i, anim1_frame, 120, 15, color_dark_white);
		PrintAsciiArt(asciiArt[0], j, anim2_frame, 90, 13, color_dark_white);
		PrintCombo(100, 4, color_yellow);
		//CopyPrintAsciiArt(1, i, anim1_frame, 120, 15, color_dark_white);
		//CopyPrintAsciiArt(0, j, anim2_frame, 90, 13, color_dark_white);

		// �� �� ���۸� �����´�
		ScreenFlipping();

		g_elapsedTimeB -= runningSpeed;
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



/// ���� - �������� ���� (�������� ��� �κ�) �� ���� ����
void UpdateRender_Song1()
{
	int anim1_frame = 7;

	g_elapsedTimeA += deltaTime;

	// �ִ� ������ ���� ����
	static int i = 0;

	// ī���ÿ� ����ƽ
	static int _countA = 0;

	if (g_elapsedTimeA >= runningSpeed)
	{
		/// ��ü ���� ���� �־��� ���ΰ�?
		//g_ms_ForDebugA[_countA] = g_elapsedTimeA;
		//g_ms_ForDebugAd[_countA] = deltaTime;

		//_countA++;

		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GameLogic(0, 91800, num_hitobjects);

		FindAsciiArt("bunny_", anim1_frame, 3);
		PrintAsciiArt(asciiArt[3], i, anim1_frame, 100, 12, color_white);
		PrintCombo(100, 4, color_yellow);

		// �� �� ���۸� �����´�
		ScreenFlipping();

		g_elapsedTimeA -= runningSpeed;
		i++;

	}
	if (i == anim1_frame)
	{
		i = 0;
	}

}


/// ���� - ����� ?
void UpdateRender_Song3()
{
	int anim1_frame = 8;

	g_elapsedTimeA += deltaTime;

	// �ִ� ������ ���� ����
	static int i = 0;

	// ī���ÿ� ����ƽ
	static int _countA = 0;

	if (g_elapsedTimeA >= runningSpeed)
	{
		/// ��ü ���� ���� �־��� ���ΰ�?
		//g_ms_ForDebugA[_countA] = g_elapsedTimeA;
		//g_ms_ForDebugAd[_countA] = deltaTime;

		//_countA++;

		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GameLogic(2, 131500, num_hitobjects3);

		FindAsciiArt("DeLaRue_", anim1_frame, 4);
		PrintAsciiArt(asciiArt[4], i, anim1_frame, 100, 14, color_white);
		PrintCombo(100, 4, color_yellow);

		// �� �� ���۸� �����´�
		ScreenFlipping();

		g_elapsedTimeA -= runningSpeed;
		i++;

	}
	if (i == anim1_frame)
	{
		i = 0;
	}

}



///
/// �ִϸ��̼� ���
/// 

// ���� ���, �ش� �ִϸ��̼��� ����ϴµ��� �ʿ��� ������ ����, ���° �ƽ�Ű ��Ʈ����
void FindAsciiArt(const char* asciiArtFilePath, int fileNum, int asciiNum)
{
	FILE* fp;
	char buffer[256];
	int line = 0;
	int index = 0;
	asciiArt[asciiNum] = (char**)malloc(sizeof(char*) * fileNum);


	// n ���� �ƽ�Ű ��Ʈ ���� �б�
	for (int i = 0; i < fileNum; i++)
	{
		asciiArt[asciiNum][i] = (char*)malloc(sizeof(char) * 5000);
		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
		errno_t err = fopen_s(&fp, buffer, "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(asciiArt[asciiNum][i], sizeof(char), 5000, fp);
		asciiArt[asciiNum][i][readSize] = '\0'; // �������� �� ���� �߰� > �̰� �� ���� �ϳ� ���� �ƽ�Ű ��Ʈ ���� ����
		fclose(fp);
	}

}

size_t starlen(const char* str)
{
	const char* s;
	for (s = str; *s; ++s);
	return(s - str);
}


// ���� ���, �ش� �ִϸ��̼��� ����ϴµ��� �ʿ��� ������ ����, ���° �ƽ�Ű ��Ʈ����
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
//	// n ���� �ƽ�Ű ��Ʈ ���� �б�
//	for (int i = 0; i < fileNum; i++)
//	{
//		asciiArt[i] = (char*)malloc(sizeof(char) * 3000);
//		copyAnim[asciiNum][i] = (char*)malloc(sizeof(char) * 3000);
//		
//
//		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
//		// fopen �Լ��� ���� �б⸦ �����ϸ� 0�� ��ȯ�Ѵ�. ������ 0 �� �ƴϸ� ������ ���� ���� �����
//		errno_t err = fopen_s(&fp, buffer, "r");
//		if (err != 0) {
//			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
//			exit(1);
//		}
//		
//
//		
//	}
//
//	// �����Ҵ� ����
//	for (int i = 0; i < fileNum; i++)
//	{
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//}






/// ���� ���, �ش� �ִϸ��̼��� ����ϴµ��� �ʿ��� ������ ����, ���° �ƽ�Ű ��Ʈ����
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
//	// n ���� �ƽ�Ű ��Ʈ ���� �б�
//	for (int i = 0; i < fileNum; i++)
//	{
//		int index = 0;
//		asciiArt[i] = (char*)malloc(sizeof(char) * 3000);
//		copyAnim[asciiNum][i] = (char*)malloc(sizeof(char) * 3000);
//
//
//		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
//		// fopen �Լ��� ���� �б⸦ �����ϸ� 0�� ��ȯ�Ѵ�. ������ 0 �� �ƴϸ� ������ ���� ���� �����
//		errno_t err = fopen_s(&fp, buffer, "r");
//		if (err != 0) {
//			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
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
//		//// �������� �� ���� �߰� > �̰� �� ���� �ϳ� ���� �ƽ�Ű ��Ʈ ���� ����
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
//		// �������� �� ���� �߰� > �̰� �� ���� �ϳ� ���� �ƽ�Ű ��Ʈ ���� ����
//		asciiArt[i][starlen(asciiArt[i])] = '\0';
//		int length = starlen(asciiArt[i]);
//
//		memcpy(copyAnim[asciiNum][i], asciiArt[i], sizeof(char) * (length + 1));
//		fclose(fp);
//
//	}
//
//	// �����Ҵ� ����
//	for (int i = 0; i < fileNum; i++)
//	{
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//	/*int x = posx;
//	int y = posy;*/   // ���� ��� ��ġ ����
//
//
//	// i = 0 ���� i = 3 �� ������ �ε����� �ǹ�
//	//for (int i = 0; i < fileNum; i++)
//	//{
//	//	int line = 0;
//	//	int index = 0;
//
//
//	//	// while ���� readSize �� �о�� �ƽ�Ű ���ڼ��� �������� �߰��� �ι��ڸ� ������ ������ ������
//	//	while (copyAnim[asciiNum][index] != '\0')
//	//	{
//	//		setColor(color_black, color_white);
//	//		ScreenPrint(x, y, (char)copyAnim[asciiNum][index++], 1);
//	//		x++;
//	//		if (copyAnim[asciiNum][index] == '\n')
//	//		{
//	//			x = posx;
//	//			index++;
//	//			y++;   // ���� �ٷ� �̵��� ������ ����
//	//			ScreenPrint(x, y, NULL, 0);
//
//	//		}
//	//	}
//	//}
//
//
//}


// ���ۿ� ����� �ƽ�Ű ��Ʈ�� ����Ѵ� , �ƽ�Ű ��Ʈ�� ����, ���� ���� ���� ����, ���� ����, x,y ��ǥ
void PrintAsciiArt(char** asciiArt, int i, int n, int posx, int posy, int textcolor)
{

	int x = posx;
	int y = posy;   // ���� ��� ��ġ ����


	// i = 0 ���� i = 3 �� ������ �ε����� �ǹ�
	//for (int i = 0; i < 1; i++)
	{
		int line = 0;
		int index = 0;
		
		
		// while ���� readSize �� �о�� �ƽ�Ű ���ڼ��� �������� �߰��� �ι��ڸ� ������ ������ ������
		while (asciiArt[i][index] != '\0')
		{
			setColor(color_black, textcolor);
			ScreenPrint(x , y, asciiArt[i][index++], 1);
			x++;
			if (asciiArt[i][index] == '\n')
			{
				x = posx;
				index++;
				y++;   // ���� �ٷ� �̵��� ������ ����
				ScreenPrint(x, y, NULL, 0);
				
			}	
		}
	}

	// �����Ҵ� ����
	for (int i = 0; i < n; i++)
	{
		free(asciiArt[i]);
	}
	free(asciiArt);

}


// ���ۿ� ����� �ƽ�Ű ��Ʈ�� ����Ѵ� , �ƽ�Ű ��Ʈ�� ����, ���� ���� ���� ����, ���� ����, x,y ��ǥ
//void CopyPrintAsciiArt( int i, int n, int posx, int posy, int textcolor)
//{
//
//	int x = posx;
//	int y = posy;   // ���� ��� ��ġ ����
//
//
//	// i = 0 ���� i = 3 �� ������ �ε����� �ǹ�
//	//for (int i = 0; i < 1; i++)
//	{
//		int line = 0;
//		static int index = 0;
//		int c = 0;
//
//		// while ���� readSize �� �о�� �ƽ�Ű ���ڼ��� �������� �߰��� �ι��ڸ� ������ ������ ������
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
//				y++;   // ���� �ٷ� �̵��� ������ ����
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
/// �����Ҵ� �޸� �����ϴ� �Լ�
/// Ȥ�ó� ���������� ������ �ǳ�
/// ���̷���
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


	// a �� �ִ� ���� b�� �����Ѵ�
	//memcpy(b, a, n * sizeof(int));
	memcpy(copy_temp[index], print_temp, sizeof(print_temp));

	if (err != 0)
	{
		//printf("���� �ҷ����⿡ �����߽��ϴ�.\n");
		exit(1);
	}

	// ������ ���ڿ� �ι��� �߰�
	int readSize = fread(copy_temp[index], sizeof(char), 3000, rfp);
	copy_temp[index][readSize] = '\0';
	fclose(rfp);

	free(print_temp);

}






///
/// 
/// Ÿ��Ʋ �޴� ��� �Լ�
/// ��ǥ�� ���� (10,2)

// Ÿ��Ʋ �ƽ�Ű�� ����ϴ� �Լ��Դϴ�.
void DrawTitle(int posx, int posy)
{
	//char* print_temp;
	//FILE* rfp;
	//errno_t err = fopen_s(&rfp, "FMR_title.txt", "rt");
	//print_temp = (char*)malloc(sizeof(char) * 3000);

	//
	//if (err != 0)
	//{
	//	//printf("���� �ҷ����⿡ �����߽��ϴ�.\n");
	//	exit(1);
	//}
	//// ������ ���ڿ� �ι��� �߰�
	//int readSize = fread(print_temp, sizeof(char), 3000, rfp);
	//print_temp[readSize] = '\0';
	//fclose(rfp);

	


	int x = posx;
	int y = posy;
	static int color = color_blue;
	int index = 0;

	FILE* fp2;
	char bufferf[5000];
	snprintf(bufferf, 5000, "FMR_title.txt");
	errno_t err = fopen_s(&fp2, bufferf, "r");
	if (err != 0) {
		//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
		exit(1);
	}
	int readSize = fread(bufferf, sizeof(char), 5000, fp2);

	bufferf[readSize] = '\0';
	fclose(fp2);

	color = (color + 1) % 12 + 1;
	while (bufferf[index] != '\0')
	{
		color = (color + 1) % 12 + 1;
		setColor(color_black, color);

		ScreenPrint(x, y, bufferf[index++], 1);
		x++;
		if (bufferf[index] == '\n')
		{
			x = posx;
			//index++;
			y++;   // ���� �ٷ� �̵��� ������ ����
			//setColor(color_black, color);
			//ScreenPrint(x, y, NULL, 0);
		}
	}
	
	//color = (color + 1) % 12 + 1;
	//while (copy_temp[0][index] != '\0')
	//{
	//	color = (color + 1) % 12 + 1;
	//	setColor(color_black, color);
	//	
	//	ScreenPrint(x, y, copy_temp[0][index++], 1);
	//	x++;
	//	if (copy_temp[0][index] == '\n')
	//	{
	//		x = posx;
	//		//index++;
	//		y++;   // ���� �ٷ� �̵��� ������ ����
	//		//setColor(color_black, color);
	//		//ScreenPrint(x, y, NULL, 0);
	//	}
	//}

	//free(print_temp);
	
}




/// 
/// y=32 ~ 40, ��� �÷� ����
/// �ƹ�Ű�� ���� ����.. ������ �ִϸ��̼����� ����� �ʹ�
void DrawSubtitle(int posx, int posy, int printcolor, int backcolor)
{
	// ������ ������
	int x = posx;
	int y = posy;

	// �۾�����
	int x2 = posx + 15;
	int y2 = posy + 2;


	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;


	char print_temp[5000];
	FILE* rfp;
	snprintf(print_temp, 5000, "subTitle2.txt");
	errno_t err = fopen_s(&rfp, print_temp, "rt");
	


	int index = 0;


	if (err != 0)
	{
		//printf("���� �ҷ����⿡ �����߽��ϴ�.\n");
		exit(1);
	}

	// ������ ���ڿ� �ι��� �߰�
	int readSize = fread(print_temp, sizeof(char), 5000, rfp);
	print_temp[readSize] = '\0';
	fclose(rfp);



	// ȸ�� ���� ��� �ڵ�
	//for (int j = 0; j <11; j++)
	//{
	//	// �ϴ� ȭ�� ũ�� Ȯ���Ȱ� �ƴϴϱ� ȭ�� ���ۻ���� �°� ��µǵ��� �����س���
	//	for (int i = 0; i < consoleScreenSize.Right - consoleScreenSize.Left + 1; i++)
	//	{
	//		setColor(backcolor, color_white);
	//		ScreenPrint(x, y, ' ', 1);
	//		x++;
	//	}
	//	y++;
	//}



	// ���ڸ� ���ڰŸ��� �ϰ�ʹ�

	// ���� ���� ����
	static bool setPrint = true;
	
	if (setPrint == true)
	{
		while (print_temp[index] != '\0')
		{
			// # ���ڸ� �������� ����ϴ°� �̻۰Ű���
			char c = print_temp[index];

			// # �ΰ��
			if (c == '#')
			{
				setColor(printcolor, color_dark_yellow);
				ScreenPrint(x2, y2, ' ', 1);
				//ScreenDraw(x2, y2, "#");
				x2++;
			}
			//setColor(color_gray, color_black);

			// �ٹٲ��̸� y �� ����, x ���� ���� ���������� �̵�
			if (c == '\n')
			{
				x2 = posx + 12;
				y2++;   // ���� �ٷ� �̵��� ������ ����
				setColor(backcolor, color_black);
				ScreenPrint(x2, y2, ' ', 1);
			}

			// ���� ������ ������ ������ ���ٲ㼭 �ƹ��͵� �Ⱥ��̰�
			if (c == ' ')
			{
				setColor(backcolor, color_black);
				ScreenPrint(x2, y2, c, 1);
				x2++;
			}
			index++;
		}


	}
	// ���� 1 �� �������� �Ⱥ��� ����
	if (elapsedTime >= 1000)
	{
		setPrint = false;

		//elapsedTime -= 1000;
		elapsedTime = 0;
	}
	else
	{
		setPrint = true;
	}

	//free(print_temp);

}


/// <summary>
/// Ÿ��Ʋ�� ��µ� ��� �Լ��� ������۸� ����
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
/// �� ���� �޴��� �׸���
/// 
/// �ش� ������ Ŀ���� �̵��ϸ�
/// 1. �����ִ� �� � ���� �ƽ�Ű ��Ʈ �ִϸ��̼��� �����̰�
/// 2. �ش� ���� �÷��̵ǰ�..!
/// ���Ժ��� ��Ŭ�� �ϵ��� �ϳ� ��
/// 


int GetSelectedMenu()
{
	// 1~3 �� ���� � ������ Ȯ���ϴ� ����
	static int menunumber = 1;

	// �Է� ���� ó���ϴ� �κ�..
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
			FMOD_Channel_Stop(channel2);
			bPlaySound = true;
			menunumber++;
		}
		if (GetKeyTable(LEFT))
		{
			SetKeyTable(LEFT, false);
			FMOD_Channel_Stop(channel2);
			bPlaySound = true;
			menunumber--;
		}
		if (GetKeyTable(SPACE))
		{
			//SetKeyTable(SPACE, false);
			return menunumber;
		}


	}

	// ���⼭ ������� �����ߴ°��� �޾ư���
	return menunumber;
}







/// ���� �ش� �޴��ٿ� Ŀ���� ���ٴ�� ���ڻ��� �ٲ� + (�뷡���� ����)
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
	ScreenDraw(8, 38, "< H Y P E  B O Y > - N E W  J E A N S");
	setColor(color_black, menucolor1);
	ScreenDraw(23, 40, " E A S Y ");

	setColor(color_black, menucolor2);
	ScreenDraw(58, 38, "< �� �� �� ��  �� �� �� �� ? > - �� �� �� ��");
	setColor(color_black, menucolor2);
	ScreenDraw(73, 40, " N O R M A L ");

	setColor(color_black, menucolor3);
	ScreenDraw(110, 38, "< A  S i t e  D e  L a  R u e > - E Z 2 A C");
	setColor(color_black, menucolor3);
	ScreenDraw(127, 40, " H A R D ");

}





///
/// � �� �����ߴ��� ���ϰ����� ��ȯ
void DrawMenu()
{

	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;
	
	int menunumber = GetSelectedMenu();


	if (elapsedTime >= runningSpeed)
	{

		ScreenClear();

		GetSelectedMenu();
		PlayAnim1(menunumber, 8, 3);
		PlayAnim2(menunumber, 63, 6);
		PlayAnim3(menunumber, 110, 7);
		DrawMenuList(menunumber);


		ScreenFlipping();


		elapsedTime -= runningSpeed;
	}


}






///
/// ��Ŭ�� �ִ� ���
/// bool ������ �ִϸ� ����ϴ°��� ���� ����, x y �� ��ǥ
void PlayAnim2(int menunumber, int posx, int posy)
{
	int anim_frame = 7;

	static int i = 0;

	FindAsciiArt("ascii_art_ ", anim_frame, 0);
	

	if (menunumber == 2)
	{
		PrintAsciiArt(asciiArt[0], i, anim_frame, posx, posy, color_yellow);
		if (bPlaySound == true)
		{
			AudioPlay(fanclub, &channel2);
			bPlaySound = false;
		}
		//CopyPrintAsciiArt(0, anim_frame, posx, posy, color_yellow);
		i++;
	}
	else
	{
		PrintAsciiArt(asciiArt[0], i, anim_frame, posx, posy, color_dark_white);
	}

	if (i == anim_frame)
	{
		i = 0;
	}

}


/// ����
void PlayAnim1(int menunumber, int posx, int posy)
{
	int anim_frame = 7;

	static int i = 0;

	FindAsciiArt("bunny_", anim_frame, 1);


	if (menunumber == 1)
	{
		PrintAsciiArt(asciiArt[1], i, anim_frame, posx, posy, color_yellow);
		if (bPlaySound == true)
		{
			AudioPlay(hypeboy, &channel2);
			bPlaySound = false;
		}
		//CopyPrintAsciiArt(2, anim_frame, posx, posy, color_yellow);
		i++;
	}
	else
	{
		
		PrintAsciiArt(asciiArt[1], i, anim_frame, posx, posy, color_white);
	}

	if (i == anim_frame)
	{
		i = 0;
	}

}


void PlayAnim3(int menunumber, int posx, int posy)
{
	//int x = posx;
	//int y = posy;
	//const char* str = "                                 ";
	int anim_frame = 8;

	static int i = 0;

	FindAsciiArt("DeLaRue_", anim_frame, 4);

	if (menunumber == 3)
	{
		/*setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "     �� �� �� : �� �� �� ��    ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "          ��  ��  ��           ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, " < F E E L  M Y  R H Y T H M > ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x , y++, "                               ");
		setColor(color_dark_white, color_black);
		ScreenDraw(x, y++, "                               ");*/

		PrintAsciiArt(asciiArt[4], i, anim_frame, posx, posy, color_yellow);
		if (bPlaySound == true)
		{
			AudioPlay(delarue, &channel2);
			bPlaySound = false;
		}
		//CopyPrintAsciiArt(2, anim_frame, posx, posy, color_yellow);
		i++;

	}
	else
	{
		PrintAsciiArt(asciiArt[4], i, anim_frame, posx, posy, color_white);
	}

	if (i == anim_frame)
	{
		i = 0;
	}


}

















///
/// 04.20
/// ���� �Լ�
/// y ���� Ư�� ������ �Է��� �޾Ҵ°�,, �� �����Ѵ�
/// ���� ���� ���� �ִ� ���� �޺��� �ö󰡰�..
/// ���� ���� �ۿ� �ִ� ���� �̽� ī��Ʈ�� �ö󰣴�..
/// ���� bool ������ ������ �ʿ䰡 �ִ�
/// is Good, is Miss
/// 


/// ���� ���� ���� ���߿� ��Ʈ ������ �����ֱ�
// 4 �� �¿���� Ű ����, 100 �� ��Ʈ ����
bool isGood[4][2000];
bool isMiss[4][100];

/// <summary>
/// ����
/// </summary>
/// <param name="y">������ ������ �Ǵ� y���� ��ġ</param>
/// <param name="i">��Ʈ �迭�� index</param>
/// <param name="key">�����¿� �� ���� Ű����</param>
/// <returns>isGood �� ���� ��ȯ��</returns>
bool HitBox(double y, int i, int key)
{
	//LEFT = 1;
	//RIGHT = 2;
	//UP = 3;
	//DOWN = 4;

	// 4 �� �¿���� Ű ����, 100 �� ��Ʈ ����
	bool isGood[4][2000];
	bool isMiss[4][500];

	if (y > 2.f && y <= 7.f)
	{
		if (GetKeyTable(key)&&!isGood[key][i])
		{
			//SetKeyTable(key, false);
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
			//SetKeyTable(key, false);
		}
	}

	else if (y <= 2.f && !isGood[key][i] && !isMiss[key][i])
	{
		// �̽�����Ʈ �ְ�ʹ�...
		//ScreenClear1(color_white, color_dark_purple);
		//ScreenClear1(color_white, color_dark_purple);
		{
			missCount++;

			setColor(color_red, color_green);
			ScreenPrint(100, 3, 'SSiM', 4);
			isMiss[key][i] = true;
			isGood[key][i] = false;
		}
		
	}
	
	// �޺�, �̽� ���� Ȯ�� ��¿� �ڵ�
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
	
	
	// ���� �� ��ȯ�ؼ� ��Ʈ ������ �Լ����� ��Ʈ ��� ����µ��� ����
	return isGood[key][i];
}

void HitBox2(int time, int i, int key)
{
	// ����� �÷���Ÿ���� �����ϴ� ����
	unsigned _hitElapsedTime = GetAudioPlaybackTime(channel2);

	// 4 �� �¿���� Ű ����, 100 �� ��Ʈ ����
	bool isGood[4][2000];
	bool isMiss[4][500];
	if (time + 500 > _hitElapsedTime && time - 500 < _hitElapsedTime)
	{
		if (GetKeyTable(key) && !isGood[key][i])
		{
			//SetKeyTable(key, false);
			isGood[key][i] = true;
			setColor(color_green, color_red);
			ScreenPrint(100, 2, 'DOOG', 4);
			combo++;
		}

		else
		{
			//SetKeyTable(key, false);
		}
	}
	else if (time + 500 > _hitElapsedTime && !isGood[key][i] && !isMiss[key][i])
	{
		missCount++;

		setColor(color_green, color_red);
		ScreenPrint(100, 3, 'SSiM', 4);
		isMiss[key][i] = true;
		isGood[key][i] = false;
	}


	/// Ȯ�ο�
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

void HitBox3(HitObject obj, int i)
{
	// ����� �÷���Ÿ���� �����ϴ� ����
	unsigned _hitElapsedTime = GetAudioPlaybackTime(channel2);
	int key;

	if (obj.x == 64)
		key = LEFT;
	else if (obj.x == 192)
		key = DOWN;
	else if (obj.x == 320)
		key = UP;
	else if (obj.x == 448)
		key = RIGHT;

	if (obj.time + 200 > _hitElapsedTime && obj.time - 100 < _hitElapsedTime)
	{
		if (GetKeyTable(key) && !isGood[key][i])
		{
			//AudioPlay(tick, &channel);
			//SetKeyTable(key, false);
			isGood[key][i] = true;
			setColor(color_green, color_red);
			ScreenPrint(100, 2, 'DOOG', 4);
			combo++;
			/*SetKeyTable(LEFT, false);
			SetKeyTable(RIGHT, false);
			SetKeyTable(UP, false);
			SetKeyTable(DOWN, false);*/
			SetKeyTable(key, false);
		}
		else
		{
			/*SetKeyTable(LEFT, false);
			SetKeyTable(RIGHT, false);
			SetKeyTable(UP, false);
			SetKeyTable(DOWN, false);*/
			SetKeyTable(key, false);
		}
	}
	else if (obj.time + 200 <= _hitElapsedTime && !isGood[key][i] && !isMiss[key][i])
	{
		missCount++;
		combo = 0;
		setColor(color_red, color_green);
		ScreenPrint(100, 3, 'SSiM', 4);
		isMiss[key][i] = true;
		isGood[key][i] = false;
		SetKeyTable(LEFT, false);
		SetKeyTable(RIGHT, false);
		SetKeyTable(UP, false);
		SetKeyTable(DOWN, false);
	}

	


	/// Ȯ�ο�
	char convertMiss[20] = { 0 };
	char convertCombo[20] = { 0 };

	snprintf(convertCombo, sizeof(convertCombo), "%d", combo);
	const char* constCombo = convertCombo;


	snprintf(convertMiss, sizeof(convertMiss), "%d", missCount);
	const char* constMiss = convertMiss;
	setColor(color_yellow, color_blue);

	//ScreenPrint(90, 4, 'MOC', 6);
	//ScreenPrint(90, 6, 'SSIM', 4);
	//for (int i = 0; i < 5; i++)
	{
		//ScreenPrint(100 + i, 4, convertCombo[i], 1);
		//ScreenPrint(100 + i, 6, convertMiss[i], 1);
	}

}




///
/// 04.21
/// hp ������ & ���� ���
/// ������ 100000 �ʸ���

void PrintScore(int posx, int posy, int color)
{
	int score = 0;
	score = 999999 - missCount * 34600;

	int c[6];
	int x = posx;
	int y = posy;


	if (score <= 0)
	{
		x = posx - 45;
		y = posy - 5;

		FILE* fp2;
		char bufferf[3000];
		snprintf(bufferf, 3000, "gameover1.txt");
		errno_t err = fopen_s(&fp2, bufferf, "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(bufferf, sizeof(char), 3000, fp2);

		bufferf[readSize] = '\0';
		fclose(fp2);

		int n = 0;

		while (bufferf[n] != '\0')
		{
			char o = bufferf[n];

			if (bufferf[n] == '\n')
			{
				x = posx - 45;
				y++;   // ���� �ٷ� �̵��� ������ ����
			}

			else if (o != ' ')
			{
				setColor(color_dark_red, color_dark_red);

				ScreenPrint(x, y, o, 1);
				x++;
			}

			if (o == ' ')
			{
				setColor(color_black, color_black);
				ScreenPrint(x, y, ' ', 1);
				x++;
			}

			
			n++;
		}
		return;
	}

		
	
	
	{
		c[0] = score / 100000;
		c[1] = (score % 100000) / 10000;
		c[2] = (score % 10000) / 1000;
		c[3] = (score % 1000) / 100;
		c[4] = (score % 100) / 10;
		c[5] = (score % 10) / 1;
	}


	
	FILE* fp3;
	char bufferff[3000];
	snprintf(bufferff, 3000, "score.txt");
	errno_t err = fopen_s(&fp3, bufferff, "r");
	if (err != 0) {
		//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
		exit(1);
	}
	int readSize = fread(bufferff, sizeof(char), 3000, fp3);

	bufferff[readSize] = '\0';
	fclose(fp3);

	int n = 0;

	
	y = posy - 10;
	x = posx + 3;

	while (bufferff[n] != '\0')
	{
		char o = bufferff[n];
		

		if (o == '#')
		{
			setColor(color_purple, color_purple);

			ScreenPrint(x, y, ' ', 1);
			x++;
		}

		if (o == ' ')
		{
			setColor(color_black, color_black);
			ScreenPrint(x, y, ' ', 1);
			x++;
		}

		if (o == '\n')
		{
			x = posx + 3;
			y++;   // ���� �ٷ� �̵��� ������ ����
		}
		n++;
	}


	FILE* fp[6];
	
	char buffer_forN[6][50];
	char buffer_for9[50];
	



	for (int i = 0; i < 6; i++)
	{
		int n = 0;

		snprintf(buffer_forN[i], 50, "%s%d.txt", "num_", c[i]);
		errno_t err = fopen_s(&fp[i], buffer_forN[i], "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(buffer_forN[i], sizeof(char), 50, fp[i]);
	
		buffer_forN[i][readSize] = '\0';
		fclose(fp[i]);

		
		x = posx + i * 8 + 2;
		y = posy + 3;

		while (buffer_forN[i][n] != '\0')
		{
			char o = buffer_forN[i][n];

			if (o == '#')
			{
				setColor(color, color);

				ScreenPrint(x, y, ' ', 1);
				x++;
			}
			
			if (o == ' ')
			{
				setColor(color_black, color_black);
				ScreenPrint(x, y, ' ', 1);
				x++;
			}

			if (buffer_forN[i][n] == '\n')
			{
				x = posx + i * 8 + 2;
				y++;   // ���� �ٷ� �̵��� ������ ����
			}
			n++;
		}
	}

}

int scoreDelay = 0;

void RenderScore()
{
	static ULONGLONG scoreElapsedTime;
	scoreElapsedTime += deltaTime;

	if (scoreElapsedTime >= runningSpeed)
	{
		ScreenClear();

		PrintScore(55, 25, color_white);

		ScreenFlipping();

		scoreDelay++;

		if (scoreDelay == 50)
		{
			bPlaySound = true;
			selectedNum = 0;
		}

		scoreElapsedTime -= runningSpeed;
	}

}






/// <summary>
/// 04.26 �Լ� ��ġ��
/// GetAudioPlaybackTime(channel2) �� �ν��� �� �ȵǴ� Ȯ���غ���
/// </summary>
void ParcingNote()
{
	/// �������
	yHeight -= judgeLineY;
	ULONGLONG startTime = GetTickCount64();

	//AudioSystem();
	AudioCreate("Assets/maintheme.mp3", &maintheme);
	AudioCreate("Assets/fanclub.mp3", &fanclub);
	AudioCreate("Assets/Kamui.mp3", &kamui);
	AudioCreate("Assets/hypeboy.mp3", &hypeboy);
	AudioCreate("Assets/tick.wav", &tick);
	AudioCreate("Assets/start.wav", &start);
	AudioCreate("Assets/DeLaRue.mp3", &delarue);
	AudioCreate("Assets/clear.mp3", &clear);
	AudioCreate("Assets/gameover.mp3", &gameover);
	
	//AudioPlay(kamui, &channel2);
	//AudioPlay(hypeboy, &channel2);

	fp1 = fopen(filename_hypeboy, "r");
	if (fp1 == NULL) {
		printf("������ �ҷ��� �� �����ϴ�. %s\n", filename_hypeboy);
		exit(1);
	}

	// �� hit object line�� �Ľ� 
	while (fgets(buffer, sizeof(buffer), fp1)) {
		if (sscanf(buffer, "%d,%d,%d,%d,%d,%*d:%*d:%*d:%*d:\n",
			&hitobjects[0][num_hitobjects].x,
			&hitobjects[0][num_hitobjects].y,
			&hitobjects[0][num_hitobjects].time,
			&hitobjects[0][num_hitobjects].type,
			&hitobjects[0][num_hitobjects].hit_sound) != 5) {
			break;
		}
		num_hitobjects++;
	}

	fclose(fp1);


	
	fp2 = fopen(filename_fanclub, "r");
	if (fp2 == NULL) {
		printf("������ �ҷ��� �� �����ϴ�. %s\n", filename_fanclub);
		exit(1);
	}

	// �� hit object line�� �Ľ� 
	while (fgets(buffer2, sizeof(buffer2), fp2)) {
		if (sscanf(buffer2, "%d,%d,%d,%d,%d,%*d:%*d:%*d:%*d:\n",
			&hitobjects[1][num_hitobjects2].x,
			&hitobjects[1][num_hitobjects2].y,
			&hitobjects[1][num_hitobjects2].time,
			&hitobjects[1][num_hitobjects2].type,
			&hitobjects[1][num_hitobjects2].hit_sound) != 5) {
			break;
		}
		num_hitobjects2++;
	}

	fclose(fp2);


	fp3 = fopen(filename_delarue, "r");
	if (fp3 == NULL) {
		printf("������ �ҷ��� �� �����ϴ�. %s\n", filename_delarue);
		exit(1);
	}

	// �� hit object line�� �Ľ� 
	while (fgets(buffer3, sizeof(buffer3), fp3)) {
		if (sscanf(buffer3, "%d,%d,%d,%d,%d,%*d:%*d:%*d:%*d:\n",
			&hitobjects[2][num_hitobjects3].x,
			&hitobjects[2][num_hitobjects3].y,
			&hitobjects[2][num_hitobjects3].time,
			&hitobjects[2][num_hitobjects3].type,
			&hitobjects[2][num_hitobjects3].hit_sound) != 5) {
			break;
		}
		num_hitobjects3++;
	}

	fclose(fp3);


	/// ������� �߰���
}



bool isExitGame = false;

/// <summary>
/// ����ؾ� �ϴ°� ���� �ִ� ����
/// num �� �� ���� ��� ������, ��Ÿ���� �뷡 �ѽð�, �迭 ��
/// </summary>
void GameLogic(int num, unsigned run_time, int number_hitobjects)
{
	const int x = 8;
	const int padding = 16;


	// ����� �÷���Ÿ���� �����ϴ� ����
	unsigned elapsedTime = GetAudioPlaybackTime(channel2);
	if (elapsedTime >= run_time)
	{
		AudioPlay(clear, &channel);
		FMOD_Channel_Stop(channel2);
		isExitGame = true;
	}
	if (missCount >= 20)
	{
		// Ÿ��Ʋ ���� �ߴ�
		AudioPlay(gameover, &channel);
		FMOD_Channel_Stop(channel2);
		isExitGame = true;
		missCount = 1000;
	}

	for (int i = startNote; i < number_hitobjects; ++i)
	{
		COORD posLeft = { x + padding * 0, hitobjects[num][i].y };	// 8
		COORD posDown = { x + padding * 1, hitobjects[num][i].y };	// 24
		COORD posUp = { x + padding * 2, hitobjects[num][i].y };		// 40
		COORD posRight = { x + padding * 3, hitobjects[num][i].y };	// 56

		// hitobject.time �� �� ��ü�� �����Ǿ���ϴ� �ð� - �÷��� �ð�
		int isPass = hitobjects[num][i].time - elapsedTime;

		// �����Ǳ� ��, ������ �ʿ� ���� �ֵ��� Ž���� ������
		if (isPass >= scrollSpeed)
			break;

		// y ���� ��ȭ��
		hitobjects[num][i].y = (double)(hitobjects[num][i].time - elapsedTime) / scrollSpeed * (yHeight-3);

		HitBox3(hitobjects[num][i], i);
		
		if ((hitobjects[num][i].y > 1.f) && (hitobjects[num][i].y < 42.f))
		{
			if (hitobjects[num][i].x == 64)
			{
				//HitBox2(hitobjects[num][i].time, i, LEFT);
				ScreenDrawLeftArrow(posLeft, color_blue);
			}
			else if (hitobjects[num][i].x == 192)
			{
				//HitBox2(hitobjects[num][i].time, i, DOWN);
				ScreenDrawDownArrow(posDown, color_green);
			}
			else if (hitobjects[num][i].x == 320)
			{
				//HitBox2(hitobjects[num][i].time, i, UP);
				ScreenDrawUpArrow(posUp, color_red);
			}
			else if (hitobjects[num][i].x == 448)
			{
				//HitBox2(hitobjects[num][i].time, i, RIGHT);
				ScreenDrawRightArrow(posRight, color_dark_yellow);
			}
		}
		
		// �̹� ������ �ֵ� (�� ��Ž���� �����ð��� ���� �ֵ�)�� ���ܽ�Ų��
		if (hitobjects[num][i].time + 2000 < elapsedTime)
		{
			++startNote;
		}
	}
}



void PrintCombo(int posx, int posy, int color)
{
	int c[3];

	c[0] = (combo % 1000) / 100;
	c[1] = (combo % 100) / 10;
	c[2] = (combo % 10) / 1;

	FILE* fp[3];

	char buffer_forN[3][50];

	int x = posx;
	int y = posy;

	for (int i = 0; i < 3; i++)
	{
		int n = 0;
		snprintf(buffer_forN[i], 50, "%s%d.txt", "num_", c[i]);
		errno_t err = fopen_s(&fp[i], buffer_forN[i], "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(buffer_forN[i], sizeof(char), 50, fp[i]);

		buffer_forN[i][readSize] = '\0';
		fclose(fp[i]);


		x = posx + i * 8;
		y = posy;

		while (buffer_forN[i][n] != '\0')
		{
			char o = buffer_forN[i][n];
			

			if (o == '#')
			{
				setColor(color, color);
				ScreenPrint(x, y, ' ', 1);
				x++;
			}

			if (o == ' ')
			{
				setColor(color_black, color_black);
				ScreenPrint(x, y, ' ', 1);
				x++;
			}

			if (buffer_forN[i][n] == '\n')
			{
				x = posx + i * 8;
				y++;   // ���� �ٷ� �̵��� ������ ����
			}
			n++;
		}
	}

}


void InitHitBox()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2000; j++)
		{
			isGood[i][j] = NULL;
		}
		for (int j = 0; j < 100; j++)
		{
			isMiss[i][j] = NULL;
		}
	}
}







void gLoop_1()
{
	UpdateTime();
	UpdateInput();
	UpdateRender_Song1();
}



void gLoop_2()
{
	UpdateTime();
	UpdateInput();
	UpdateRender_Song2();
}


void gLoop_3()
{
	UpdateTime();
	UpdateInput();
	UpdateRender_Song3();

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


void ScoreLoop()
{
	UpdateTime();
	RenderScore();
	UpdateMenuInput();
}







void InitGame()
{
	/// �ʱ�ȭ
	// �ܼ� ����
	initConsole();
	// �ð� �ʱ�ȭ
	InitTime();

	// ����� �ý���
	AudioSystem();

	ParcingNote();

	

	// ��Ʈ ��ġ �ʱ�ȭ
	SetNotePosition(45);
	// Ÿ��Ʋ�� ����� �ƽ�Ű ��Ʈ ����
	//Copy("FMR_title.txt", 0);
	Copy("subTitle2.txt", 1);
}




void GameManager()
{
	switch (selectedNum)
	{
		// Ÿ��Ʋ
	case 0:
		if (bPlaySound == true)
		{
			InitHitBox();
			AudioPlay(maintheme, &channel2);
			bPlaySound = false;
			missCount = 0;
			combo = 0;
			scoreDelay = 0;
		}
		TitleLoop();

		if (GetKeyTable(SPACE))
		{
			// Ÿ��Ʋ ���� �ߴ�
			AudioPlay(start, &channel);
			FMOD_Channel_Stop(channel2);
			SetKeyTable(SPACE, false);
			isExitGame = false;
			selectedNum = 4;
			bPlaySound = true;
		}
		break;

	case 1:
		//if (bPlaySound == true)
		{
			AudioPlay(hypeboy, &channel2);
			bPlaySound = false;
		}
		while (!isExitGame)
		{
			gLoop_1();
		}
		SetKeyTable(SPACE, false);
		selectedNum = 5;
		startNote = 0;
		break;

	case 2:
		//if (bPlaySound == true)
		{
			AudioPlay(fanclub, &channel2);
			bPlaySound = false;
		}
		while (!isExitGame)
		{
			gLoop_2();
		}
		SetKeyTable(SPACE, false);
		selectedNum = 5;
		startNote = 0;
		break;


	case 3:
		//if (bPlaySound == true)
		{
			AudioPlay(delarue, &channel2);
			bPlaySound = false;
		}
		while (!isExitGame)
		{
			gLoop_3();
		}
		SetKeyTable(SPACE, false);
		selectedNum = 5;
		startNote = 0;
		break;


		// �޴�
	case 4:
		if (GetKeyTable(SPACE))
		{
			AudioPlay(start, &channel);
			FMOD_Channel_Stop(channel2);
			selectedNum = GetSelectedMenu();
			SetKeyTable(SPACE, false);
		}
		MenuLoop();
		break;

		// ���ھ�
	case 5: 
		ScoreLoop();
		
		break;

	default:
		break;
	}

	

}



int main()
{
	/// �ʱ�ȭ
	InitGame();

	while (1)
	{
		GameManager();
	}


	//// �ܼ� ����
	//initConsole();
	//// �ð� �ʱ�ȭ
	//InitTime();

	//// ����� �ý���
	//AudioSystem();
	//
	//ParcingNote();
	//
	//// Ÿ��Ʋ ���� ���
	//AudioPlay(maintheme, &channel2);

	//// ��Ʈ ��ġ �ʱ�ȭ
	//SetNotePosition(45);
	//// Ÿ��Ʋ�� ����� �ƽ�Ű ��Ʈ ����
	//Copy("FMR_title.txt", 0);
	//Copy("subTitle2.txt", 1);


	

	// ������ ���� ����
	//int selectedNum = GetSelectedMenu();
	// Ÿ��Ʋ ���� ���
	//AudioPlay(maintheme, &channel2);
	//// title & menu
	//while (1)
	//{
	//	// title & title sound
	//	UpdateTime();
	//	UpdateMenuInput();
	//	TitleRender(bTitle);
	//		
	//	

	//	if (GetKeyTable(SPACE))
	//	{
	//		// Ÿ��Ʋ ���� �ߴ�
	//		AudioPlay(start, &channel);
	//		FMOD_Channel_Stop(channel2);
	//		SetKeyTable(SPACE, false);
	//		// �����Ҵ� ����
	//		free(copy_temp[0]);
	//		free(copy_temp[1]);
	//		break;
	//	}

	//}

	//// menu
	//while (1)
	//{

	//	UpdateTime();
	//	UpdateMenuInput();

	//	DrawMenu();
	//	if (GetKeyTable(SPACE))
	//	{
	//		selectedNum = GetSelectedMenu();
	//		SetKeyTable(SPACE, false);
	//		break;
	//	}

	//}

	//switch (selectedNum)
	//{
	//case 1:
	//	Sleep(500);
	//	AudioPlay(hypeboy, &channel2);
	//	while (1)
	//	{
	//		gLoop1();			
	//	}

	//	break;
	//case 2:
	//	Sleep(500);
	//	AudioPlay(fanclub, &channel2);
	//	while (1)
	//	{
	//		
	//		gLoop2();

	//	}

	//	break;

	//}
	
}