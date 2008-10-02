/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Skeram
SD%Complete: 75
SDComment: Mind Control buggy.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "def_temple_of_ahnqiraj.h"
#include "Group.h"

#define SPELL_ARCANE_EXPLOSION      25679
#define SPELL_EARTH_SHOCK           26194
#define SPELL_TRUE_FULFILLMENT4     26526
#define SPELL_BLINK                 28391

#define SOUND_AGGRO1       8615                             //8615 Are you so eager to die? I would be happy to accomodate you.
#define SOUND_AGGRO2       8616                             //8616 Cower mortals! The age of darkness is at hand.
#define SOUND_AGGRO3       8621                             //8621 Tremble! The end is upon you.
#define SOUND_SLAY1        8617                             //8617 Let your death serve as an example!
#define SOUND_SLAY2        8619                             //8619 Spineless wretchers you will drown in rivers of blood!
#define SOUND_SLAY3        8620                             //8620 The screams of the dying will fill the air. A symphony of terror is about to begin!
#define SOUND_SPLIT        8618                             //8618 Prepare for the return of the ancient ones!
#define SOUND_DEATH        8622                             //8622 You only delay... the inevetable

class ov_mycoordinates
{
    public:
        float x,y,z,r;
        ov_mycoordinates(float cx, float cy, float cz, float cr)
        {
            x = cx; y = cy; z = cz; r = cr;
        }
};

struct MANGOS_DLL_DECL boss_skeramAI : public ScriptedAI
{
    boss_skeramAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (c->GetInstanceData()) ? ((ScriptedInstance*)c->GetInstanceData()) : NULL;
        IsImage = false;
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 ArcaneExplosion_Timer;
    uint32 EarthShock_Timer;
    uint32 FullFillment_Timer;
    uint32 Blink_Timer;
    uint32 Invisible_Timer;

    Creature *Image1, *Image2;

    bool Images75;
    bool Images50;
    bool Images25;
    bool IsImage;
    bool Invisible;

    void Reset()
    {
        ArcaneExplosion_Timer = 6000 + rand()%6000;
        EarthShock_Timer = 2000;
        FullFillment_Timer = 15000;
        Blink_Timer = 8000 + rand()%12000;
        Invisible_Timer = 500;

        Images75 = false;
        Images50 = false;
        Images25 = false;
        Invisible = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);

