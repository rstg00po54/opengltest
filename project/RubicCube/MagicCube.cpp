#include "stdafx.h"
#include "MainFrm.h"
#include "MagicCube.h"
#include "GLTexture.h"
#include <string.h>

#define RAD(degree) ((degree) * 3.1416f / 180.0f)

// Abbreviations of Magic Cube:
// R - Right		U - Up			F - Front
// L - Left			D - Down		B - Back
// M - Middle
// R - Right Clockwise
// R'- Right Counterclockwise
// X - (1, 0, 0)	Y - (0, 1, 0)	Z - (0, 0, 1)

void MagicCube::InitCube()
{
	InitMatrixes();
	InitPointers(m_pMatrixes);
	InitPointers(m_pColors);
	InitRotor();
	InitTextures();
}
void MagicCube::InitMatrixes()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set Centers
	// 
	// R: 0    U: 1    F: 2
	// L: 3    D: 4    B: 5
	for (int isign = 1, i = 0; i < 2; ++i)
	{
		glLoadIdentity();
		glTranslatef(isign, 0.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_centerMatrixes[i * 3]);
		glLoadIdentity();
		glTranslatef(0.0f, isign, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_centerMatrixes[i * 3 + 1]);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, isign);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_centerMatrixes[i * 3 + 2]);
		isign = -1;
	}

	// Set Edges
	// 
	//  L 
	//  |   4
	//  | 6   0
	//  |   1
	//  U - - - F
	// 
	//  L
	//  | 11  8
	//  |
	//  | 5   2
	//  E - - - F
	// 
	//  L
	//  |   10
	//  | 9   3
	//  |   7
	//  D - - - F
	for (int isign = 1, i = 0; i < 2; ++i)
	{
		for (int jsign = 1, j = 0; j < 2; ++j)
		{
			glLoadIdentity();
			glTranslatef(isign, jsign, 0.0f);
			glGetFloatv(GL_MODELVIEW_MATRIX, m_edgeMatrixes[i * 6 + j * 3]);
			glLoadIdentity();
			glTranslatef(0.0f, isign, jsign);
			glGetFloatv(GL_MODELVIEW_MATRIX, m_edgeMatrixes[i * 6 + j * 3 + 1]);
			glLoadIdentity();
			glTranslatef(jsign, 0.0f, isign);
			glGetFloatv(GL_MODELVIEW_MATRIX, m_edgeMatrixes[i * 6 + j * 3 + 2]);
			jsign = -1;
		}
		isign = -1;
	}

	// Set Vertexes
	// 
	//  L
	//  | 5   1
	//  |
	//  | 4   0
	//  U - - - F
	// 
	//  L
	//  | 7   3
	//  |
	//  | 6   2
	//  D - - - F
	for (int isign = 1, i = 0; i < 2; ++i)
	{
		for (int jsign = 1, j = 0; j < 2; ++j)
		{
			for (int ksign = 1, k = 0; k < 2; ++k)
			{
				glLoadIdentity();
				glTranslatef(isign, jsign, ksign);
				glGetFloatv(GL_MODELVIEW_MATRIX, m_vertexMatrixes[(i << 2) + (j << 1) + k]);
				ksign = -1;
			}
			jsign = -1;
		}
		isign = -1;
	}

	glPopMatrix();
}
void MagicCube::InitPointers(GLfloat* ptrs[6][9])
{
	//  5 1 6
	//  4 0 2
	//  8 3 7

	// Set Centers
	// 
	//         U * * *
	//           * 1 *
	//           * * *
	// 
	// L * * * F * * * R * * * B * * *
	//   * 5 *   * 0 *   * 2 *   * 3 *
	//   * * *   * * *   * * *   * * *
	// 
	//         D * * *
	//           * 4 *
	//           * * *

	for (MCenum i = MC_R; i <= MC_B; ++i)
	{
		ptrs[i][0] = m_centerMatrixes[i];
	}
	
	// Set Vertexes
	// 
	//         U * 4 *
	//           6 * 0
	//           * 1 *
	// 
	// L * 6 * F * 1 * R * 0 * B * 4 *
	//   11* 5   5 * 2   2 * 8   8 * 11
	//   * 9 *   * 7 *   * 3 *   * 10*
	// 
	//         D * 7 *
	//           9 * 3
	//           * 10*

	{
		InitEdgesOf(ptrs[MC_R], 0, 8, 3, 2);
		InitEdgesOf(ptrs[MC_U], 4, 0, 1, 6);
		InitEdgesOf(ptrs[MC_F], 1, 2, 7, 5);
		InitEdgesOf(ptrs[MC_L], 6, 5, 9, 11);
		InitEdgesOf(ptrs[MC_D], 7, 3, 10, 9);
		InitEdgesOf(ptrs[MC_B], 4, 11, 10, 8);
	}

	// Set Vertexes
	// 
	//         U 5 * 1
	//           * * *
	//           4 * 0
	// 
	// L 5 * 4 F 4 * 0 R 0 * 1 B 1 * 5
	//   * * *   * * *   * * *   * * *
	//   7 * 6   6 * 2   2 * 3   3 * 7
	// 
	//         D 6 * 2
	//           * * *
	//           7 * 3

	{
		InitVertexesOf(ptrs[MC_R], 0, 1, 3, 2);
		InitVertexesOf(ptrs[MC_U], 5, 1, 0, 4);
		InitVertexesOf(ptrs[MC_F], 4, 0, 2, 6);
		InitVertexesOf(ptrs[MC_L], 5, 4, 6, 7);
		InitVertexesOf(ptrs[MC_D], 6, 2, 3, 7);
		InitVertexesOf(ptrs[MC_B], 1, 5, 7, 3);
	}
}
void MagicCube::InitEdgesOf(GLfloat* ptrs[9], int e_1, int e_2, int e_3, int e_4)
{
	ptrs[1] = m_edgeMatrixes[e_1];
	ptrs[2] = m_edgeMatrixes[e_2];
	ptrs[3] = m_edgeMatrixes[e_3];
	ptrs[4] = m_edgeMatrixes[e_4];
}
void MagicCube::InitVertexesOf(GLfloat* ptrs[9], int v_1, int v_2, int v_3, int v_4)
{
	ptrs[5] = m_vertexMatrixes[v_1];
	ptrs[6] = m_vertexMatrixes[v_2];
	ptrs[7] = m_vertexMatrixes[v_3];
	ptrs[8] = m_vertexMatrixes[v_4];
}
void MagicCube::InitRotor()
{
	for (MCenum i = MC_R; i <= MC_B; ++i)
		m_rotor[i] = i;
}
void MagicCube::InitTextures()
{
	m_textures[MC_R] = GLTexture::LoadTexture("res\\Cube_R.bmp");
	m_textures[MC_U] = GLTexture::LoadTexture("res\\Cube_U.bmp");
	m_textures[MC_F] = GLTexture::LoadTexture("res\\Cube_F.bmp");
	m_textures[MC_L] = GLTexture::LoadTexture("res\\Cube_L.bmp");
	m_textures[MC_D] = GLTexture::LoadTexture("res\\Cube_D.bmp");
	m_textures[MC_B] = GLTexture::LoadTexture("res\\Cube_B.bmp");
}

