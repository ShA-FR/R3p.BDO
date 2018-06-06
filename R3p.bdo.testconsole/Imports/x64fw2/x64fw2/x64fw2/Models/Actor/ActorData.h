#ifndef H_ACTORDATA
#define H_ACTORDATA
#include "../../Utilities/MemoryObject.h"

class ActorData : public MemoryObject
{
private:
	const int o_VTable = 0x0;//-
	const int o_InteractionFrag = 0x20;//-
	const int o_ActorKey = 0x58;//-
	const int o_ActorType = 0x5C;//-
	const int o_CharacterName = 0x68;//-
	const int o_Hp = 0x98;//-
	const int o_MaxHp = 0xB8;//-
	const int o_MaxMp = 0xE4;//-
	const int o_WorldPosition = 0x110;//-
	const int o_CurrentRegion = 0x120;//-
	const int o_CharacterClass[3] = { 0x128, 0x198, 1 };//-
	const int o_NonPlayerLevel[2] = { 0x128, 0x2F4 };//-
	const int o_ServantType[2] = { 0x128, 0x32 };
	const int o_ServantKind[2] = { 0x128, 0x8 };
	const int o_ActorId = 0x130;//-
	const int o_ActiveBuffCount = 0x168;//-
	const int o_ActiveBuffList = 0x170;//-
	const int o_IsDead = 0x198;//-
	const int o_WorldPositionCollectables = 0x360;//-
	const int o_CharacterController = 0x380;//-
	const int o_CombatStance = 0x3EC;//-
	const int o_EquipmentStart = 0x4A0;//-
	const int o_DialogIndex = 0xBE0;
	const int o_MovementSpeed = 0xC00;//-
	const int o_AttackSpeed = o_MovementSpeed + 4;//-
	const int o_CastSpeed = o_AttackSpeed + 4;//-	
	const int o_ActionIndex = 0xC2C;
	const int o_IsAggro = 0xEF2;//-
	const int o_MaxWeight = 0x1068;//---	
	const int o_IsPossibleManufactureAtWarehouse = 0x10B8;//-
	const int o_IsCharacterNameTagVisible = 0x10BC;//-
	const int o_IsGuildNameTagVisible = 0x10BE;//-
	const int o_GuildName = 0x10CA;//-
	const int o_Level = 0x1120;//-
	const int o_FamilyName = 0x12A8;//-
	const int o_SkillListFirstNode = 0x15C8;//-
	const int o_SkillListCount = o_SkillListFirstNode + 0x8;//-
	const int o_LastItemUseStartTick = 0x1640;//-
	const int o_Mp = 0x1B28;//-
	const int o_IsReadyToPlay = 0x1CAD;//-
	const int o_WeightItemsAndSilver = 0x1CB0;//-
	const int o_Inventory = 0x1CB8;//-
	const int o_FreeInventorySlots = 0x1CD0;//-
	const int o_MaxInventorySlots = 0x1CD1;//-
	const int o_WeightEquippedItems = 0x1DB0;//-
	const int o_MainHand = 0x1DB8;//-
	const int o_FishGrade = 0x4110;//-
	const int o_NextFishBite = o_FishGrade - 16;//-
	const int o_NextAutoFishAutoCatch = o_FishGrade - 8;//-
	const int o_IsFishing = o_FishGrade + 4;//-
	const int o_FishHooked = o_FishGrade + 21;//-

public:
	ActorData(__int64 address);
	~ActorData();


};

#endif