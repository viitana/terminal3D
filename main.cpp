#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>

#define DELAY 10000
#define PROJNEAR 500.0f
#define PROJFAR 0.1f
#define FOV_DEG 60.0f

struct vec3d
{
	float x, y, z;
};

struct tri3d
{
	vec3d p[3];
};

struct mesh3d
{
	std::vector<tri3d> tris;
};

struct matrix4x4
{
	// row, col
	float m[4][4] = { 0 };
};

struct vec2d
{
	int x, y;
};

void mulMatVec(vec3d& i, vec3d& o, matrix4x4& m) {
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f) {
		o.x /= w; o.y /= w; o.z /= w;
	}
}

void unitVectorize(vec3d vec) {
	float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= length;
	vec.y /= length;
	vec.z /= length;
}

vec3d getNormal(tri3d tri) {
	vec3d normal, baseline1, baseline2;
	baseline1.x = tri.p[1].x - tri.p[0].x;
	baseline1.y = tri.p[1].y - tri.p[0].y;
	baseline1.z = tri.p[1].z - tri.p[0].z;

	baseline2.x = tri.p[2].x - tri.p[0].x;
	baseline2.y = tri.p[2].y - tri.p[0].y;
	baseline2.z = tri.p[2].z - tri.p[0].z;

	normal.x = baseline1.y * baseline2.z - baseline1.z * baseline2.y;
	normal.y = baseline1.z * baseline2.x - baseline1.x * baseline2.z;
	normal.z = baseline1.x * baseline2.y - baseline1.y * baseline2.x;

	unitVectorize(normal);
	return normal;
}

// basic cube
mesh3d genCube() {
	mesh3d cube;
	cube.tris = {

		{0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f},

		{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f},

		{1.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f},

		{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f},

		{0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f},

		{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f},
	};
	return cube;
}

mesh3d genCentreCube() {
	mesh3d cube;
	cube.tris = {

		{-0.5f, -0.5f, -0.5f,	-0.5f, 0.5f, -0.5f,	0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f,	0.5f, 0.5f, -0.5f,	0.5f, -0.5f, -0.5f},

		{0.5f, -0.5f, -0.5f,	0.5f, 0.5f, -0.5f,	0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f,	0.5f, 0.5f, 0.5f,	0.5f, -0.5f, 0.5f},

		{0.5f, -0.5f, 0.5f,	0.5f, 0.5f, 0.5f,	-0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f,	-0.5f, 0.5f, 0.5f,	-0.5f, -0.5f, 0.5f},

		{-0.5f, -0.5f, 0.5f,	-0.5f, 0.5f, 0.5f,	-0.5f, 0.5f, -0.5f},
		{-0.5f, -0.5f, 0.5f,	-0.5f, 0.5f, -0.5f,	-0.5f, -0.5f, -0.5f},

		{-0.5f, 0.5f, -0.5f,	-0.5f, 0.5f, 0.5f,	0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f,	0.5f, 0.5f, 0.5f,	0.5f, 0.5f, -0.5f},

		{0.5f, -0.5f, 0.5f,	-0.5f, -0.5f, 0.5f,	-0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f,	-0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f},
	};
	return cube;
}

void genRotationZ(float s, matrix4x4& mat, float fTheta) {
	mat.m[0][0] = std::cos(fTheta * s);
	mat.m[0][1] = std::sin(fTheta * s);
	mat.m[1][0] = - std::sin(fTheta * s);
	mat.m[1][1] = std::cos(fTheta * s);
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
}

void genRotationX(float s, matrix4x4& mat, float fTheta) {
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = std::cos(fTheta * s);
	mat.m[1][2] = std::sin(fTheta * s);
	mat.m[2][1] = -std::sin(fTheta * s);
	mat.m[2][2] = std::cos(fTheta * s);
	mat.m[3][3] = 1.0f;
}

void genScale(float s, matrix4x4& mat) {
	mat.m[0][0] = s;
	mat.m[1][1] = s;
	mat.m[2][2] = s;
	mat.m[3][3] = s;
}

void genProjection(int w, int h, matrix4x4 &mat) {
	float aspectRatio = (float)h / (float)w;
	float fovRad = 1.0f / std::tan(FOV_DEG * 0.5f / 180.0f * 3.14159f);

	mat.m[0][0] = aspectRatio * fovRad;
	mat.m[1][1] = fovRad;
	mat.m[2][2] = PROJFAR / (PROJFAR - PROJNEAR);
	mat.m[3][2] = (-PROJFAR * PROJNEAR) / (PROJFAR - PROJNEAR);
	mat.m[2][3] = 1.0f;
	mat.m[3][3] = 0.0f;
};

