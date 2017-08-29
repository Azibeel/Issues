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
SDName: Boss_Gorosh_the_Dervish
SD%Complete: 100
SDComment:
SDCategory: Blackrock Depths
EndScriptData */



enum Spells
{
    SPELL_WHIRLWIND                                        = 15589,
    SPELL_MORTALSTRIKE                                     = 24573
};

class boss_gorosh_the_dervish : public CreatureScript
{
public:
    boss_gorosh_the_dervish() : CreatureScript("boss_gorosh_the_dervish")
    { }

    class boss_gorosh_the_dervishAI : public ScriptedAI
    {
        public:
        boss_gorosh_the_dervishAI(Creature *c) : ScriptedAI(c) {}
    
        uint32 WhirlWind_Timer;
        uint32 MortalStrike_Timer;
    
        void Reset()
        override {
            WhirlWind_Timer = 12000;
            MortalStrike_Timer = 22000;
        }
    
        void EnterCombat(Unit *who)
        override {
        }
    
        void UpdateAI(const uint32 diff)
        override {
            //Return since we have no target
            if (!UpdateVictim() )
                return;
    
            //WhirlWind_Timer
            if (WhirlWind_Timer < diff)
            {
                DoCast(me,SPELL_WHIRLWIND);
                WhirlWind_Timer = 15000;
            }else WhirlWind_Timer -= diff;
    
            //MortalStrike_Timer
            if (MortalStrike_Timer < diff)
            {
                DoCast(me->GetVictim(),SPELL_MORTALSTRIKE);
                MortalStrike_Timer = 15000;
            }else MortalStrike_Timer -= diff;
    
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_gorosh_the_dervishAI(creature);
    }
};


void AddSC_boss_gorosh_the_dervish()
{
    new boss_gorosh_the_dervish();
}

