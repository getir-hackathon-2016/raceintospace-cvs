/*
    Copyright (C) 2005 Michael K. McCarty & Fritz Bronner

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//---------------Event Card Structure --------------
/* Sample Work for Data File Saving and Loading

MissionCode: x = Mission Type.

TurnOnly :  1 = No new astronauts
	    2 = No manned missions this year
	    3 = Hardware 50% off this season
	    4 = Hardware 50% off next season
	    5 = 50% chance of explosion on pad
	    6 = Only one launch this turn

FuturePlans:  1 = Multi-Manned Mission
	      2 = Satellite
	      3 = Manned Launch
	      4 = Joint Mission

Astronaut.Status:  0 = Active
		   1 = Dead
		   2 = Retired
		   3 = Injured
		   4 = Training Season I
		   5 = Training Season II
		   6 = Training Season III
		   7 = Advanced Training I
		   8 = Advanced Training II
		   9 = Advanced Training III
		  10 = Advanced Training IV

Astronaut.TrainingLevel:    0 = None
			    1 = Basic Training I
			    2 = Basic Training II
			    3 = Basic Training III
			    4 = Advanced Training I
			    5 = Advanced Training II
			    6 = Advanced Training III
			    7 = Advanced Training IV

*/

#include "data_defs.h"


#pragma pack (1)

struct Prest_Upd {
 char Poss[10];
 char PossVal[10];
 char HInd;
 char Qty;
 char Mnth;
};

struct Defl {char Plr1,Plr2,Lev1,Lev2,Ast1,Ast2,Input,Anim,Music,Sound;};

struct PrestType {
  char Add[4];     // 4&5 used for qty completed
  char Goal[2];    // Goal steps used for prest calculations
  i16 Points[2];   // Total points earned
  char Indec;      // the index for the first into .History[]
  char Place;      // Who got the prestige first.
  char mPlace;     // If you've done it at all
  char Month,Year; // Date of the First
};

enum EquipmentIndex {
	EQUIP_PROBE = 0,
	EQUIP_ROCKET = 1,
	EQUIP_MANNED = 2,
	EQUIP_TECH = 3,
};

typedef struct _Equipment {
  char Name[20];     // Name of Hardware
  char ID[2];        // EquipID "C0 C1 C2 C3 : Acts as Index
	i16  Safety;        // current safety factor
  i16  MisSaf;        // Safety During Mission
	i16  MSF;           // used only to hold safety for docking kludge
	char Base;         // initial safety factor
	i16  InitCost;    // Startup Cost of Unit
	char UnitCost;     // Individual Cost
	i16  UnitWeight;    // Weight of the Item
	i16  MaxPay;        // Maximum payload
	char RDCost;       // Cost of R&D per roll
	char Code;         // Equipment Code for qty scheduled
	char Num;          // quantity in inventory
  char Spok;         // qty being used on missions
  char Seas;         // Seasons Program is Active
	char Used;         // total number used in space
  char IDX[2];       // EquipID "C0 C1 C2 C3 : Acts as Index
  i16  Steps;        // Program Steps Used
	i16  Failures;     // number of program failures
	char MaxRD;        // maximum R & D
	char MaxSafety;    // maximum safety factor
	char SMods;        // safety factor mods for next launch
	char SaveCard;     // counter next failure in this prog
	char Delay;        // delay in purchase - in seasons
	char Duration;     // Days it can last in space
	char Damage;       // Damage percent for next launch
	char DCost;        // Cost to repair damage
  char MisSucc;      // Mission Successes
  char MisFail;      // Mission Failures
} Equipment;

struct MissionType {
	char Name[25];          // name of mission
	char MissionCode;       // internal code of mission type
	char Patch;             // Patch Used on this Mission
	char part;              // primary/secondary portion
	char Hard[6];           // Prime,Kick,LEM,PayLoad,Rocket
	char Joint;             // Joint Mission flag
	char Rushing;           // rushing time 0-3
	char Month;             // Month of Launch
	char Duration;          // Duration time
	char Men;               // Qty of men used in mission
	char Prog;              // Hardware program used
	char PCrew;             // Primary Crew
	char BCrew;             // Backup Crew
	char Crew;              // Actual Mission Crew
};

