#pragma once

#include <windows.h>
#include "boostheaders.h"
#include <string>
#include <vector>

class Image {
public:
    Image(LPCTSTR path);
    virtual ~Image();

    virtual void drawImage(HDC hdc, int x, int y) const;
    HBITMAP getHBmp() {return bmp; }

protected:
    HBITMAP bmp;
    //HBITMAP mask;
    COLORREF transparentColor;
    Image();
private:
};
typedef boost::shared_ptr<Image> ImageRef;

//////////////////////////////////////////////////////////////////////////
class ImgList {
public:
    virtual ~ImgList(){};
    virtual void drawElement (HDC hdc, int index, int x, int y) const =0;
    virtual int getElementWidth() const =0;
    virtual int getElementHeight() const =0;
};
typedef boost::shared_ptr<ImgList> ImgListRef;


class ImgArray : public ImgList{
public:
    ImgArray(LPCTSTR path, int nColumns, int nRows);
    virtual ~ImgArray();
    void setGridSize(int nColumns, int nRows);

    virtual void drawElement (HDC hdc, int index, int x, int y) const;
    virtual int getElementWidth() const { return elWidth; }
    virtual int getElementHeight() const { return elHeight; }
private:
    HBITMAP bmp;
    COLORREF transparentColor;

    int nColumns;
    int nRows;
    int elWidth;
    int elHeight;
};

//////////////////////////////////////////////////////////////////////////
namespace icons {
enum IconDef {
    ICON_INVISIBLE_INDEX = 0x10,
    ICON_ERROR_INDEX = 0x11,
    ICON_TRASHCAN_INDEX = 0x12,
    ICON_PROGRESS_INDEX = 0x13,
    ICON_PRIVACY_ACTIVE = 0x46,
    ICON_PRIVACY_PASSIVE = 0x47,

    ICON_SEARCH_INDEX = 0x14,

    ICON_REGISTER_INDEX = 0x15,

    ICON_MSGCOLLAPSED_INDEX = 0x16,

    ICON_MESSAGE_INDEX = 0x20,

    ICON_AUTHRQ_INDEX = 0x21,

    ICON_COMPOSING_INDEX = 0x22,
    ICON_AD_HOC=0x22,

    ICON_EXPANDED_INDEX = 0x23,

    ICON_COLLAPSED_INDEX = 0x24,

    ICON_VCARD=0x25,
    ICON_DISCO_BACK=0x26,

    ICON_DELIVERED_INDEX=0x27,
    //ICON_MESSAGE_BUTTONS = 0x25,

    ICON_PROFILE_INDEX = 0x30,

    ICON_CLOSE = 0x35,

    ICON_PRIVACY_ALLOW = 0x36,

    ICON_PRIVACY_BLOCK = 0x37,

    ICON_KEYBLOCK_INDEX = 0x17,

    ICON_MODERATOR_INDEX = 0x50,
    ICON_GROUPCHAT_INDEX = 0x40,
    ICON_GCJOIN_INDEX = 0x41,

    ICON_BOMBUSMOD=0x60,//BombusMod
	ICON_BOMBUS=0x61,//Bombus
	ICON_BOMBUS_QD=0x74,
	ICON_BOMBUS_QD_NG=0x56,
	ICON_BOMBUS_NG=0x62,//BombusNG
	ICON_PSI=0x63,//Psi
	ICON_MIRANDA=0x64,//Miranda
	ICON_BOMBUS_PLUS=0x65, //Bombus+
	ICON_GAJIM = 0x70, //GAJIM
	ICON_QIP=0x71,
	ICON_SJC=0x75,
	ICON_TKAB=0x57,
	ICON_PIDGIN=0x72,
	ICON_KOPET=0x73,
};

}

class Skin : public ImgList {
public:
    Skin(LPCTSTR path);
    virtual ~Skin();
    virtual void drawElement (HDC hdc, int index, int x, int y) const;
    virtual int getElementWidth() const;
    virtual int getElementHeight() const;
    virtual int getBaseIndex(const std::string &setName);
	virtual int Skin::getKlientIndex(char* caps);
	virtual char** Skin::readFileK(const wchar_t *fileName,int *count);
	
protected:
    std::vector<std::string> names;
    std::vector<ImgListRef> iconset;
};