tri3d translate(tri3d tri, float dx, float dy, float dz) {
	tri3d translated;

	translated.p[0].x = tri.p[0].x + dx;
	translated.p[1].x = tri.p[1].x + dx;
	translated.p[2].x = tri.p[2].x + dx;

	translated.p[0].y = tri.p[0].y + dy;
	translated.p[1].y = tri.p[1].y + dy;
	translated.p[2].y = tri.p[2].y + dy;

	translated.p[0].z = tri.p[0].z + dz;
	translated.p[1].z = tri.p[1].z + dz;
	translated.p[2].z = tri.p[2].z + dz;

	return translated;
}

float dotProduct(vec3d a, vec3d b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3d getRelativePosToCam(vec3d cam, vec3d vec) {
	return { vec.x - cam.x, vec.y - cam.y, vec.z - cam.z };
}

//void drawLine(int x0, int y0, int x1, int y1)
//{
//	int dx, dy, p, x, y;
//
//	dx = x1 - x0;
//	dy = y1 - y0;
//
//	x = x0;
//	y = y0;
//
//	p = 2 * dy - dx;
//
//	while (x < x1)
//	{
//		if (p >= 0)
//		{
//			mvprintw(y, x, "#");
//			y = y + 1;
//			p = p + 2 * dy - 2 * dx;
//		}
//		else
//		{
//			mvprintw(y, x, "#");
//			p = p + 2 * dy;
//		}
//		x = x + 1;
//	}
//}

void drawLine(int y1, int x1, int y2, int x2)
{
	if (y1 == y2) {
		for(int X = x1; X <= x2; X++) mvprintw(X, y1, "#");
		return;
	}

	if (x1 == x2) {
		for (int Y = y1; Y <= y2; Y++) mvprintw(x1, Y, "#");
		return;
	}

	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		mvprintw(x, y, "#");
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			mvprintw(x, y, "#");
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		mvprintw(x, y, "#");
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			mvprintw(x, y, "#");
		}
	}
}

void drawTri(int x0, int y0, int x1, int y1, int x2, int y2) {
	drawLine(x0, y0, x1, y1);
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x0, y0);
}

bool stSec(std::tuple<int, int> a, std::tuple<int, int> b) {
	return (std::get<1>(a) > std::get<1>(b));
}

