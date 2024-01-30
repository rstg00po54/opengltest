#include "stdafx.h"
#include "Sphere.h"
#include "MainFrm.h"
#include "FormCommandView.h"
#include "SphereDoc.h"
#include "SphereView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormCommandView

IMPLEMENT_DYNCREATE(CFormCommandView, CFormView)

CFormCommandView::CFormCommandView()
: CFormView(CFormCommandView::IDD)
{
	//{{AFX_DATA_INIT(CFormCommandView)

	BackColor = RGB(255,255,255);
	AmbientColor = RGB(128,128,128);

	m_Antialias = FALSE;
	m_Smooth = FALSE;
	m_Lighting = TRUE;
	m_CubeTextures = FALSE;
	m_CubeCenters = TRUE;
	m_CubeEdges = TRUE;
	m_CubeVertexes = TRUE;
	m_LinkScale = TRUE;
	m_LightSpot = FALSE;
	//}}AFX_DATA_INIT
}

CFormCommandView::~CFormCommandView()
{
}

void CFormCommandView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormCommandView)
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_AMBIENT3, m_ControlColorLightSpecular);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_AMBIENT2, m_ControlColorLightDiffuse);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_AMBIENT, m_ControlColorLightAmbient);
	DDX_Control(pDX, IDC_SLIDER_Z, m_SliderScaleZ);
	DDX_Control(pDX, IDC_SLIDER_Y, m_SliderScaleY);
	DDX_Control(pDX, IDC_SLIDER_X, m_SliderScaleX);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_ROTATION, m_SliderLightRt);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_DEPRESSION, m_SliderLightDp);
	DDX_Control(pDX, IDC_SLIDER_LIGHT_DISTANCE, m_SliderLightDs);
	DDX_Control(pDX, IDC_FRAME_COLOR_BACK, m_ControlBackColor);
	DDX_Control(pDX,IDC_RADIO_MODEL0,m_ControlModel0);
	DDX_Control(pDX,IDC_RADIO_MODEL1,m_ControlModel1);
	DDX_Control(pDX,IDC_RADIO_MODEL2,m_ControlModel2);
	DDX_Check(pDX, IDC_CHECK_ANTIALIAS, m_Antialias);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_Smooth);
	DDX_Check(pDX, IDC_CHECK_LIGHTING, m_Lighting);
	DDX_Check(pDX, IDC_CHECK_TEXTURES, m_CubeTextures);
	DDX_Check(pDX, IDC_CHECK_CENTERS, m_CubeCenters);
	DDX_Check(pDX, IDC_CHECK_EDGES, m_CubeEdges);
	DDX_Check(pDX, IDC_CHECK_VERTEXES, m_CubeVertexes);
	DDX_Check(pDX, IDC_CHECK_ROTATION_LIMIT, m_CubeLimit);
	DDX_Check(pDX, IDC_CHECK_LINK_SCALE, m_LinkScale);
	DDX_Check(pDX, IDC_CHECK_LIGHT_SPOT, m_LightSpot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormCommandView, CFormView)
	//{{AFX_MSG_MAP(CFormCommandView)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_MODEL0, OnRadioModel0)
	ON_BN_CLICKED(IDC_RADIO_MODEL1, OnRadioModel1)
	ON_BN_CLICKED(IDC_RADIO_MODEL2, OnRadioModel2)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_ANTIALIAS, OnCheckAntialias)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, OnCheckSmooth)
	ON_BN_CLICKED(IDC_CHECK_LIGHTING, OnCheckLighting)
	ON_BN_CLICKED(IDC_CHECK_TEXTURES, OnCheckTextures)
	ON_BN_CLICKED(IDC_CHECK_CENTERS, OnCheckCenters)
	ON_BN_CLICKED(IDC_CHECK_EDGES, OnCheckEdges)
	ON_BN_CLICKED(IDC_CHECK_VERTEXES, OnCheckVertexes)
	ON_BN_CLICKED(IDC_CHECK_ROTATION_LIMIT, OnCheckRotationLimit)
	ON_BN_CLICKED(IDC_CHECK_LINK_SCALE, OnCheckLinkScale)
	ON_BN_CLICKED(IDC_CHECK_LIGHT_SPOT, OnCheckLightSpot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormCommandView diagnostics

#ifdef _DEBUG
void CFormCommandView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormCommandView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSphereDoc* CFormCommandView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSphereDoc)));
	return (CSphereDoc*)m_pDocument;
}

