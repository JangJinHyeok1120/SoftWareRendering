
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "DataStruct.h"
#include "Renderer.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);
void DrawLine(Vertex3 vec1, Vertex3 vec2, int density);
void FillTriangle(Vertex3 vec1, Vertex3 vec2, Vertex3 vec3, int density);
bool GetMeetPoint(Vertex3 vec1, Vertex3 vec2, Vertex3 vec3, Vertex3 vec4, Vertex3& returnVec);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);


	Vertex3 vec1(0, 0, 0);
	Vertex3 vec2(100.0f, 0, 0);
	Vertex3 vec3(-50.0f, 50.0f, 0);

	FillTriangle(vec1, vec2, vec3, 100);

	// Buffer Swap 
	BufferSwap();
}

void FillTriangle(Vertex3 vec1, Vertex3 vec2, Vertex3 vec3,int density)
{
	DrawLine(vec1, vec2, density);
	DrawLine(vec2, vec3, density);
	DrawLine(vec3, vec1, density);

	float TopY;
	float LeftX;
	float RightX;
	float BottomY;
	
	TopY = vec1.y > vec2.y ? vec1.y : vec2.y;
	TopY = vec3.y > TopY ? vec3.y : TopY;

	LeftX = vec1.x < vec2.x ? vec1.x : vec2.x;
	LeftX = vec3.x < LeftX ? vec3.x : LeftX;

	RightX = vec1.x > vec2.x ? vec1.x : vec2.x;
	RightX = vec3.x > RightX ? vec3.x : RightX;

	BottomY = vec1.y < vec2.y ? vec1.y : vec2.y;
	BottomY = vec3.y < BottomY ? vec3.y : BottomY;
	
	float ScanY = (TopY - BottomY) / density;

	Vertex3 LeftVertex(LeftX, 0, 0);
	Vertex3 RigthVertex(RightX,0,0);
	Vertex3 temp(0, 0, 0);
	Vertex3 temp2(0, 0, 0);

	for (int i = 0; i < density; i++)
	{
		LeftVertex.y = TopY -  ScanY * (i + 1);
		RigthVertex.y = TopY - ScanY * (i + 1);

		if (GetMeetPoint(LeftVertex, RigthVertex, vec1, vec2, temp))
		{
			if (GetMeetPoint(LeftVertex, RigthVertex, vec2, vec3, temp2))
			{
				DrawLine(temp, temp2, density);
			}
			else if (GetMeetPoint(LeftVertex, RigthVertex, vec1, vec3, temp2))
			{
				DrawLine(temp, temp2, density);
			}
		}
		else
		{
			if (GetMeetPoint(LeftVertex, RigthVertex, vec2, vec3, temp))
			{
				if (GetMeetPoint(LeftVertex, RigthVertex, vec1, vec3, temp2))
				{
					DrawLine(temp, temp2, density);
				}
			}
			
		}
	}
}

bool GetMeetPoint(Vertex3 vec1, Vertex3 vec2, Vertex3 vec3, Vertex3 vec4,Vertex3& returnVec)
{
	float t;
	float s;

	float under = (vec4.y - vec3.y) * (vec2.x - vec1.x) - (vec4.x - vec3.x)*(vec2.y - vec1.y);
	if (under == 0)
		return false;

	float _t = (vec4.x - vec3.x) * (vec1.y - vec3.y) - (vec4.y - vec3.y)*(vec1.x - vec3.x);
	float _s = (vec2.x - vec1.x) * (vec1.y - vec3.y) - (vec2.y - vec1.y) * (vec1.x - vec3.x);

	t = _t / under;
	s = _s / under;

	if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f)
		return false;
	if (_t == 0 && _s == 0)
		return false;

	returnVec.x = vec1.x + t * (vec2.x - vec1.x);
	returnVec.y = vec1.y + t * (vec2.y - vec1.y);

	return true;

}

void DrawLine(Vertex3 vec1, Vertex3 vec2,int density)
{
	float tempX = (vec2.x - vec1.x) / density;
	float tempY = (vec2.y - vec1.y) / density;

	float drawX;
	float drawY;

	for (int i = 0; i < density; i++)
	{
		/*if (vec1.x + tempX * (i + 1) > vec2.x)
			drawX = vec2.x;
		else*/
			drawX = vec1.x + tempX * (i + 1);

		/*if (vec1.y + tempY * (i + 1) > vec2.y)
			drawY = vec2.y;
		else*/
			drawY = vec1.y + tempY * (i + 1);

		PutPixel(drawX, drawY);


	}
}

void Draw()
{
	Mesh m;
	m.VertexList.push_front(Vertex3(0, 0, 0));
	m.VertexList.push_front(Vertex3(1.0f, 0, 0));
	m.VertexList.push_front(Vertex3(0.5f, 0.5f, 0));

	m.IndexList.push_front(0);
	m.IndexList.push_front(1);
	m.IndexList.push_front(2);



}
