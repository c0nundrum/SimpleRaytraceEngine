//Copyright(C) 2018  Javidx9
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under certain conditions; See license for details.
//Original works located at :
//https://www.github.com/onelonecoder
//https://www.onelonecoder.com
//https://www.youtube.com/javidx9
//GNU GPLv3
//https ://github.com/OneLoneCoder/videos/blob/master/LICENSE
//*********************************************************************//
//Based On that awesome tutorial, with a few hacks

#include "pch.h"
#include <utility>
#include <algorithm>
using namespace std;

#include "olcConsoleGameEngine.h"

class OneLoneCoder_UltimateFPS : public olcConsoleGameEngine {
public:
	OneLoneCoder_UltimateFPS() {
		m_sAppName = L"Ultimate First Person Shooter";
	}

	virtual bool OnUserCreate() {

		map += L"#########.......#########.......";
		map += L"#...............#...............";
		map += L"#.......#########.......########";
		map += L"#..............##..............#";
		map += L"#......##......##......##......#";
		map += L"#......##..............##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#..............##..............#";
		map += L"#...........#####...........####";
		map += L"#..............................#";
		map += L"###..####....########....#######";
		map += L"####.####.......######..........";
		map += L"#...............#...............";
		map += L"#.......#########.......##..####";
		map += L"#..............##..............#";
		map += L"#......##......##.......#......#";
		map += L"#......##......##......##......#";
		map += L"#..............##..............#";
		map += L"###............####............#";
		map += L"##.............###.............#";
		map += L"#............####............###";
		map += L"#..............................#";
		map += L"#..............................#";
		map += L"#..............##..............#";
		map += L"#...........##..............####";
		map += L"#..............##..............#";
		map += L"################################";

		spriteWall = new olcSprite(L"fps_wall1.spr");
		spriteLamp = new olcSprite(L"fps_lamp1.spr");
		spriteFireBall = new olcSprite(L"fps_fireball1.spr");


		fDepthBuffer = new float[ScreenWidth()];

		listObjects = {
			{8.5f, 8.5f, 0.0f, 0.0f, false, spriteLamp},
			{7.5f, 7.5f, 0.0f, 0.0f, false, spriteLamp},
			{10.5f, 3.5f, 0.0f, 0.0f, false, spriteLamp},
		};

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		//Controls
		if (m_keys[L'A'].bHeld)//A key
			fPlayerA -= (0.8f) * fElapsedTime;

		if (m_keys[L'D'].bHeld)//D key
			fPlayerA += (0.8f) * fElapsedTime;

		//W Key
		if (m_keys[L'W'].bHeld) {
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {

				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			}
		}

		//S Key
		if (m_keys[L'S'].bHeld) {
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {

				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			}

		}

		//Strafe Right Key
		if (m_keys[L'E'].bHeld) {
			fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {

				fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;

			}

		}

		//Strafe Left Key
		if (m_keys[L'Q'].bHeld) {
			fPlayerX -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += sinf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') {

				fPlayerX += cosf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= sinf(fPlayerA) * 5.0f * fElapsedTime;

			}

		}

		//Fire Bullets
		if (m_keys[VK_SPACE].bReleased) {
			sObject o;
			o.x = fPlayerX;
			o.y = fPlayerY;
			float fNoise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
			o.vx = sinf(fPlayerA + fNoise) * 8.0f;
			o.vy = cosf(fPlayerA + fNoise) * 8.0f;
			o.sprite = spriteFireBall;
			o.bRemove = false;
			listObjects.push_back(o);
		}

		for (int x = 0; x < ScreenWidth(); x++) {
			// For each column, calculate the profected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;

			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			float fSampleX = 0.0f;

			while (!bHitWall && fDistanceToWall < fDepth) {

				fDistanceToWall += 0.01f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				//Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {

					bHitWall = true; // Just set the distance to maximum depth
					fDistanceToWall = fDepth;

				}
				else {
					//Ray is inbounds so test to see if the ray cell is a wall block
					if (map[nTestY * nMapWidth + nTestX] == '#') {

						bHitWall = true;

						float fBlockMidX = (float)nTestX + 0.5f;
						float fBlockMidY = (float)nTestY + 0.5f;

						float fTestPointX = fPlayerX + fEyeX * fDistanceToWall;
						float fTestPointY = fPlayerY + fEyeY * fDistanceToWall;

						float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

						if (fTestAngle >= -3.14159f * 0.25f && fTestAngle < 3.14159f * 0.25f)
							fSampleX = fTestPointY - (float)nTestY;
						if (fTestAngle >= 3.14159f * 0.25f && fTestAngle < 3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle < -3.14159f * 0.25f && fTestAngle >= -3.14159f * 0.75f)
							fSampleX = fTestPointX - (float)nTestX;
						if (fTestAngle >= 3.14159f * 0.75f || fTestAngle < -3.14159f * 0.75f)
							fSampleX = fTestPointY - (float)nTestY;

					}
				}

			}

