#include "ScriptPCH.h"
#include "blackwing_descent.h"
#include "Vehicle.h"
#include "Unit.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "ObjectMgr.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "Spell.h"
#include "Player.h"
#include "Map.h"
#include "InstanceScript.h"
#pragma execution_character_set("UTF-8")

enum Events
{
	EVENT_DOUBLE_ATTACK = 1,
	EVENT_DOUBLE_ATTACK_REAL,
	EVENT_CAUSTIC_SLIME,
	EVENT_CAUSTIC_SLIME_BREAK,
	EVENT_CAUSTIC_SLIME_BREAK_END,
	EVENT_MASSACRE,
	EVENT_BREAK_BILE_O_TRON,
	EVENT_FEUD,
	EVENT_SEC_MASSACRE,
	EVENT_BREAK,
	EVENT_REMOVE_FEUD,
	EVENT_BERSERK,

    EVENT_AGGRO_NEF
};

enum Actions
{
    ACTION_BILE_O_TRON_EVENT_START      = 1,
    ACTION_BILE_O_TRON_SYSTEM_FAILURE,
    ACTION_BILE_O_TRON_RESET
};

enum Spells
{
    // Chimaeron
    SPELL_ASLEEP                        = 82706,
    SPELL_DOUBLE_ATTACK                 = 88826,
    SPELL_DOUBLE_ATTACK_REAL            = 82882,
    SPELL_CAUSTIC_SLIME                 = 82913,
	//SPELL_CAUSTIC_SLIME                 = 82935,
    SPELL_MASSACRE                      = 82848,
    SPELL_FEUD                          = 88872,
    SPELL_BREAK                         = 82881,

    SPELL_MORTALITY                     = 82934,
    SPELL_MORTALITY_RAID_DEBUFF         = 82890,

	SPELL_RANGE                        = 26662,       

    // Bile O Tron
    SPELL_FINKLES_MIXTURE               = 82705,
    SPELL_FINKLES_MIXTURE_VISUAL        = 91106,
    SPELL_SYSTEM_FAILURE                = 88853,
    SPELL_REROUTE_POWER                 = 88861,
	SPELL_LOW_HEALTH                    = 89084,

   // Nefarian
   SPELL_MOCKING_SHADOWS                = 91307
};

enum ScriptTexts
{
    SAY_MASSACRE                    = -1900028, // announce - Chimaeron prepares to massacre his foes!

    // Finkle
    SAY_INTRO                       = -1851023,
    SAY_SYSTEM_FAILURE              = -1851026,
    SAY_DEATH                       = -1851027,
    SAY_F_OUTRO                     = -1851028,
    SAY_P2                          = -1851029
};

Position const BilePositions[6] =
{
    {-135.795151f, 15.569847f, 73.165909f, 4.646072f},
    {-129.176636f, -10.488489f, 73.079071f, 5.631739f},
    {-106.186249f, -18.533386f, 72.798332f, 1.555510f},
    {-77.951973f, 0.702321f, 73.093552f, 1.509125f},
    {-77.466125f, 31.038124f, 73.177673f, 4.489712f},
    {-120.426445f, 34.491863f, 72.057610f, 4.116642f}
};

class boss_chimaeron : public CreatureScript
{
public:
    boss_chimaeron() : CreatureScript("boss_chimaeron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_chimaeronAI (creature);
    }

    struct boss_chimaeronAI: public BossAI
    {
        boss_chimaeronAI(Creature* creature) : BossAI(creature, DATA_CHIMAERON)
        {
			instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;
		bool bJustReset;
		uint32 ResetTimer;
        uint8 phase;
		uint32 masscrac;

        void Reset()
        {
            events.Reset();
            me->SetReactState(REACT_PASSIVE);
            phase = 1;

            me->RemoveAura(SPELL_DOUBLE_ATTACK);
            me->RemoveAura(SPELL_MORTALITY);
            me->RemoveAura(SPELL_MORTALITY_RAID_DEBUFF);
            me->AddAura(SPELL_ASLEEP, me);
            //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

			masscrac = 0;
            if (instance)
                instance->SetData(DATA_CHIMAERON, NOT_STARTED);

            if (Creature* finkle_einhorn = me->FindNearestCreature(NPC_FINKLE_EINHORN, 150.0f, true))
                finkle_einhorn->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if (Creature* bilotron = me->FindNearestCreature(NPC_BILE_O_TRON, 150.0f, true))
                bilotron->AI()->DoAction(ACTION_BILE_O_TRON_RESET);

            if(Creature* nefarianHelperheroic = me->FindNearestCreature(NPC_NEFARIAN_HELPER_HEROIC, 150.0f, true))
                nefarianHelperheroic->DespawnOrUnsummon(100);

            _Reset();

			instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BREAK);
        }

