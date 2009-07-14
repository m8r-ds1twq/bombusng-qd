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
	bool xmllog;
	int avatarWidth;
	int tabconf;
	std::string  avtomessage;
	int reconnectTries;
	int msg_font_width;
	int msg_font_height;
	int roster_font_width;
	int roster_font_height;
	int tolshina;
	int ping_aliv;
	int pong_aliv;
	bool avtostatus;
	bool autojoinroom;
	bool tune_status;
	int  time_avtostatus;
	bool tune_status_pep;
		bool his_ch_d;
		bool his_muc_d;
	//int id_avtostatus;


/*
"*"-в разработке
"+"-реализовано
------------------------------Build9-------------------------------------
+Добавлены настройки высоты/ширины графического шрифта(дефолтный по прежнему Tahoma)


------------------------------Build8-------------------------------------
+Jabber:
+Определение имени клиента и версии через Entity Caps
*Подключение с загрузкой Контакт Листа/без загрузки Контакт Листа
*Работа с закладками:
*Сохранение и удаление
*Аккаунт:
*Кип-алив.Выбор типа и периода отправки
*Подключение с/без загрузки ростера

GUI:
Окно чата:
+Уменьшение поля ввода сообщения в 2 раза
+Иконки клиентов в skin.png(QVGA)
*Иконки клиентов для VGA
+Скрытие клавиатуры после отправки сообщения
+Пальцеориентированный интерфейс табов:
*Реализовать Опционально
*Расширение верхнего инфо бара для показа информации:
|           | +Jid   +Иконки быстрого доступа
| *Аватара  | *Иконка клиента
|           | *Статусное сообщение или идентификатор статуса 
*Пользовательская настройка шрифтов(+Добавлен новый дефолтный шрифт):
*Выбор типа шрифта
*Размер шрифта
*Стиль шрифта


------------------------------Build5-------------------------------------
GUI:
+Различные цвета контактов в зависимости от статуса
+Статусные опции:
+Show conference Statuses(показ статусов в конференции)
+Show statuses in simple chat
История:
+Добавлена опция Save History for Chatrooms(Хранить историю для конференций)
+История для конференции пишется в отдельную папку \\history\\chatrooms\\..
+История для контакт-листа пишется в папку \\history\\..
+История приватной переписки в конференции не сохраняется
+Добавлена опция Save History in HTML(Сохранение истории в псевдо html,
т.е. основные используемые теги - font,br)





------------------------------Build1-------------------------------------
pathes from extraterrestrial:
+ иконка информации в диалогах
+ выход/перезаход в конференции доступно в меню комнаты
+ прокрутка сообщений конференции при входе 
+ смайлики VGA|QVGA ( см. тему Bombus на 4pda.ru/forum все копирайты и т.д. все там) 
+ новые иконки меню VGA,qVGA 
+ дополнительное меню статусов, быстрый статус
+ переключение табов в настройках джойстиком вправо/влево
+ ИСПРАВЛЕН глюк джойстика с табами
-------------------------------------------------------------------------
*/

	bool showMucPresences;
	bool showStatusInSimpleChat;
	bool saveHistoryMuc;
	bool saveHistoryHtml;
	int id_avtostatus;
    void save();
private:
    void serialize(Serialize &s);
    static Config::ref instance;

	Config();
};
