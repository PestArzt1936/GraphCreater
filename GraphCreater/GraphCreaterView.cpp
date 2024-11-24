
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
	ON_EN_CHANGE(5, &CGraphCreaterView::OnEnChangeMyEdit)
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
CString Convertio(std::string name) {
	CString end;
	for (auto i : name) {
		end.AppendChar(i);
	}
	return end;
}
std::string ConvertioToSTD(CString name) {
	std::string end;
	for (int i = 0; i < name.GetLength();i++) {
		end.push_back(name[i]);
	}
	return end;
}
std::wstring ToWString(std::string name) {
	//Код ниже попытка отобразить русские символы тоже(неудачная попытка)
	/*if (name.empty())
		return std::wstring();
	int utf16_length = MultiByteToWideChar(CP_UTF8, 0, name.data(), name.size(), NULL, 0);
	if (utf16_length == 0)
		throw std::runtime_error("Ошибка конвертации в UTF-16");
	std::wstring utf16_string(utf16_length, 0);
	MultiByteToWideChar(CP_UTF8, 0, name.data(), name.size(), &utf16_string[0], utf16_length);
	return utf16_string;*/
	std::vector<wchar_t> buffer(name.size() + 1);
	mbstowcs(buffer.data(), name.c_str(), name.size() + 1);
	std::wstring wstr(buffer.data());
	return wstr;
}
void CGraphCreaterView::OnDraw(CDC* pDC)
{
	CGraphCreaterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBrush hBrush;
	CBrush* old_brush = nullptr;
	hBrush.CreateSolidBrush(RGB(30, 30, 230));
	old_brush = pDC->SelectObject(&hBrush);
	for (int i = 0; i < pDoc->Verticals.size();i++) {
		pDC->Ellipse(pDoc->Verticals[i].x-radius, pDoc->Verticals[i].y - radius, pDoc->Verticals[i].x + radius, pDoc->Verticals[i].y + radius);
		CString name = Convertio(pDoc->Verticals[i].GetName());
		RECT rect = { pDoc->Verticals[i].x-name.GetLength() * 4,pDoc->Verticals[i].y + radius * 1.5,pDoc->Verticals[i].x+name.GetLength() * 4,pDoc->Verticals[i].y + radius * 1.5+20 };
		pDC->DrawText(ToWString(pDoc->Verticals[i].GetName()).c_str(), pDoc->Verticals[i].GetName().size(), &rect, DT_CENTER);
	}
	pDC->SelectObject(old_brush);
	old_brush = nullptr;
	CPen hPen;
	CPen* oldPen;
	hPen.CreatePen(PS_SOLID, 2, RGB(30, 30, 230));
	oldPen = pDC->SelectObject(&hPen);
	for (int i = 0; i < pDoc->Edges.size(); i++) {
		CPoint points[2];
		points[0].SetPoint(pDoc->Edges[i].verts[0]->x, pDoc->Edges[i].verts[0]->y);
		points[1].SetPoint(pDoc->Edges[i].verts[1]->x, pDoc->Edges[i].verts[1]->y);
		DrawArrow(pDC,points);
		CString name = Convertio(pDoc->Edges[i].GetName());
		int deltaX = pDoc->Edges[i].verts[0]->x - pDoc->Edges[i].verts[1]->x;
		int deltaY= pDoc->Edges[i].verts[0]->y - pDoc->Edges[i].verts[1]->y;
		pDC->TextOut(pDoc->Edges[i].verts[0]->x-deltaX/2 - name.GetLength() * 4, pDoc->Edges[i].verts[0]->y-deltaY/2 - radius * 2.0, name, name.GetLength());
	}
	if (onDrawState) {
		if (pDoc->ChosenType == 'E') {
			CPoint points[2];
			points[0].x = temp.verts[0]->x;
			points[0].y = temp.verts[0]->y;
			points[1].x = temp.verts[1]->x;
			points[1].y = temp.verts[1]->y;
			DrawArrow(pDC, points);
		}
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
		std::string ss = std::to_string(pDoc->Verticals.size()+1);
		Vertical vert(ss, int(point.x), int(point.y));
		pDoc->Verticals.push_back(vert);
		pDoc->Modified();
		Invalidate();
	}
	else if (pDoc->ChosenType=='M') {
		for (int i = 0; i < pDoc->Verticals.size();i++) {
			if ((pDoc->Verticals[i].x - 15 <= point.x && pDoc->Verticals[i].x + 15 >= point.x) && (pDoc->Verticals[i].y - 15 <= point.y && pDoc->Verticals[i].y + 15 >= point.y) && onDrawState == false) {
				pDoc->temp = &pDoc->Verticals[i];
				onDrawState = true;
				Invalidate();
				break;
			}
		}
	}
	else if(pDoc->ChosenType == 'E') {
		Vertical* vert;
		for (int i = 0; i < pDoc->Verticals.size(); i++) {
			if ((pDoc->Verticals[i].x - 15 <= point.x && pDoc->Verticals[i].x + 15 >= point.x)&&(pDoc->Verticals[i].y - 15 <= point.y && pDoc->Verticals[i].y + 15 >= point.y)&&onDrawState==false) {
				vert = &(pDoc->Verticals[i]);
				temp.ReWriteFirst(vert);
				pDoc->Necessary = *vert;
				temp.AddConnection(&pDoc->Necessary);
				onDrawState = true;
				Invalidate();
				break;
			}
		}	
	}
	else if (pDoc->ChosenType=='C') {
		if (m_edit.GetSafeHwnd() != nullptr){
				m_edit.DestroyWindow();
			}
		for (int i = 0; i < pDoc->Verticals.size();i++) {
			RECT rect = { pDoc->Verticals[i].x - pDoc->Verticals[i].GetName().size() * 4,pDoc->Verticals[i].y + radius * 1.5,pDoc->Verticals[i].x + pDoc->Verticals[i].GetName().size() * 4,pDoc->Verticals[i].y + radius * 1.5 + 20};
			CPaintDC DC(this);
			CSize size=DC.GetTextExtent(_T("mmmmmmmmmmmmmmmmmmmm"));
			rect.left = rect.left - size.cx / 2;
			rect.right = rect.right + size.cx / 2;
			if (PtInRect(&rect, point)) {
					pDoc->temp = &pDoc->Verticals[i];
					rect.left = rect.left - 20;
					rect.right = rect.right + 20;
					m_edit.Create(WS_CHILDWINDOW | WS_VISIBLE | ES_CENTER | WS_BORDER, rect, this, 5);
					m_edit.SetWindowText(ToWString(pDoc->Verticals[i].GetName()).c_str());
					m_edit.SetFocus();
					m_edit.SetOwner(this);
					m_edit.SetDlgCtrlID(5);
					Invalidate();	
				break;
			}
		}
	}
	CView::OnLButtonDown(nFlags, point);
}
void CGraphCreaterView::OnEnChangeMyEdit() {
	CString text;
	m_edit.GetWindowText(text);
	if (text.GetLength() <= 20) {
		CDC* pDC = GetDC();
		CSize textSize = pDC->GetTextExtent(text);
		CRect rect;
		m_edit.GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.right = rect.left + textSize.cx + 15;
		m_edit.MoveWindow(&rect);
		ReleaseDC(pDC);
	}
}

void CGraphCreaterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (onDrawState) {
		auto pDoc = GetDocument();
		if (pDoc->ChosenType == 'E') {
			Vertical* vert;
			bool VertFound = false;
			for (int i = 0; i < pDoc->Verticals.size(); i++) {
				if ((pDoc->Verticals[i].x - 15 <= point.x && pDoc->Verticals[i].x + 15 >= point.x) && (pDoc->Verticals[i].y - 15 <= point.y && pDoc->Verticals[i].y + 15 >= point.y)) {
					vert = &(pDoc->Verticals[i]);
					temp.AddConnection(vert);
					temp.ChangeName('x' + std::to_string(pDoc->Edges.size() + 1));
					pDoc->Edges.push_back(temp);
					onDrawState = false;
					pDoc->Modified();
					VertFound = true;
					break;
				}
			}
			if (!VertFound) {
				onDrawState = false;
			}
			pDoc->temp = nullptr;
		}
		else if (pDoc->ChosenType=='M') {
			pDoc->temp = nullptr;
			pDoc->Modified();
			onDrawState = false;
		}
		Invalidate();
	}
	CView::OnLButtonUp(nFlags, point);
}


void CGraphCreaterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (onDrawState) {
		auto pDoc = GetDocument();
		if (pDoc->ChosenType == 'E') {
			temp.verts[1]->x = point.x;
			temp.verts[1]->y = point.y;
		}
		else if (pDoc->ChosenType=='M') {
			pDoc->temp->x = point.x;
			pDoc->temp->y = point.y;
		}
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}

BOOL CGraphCreaterView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: добавьте специализированный код или вызов базового класса
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)) {
		CGraphCreaterDoc* pDoc = GetDocument();
		if (pMsg->wParam == VK_RETURN) {
			if (m_edit.GetSafeHwnd() != nullptr) {
				CString text;
				m_edit.GetWindowText(text);
				pDoc->temp->ChangeName(ConvertioToSTD(text));
				m_edit.DestroyWindow();
				pDoc->temp = nullptr;
				Invalidate();
			}
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			if (m_edit.GetSafeHwnd() != nullptr) {
				m_edit.DestroyWindow();
			}
		}
		return true;
	}
	return CView::PreTranslateMessage(pMsg);
}
