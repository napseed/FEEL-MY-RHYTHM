#define MAX_NOTE 6
#define ROW_SIZE 160
#define COLUMN_SIZE 50
#include "input.h"
#include "main.h"
#include "audio.h"



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


/// <summary>
/// �ʱ�ȭ
/// ���� �ޱ� -> ���߿� �ܼ�â ũ�� ���� �����ϱ�
/// </summary>
void initConsole()
{
	//system("mode con cols=160 lines=50 | title FEEL_THE_RHYTHM");
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
	DWORD dwConSize;


	// �ܼ� â�� �������� ä���
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwConSize);

	// ���� �Ӽ��� �����Ͽ� �ܼ� â�� �����
	WORD wColors = ((WORD)0 << 4) | (WORD)15; // ��� ���ڻ�, ���� ����
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwConSize);
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

/// ScreenPrint(10, 2, '@', 1)
/// ���ۿ� �׸��׸��� (���� �� ĭ ���) (���� �ϳ��� ���� �� ����)
void ScreenPrint(int x, int y, const char* str, int length)
{
	DWORD dw;	// unsigned long ����ü
	COORD CursorPosition = { x, y };
	


	SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
	// 2 �� ���ڿ� ����

	



	WriteFile(GetScreenHandle(), &str, length, &dw, NULL);
}


///
/// ū ����ǥ �ȿ� ����� ���ڿ��� �ִ´� "���ڿ� ��¼��~~~"
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
ULONGLONG startTime;
// ���� �ð�
ULONGLONG previousTime;
// ���� �ð�
ULONGLONG currentTime;
// �����ð� - ����ð�
ULONGLONG deltaTime;


const ULONGLONG runningSpeed = 50;		// 0.01 ��
const ULONGLONG BPM = 120;				// ���� 120 bpm �����̸�?
const ULONGLONG noteSpeed = 100;


