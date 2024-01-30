// OpenGLView.h : interface of the COpenGLView class
// Create a base class for supporting OpenGL

#ifndef ___OPENGL_VIEW_H___
#define ___OPENGL_VIEW_H___

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Include the OpenGL headers
#include <GL\gl.h>
#include <GL\glu.h>

class COpenGLView : public CView
{
// OpenGL support
public:
	virtual BOOL SetupPixelFormat();
	virtual void Init();
	virtual void Display();
//	virtual void Reshape(int w, int h);

private:
	BOOL InitializeOpenGL();

protected:
	HGLRC hRC;		// Rendering Context
	CDC* pDC;		// Device Context

protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg virtual void OnDestroy();
	afx_msg virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
