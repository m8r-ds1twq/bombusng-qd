#include "History.h"
#include "utf8.hpp"
#include "config.h"

#include <stdio.h>

History::ref History::getInstance() {
    if (!instance) instance=History::ref(new History());
    return instance;
}

void History::appendHistory( Contact::ref c, Message::ref msg, bool isMuc) {
    if (!Config::getInstance()->history) return;
    std::wstring filePath=historyPath;
	if(isMuc){
	  filePath+=L"\\chatrooms\\";
	}else{
	  filePath+='\\';
	}
    //todo: normalize filename
    filePath+=utf8::utf8_wchar(c->jid.getBareJid());
	filePath+=Config::getInstance()->saveHistoryHtml?L".html":L".txt";

    FILE *f=_wfopen(filePath.c_str(), L"a");
    if (f==NULL) return;


    std::string t=strtime::toLocalDateTime(msg->time);
//[18.02.2009,00:00:00]
//(nick) message
	if(!Config::getInstance()->saveHistoryHtml){
	   fprintf(f, "[%s] %s%s\n", t.c_str(), isMuc?"":msg->fromName.c_str(), msg->getMessageText().c_str());
	}else{
	   fprintf(f,
		isMuc?"<font color=\"#9C9C9C\">[%s]%s</font><BR></font><font color=\"#363636\">%s</font><BR>":
		      "<font color=\"#9C9C9C\">[%s]</font><BR><font color=\"#B22222\">%s:</font><font color=\"#363636\"> %s</font><BR>", 
		t.c_str(), 
		isMuc?"":msg->fromName.c_str(),
		msg->getMessageText().c_str());
	}
    fclose(f);
}

extern std::wstring appRootPath;

History::History() {
    historyPath=appRootPath+L"history";
    CreateDirectory(historyPath.c_str(), NULL);

    /*DWORD attrs=GetFileAttributes(historyPath.c_str());
    if (attrs==0xFFFFFFFF) {
        writeHistory=false;
    } else {
        writeHistory=(attrs & FILE_ATTRIBUTE_DIRECTORY)!=0;
    }*/
}
History::ref History::instance=History::ref();