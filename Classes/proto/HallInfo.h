﻿
#ifndef __HallInfo__
#define __HallInfo__

#include "AppMacros.h"
#include "ccEvent.h"


class HallInfo:public Object
{
public:
	HallInfo();
	~HallInfo();
	static HallInfo* getIns();
	bool init();
public:
	void SendCRank(int type,int index);
	void HandlerSRankHand(ccEvent *event);

	void SendCShop(int type);
	void HandlerSShop(ccEvent *event);

	void SendCMail();
	void HandlerSMail(ccEvent *event);

	void SendCFriend();
	void HandlerSFriend(ccEvent *event);

	void SendCFindFriend(string uid,int type);
	void HandlerSFindFriend(ccEvent *event);

	void SendCGiveFriend(string uid);
	void HandlerSGiveFriend(ccEvent *event);

	void SendCAddFriend(string uid);
	void HandlerSAddFriend(ccEvent *event);

	void SendCAddFriendList();
	void HandlerSAddFriendList(ccEvent *event);

	void SendCActive(int type);
	void HandlerSActive(ccEvent *event);


	map<int, Rank> getSRank(int type);
	void eraseRank(int type,int lv);

	SShop getSShop(int type);
	SMail getSMail(){
		return m_pSMail;
	}
	SFindFriend getSFindFriend(){
		return m_pSFindFriend;
	}
	SFriend getSFriend(){
		return m_pSFriend;
	}
private:
	static HallInfo *m_shareHallInfo;
	map<int, map<int,Rank>> m_pSRanks;
	map<int, SShop>m_pSShops;
	SActive m_pSActive;
	SMail m_pSMail;
	SFindFriend m_pSFindFriend;
	SFriend m_pSFriend;
	
};

#endif 