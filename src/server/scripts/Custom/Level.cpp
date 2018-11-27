#include "ScriptPCH.h"
#include "Unit.h"
#pragma execution_character_set("UTF-8")

enum
{
	item_id = 90001,
	max_lvl = 85,
	lvl = 1
};
class Level_item : public ItemScript
{
public:Level_item() : ItemScript("Level_item") { }

		bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
		{
			if (player->getLevel() == max_lvl)
			{
				player->GetSession()->SendNotification("您已经满级了...");
				return false;
			}
			else
			{
				player->SetLevel(max_lvl);
				player->DestroyItemCount(item->GetEntry(), 1, true, false);
				return true;
			}
		}
};

void AddSC_Level_item()
{
	new Level_item();            //满级卷轴
}