void MagicCube::UpdatePointers()
{
	// 4 adjacent faces.
	// r: relative.

	MCenum rU, rR, rD, rL;

	// 4 adjacent layers each.
	// e.g. "nU = 3" means the layer in rU that contains the edge No.3 is the adjacency of m_currentOp face.

	int nU, nR, nD, nL;

	// Define acquaintant faces & layers.

	switch (m_rotorOp)
	{
	case MC_R:
		rU = MC_U;
		nU = 2;
		rR = MC_B;
		nR = 4;
		rD = MC_D;
		nD = 2;
		rL = MC_F;
		nL = 2;
		break;
	case MC_U:
		rU = MC_B;
		nU = 1;
		rR = MC_R;
		nR = 1;
		rD = MC_F;
		nD = 1;
		rL = MC_L;
		nL = 1;
		break;
	case MC_F:
		rU = MC_U;
		nU = 3;
		rR = MC_R;
		nR = 4;
		rD = MC_D;
		nD = 1;
		rL = MC_L;
		nL = 2;
		break;
	case MC_L:
		rU = MC_U;
		nU = 4;
		rR = MC_F;
		nR = 4;
		rD = MC_D;
		nD = 4;
		rL = MC_B;
		nL = 2;
		break;
	case MC_D:
		rU = MC_F;
		nU = 3;
		rR = MC_R;
		nR = 3;
		rD = MC_B;
		nD = 3;
		rL = MC_L;
		nL = 3;
		break;
	case MC_B:
		rU = MC_U;
		nU = 1;
		rR = MC_L;
		nR = 4;
		rD = MC_D;
		nD = 3;
		rL = MC_R;
		nL = 2;
		break;
	default:
		return;
	}

	//////////////////////////////////
	// Redirect pointers.

	// Rotate m_currentOp face.

	if (m_currentShift)
	{
		GLfloat* tmpEdge = m_pMatrixes[m_rotorOp][1];
		GLfloat* tmpVertex = m_pMatrixes[m_rotorOp][5];

		for (int i = 1; i < 4; ++i)
		{
			m_pMatrixes[m_rotorOp][i] = m_pMatrixes[m_rotorOp][i + 1];
			m_pMatrixes[m_rotorOp][i + 4] = m_pMatrixes[m_rotorOp][i + 5];
		}
		m_pMatrixes[m_rotorOp][4] = tmpEdge;
		m_pMatrixes[m_rotorOp][8] = tmpVertex;
	}
	else
	{
		GLfloat* tmpEdge = m_pMatrixes[m_rotorOp][4];
		GLfloat* tmpVertex = m_pMatrixes[m_rotorOp][8];

		for (int i = 4; i > 1; --i)
		{
			m_pMatrixes[m_rotorOp][i] = m_pMatrixes[m_rotorOp][i - 1];
			m_pMatrixes[m_rotorOp][i + 4] = m_pMatrixes[m_rotorOp][i + 3];
		}
		m_pMatrixes[m_rotorOp][1] = tmpEdge;
		m_pMatrixes[m_rotorOp][5] = tmpVertex;
	}

	// Redirect adjacent edges

	m_pMatrixes[rU][nU] = m_pMatrixes[m_rotorOp][1];
	m_pMatrixes[rR][nR] = m_pMatrixes[m_rotorOp][2];
	m_pMatrixes[rD][nD] = m_pMatrixes[m_rotorOp][3];
	m_pMatrixes[rL][nL] = m_pMatrixes[m_rotorOp][4];

	// Redirect adjacent vertexes

	m_pMatrixes[rL][nL + 4] = m_pMatrixes[rU][nU + 5 == 9 ? 5 : nU + 5] = m_pMatrixes[m_rotorOp][5];
	m_pMatrixes[rU][nU + 4] = m_pMatrixes[rR][nR + 5 == 9 ? 5 : nR + 5] = m_pMatrixes[m_rotorOp][6];
	m_pMatrixes[rR][nR + 4] = m_pMatrixes[rD][nD + 5 == 9 ? 5 : nD + 5] = m_pMatrixes[m_rotorOp][7];
	m_pMatrixes[rD][nD + 4] = m_pMatrixes[rL][nL + 5 == 9 ? 5 : nL + 5] = m_pMatrixes[m_rotorOp][8];
}
void MagicCube::UpdateRotor()
{
	switch (m_currentOp)
	{
	case MC_R:
		if (m_currentShift) RotateRotor(MC_F, MC_U, MC_B, MC_D);
		else RotateRotor(MC_U, MC_F, MC_D, MC_B);
		break;
	case MC_U:
		if (m_currentShift) RotateRotor(MC_R, MC_F, MC_L, MC_B);
		else RotateRotor(MC_F, MC_R, MC_B, MC_L);
		break;
	case MC_F:
		if (m_currentShift) RotateRotor(MC_U, MC_R, MC_D, MC_L);
		else RotateRotor(MC_R, MC_U, MC_L, MC_D);
		break;
	default:
		return;
	}
}
void MagicCube::RotateRotor(MCenum f_1, MCenum f_2, MCenum f_3, MCenum f_4)
{
	MCenum tmp = m_rotor[f_1];
	m_rotor[f_1] = m_rotor[f_2];
	m_rotor[f_2] = m_rotor[f_3];
	m_rotor[f_3] = m_rotor[f_4];
	m_rotor[f_4] = tmp;
}

