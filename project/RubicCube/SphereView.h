// SphereView.h : interface of the CSphereView class

#ifndef ___SPHERE_VIEW_H___
#define ___SPHERE_VIEW_H___

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OpenGLView.h"
#include "SphereDoc.h"
#include "MagicCube.h"

// The struct of light bulb which can be controlled
struct Bulb
{
	GLenum LIGHT;			// GL_LIGHT*
	GLfloat longitude;		// 0 ~ 359 degree
	GLfloat latitude;		// -90 ~ 90 degree
	GLfloat radius;			// Marked at 0 ~ 2
	BOOL isSpot;			// Is this bulb a spotlight?
	BOOL isMuted;			// Is this bulb muted?
};

class CSphereView : public COpenGLView
{
protected: // create from serialization only
	CSphereView();
	DECLARE_DYNCREATE(CSphereView)

// Attributes
public:
	CSphereDoc* GetDocument();

// Operations
public:
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;

	float m_xScaling;
	float m_yScaling;
	float m_zScaling;

	float m_xRotation;
	float m_yRotation;
	BOOL m_yRotationLimit;

	CPoint m_LeftDownPos;
	BOOL m_LeftButtonDown;

	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	// Structure of light bulb.
	Bulb m_light;
	void DrawBulb();

	// Instance of main class.
	MagicCube m_cube;

	CPoint m_pointBegin;
	// Do once selection on L/R click.
	int Selection(CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSphereView)
	public:
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	//void Init();
	void Display();
//	void DrawTriangle(float *v1, float *v2, float *v3);
//	void Subdivide(float *v1, float *v2, float *v3, long depth);
//	void NormCrossProd(float v1[3], float v2[3], float out[3]);
//	void Normalize(float v[3]);
	virtual ~CSphereView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSphereView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
//	void DrawNormSphere(void);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in MyTestOpenGLView.cpp
inline CSphereDoc* CSphereView::GetDocument()
   { return (CSphereDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
