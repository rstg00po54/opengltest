#pragma once
#include "glm.h"
#include <string>

///////////////////////////////////
// Defines

#ifndef MC_MODE_LAYER

// Magic Cube: Rotation mode - Layer
#define MC_MODE_LAYER	0x0001
// Magic Cube: Rotation mode - View
#define MC_MODE_VIEW	0x0002
// Magic Cube: Rotation mode - Auto play
#define MC_MODE_AUTO	0x0004

// Magic Cube: Default - No operation
#define MC_N -1
// Magic Cube: Right
#define MC_R 0
// Magic Cube: Up
#define MC_U 1
// Magic Cube: Front
#define MC_F 2
// Magic Cube: Left
#define MC_L 3
// Magic Cube: Down
#define MC_D 4
// Magic Cube: Back
#define MC_B 5

// Magic Cube: Axis +x
#define MC_AXIS_R  1.0f,  0.0f,  0.0f
// Magic Cube: Axis +y
#define MC_AXIS_U  0.0f,  1.0f,  0.0f
// Magic Cube: Axis +z
#define MC_AXIS_F  0.0f,  0.0f,  1.0f
// Magic Cube: Axis -x
#define MC_AXIS_L -1.0f,  0.0f,  0.0f
// Magic Cube: Axis -y
#define MC_AXIS_D  0.0f, -1.0f,  0.0f
// Magic Cube: Axis -z
#define MC_AXIS_B  0.0f,  0.0f, -1.0f

// Magic Cube: Default - Transparent
#define MC_TRS 0.0f, 0.0f, 0.0f, 0.0f
// Magic Cube: Blue
#define MC_BLU 0.2f, 0.2f, 0.9f, 1.0f
// Magic Cube: Yellow
#define MC_YLW 1.0f, 0.9f, 0.3f, 1.0f
// Magic Cube: Red
#define MC_RED 1.0f, 0.0f, 0.0f, 1.0f
// Magic Cube: Green
#define MC_GRN 0.2f, 0.6f, 0.3f, 1.0f
// Magic Cube: White
#define MC_WHT 0.9f, 0.9f, 1.0f, 1.0f
// Magic Cube: Orange
#define MC_ORN 1.0f, 0.6f, 0.1f, 1.0f
// Magic Cube: Black
#define MC_BLK 0.0f, 0.0f, 0.1f, 1.0f

#endif // !MC_MODE_LAYER

// Magic cube: Enum of operations
typedef int MCenum;


///////////////////////////////////
// Main class of opengl magic cube
class MagicCube
{
// Private variables
private:

	// Transform Matrixes of 26 little cubes.
	GLfloat m_centerMatrixes[6][16];
	GLfloat m_edgeMatrixes[12][16];
	GLfloat m_vertexMatrixes[8][16];

	// Matrix pointers of 9 faces.
	GLfloat* m_pMatrixes[6][9];
	GLfloat* m_pColors[6][9];

	// Enigma rotor, used when view rotate.
	MCenum m_rotor[6];

	// Current operation.
	MCenum m_currentOp;
	// Current operation after through rotor.
	MCenum m_rotorOp;
	// Decide whether current rotation uses rotor or not;
	BOOL m_useRotor;
	// Clockwise when FALSE, counterclockwise when TRUE.
	BOOL m_currentShift;
	// Current rotate angle of current operation.
	int m_rotateAngle;
	// Rotate 9 degrees per 25ms.
	// Used in RotateStep().
	const int m_rotateStep = 9;

	// Textures of 6 faces.
	GLuint m_textures[6];

	// Formatted formula string of auto play.
	std::string m_formula;



// Public variables
public:

	// Checks at side column.
	BOOL m_drawCenters = TRUE;
	BOOL m_drawEdges = TRUE;
	BOOL m_drawVertexes = TRUE;

	// Decide whether use textures or colors.
	BOOL m_useTextures = FALSE;
	// Decide whether use GL_MODULATE or not.
	BOOL m_texModulate = TRUE;



// Private sub-functions
private:

	// Initialize or reset matrixes of all little cubes.
	// The sub-function of InitCube().
	void InitMatrixes();

	// Initialize or reset pointers of all faces.
	// The sub-function of InitCube().
	// Must be invoked after InitMatrixes().
	void InitPointers(GLfloat* ptrs[6][9]);

	// The sub-function of InitPointers().
	void InitEdgesOf(GLfloat* ptrs[9], int e_1, int e_2, int e_3, int e_4);

	// The sub-function of InitPointers().
	void InitVertexesOf(GLfloat* ptrs[9], int v_1, int v_2, int v_3, int v_4);

	// The sub-function of InitCube().
	void InitRotor();

	// The sub-function of InitCube().
	void InitTextures();

	// Draw one little cube (1 / 26) with OpenGL.
	// The sub-function of DrawMain().
	void DrawLittle();

	// Draw one piece of Color (1 / 54) with OpenGL.
	// The sub-function of DrawMain().
	void DrawColor(MCenum face);

	// Set a material with Color4f(r, g, b, a).
	// The sub-function of DrawColor().
	void SetColorMat(float r, float g, float b, float a);

	// Multiply a rotation matrix by m_currentOp.
	// The sub-function of RotateXXXStep().
	BOOL SetAxis();

	// Invoke when one rotation finished.
	// The sub-function of RotateStep().
	void RotateFinish(MCenum mode);

	// Update pointers after finishing a layer rotation.
	// The sub-function of RotateFinish().
	void UpdatePointers();

	// Update rotor after finishing a view rotation.
	// The sub-function of RotateFinish().
	void UpdateRotor();

	// Rotate the 4 faces of m_rotor in m_currentShift.
	// The sub-function of UpdateRotor().
	void RotateRotor(MCenum f_1, MCenum f_2, MCenum f_3, MCenum f_4);



// Interfaces
public:

	// Initialize or reset matrixes and pointers
	void InitCube();

	// Draw entire magic cube at the origin of OpenGL.
	void DrawMain(BOOL withColor);

	// Trigger a timer with the operation "op" that about to rotate.
	void SetRotation(MCenum mode, MCenum operation, BOOL shift);

	// Rotate m_currentOp's layer slightly till 90 degrees.
	// The CALLBACK of SetRotation().
	void RotateLayerStep();

	// Rotate client's view slightly till 90 degrees.
	// The CALLBACK of SetRotation().
	void RotateViewStep();

	// Auto operate with the formated string.
	void AutoPlay(char* string);

	// Perform one step of the formated string.
	// The CALLBACK of AutoPlay().
	void AutoPlayStep();

	// Stop playing mode compulsorily.
	// The sub-function of AutoPlayStep().
	void StopPlaying();

	// Return whether all timers killed or not.
	BOOL IsAvailable() const;

	// Draw auxiliary axis.
	void AuxAxis(float xRotation, float yRotation);



// Constuctor & Destructor
public:
	MagicCube();
	~MagicCube();
};
