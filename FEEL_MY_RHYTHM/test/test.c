#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

// 이전 시간
ULONGLONG previousTime;
// 현재 시간
ULONGLONG currentTime;
// 이전시간 - 현재시간
ULONGLONG deltaTime;


enum KeyCode
{
	ESCAPE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE
};

bool keyTable[6];

bool GetKeyTable(const int keyIndex)
{
	return keyTable[keyIndex];
}

/// 키 값의 참 거짓을 바꿔주는 함수
void SetKeyTable(const int keyIndex, bool isOn)
{
	keyTable[keyIndex] = isOn;
}


/// GetAsyncKeyState 사용
void UpdateInput()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		SetKeyTable(ESCAPE, true);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		SetKeyTable(SPACE, true);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		SetKeyTable(LEFT, true);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		SetKeyTable(RIGHT, true);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		SetKeyTable(UP, true);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		SetKeyTable(DOWN, true);
	}

}


void InitTime()
{
	// 델타타임 초기화 = 0 (누적시간 0)
	currentTime = previousTime = GetTickCount64();
	// 1ms 단위로 반환
}

void UpdateTime()
{
	previousTime = currentTime;
	currentTime = GetTickCount64();

	deltaTime = currentTime - previousTime;
}

void gotoXY(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

void Timer()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	if (elapsedTime >= 5000)
	{
		elapsedTime -= 5000;
	}

	gotoXY(5, 5);
	printf("  %2d  ", 50-elapsedTime/100);
}

ULONGLONG arr[] = { 5000, 10000, 15000, 20000 };


void HitBox()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	// 다른 방향키랑 동시 입력이 안되는 문제가 생길듯
	// 그냥 키 별로 변수 전부 따로 만들어야지
	static bool isGood = false;

	if (elapsedTime < arr[0] - 1000)
	{
		SetKeyTable(SPACE, false);
	}

	for (int i = 0; i < sizeof(arr) / sizeof(ULONGLONG); i++)
	{
		//SetKeyTable(SPACE, false);
		
		// 앞 뒤 시간 패딩 앞 1초 뒤 0.5초

		// GOOD 판정
		if ((elapsedTime >= arr[i] - 1000) && (elapsedTime < arr[i] + 500))
		{
			if (GetKeyTable(SPACE))
			{
				SetKeyTable(SPACE, false);
				isGood = true;
				
			}
			if (isGood)
			{
				gotoXY(5, 10);
				// 함수 호출해서 정해진 시간동안만 GOOD 이 출력되도록 하면 될듯
				printf("  GOOD  ");
				// HP ++
			}
		}

		// MISS 판정 (정해진 판정시간 안에 입력이 안들어온 경우)
		if (elapsedTime >= arr[i] + 500 && elapsedTime < arr[i] + 1000)
		{
			if (!isGood)
			{
				gotoXY(5, 10);
				printf("  MISS  ");
				// HP --
			}

			// 이미 앞에서 판정 입력을 한 경우
			if (isGood)
			{
				gotoXY(5, 10);
				printf("        ");
			}
			SetKeyTable(SPACE, false);
		}

		// else 의 경우인데,, else 를 쓰면 오류가 뜸 (?)
		// 판정 시간이 아닐때, 키를 입력받으면 false 로 바꿔주는 부분 + isGood 변수 초기화
		if(elapsedTime >= arr[i]+1000 && elapsedTime <arr[i+1]-1000)
		{
			isGood = false;
			gotoXY(5, 10);
			printf("        ");
			SetKeyTable(SPACE, false);
		}
	}
	
}
// 근데 이러면 모든 키를 계속 누르고 있으면 다 GOOD 판정이 되는데..
// 안 누르고 있다가 누른 경우만 참이나오게.. getch 랑 같은 기능이되어버림 근데 롱노트 안쓸거니까..
// 인터페이스 유지하려면 판정용 인풋시스템 따로 다시 만드는게 조켓다..ㅠㅠ
/// 인풋 시스템 비트연산 리턴값 찾아보기!


int main()
{
	InitTime();

	while (1)
	{
		UpdateTime();
		UpdateInput();
		Timer();
		HitBox();
		//gotoXY(5, 20);
		//printf("space check : %d", keyTable[SPACE]);
		
	}
}