		void EnterEvadeMode()
		{
			events.Reset();
			instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
			instance->SetData(DATA_CHIMAERON, FAIL);
			me->GetMotionMaster()->MoveTargetedHome();
			//_DespawnAtEvade();
			_Reset();
			_EnterEvadeMode();

			ScriptedAI::EnterEvadeMode();
		}

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
            {
                instance->SetData(DATA_CHIMAERON, IN_PROGRESS);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me); // Add
            }

			if (Creature* finkle_einhorn = me->FindNearestCreature(NPC_FINKLE_EINHORN, 150.0f, true))
				finkle_einhorn->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if(me->GetMap()->IsHeroic())
            {
				me->SummonCreature(NPC_NEFARIAN_HELPER_HEROIC, -113.206f, 43.0277f, 80.1364f, 4.60826f, TEMPSUMMON_MANUAL_DESPAWN);
                events.ScheduleEvent(EVENT_AGGRO_NEF, 1500);
            }

			me->SetReactState(REACT_AGGRESSIVE);

			masscrac = 0;

            events.ScheduleEvent(EVENT_MASSACRE, 26000);
            events.ScheduleEvent(EVENT_DOUBLE_ATTACK, 15000);
            events.ScheduleEvent(EVENT_BREAK, 4500);
			events.ScheduleEvent(EVENT_BERSERK, 450000);

