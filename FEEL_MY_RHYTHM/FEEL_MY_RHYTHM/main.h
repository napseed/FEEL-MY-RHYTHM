#pragma once
//#include "input.h"
//#include "audio.h"
#include "menu.h"
#include <string.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 



void ScreenDrawUpArrow(COORD pos, int color);
void setColor(int, int);
void ScreenDrawKeyInterface();
//void UpdateNote();
HANDLE GetScreenHandle();
void GenerateNote();
void UpdateNotePosition_left(int);
void ScreenPrint(int x, int y, const char* str, int length);
void PrintAsciiArt(char**, int , int , int , int, int);
bool HitBox(int, int, int);
void SetNotePosition(int );
void PlayAnim1(int, int, int);
int GetSelectedMenu();
void DrawMenuList(int);
void PlayAnim2(int, int, int);
void PlayAnim3(int, int, int);
void CopyPrintAsciiArt(int, int, int , int, int);
void FindAsciiArt(const char* asciiArtFilePath, int fileNum, int asciiNum);
void GameLogic();
void PrintCombo(int, int, int);


/// <summary>
/// 상하좌우 방향키 노트 
/// *** 마지막 노트는 꼭 0과 1이 아닌 숫자로 채워야함!!!!! ***
/// 노트 배열 크기 설정부분은 카운트 초기화 함수 위에 있삼
/// </summary>
int d_note[] = { 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 };
int u_note[] = { 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 };
int l_note[] = { 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 };
int r_note[] = { 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 };

int note[4][43] = { { 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 },
					{ 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 },
					{ 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 },
					{ 0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,2 } };


// 테스트용 배열 재출력 방지 체크 변수
bool noteCheck[4][43];

/// <summary>
/// 노트 카운트랑 노트 초기위치 SetNotePosition 함수에서 초기화 함
/// </summary>
int noteCount;
int missCount = 0;
int combo = 0;


//COORD NoteprePos_l[] = { {8,40},{8,40},{8,40},{8,40},{8,40},{8,40} };



//const char* asciiArtFilePath2 = "D:\\SVN\\GA2023\\DataStructureAndAlgorithm Assignment\\Console Mini Project\\FEEL_MY_RHYTHM\\4_Resources\\ascii_art\\fanclub\\ascii_art_";
//const char* asciiArtFilePath1 = "D:\\SVN\\GA2023\\DataStructureAndAlgorithm Assignment\\Console Mini Project\\FEEL_MY_RHYTHM\\4_Resources\\fanclub_ascii\\ascii_art_ ";





/// <summary>
/// 파일 경로는 프로젝트 폴더 안에 넣어두기만 해도 자동으로 읽어오더라
/// 프로젝트 폴더 안에 두고 파일명을 지정해주면 됨
/// exe 로 실행할때엔 디버그 폴더안에도 둬야함
/// </summary>
const char* asciiArtFilePath1 = "ascii_art_";
const char* asciiArtFilePath2 = "ascii_art_ ";




/// <summary>
/// 색깔 팔레트
/// </summary>
/// 
enum Color
{
	color_black,
	color_dark_blue,
	color_dark_green,
	color_dark_aqua,
	color_dark_red,
	color_dark_purple,
	color_dark_yellow,
	color_dark_white,
	color_gray,
	color_blue,
	color_green,
	color_aqua,
	color_red,
	color_purple,
	color_yellow,
	color_white
};


//#include <stdio.h>
//#include <windows.h>
//#include <stdbool.h>