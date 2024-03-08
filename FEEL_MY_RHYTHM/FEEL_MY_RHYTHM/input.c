#include "input.h"
#include "audio.h"

const int ESCAPE = 5;
const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;
const int SPACE = 4;

const DWORD shortKeyTime = 50;
const DWORD longKeyTime = 500;

#define MAX_KEY = 6;
// Ű�� �ԷµǾ�����! �ԷµǾ��ٸ� true
bool keyTable[6];


/// ��, ��, ��, �� �� �ش� keyTable ���� �޾ƿ´�
bool GetKeyTable(const int keyIndex)
{
	return keyTable[keyIndex];
}

/// Ű ���� �� ������ �ٲ��ִ� �Լ�
void SetKeyTable(const int keyIndex, bool isOn)
{
	keyTable[keyIndex] = isOn;
}


/// GetAsyncKeyState ���
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


	//static bool tempL;
	//static bool isDownL;
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	if (!tempL)
	//	{
	//		SetKeyTable(LEFT, true);
	//		//isDownL = true;
	//	}

	//	tempL = true;
	//	
	//}
	//else
	//{
	//	tempL = false;
	//	//isDownL = false;
	//}

	static bool continueR;			// �����Է��� ����?
	static bool delayR;				// ��ȿ�� �Է��� �ֳ�
	static bool isDownR;			// ������ �����ִ°�?
	static DWORD lastKeyDownTime = 0;
	static DWORD currentTime = 0;
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState(0x4B) & 0x8000)
	{

		// �ȴ��� ����, ����
		if (!delayR&&!isDownR&&!continueR)
		{
			SetKeyTable(RIGHT, true);
			isDownR = true;
			delayR = true;
			//continueR = true;
			lastKeyDownTime = GetTickCount();
		}
		//delayR = true;

		//// �̹� ���� �������� ���Է��� �޾Ƶ� �ٽ� ������
		//if (delayR && isDownR && continueR)
		//{
		//	//!delayR;
		//	continueR = false;
		//	SetKeyTable(RIGHT, true);

		//}
		
		// �̹� ���� ���¿��� �Է��� �ȵ��� ����
		if (delayR && isDownR)
		{
			currentTime = GetTickCount();
			if (currentTime - lastKeyDownTime <= longKeyTime)
			{
				if (continueR)
				{
					// ����
				}
				else
				{
					SetKeyTable(RIGHT, true);
					isDownR = true;
					delayR = true;
				}

			}
			else continueR = false;
		}

	}

	else
	{
		
		// �������� 1�ʰ� �������¸� ����
		currentTime = GetTickCount();
		if(currentTime-lastKeyDownTime> shortKeyTime)
		{
			isDownR = false;
			delayR = false;
			continueR = false;
		}
		// 1�ʰ� ���� �ʾ����� ������ ���¸� 1�� ����
		else
		{
			SetKeyTable(RIGHT, true);
			isDownR = true;
			delayR = false;
			continueR = true;
		}
		//else SetKeyTable(RIGHT, true);
	
		// �����Է� ����
		//delayR = false;
		

	}
	
	static bool continueL;			
	static bool delayL;				
	static bool isDownL;			
	static DWORD lastKeyDownTimeL = 0;
	static DWORD currentTimeL = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(0x44) & 0x8000)
	{

		if (!delayL && !isDownL && !continueL)
		{
			SetKeyTable(LEFT, true);
			isDownL = true;
			delayL = true;

			lastKeyDownTimeL = GetTickCount();
		}
		if (delayL && isDownL)
		{
			currentTimeL = GetTickCount();
			if (currentTimeL - lastKeyDownTimeL <= longKeyTime)
			{
				if (!continueL)
				{
					SetKeyTable(LEFT, true);
					isDownL = true;
					delayL = true;
				}
				

			}
			else continueL = false;
		}

	}
	else
	{
		currentTimeL = GetTickCount();
		if (currentTimeL - lastKeyDownTimeL > shortKeyTime)
		{
			isDownL = false;
			delayL = false;
			continueL = false;
		}
		else
		{
			SetKeyTable(LEFT, true);
			isDownL = true;
			delayL = false;
			continueL = true;
		}
	
	}

	static bool continueU;
	static bool delayU;
	static bool isDownU;
	static DWORD lastKeyDownTimeU = 0;
	static DWORD currentTimeU = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(0x4A) & 0x8000)
	{

		if (!delayU && !isDownU && !continueU)
		{
			SetKeyTable(UP, true);
			isDownU = true;
			delayU = true;

			lastKeyDownTimeU = GetTickCount();
		}
		if (delayU && isDownU)
		{
			currentTimeU = GetTickCount();
			if (currentTimeU - lastKeyDownTimeU <= longKeyTime)
			{
				if (!continueU)
				{
					SetKeyTable(UP, true);
					isDownU = true;
					delayU = true;
				}


			}
			else continueU = false;
		}

	}
	else
	{
		currentTimeU = GetTickCount();
		if (currentTimeU - lastKeyDownTimeU > shortKeyTime)
		{
			isDownU = false;
			delayU = false;
			continueU = false;
		}
		else
		{
			SetKeyTable(UP, true);
			isDownU = true;
			delayU = false;
			continueU = true;
		}

	}

	static bool continueD;
	static bool delayD;
	static bool isDownD;
	static DWORD lastKeyDownTimeD = 0;
	static DWORD currentTimeD = 0;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState(0x46) & 0x8000)
	{

		if (!delayD && !isDownD && !continueD)
		{
			SetKeyTable(DOWN, true);
			isDownD = true;
			delayD = true;

			lastKeyDownTimeD = GetTickCount();
		}
		if (delayD && isDownD)
		{
			currentTimeD = GetTickCount();
			if (currentTimeD - lastKeyDownTimeD <= longKeyTime)
			{
				if (!continueD)
				{
					SetKeyTable(DOWN, true);
					isDownD = true;
					delayD = true;
				}


			}
			else continueD = false;
		}

	}
	else
	{
		currentTimeD = GetTickCount();
		if (currentTimeD - lastKeyDownTimeD > shortKeyTime)
		{
			isDownD = false;
			delayD = false;
			continueD = false;
		}
		else
		{
			SetKeyTable(DOWN, true);
			isDownD = true;
			delayD = false;
			continueD = true;
		}

	}

}



/// <summary>
/// �޴��� ����ϴ� �Է� �ý���
/// Ű�� �����Է��� ���� �ʴ´�
/// </summary>
void UpdateMenuInput()
{
	static bool tempS;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!tempS)
		{
			SetKeyTable(SPACE, true);
		}
		tempS = true;
	}
	else
	{
		tempS = false;
	}


	static bool tempL;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (!tempL)
		{
			SetKeyTable(LEFT, true);
		}
		tempL = true;
	}
	else
	{
		tempL = false;
	}


	static bool tempR;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (!tempR)
		{
			SetKeyTable(RIGHT, true);
		}
		tempR = true;
	}
	else
	{
		tempR = false;
	}


}