/// <summary>
/// �ð� �ʱ�ȭ
/// </summary>
void InitTime()
{
	// ��ŸŸ�� �ʱ�ȭ = 0 (�����ð� 0)
	startTime = currentTime = previousTime = GetTickCount64();
	// 1ms ������ ��ȯ
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
//	return deltaTime;								// ���� : 1/1000 ��
//}




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
///  ���� ��Ʈ �ε��� �������� ������
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



// �ƽ�Ű ��Ʈ
char** asciiArt[3];
//char** copyAnim[5];




/// ���� (�������� ��� �κ�) �� ���� ����
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
		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GenerateNote();
		//UpdateNote();
		FindAsciiArt(asciiArtFilePath1, 10, 0);
		FindAsciiArt(asciiArtFilePath2, 10, 2);
		PrintAsciiArt(asciiArt[2], i, anim1_frame, 120, 15, color_dark_white);
		PrintAsciiArt(asciiArt[0], j, anim2_frame, 90, 13, color_dark_white);

		//CopyPrintAsciiArt(1, i, anim1_frame, 120, 15, color_dark_white);
		//CopyPrintAsciiArt(0, j, anim2_frame, 90, 13, color_dark_white);

		// �� �� ���۸� �����´�
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

/// ���� (�������� ��� �κ�) �� ���� ����
void UpdateRender_Song1()
{
	int anim1_frame = 4;
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	static int i = 0;

	if (elapsedTime >= runningSpeed)
	{
		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GenerateNote();
		//UpdateNote();
		FindAsciiArt("bunny_", anim1_frame, 3);
		PrintAsciiArt(asciiArt[3], i, anim1_frame, 100, 10, color_white);

		//CopyPrintAsciiArt(1, i, anim1_frame, 120, 15, color_dark_white);
		//CopyPrintAsciiArt(0, j, anim2_frame, 90, 13, color_dark_white);

		// �� �� ���۸� �����´�
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
			y++;   // ���� �ٷ� �̵��� ������ ����
			//setColor(color_black, color);
			//ScreenPrint(x, y, NULL, 0);
		}
	}

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


	/*char* print_temp;
	FILE* rfp;
	errno_t err = fopen_s(&rfp, "subTitle2.txt", "rt");
	print_temp = (char*)malloc(sizeof(char) * 5000);*/


	int index = 0;


	//if (err != 0)
	//{
	//	//printf("���� �ҷ����⿡ �����߽��ϴ�.\n");
	//	exit(1);
	//}

	//// ������ ���ڿ� �ι��� �߰�
	//int readSize = fread(print_temp, sizeof(char), 5000, rfp);
	//print_temp[readSize] = '\0';
	//fclose(rfp);



	// ȸ�� ���� ��� �ڵ�
	for (int j = 0; j <11; j++)
	{
		// �ϴ� ȭ�� ũ�� Ȯ���Ȱ� �ƴϴϱ� ȭ�� ���ۻ���� �°� ��µǵ��� �����س���
		for (int i = 0; i < consoleScreenSize.Right - consoleScreenSize.Left + 1; i++)
		{
			setColor(backcolor, color_white);
			ScreenPrint(x, y, ' ', 1);
			x++;
		}
		y++;
	}



	// ���ڸ� ���ڰŸ��� �ϰ�ʹ�

	// ���� ���� ����
	static bool setPrint = true;
	
	if (setPrint == true)
	{
		while (copy_temp[1][index] != '\0')
		{
			// # ���ڸ� �������� ����ϴ°� �̻۰Ű���
			char c = copy_temp[1][index];

			// # �ΰ��
			if (c == '#')
			{
				setColor(printcolor, color_black);
				ScreenPrint(x2, y2, ' ', 1);
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

		elapsedTime -= 1000;
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
	ScreenDraw(15, 38, "< H Y P E  B O Y > - N E W  J E A N S");

	setColor(color_black, menucolor2);
	ScreenDraw(65, 38, "< �� �� �� ��  �� �� �� �� ? > - �� �� �� ��");

	setColor(color_black, menucolor3);
	ScreenDraw(125, 38, " �� �� �� . . .");


}










///
/// � �� �����ߴ��� ���ϰ����� ��ȯ
void DrawMenu()
{

	// �޴��� Ȱ��ȭ�ϴ� ����
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
/// ��Ŭ�� �ִ� ���
/// bool ������ �ִϸ� ����ϴ°��� ���� ����, x y �� ��ǥ
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


/// ����
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
		ScreenDraw(x, y++, "                               ");
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



/// <summary>
/// ����
/// </summary>
/// <param name="y">������ ������ �Ǵ� y���� ��ġ</param>
/// <param name="i">��Ʈ �迭�� index</param>
/// <param name="key">�����¿� �� ���� Ű����</param>
/// <returns>isGood �� ���� ��ȯ��</returns>
bool HitBox(int y, int i, int key)
{
	//LEFT = 1;
	//RIGHT = 2;
	//UP = 3;
	//DOWN = 4;

	/// ���� ���� ���� ���߿� ��Ʈ ������ �����ֱ�
	// 4 �� �¿���� Ű ����, 100 �� ��Ʈ ����
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
		// �̽�����Ʈ �ְ�ʹ�...
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






///
/// 04.21
/// hp ������ & ���� ���
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


/// ��ȣ�� ���� ���� ����Ѵ�
void BG_Draw(int selectedNum)
{
	// define ����
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
	/// �ʱ�ȭ
	// �ܼ� ����
	initConsole();
	// �ð� �ʱ�ȭ
	InitTime();

	// ����� �ý���
	AudioSystem();
	// ����� ����
	AudioCreate("Assets/fanclub.mp3", &fanclub);
	AudioCreate("Assets/hihat.mp3", &sound2);
	AudioPlay(fanclub, channel);
	

	// ��Ʈ ��ġ �ʱ�ȭ
	SetNotePosition(45);
	// Ÿ��Ʋ�� ����� �ƽ�Ű ��Ʈ ����
	Copy("FMR_title.txt", 0);
	Copy("subTitle2.txt", 1);


	// Ÿ��Ʋ ��� ���� ���� ����
	bool bTitle = true;

	// ������ ���� ����
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
			// �����Ҵ� ����
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