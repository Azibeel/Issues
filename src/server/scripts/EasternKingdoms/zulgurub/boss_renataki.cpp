/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Boss_Renataki
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */


#include "def_zulgurub.h"

#define SPELL_AMBUSH         24337
#define SPELL_THOUSANDBLADES 24649

class boss_renataki : public CreatureScript
{
public:
    boss_renataki() : CreatureScript("boss_renataki")
    { }

    class boss_renatakiAI : public ScriptedAI
    {
        public:
        boss_renatakiAI(Creature *c) : ScriptedAI(c) {}
    
        uint32 Invisible_Timer;
        uint32 Ambush_Timer;
        uint32 Visible_Timer;
        uint32 Aggro_Timer;
        uint32 ThousandBlades_Timer;
    
        bool Invisible;
        bool Ambushed;
    
        void Reset()
        override {
            Invisible_Timer = 8000 + rand()%10000;
            Ambush_Timer = 3000;
            Visible_Timer = 4000;
            Aggro_Timer = 15000 + rand()%10000;
            ThousandBlades_Timer = 4000 + rand()%4000;
    
            Invisible = false;
            Ambushed = false;
        }
    
        void EnterCombat(Unit *who)
        override {
        }
    
        void UpdateAI(const uint32 diff)
        override {
            if (!UpdateVictim())
                return;
    
            //Invisible_Timer
            if (Invisible_Timer < diff)
            {
                me->InterruptSpell(CURRENT_GENERIC_SPELL);
                me->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
                me->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO , 218171138);
                me->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO  + 1, 3);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);
                Invisible = true;
    
                Invisible_Timer = 15000 + rand()%15000;
            }else Invisible_Timer -= diff;
    
            if (Invisible)
            {
                if (Ambush_Timer < diff)
                {
                    Unit* target = nullptr;
                    target = SelectTarget(SELECT_TARGET_RANDOM,0);
                    if (target)
                    {
                        DoTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        DoCast(target,SPELL_AMBUSH);
                    }
    
                    Ambushed = true;
                    Ambush_Timer = 3000;
                }else Ambush_Timer -= diff;
            }
    
            if (Ambushed)
            {
                if (Visible_Timer < diff)
                {
                    me->InterruptSpell(CURRENT_GENERIC_SPELL);
                    me->SetUInt32Value(UNIT_FIELD_DISPLAYID,15268);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 31818);
                    me->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO , 218171138);
                    me->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO  + 1, 3);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    Invisible = false;
    
                    Visible_Timer = 4000;
                }else Visible_Timer -= diff;
            }
    
            //Resetting some aggro so he attacks other gamers
            if(!Invisible)
            {
                if (Aggro_Timer < diff)
                {
                    Unit* target = nullptr;
                    target = SelectTarget(SELECT_TARGET_RANDOM,1);
    
                    if(me->GetThreat(me->GetVictim()))
                        DoModifyThreatPercent(me->GetVictim(),-50);
    
                    if (target)
                        AttackStart(target);
    
                    Aggro_Timer = 7000 + rand()%13000;
                }else Aggro_Timer -= diff;
            }
            
            if (!Invisible)
            {
                if(ThousandBlades_Timer < diff)
                {
                    DoCast(me->GetVictim(), SPELL_THOUSANDBLADES);
                    ThousandBlades_Timer = 7000 + rand()%5000;
                } else { ThousandBlades_Timer -= diff; }
            }
    
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_renatakiAI(creature);
    }
};


void AddSC_boss_renataki()
{
    new boss_renataki();
}

