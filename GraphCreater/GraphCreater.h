
// GraphCreater.h: основной файл заголовка для приложения GraphCreater
//
#pragma once
#include "CMatrixDlg.h"
#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CGraphCreaterApp:
// Сведения о реализации этого класса: GraphCreater.cpp
//

class CGraphCreaterApp : public CWinApp
{
	
public:
	CGraphCreaterApp() noexcept;

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Реализация
	void ChoseOfVertical();
	void ChoseOfEdge();
	void ChoseOfMove();
	void ChangeName();
	void CreateMatrix();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
/*#ifndef _DEBUG  // версия отладки в GraphCreaterView.cpp
inline CGraphCreaterDoc* CGraphCreaterApp::GetDocument() const
{
	return reinterpret_cast<CGraphCreaterDoc*>(m_pDocument);
}
#endif*/
extern CGraphCreaterApp theApp;
