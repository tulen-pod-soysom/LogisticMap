
// LogisticMapDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "LogisticMap.h"
#include "LogisticMapDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	void program_update();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// Диалоговое окно CLogisticMapDlg



CLogisticMapDlg::CLogisticMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGISTICMAP_DIALOG, pParent) , program()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogisticMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICGRAPH, graph);
	DDX_Control(pDX, IDC_STATICGRAPH2, graph2);
	DDX_Control(pDX, IDC_SLIDER1, slider);
	DDX_Control(pDX, IDC_SLIDER2, slider2);
}

BEGIN_MESSAGE_MAP(CLogisticMapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// Обработчики сообщений CLogisticMapDlg

BOOL CLogisticMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	slider.SetRange(0, 400, 1);
	slider2.SetRange(1, 100, 1);

	slider2.SetPos(50);

	// TODO: добавьте дополнительную инициализацию
	program.evaluate();
	program.evaluate_2();
	graph.worldRect = { 0,0,4,1 };
	graph.title.Format(L"Бифуркационная диаграмма (X0 = %f)", program.x0);
	graph.captionX = "R";
	graph.captionY.Format(L"X^%u", program.N);
	graph.program = &program;
	graph.graph_type = 1;


	graph2.worldRect = { 0,0,(float)program.N,1 };
	graph2.title.Format(L"График при R = %f, X0 = %f", program.R0, program.x0);
	graph2.captionX = "n";
	graph2.captionY.Format(L"X^n");
	graph2.program = &program;
	graph2.graph_type = 2;

	RedrawWindow();
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CLogisticMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CLogisticMapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CLogisticMapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLogisticMapDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	program.R0 = 0.0 + slider.GetPos() / 100.;
	program.x0 = 0.0 + slider2.GetPos() / 100.;

	program.evaluate_2();

	graph2.worldRect = { 0,0,(float)program.N,1 };
	graph2.title.Format(L"График при R = %f, X0 = %f", program.R0,program.x0);
	graph2.captionX = "n";
	graph2.captionY.Format(L"X^n");
	graph2.program = &program;
	graph2.graph_type = 2;

	graph2.RedrawWindow();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CLogisticMapDlg::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса

	program.evaluate();

	graph.title.Format(L"Бифуркационная диаграмма (X0 = %f)", program.x0);
	graph.RedrawWindow();

	//CDialogEx::OnOK();
}
