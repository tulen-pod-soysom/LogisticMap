
// LogisticMapDlg.h: файл заголовка
//
#include "Graph.h"


#pragma once


// Диалоговое окно CLogisticMapDlg
class CLogisticMapDlg : public CDialogEx
{
// Создание
public:
	CLogisticMapDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGISTICMAP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Graph graph;
	Program program;
	Graph graph2;
	CSliderCtrl slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl slider2;
	virtual void OnOK();
};
