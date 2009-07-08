#pragma once

#include "boostheaders.h"

#include <windows.h>

#include <string>
#include <utf8.hpp>

#include "Wnd.h"
#include "VirtualListView.h"

#include "Contact.h"
#include "OwnerDrawRect.h"


class ChessView : public Wnd{
public:
    ChessView(HWND parent, Contact::ref contact);
    //virtual ~ChessView();
int flagaktiv;
int fokus_x;
int fokus_y;
int fokus_f;
int rocb,rokch;//��������� 1���������,2���� ������ ,3���� ����� ,4���������
POINT pt2;
int Chesspole[9][9];
int Chesspolest[9][9];//������ ����
//[y][x] 1-8

//������ �������� 0-�����  �����  1-�����  2-����  3-����  4-����  5-����� 6-������
//                         ������ 11-����� 12-���� 13-���� 14-���� 15-����� 16-������
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual const ODR * getODR() const;

    typedef boost::shared_ptr<ChatView> ref;

    //virtual bool showWindow(bool show);

protected:
    int width;

    Contact::ref contact;

private:
void ChessView::initpole(void);
void ChessView::paintchess(HDC hdc);
    static ATOM windowClass;
    ATOM RegisterWindowClass();
};