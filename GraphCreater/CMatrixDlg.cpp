// CMatrixDlg.cpp: файл реализации
//

#include "pch.h"
#include "GraphCreater.h"
#include "afxdialogex.h"
#include "CMatrixDlg.h"
#include <string>


// Диалоговое окно CMatrixDlg
#define CUSTOMBOX_CLASSNAME _T("CustomBox")
IMPLEMENT_DYNAMIC(CMatrixDlg, CDialogEx)

CMatrixDlg::CMatrixDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMATRIXDLG, pParent)
{
	
}

CMatrixDlg::~CMatrixDlg()
{
}

void CMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CUSTOMBOX, MatrixRuler);
}


BEGIN_MESSAGE_MAP(CMatrixDlg, CDialogEx)
	ON_BN_CLICKED(IDOK,&CMatrixDlg::ClickedOK)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CMatrixDlg::ClickedOK() {
	this->EndDialog(IDOK);
}
// Обработчики сообщений CMatrixDlg
void CMatrixDlg::SetDocument(CGraphCreaterDoc* doc) {
	this->pDoc = doc;
}

BOOL CMatrixDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//CRect rect(10, 10, 200, 200);
	//if (!MatrixRuler.Create(CUSTOMBOX_CLASSNAME, _T("Matrix"), WS_CHILD | WS_VISIBLE, rect, this, IDC_CUSTOMBOX)) {
	//	AfxMessageBox(_T("Failed to create custom control"));
	//	return false;
	//}
	// TODO:  Добавить дополнительную инициализацию
	GetClientRect(DialogSize);
	DialogSize.bottom = DialogSize.bottom - 60;
	DialogSize.right = DialogSize.right - 20;
	DialogSize.left = DialogSize.left + 20;
	DialogSize.top = DialogSize.top + 20;
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
CString ConvertioD(std::string name) {
	CString end;
	for (auto i : name) {
		end.AppendChar(i);
	}
	return end;
}
void CMatrixDlg::DrawLines(CPaintDC* dc){
	int MaxName = 1;
	double ky = (DialogSize.bottom - DialogSize.top) / (pDoc->Verticals.size()+1);
	double kx = (DialogSize.right - DialogSize.left) / (pDoc->Verticals.size()+1);
	CSize textSize;
	for (auto i : pDoc->Verticals) {
		if (i.GetName().size() > MaxName) {
			MaxName = i.GetName().size();
			textSize = dc->GetTextExtent(ConvertioD(i.GetName()));
		}
	}
	for (int i = 1; i < pDoc->Verticals.size()+2; i++) {
		dc->MoveTo(DialogSize.left,DialogSize.top+i*ky);
		dc->LineTo(DialogSize.left + kx * (pDoc->Verticals.size()+1), DialogSize.top + i * ky);
		dc -> MoveTo(DialogSize.left + kx * i, DialogSize.top);
		dc->LineTo(DialogSize.left + kx * i, DialogSize.top + (pDoc->Verticals.size()+1) * ky);
	}
}
void CMatrixDlg::GetRightFont(std::string text,CPaintDC* dc,double MaxWidth,double MaxHeight) {
	CFont font, * oldFont;
	int fontSize = 160;
	font.CreatePointFont(fontSize, _T("Arial"));
	oldFont = dc->SelectObject(&font);
	CSize textsize = dc->GetTextExtent(ConvertioD(text));
	while ((textsize.cx > MaxWidth || textsize.cy > MaxHeight) && fontSize > 0) {
		fontSize = fontSize - 10;
		font.DeleteObject();
		font.CreatePointFont(fontSize, _T("Arial"));
		dc->SelectObject(&font);
		textsize = dc->GetTextExtent(ConvertioD(text));
	}
}
std::wstring ToWStringD(std::string name) {
	std::vector<wchar_t> buffer(name.size() + 1);
	mbstowcs(buffer.data(), name.c_str(), name.size() + 1);
	std::wstring wstr(buffer.data());
	return wstr;
}
void CMatrixDlg::DrawNames(CPaintDC* dc) {
	double ky = (DialogSize.bottom - DialogSize.top) / (pDoc->Verticals.size()+1);
	double kx = (DialogSize.right - DialogSize.left) / (pDoc->Verticals.size()+1);
	for (int i = 0; i < pDoc->Verticals.size();i++) {
		GetRightFont(pDoc->Verticals[i].GetName(), dc, kx, ky);
		CRect rect = { int(DialogSize.left + kx * (i+1)),int(DialogSize.top),int(DialogSize.left + kx * (i + 2)),int(DialogSize.top + ky) };
		dc->DrawText(ToWStringD(pDoc->Verticals[i].GetName()).c_str(), pDoc->Verticals[i].GetName().size(), &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		rect = { int(DialogSize.left),int(DialogSize.top + ky * (i+1)),int(DialogSize.left + kx),int(DialogSize.top + ky * (i+2)) };
		dc->DrawText(ToWStringD(pDoc->Verticals[i].GetName()).c_str(), pDoc->Verticals[i].GetName().size(), &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
}
void CMatrixDlg::DrawNumbers(CPaintDC* dc) {
	int size = pDoc->Verticals.size();
	double ky = (DialogSize.bottom - DialogSize.top) / (size + 1);
	double kx = (DialogSize.right - DialogSize.left) / (size + 1);
	int** SubMatrix = new int*[size];
	
	for (int i = 0; i < size; i++) {
		SubMatrix[i] = new int[size];
	}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			SubMatrix[i][j] = 0;
	for (auto k : pDoc->Edges) {
		for (int first = 0; first <size; first++) {
			if (k.verts[0] == &pDoc->Verticals[first]) {
				for (int second = 0; second < size; second++) {
					if (k.verts[1] == &pDoc->Verticals[second]) {
						SubMatrix[first][second] = 1;
						break;
					}
				}
				break;
			}
		}
	}
	for (int Y = 0; Y < size; Y++) {
		for (int X = 0; X < size; X++) {
			GetRightFont("0", dc, kx, ky);
			CRect rect = { int(DialogSize.left + kx * (X + 1)),int(DialogSize.top+ky*(Y+1)),int(DialogSize.left + kx * (X + 2)),int(DialogSize.top + ky*(Y+2)) };
			dc->DrawText(std::to_wstring(SubMatrix[Y][X]).c_str(), 1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}
}
void CMatrixDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: добавьте свой код обработчика сообщений
	// Не вызывать CDialogEx::OnPaint() для сообщений рисования
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen);
	DrawLines(&dc);
	DrawNames(&dc);
	DrawNumbers(&dc);
	dc.SelectObject(pOldPen);
}
