
// GraphCreaterView.cpp: реализация класса CGraphCreaterView
//

#include <cmath>
#include "pch.h"
#include "framework.h"
#include <sstream>
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GraphCreater.h"
#endif

#include "GraphCreaterDoc.h"
#include "GraphCreaterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define radius 15

// CGraphCreaterView

IMPLEMENT_DYNCREATE(CGraphCreaterView, CView)

BEGIN_MESSAGE_MAP(CGraphCreaterView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// Создание или уничтожение CGraphCreaterView

CGraphCreaterView::CGraphCreaterView() noexcept
{
	// TODO: добавьте код создания

}

CGraphCreaterView::~CGraphCreaterView()
{
}

BOOL CGraphCreaterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void CGraphCreaterView::DrawArrow(CDC* pDC, CPoint* points) {
	pDC->MoveTo(points[0].x, points[0].y);
	double j = atan2(double(points[1].y - points[0].y), double(points[1].x - points[0].x));
	pDC->LineTo(points[1].x - radius * cos(j), points[1].y - radius * sin(j));
	CPoint end(points[1].x - radius * cos(j), points[1].y - radius * sin(j));
	const double angle = 3.14159265358979323846 /9;
	int Length = 25;
	pDC->MoveTo(end);
	j = atan2(double(end.y - points[0].y), double(end.x - points[0].x));
	int Xn = static_cast<int>(end.x - Length * cos(j-angle));
	int Yn = static_cast<int>(end.y - Length * sin(j-angle));
	pDC -> LineTo(Xn,Yn);
	pDC->MoveTo(end);
	 Xn = static_cast<int>(end.x - Length * cos(j+angle));
	 Yn = static_cast<int>(end.y - Length * sin(j+angle));
	pDC->LineTo(Xn, Yn);
}
// Рисование CGraphCreaterView

void CGraphCreaterView::OnDraw(CDC* pDC)
{
	CGraphCreaterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	LPCTSTR lpsz;
	HWND* hWnd;
	RECT rect;
	CPoint ptr = pDC->GetWindowOrg();
	CBrush hBrush;
	CBrush* old_brush = nullptr;
	hBrush.CreateSolidBrush(RGB(230, 30, 30));
	old_brush = pDC->SelectObject(&hBrush);
	for (int i = 0; i < pDoc->Verticals.GetSize();i++) {
		pDC->Ellipse(pDoc->Verticals[i].x-radius, pDoc->Verticals[i].y - radius, pDoc->Verticals[i].x + radius, pDoc->Verticals[i].y + radius);
	}
	pDC->SelectObject(old_brush);
	old_brush = nullptr;
	CPen hPen;
	CPen* oldPen;
	hPen.CreatePen(PS_SOLID, 2, RGB(230, 30, 30));
	oldPen = pDC->SelectObject(&hPen);
	for (int i = 0; i < pDoc->Edges.GetSize(); i++) {
		CPoint points[2];
		points[0].SetPoint(pDoc->Edges[i].verts[0].x, pDoc->Edges[i].verts[0].y);
		points[1].SetPoint(pDoc->Edges[i].verts[1].x, pDoc->Edges[i].verts[1].y);
		DrawArrow(pDC,points);
	}
	if (onDrawState) {
		CPoint points[2];
		points[0].x = temp.verts[0].x;
		points[0].y = temp.verts[0].y;
		points[1].x = temp.verts[1].x;
		points[1].y = temp.verts[1].y;
		DrawArrow(pDC,points);
	}
	DeleteObject(hBrush);
	DeleteObject(hPen);
	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Печать CGraphCreaterView

BOOL CGraphCreaterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CGraphCreaterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CGraphCreaterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CGraphCreaterView

#ifdef _DEBUG
void CGraphCreaterView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphCreaterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphCreaterDoc* CGraphCreaterView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphCreaterDoc)));
	return (CGraphCreaterDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CGraphCreaterView


void CGraphCreaterView::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	auto pDoc = GetDocument();
	if (pDoc->ChosenType == 'V') {
		std::string ss = std::to_string(pDoc->Verticals.GetSize()+1);
		Vertical vert(ss, int(point.x), int(point.y));
		pDoc->Verticals.Add(vert);
		Invalidate();
	}
	else if(pDoc->ChosenType == 'E') {
		Vertical* vert;
		for (int i = 0; i < pDoc->Verticals.GetSize(); i++) {
			if ((pDoc->Verticals[i].x - 15 <= point.x && pDoc->Verticals[i].x + 15 >= point.x)&&(pDoc->Verticals[i].y - 15 <= point.y && pDoc->Verticals[i].y + 15 >= point.y)&&onDrawState==false) {
				vert = &(pDoc->Verticals[i]);
				temp.ReWriteFirst(*vert);
				temp.AddConnection(*vert);
				onDrawState = true;
				Invalidate();
				break;
			}
		}	
	}
	CView::OnLButtonDown(nFlags, point);
}


void CGraphCreaterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (onDrawState) {
		auto pDoc = GetDocument();
		Vertical* vert;
		for (int i = 0; i < pDoc->Verticals.GetSize(); i++) {
			if ((pDoc->Verticals[i].x - 15 <= point.x && pDoc->Verticals[i].x + 15 >= point.x) && (pDoc->Verticals[i].y - 15 <= point.y && pDoc->Verticals[i].y + 15 >= point.y)) {
				vert = &(pDoc->Verticals[i]);
				temp.AddConnection(*vert);
				pDoc->Edges.Add(temp);
				onDrawState = false;
				
				break;
			}
		}
	}
	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}


void CGraphCreaterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (onDrawState) {
		temp.verts[1].x = point.x;
		temp.verts[1].y = point.y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
