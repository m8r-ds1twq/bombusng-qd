#include "IconTextElement.h"
#include <windows.h> 
#include <Wingdi.h> 
#include "config.h"
extern ImgListRef skin;


//////////////////////////////////////////////////////////////////////////
// WARNING!!! ONLY FOR WM2003 and higher
//////////////////////////////////////////////////////////////////////////
#ifndef DT_END_ELLIPSIS
#define DT_END_ELLIPSIS 0x00008000
#endif
//////////////////////////////////////////////////////////////////////////

LOGFONTW FONT_ICON_TXT; //����� �������

void IconTextElement::init() {
    HDC tmp=CreateCompatibleDC(NULL);
    RECT r={0,0,10,10};
    DrawText(tmp, getText(), -1, &r, DT_CALCRECT | DT_LEFT | DT_TOP);
    width=r.right+skin->getElementWidth()+ICON_SPACING;
	height=r.bottom;  
    int himg=skin->getElementWidth();
    if (height<himg) height=himg;
    DeleteDC(tmp);
}

void IconTextElement::draw(HDC hdc, RECT &rt) const {
    //SetBkMode(hdc, TRANSPARENT);
    int iconIdx=getIconIndex();

	
    if (iconIdx>=0) {
        skin->drawElement(hdc, getIconIndex(), rt.left, rt.top);
		rt.left+=skin->getElementWidth()+ICON_SPACING;
		if(clientIcon>0){//��������� ������ �������
			//skin->drawElement(hdc, clientIcon, (rt.left+skin->getElementWidth()), rt.top);
			//rt.left+=skin->getElementWidth()+ICON_SPACING;
			skin->drawElement(hdc, clientIcon, (rt.right-skin->getElementWidth()), rt.top);
			rt.right-=skin->getElementWidth()-ICON_SPACING;
		}
	} else rt.left+=1;

	strcpy((char*)FONT_ICON_TXT.lfFaceName, "Tahoma"); 
	FONT_ICON_TXT.lfHeight = Config::getInstance()->roster_font_height; 
	FONT_ICON_TXT.lfWidth = Config::getInstance()->roster_font_width;
	FONT_ICON_TXT.lfWeight = Config::getInstance()->tolshina;
	FONT_ICON_TXT.lfItalic = false; 
	FONT_ICON_TXT.lfStrikeOut = false; 
	FONT_ICON_TXT.lfUnderline = false; 
	FONT_ICON_TXT.lfOrientation = 0; 
	FONT_ICON_TXT.lfEscapement = 0; 
	HFONT NormalFont  = CreateFontIndirectW(&FONT_ICON_TXT); 
	SelectObject(hdc, NormalFont); 
    DrawText(hdc, getText(), -1, &rt, DT_LEFT | DT_TOP | DT_END_ELLIPSIS);
	DeleteObject(NormalFont);
}

int IconTextElement::getWidth() const { return width;}
int IconTextElement::getHeight() const { return  Config::getInstance()->roster_font_height; }

void IconTextElement::createContextMenu( HMENU ) const { return; }

void IconTextElement::onCommand( int cmdId, ResourceContextRef rc ) { return; }

IconTextElementContainer::IconTextElementContainer(const std::wstring &wstr, int icon ) {
    this->iconIndex=icon;
    this->wstr=wstr;
    init();
}
int IconTextElementContainer::getIconIndex() const { return iconIndex; }

const wchar_t * IconTextElementContainer::getText() const { return wstr.c_str(); }

int IconTextElementContainer::getColor() const { return 0; }