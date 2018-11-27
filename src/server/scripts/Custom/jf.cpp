#include "ScriptPCH.h"
#pragma execution_character_set("UTF-8")

class vendor_npc : public CreatureScript
{
public:
	vendor_npc() : CreatureScript("vendor_npc") { }

	bool OnGossipHello(Player *player, Creature *creature)
	{
		player->ADD_GOSSIP_ITEM(0, "��ѯ�ҵĳ�ֵ������", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->ADD_GOSSIP_ITEM(0, "�����Ի�������ҡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		/*player->ADD_GOSSIP_ITEM(0,_ToUTF8("[���ֵ���]->"),GOSSIP_SENDER_MAIN,1);
		player->ADD_GOSSIP_ITEM(0,_ToUTF8("[ħ�ҵ���]->"),GOSSIP_SENDER_MAIN,2);
		player->ADD_GOSSIP_ITEM(0,_ToUTF8("�鿴�ҵĵ���"),GOSSIP_SENDER_MAIN,3);*/
		player->ADD_GOSSIP_ITEM(4, "�رմ��ڡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);

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
		case GOSSIP_ACTION_INFO_DEF + 1://�����Ի�����
		{
			if (player->Getjifen() == 0)
			{
				player->GetSession()->SendNotification("|cFFFFFF33�㵱ǰ�ʻ�û�г�ֵ����|r ");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(0, "[50]������[50]���ҡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
				player->ADD_GOSSIP_ITEM(0, "[100]������[100]���ҡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);
				player->ADD_GOSSIP_ITEM(0, "[200]������[200]���ҡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);
				player->ADD_GOSSIP_ITEM(0, "[500]������[500]���ҡ�", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);
				player->ADD_GOSSIP_ITEM(4, "�رմ���", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 2://��ѯ����
		{
			//player->GetSession()->SendAreaTriggerMessage("�ҹ���%u�㡡",player->Getjifen());
			player->GetSession()->SendNotification("|cFFFFFF33�㵱ǰ�ʻ����֣�|r|cFF33FF00%d|r ", player->Getjifen());
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 999:// Close Window
		{
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 100: // [50 ����] �� [50 ����]
		{
			if (player->Getjifen() >= 50)
			{
				player->AddItem(38186, 50);
				player->Modifyjifen(-(int32)50);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33��������|r��");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 101: // [100 ����] �� [100 ����]
		{
			if (player->Getjifen() >= 100)
			{
				player->AddItem(38186, 100);
				player->Modifyjifen(-(int32)100);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33��������|r��");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 102: // [200 ����] �� [200 ����]
		{
			if (player->Getjifen() >= 200)
			{
				player->AddItem(38186, 200);
				player->Modifyjifen(-(int32)200);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33��������|r��");
			}
			break;
		}
		case GOSSIP_ACTION_INFO_DEF + 103: // [500 ����] �� [500 ����]
		{
			if (player->Getjifen() >= 500)
			{
				player->AddItem(38186, 500);
				player->Modifyjifen(-(int32)500);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cFFFFFF33��������|r��");
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