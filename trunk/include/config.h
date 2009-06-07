#pragma once

#include <string>
#include "boostheaders.h"
#include "Serialize.h"

class Config
{
public:
    ~Config(){};

    typedef boost::shared_ptr<Config> ref;

    static Config::ref getInstance();

    bool showOfflines;
    bool showGroups;
    bool sortByStatus;

    bool composing;
    bool delivered;
    bool history;
	bool blink;
	bool blink2;
    bool vibra;
    bool sounds;
	bool sounds_status;
	bool vs_status;
	bool vsmess;
	bool vstrymess;
	bool scomposing;
    bool raiseSIP;
	bool editx2;
    bool connectOnStartup;
	bool confchat;
	bool confclient;
	int avatarWidth;
	int tabconf;
	
	int reconnectTries;
	int msg_font_width;
	int msg_font_height;
	int roster_font_width;
	int roster_font_height;
	int tolshina;
/*
"*"-� ����������
"+"-�����������
------------------------------Build9-------------------------------------
+��������� ��������� ������/������ ������������ ������(��������� �� �������� Tahoma)


------------------------------Build8-------------------------------------
+Jabber:
+����������� ����� ������� � ������ ����� Entity Caps
*����������� � ��������� ������� �����/��� �������� ������� �����
*������ � ����������:
*���������� � ��������
*�������:
*���-����.����� ���� � ������� ��������
*����������� �/��� �������� �������

GUI:
���� ����:
+���������� ���� ����� ��������� � 2 ����
+������ �������� � skin.png(QVGA)
*������ �������� ��� VGA
+������� ���������� ����� �������� ���������
+��������������������� ��������� �����:
*����������� �����������
*���������� �������� ���� ���� ��� ������ ����������:
|           | +Jid   +������ �������� �������
| *�������  | *������ �������
|           | *��������� ��������� ��� ������������� ������� 
*���������������� ��������� �������(+�������� ����� ��������� �����):
*����� ���� ������
*������ ������
*����� ������


------------------------------Build5-------------------------------------
GUI:
+��������� ����� ��������� � ����������� �� �������
+��������� �����:
+Show conference Statuses(����� �������� � �����������)
+Show statuses in simple chat
�������:
+��������� ����� Save History for Chatrooms(������� ������� ��� �����������)
+������� ��� ����������� ������� � ��������� ����� \\history\\chatrooms\\..
+������� ��� �������-����� ������� � ����� \\history\\..
+������� ��������� ��������� � ����������� �� �����������
+��������� ����� Save History in HTML(���������� ������� � ������ html,
�.�. �������� ������������ ���� - font,br)





------------------------------Build1-------------------------------------
pathes from extraterrestrial:
+ ������ ���������� � ��������
+ �����/��������� � ����������� �������� � ���� �������
+ ��������� ��������� ����������� ��� ����� 
+ �������� VGA|QVGA ( ��. ���� Bombus �� 4pda.ru/forum ��� ��������� � �.�. ��� ���) 
+ ����� ������ ���� VGA,qVGA 
+ �������������� ���� ��������, ������� ������
+ ������������ ����� � ���������� ���������� ������/�����
+ ��������� ���� ��������� � ������
-------------------------------------------------------------------------
*/

	bool showMucPresences;
	bool showStatusInSimpleChat;
	bool saveHistoryMuc;
	bool saveHistoryHtml;

    void save();
private:
    void serialize(Serialize &s);
    static Config::ref instance;

	Config();
};
