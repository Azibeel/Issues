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
SDName: Dun_Morogh
SD%Complete: 50
SDComment: Quest support: 1783
SDCategory: Dun Morogh
EndScriptData */

/* ContentData
npc_narm_faulk
EndContentData */



/*######
## npc_narm_faulk
######*/

#define SAY_HEAL -1000280


class npc_narm_faulk : public CreatureScript
{
public:
    npc_narm_faulk() : CreatureScript("npc_narm_faulk")
    { }

    class npc_narm_faulkAI : public ScriptedAI
    {
        public:
        uint32 lifeTimer;
        bool spellHit;
    
        npc_narm_faulkAI(Creature *c) : ScriptedAI(c) {}
    
        void Reset()
        override {
            lifeTimer = 120000;
            me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 32);
            me->SetUInt32Value(UNIT_FIELD_BYTES_1,7);   // lay down
            spellHit = false;
        }
    
        void EnterCombat(Unit* pWho)
        override {
        }
    
        void MoveInLineOfSight(Unit* pWho)
        override {
            return;
        }
    
        void UpdateAI(const uint32 diff)
        override {
            if (!me->GetUInt32Value(UNIT_FIELD_BYTES_1))
            {
                if(lifeTimer < diff)
                {
                    EnterEvadeMode();
                    return;
                }
                else
                    lifeTimer -= diff;
            }
        }
    
        void SpellHit(Unit* Hitter, const SpellInfo* Spellkind)
        override {
            if(Spellkind->Id == 8593 && !spellHit)
            {
                DoCast(me,32343);
                me->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
                me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
                //me->RemoveAllAuras();
                DoScriptText(SAY_HEAL, me);
                spellHit = true;
            }
        }
    
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_narm_faulkAI(creature);
    }
};


void AddSC_dun_morogh()
{

    new npc_narm_faulk();
}

