// SphereDoc.h : interface of the CSPhereDoc class

#ifndef ___SPHERE_DOC_H___
#define ___SPHERE_DOC_H___

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSphereDoc : public CDocument
{
protected: // create from serialization only
	CSphereDoc();
	DECLARE_DYNCREATE(CSphereDoc)

// Attributes
public:
	char m_string[256];
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSphereDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSphereDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSphereDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTESTOPENGLDOC_H__5E04A7D9_D353_4E60_AE7C_342C4A8D3C64__INCLUDED_)