enum AstronautPoolClass
{
	ASTRO_POOL_LVL1	= 7,
	ASTRO_POOL_LVL2 = 9,
	ASTRO_POOL_LVL3 = 14,
	ASTRO_POOL_LVL4 = 16,
	ASTRO_POOL_LVL5 = 14
};

struct Astros {
	char Name[14];       // Astronaut Name
	char Face;           // Code for Astronaut Face
	char Sex;            // Male or Female
	char Compat;         // Compatability Code
	char CR;             // Range of Compatability Right
	char CL;             // Range of Compatability Left
	char Moved;          // if 0 Astro Moved around this turn
	char Mis;            // Successful Mission this Turn
	char Happy;          // Happy within group
			               // 1 = Successful Mission
			               // 2 = Personal First
	char Prime;          // 2 = Primary Crew Member
			               // 1 = Backup Crew Member
			               // 0 = Not selected
	char Active;         // Seasons Active
	char AISpecial;      // AI Special Astro Note
	char Service;        // Service of Origination
	char Status;         // Current status of Astronaut
	char Focus;          // Focus of Advanced Training 0-4
	char IDelay;         // Injury Delay - Ok if 0
	char RDelay;         // Retire Delay - Retire if 0
	char RetReas;        // Retire Reason flag
	char Special;        // Special for announcements
	char Assign;         // Location of Astro 0=limbo
	char oldAssign;      // program of last turn
	char Una;            // Unassigned flag
	char Crew;           // Crew Assiciated in prog Assigm
	char Task;           // What his job is in Flt Crew
	i16  Prestige;       // Prestige Earned
	char Days;           // Days in Space
	char Medals;         // Medals Earned
	char Save;           // Astro Saving Roll Percent
	char Missions;       // Number of Missions Flown
	char MissionNum[10]; // Mission numbers participated
	char Cap;            // Capsule Pilot Skills
	char LM;             // LEM Pilot Skills
	char EVA;            // EVA Skills
	char Docking;        // Docking Skills
	char Endurance;      // Constitution Skills
	char TrainingLevel;  // Level of Schooling
	char Group;          // Group Entered with
	char Mood;           // Mood of Astronaut
	char Pool;           // Temp Storage for Astros
	char Hero;           // Hero Status
};

struct PastInfo {  // For Records of Missions
	char MissionName[2][25];         // Name of Mission
	char Patch[2];                   // Patch Type
	char MissionCode;                // Type of Mission
	char MissionYear;                // Year Mission Took Place
	char Month;                      // Month of Launch
	char Time;                       // Early / Late Flag
	char Man[2][4];			         // Astros # involved in Mis
	char Hard[2][5];                 // Set of hardware
	ui16 result;             // Success / Failure
	ui16 spResult;           // Actual deciding Result
	char Event;                      // Single event code
  char Saf;                        // Safety of the Equipment for delays
	char Part;                       // Participant in event code
	i16  Prestige;			            // Prestige Earned on this Mission
	char Duration;			            // Length of Mission (units:ABCDEF)
};