void fillBottomFlatTriangle(vec2d v1, vec2d v2, vec2d v3)
{
	float invslope1 = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
	float invslope2 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

	float curx1 = v1.x;
	float curx2 = v1.x;

	for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
	{
		drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void fillTopFlatTriangle(vec2d v1, vec2d v2, vec2d v3)
{
	float invslope1 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
	float invslope2 = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

	float curx1 = v3.x;
	float curx2 = v3.x;

	for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
	{
		drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void fillTri(int x0, int y0, int x1, int y1, int x2, int y2) {
	//std::vector<std::tuple<int, int> > v;
	//v.push_back(std::make_tuple(x0, y0));
	//v.push_back(std::make_tuple(x1, y1));
	//v.push_back(std::make_tuple(x2, y2));

	vec2d v1, v2, v3;
	//v3.x = std::get<0>(v.back());
	//v3.y = std::get<1>(v.back());
	//v.pop_back();

	//v2.x = std::get<0>(v.back());
	//v2.y = std::get<1>(v.back());
	//v.pop_back();

	//v1.x = std::get<0>(v.back());
	//v1.y = std::get<1>(v.back());
	//v.pop_back();



	if (y0 <= y1)
	{
		if (y0 <= y2)
		{
			if (y1 <= y2) // y0 < y1 < y2
			{
				v1.x = x0; v1.y = y0;
				v2.x = x1; v2.y = y1;
				v3.x = x2; v3.y = y2;
			}
			else // y0 < y2 < y1
			{
				v1.x = x0; v1.y = y0;
				v2.x = x2; v2.y = y2;
				v3.x = x1; v3.y = y1;
			}
		}
		else // y2 < y0 < y1
		{
			v1.x = x2; v1.y = y2;
			v2.x = x0; v2.y = y0;
			v3.x = x1; v3.y = y1;
		}
	}
	else
	{
		if (y0 <= y2) // y1 < y0 < y2
		{
			v1.x = x1; v1.y = y1;
			v2.x = x0; v2.y = y0;
			v3.x = x2; v3.y = y2;
		}
		else
		{
			if (y1 <= y2) // y1 < y2 < y0
			{
				v1.x = x1; v1.y = y1;
				v2.x = x2; v2.y = y2;
				v3.x = x0; v3.y = y0;
			}
			else // y2 < y1 < y0
			{
				v1.x = x2; v1.y = y2;
				v2.x = x1; v2.y = y1;
				v3.x = x0; v3.y = y0;
			}
		}
	}

	//std::sort(v.begin(), v.end(), stSec);
	//for (int i = 0; i < v.size(); i++)
	
	if (v2.y == v3.y)
	{
		fillBottomFlatTriangle(v1, v2, v3);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		fillTopFlatTriangle(v1, v2, v3);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		vec2d v4 = { (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y };
		fillBottomFlatTriangle(v1, v2, v4);
		fillTopFlatTriangle(v2, v4, v3);
	}
}

void initColors() {
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
}

int main()
{
	//if (has_colors() == FALSE) {
	//	endwin();
	//	printf("Your terminal does not support color output :(\n");
	//	exit(1);
	//}

	auto begin = std::chrono::high_resolution_clock::now();
	int max_y = 0, max_x = 0;
	float fTheta = 0.0f;

	vec3d camera = { 0.0f, 0.0f, 0.0f };

	vec3d illuminationLight = { 0.0f, 0.0f, -1.0f };
	unitVectorize(illuminationLight);

	int n = 0;

	use_default_colors();
	initscr();
	cbreak();
	noecho();

	curs_set(FALSE);

	while (1) {
		getmaxyx(stdscr, max_y, max_x);
		clear();
		n += 1;
		printw("Atte's brilliant graphics engine.\nBehold the mighty shapes!\nx:%d, y:%d\nTotal frames: %d", max_x, max_y, n);

		mesh3d cube = genCentreCube();
		matrix4x4 projection, rotZ, rotX, scale;

		genScale(2.0f, scale);

		genProjection(max_x, max_y, projection);

		auto now = std::chrono::high_resolution_clock::now();
		auto dur = begin - now;
		fTheta = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() / 1000.f;

		genRotationX(0.5f, rotX, fTheta);
		genRotationZ(1.0f, rotZ, fTheta);

		// Draw mesh triangles
		for (auto tri : cube.tris) {

			// Projection
			tri3d triProjected, triTranslated, triRotatedZ, triRotatedZX, triScaled;

			// Rotate Z
			mulMatVec(tri.p[0], triRotatedZ.p[0], rotZ);
			mulMatVec(tri.p[1], triRotatedZ.p[1], rotZ);
			mulMatVec(tri.p[2], triRotatedZ.p[2], rotZ);

			// Rotate X
			mulMatVec(triRotatedZ.p[0], triRotatedZX.p[0], rotX);
			mulMatVec(triRotatedZ.p[1], triRotatedZX.p[1], rotX);
			mulMatVec(triRotatedZ.p[2], triRotatedZX.p[2], rotX);

			triTranslated = translate(triRotatedZX, 0.0f, 0.0f, 2.0f);

			vec3d normal = getNormal(triRotatedZX);
			vec3d triPos = getRelativePosToCam(camera, triTranslated.p[0]);

			// Only do 3D projection if tri is visible
			if (dotProduct(normal, triPos) < 0.0f) {

				// Illumination
				float dotProd = dotProduct(normal, illuminationLight);

				// Project
				mulMatVec(triTranslated.p[0], triProjected.p[0], projection);
				mulMatVec(triTranslated.p[1], triProjected.p[1], projection);
				mulMatVec(triTranslated.p[2], triProjected.p[2], projection);

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

				//drawTri(
				//	triProjected.p[0].x,
				//	triProjected.p[0].y,
				//	triProjected.p[1].x,
				//	triProjected.p[1].y,
				//	triProjected.p[2].x,
				//	triProjected.p[2].y);

				//fillTri(
				//	triProjected.p[0].x,
				//	triProjected.p[0].y,
				//	triProjected.p[1].x,
				//	triProjected.p[1].y,
				//	triProjected.p[2].x,
				//	triProjected.p[2].y);
			}
		}

		//fillTri(10, 10, 20, 30, 60, 30);
		//drawTri(70, 10, 80, 30, 120, 30);

		//fillTri(30, 10, 10, 40, 20, 50);
		//fillTri(10, 40, 30, 10, 20, 50);
		//fillTri(20, 50, 10, 40, 30, 10);
		fillTri(30, 10, 20, 50, 10, 40);



		//fillTri(30, 80, 10, 40, 20, 50);
		
		refresh();
		usleep(DELAY);
	}

	endwin();

    return 0;
}