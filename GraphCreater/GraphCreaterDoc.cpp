
// GraphCreaterDoc.cpp: реализация класса CGraphCreaterDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GraphCreater.h"
#endif

#include "GraphCreaterDoc.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <propkey.h>
#include "Vertical.h"
#include <vector>
#include "Edge.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define file_name "Graphs.json"
// CGraphCreaterDoc

IMPLEMENT_DYNCREATE(CGraphCreaterDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphCreaterDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CGraphCreaterDoc

CGraphCreaterDoc::CGraphCreaterDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CGraphCreaterDoc::~CGraphCreaterDoc()
{
}

BOOL CGraphCreaterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}

void CGraphCreaterDoc::LoadVerticalsFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Can't load file");
	}
	nlohmann::json j;
	file >> j;
	for (const auto& item : j["verticals"]) {
		Vertical vertical;
		vertical.ChangeName(item["name"]);
		vertical.x = item["X"];
		vertical.y = item["Y"];
		Verticals.Add(vertical);
	}
}

// Сериализация CGraphCreaterDoc
void CGraphCreaterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		AfxMessageBox(_T("Попытка сериализации"));
		Verticals.RemoveAll();
		LoadVerticalsFromFile(file_name);
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CGraphCreaterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CGraphCreaterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CGraphCreaterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CGraphCreaterDoc

#ifdef _DEBUG
void CGraphCreaterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphCreaterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CGraphCreaterDoc
