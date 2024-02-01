// SphereView.cpp : implementation of the CSphereView class

#include "stdafx.h"

#include "Sphere.h"
#include "SphereDoc.h"
#include "SphereView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RAD(degree) ((degree) * 3.1416f / 180.0f)

/////////////////////////////////////////////////////////////////////////////
// CSphereView

IMPLEMENT_DYNCREATE(CSphereView, COpenGLView)

BEGIN_MESSAGE_MAP(CSphereView, COpenGLView)
	//{{AFX_MSG_MAP(CSphereView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSphereView construction/destruction
CSphereView::CSphereView()
{
	// TODO: add construction code here
	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = 0.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_xRotation = 30.0f;
	m_yRotation = -45.0f;
	m_yRotationLimit = FALSE;

	m_LeftButtonDown = FALSE;

	m_ClearColorRed = 0.8f;
	m_ClearColorGreen = 0.75f;
	m_ClearColorBlue = 0.9f;

	m_light.LIGHT = GL_LIGHT0;
	m_light.longitude = 225.0f;
	m_light.latitude = 45.0f;
	m_light.radius = 15.0f;
	m_light.isSpot = FALSE;
	m_light.isMuted = FALSE;
}

CSphereView::~CSphereView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSphereView printing

BOOL CSphereView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSphereView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSphereView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSphereView diagnostics

#ifdef _DEBUG
void CSphereView::AssertValid() const
{
	CView::AssertValid();
}

void CSphereView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSphereDoc* CSphereView::GetDocument() // non-debug version is inline   在视图类中访问相应的文档类
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSphereDoc)));
	return (CSphereDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSphereView message handlers
void CSphereView::Display()
{
	// Clear background.
	glClearColor(m_ClearColorRed, m_ClearColorGreen, m_ClearColorBlue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	DrawBulb();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yRotation, 0.0f, 1.0f, 0.0f);
	glScalef(m_xScaling, m_yScaling, m_zScaling);
	m_cube.DrawMain(TRUE);
	glPopMatrix();
}


void CSphereView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	// Operations only if the cube is available.
	if (m_cube.IsAvailable())
	{
		// Use keys to interact with the cube.
		if (nChar == 'R')
			m_cube.SetRotation(MC_MODE_LAYER, MC_R, bShift);
		else if (nChar == 'U')
			m_cube.SetRotation(MC_MODE_LAYER, MC_U, bShift);
		else if (nChar == 'F')
			m_cube.SetRotation(MC_MODE_LAYER, MC_F, bShift);
		else if (nChar == 'L')
			m_cube.SetRotation(MC_MODE_LAYER, MC_L, bShift);
		else if (nChar == 'D')
			m_cube.SetRotation(MC_MODE_LAYER, MC_D, bShift);
		else if (nChar == 'B')
			m_cube.SetRotation(MC_MODE_LAYER, MC_B, bShift);

		// Use X/Y/Z to rotate view
		else if (nChar == 'X')
			m_cube.SetRotation(MC_MODE_VIEW, MC_R, bShift);
		else if (nChar == 'Y')
			m_cube.SetRotation(MC_MODE_VIEW, MC_U, bShift);
		else if (nChar == 'Z')
			m_cube.SetRotation(MC_MODE_VIEW, MC_F, bShift);

		// Use A to play auto mode after opening a txt file.
		else if (nChar == 'A')
		{
			CSphereDoc* pDoc = GetDocument();
			if (pDoc->m_string[0] != '\0')
				m_cube.AutoPlay(pDoc->m_string);
		}

		// Use return key to reset all
		else if (nChar == VK_RETURN)
		{
			m_cube.InitCube();
			InvalidateRect(NULL, FALSE);
		}
	}

	if (nChar == 'E')
		m_cube.StopPlaying();

	// Use arrow keys to move the cube model.
	if (nChar == VK_LEFT)
	{
		m_xTranslation -= 0.05f; 
		InvalidateRect(NULL, FALSE);
	}
	else if (nChar == VK_RIGHT)
	{
		m_xTranslation += 0.05f; 
		InvalidateRect(NULL, FALSE);
	}
	else if (nChar == VK_UP)
	{
		m_yTranslation += 0.05f; 
		InvalidateRect(NULL, FALSE);
	}
	else if (nChar == VK_DOWN)
	{
		m_yTranslation -= 0.05f; 
		InvalidateRect(NULL, FALSE);
	}

	COpenGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CSphereView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	m_pointBegin = point;
	
	InvalidateRect(NULL, FALSE);
	COpenGLView::OnLButtonDown(nFlags, point);
}
void CSphereView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pointBegin = point;

	COpenGLView::OnRButtonDown(nFlags, point);
}
void CSphereView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LeftButtonDown = FALSE;

	if (m_pointBegin == point)
		m_cube.SetRotation(MC_MODE_LAYER, Selection(point), TRUE);
	COpenGLView::OnLButtonUp(nFlags, point);
}
void CSphereView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pointBegin == point)
		m_cube.SetRotation(MC_MODE_LAYER, Selection(point), FALSE);
	COpenGLView::OnRButtonUp(nFlags, point);
}
void CSphereView::OnMouseMove(UINT nFlags, CPoint point)
{                    
	if(m_LeftButtonDown)
	{
		m_xRotation += (float)(point.y - m_LeftDownPos.y) / 3.0f;
		m_yRotation += (float)(point.x - m_LeftDownPos.x) / 3.0f;
		// Clamp
		if (m_xRotation >  90.0f) m_xRotation =  90.0f;
		else if (m_xRotation < -90.0f) m_xRotation = -90.0f;
		if (m_yRotationLimit)
		{
			if (m_yRotation > 90.0f) m_yRotation = 90.0f;
			else if (m_yRotation < -90.0f) m_yRotation = -90.0f;
		}
		m_LeftDownPos = point;
		InvalidateRect(NULL, FALSE);
	}
	COpenGLView::OnMouseMove(nFlags, point);
}

