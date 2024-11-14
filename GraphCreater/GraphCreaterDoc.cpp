
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
static TCHAR BASED_CODE szFilter[] = _T("Файл JSON(*.json)|*.json|");
// CGraphCreaterDoc

IMPLEMENT_DYNCREATE(CGraphCreaterDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphCreaterDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE,&CGraphCreaterDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CGraphCreaterDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW,&CGraphCreaterDoc::OnFileNew)
	ON_COMMAND(ID_FILE_CLOSE,&CGraphCreaterDoc::OnCloseDocument)
	ON_COMMAND(ID_FILE_OPEN, &CGraphCreaterDoc::OnFileOpen)
END_MESSAGE_MAP()


// Создание или уничтожение CGraphCreaterDoc
CString CGraphCreaterDoc::GetFilterType(DWORD num) {
	switch (num)
	{
	case 1:
		return(_T(".json"));
	default:
		break;
	}
}
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

void CGraphCreaterDoc::LoadVerticalsFromFile(CString filename) {
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
		Verticals.push_back(vertical);
	}
}
void CGraphCreaterDoc::LoadEdgesFromFile(CString filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Can't load file");
	}
	nlohmann::json j;
	file >> j;
	for (const auto& item : j["edges"]) {
		Edge edge;
		edge.ChangeName(item["name"]);
		Vertical* first=nullptr;
		Vertical* second=nullptr;
		for (int i = 0; i < Verticals.size(); i++) { 
			if (Verticals[i].GetName() == std::string(item["first"]))
				first = &Verticals[i];
			if (Verticals[i].GetName() == std::string(item["second"]))
				second = &Verticals[i];
		}
		if (first == nullptr || second == nullptr) {
			continue;
		}
		else{
			edge.ReWriteFirst(first);
			edge.AddConnection(second);
			Edges.push_back(edge);
		}
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
	}
}
void CGraphCreaterDoc::OnFileSave() {
#ifdef _DEBUG
	AfxMessageBox(_T("Произошло сохранение"));
#else
#endif
	if (m_SavedFilePath == "") {
		OnFileSaveAs();
		return;
	}
	else {
		SaveToJSON(m_SavedFilePath);
	}
}
void CGraphCreaterDoc::OnFileSaveAs() {
#ifdef _DEBUG
	AfxMessageBox(_T("Произошло сохранение как..."));
#else
#endif
	CFileDialog MyDialog(false, NULL, m_SavedFilePath!=""?m_SavedFilePath:NULL, NULL, szFilter, NULL, 0, true);
	if (MyDialog.DoModal() == IDOK) {
		CString FileName= MyDialog.GetPathName();
		FileName.Append(GetFilterType(MyDialog.GetOFN().nFilterIndex));
		SaveToJSON(FileName);
		m_SavedFilePath = FileName;
	}
}
void CGraphCreaterDoc::SaveToJSON(CString filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Can't load file");
	}
	if (file.is_open()) {
		nlohmann::json jsonData;
		jsonData["verticals"] = nlohmann::json::array();
		jsonData["edges"] = nlohmann::json::array();
		for (int i = 0; i < Verticals.size(); i++) {
			jsonData["verticals"].push_back({ {"name",Verticals[i].GetName()},{"X",Verticals[i].x},{"Y",Verticals[i].y} });
		}
		for (int i = 0; i < Edges.size(); i++) {
			jsonData["edges"].push_back({ {"name",Edges[i].GetName()},{"first",Edges[i].verts[0]->GetName()},{"second",Edges[i].verts[1]->GetName()} });
		}
		file << jsonData.dump(4);
		file.close();
		
	}
	else {
		throw std::runtime_error("Unpredictable ERROR");
	}
	SetModifiedFlag(false);
}
void CGraphCreaterDoc::OnFileNew() {
	AfxMessageBox(_T("CDoc"));
	SaveAsk();
	SetModifiedFlag(false);
	for (int i = 0; i < Edges.size(); i++) {
		delete Edges[i].verts[0];
		Edges[i].verts[0] = nullptr;
		delete Edges[i].verts[1];
		Edges[i].verts[1] = nullptr;
	}
	Edges.clear();
	Verticals.clear();
	m_SavedFilePath = _T("");
	CDocument::OnNewDocument();
	Invalidate();
}
void CGraphCreaterDoc::Invalidate() {
	POSITION pos = GetFirstViewPosition();
	while (pos != nullptr) {
		CView* pView = GetNextView(pos);
		if (pView != nullptr) {
			pView->Invalidate();
		}
	}
}
void CGraphCreaterDoc::Modified() {
	SetModifiedFlag(true);
}
void CGraphCreaterDoc::SaveAsk() {
	if (IsModified()) {
		if (AfxMessageBox(_T("Сохранить изменения?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			OnFileSave();
		}
		else
			return;
	}
}
void CGraphCreaterDoc::OnCloseDocument() {
	if (IsModified()) {
		if (AfxMessageBox(_T("Сохранить изменения?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			OnFileSave();
		}
		
	}
	CDocument::OnCloseDocument();
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


void CGraphCreaterDoc::OnFileOpen()
{
	// TODO: добавьте свой код обработчика команд
#ifdef _DEBUG
	AfxMessageBox(_T("Произошло открытие"));
#else
#endif
	CFileDialog MyDialog(true, NULL, NULL, NULL, szFilter, NULL, 0, true);
	if (MyDialog.DoModal() == IDOK) {
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		m_SavedFilePath = MyDialog.GetPathName();
		LoadVerticalsFromFile(CString(MyDialog.GetPathName()));
		LoadEdgesFromFile(CString(MyDialog.GetPathName()));
		Invalidate();
	}
}
