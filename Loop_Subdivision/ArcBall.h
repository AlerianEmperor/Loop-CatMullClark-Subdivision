#ifndef _ARC_BALL_H_
#define _ARC_BALL_H_
#include "vec3.h"
#include <gl\GLU.H>
#include <gl\glut.h>
#include <gl\GL.H>
#define winWidth 800
#define winHeight 600
#define halfWidth winWidth / 2
#define halfHeight winHeight / 2;
#define iWidth 1 / winWidth
#define iHeight 1 / winHeight

int lastX = 0, lastY = 0, curX = 0, curY = 0;
float angle;
vec3 rotation_axis;
float rotation_matrix[16];
bool arcball_on = false;

vec3 arcball_coordinate(const int& x, const int& y)
{
	float cx = 2 * x * iWidth - 1.0f;
	float cy = 1.0f - 2 * y * iHeight;

	float d = cx * cx + cy * cy;

	float z;
	if (d < 0.5f)
		z = sqrt14(1.0f - d);
	else
		z = 0.5f / sqrt14(d);

	return vec3(cx, cy, z);
}

void compute_axis()
{
	if (curX != lastX || curY != lastY)
	{
		vec3 a = arcball_coordinate(lastX, lastY);
		vec3 b = arcball_coordinate(curX, curY);

		a.normalize();
		b.normalize();

		rotation_axis = a.cross(b);

		float dot = a.dot(b);

		angle = acosf(dot) * pi / 180;

		//lastX = curX;
		//lastY = curY;
	}
}

void build_rotation_matrix()
{
	float qr = angle;
	float qi = rotation_axis.x;
	float qj = rotation_axis.y;
	float qk = rotation_axis.z;

	rotation_matrix[0] = 1.0f - 2.0f * (qj * qj + qk * qk);
	rotation_matrix[1] = 2.0f * (qi * qj - qk * qr);
	rotation_matrix[2] = 2.0f * (qi * qk + qj * qr);
	rotation_matrix[3] = 0.0f;

	rotation_matrix[4] = 2.0f * (qi * qj + qk * qr);
	rotation_matrix[5] = 1 - 2.0f * (qi * qi + qk * qk);
	rotation_matrix[6] = 2.0f * (qj * qk - qi * qr);
	rotation_matrix[7] = 0.0f;

	rotation_matrix[8] = 2.0f * (qi * qk - qj * qr);
	rotation_matrix[9] = 2.0f * (qj * qk + qi * qr);
	rotation_matrix[10] = 1.0f - 2.0f * (qi * qi + qj * qj);
	rotation_matrix[11] = 0.0f;

	rotation_matrix[12] = 0.0f;
	rotation_matrix[13] = 0.0f;
	rotation_matrix[14] = 0.0f;
	rotation_matrix[15] = 1.0f;
}

void onMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		arcball_on = true;
		lastX = curX = x;
		lastY = curY = y;
	}
	else
		arcball_on = false;
}

void onMotion(int x, int y)
{
	if (arcball_on)
	{
		curX = x;
		curY = y;
	}
}

#endif // !_ARCBALL_H_