void MagicCube::DrawMain(BOOL withColor)
{
	// Material & Color Settings
	{
		GLfloat ambientMat[] = { 0.600000f, 0.600000f, 0.700000f, 1.000000f };
		GLfloat diffuseMat[] = { 0.800000f, 0.800000f, 0.850000f, 1.000000f };
		GLfloat speculrMat[] = { 0.333333f, 0.333333f, 0.333333f, 1.000000f };
		GLfloat shinnssMat[] = { 10.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
		glMaterialfv(GL_FRONT, GL_SPECULAR, speculrMat);
		glMaterialfv(GL_FRONT, GL_SHININESS, shinnssMat);
		if (m_texModulate)
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glColor4f(MC_BLK);
	}

	//////////////////////////////////
	// Little cubes
	
	// Centers
	if (m_drawCenters)
	{
		for (MCenum i = MC_R; i <= MC_B; ++i)
		{
			glPushMatrix();
			glLoadName(i);
			glMultMatrixf(m_pMatrixes[m_rotor[i]][0]);
			DrawLittle();
			glPopMatrix();
		}
		glLoadName(MC_N);
	}

	// Edges
	if (m_drawEdges)
	{
		for (int i = 0; i < 12; ++i)
		{
			glPushMatrix();
			glMultMatrixf(m_edgeMatrixes[i]);
			DrawLittle();
			glPopMatrix();
		}
	}

	// Vertexes
	if (m_drawVertexes)
	{
		for (int i = 0; i < 8; ++i)
		{
			glPushMatrix();
			glMultMatrixf(m_vertexMatrixes[i]);
			DrawLittle();
			glPopMatrix();
		}
	}

	//////////////////////////////////
	// Colors

	if (withColor)
	{
		// Centers
		if (m_drawCenters)
		{
			for (MCenum i = MC_R; i <= MC_B; ++i)
			{
				glPushMatrix();
				glMultMatrixf(m_pColors[i][0]);
				DrawColor(i);
				glPopMatrix();
			}
		}

		// Edges
		if (m_drawEdges)
		{
			for (MCenum i = MC_R; i <= MC_B; ++i)
				for (int j = 1; j < 5; ++j)
				{
					glPushMatrix();
					glMultMatrixf(m_pColors[i][j]);
					DrawColor(i);
					glPopMatrix();
				}
		}

		// Vertexes
		if (m_drawVertexes)
		{
			for (MCenum i = MC_R; i <= MC_B; ++i)
				for (int j = 5; j < 9; ++j)
				{
					glPushMatrix();
					glMultMatrixf(m_pColors[i][j]);
					DrawColor(i);
					glPopMatrix();
				}
		}
	}
}
void MagicCube::DrawLittle()
{
	// Editable variables
	float a = 0.44f;
	float r = 0.05f;

	// Internal variables
	int xsign = 1;
	int ysign = 1;
	int zsign = 1;

	// 4 faces around
	glBegin(GL_QUAD_STRIP);
	for (int ideg = 0; ideg <= 360; ideg += 30)
	{
		float irad = RAD(ideg);
		glNormal3f(0.0f, cosf(irad), sinf(irad));
		glVertex3f(-a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));
		glVertex3f( a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));

		if (ideg == 90 || ideg == 270)
		{
			glNormal3f(0.0f, 0.0f, ysign);
			ysign = -ysign;
			glVertex3f(-a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));
			glVertex3f( a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));
		}
		else if (ideg == 180 || ideg == 360)
		{
			glNormal3f(0.0f, -zsign, 0.0f);
			zsign = -zsign;
			glVertex3f(-a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));
			glVertex3f( a, ysign * a + r * cosf(irad), zsign * a + r * sinf(irad));
		}
	}
	glEnd();

	// 2 faces up and down
	glBegin(GL_QUADS);
	{
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(a + r, -a, -a);
		glVertex3f(a + r, -a, a);
		glVertex3f(a + r, a, a);
		glVertex3f(a + r, a, -a);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-a - r, -a, -a);
		glVertex3f(-a - r, -a, a);
		glVertex3f(-a - r, a, a);
		glVertex3f(-a - r, a, -a);
	}
	glEnd();

	// Vertexes & rest edges
	for (int laDeg = 0; laDeg < 180; laDeg += 30)
	{
		if (laDeg == 90)
			xsign = -1;

		glBegin(GL_QUAD_STRIP);
		float latit = RAD(laDeg);
		float latitNext = RAD(laDeg + 30);
		for (int lodeg = 0; lodeg <= 360; lodeg += 30)
		{
			float longi = RAD(lodeg);
			glNormal3f(cosf(latit), sinf(latit) * cosf(longi), sinf(latit) * sinf(longi));
			glVertex3f(
				xsign * a + r * cosf(latit),
				ysign * a + r * sinf(latit) * cosf(longi),
				zsign * a + r * sinf(latit) * sinf(longi));
			glVertex3f(
				xsign * a + r * cosf(latitNext),
				ysign * a + r * sinf(latitNext) * cosf(longi),
				zsign * a + r * sinf(latitNext) * sinf(longi));

			if (lodeg == 90 || lodeg == 270)
				ysign = -ysign;
			if (lodeg == 180 || lodeg == 360)
				zsign = -zsign;
			if (lodeg == 90 || lodeg == 180 || lodeg == 270 || lodeg == 360)
			{
				glNormal3f(cosf(latit), sinf(latit) * cosf(longi), sinf(latit) * sinf(longi));
				glVertex3f(
					xsign * a + r * cosf(latit),
					ysign * a + r * sinf(latit) * cosf(longi),
					zsign * a + r * sinf(latit) * sinf(longi));
				glVertex3f(
					xsign * a + r * cosf(latitNext),
					ysign * a + r * sinf(latitNext) * cosf(longi),
					zsign * a + r * sinf(latitNext) * sinf(longi));
			}
		}
		glEnd();
	}
}
void MagicCube::DrawColor(MCenum face)
{
	glPushMatrix();

	// Rotate & Set Color.
	switch (face)
	{
	case MC_R:
		if (!m_useTextures) SetColorMat(MC_RED);
		break;
	case MC_U:
		glRotatef( 90.0f, MC_AXIS_F);
		if (!m_useTextures) SetColorMat(MC_YLW);
		break;
	case MC_F:
		glRotatef( 90.0f, MC_AXIS_D);
		if (!m_useTextures) SetColorMat(MC_BLU);
		break;
	case MC_L:
		glRotatef(180.0f, MC_AXIS_U);
		if (!m_useTextures) SetColorMat(MC_ORN);
		break;
	case MC_D:
		glRotatef( 90.0f, MC_AXIS_B);
		if (!m_useTextures) SetColorMat(MC_WHT);
		break;
	case MC_B:
		glRotatef( 90.0f, MC_AXIS_U);
		if (!m_useTextures) SetColorMat(MC_GRN);
		break;
	default:
		SetColorMat(MC_TRS);
		break;
	}

	glNormal3f(1.0f, 0.0f, 0.0f);

	if (m_useTextures)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textures[face]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(0.5f,  0.4f,  0.4f);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(0.5f, -0.4f,  0.4f);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(0.5f, -0.4f, -0.4f);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(0.5f,  0.4f, -0.4f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glBegin(GL_QUADS);
		glVertex3f(0.5f,  0.4f,  0.4f);
		glVertex3f(0.5f, -0.4f,  0.4f);
		glVertex3f(0.5f, -0.4f, -0.4f);
		glVertex3f(0.5f,  0.4f, -0.4f);
		glEnd();
	}

	glPopMatrix();
}
void MagicCube::SetColorMat(float r, float g, float b, float a)
{
	GLfloat ambientMat[] = { 0.6f * r, 0.6f * g, 0.7f * b, a };
	GLfloat diffuseMat[] = { 0.8f * r, 0.8f * g, 0.9f * b, a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
	glColor4f(r, g, b, a);
}

void MagicCube::SetRotation(MCenum mode, MCenum op, BOOL shift)
{
	// Important: Do NOT continue while process not finished.
	if (m_currentOp != MC_N)
		return;

	// Do NOT continue if "op" out of bound.
	if (op < MC_R || op > MC_B)
		return;

	// Sets & Resets.
	m_currentOp = op;
	m_rotorOp = m_rotor[op];
	m_currentShift = shift;

	// Set a new timer.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = pFrame->GetActiveView();
	pView->SetTimer(mode, 25, NULL);
}
BOOL MagicCube::SetAxis()
{
	switch (m_currentOp)
	{
	case MC_R:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_R);
		break;
	case MC_U:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_U);
		break;
	case MC_F:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_F);
		break;
	case MC_L:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_L);
		break;
	case MC_D:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_D);
		break;
	case MC_B:
		glRotatef(m_currentShift ? m_rotateStep : -m_rotateStep, MC_AXIS_B);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
