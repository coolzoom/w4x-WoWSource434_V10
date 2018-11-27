#include "ScriptPCH.h"
#pragma execution_character_set("UTF-8")

class vendor_npc : public CreatureScript
{
public:
	vendor_npc() : CreatureScript("vendor_npc") { }

	bool OnGossipHello(Player *player, Creature *creature)
	{
		player->ADD_GOSSIP_ITEM(0, "查询我的充值点数　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->ADD_GOSSIP_ITEM(0, "点数对换虚灵货币　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		/*player->ADD_GOSSIP_ITEM(0,_ToUTF8("[积分点数]->"),GOSSIP_SENDER_MAIN,1);
		player->ADD_GOSSIP_ITEM(0,_ToUTF8("[魔币点数]->"),GOSSIP_SENDER_MAIN,2);
		player->ADD_GOSSIP_ITEM(0,_ToUTF8("查看我的点数"),GOSSIP_SENDER_MAIN,3);*/
		player->ADD_GOSSIP_ITEM(4, "关闭窗口　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		if (!player)
		{
			return false;
		}


		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF + 1://点数对换货币
		{
			if (player->Getjifen() == 0)
			{
				player->GetSession()->SendNotification("|cFFFFFF33你当前帐户没有充值点数|r ");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(0, "[50]点数换[50]货币　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
				player->ADD_GOSSIP_ITEM(0, "[100]点数换[100]货币　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);
				player->ADD_GOSSIP_ITEM(0, "[200]点数换[200]货币　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);
				player->ADD_GOSSIP_ITEM(0, "[500]点数换[500]货币　", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);
				player->ADD_GOSSIP_ITEM(4, "关闭窗口", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 2://查询点数
		{
			//player->GetSession()->SendAreaTriggerMessage("我共有%u点　",player->Getjifen());
			player->GetSession()->SendNotification("|cFFFFFF33你当前帐户积分：|r|cFF33FF00%d|r ", player->Getjifen());
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 999:// Close Window
		{
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 100: // [50 点数] 换 [50 货币]
		{
			if (player->Getjifen() >= 50)
			{
				player->AddItem(38186, 50);
				player->Modifyjifen(-(int32)50);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33点数不足|r　");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 101: // [100 点数] 换 [100 货币]
		{
			if (player->Getjifen() >= 100)
			{
				player->AddItem(38186, 100);
				player->Modifyjifen(-(int32)100);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33点数不足|r　");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 102: // [200 点数] 换 [200 货币]
		{
			if (player->Getjifen() >= 200)
			{
				player->AddItem(38186, 200);
				player->Modifyjifen(-(int32)200);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33点数不足|r　");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 103: // [500 点数] 换 [500 货币]
		{
			if (player->Getjifen() >= 500)
			{
				player->AddItem(38186, 500);
				player->Modifyjifen(-(int32)500);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33点数不足|r　");
			}
			break;
		}
		}
		return true;
	}
};

void AddSC_vendor_npc()
{
	new vendor_npc;
}