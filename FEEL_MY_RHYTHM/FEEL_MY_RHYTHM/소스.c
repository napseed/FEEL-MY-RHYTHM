#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

CHAR_INFO screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
CHAR_INFO backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
CHAR_INFO* pScreenBuffer = &screenBuffer[0][0];

unsigned long startTime = GetTickCount64();

void Draw();
void Draw2();

void Render();
void Present();

int main() {
	//콘솔 크기 설정
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD coBufferCoord = { 0,0 };
	SMALL_RECT srctWriteRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	SetConsoleScreenBufferSize(hOut, coBufferSize);
	SetConsoleWindowInfo(hOut, TRUE, &srctWriteRect);

	// 이중 버퍼 초기화
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			backBuffer[i][j].Char.AsciiChar = ' ';
			backBuffer[i][j].Attributes = 0;
		}
	}

	while (TRUE)
	{
		if (GetTickCount64() - startTime > 30)
		{
			Draw();
			Render();
			Present();
		}

		if (GetTickCount64() - startTime > 60)
		{
			Draw2();
			Render();
			Present();
			startTime = GetTickCount64();
		}
	}

	return 0;
}

void Draw() {
	//backBuffer에 그래픽 그리기
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			backBuffer[y][x].Char.AsciiChar = '*';
			backBuffer[y][x].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		}
	}
}

void Draw2() {
	//backBuffer에 그래픽 그리기
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			backBuffer[y][x].Char.AsciiChar = '/';
			backBuffer[y][x].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
	}
}

void Render() {
	//backBuffer를 screenBuffer에 복사
	memcpy(pScreenBuffer, backBuffer, sizeof(backBuffer));
}

void Present() {
	//screenBuffer를 콘솔에 출력
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD bufferCoord = { 0, 0 };
	SMALL_RECT writeRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleOutput(hStdOut, (CHAR_INFO*)screenBuffer, bufferSize, bufferCoord, &writeRegion);
}