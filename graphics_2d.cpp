#ifndef ENG_GRAPHICS_2D
#define ENG_GRAPHICS_2D

#include <algorithm>
#include <math.h>
#include <ncurses.h>
#include "graphics_3d.cpp"

void drawLine(int y1, int x1, int y2, int x2, char c, bool glitch)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

    if (glitch && y1 == y2) {
		for(int X = x1; X <= x2; X++) mvaddch(X, y1, c);
		return;
	}

	if (glitch && x1 == x2) {
		for (int Y = y1; Y <= y2; Y++) mvaddch(x1, Y, c);
		return;
	}

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
		mvaddch(x, y, c);
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
			mvaddch(x, y, c);
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
		mvaddch(x, y, c);
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
			mvaddch(x, y, c);
		}
	}
}

void drawTri(int x0, int y0, int x1, int y1, int x2, int y2, char c, bool glitch) {
	drawLine(x0, y0, x1, y1, c, glitch);
	drawLine(x1, y1, x2, y2, c, glitch);
	drawLine(x2, y2, x0, y0, c, glitch);
}

void fillBottomFlatTriangle(vec2d v1, vec2d v2, vec2d v3, char c, bool glitch)
{
	float invslope1 = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
	float invslope2 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

	float curx1 = v1.x;
	float curx2 = v1.x;

	for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
	{
		drawLine((int)curx1, scanlineY, (int)curx2, scanlineY, c, glitch);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void fillTopFlatTriangle(vec2d v1, vec2d v2, vec2d v3, char c, bool glitch)
{
	float invslope1 = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
	float invslope2 = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

	float curx1 = v3.x;
	float curx2 = v3.x;

	for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
	{
		drawLine((int)curx2, scanlineY, (int)curx1, scanlineY, c, glitch);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void fillTri(int x0, int y0, int x1, int y1, int x2, int y2, char c, bool glitch)
{
	vec2d v1, v2, v3;

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
	
	if (v2.y == v3.y)
	{
		fillBottomFlatTriangle(v1, v2, v3, c, glitch);
	}
	/* check for trivial case of top-flat triangle */
	else if (v1.y == v2.y)
	{
		fillTopFlatTriangle(v1, v2, v3, c, glitch);
	}
	else
	{
		/* general case - split the triangle in a topflat and bottom-flat one */
		vec2d v4 = { (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y };
		fillBottomFlatTriangle(v1, v2, v4, c, glitch);
		fillTopFlatTriangle(v2, v4, v3, c, glitch);
	}
}

#endif /* ENG_GRAPHICS_2D */
