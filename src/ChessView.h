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
int rocb,rokch;//рокировка 1разрешена,2тока вправо ,3тока влево ,4запретить
POINT pt2;
int Chesspole[9][9];
int Chesspolest[9][9];//старое поле
//[y][x] 1-8

//фигура квадрата 0-пусто  белые  1-пешка  2-тура  3-конь  4-слон  5-ферзь 6-король
//                         чёрные 11-пешка 12-тура 13-конь 14-слон 15-ферзь 16-король
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