#endif //_DEBUG

//********************************************
// OnPaint
//********************************************
void CFormCommandView::OnPaint() 
{
	// Device context for painting
	CPaintDC dc(this); 
	
	// Options are stored in Application
	CSphereApp *pApp = (CSphereApp *)AfxGetApp();
	CRect rect;
	
	// Color back
	m_ControlBackColor.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(BackColor);
	dc.FillRect(&rect,&BrushBack);
	
	// Color light ambient
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightAmbient(AmbientColor);
	dc.FillRect(&rect,&BrushLightAmbient);

	// Color light diffuse
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightDiffuse(DiffuseColor);
	dc.FillRect(&rect,&BrushLightDiffuse);

	// Color light specular
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightSpecular(SpecularColor);
	dc.FillRect(&rect,&BrushLightSpecular);
}

//********************************************
// OnLButtonUp
//********************************************
void CFormCommandView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;
	CSphereApp *pApp = (CSphereApp *)AfxGetApp();
	float r,g,b;

	// back color
	m_ControlBackColor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(BackColor);
		if(dlg.DoModal()==IDOK)
		{
			BackColor = dlg.GetColor();	
			CSphereView *pView = (CSphereView *)GetRenderView();
			r = (float)GetRValue(BackColor) / 255.0f;
			g = (float)GetGValue(BackColor) / 255.0f;
			b = (float)GetBValue(BackColor) / 255.0f;
			pView->m_ClearColorRed   = r;
			pView->m_ClearColorGreen = g;
			pView->m_ClearColorBlue  = b;
			glClearColor(r,g,b,1.0f);
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE);
		}
	}

	// ambient light color
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(AmbientColor);
		if(dlg.DoModal()==IDOK)
		{
			AmbientColor = dlg.GetColor();	
			CSphereView *pView = (CSphereView *)GetRenderView();
			// Refresh Light0
			r = (float)GetRValue(AmbientColor) / 255.0f;
			g = (float)GetGValue(AmbientColor) / 255.0f;
			b = (float)GetBValue(AmbientColor) / 255.0f;
			float	ambientProperties[]  = {r,g,b,1.0f};
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
			// Refresh views
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE); 
		}
	}		
	
	// specular light color
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(SpecularColor);
		if(dlg.DoModal()==IDOK)
		{
			SpecularColor = dlg.GetColor();	
			CSphereView*pView = (CSphereView*)GetRenderView();
			// Refresh Light0
			float r = (float)GetRValue(SpecularColor) / 255.0f;
			float g = (float)GetGValue(SpecularColor) / 255.0f;
			float b = (float)GetBValue(SpecularColor) / 255.0f;
			float	specularProperties[]  = {r,g,b,1.0f};
			glLightfv( GL_LIGHT0, GL_DIFFUSE, specularProperties);
			// Refresh views
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE); 
		}
	}		

	// diffuse light color
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(DiffuseColor);
		if(dlg.DoModal()==IDOK)
		{
			DiffuseColor = dlg.GetColor();	
			CSphereView*pView = (CSphereView*)GetRenderView();
			// Refresh Light0
			float r = (float)GetRValue(DiffuseColor) / 255.0f;
			float g = (float)GetGValue(DiffuseColor) / 255.0f;
			float b = (float)GetBValue(DiffuseColor) / 255.0f;
			float	diffuseProperties[]  = {r,g,b,1.0f};
			glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
			// Refresh views
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE); 
		}
	}		
	CFormView::OnLButtonUp(nFlags, point);
}


//********************************************
// GetRenderView
//********************************************
CView *CFormCommandView::GetRenderView() 
{
	CSphereApp *pApp = (CSphereApp*)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	CView *pView = (CView *)pFrame->m_wndSplitter.GetPane(0,1);
	return pView;
}

//********************************************
// Model
//********************************************
void CFormCommandView::OnRadioModel0() 
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	GetRenderView()->InvalidateRect(NULL,FALSE); 
}

void CFormCommandView::OnRadioModel1() 
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	GetRenderView()->InvalidateRect(NULL,FALSE); 
}

void CFormCommandView::OnRadioModel2() 
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	GetRenderView()->InvalidateRect(NULL,FALSE); 
}

