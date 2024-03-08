#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "audio.h"
#include "menu.h"




//#include "draw.h"

#define MAX_LINES 5000

void gotoxy(int x, int y)
{

	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef struct
{
	int x;
	double y;
	int time;
	int type;
	int hit_sound;
} HitObject;

int GetNotePositionY(ULONGLONG _elapsedTime, int _noteTiming)
{

}

int scrollSpeed = 1500;
int judgeLineY = 2;
int yHeight = 50;



// osu������ �Ľ��ϴ� �Լ��Դϴ�.
int main()
{
	yHeight -= judgeLineY;
	ULONGLONG startTime = GetTickCount64();
	char* filename = "Kamui.osu";
	FILE* fp;
	char buffer[1024];
	char* token;
	HitObject hitobjects[MAX_LINES];
	int num_hitobjects = 0;
	AudioSystem();
	AudioCreate("Assets/Kamui.mp3", &kamui);
	AudioPlay(kamui, &channel2);



	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("������ �ҷ��� �� �����ϴ�. %s\n", filename);
		exit(1);
	}

	// �� hit object line�� �Ľ� 
	while (fgets(buffer, sizeof(buffer), fp)) {
		if (sscanf(buffer, "%d,%d,%d,%d,%d,%*d:%*d:%*d:%*d:\n",
			&hitobjects[num_hitobjects].x,
			&hitobjects[num_hitobjects].y,
			&hitobjects[num_hitobjects].time,
			&hitobjects[num_hitobjects].type,
			&hitobjects[num_hitobjects].hit_sound) != 5) {
			break;
		}
		num_hitobjects++;
	}

	fclose(fp);


	// �Ľ��� hit object ���� ���� ��Ʈ ������ Ȯ���մϴ� 
	int startNote = 0;

	while (1)
	{

		/*ULONGLONG currentTime = GetTickCount64();
		ULONGLONG elapsedTime = currentTime - startTime;*/
		unsigned elapsedTime = GetAudioPlaybackTime(channel2);

		for (int i = startNote; i < num_hitobjects; ++i)
		{
			int isPass = hitobjects[i].time - elapsedTime;
			if (isPass >= scrollSpeed)
				break;

			hitobjects[i].y = (double)(hitobjects[i].time - elapsedTime) / scrollSpeed * yHeight;
			if (hitobjects[i].x == 64)
			{
				gotoxy(8, hitobjects[i].y); printf("1�� ��Ʈ");
				gotoxy(8, hitobjects[i].y + 1); printf("        ");
			}
			else if (hitobjects[i].x == 192)
			{
				gotoxy(24, hitobjects[i].y); printf("2�� ��Ʈ");
				gotoxy(24, hitobjects[i].y + 1); printf("        ");
			}
			else if (hitobjects[i].x == 320)
			{
				gotoxy(40, hitobjects[i].y); printf("3�� ��Ʈ");
				gotoxy(40, hitobjects[i].y + 1); printf("        ");
			}
			else if (hitobjects[i].x == 448)
			{
				gotoxy(56, hitobjects[i].y); printf("4�� ��Ʈ");
				gotoxy(56, hitobjects[i].y + 1); printf("        ");
			}

			if (hitobjects[i].time <= elapsedTime)
			{
				++startNote;
			}
		}



		/*for (int i = startNote; i < num_hitobjects; ++i)
		{
			if (hitobjects[i].time <= elapsedTime)
			{
				++startNote;
				printf("��Ʈ�� ��� �������� �������ϴ�.");
			}
		}*/
	}

	// hit objects ���
	/*for (int i = 0; i < num_hitobjects; i++) {
		printf("%d,%d,%d,%d,%d\n",
			hitobjects[i].x,
			hitobjects[i].y,
			hitobjects[i].time,
			hitobjects[i].type,
			hitobjects[i].hit_sound);
	}*/

}