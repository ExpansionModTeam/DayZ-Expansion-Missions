/**
 * init.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

void main()
{


	//! If you have the DayZ-Expansion-Market mod loaded, copy this lines...
	bool loadTraderObjects = true;
	bool loadTraderNPCs = true;

	string MissionWorldName = "empty"; // Do not change
	GetGame().GetWorldName(MissionWorldName);

	if (MissionWorldName != "empty") // Do not change
	{
		//! Spawn mission objects and traders
		ExpansionObjectSpawnTools.FindMissionFiles("$CurrentDir:\\mpmissions\\dayzOffline." + MissionWorldName, loadTraderObjects, loadTraderNPCs);

		// If your mission is not named dayzOffline.MapName (example: dayzOffline.Chernarusplus)
		// but instead something different like empty.MapName (example: empty.deerisle)
		// replace dayzOffline by your mission name (in the example it was the word "empty")
	}
	//! ....Until here !


}