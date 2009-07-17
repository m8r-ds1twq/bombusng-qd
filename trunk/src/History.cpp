#include "History.h"
#include "utf8.hpp"
#include "config.h"

#include <stdio.h>
extern std::wstring appRootPath;
History::ref History::getInstance() {
    if (!instance) instance=History::ref(new History());
    return instance;
}

void History::appendHistory( Contact::ref c, Message::ref msg, bool isMuc) {
    if (!Config::getInstance()->history) return;
    std::wstring filePath=historyPath;
	std::wstring filePathavatar=appRootPath;
    std::string dd=strtime::toDateLocal(msg->time);
	if(isMuc){
	  filePath+=L"\\chatrooms\\";
		   if(Config::getInstance()->his_muc_d){filePath+=utf8::utf8_wchar(dd.c_str());
		   filePath+=L"\\";}
	}else{
		filePathavatar+=L"userdata\\avatars\\";

	  filePath+=L"\\";
	  if(Config::getInstance()->his_ch_d){filePath+=utf8::utf8_wchar(dd.c_str());
	  filePath+=L"\\";}
	}
CreateDirectory(filePath.c_str(), NULL);
    //todo: normalize filename
	std::string  fulljid=c->jid.getBareJid();
    filePath+=utf8::utf8_wchar(c->jid.getBareJid());
	filePathavatar+=utf8::utf8_wchar(c->jid.getBareJid());
std::string filePathavatar2=utf8::wchar_utf8(filePathavatar);
	filePath+=Config::getInstance()->saveHistoryHtml?L".html":L".txt";
DWORD attrs=GetFileAttributes(filePath.c_str());
BOOL flagfilehistori=0;
if (attrs==0xFFFFFFFF) {flagfilehistori=1;}
    FILE *f=_wfopen(filePath.c_str(), L"a");
    if (f==NULL) return;


    std::string t=strtime::toLocalDateTime(msg->time);

//[18.02.2009,00:00:00]
//(nick) message

	if(!Config::getInstance()->saveHistoryHtml){
	   fprintf(f, "[%s] %s%s\n", t.c_str(), isMuc?"":msg->fromName.c_str(), msg->getMessageText().c_str());
	}else{
		if(flagfilehistori){fprintf(f,isMuc?"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><title>history BombusQD-NG</title></head><body>":"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><title>history BombusQD</title></head><body><a href=\"%s.jpg\"><img alt=\" %s\" src=\" %s.jpg\"  />%s</a>",isMuc?"":filePathavatar2.c_str(),fulljid.c_str(),filePathavatar2.c_str(),fulljid.c_str());}
		
		
	fprintf(f,
		isMuc?"<font color=\"#9C9C9C\">[%s]%s</font><BR></font><font color=\"#363636\">%s</font><BR></body></html>":
		      "<font color=\"#9C9C9C\">[%s]</font><BR><font color=\"#B22222\">%s:</font><font color=\"#363636\"> %s</font><BR></body></html>", 
		t.c_str(), 
		isMuc?"":msg->fromName.c_str(),
		msg->getMessageText().c_str());
	}
    fclose(f);
}



History::History() {
    historyPath=appRootPath+L"history";
    historyPath2=appRootPath+L"history\\chatrooms";
    CreateDirectory(historyPath.c_str(), NULL);
    CreateDirectory(historyPath2.c_str(), NULL);
 
//historyPath=appRootPath+L"\\chatrooms\\";

    /*DWORD attrs=GetFileAttributes(historyPath.c_str());
    if (attrs==0xFFFFFFFF) {
        writeHistory=false;
    } else {
        writeHistory=(attrs & FILE_ATTRIBUTE_DIRECTORY)!=0;
    }*/
}
History::ref History::instance=History::ref();