            _EnterCombat();
        }

        void JustSummoned(Creature* summon)
        {
            summon->AI()->DoZoneInCombat();
        }

        void UpdateAI(const uint32 diff)
        {

            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

			if (me->GetDistance(-106, -48, 76) <= 2.5)
			{
				me->AI()->EnterEvadeMode();
				return;
			}

			if (me->HasAura(SPELL_DOUBLE_ATTACK))
			{
				if (Unit* target = me->GetVictim())
					if (me->isAttackReady() && me->IsWithinMeleeRange(me->GetVictim()))
					{
						me->resetAttackTimer();
						me->AttackerStateUpdate(me->GetVictim());
						DoCast(target, SPELL_DOUBLE_ATTACK_REAL);
						me->RemoveAura(SPELL_DOUBLE_ATTACK);
					}
			}

            if(me->GetHealthPct() < 21 && phase == 1)
            {
                phase = 2;
				me->RemoveAura(SPELL_FEUD);
                DoCast(me, SPELL_MORTALITY);
                DoCastAOE(SPELL_MORTALITY_RAID_DEBUFF);

                if (Creature* nefarianHelperheroic = me->FindNearestCreature(NPC_NEFARIAN_HELPER_HEROIC, 150.0f, true))
                {
                    nefarianHelperheroic->AI()->Talk(33);
                    nefarianHelperheroic->CastSpell(nefarianHelperheroic, SPELL_MOCKING_SHADOWS, true);
                }

				if (Creature* finkle_einhorn = me->FindNearestCreature(NPC_FINKLE_EINHORN, 150.0f, true))
					finkle_einhorn->AI()->Talk(4);

				if (Creature* bilotron = me->FindNearestCreature(NPC_BILE_O_TRON, 150.0f, true))
					bilotron->RemoveAura(SPELL_SYSTEM_FAILURE);

                events.CancelEvent(EVENT_MASSACRE);
                events.CancelEvent(EVENT_BREAK);
                events.CancelEvent(EVENT_CAUSTIC_SLIME);
				events.RescheduleEvent(EVENT_DOUBLE_ATTACK,5000);
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    
                    case EVENT_AGGRO_NEF:
                        if (Creature* nefarianHelperheroic = me->FindNearestCreature(NPC_NEFARIAN_HELPER_HEROIC, 150.0f, true))
                        nefarianHelperheroic->AI()->Talk(32);
                        return;
						break;
                    case EVENT_MASSACRE:
						me->InterruptNonMeleeSpells(false);
						me->InterruptSpell(CURRENT_GENERIC_SPELL);
                        DoCastAOE(SPELL_MASSACRE);
						masscrac = (masscrac + 1);
                    
						events.ScheduleEvent(EVENT_CAUSTIC_SLIME, 19000);

						if (masscrac == 2 && urand(1, 2) == 1)
						{ 
							events.ScheduleEvent(EVENT_BREAK_BILE_O_TRON, 1000);
						}
						else if (masscrac == 3)
							events.ScheduleEvent(EVENT_BREAK_BILE_O_TRON, 1000);
						else
							events.ScheduleEvent(EVENT_MASSACRE, 30000);
                    
                        break;
                    case EVENT_BREAK_BILE_O_TRON:
                        if (Creature* bilotron = me->FindNearestCreature(NPC_BILE_O_TRON, 150.0f, true))
                        {
                            bilotron->AI()->DoAction(ACTION_BILE_O_TRON_SYSTEM_FAILURE);       
                            events.ScheduleEvent(EVENT_FEUD, 1000);
                        }
						events.CancelEvent(EVENT_CAUSTIC_SLIME);
						events.CancelEvent(EVENT_MASSACRE);

						if (!me->GetMap()->IsHeroic())
						{ 
							events.CancelEvent(EVENT_BREAK);
							events.CancelEvent(EVENT_DOUBLE_ATTACK);
						}
                        break;
					case EVENT_FEUD:
						me->AddAura(SPELL_FEUD, me);
						masscrac = 0; 
     					me->SetReactState(REACT_PASSIVE);
						me->AttackStop();
						//DoCast(me,SPELL_FEUD);
						events.ScheduleEvent(EVENT_MASSACRE, 30500);
						events.ScheduleEvent(EVENT_CAUSTIC_SLIME_BREAK, 13000);
						events.ScheduleEvent(EVENT_CAUSTIC_SLIME_BREAK_END, 28000);

			

						if (!me->GetMap()->IsHeroic())
						{ 
							events.ScheduleEvent(EVENT_DOUBLE_ATTACK, 45000);
							events.ScheduleEvent(EVENT_BREAK, 40000);
						}

						if (me->GetMap()->IsHeroic())
						{
							if (Creature* nefarianHelperheroic = me->FindNearestCreature(NPC_NEFARIAN_HELPER_HEROIC, 150.0f, true))
								nefarianHelperheroic->AI()->Talk(34);

							events.ScheduleEvent(EVENT_REMOVE_FEUD, 3000);
						}
						break;
                    case EVENT_REMOVE_FEUD:
                        me->RemoveAurasDueToSpell(SPELL_FEUD);
						break;
                    case EVENT_DOUBLE_ATTACK:
                        DoCast(me, SPELL_DOUBLE_ATTACK);
                        events.ScheduleEvent(EVENT_DOUBLE_ATTACK, 16000);
                        //events.ScheduleEvent(EVENT_DOUBLE_ATTACK_REAL, 1500);
                        break;
                    
      //              case EVENT_DOUBLE_ATTACK_REAL:
      //                  DoCastVictim(SPELL_DOUBLE_ATTACK_REAL);
      //                  me->RemoveAura(SPELL_DOUBLE_ATTACK);
      //                  return;
						//break;
                    case EVENT_CAUSTIC_SLIME:
                    {
                        std::list<Unit*> targets;
						SelectTargetList(targets, RAID_MODE(2, 5, 2, 5), SELECT_TARGET_RANDOM, 100.0f, true);
                            if (!targets.empty())
                                for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                                    DoCast(*itr, SPELL_CAUSTIC_SLIME);
                    }
                        events.ScheduleEvent(EVENT_CAUSTIC_SLIME, 19000);                
                        break;
					case EVENT_CAUSTIC_SLIME_BREAK:
					{
						std::list<Unit*> targets;
						SelectTargetList(targets, RAID_MODE(2, 5, 2, 5), SELECT_TARGET_RANDOM, 100.0f, true);
						if (!targets.empty())
							for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
								DoCast(*itr, SPELL_CAUSTIC_SLIME);
					}
					events.ScheduleEvent(EVENT_CAUSTIC_SLIME_BREAK, 5000);
					break;
					case EVENT_CAUSTIC_SLIME_BREAK_END:
						events.CancelEvent(EVENT_CAUSTIC_SLIME_BREAK);
						break;
                    case EVENT_BREAK:
                        DoCastVictim(SPELL_BREAK);
                        events.ScheduleEvent(EVENT_BREAK, 15000);
                        break;
					case EVENT_BERSERK:
						DoCast(SPELL_RANGE);
						break;
                    default:
                        break;
                }
            }

            if(!me->HasAura(SPELL_FEUD))
                DoMeleeAttackIfReady();
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if(me->HasReactState(REACT_PASSIVE))
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveAurasDueToSpell(SPELL_ASLEEP);
                DoZoneInCombat(me);
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            me->RemoveAllAuras();

            if (instance)
            {
                instance->SetData(DATA_CHIMAERON, DONE);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove
            }

            if (Creature* bilotron = me->FindNearestCreature(NPC_BILE_O_TRON, 150.0f, true))
                bilotron->AI()->DoAction(ACTION_BILE_O_TRON_RESET);

            if (Creature* nefarianHelperheroic = me->FindNearestCreature(NPC_NEFARIAN_HELPER_HEROIC, 150.0f, true))
            {
                nefarianHelperheroic->RemoveAllAuras();
                nefarianHelperheroic->AI()->Talk(31);
                nefarianHelperheroic->DespawnOrUnsummon(9000);
            }

			if (Creature* finkle_einhorn = me->FindNearestCreature(NPC_FINKLE_EINHORN, 150.0f, true))
				finkle_einhorn->AI()->Talk(5);

            _JustDied();
        }
    };
};

