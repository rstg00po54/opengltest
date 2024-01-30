// OpenGLView.cpp : implementation of the COpenGLView class
// Create a base class for supporting OpenGL

#include "stdafx.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView construction/destruction

COpenGLView::COpenGLView() : hRC(NULL), pDC(NULL)
{
	// TODO: add construction code here

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	// An OpenGL window must be created with the following
	// flags and must not include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
	Display();

	SwapBuffers(pDC->GetSafeHdc());
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView printing

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	InitializeOpenGL();	

	return 0;
}

void COpenGLView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here

	wglMakeCurrent(0,0);		// Cleanup rendering and device contexts
	wglDeleteContext(hRC);      
	if (pDC) delete pDC;

	
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;	// Tell windows not to erase the background
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ((cx > 0) && (cy > 0)){
		float r = (float)(cx < cy ? cx : cy);
		glViewport(0, 0, (GLsizei)cx, (GLsizei)cy);	// 设置窗口中的绘制区域
		glMatrixMode(GL_PROJECTION);		   		// 设置投影矩阵
		glLoadIdentity();
		glFrustum(-cx / r / 25.0, cx / r / 25.0, -cy / r / 25.0, cy / r / 25.0, 0.1, 20.0);
		gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//glOrtho(-cx / r, cx / r, -cy / r, cy / r, -10.0, 10.0);
	}
}

//*********************************************
//
// COpenGLView::InitializeOpenGL
// 	Intialize device and rendering contexts
//	for OpenGL usage.
//
// Parameters: NONE
//
// Returns: Success of operation
//
// Written by: H. Welch 27-December-1999
//	Based on framework by Ron Fosner
//
//*********************************************


BOOL COpenGLView::InitializeOpenGL()
{
	pDC = new CClientDC(this);		// Create device context

	if (NULL == pDC) return FALSE;		// Creation failure

	if (!SetupPixelFormat()) return FALSE;	// Select graphics mode

	if (0 == (hRC = wglCreateContext(pDC->GetSafeHdc())))
		return FALSE;			// Initialize rendering context

	if (!wglMakeCurrent(pDC->GetSafeHdc(), hRC))
		return FALSE;			// Make it the active one

	Init();					// Initialize OpenGL state

	return TRUE;
}

//*********************************************
//
// COpenGLView::SetupPixelFormat
// 	Intialize device and rendering contexts
//	for OpenGL usage.
//
// Parameters: NONE
//
// Returns: Success of operation
//
// Written by: H. Welch 27-December-1999
//	Based on framework by Ron Fosner
//
//*********************************************

BOOL COpenGLView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = 
		{
		sizeof(PIXELFORMATDESCRIPTOR),		// Size
		1,					// Version number
		PFD_DRAW_TO_WINDOW |			// Support window
		PFD_SUPPORT_OPENGL |			// Support OpenGL
		PFD_DOUBLEBUFFER,				// Support double buffer
		PFD_TYPE_RGBA,				// Support true RGB color
		24,					// Color depth
		0,0,0,0,0,0,				// Color bits ignored
		0,					// No alpha buffer
		0,					// Ignore shift bit
		0,					// No accumulation buffer
		0,0,0,0,				// Accumulation bits ignored
		0,					// No stencil buffer
		0,					// No auxilliary buffer
		PFD_MAIN_PLANE,				// main layer
		0,					// Reserved
		0,0,0					// Layer masks ignored
		};
	int pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd);

	if (0 == pixelformat)
		return FALSE;				// Match closest pixel format

	if (!SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd))
		return FALSE;				// Set closest match

	return TRUE;
}

//*********************************************
//
// COpenGLView::Init
// 	Intialize OpenGL per application needs
//
// Parameters: NONE
//
// Returns: Nothing
//
// Written by: H. Welch 27-December-1999
//
//*********************************************

void COpenGLView::Init()
{
	// Set background color to light grey
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
}

//*********************************************
//
// COpenGLView::Display
// 	Draw OpenGL picture
//
// Parameters: NONE
//
// Returns: Nothing
//
// Written by: H. Welch 27-December-1999
//
//*********************************************

void COpenGLView::Display()
{
	// Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw simple triangle
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
		glVertex3f(0.25, 0.25, 0.0);
		glVertex3f(0.75, 0.25, 0.0);
		glVertex3f(0.5, 0.75, 0.0);
	glEnd();
}
