#include "ScriptPCH.h"

class Reset_OnDuelStart : public PlayerScript
{
public:
	Reset_OnDuelStart() : PlayerScript("Reset_OnDuelStart") {}

	void OnDuelStart(Player *player1, Player *player2)
	{
		// Resets Players 
		player1->RemoveArenaSpellCooldowns();
		player1->SetHealth(player1->GetMaxHealth());
		player1->SetPower(player1->getPowerType(), player1->GetMaxPower(player1->getPowerType()));

		player2->RemoveArenaSpellCooldowns();
		player2->SetHealth(player2->GetMaxHealth());
		player2->SetPower(player2->getPowerType(), player2->GetMaxPower(player2->getPowerType()));
	}
};

void AddSC_Reset()
{
	new Reset_OnDuelStart;          //¾ö¶·ÖØÖÃÀäÈ´
}