int CSphereView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Default mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 0.8f, 0.8f, 0.6f, 1.0f };

	glClearDepth(1.0f);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	// Default: lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_ClearColorRed, m_ClearColorGreen, m_ClearColorBlue, 1.0f);

	// Initialize the magic cube
	m_cube.InitCube();

	return 0;
}
void CSphereView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == MC_MODE_LAYER) m_cube.RotateLayerStep();
	if (nIDEvent == MC_MODE_VIEW) m_cube.RotateViewStep();
	if (nIDEvent == MC_MODE_AUTO) m_cube.AutoPlayStep();

	InvalidateRect(NULL, FALSE);
	COpenGLView::OnTimer(nIDEvent);
}

void CSphereView::DrawBulb()
{
	GLenum LIGHT = m_light.LIGHT;
	GLfloat longitude = m_light.longitude;
	GLfloat latitude = m_light.latitude;
	GLfloat radius = m_light.radius;
	BOOL isSpot = m_light.isSpot;
	BOOL isMuted = m_light.isMuted;

	// Is muted?
	if (isMuted)
	{
		glDisable(LIGHT);
	}
	else
	{
		// Position of LIGHT
		GLfloat lgt_position[] = {
			radius * cos(RAD(longitude)) * cos(RAD(latitude)),
			radius * sin(RAD(latitude)),
			-radius * sin(RAD(longitude)) * cos(RAD(latitude)), 1.0f };
		glLightfv(LIGHT, GL_POSITION, lgt_position);

		// Is spotlight?
		if (isSpot)
		{
			GLfloat lgt_direction[] = {
				-radius * cos(RAD(longitude)) * cos(RAD(latitude)),
				-radius * sin(RAD(latitude)),
				radius * sin(RAD(longitude)) * cos(RAD(latitude)) };
			GLfloat lgt_exponent[] = { 10.0f };
			GLfloat lgt_cutoff[] = { 30.0f };	// or graphically atan(0.5) = 13.3 degrees
			glLightfv(LIGHT, GL_SPOT_DIRECTION, lgt_direction);
			glLightfv(LIGHT, GL_SPOT_EXPONENT, lgt_exponent);
			glLightfv(LIGHT, GL_SPOT_CUTOFF, lgt_cutoff);
		}
		else
		{
			GLfloat lgt_direction[] = { 0.0f, 0.0f, -1.0f };
			GLfloat lgt_exponent[] = { 0.0f };
			GLfloat lgt_cutoff[] = { 180.0f };
			glLightfv(LIGHT, GL_SPOT_DIRECTION, lgt_direction);
			glLightfv(LIGHT, GL_SPOT_EXPONENT, lgt_exponent);
			glLightfv(LIGHT, GL_SPOT_CUTOFF, lgt_cutoff);
		}

		glEnable(LIGHT);
	}

	// Start to draw bulb model
	glPushMatrix();
	glLoadIdentity();

	// Translation of bulb model
	glRotatef(longitude, 0.0f, 1.0f, 0.0f);
	glRotatef(latitude, 0.0f, 0.0f, 1.0f);
	glTranslatef(radius + 0.01f, 0.0f, 0.0f);

	// Material / Color of bulb model
	// Is muted?
	if (isMuted)
		glColor4ub(0x66, 0x66, 0x6f, 0xff);
	else
		glColor4ub(0xff, 0xbb, 0x33, 0xff);

	GLfloat mat_ambient[] = { 0.247250f, 0.199500f, 0.074500f, 1.000000f };
	GLfloat mat_diffuse[] = { 0.751640f, 0.606480f, 0.226480f, 1.000000f };
	GLfloat mat_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.000000f };
	GLfloat mat_shininess[] = { 51.200001 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i <= 360; i += 30)
	{
		glNormal3f(-0.5f, cos(RAD(i)), sin(RAD(i)));
		glVertex3f(-0.2f, 0.1f * sin(RAD(i)), 0.1f * cos(RAD(i)));
	}
	glEnd();

	glPopMatrix();
}
int CSphereView::Selection(CPoint point)
{
	GLuint buffer[256];
	glSelectBuffer(256, buffer);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int cx = viewport[2] - viewport[0];				// Width of viewport
	int cy = viewport[3] - viewport[1];				// Height of viewport
	float r = (float)(cx < cy ? cx : cy);			// The shorter one of cx & cy

	// Enter SELECT mode.
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(-1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// The view to pick.
	gluPickMatrix((GLdouble)point.x, (GLdouble)viewport[3] - point.y, 1.0, 1.0, viewport);
	// Adapt to viewport (see "Onsize()" in OpenGLView.cpp).
	glFrustum(-cx / r / 25.0, cx / r / 25.0, -cy / r / 25.0, cy / r / 25.0, 0.1, 20.0);
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glOrtho(-cx / r, cx / r, -cy / r, cy / r, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	// Draw target in SELECT mode.
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yRotation, 0.0f, 1.0f, 0.0f);
	glScalef(m_xScaling, m_yScaling, m_zScaling);
	m_cube.DrawMain(FALSE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	GLint hits = glRenderMode(GL_RENDER);

	// Hit anyone?
	if (hits > 0)
	{
		int choose = buffer[3];
		int depth = buffer[1];

		for (int i = 1; i < hits; ++i)
		{
			if (buffer[i * 4 + 1] < GLuint(depth))
			{
				choose = buffer[i * 4 + 3];
				depth = buffer[i * 4 + 1];
			}
		}
		return choose;
	}
	return -1;
}
