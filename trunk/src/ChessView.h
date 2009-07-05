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

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual const ODR * getODR() const;

    typedef boost::shared_ptr<ChatView> ref;

    //virtual bool showWindow(bool show);

protected:
    int width;

    Contact::ref contact;

private:
    static ATOM windowClass;
    ATOM RegisterWindowClass();
};