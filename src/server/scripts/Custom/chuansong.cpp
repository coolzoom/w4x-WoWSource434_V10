/*
Teleporter Script for Trinity
Coded By Lhorath
Coding Inspired From Rochet2 Teleporter
*/

//     Link Types//
/*
{#,    #,    2,    "|cff4169E1 Non-Icon Links|r",    0,    0,    0,    0,    0,    0,    0,    0},
{#,    #,    2,    "|cff4169E1|TInterface\\icons\\Achievement_Leader_King_Varian_Wrynn:30|t Icon & Color Links|r",    0,    0,    0,    0,    0,    0,    0,    0},
{#,    #,    2,    "|TInterface\\icons\\Achievement_Leader_King_Varian_Wrynn:30|t Icon Links (NoColor)|t",    0,    0,    0,    0,    0,    0,    0,    0},
{#,    #,    2,    "Non-Formated Links Links",    0,    0,    0,    0,    0,    0,    0,    0},
*/

// Color List //

//��ɫ��    |cffffa500
//���ɫ��  |cffff8c00
//��ɫ��    |cffff0000
//����ɫ��  |cffd63931
//�ʼ���    |cff4169e1
//ǳ��ɫ��  |cffadd8e6
//��ɫ��    |cffffff00
//��ɫ��    |cff008000
//��ɫ��    |cffda70d6
//��ɫ��    |cffffffff
//�ǰ�      |cffbbbbbb


#include "ScriptPCH.h"
#pragma execution_character_set("UTF-8")
// Teleport data

struct Eric
{
	uint32 menu_id, next_menu_id;
	uint8 icon;
	std::string name;
	uint32 cost;
	uint8 level, faction; // 0 ����, 1 ����, 2 ����
	uint32 map;
	float x, y, z, o;
};
typedef struct Eric Rochet2;