void CFormCommandView::OnCheckAntialias()
{
	m_Antialias = !m_Antialias;

	if (m_Antialias)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glLineWidth(1.5f);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glLineWidth(1.0f);
	}
	GetRenderView()->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckSmooth()
{
	m_Smooth = !m_Smooth;
	if (m_Smooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
	GetRenderView()->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckLighting()
{
	m_Lighting = !m_Lighting;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_Lighting)
	{
		pView->m_cube.m_texModulate = TRUE;
		glEnable(GL_LIGHTING);
	}
	else
	{
		pView->m_cube.m_texModulate = FALSE;
		glDisable(GL_LIGHTING);
	}
	
	GetRenderView()->InvalidateRect(NULL,FALSE); 
}

void CFormCommandView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// Slider
	TRACE("Sliders : updating...\n");
	m_SliderScaleX.SetRange(1,100,TRUE);
	m_SliderScaleY.SetRange(1,100,TRUE);
	m_SliderScaleZ.SetRange(1,100,TRUE);

	m_SliderScaleX.SetPos(50);
	m_SliderScaleY.SetPos(50);
	m_SliderScaleZ.SetPos(50);

	m_SliderLightRt.SetRange(0,359,TRUE);
	m_SliderLightDp.SetRange(-90,90,TRUE);
	m_SliderLightDs.SetRange(30,200,TRUE);

	m_SliderLightRt.SetPos(225);
	m_SliderLightDp.SetPos(45);
	m_SliderLightDs.SetPos(150);

	m_ControlModel2.SetCheck(TRUE);
	
}

void CFormCommandView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateScale();
	GetRenderView()->InvalidateRect(NULL,FALSE); 
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CFormCommandView::UpdateScale() 
{
	CSphereView *pView = (CSphereView *)GetRenderView();
	pView->m_xScaling = (float)m_SliderScaleX.GetPos()/50.0f;
	pView->m_yScaling = (float)m_SliderScaleY.GetPos()/50.0f;
	pView->m_zScaling = (float)m_SliderScaleZ.GetPos()/50.0f;

	if(m_LinkScale)
	{
		m_SliderScaleY.SetPos(m_SliderScaleX.GetPos());
		m_SliderScaleZ.SetPos(m_SliderScaleX.GetPos());
		pView->m_yScaling = pView->m_zScaling = pView->m_xScaling;
	}

	pView->m_light.longitude = (float)m_SliderLightRt.GetPos();
	pView->m_light.latitude = (float)m_SliderLightDp.GetPos();
	pView->m_light.radius = (float)m_SliderLightDs.GetPos() / 10.0f;

	return TRUE;
}

void CFormCommandView::OnCheckLinkScale() 
{
	m_LinkScale = !m_LinkScale;	

	if(m_LinkScale)
	{
		CSphereView *pView = (CSphereView*)GetRenderView();
		m_SliderScaleY.SetPos(m_SliderScaleX.GetPos());
		m_SliderScaleZ.SetPos(m_SliderScaleX.GetPos());
		pView->m_yScaling = pView->m_zScaling = pView->m_xScaling;
	}

	m_SliderScaleY.EnableWindow(!m_LinkScale);
	m_SliderScaleZ.EnableWindow(!m_LinkScale);
	GetRenderView()->InvalidateRect(NULL,FALSE);
}

void CFormCommandView::OnCheckTextures()
{
	m_CubeTextures = !m_CubeTextures;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_CubeTextures)
		pView->m_cube.m_useTextures = TRUE;
	else
		pView->m_cube.m_useTextures = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckCenters()
{
	m_CubeCenters = !m_CubeCenters;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_CubeCenters)
		pView->m_cube.m_drawCenters = TRUE;
	else
		pView->m_cube.m_drawCenters = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckEdges()
{
	m_CubeEdges = !m_CubeEdges;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_CubeEdges)
		pView->m_cube.m_drawEdges = TRUE;
	else
		pView->m_cube.m_drawEdges = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckVertexes()
{
	m_CubeVertexes = !m_CubeVertexes;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_CubeVertexes)
		pView->m_cube.m_drawVertexes = TRUE;
	else
		pView->m_cube.m_drawVertexes = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckRotationLimit()
{
	m_CubeLimit = !m_CubeLimit;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_CubeLimit)
		pView->m_yRotationLimit = TRUE;
	else
		pView->m_yRotationLimit = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView::OnCheckLightSpot()
{
	m_LightSpot = !m_LightSpot;
	CSphereView* pView = (CSphereView*)GetRenderView();
	if (m_LightSpot)
		pView->m_light.isSpot = TRUE;
	else
		pView->m_light.isSpot = FALSE;
	pView->InvalidateRect(NULL, FALSE);
}