void MagicCube::RotateLayerStep()
{
	m_rotateAngle += m_rotateStep;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if(!SetAxis()) return;

	for (int i = 0; i < 9; ++i)
	{
		glPushMatrix();
		glMultMatrixf(m_pMatrixes[m_rotorOp][i]);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_pMatrixes[m_rotorOp][i]);
		glPopMatrix();
	}
	glPopMatrix();

	if (m_rotateAngle >= 90)
		RotateFinish(MC_MODE_LAYER);
}
void MagicCube::RotateViewStep()
{
	m_rotateAngle += m_rotateStep;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if (!SetAxis()) return;

	for (int i = 0; i < 6; ++i)
	{
		glPushMatrix();
		glMultMatrixf(m_centerMatrixes[i]);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_centerMatrixes[i]);
		glPopMatrix();
	}
	for (int i = 0; i < 12; ++i)
	{
		glPushMatrix();
		glMultMatrixf(m_edgeMatrixes[i]);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_edgeMatrixes[i]);
		glPopMatrix();
	}
	for (int i = 0; i < 8; ++i)
	{
		glPushMatrix();
		glMultMatrixf(m_vertexMatrixes[i]);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_vertexMatrixes[i]);
		glPopMatrix();
	}
	glPopMatrix();

	if (m_rotateAngle >= 90)
		RotateFinish(MC_MODE_VIEW);
}
void MagicCube::RotateFinish(MCenum mode)
{
	// Kill current timer.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = pFrame->GetActiveView();
	pView->KillTimer(mode);

	// Update pointers or rotor decided by mode.
	if (mode == MC_MODE_LAYER) UpdatePointers();
	else if (mode == MC_MODE_VIEW) UpdateRotor();

	// Important: Reset operation to default.
	m_currentOp = m_rotorOp = MC_N;
	m_rotateAngle = 0;
}

