#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 30;
int nMapWidth = 16;
int nMapHeight = 16;

float PlayerX = 1.0;
float PlayerY = 1.0;
float PlayerA = 0.0;
float FOV = 3.14159 / 4.0;
float Depth = 20.0;
float Speed = 2.0;
float povorot = 0.0;
float norma = 3.14159 / 2.0;

int main()
{

	POINT p;

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;
	map += L"################";
	map += L"#              #";
	map += L"#              #";
	map += L"#              #";
	map += L"#              #";
	map += L"#      ##      #";
	map += L"#      ##      #";
	map += L"#              #";
	map += L"#              #";
	map += L"#              #";
	map += L"#      ##      #";
	map += L"#      ##      #";
	map += L"#              #";
	map += L"#              #";
	map += L"#              #";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (true)
	{

		GetCursorPos(&p);
		float coordx = p.x;
		PlayerA = coordx / 100 + povorot * 4;

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float ElapsedTime = elapsedTime.count();

		GetCursorPos(&p);
		if (p.x > 1534) {
			povorot += (Speed * 0.75f) * ElapsedTime;
			SetCursorPos(1533, p.y);
		}
		if ((p.x < 1)) {
			povorot -= (Speed * 0.75f) * ElapsedTime;
			SetCursorPos(2, p.y);
		}
		if ((p.y > 860)) {
			SetCursorPos(p.x, 855);
		}
		if ((p.y < 5)) {
			SetCursorPos(p.x, 10);
		}

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			PlayerX += sin(PlayerA - norma) * Speed * ElapsedTime;;
			PlayerY += cos(PlayerA - norma) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#')
			{
				PlayerX -= sinf(PlayerA - norma) * Speed * ElapsedTime;;
				PlayerY -= cosf(PlayerA - norma) * Speed * ElapsedTime;;
			}
		}

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			PlayerX -= sinf(PlayerA - norma) * Speed * ElapsedTime;;
			PlayerY -= cosf(PlayerA - norma) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#')
			{
				PlayerX += sinf(PlayerA - norma) * Speed * ElapsedTime;;
				PlayerY += cosf(PlayerA - norma) * Speed * ElapsedTime;;
			}
		}

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			PlayerX += sin(PlayerA) * Speed * ElapsedTime;;
			PlayerY += cos(PlayerA) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#')
			{
				PlayerX -= sin(PlayerA) * Speed * ElapsedTime;;
				PlayerY -= cos(PlayerA) * Speed * ElapsedTime;;
			}
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			PlayerX -= sinf(PlayerA) * Speed * ElapsedTime;;
			PlayerY -= cosf(PlayerA) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#')
			{
				PlayerX += sinf(PlayerA) * Speed * ElapsedTime;;
				PlayerY += cosf(PlayerA) * Speed * ElapsedTime;;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)
		{

			float fRayAngle = (PlayerA - FOV / 2.0) + ((float)x / (float)nScreenWidth) * FOV;

			float fStepSize = 0.1;
			float fDistanceToWall = 0.0;

			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < Depth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(PlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(PlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = Depth;
				}
				else
				{

					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{

						bHitWall = true;

						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{

								float vy = (float)nTestY + ty - PlayerY;
								float vx = (float)nTestX + tx - PlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) { bBoundary = true; }
						if (acos(p.at(1).second) < fBound) { bBoundary = true; }
						if (acos(p.at(2).second) < fBound) { bBoundary = true; }
					}
				}
			}

			int nCeiling = (float)(nScreenHeight / 1.8) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';
			if (fDistanceToWall <= Depth / 4.0) {
				nShade = 0x2588;
			}
			else if (fDistanceToWall < Depth / 3.0) {
				nShade = 0x2593;
			}
			else if (fDistanceToWall < Depth / 2.0) {
				nShade = 0x2592;
			}
			else if (fDistanceToWall < Depth) {
				nShade = 0x2591;
			}
			else {
				nShade = ' ';
			}

			if (bBoundary)		nShade = ' ';

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling) {
					screen[y * nScreenWidth + x] = ' ';
				}
				else if (y > nCeiling && y <= nFloor) {
					screen[y * nScreenWidth + x] = nShade;
				}
				else
				{
					float distfloor = 1.0 - (((float)y - nScreenHeight / 2.0) / ((float)nScreenHeight / 2.0));
					if (distfloor < 0.25) {
						nShade = '=';
					}
					else if (distfloor < 0.5) {
						nShade = ':';
					}
					else if (distfloor < 0.75) {
						nShade = '.';
					}
					else if (distfloor < 0.9) {
						nShade = '-';
					}
					else {
						nShade = ' ';
					}
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=3.2f ", PlayerX, PlayerY, PlayerA, 1.0 / ElapsedTime);

		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		if (sin(PlayerA) > -0.785 and sin(PlayerA) < 0.785) {
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '>';
		}
		if (sin(PlayerA) > 0.785 and sin(PlayerA) < 2.355) {
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = 'v';
		}
		if (abs(sin(PlayerA)) > 2.355 and abs(sin(PlayerA)) < 3.925) {
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '<';
		}
		if (abs(sin(PlayerA)) > 3.925 and abs(sin(PlayerA) < 5.495)) {
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '^';
		}


		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}