//    {menu_id, next_menu_id, icon, name, cost, level, faction, map, x, y, z, o}
Rochet2 TeLe[] = // key = i & Key
{
	{ 1, 2, 2, "|cff4169E1|TInterface\\icons\\Achievement_Leader_King_Varian_Wrynn:30|t ���������ǡ�|r", 0, 0, 2, 0, 0, 0, 0, 0 },
	{ 1, 3, 2, "|cffFF0000|TInterface\\icons\\Achievement_Leader_Sylvanas:30|t ���������ǡ�|r", 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 4, 2, "|cff008000|TInterface\\icons\\Temp:30|t ���������ǡ�|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 5, 9, "|cffFF8C00|TInterface\\icons\\Achievement_FeatsOfStrength_Gladiator_10:30|t ��PVP����|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 6, 2, "|cffFF0000|TInterface\\icons\\Achievement_Dungeon_Icecrown_Frostmourne:30|t �����ֱ����紫�͡�|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 7, 2, " ������ķ�ࡿ|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 8, 2, " ������������|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 9, 2, " ������|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 10, 2, " ��ŵɭ�¡�|r", 0, 0, 0, 0, 0, 0, 0, 0 },

	{ 2, 0, 2, "�����", 0, 0, 2, 0, -8842.09f, 626.358f, 94.0867f, 3.61363f },
	{ 2, 0, 2, "������˹", 0, 0, 2, 1, 9869.91f, 2493.58f, 1315.88f, 2.78897f },
	{ 2, 0, 2, "��¯��", 0, 0, 2, 0, -4900.47f, -962.585f, 501.455f, 5.40538f },
	{ 2, 0, 2, "������", 0, 0, 2, 530, -3864.92f, -11643.7f, -137.644f, 5.50862f },
	{ 2, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },

	{ 3, 0, 2, "�¸�����", 0, 0, 1, 1, 1567.1f, -4400.1f, 16.2f, 0.45f },
	{ 3, 0, 2, "������", 0, 0, 1, 1, -1266.55f, 69.8601f, 128.159f, 1.10623f },
	{ 3, 0, 2, "�İ���", 0, 0, 1, 0, 1633.75f, 240.167f, -43.1034f, 6.26128f },
	{ 3, 0, 2, "���³�", 0, 0, 1, 530, 9733.33f, -7456.35f, 13.57f, 3.643914f },
	{ 3, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },

	{ 4, 0, 2, "����Ȼ", 0, 67, 0, 571, 5813.54f, 449.37f, 658.752f, 4.93338f },
	{ 4, 0, 2, "ɳ��˹", 0, 57, 0, 530, -1882.23f, 5419.88f, -12.42f, 0.52835f },
	{ 4, 0, 2, "���ݳ�", 0, 10, 0, 1, -977.1f, -3788.1f, 6.0422f, 4.860144f },
	{ 4, 0, 2, "�ر�����", 0, 25, 0, 0, -14281.9f, 552.564f, 8.90422f, 0.860144f },
	{ 4, 0, 2, "�ӻ�ɭ", 0, 45, 0, 1, -7116.81f, -3761.51f, 8.45f, 4.1053f },
	{ 4, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },

	{ 5, 0, 2, "������ʿ������", 0, 30, 0, 0, -13181.8f, 339.356f, 42.9805f, 1.18013f },
	{ 5, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },

	{ 6, 0, 2, "(80)���Ӷ�ɽ", 0, 80, 0, 1, 5513.701f, -3596.48f, 1570.06f, 2.24753f },
	{ 6, 0, 2, "(80)��˹����", 0, 80, 0, 0, -6114.48f, 4289.47f, -349.01f, 2.17599f },
	{ 6, 0, 2, "(82)����֮��", 0, 82, 0, 646, 908.77f, 502.86f, -49.21f, 0.216f },
	{ 6, 0, 2, "(83)�µ�ķ", 0, 83, 0, 1, -9440.11f, -958.85f, 111.0589f, 0.01f },
	{ 6, 0, 2, "(84)ĺ��ߵ�", 0, 84, 0, 0, -4205.18f, -5101.18f, 25.73f, 4.97293f },
	{ 6, 0, 2, "(85)�ж�������", 0, 85, 0, 732, -702.01f, 1185.01f, 101.85f, 3.0023f },
	{ 6, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },
	
	{ 7, 0, 2, "(1)��¡����", 0, 1, 1, 1, 319.54f, -4743.54f, 9.6f, 2.58f },
	{ 7, 0, 2, "(1)Ī����", 0, 1, 1, 1, -2323.49f, -559.14f, -5.01f, 1.37599f },
	{ 7, 0, 2, "(10)ƶ�֮��", 0, 10, 0, 1, 908.77f, 502.86f, -49.21f, 0.216f },
	{ 7, 0, 2, "(10)�ں���", 0, 10, 2, 1, 7453.66f, -326.84f, 8.01f, 3.216f },
	{ 7, 0, 2, "(10)������", 0, 10, 0, 1, 2784.64f, -3831.784f, 85.01f, 4.516f },
	{ 7, 0, 2, "(20)�ҹ�", 0, 20, 0, 1, -2414.88f, -1393.23f, 125.16f, 1.22793f },
	{ 7, 0, 2, "(25)ʯצɽ��", 0, 25, 0, 1, 930.11f, 712.35f, 105.1589f, 4.21f },
	{ 7, 0, 2, "(30)����֮��", 0, 30, 0, 1, -1600.7711f, 1802.02f, 63.7489f, 3.01f },
	{ 7, 0, 2, "(35)����˹", 0, 35, 0, 1, -4813.1311f, 1164.08f, 89.5489f, 1.71f },
	{ 7, 0, 2, "(40)ǧ��ʯ��", 0, 40, 0, 1, -4903.318f, -2184.0744f, 28.05f, 2.82023f },
	{ 7, 0, 2, "(45)�����ɭ��", 0, 45, 0, 1, 3635.67f, -1507.01f, 177.48f, 0.0023f },
	{ 7, 0, 2, "(50)��Ȫ��", 0, 50, 0, 1, 6814.67f, -2321.01f, 585.48f, 4.8523f },
	{ 7, 1, 7, "|cffffffff����..|r", 0, 0, 0, 0, 0, 0, 0, 0 },

};
	//    {10,    0,    2,    "Teleport",    0,    0,    0,    map,    xf,    yf,    zf,    of},
	//    {10,    1,    7,    "Back..",    0,    0,    0,    0,    0,    0,    0,    0},