void MagicCube::AutoPlay(char* string)
{
	// Important: Do NOT continue while still playing.
	if (!m_formula.empty())
		return;

	// Set
	m_formula = string;

	// Set a new timer.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = pFrame->GetActiveView();
	pView->SetTimer(MC_MODE_AUTO, 500, NULL);
}
void MagicCube::AutoPlayStep()
{
	// Stop when empty.
	if (m_formula.empty())
	{
		RotateFinish(MC_MODE_AUTO);
		return;
	}

	// Temp variables
	MCenum mode = MC_N;
	MCenum op = MC_N;
	BOOL shift = FALSE;

	// Jump over all spaces.
	while (!m_formula.empty() && isspace(m_formula.front()))
		m_formula = m_formula.substr(1);

	// Stop when empty.
	if (m_formula.empty())
	{
		RotateFinish(MC_MODE_AUTO);
		return;
	}

	// Read first.
	switch (m_formula.front())
	{
	case 'R':
		mode = MC_MODE_LAYER;
		op = MC_R;
		break;
	case 'U':
		mode = MC_MODE_LAYER;
		op = MC_U;
		break;
	case 'F':
		mode = MC_MODE_LAYER;
		op = MC_F;
		break;
	case 'L':
		mode = MC_MODE_LAYER;
		op = MC_L;
		break;
	case 'D':
		mode = MC_MODE_LAYER;
		op = MC_D;
		break;
	case 'B':
		mode = MC_MODE_LAYER;
		op = MC_B;
		break;
	case 'x':
		mode = MC_MODE_VIEW;
		op = MC_R;
		break;
	case 'y':
		mode = MC_MODE_VIEW;
		op = MC_U;
		break;
	case 'z':
		mode = MC_MODE_VIEW;
		op = MC_F;
		break;
	default:
		StopPlaying();
		return;
	}

	// Read second.
	if (m_formula.length() >= 2)
	{
		if (m_formula[1] == '\'')
		{
			shift = !shift;
			m_formula[1] = ' ';
		}
		if (m_formula[1] == '2')
			m_formula[1] = m_formula[0];
	}
	m_formula = m_formula.substr(1);

	// Perform.
	SetRotation(mode, op, shift);
}
void MagicCube::StopPlaying()
{
	if (m_formula.empty())
		return;

	// Kill auto mode timer.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = pFrame->GetActiveView();
	pView->KillTimer(MC_MODE_AUTO);

	m_formula.clear();
}

BOOL MagicCube::IsAvailable() const
{
	return m_rotateAngle == 0 && m_formula.empty();
}

void MagicCube::AuxAxis(float xRotation, float yRotation)
{
	float length = 3.0f;
	float xR = RAD(xRotation);
	float yR = RAD(yRotation);

	glBegin(GL_LINES);

	// Axis x.
	SetColorMat(MC_RED);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(length * cosf(yR), length * sinf(xR) * sinf(yR));
	// Axis y.
	SetColorMat(MC_BLU);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(length * sinf(yR), length * sinf(xR) *-cosf(yR));
	// Axis z.
	SetColorMat(MC_GRN);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, length * cosf(xR));

	glEnd();
}

MagicCube::MagicCube()
{
	m_currentOp		= MC_N;
	m_rotorOp		= MC_N;
	m_useRotor		= FALSE;
	m_currentShift	= FALSE;
	m_rotateAngle	= 0;

	m_drawCenters	= TRUE;
	m_drawEdges		= TRUE;
	m_drawVertexes	= TRUE;
	m_useTextures	= FALSE;

	m_formula = "";
}
MagicCube::~MagicCube() {}
