
// GraphCreaterDoc.h: интерфейс класса CGraphCreaterDoc 
//


#pragma once
#include "Edge.h"
#include <deque>
#include <vector>

class CGraphCreaterDoc : public CDocument
{
protected: // создать только из сериализации
	CGraphCreaterDoc() noexcept;
	DECLARE_DYNCREATE(CGraphCreaterDoc)

// Атрибуты
public:
	char ChosenType = 'M';
	Vertical Necessary;
	Vertical* temp;
	std::deque<Vertical> Verticals;
	std::vector<Edge> Edges;
	CString m_SavedFilePath;
// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	void Invalidate();
	void SaveAsk();
	void ChangeWindowText(CString filename);
	void Modified();
	void SaveToJSON(CString filename);
	CString GetFilterType(DWORD num);
	void LoadVerticalsFromFile(CString filename);
	void LoadEdgesFromFile(CString filename);
	virtual ~CGraphCreaterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
};
