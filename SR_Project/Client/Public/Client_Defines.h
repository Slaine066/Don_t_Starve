#pragma once

#include "Engine_Defines.h"
#include <process.h>

class Item;

extern HWND	g_hWnd;
extern HINSTANCE g_hInst;

namespace Client
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HUNT, LEVEL_MAZE, LEVEL_BOSS, LEVEL_END };
	enum OBJID { OBJ_PLAYER, OBJ_MOUSE, OBJ_BUTTON, OBJ_MONSTER, OBJ_BULLET, OBJ_ITEM, OBJ_OBJECT, OBJ_NPC, OBJ_PORTAL, OBJ_BACKGROUND, OBJ_END };
	enum DIR_STATE { DIR_DOWN, DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_END };
	enum WEAPON_TYPE { WEAPON_HAND, WEAPON_SWORD, WEAPON_STAFF, WEAPON_DART, WEAPON_BOMB, WEAPON_SMOKE, WEAPON_LIGHTNING, WEAPON_ICESPIKE1, WEAPON_ICESPIKE2, WEAPON_ICESPIKE3, WEAPON_ICESPIKE4, WEAPON_ICESMOKE,
		WEAPON_MINES, WEAPON_MINE, WEAPON_PUFF, WEAPON_ICEBLAST, BEARGER_SPECIAL, BOARRIOR_SPECIAL, WEAPON_END };
	enum SKILL_TYPE { SKILL_HEAL, SKILL_END};
	enum TOOLTYPE { TOOL_GATHERING, TOOL_WEAPON, TOOL_TEST1, TOOL_TEST2, TOOL_END };
	enum ITEMID { ITEM_FOOD, ITEM_EQUIPMENT, ITEM_MATERIAL, ITEM_BAG, ITEM_ARMOR, ITEM_HAND, ITEM_HAT,ITEM_STRUCT, ITEM_END };
	enum INTERACTOBJ_ID { TREE, GRASS, BOULDER, BERRYBUSH, ITEMS, CARROT, NPC, PORTAL, COOKPOT, TENT, SKELETON, DIRT, ID_END };
	enum ITEMNAME {
		ITEMNAME_ARMOR, ITEMNAME_AXE, ITEMNAME_BAG, ITEMNAME_BERRY, ITEMNAME_SHOTTER, ITEMNAME_WOOD, ITEMNAME_CARROT, ITEMNAME_COAL,
		ITEMNAME_STAFF, ITEMNAME_GRASS, ITEMNAME_ROCK, ITEMNAME_HELMET, ITEMNAME_GOLD, ITEMNAME_HAMBAT, ITEMNAME_WOOD2, ITEMNAME_MEAT,
		ITEMNAME_SPIDERMEAT, ITEMNAME_PICK, ITEMNAME_PIGTAIL, ITEMNAME_ROCK2, ITEMNAME_ROPE, ITEMNAME_WEB, ITEMNAME_TORCH,
		ITEMNAME_QUEST1 , ITEMNAME_POT , ITEMNAME_BLOCK ,ITEMNAME_FENCE, ITEMNAME_TENT,ITEMNAME_MEATBALL ,ITEMNAME_NUGGET, ITEMNAME_LASAGNA, ITEMNAME_BERRYPIE,
		ITEMNAME_TRASH1 , ITEMNAME_TRASH2 , ITEMNAME_QUEST2,ITEMNAME_END
	};
	enum NPCID {NPC_PIGKING, NPC_WENDY}; 

	enum MONSTERID { MONSTER_PIG , MONSTER_SPIDER, MONSTER_SPIDERWARRIOR , MONSTER_BEARGER , MONSTER_BOARON, MONSTER_BOARRIOR, MONSTER_END };
		

	enum MAKEWHAT { MAKE_AXE, MAKE_PICK, MAKE_HAMBAT, MAKE_SHOTTER, MAKE_STAFF, MAKE_ARMOR, MAKE_HELMET,MAKE_FENCE,MAKE_POT,MAKE_TENT,MAKE_ROPE,MAKE_COAL,MAKE_TORCH, MAKE_END };


	enum SHADER_STATE { SHADER_IDLE_ALPHATEST, SHADER_IDLE_ALPHABLEND, SHADER_HIT, SHADER_PICKING, SHADER_DEAD, SHADER_LIGHT, SHADER_DARK };
	enum UI_SHADER_STATE { UI_SHADER_IDLE , UI_SHADER_SCREEN };

	const  _float  g_iWinSizeX = 1280;
	const  _float  g_iWinSizeY = 720;

	typedef struct ObjectInfo
	{
		float iMaxHp;
		float iCurrentHp;
		float fDamage;
		//vector<Item*> vItemDrops;
	}OBJINFO;

	typedef struct foreffect
	{
		_float3 pos;
		_float dmg;
		ITEMNAME itemname;

	}Poteffect;

	

	typedef struct tagBulletData {
		_bool bIsPlayerBullet;
		WEAPON_TYPE eWeaponType;
		DIR_STATE eDirState;
		_float3 vPosition;
		_float3	vTargetPos;		
		_float3 vLook;
		_float3 vRight;
		_float3 vScale;
		_float fOffsetUp;
		_float fOffsetDown;
		_float fOffsetSide;
	}BULLETDATA;

}


#include <process.h>
#pragma warning (disable : 4819)
// Client_Defines.h

extern HWND		g_hWnd;
extern HINSTANCE g_hInst;
extern _bool		g_bLoadingfirst;
extern _bool		g_bUIMadefirst;
extern _bool        g_bscenechange;
extern _uint       g_iQuestnum;
extern _bool       g_boss;
extern _bool	   g_ColliderRender;

using namespace Client;