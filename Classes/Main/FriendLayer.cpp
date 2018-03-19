﻿#include "FriendLayer.h"
#include "GameControl.h"
#include "GameDataSet.h"
#include "ClientSocket.h"
#include "LogoScene.h"
#include "LoginInfo.h"
#include "HallInfo.h"



FriendChatItemLayer::FriendChatItemLayer(){

}

FriendChatItemLayer::~FriendChatItemLayer(){

}

FriendChatItemLayer *FriendChatItemLayer::create(Friend hall){
	FriendChatItemLayer *p = new FriendChatItemLayer();
	if (p&&p->init(hall)){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

bool FriendChatItemLayer::init(Friend hall)
{
	if (!Layer::init())
	{
		return false;
	}
	m_hall = hall;
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("friendchatitem.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());

	SEL_TouchEvent selector = toucheventselector(FriendChatItemLayer::TouchEvent);
	GameDataSet::getButton(m_RootLayer, "chat", selector, this);
	GameDataSet::getButton(m_RootLayer, "zeng", selector, this);

	bool online = hall.online();
	string username = hall.userinfo().username();
	long time = hall.time();
	char buff[100];
	sprintf(buff,"[%s]%s",online?XXIconv::GBK2UTF("在线").c_str():XXIconv::GBK2UTF("离线").c_str(),username.c_str());
	GameDataSet::setText(m_RootLayer, "name", buff);
	if (time < 60){
		sprintf(buff,XXIconv::GBK2UTF("上次在线%d秒前").c_str(),time);
	}
	else if (time < 60 * 60){
		sprintf(buff, XXIconv::GBK2UTF("上次在线%d分前").c_str(), time / 60);
	}
	else if (time<60*60*24){
		sprintf(buff, XXIconv::GBK2UTF("上次在线%d小时前").c_str(), time / (60 * 60));
	}
	else{
		sprintf(buff, XXIconv::GBK2UTF("上次在线%d天前").c_str(), time / (60 * 60*24));
	}
	GameDataSet::setText(m_RootLayer, "time", buff);
	
	return true;
}

void FriendChatItemLayer::TouchEvent(CCObject *obj, TouchEventType type){
	Button *btn = (Button *)obj;
	string name = btn->getName();
	if (type == TOUCH_EVENT_ENDED){
		if (name.compare("chat") == 0){
			log("chat");
		}
		else if (name.compare("zeng") == 0){
			log("zeng");
		}
	}
}







FriendNoticeLayer::FriendNoticeLayer(){

}

FriendNoticeLayer::~FriendNoticeLayer(){

}

FriendNoticeLayer *FriendNoticeLayer::create(Rank hall){
	FriendNoticeLayer *p = new FriendNoticeLayer();
	if (p&&p->init(hall)){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

bool FriendNoticeLayer::init(Rank hall)
{
	if (!Layer::init())
	{
		return false;
	}
	m_hall = hall;
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("friendtongzhi.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());



	return true;
}

void FriendNoticeLayer::TouchEvent(){

}



















FriendLayer::FriendLayer(){
	memset(m_sbg,NULL,sizeof(Layout *));
	GameControl::getIns()->setFriendLayer(this);
}

FriendLayer::~FriendLayer(){
	if (this == GameControl::getIns()->getFriendLayer()){
		GameControl::getIns()->setFriendLayer(NULL);
	}
}

FriendLayer *FriendLayer::create(){
	FriendLayer *p = new FriendLayer();
	if (p&&p->init()){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

bool FriendLayer::init()
{             
	if (!Layer::init())
    {
        return false;
    }
	
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("friend.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());
	SEL_TouchEvent selector = toucheventselector(FriendLayer::TouchEvent);
	GameDataSet::getButton(m_RootLayer, "close_btn", selector, this);
	
	m_btns[0] = GameDataSet::getButton(m_RootLayer, "find", selector, this);
	m_btns[1] = GameDataSet::getButton(m_RootLayer, "friend", selector, this);
	m_btns[2] = GameDataSet::getButton(m_RootLayer, "tongzhi", selector, this);
	GameDataSet::getButton(m_RootLayer, "search", selector, this);
	GameDataSet::getButton(m_RootLayer, "huan", selector, this);

	m_btntext[0] = (TextBMFont *)GameDataSet::getLayout(m_RootLayer, "BitmapLabel_find");
	m_btntext[1] = (TextBMFont *)GameDataSet::getLayout(m_RootLayer, "BitmapLabel_friend");
	m_btntext[2] = (TextBMFont *)GameDataSet::getLayout(m_RootLayer, "BitmapLabel_tongzhi");

	m_bg[0] = GameDataSet::getLayout(m_RootLayer,"findbg");
	m_bg[1] = GameDataSet::getLayout(m_RootLayer, "friendbg");
	m_bg[2] = GameDataSet::getLayout(m_RootLayer, "tongzhibg");
	SelectItem(0);

	DBUserInfo user = LoginInfo::getIns()->getMyDBUserInfo();
	string uid = user.userid();
	GameDataSet::setTextBMFont(m_RootLayer, "BitmapLabel_id", uid);

	char buff[30];
	for (int i = 0; i < 2; i++){
		sprintf(buff,"ScrollView%d",i+1);
		m_ScrollView[i] = (ui::ScrollView *)GameDataSet::getButton(m_RootLayer, buff, selector, this);
		sprintf(buff, "sbg%d", i + 1);
		m_sbg[i] = GameDataSet::getLayout(m_RootLayer, buff);
	}
	for (int i = 0; i < 4; i++){
		sprintf(buff, "smallbg%d", i + 1);
		Layout *ly = GameDataSet::getLayout(m_RootLayer, buff);
		GameDataSet::getButton(ly, "btn", selector, this);
	}

	Layout *in = GameDataSet::getLayout(m_RootLayer,"in");
	m_input = LogoLayer::AddCursorTextField(in,20);
	m_input->setPlaceHolder(XXIconv::GBK2UTF("请输入对方id号").c_str());
	m_input->setFontColor(ccc3(0x38,0x4E,0x9C));
	m_input->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
	HallInfo::getIns()->SendCFindFriend("",2);
    return true;
}

void FriendLayer::TouchEvent(CCObject *obj, TouchEventType type){
	Button *btn = (Button *)obj;
	string name = btn->getName();
	if (type == TOUCH_EVENT_ENDED){
		if (name.compare("close_btn") == 0){
			this->removeFromParentAndCleanup(true);
		}
		else if (name.compare("find") == 0){
			SelectItem(0);
		}
		else if (name.compare("friend") == 0){
			SelectItem(1);
		}
		else if (name.compare("tongzhi") == 0){
			SelectItem(2);
		}
		else if (name.compare("search") == 0){
			string id = m_input->getText();
			if (!id.empty()){
				HallInfo::getIns()->SendCFindFriend(id, 0);
			}
			else{
				log("%s",XXIconv::GBK2UTF("您输入的id空").c_str());
			}
		}
		else if (name.compare("huan") == 0){
			HallInfo::getIns()->SendCFindFriend("", 2);
		}
		else if (name.compare("btn")==0){
			string pname = btn->getParent()->getName();
			log("%s", pname.c_str());
			if (pname.compare("smallbg1") == 0){
				
			}
			else if (pname.compare("smallbg2") == 0){

			}
			else if (pname.compare("smallbg3") == 0){

			}
			else if (pname.compare("smallbg4") == 0){

			}
		}
	}
}


void FriendLayer::SelectItem(int index){
	for (int i = 0; i < 3; i++){
		m_bg[i]->setVisible(false);
		if (m_btns[i]){
			m_btns[i]->setBright(true);
		}
		if (m_btntext[i]){
			m_btntext[i]->setFntFile("fonts/xiaodan10-1.fnt");
		}
	}
	if (m_btns[index]){
		m_btns[index]->setBright(false);
	}
	if (m_btntext[index]){
		m_btntext[index]->setFntFile("fonts/xiaodan10.fnt");
	}
	m_bg[index]->setVisible(true);
	int count = 0;
	if (m_sbg[index]){
		count=m_sbg[index]->getChildrenCount();
	}
	if (index==1&&count == 0){
		HallInfo::getIns()->SendCFriend();
	}
	else if (index == 2 && count == 0){
		HallInfo::getIns()->SendCAddFriendList();
	}
	else{
		ShowFriendEvent(index);
	}
	//ShowFriendEvent(index);
}

void FriendLayer::ShowFriendEvent(int index){
	if (index > 0){
		ui::ScrollView *scroll = m_ScrollView[index-1];
		Layout *sbg = m_sbg[index-1];
		if (sbg->getChildrenCount() == 0){
			if (index == 1){
				for (int i = 0; i < 10; i++){
					Friend hall;
					FriendChatItemLayer *p = FriendChatItemLayer::create(hall);
					GameDataSet::PushScrollItem(sbg, 2, 0, p, i, scroll);
				}
			}
			else{
				for (int i = 0; i < 10; i++){
					Rank hall;
					FriendNoticeLayer *p = FriendNoticeLayer::create(hall);
					GameDataSet::PushScrollItem(sbg, 0, 0, p, i, scroll);
				}
			}
		}
	}
	else{
		char buff[100];
		SFindFriend fris = HallInfo::getIns()->getSFindFriend();
		int sz = 3;
		for (int i = 0; i < 4;i++){
			sprintf(buff,"smallbg%d",i+1);
			Layout *ly = GameDataSet::getLayout(m_RootLayer,buff);
			if (i < sz){
				Friend fri;
				fri.set_acttype(i%3+1);
				DBUserInfo user = fri.userinfo();
				sprintf(buff,"10000%d",i);
				user.set_username(buff);
				string name = user.username();
				bool online = fri.online();
				int act = fri.acttype();
				GameDataSet::setText(ly, "name", name);
				sprintf(buff,"friend/HY_LY%d.png",act);
				GameDataSet::setImageView(ly, "tt", buff, cocos2d::ui::Widget::TextureResType::PLIST);
			}
			else{
				ly->setVisible(false);
			}
		}
	}
}