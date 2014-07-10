// FireworkDoc.cpp : implementation of the CFireworkDoc class
//

#include "stdafx.h"
#include "Firework.h"

#include "FireworkDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFireworkDoc

IMPLEMENT_DYNCREATE(CFireworkDoc, CDocument)

BEGIN_MESSAGE_MAP(CFireworkDoc, CDocument)
END_MESSAGE_MAP()


// CFireworkDoc construction/destruction

CFireworkDoc::CFireworkDoc()
{
	// TODO: add one-time construction code here

}

CFireworkDoc::~CFireworkDoc()
{
}

BOOL CFireworkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CFireworkDoc serialization

void CFireworkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CFireworkDoc diagnostics

#ifdef _DEBUG
void CFireworkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFireworkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFireworkDoc commands
