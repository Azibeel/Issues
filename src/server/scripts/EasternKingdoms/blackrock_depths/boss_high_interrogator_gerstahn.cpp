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
SDName: Boss_High_Interrogator_Gerstahn
SD%Complete: 100
SDComment:
SDCategory: Blackrock Depths
EndScriptData */



enum Spells
{
    SPELL_SHADOWWORDPAIN                                   = 10894,
    SPELL_MANABURN                                         = 10876,
    SPELL_PSYCHICSCREAM                                    = 8122,
    SPELL_SHADOWSHIELD                                     = 22417
};

class boss_high_interrogator_gerstahn : public CreatureScript
{
public:
    boss_high_interrogator_gerstahn() : CreatureScript("boss_high_interrogator_gerstahn")
    { }

    class boss_high_interrogator_gerstahnAI : public ScriptedAI
    {
        public:
        boss_high_interrogator_gerstahnAI(Creature *c) : ScriptedAI(c) {}
    
        uint32 ShadowWordPain_Timer;
        uint32 ManaBurn_Timer;
        uint32 PsychicScream_Timer;
        uint32 ShadowShield_Timer;
    
        void Reset()
        override {
            ShadowWordPain_Timer = 4000;
            ManaBurn_Timer = 14000;
            PsychicScream_Timer = 32000;
            ShadowShield_Timer = 8000;
        }
    
        void EnterCombat(Unit *who)
        override {
        }
    
        void UpdateAI(const uint32 diff)
        override {
            //Return since we have no target
            if (!UpdateVictim() )
                return;
    
            //ShadowWordPain_Timer
            if (ShadowWordPain_Timer < diff)
            {
                Unit* target = nullptr;
                target = SelectTarget(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_SHADOWWORDPAIN);
                ShadowWordPain_Timer = 7000;
            }else ShadowWordPain_Timer -= diff;
    
            //ManaBurn_Timer
            if (ManaBurn_Timer < diff)
            {
                Unit* target = nullptr;
                target = SelectTarget(SELECT_TARGET_RANDOM,0);
                if (target)DoCast(target,SPELL_MANABURN);
                ManaBurn_Timer = 10000;
            }else ManaBurn_Timer -= diff;
    
            //PsychicScream_Timer
            if (PsychicScream_Timer < diff)
            {
                DoCast(me->GetVictim(),SPELL_PSYCHICSCREAM);
                PsychicScream_Timer = 30000;
            }else PsychicScream_Timer -= diff;
    
            //ShadowShield_Timer
            if (ShadowShield_Timer < diff)
            {
                DoCast(me,SPELL_SHADOWSHIELD);
                ShadowShield_Timer = 25000;
            }else ShadowShield_Timer -= diff;
    
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_high_interrogator_gerstahnAI(creature);
    }
};


void AddSC_boss_high_interrogator_gerstahn()
{
    new boss_high_interrogator_gerstahn();
}