// TextIDs from npc_text
enum eEnums
{
	TEXT_MAIN_H = 400000,            //Horde main menu text
	TEXT_MAIN_A = 400001,            //Alliance main menu text
	TEXT_DUNGEON = 400002,            //Dungeon teleport menu texts
	TEXT_RAID = 400003,            //Raid teleport menu text
	TEXT_AREA = 400004,            //Area teleport location menu texts

	TELEPORT_COUNT = sizeof TeLe / sizeof(*TeLe),
};

#define ARE_YOU_SURE    "|cffffffff���͵�|r "
#define ERROR_COMBAT    "|cffff0000Kill Whatever that thing is first!|r"

bool Custom_FactCheck(uint32 Fact, unsigned char Key)
{
	bool Show = false;
	switch (TeLe[Key].faction)
	{
	case 0:
		Show = true;
		break;
	case 1:
		if (Fact == HORDE)
			Show = true;
		break;
	case 2:
		if (Fact == ALLIANCE)
			Show = true;
		break;
	}
	return (Show);
}

uint32 Custom_GetText(unsigned int menu, Player* pPlayer)
{
	uint32 TEXT = TEXT_AREA;
	switch (menu)
	{
	case 0:
		switch (pPlayer->GetTeam())
		{
		case ALLIANCE:
			TEXT = TEXT_MAIN_A;
			break;
		case HORDE:
			TEXT = TEXT_MAIN_H;
			break;
		}
	case 1:
	case 2:
	case 3:
		TEXT = TEXT_DUNGEON;
		break;
	case 4:
		TEXT = TEXT_RAID;
		break;
	}
	return (TEXT);
}

void Custom_GetMenu(Player* pPlayer, Creature* pCreature, uint32 Key)
{
	bool ENDMENU = false;
	for (uint32 i = 0; i < TELEPORT_COUNT; i++)
	{
		if (ENDMENU && TeLe[i].menu_id != Key)
			break;
		if (TeLe[i].menu_id == Key && pPlayer->getLevel() >= TeLe[i].level && Custom_FactCheck(pPlayer->GetTeam(), i))
		{
			if (TeLe[i].next_menu_id != 0)
				pPlayer->ADD_GOSSIP_ITEM_EXTENDED(TeLe[i].icon, TeLe[i].name, GOSSIP_SENDER_MAIN, i, "", TeLe[i].cost, false);
			else
				pPlayer->ADD_GOSSIP_ITEM_EXTENDED(TeLe[i].icon, TeLe[i].name, GOSSIP_SENDER_MAIN, i, ARE_YOU_SURE + TeLe[i].name, TeLe[i].cost, false);
			ENDMENU = true;
		}
	}
	pPlayer->PlayerTalkClass->SendGossipMenu(Custom_GetText(Key, pPlayer), pCreature->GetGUID());
}

class tele_script_mortal : public CreatureScript
{
public:
	tele_script_mortal()
		: CreatureScript("tele_script_mortal")
	{
	}

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		Custom_GetMenu(pPlayer, pCreature, 1);
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus(); // clear the menu

		pPlayer->ModifyMoney(-1 * TeLe[uiAction].cost); // take cash
		uint32 Key = TeLe[uiAction].next_menu_id;
		if (Key == 0) // if no next_menu_id, then teleport to coords
		{
			if (!pPlayer->isInCombat())
			{
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(TeLe[uiAction].map, TeLe[uiAction].x, TeLe[uiAction].y, TeLe[uiAction].z, TeLe[uiAction].o);
				return true;
			}
			pPlayer->GetSession()->SendAreaTriggerMessage(ERROR_COMBAT);
			Key = TeLe[uiAction].menu_id;
		}

		Custom_GetMenu(pPlayer, pCreature, Key);
		return true;
	}
};

void AddSC_tele_script_mortal()
{
	new tele_script_mortal();  //����Ա�ű�
}