struct BuzzData {                   // master data list for Buzz Aldrin's
	char Header[4];                  // Sync information
	char Name[20];                   // Player Name
	char Level;                      // Level of Play
	short Cash;                      // Current Cash on Hand
	short Budget;                    // Next Season's Budget
	i16 Prestige;                    // Prestige Earned
	i16 PrestHist[5][2];             // Prestige History array
	i16 PresRev[5];                  // Pres. Review History
	i16 tempPrestige[2];		         // Holds this and prev prestige
	short BudgetHistory[40];         // Record of Last 40 Budgets
	short BudgetHistoryF[40];        // Record of Last 40 Budgets
	i16 Spend[5][4];                 // Record of last 5 seasons spending
					                     //  of each equip type
	char RDMods;                     // R & D Mods for turn only
	char RDYear;                     // R & D Mods for full year
	char TurnOnly;                   // Events for this turn only
	char Plans;                      // Code for future plans
					                     // 1 = Manned Launch, -10 MB
	char FuturePlans;                // Special future mission
	char DurLevel;			            // Current Duration Level
	char LMpts;                      // Accumulation of any LM Test points
	#if 0
	Equipment Hardware[4][7];
	#else
	Equipment Probe[7];       // 0 = Orbital
					                     // 1 = Inter Planetary
					                     // 2 = Lunar Probe
	Equipment Rocket[7];      // 0 = One Stage
					                     // 1 = Two Stage
					                     // 2 = Three Stage
					                     // 3 = Mega Stage
					                     // 4 = Strap On Boosters
	Equipment Manned[7];      // 0 = One Man Capsule
					                     // 1 = Two Man Capsule
					                     // 2 = Three Man Capsule
					                     // 3 = Minishuttle
					                     // 4 = Four Man Cap/Mod
					                     // 5 = Two Man Module
					                     // 6 = One Man Module
	Equipment Misc[7];        // 0 = Kicker Level A
					                     // 1 = Kicker Level B
					                     // 2 = Kicker Level C
					                     // 3 = EVA Suits
					                     // 4 = Docking Modules
					                     // 5 = Photo Recon
#endif
	char ZCost;                      // Cost for Zond upgrade 10-20
	char ZFlag;                      // Flag for Zond program
	char DMod;                       // Docking Module in Orbit
	char LaunchFacility[MAX_LAUNCHPADS];          // Cost to repair (-1=none)
	char AstroCount;                 // Current # in Program
	char AstroLevel;                 // Level of selection
	char AstroDelay;                 // Wait until next selection
	struct Astros Pool[65];          // Pool of SpaceMen
	struct WantList {
	  char Vle;
	  char Asp;
	} WList[5];
   struct Prest_Upd Udp[3];
	char WTop;
	char VList[5];
	char VTop;
	char Table[5][7];                // safety factor for intelligence
	char Gcnt[ASTRONAUT_POOLS+1][ASTRONAUT_CREW_MAX];                 // Count of num in groups
	char Crew[ASTRONAUT_POOLS+1][ASTRONAUT_CREW_MAX][ASTRONAUT_FLT_CREW_MAX+1];              // Flt Crews
					 // [5] - Program #
					 // [8] - Eight Crews Max
					 // [4] - Four Max per Crew
	char Female;                     // Females allowed in program
	struct MissionType Mission[MAX_MISSIONS];   // Current Mission Info
	char Block;                      // Blocked Mission Number
	struct MissionType Future[MAX_MISSIONS];    // Future Mission Info
	struct PastInfo History[100];    // Past Mission History
	short PastMis;                   // Number of Past Missions
	char Other;                      // Catastrophic Fail on Turn
					 // 1 = Cat Failure
					 // 2 = Program Death
					 // 3 = Program First
	char Buy[4][7];                  // Record of R&D for Turn
  char eCount;
	struct Intelligence {
	  char code;
	  i16 num;
	  char prog;
	  char cur;
	  char index;
	  char sf;
	  char cdex;
	 } PastIntel[30];
	char AIpath[23];                 // Track AI choices from evts
	char Track[10];                  // Misc Values
	char AILunar,                    // Flag to tell way to moon
	     AIPrim,                     // Currect Primary Prog
	     AISec,                      // Currect Secondary Prog
	     AIEva,
	     AIDur,
	     AIDck,
	     AIStat,                     // the status of program
	     AITech,                     // Level of AI technology
	     AILem;
	char X[20];                      // AI where the hell am I status
	char M[62];                      // How many missions of type done
	char BadCard[15];                // Flag for Bad Events
   char Port[40];                   // Levels for SPort display
};

