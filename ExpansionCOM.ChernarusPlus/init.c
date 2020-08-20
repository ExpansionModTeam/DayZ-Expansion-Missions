//#define EXPANSION_LOOT_SPAWN_POSITION_THINGY

#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\core\\BaseModuleInclude.c"

#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\expansion\\missions\\MissionConstructor.c"

Mission CreateCustomMission(string path)
{
	return new CommunityOfflineClient();
}

typedef string CFString;

class CFString: string
{
	void Test()
	{
		Print( string.Format( "Test: %1", value ) );
	}
};

void main()
{
	CFString test_string = "Hello";
	test_string.Test();

	bool loadTraderObjects = true;
	bool loadTraderNPCs = false;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);
	
	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}
	
	//! Init server central economy
	// Hive ce = CreateHive();
	// if ( ce )
	// 	ce.InitOffline();
}