class mob_finkle_einhorn : public CreatureScript
{
public:
    mob_finkle_einhorn() : CreatureScript("mob_finkle_einhorn") { }

    bool OnGossipHello(Player* pPlayer, Creature* creature)
    {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "我想你会需要这个笼子的钥匙吧?慢着,先别说话.我后面那只鬼吼鬼叫的怪物把它吃了,对吧?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(66666,creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "我记得,你是被困住了.情况实在太雷同了.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(66667, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "譬如,穿着熔岩服的地精.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(66668, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "没有.我...呃...没看到.你刚刚是要说什么?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(66669, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "条件?什么条件?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                player->SEND_GOSSIP_MENU(66670, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                if(InstanceScript* instance = creature->GetInstanceScript())
                {
                    if(Creature* bilotron = creature->FindNearestCreature(NPC_BILE_O_TRON, 100.0f, true))
                    {
                        bilotron->AI()->DoAction(ACTION_BILE_O_TRON_EVENT_START);
                        creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    }
                }
                player->SEND_GOSSIP_MENU(66671, creature->GetGUID());
                break;
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_finkle_einhornAI(creature);
    }

    struct mob_finkle_einhornAI : public ScriptedAI
    {
        mob_finkle_einhornAI(Creature* creature) : ScriptedAI(creature)
        {
            canFree = false;
            timer = 1000;
            despawnTimer = 500000; // set it initially so does not despawn from start xD
        }

        uint32 timer;
        uint32 despawnTimer;
        bool canFree;

        void UpdateAI(uint32 const diff) 
        {
            if (GameObject* cage = me->FindNearestGameObject(204929, 30.0f)) // Cage
                if (cage && cage->GetGoState() == GO_STATE_ACTIVE)
                {
                    canFree = true;
                    despawnTimer = 1000;
                }

            if (timer <= diff)
            {
                if (Player* target = me->FindNearestPlayer(85.0f, true))
                {
                    if (target->GetDistance(me) < 85.0f  && me->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                    {
						if (me->IsVisible())
						{ 
						Talk(urand(0, 2));
						}
                        timer = 25000;
                    } else
                        timer = 1000;
                }
                else
                    timer = 1000;

            } else timer -= diff;

            if (despawnTimer <= diff && canFree)
            {
                if (Creature* bilotron = me->FindNearestCreature(NPC_BILE_O_TRON, 150.0f, true))
                    bilotron->SetVisible(false);

                //DoScriptText(SAY_F_OUTRO, me);
				Talk(6);
                me->GetMotionMaster()->MovePoint(1, -110.295f, 41.662f, 72.657f);
                me->DespawnOrUnsummon(30000);
                canFree = false;
            }
            else despawnTimer -= diff;
        }
    };
};

class mob_bile_o_tron : public CreatureScript
{
public:
    mob_bile_o_tron() : CreatureScript("mob_bile_o_tron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_bile_o_tronAI (creature);
    }

    struct mob_bile_o_tronAI : public ScriptedAI
    {
        mob_bile_o_tronAI(Creature* creature) : ScriptedAI(creature), waypoint(7), uiSystemFailureTimer(0), activated(false)
        {
            instance = creature->GetInstanceScript();
            creature->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
            me->SetReactState(REACT_PASSIVE);
            isFailureActive = false;
            chimaeronWaken = false;
            timerMove = 10000;
        }

        InstanceScript* instance;
        uint8 waypoint;
        uint32 timerMove;
        uint32 uiSystemFailureTimer;
        uint32 wakeChimaeronTimer;
        bool activated;
        bool isFailureActive;
        bool chimaeronWaken;

        void UpdateAI(const uint32 diff)
        {
            if(!activated)
                return;

            if (!me->HasAura(SPELL_SYSTEM_FAILURE) && !me->HasAura(SPELL_FINKLES_MIXTURE))
                me->AddAura(SPELL_FINKLES_MIXTURE, me);

            if (wakeChimaeronTimer <= diff && !chimaeronWaken)
            {
                if(Creature* chimaeron = me->FindNearestCreature(BOSS_CHIMAERON, 200.0f))
                {
                    chimaeron->RemoveAurasDueToSpell(SPELL_ASLEEP);
                    if (Unit*target = chimaeron->FindNearestPlayer(50.0f, true))
					{ 
                        chimaeron->AI()->AttackStart(target);
						chimaeron->AI()->DoZoneInCombat(chimaeron);
					}

                    chimaeron->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    chimaeronWaken = true;
                }
            }
            else wakeChimaeronTimer -= diff;

            if (timerMove <= diff && waypoint != 7)
            {
                me->GetMotionMaster()->MovePoint(1, BilePositions[waypoint]); 

                if(waypoint >= 5)
                   waypoint = 0;
                else
                   waypoint++;

            } else timerMove -= diff;

            if (uiSystemFailureTimer <= diff && isFailureActive)
            { // Reroute Power
                me->RemoveAura(SPELL_SYSTEM_FAILURE);
                me->GetMotionMaster()->MovePoint(1,BilePositions[waypoint]);
                isFailureActive = false;
            }
            else uiSystemFailureTimer -= diff;
        }

        void DoAction(const int32 action)
        {

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            switch(action)
            {

                case ACTION_BILE_O_TRON_EVENT_START:
                    DoCast(me,SPELL_FINKLES_MIXTURE_VISUAL,true);
                    waypoint = 8;
                    me->GetMotionMaster()->MovePoint(1, BilePositions[0]);
                    activated = true;
                    wakeChimaeronTimer = 10000;
                    isFailureActive = false;
                    break;
                
                case ACTION_BILE_O_TRON_SYSTEM_FAILURE:
                    if (!activated)
                        return;
                
					if (Creature* finkle_einhorn = me->FindNearestCreature(NPC_FINKLE_EINHORN, 150.0f, true))
						finkle_einhorn->AI()->Talk(3);
                
                    me->RemoveAllAuras();
                    DoCast(me,SPELL_SYSTEM_FAILURE, true);
					DoCast(me, SPELL_REROUTE_POWER, true);
                    isFailureActive = true;
                    uiSystemFailureTimer = 26000;
                    break;
                
                case ACTION_BILE_O_TRON_RESET:
                    chimaeronWaken = false;
                    me->RemoveAllAuras();
                    if(Creature* chimaeron = me->FindNearestCreature(BOSS_CHIMAERON, 200.0f))
                        me->AddAura(SPELL_ASLEEP, chimaeron);
                    me->GetMotionMaster()->MoveTargetedHome();
                    waypoint = 7;
                    uiSystemFailureTimer = 0;
                    activated = false;
                    break;
            }
        }

        void JustReachedHome()
        {
            me->RemoveAllAuras();
        }
    };
};

// Finkle's Mixture
class spell_finkles_mixture : public SpellScriptLoader
{
public:
	spell_finkles_mixture() : SpellScriptLoader("spell_finkles_mixture") { }

	class spell_finkles_mixture_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_finkles_mixture_AuraScript);

		void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool& /*canBeRecalculated*/)
		{
			amount = -1;
		}

		void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
		{
			if (!GetTarget())
				return;

			uint32 curHealth = GetTarget()->GetHealth();

			if (curHealth >= 10000)
			{
				absorbAmount = dmgInfo.GetDamage() - curHealth + 1;
			}
		}

		void PeriodicTick(AuraEffect const* aurEff)
		{
			if (!GetTarget())
				return;

			if (GetTarget()->GetHealth() < 10000)
				GetTarget()->CastSpell(GetTarget(), SPELL_LOW_HEALTH, true);
			else
			{
				if (GetTarget()->HasAura(SPELL_LOW_HEALTH))
					GetTarget()->RemoveAurasDueToSpell(SPELL_LOW_HEALTH);
			}
		}

		void Register()
		{
			DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_finkles_mixture_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
			OnEffectAbsorb += AuraEffectAbsorbFn(spell_finkles_mixture_AuraScript::Absorb, EFFECT_1);
			OnEffectPeriodic += AuraEffectPeriodicFn(spell_finkles_mixture_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
		}
	};

	AuraScript* GetAuraScript() const
	{
		return new spell_finkles_mixture_AuraScript();
	}
};

void AddSC_boss_chimaeron()
{
    new boss_chimaeron();
    new mob_finkle_einhorn();
    new mob_bile_o_tron();
    new spell_finkles_mixture();
}
