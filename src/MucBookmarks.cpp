#include "MucBookmarks.h"
#include "JabberStream.h"
#include "TimeFunc.h"
#include "Log.h"
#include "ProcessMUC.h"
#include "config.h"
ProcessResult MucBookmarks::blockArrived( JabberDataBlockRef block, const ResourceContextRef rc ) {
    std::string &type=block->getAttribute("type");
    if (type=="error") {
        return LAST_BLOCK_PROCESSED;
    }

    bookmarksAvailable=true;
    if (type=="result") {
        JabberDataBlockRef pvt=block->findChildNamespace("query","jabber:iq:private");
        if (!pvt) return LAST_BLOCK_PROCESSED;
        JabberDataBlockRef storage=pvt->findChildNamespace("storage","storage:bookmarks");

        JabberDataBlockRefList::iterator i=storage->getChilds()->begin();
        while (i!=storage->getChilds()->end()) {
            JabberDataBlockRef item=*(i++);
            const std::string &tagName=item->getTagName();

            MucBookmarkItem::ref b=addNewBookmark();
            b->name=item->getAttribute("name");

            if (tagName=="url") {
                b->url=item->getAttribute("url");
                if (b->name.empty()) b->name=b->url;
            }

            if (tagName=="conference") {
                b->jid=item->getAttribute("jid");
                b->nick=item->getChildText("nick");
                b->password=item->getChildText("password");
                const std::string &autoJoin=item->getAttribute("autojoin");
				if(Config::getInstance()->autojoinroom){
				if(autoJoin=="1" || autoJoin=="true"){
				 Jid roomNode;
            
            roomNode.setServer(item->getAttribute("jid"));
            roomNode.setResource(item->getChildText("nick"));
ProcessMuc::initMuc(roomNode.getJid(), item->getChildText("password"), rc);
JabberDataBlockRef joinPresence=constructPresence(
                roomNode.getJid().c_str(), 
                rc->status, 
               rc->presenceMessage, 
               rc->priority); 
			JabberDataBlockRef xMuc=joinPresence->addnod("c", "http://bombusng-qd.googlecode.com");
           xMuc=joinPresence->addChildNS("x", "http://jabber.org/protocol/muc");
            if (b->password.c_str()) xMuc->addChild("password",b->password.c_str());

           
                rc->jabberStream->sendStanza(joinPresence);
				}}
                b->autoJoin=(autoJoin=="1" || autoJoin=="true"); 
            }
        }
        std::stable_sort(bookmarks.begin(), bookmarks.end(), MucBookmarkItem::compare);
    }
    Log::getInstance()->msg("Bookmarks received successfully");
    return LAST_BLOCK_PROCESSED;
}

MucBookmarkItem::ref MucBookmarks::addNewBookmark() {
    bookmarks.push_back(MucBookmarkItem::ref(new MucBookmarkItem()));
    return bookmarks.back();
}
void MucBookmarks::doQueryBookmarks( ResourceContextRef rc ) {
    bookmarksAvailable=false;
    id=strtime::getRandom();
    JabberDataBlock getBm("iq");
    getBm.setAttribute("type", "get");
    getBm.setAttribute("id", id.c_str());
    getBm.addChildNS("query","jabber:iq:private")->
          addChildNS("storage","storage:bookmarks");
    rc->jabberStanzaDispatcherRT->addListener(rc->bookmarks);
    rc->jabberStream->sendStanza(getBm);
}
MucBookmarkItem::ref MucBookmarks::get( int i ) { return bookmarks[i]; }
int MucBookmarks::getBookmarkCount() const { return bookmarks.size(); }

bool MucBookmarkItem::compare( MucBookmarkItem::ref l, MucBookmarkItem::ref r ) {
    return (l->name.compare(r->name) < 0);

}