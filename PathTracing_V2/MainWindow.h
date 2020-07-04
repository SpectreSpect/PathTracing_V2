#pragma once
#include "BaseWindow.h"
class MainWindow : public BaseWindow<MainWindow>
{
public:
	virtual LPCWSTR ClassName() override { return L"MAINWINDOW123"; };
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