			//Calculate distance to ceiling and floor
			int nCeiling = (float)(ScreenHeight() / 2.0f) - ScreenHeight() / ((float)fDistanceToWall);
			int nFloor = ScreenHeight() - nCeiling;

			fDepthBuffer[x] = fDistanceToWall;

			short nShade = ' ';
			short nFloorShade = ' ';

			

			for (int y = 0; y < ScreenHeight(); y++) {
				if (y <= nCeiling)//Ceiling
					Draw(x, y, L' ');
				else if (y > nCeiling && y <= nFloor)//Wall
				{
					if (fDistanceToWall < fDepth) {
						float fSampleY = ((float)y - (float)nCeiling) / ((float)nFloor - (float)nCeiling);
						Draw(x, y, spriteWall->SampleGlyph(fSampleX, fSampleY), spriteWall->SampleColour(fSampleX, fSampleY));
					}
					else {
						Draw(x, y, PIXEL_SOLID, 0);
					}

				}
				else // Floor
				{
					Draw(x, y, PIXEL_SOLID, FG_DARK_GREEN);
				}

			}

		}

		//Update and draw objects
		for (auto &object : listObjects) {

			// Update Object Physics
			object.x += object.vx * fElapsedTime;
			object.y += object.vy * fElapsedTime;

			// Check if object is inside wall - set flag for removal
			if (map[(int)object.x * nMapWidth + (int)object.y] == '#')
				object.bRemove = true;

			//Can Object be seen?
			float fVecX = object.x - fPlayerX;
			float fVecY = object.y - fPlayerY;
			float fDistanceFromPlayer = sqrtf(fVecX*fVecX + fVecY * fVecY);

			//Calculate the angle between lamp and player feet
			float fEyeX = sinf(fPlayerA);
			float fEyeY = cosf(fPlayerA);
			float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);
			if (fObjectAngle < -3.14159f)
				fObjectAngle += 2.0f * 3.13159f;
			if (fObjectAngle > 3.14159f)
				fObjectAngle -= 2.0f * 3.13159f;

			bool bInPlayerFOV = fabs(fObjectAngle) < fFOV / 2.0f;

			if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth) {

				float fObjectCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceFromPlayer);
				float fObjectFloor = ScreenHeight() - fObjectCeiling;
				float fObjectHeight = fObjectFloor - fObjectCeiling;

				float fObjectAspectRatio = (float)object.sprite->nHeight / (float)object.sprite->nWidth;
				float fObjectWidth = fObjectHeight / fObjectAspectRatio;

				float fMiddleOfObject = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float)ScreenWidth();

				for (float lx = 0; lx < fObjectWidth; lx++) {
					for (float ly = 0; ly < fObjectHeight; ly++) {
						float fSampleX = lx / fObjectWidth;
						float fSampleY = ly / fObjectHeight;
						wchar_t c = object.sprite->SampleGlyph(fSampleX, fSampleY);
						int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth / 2.0f));
						if (nObjectColumn >= 0 && nObjectColumn < ScreenWidth()) {
							if (c != L' ' && fDepthBuffer[nObjectColumn] >= fDistanceFromPlayer) {
								Draw(nObjectColumn, fObjectCeiling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
								fDepthBuffer[nObjectColumn] = fDistanceFromPlayer;
							}

						}
					}
				}

			}

		}

		// Remove dead objects from object list
		listObjects.remove_if([](sObject &o) {return o.bRemove; });

		//Display Map
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapHeight; ny++) {
				Draw(nx + 1, ny + 1, map[ny * nMapWidth + nx]);
			}
		Draw((int)fPlayerY + 1, (int)fPlayerX + 1, L'P');

		return true;
	}

private:

	float fPlayerX = 8.0f;
	float fPlayerY = 8.0f;
	float fPlayerA = 0.0f;
	wstring map;

	int nMapHeight = 32;
	int nMapWidth = 32;

	float fFOV = 3.14159 / 4.0;
	float fDepth = 16.0f;

	olcSprite *spriteWall;
	olcSprite *spriteLamp;
	olcSprite *spriteFireBall;

	float *fDepthBuffer = nullptr;

	struct sObject {
		float x;
		float y;
		float vx;
		float vy;
		bool bRemove;
		olcSprite *sprite;
	};

	list<sObject> listObjects;
};



int main()
{

	OneLoneCoder_UltimateFPS game;
	game.ConstructConsole(320, 240, 4, 4);
	game.Start();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
