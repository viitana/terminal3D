#ifndef ENG_GRAPHICS_3D
#define ENG_GRAPHICS_3D

#include <vector>
#include <math.h>

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
	float m[4][4] = { 0 }; // row, col
};

struct vec2d
{
	int x, y;
};

void multVecMat(vec3d& i, vec3d& o, matrix4x4& m) {
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

#endif /* ENG_GRAPHICS_3D */
