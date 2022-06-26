#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>        //ЕСЛИ ЕСТЬ ПРОБЛЕМА С ИЗОБРАЖЕНИЕ, ТО НАДО РАСТЯНУТЬ НА ВЕСЬ ЭКРАН ИЛИ ПОСТАВИТЬ РАЗРЕШЕИЕ КОНСОЛЬНОГО ОКНА
#include <chrono>           //КАК НАПИСАНО НИЖЕ В 11 СТРОКЕ
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 120;   // Разрешение на весь экран 213 (просто надо растянуть на весь экран) (если надо небольшое консольное окно, то можно поставить 120)
int nScreenHeight = 50;   //Высота консоли
int nMapWidth = 16;     //Задают размеры карты, по которой передвигается игрок
int nMapHeight = 16;
						//Задание данных для положения игрока 
float PlayerX = 1.0;   //Положение играко по координатам x
float PlayerY = 1.0;   //Положение игрока по координатам y
float PlayerA = 0.0;   //Положение игрока по координатам z
float FOV = 3.14159 / 4.0;   //Отечает за поле зрения 
float Depth = 20.0;   //Задает дальность прорисовки дальних объектов
float Speed = 2.0;    //Задает скорость передвижения игрока
float povorot = 0.0;    //Переменная необходимая для повора камеры, если курсор ушел за пределы экрана
float norma = 3.14159 / 2.0;    //Переменная которая нормализует поворот, для правильного перемещения вправо и влево

//В данной функции создается карта из массива и добавляется в буфер консоли.
//При нажатии на клавиатуре на клавишу w, игрок начнет двигаться вперед
//При нажатии на клавиатуре на клавишу s, игрок начнет двигаться назад
//При нажатии на клавиатуре на клавишу a, игрок начнет двигаться влево
//При нажатии на клавиатуре на клавишу d, игрок начнет двигаться вправо
//Также поворот камеры осуществляется управлением мыши, даже при выходе за границы экрана, игрок всее равно будет поворачиватся в стороны
//После по технологии raycasting из точки, где находится игрок, будут выпускатся векторы, по мини карте. Таким образом будет определятся расстояние до препядствий.
//И чем больше вектор, тем дальше объект и тем меньше он будет выводится в консоль
//После определения длин векторов будет получатся картикна в виде 3D изображения
//Также объекты на разном расстояние будут отображаться различными символами, с различной яркостью
//После всего этого полученные данные будут выводится из буфера консоли в неё саму

int main(float PlayerX, float PlayerY, float PlayerA)
{

	POINT p;

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wchar_t s[3];

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
			SetCursorPos(1533, p.y);}
		if ((p.x < 1)) {
			povorot -= (Speed * 0.75f) * ElapsedTime;
			SetCursorPos(2, p.y);}
		if ((p.y > 860)) {
			SetCursorPos(p.x, 855);}
		if ((p.y < 5)) {
			SetCursorPos(p.x, 10);}

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) 
		{
			PlayerX += sin(PlayerA - norma) * Speed * ElapsedTime;;
			PlayerY += cos(PlayerA - norma) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#') 
			{
				PlayerX -= sinf(PlayerA - norma) * Speed * ElapsedTime;;
				PlayerY -= cosf(PlayerA - norma) * Speed * ElapsedTime;;}
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) 
		{
			PlayerX -= sinf(PlayerA - norma) * Speed * ElapsedTime;;
			PlayerY -= cosf(PlayerA - norma) * Speed * ElapsedTime;;
			if (map.c_str()[(int)PlayerX * nMapWidth + (int)PlayerY] == '#') 
			{
				PlayerX += sinf(PlayerA - norma) * Speed * ElapsedTime;;
				PlayerY += cosf(PlayerA - norma) * Speed * ElapsedTime;;}
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

		for (int x = 0; x < nScreenWidth; x++) {

			float fRayAngle = (PlayerA - FOV / 2.0) + ((float)x / (float)nScreenWidth) * FOV;

			float fStepSize = 0.1;
			float fDistanceToWall = 0.0;

			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < Depth) {
				fDistanceToWall += fStepSize;
				int nTestX = (int)(PlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(PlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = Depth; }
				else {
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') {
						bHitWall = true;
						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++) {

								float vy = (float)nTestY + ty - PlayerY;
								float vx = (float)nTestX + tx - PlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot)); }

						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						float fBound = 0.004;
						if (acos(p.at(0).second) < fBound) { bBoundary = true; }
						if (acos(p.at(1).second) < fBound) { bBoundary = true; }
						if (acos(p.at(2).second) < fBound) { bBoundary = true; }
					}
				}
			}

			int nCeiling = (float)(nScreenHeight / 1.8) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';
			if (fDistanceToWall <= Depth / 4.0) 
			{
				nShade = 0x2588; }   //Препядствие очень близко 
			else if (fDistanceToWall < Depth / 3.0) 
			{
				nShade = 0x2593; }
			else if (fDistanceToWall < Depth / 2.0) 
			{
				nShade = 0x2592; }
			else if (fDistanceToWall < Depth) 
			{
				nShade = 0x2591; }   //Препядствие очень далеко
			
			else {
				nShade = ' ';}   //Препядствия слишком далеко чтобы оно было видно

			if (bBoundary)		nShade = '|';

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling) {
					screen[y * nScreenWidth + x] = ' '; }
				else if (y > nCeiling && y <= nFloor) {
					screen[y * nScreenWidth + x] = nShade; }
				else 
				{

					float distfloor = 1.0 - (((float)y - nScreenHeight / 2.0) / ((float)nScreenHeight / 2.0));
					if (distfloor < 0.25) {
						nShade = '=';   //Пол почти под игроком
					}
					else if (distfloor < 0.5) {
						nShade = ';'; }
					else if (distfloor < 0.75) {
						nShade = ':'; }
					else if (distfloor < 0.9) {
						nShade = '.'; }
					else {
						nShade = ' '; }
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", PlayerX, PlayerY, PlayerA, 1.0 / ElapsedTime);

		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++) {
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx]; }

		if ((cos(PlayerA) > 0.7) and ((sin(PlayerA) < 0.7) and (sin(PlayerA) > -0.7))) 
		{
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '>';}   //Если игрок смотрит вправо
		if ((cos(PlayerA) < -0.7) and ((sin(PlayerA) < 0.7) and (sin(PlayerA) > -0.7))) 
		{
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '<';}   //Если игрок смотрит влево
		if ((sin(PlayerA) > 0.7) and ((cos(PlayerA) < 0.7) and (cos(PlayerA) > -0.7))) 
		{
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = 'v';}   //Если игрок смотрит вниз
		if ((sin(PlayerA) < -0.7) and ((cos(PlayerA) < 0.7) and (cos(PlayerA) > -0.7))) 
		{
			screen[((int)PlayerX + 1) * nScreenWidth + (int)PlayerY] = '^';}   //Если игрок смотрит вверх

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	return 0;
return 1;
}