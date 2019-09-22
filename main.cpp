#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <chrono>
#include <bits/stdc++.h>
#include "menu.cpp"
#include "graphics_2d.cpp"
#include "graphics_3d.cpp"

void initColors() {
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
}

void drawGraphics(vec3d camera, vec3d illuminationLight, mesh3d cube, matrix4x4 projection, matrix4x4 rotX, matrix4x4 rotZ, int max_x, int max_y, settings conf) {
	// Draw mesh triangles
	for (auto tri : cube.tris) {

		// Projection
		tri3d triProjected, triTranslated, triRotatedZ, triRotatedZX, triScaled;

		// Rotate Z
		multVecMat(tri.p[0], triRotatedZ.p[0], rotZ);
		multVecMat(tri.p[1], triRotatedZ.p[1], rotZ);
		multVecMat(tri.p[2], triRotatedZ.p[2], rotZ);

		// Rotate X
		multVecMat(triRotatedZ.p[0], triRotatedZX.p[0], rotX);
		multVecMat(triRotatedZ.p[1], triRotatedZX.p[1], rotX);
		multVecMat(triRotatedZ.p[2], triRotatedZX.p[2], rotX);

		triTranslated = translate(triRotatedZX, 0.0f, 0.0f, 2.0f);

		vec3d normal = getNormal(triRotatedZX);
		vec3d triPos = getRelativePosToCam(camera, triTranslated.p[0]);

		// Only do 3D projection if tri is visible
		if (dotProduct(normal, triPos) < 0.0f) {

			// Illumination
			float dotProd = dotProduct(normal, illuminationLight);

			// Project
			multVecMat(triTranslated.p[0], triProjected.p[0], projection);
			multVecMat(triTranslated.p[1], triProjected.p[1], projection);
			multVecMat(triTranslated.p[2], triProjected.p[2], projection);

			// Prescale in viewport
			triProjected.p[0].x *= 2.0f;
			triProjected.p[1].x *= 2.0f;
			triProjected.p[2].x *= 2.0f;

			// Scaling to viewport
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * (float)max_x;
			triProjected.p[0].y *= 0.5f * (float)max_y;
			triProjected.p[1].x *= 0.5f * (float)max_x;
			triProjected.p[1].y *= 0.5f * (float)max_y;
			triProjected.p[2].x *= 0.5f * (float)max_x;
			triProjected.p[2].y *= 0.5f * (float)max_y;

			if (conf.settingValues[1])
				fillTri(
					triProjected.p[0].x,
					triProjected.p[0].y,
					triProjected.p[1].x,
					triProjected.p[1].y,
					triProjected.p[2].x,
					triProjected.p[2].y, '.', conf.settingValues[3]);
			if (conf.settingValues[0])
				drawTri(
					triProjected.p[0].x,
					triProjected.p[0].y,
					triProjected.p[1].x,
					triProjected.p[1].y,
					triProjected.p[2].x,
					triProjected.p[2].y, '#', conf.settingValues[3]);
		}
	}
	refresh();
}

int main()
{
	//if (has_colors() == FALSE) {
	//	endwin();
	//	printf("Your terminal does not support color output :(\n");
	//	exit(1);
	//}

	int frames = 0;
	int framerate = 50;
	float framedelay_usec = (1.0f / (float)framerate) * 1000000.0f;

	auto begin = std::chrono::high_resolution_clock::now();
	int max_y = 0, max_x = 0;
	float fTheta = 0.0f;
	float t = 0.0;

	char* noColor = "Your terminal does not support color output :(";
	char* yesColor = "Your terminal supports color output :)";
	char* colormsg = (has_colors() == FALSE) ? noColor : yesColor;

	vec3d camera = { 0.0f, 0.0f, 0.0f };

	vec3d illuminationLight = { 0.0f, 0.0f, -1.0f };
	unitVectorize(illuminationLight);

	menu sett = {
			0,
			{
				{"Draw mesh", "Fill mesh", "Rotate", "Glitch out", "Framerate", "kek"},
				{true, false, true, false, false, false},
			}
	};

	use_default_colors();
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	int ch;

	while (1) {
		if ((ch = getch()) == ERR) {
			getmaxyx(stdscr, max_y, max_x);
			clear();

			t += 0.025f;
			frames += 1;

			printw("Atte's brilliant graphics engine.\nBehold the mighty shapes!\nx:%d, y:%d\nTotal frames: %d\n\n%s", max_x, max_y, frames, colormsg);

			drawMenu(sett, 2, 7);

			mesh3d cube = genCentreCube();
			matrix4x4 projection, rotZ, rotX, scale;

			genScale(2.0f, scale);

			genProjection(max_x, max_y, projection);

			auto now = std::chrono::high_resolution_clock::now();
			auto dur = begin - now;
			if (sett.setting.settingValues[2]) fTheta = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() / 1000.f;

			genRotationX(0.5f, rotX, fTheta);
			genRotationZ(1.0f, rotZ, fTheta);

			drawGraphics(camera, illuminationLight, cube, projection, rotX, rotZ, max_x, max_y, sett.setting);
		}
		else {
			switch (ch) {
			case KEY_RIGHT: /* user pressed backspace */
				sett.setting.settingValues[sett.cursor] ^= true; break;
			case KEY_LEFT:
				sett.setting.settingValues[sett.cursor] ^= true; break;
			case KEY_ENTER:
				sett.setting.settingValues[sett.cursor] = true; break;
			case KEY_UP:  /* user pressed up arrow key */
				sett.cursor = std::max(0, sett.cursor-1); break;
			case KEY_DOWN:  /* user pressed up arrow key */
				sett.cursor = std::min((int)sett.setting.settingLabels.size()-1 , sett.cursor+1);
			}

		}
		
		usleep((int)framedelay_usec);
	}

	endwin();

    return 0;
}
