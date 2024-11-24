#pragma once
#include "afxdialogex.h"
#include "Vertical.h"
#include "Edge.h"
#include "GraphCreaterDoc.h"


// Диалоговое окно CMatrixDlg

class CMatrixDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMatrixDlg)
	CGraphCreaterDoc* pDoc = nullptr;
	
public:
	CMatrixDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMatrixDlg();
	afx_msg void ClickedOK();
	void SetDocument(CGraphCreaterDoc* doc);
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMatrixDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	CRect DialogSize;
	DECLARE_MESSAGE_MAP()
public:
	void DrawLines(CPaintDC* dc);
	void DrawNames(CPaintDC* dc);
	void DrawNumbers(CPaintDC* dc);
	void GetRightFont(std::string text,CPaintDC* dc,double MaxWidth,double MaxHeight);
	virtual BOOL OnInitDialog();
	//CMatrixView MatrixRuler;
	afx_msg void OnPaint();
};
