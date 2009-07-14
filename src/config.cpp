#include "config.h"

#define CONFIG_FILE L"config\\preferences.bin"

Config::ref Config::instance=Config::ref();

Config::ref Config::getInstance() {
	if (!instance) {
        instance=Config::ref(new Config());
	}
	return instance;
}

Config::Config(void){
    Serialize s(CONFIG_FILE, Serialize::READ);
    serialize(s);
}

extern std::wstring appRootPath;
void Config::save() {
    std::wstring configPath=appRootPath+L"config";
    CreateDirectory(configPath.c_str(), NULL);

    Serialize s(CONFIG_FILE, Serialize::WRITE);
    serialize(s);
}


void Config::serialize( Serialize &s ) {

    //////////////////////////////////////////////////////////////////////////
    // Serialization
    int version=1;
    s.streamInt(version, 1);
    
    s.streamBool(showOfflines, true);
	s.streamBool(editx2, true);
    //message events
    s.streamBool(composing, true);
    s.streamBool(delivered, true);

    //local history
    s.streamBool(history, true);

    //signalling

	s.streamBool(blink, false);
	s.streamBool(blink2, false);
    s.streamBool(vibra, true);
    s.streamBool(sounds, true);
	s.streamBool(sounds_status, true);
    s.streamBool(vs_status, false);
	s.streamBool(scomposing, true);

    s.streamBool(showGroups, true);
    s.streamBool(sortByStatus, true);
    s.streamBool(vsmess, false);
	s.streamBool(vstrymess, false);
    //SIP control
    s.streamBool(raiseSIP, false);
	std::string tempav="Autostatus: ";
	s.streamString(avtomessage,tempav.c_str());
    

    //Automatic connection
    s.streamBool(connectOnStartup, false);



    //Presences
	s.streamBool(showMucPresences, true);
	s.streamBool(showStatusInSimpleChat, true);
	//History
	s.streamBool(saveHistoryMuc, false);
	s.streamBool(saveHistoryHtml, false);
	s.streamBool(confchat, true);
	s.streamBool(confclient, true);
	
	s.streamInt(avatarWidth,50);
	s.streamInt(tabconf,12);
	
	s.streamInt(reconnectTries,3);
	s.streamInt(tolshina,400);
	s.streamInt(msg_font_height,14);
	s.streamInt(msg_font_width,5);

	s.streamInt(roster_font_height,16);
	s.streamInt(roster_font_width,6);

	s.streamInt(time_avtostatus,300);
	s.streamInt(ping_aliv,150);
	s.streamInt(pong_aliv,90);
	s.streamInt(id_avtostatus,3);
	s.streamBool(autojoinroom,true);
	s.streamBool(avtostatus, true);
	s.streamBool(tune_status, true);
	s.streamBool(his_muc_d, true);
	s.streamBool(his_ch_d, true);
	s.streamBool(tune_status_pep, true);
    s.streamBool(xmllog, false);

}

