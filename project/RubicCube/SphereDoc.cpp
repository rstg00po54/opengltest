// SphereDoc.cpp : implementation of the CSphereDoc class

#include "stdafx.h"
#include "Sphere.h"

#include "SphereDoc.h"
#include "MainFrm.h"
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSphereDoc

IMPLEMENT_DYNCREATE(CSphereDoc, CDocument)

BEGIN_MESSAGE_MAP(CSphereDoc, CDocument)
	//{{AFX_MSG_MAP(CSphereDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSphereDoc construction/destruction

CSphereDoc::CSphereDoc()
{
	memset(m_string, '\0', 256);
}

CSphereDoc::~CSphereDoc()
{
}

BOOL CSphereDoc::OnNewDocument()
{
	CDocument::SetTitle("Magic Cube");
	return CDocument::OnNewDocument();
}



/////////////////////////////////////////////////////////////////////////////
// CSphereDoc serialization

void CSphereDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSphereDoc diagnostics

#ifdef _DEBUG
void CSphereDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSphereDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



BOOL CSphereDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	FILE* pFile;
	fopen_s(&pFile, lpszPathName, "r");
	if (pFile != NULL)
	{
		struct stat statbuf;
		stat(lpszPathName, &statbuf);
		fread_s(m_string, 255, 1, statbuf.st_size, pFile);
	}
	return NULL;
}


void CSphereDoc::OnCloseDocument()
{
	memset(m_string, '\0', 256);
	CDocument::OnCloseDocument();
}
