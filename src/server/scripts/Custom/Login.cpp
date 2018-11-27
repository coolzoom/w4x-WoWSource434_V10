


#include "ScriptPCH.h"
#pragma execution_character_set("UTF-8")


class announce_login : public PlayerScript
{
public : announce_login() : PlayerScript("announce_login") { }

		 void OnLogin(Player* player)
	{
		if (player->GetTeam() == ALLIANCE)
		{
			std::ostringstream ss;
			ss << "|cff0060ff[����]|cffFFD800����ʿ|cff3DAEFF " << player->GetName() << " |cffFFD800�����ˣ�";
			sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
		}
		else
		{
			std::ostringstream ss;
			ss << "|cffFF0000[����]|cffFFD800����ʿ|cff3DAEFF " << player->GetName() << " |cffFFD800�����ˣ�";
			sWorld->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
		}
	}
};

void AddSC_announce_login()
{
	new announce_login;     //��������
}