struct Players {
	char BUZZ[4];                    // Save Version Marker
	ui32 Checksum;                   // Checksum of Data
	char plr[NUM_PLAYERS];                     // Order of Turns
	struct Defl Def;                 // Defaults
	char Year;                       // Game Turn
	char Season;                     // Season of Year
	struct PrestType Prestige[28];   // Definitions of Prest Vals
	struct BuzzData P[NUM_PLAYERS];            // Player Game Data
	char EMark[4];                   // Event Marker
	char Events[100];                // History of Event Cards
	char Count;                      // Number of Events Picked
   char PD[NUM_PLAYERS][28];
	char Mile[NUM_PLAYERS][10];                // MileStone Calcs
};



/* MissionHard Descriptions
   0=Capsule
   1=Kicker
   2=LM
   3=Probe/DM
   4=Primary Booster
   5=EVA
   6=Photo Recon
   7=Secondary Booster
*/

enum MissionHardwareType
{
	Mission_Capsule = 0,			// 0
	Mission_Kicker,						// 1
	Mission_LM,								// 2
	Mission_Probe_DM,					// 3
	Mission_PrimaryBooster,		// 4
	Mission_EVA,							// 5
	Mission_PhotoRecon,				// 6
	Mission_SecondaryBooster	// 7
};

struct MisAst {  // This struct will be -1's if empty 
  struct Astros *A;
  char loc;         
};

struct MisEval {
  char step;             // actual step id number
  char loc;              // Mission Step Name Index
  ui16 StepInfo; // ID of step success  1=succ   !1=fail
  Equipment *E;   // Pointer into equipment`
  char Prest;            // Prestige Step #  (-1 for none)
  char PComp;            // PComp will be set to amt of prest to be awarded.
  char pad;              // pad location  (Index into First Part of MH[x][]
  char Class;            // VAB Order index into Class types MH[][x]
  char fgoto;            // goto on failure
  char sgoto;            // goto on success
  char dgoto;            // where to branch on deaths
  char trace;            // allow tracing of mission (for prest)
  char dice;             // Die roll to check safety against
  i16 rnum;              // Random number holder for failure type
  char ast;              // Astro specialist num
  char asf;              // Astro specialist safety addition
  char FName[5];         // Name of Failure File (without .DAT)
  char Name[10];         // Name of Anim Code
};

struct XFails {
  i32 per;               // Failure Key number
  i16 code,val,xtra;     // Failure Codes
  i16 fail;              // failure value ID
  char text[200];// Failure Text
};

struct AnimType {
  char ID[8];
  char OVL[4];
  char SD[2][4];         // Sound ID : Max 2
  i16 w,h;
  ui8 sPlay[2];// Frame to play the Sound
  ui8 fNum;    // Number of frames
  ui8 fLoop;   // Times to loop
  ui8 cOff;    // Color offsets
  ui8 cNum;    // Number of Colors
  // char cPal[cNum];    // Placemarker for RGB Colors
};

struct BlockHead {
  ui8 cType;   // Type of Compression
  i32 fSize;            // Size of data frame
  // char aFrame[fSize];
};

struct mStr {
  char Index;         // Mission Index Number
  char Name[50];      // Name of Mission
  char Abbr[24];      // Name of Mission Abbreviated
  char Code[62];      // Mission Coding String
  char Alt[36];       // Alternate Sequence
  char AltD[36];      // Alternate Sequence for Deaths (Jt Missions Only)
  char Days,          // Duration Level
       Dur,         // Duration Mission
       Doc,         // Docking Mission
       EVA,         // EVA Mission
       LM,          // LM Mission
       Jt,          // Joint Mission
       Lun,         // Lunar Mission
       mEq,           // Minimum Equipment
       mCrew;         // Useful for Morgans Code
  ui8 mVab[2];       // Hardware Requirements
  char PCat[5],       // Prestige Category List
       LMAd;          // LM Addition Points
};

struct MXM {
   char Code[8];
   i16  Qty;
   i32 Offset[6];
};

struct VInfo {char name[15],qty,ac,sf,dex,img;i16 wt;};
struct ManPool {char Name[14],Sex,Cap,LM,EVA,Docking,Endurance;};
struct order {i16 plr,loc,budget,date;};    

struct mfab {
  char Text[2][75];
  char qty;
  i16 idx[4];
};

// EOF
