
// GraphCreaterView.h: интерфейс класса CGraphCreaterView
//

#pragma once


class CGraphCreaterView : public CView
{
	void DrawArrow(CDC* pDC, CPoint* points);
protected: // создать только из сериализации
	CGraphCreaterView() noexcept;
	DECLARE_DYNCREATE(CGraphCreaterView)

// Атрибуты
public:
	CGraphCreaterDoc* GetDocument() const;
	Edge temp;
	bool onDrawState = false;
// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CGraphCreaterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // версия отладки в GraphCreaterView.cpp
inline CGraphCreaterDoc* CGraphCreaterView::GetDocument() const
   { return reinterpret_cast<CGraphCreaterDoc*>(m_pDocument); }
#endif