        if (IsImage)
            m_creature->setDeathState(JUST_DIED);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
            case 0:
                DoPlaySoundToSet(m_creature,SOUND_SLAY1);
                break;
            case 1:
                DoPlaySoundToSet(m_creature,SOUND_SLAY2);
                break;
            case 2:
                DoPlaySoundToSet(m_creature,SOUND_SLAY3);
                break;
        }
    }

    void JustDied(Unit* Killer)
    {
        if (!IsImage)
            DoPlaySoundToSet(m_creature,SOUND_DEATH);
    }

    void Aggro(Unit *who)
    {
        if (IsImage || Images75)
            return;
        switch(rand()%3)
        {
            case 0:
                DoPlaySoundToSet(m_creature,SOUND_AGGRO1);
                break;
            case 1:
                DoPlaySoundToSet(m_creature,SOUND_AGGRO2);
                break;
            case 2:
                DoPlaySoundToSet(m_creature,SOUND_AGGRO3);
                break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            ArcaneExplosion_Timer = 8000 + rand()%10000;
        }else ArcaneExplosion_Timer -= diff;

        //If we are within range melee the target
        if( m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            //Make sure our attack is ready and we arn't currently casting
            if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
            {
                m_creature->AttackerStateUpdate(m_creature->getVictim());
                m_creature->resetAttackTimer();
            }
        }else
        {
            //EarthShock_Timer
            if (EarthShock_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_EARTH_SHOCK);
                EarthShock_Timer = 1000;
            }else EarthShock_Timer -= diff;
        }

        //Blink_Timer
        if (Blink_Timer < diff)
        {
            //DoCast(m_creature, SPELL_BLINK);
            switch(rand()%3)
            {
                case 0:
                    m_creature->Relocate(-8340.782227,2083.814453,125.648788,0);
                    DoResetThreat();
                    break;
                case 1:
                    m_creature->Relocate(-8341.546875,2118.504639,133.058151,0);
                    DoResetThreat();
                    break;
                case 2:
                    m_creature->Relocate(-8318.822266,2058.231201,133.058151,0);
                    DoResetThreat();
                    break;
            }
            DoStopAttack();

            Blink_Timer= 20000 + rand()%20000;
        }else Blink_Timer -= diff;

        int procent = (int) (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() +0.5);

        //Summoning 2 Images and teleporting to a random position on 75% health
        if ( (!Images75 && !IsImage) &&
            (procent <= 75 && procent > 70) )
            DoSplit(75);

        //Summoning 2 Images and teleporting to a random position on 50% health
        if ( (!Images50 && !IsImage) &&
            (procent <= 50 && procent > 45) )
            DoSplit(50);

        //Summoning 2 Images and teleporting to a random position on 25% health
        if ( (!Images25 && !IsImage) &&
            (procent <= 25 && procent > 20) )
            DoSplit(25);

        //Invisible_Timer
        if (Invisible)
        {
            if (Invisible_Timer < diff)
            {
                //Making Skeram visible after telporting
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                Invisible_Timer = 2500;
                Invisible = false;
            }else Invisible_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }

    void DoSplit(int atPercent /* 75 50 25 */)
    {
        DoPlaySoundToSet(m_creature,SOUND_SPLIT);
        Unit* target = NULL;
        target = SelectUnit(SELECT_TARGET_RANDOM,0);

        ov_mycoordinates *place1 = new ov_mycoordinates(-8340.782227,2083.814453,125.648788,0);
        ov_mycoordinates *place2 = new ov_mycoordinates(-8341.546875,2118.504639,133.058151,0);
        ov_mycoordinates *place3 = new ov_mycoordinates(-8318.822266,2058.231201,133.058151,0);

        ov_mycoordinates *bossc=place1, *i1=place2, *i2=place3;

        switch(rand()%3)
        {
            case 0:
                bossc=place1;i1=place2;i2=place3;
                break;
            case 1:
                bossc=place2;i1=place1;i2=place3;
                break;
            case 2:
                bossc=place3;i1=place1;i2=place2;
                break;
        }

        for (int tryi = 0; tryi < 41; tryi ++)
        {
            Unit *targetpl = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if (targetpl->isType(TYPEMASK_PLAYER))
            {
                Group *grp = ((Player *)targetpl)->GetGroup();
                if (grp)
                {
                    for (int ici = 0; ici < TARGETICONCOUNT; ici++)
                    {
                        //if (grp ->m_targetIcons[ici] == m_creature->GetGUID()) -- private member:(
                        grp->SetTargetIcon(ici, 0);
                    }
                }
                break;
            }
        }

        m_creature->RemoveAllAuras();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->Relocate(bossc->x, bossc->y, bossc->z, bossc->r);
        Invisible = true;
        DoResetThreat();
        DoStopAttack();

        switch (atPercent)
        {
            case 75: Images75 = true; break;
            case 50: Images50 = true; break;
            case 25: Images25 = true; break;
        }

        Image1 = m_creature->SummonCreature(15263, i1->x, i1->y, i1->z, i1->r, TEMPSUMMON_CORPSE_DESPAWN, 30000);
        Image1->SetMaxHealth(m_creature->GetMaxHealth() / 5);
        Image1->SetHealth(m_creature->GetHealth() / 5);
        Image1->AI()->AttackStart(target);

        Image2 = m_creature->SummonCreature(15263,i2->x, i2->y, i2->z, i2->r, TEMPSUMMON_CORPSE_DESPAWN, 30000);
        Image2->SetMaxHealth(m_creature->GetMaxHealth() / 5);
        Image2->SetHealth(m_creature->GetHealth() / 5);
        Image2->AI()->AttackStart(target);

        ((boss_skeramAI*)Image1->AI())->IsImage = true;
        ((boss_skeramAI*)Image2->AI())->IsImage = true;

        Invisible = true;
    }
};

CreatureAI* GetAI_boss_skeram(Creature *_Creature)
{
    return new boss_skeramAI (_Creature);
}

void AddSC_boss_skeram()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_skeram";
    newscript->GetAI = GetAI_boss_skeram;
    m_scripts[nrscripts++] = newscript;
}
