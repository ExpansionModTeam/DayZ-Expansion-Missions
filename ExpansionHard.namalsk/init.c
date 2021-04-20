/**
 * init.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#include "$CurrentDir:\\mpmissions\\ExpansionHard.namalsk\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\mpmissions\\ExpansionHard.namalsk\\expansion\\missions\\MissionConstructor.c"

void main()
{
	bool loadTraderObjects = false;
	bool loadTraderNPCs = false;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);

	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}
	/*
	  [Namalsk] CE init (offline)
	*/
	CreateHive();
	GetHive().InitOffline();

	/*
	  [Namalsk] Weather init
	   Warning: DO NOT ALTER following values as they are interconnected with other Namalsk-specific systems!
	   To ensure correct functionality, it is necessary to include weaher init AFTER the hive init.
	*/
	Weather weather = GetGame().GetWeather();
	weather.MissionWeather( true );
	weather.GetOvercast().SetLimits( 0.10, 0.30 );
	weather.GetRain().SetLimits( 0.0, 0.0 );
	weather.GetFog().SetLimits( 0.0, 0.0 );
	weather.GetOvercast().SetForecastChangeLimits( 0.1, 0.3 );
	weather.GetOvercast().SetForecastTimeLimits( 1600, 2100 );
	weather.GetOvercast().Set( Math.RandomFloatInclusive( 0.1, 0.2 ), 0, 0 );	// ignored if storage is present
	weather.GetRain().Set( 0, 0, 0 );											// ignored if storage is present
	weather.GetFog().Set( 0, 0, 0 );											// ignored if storage is present
	weather.SetWindMaximumSpeed( 30 );
	weather.SetWindFunctionParams( 0.1, 1.0, 42 );
	weather.SetStorm( 0, 1, 1 );
	weather.SetRainThresholds( 0.0, 1.0, 0 );

	/*
	  [Namalsk] Mission time init
	   after CE init to determine if storage mission type is outside of the required time-frame
	   currently recommended time-frame is:
		12/1 -> 12/31
		keep in mind that gameplay features are tied to the mission date (stored in the storage) and that it SHOULD remain this period!
	   while using:
		day accelerated 6 times (serverTimeAcceleration=6), resulting in an average 78 min of day-time (RL)
		night accelerated 24 times (serverNightTimeAcceleration=4), resulting in an average of 26 min of night-time (RL)
	*/
	int year, month, day, hour, minute;
	GetGame().GetWorld().GetDate( year, month, day, hour, minute );

    if ( month < 12 )
    {
    	year = 2011;
        month = 12;
        day = 1;
		
		GetGame().GetWorld().SetDate( year, month, day, hour, minute );
	}
};

class CustomMission: MissionServer
{
	override void OnInit()
	{
		ExpansionMissionModule missionModule;
		if ( Class.CastTo( missionModule, GetModuleManager().GetModule( ExpansionMissionModule ) ) )
		{
			missionModule.SetMissionConstructor( COMMissionConstructor );
		}

		super.OnInit();

		// this piece of code is recommended otherwise event system is switched on automatically and runs from default values
		// comment this whole block if NOT using Namalsk Survival
		if ( m_EventManagerServer )
		{
			// enable/disable event system, min time between events, max time between events, max number of events at the same time
			m_EventManagerServer.OnInitServer( true, 550, 1000, 2 );
			// registering events and their probability
			m_EventManagerServer.RegisterEvent( Aurora, 0.85 );
			m_EventManagerServer.RegisterEvent( Blizzard, 0.4 );
			m_EventManagerServer.RegisterEvent( ExtremeCold, 0.4 );
			m_EventManagerServer.RegisterEvent( Snowfall, 0.6 );
			m_EventManagerServer.RegisterEvent( EVRStorm, 0.35 );
			m_EventManagerServer.RegisterEvent( HeavyFog, 0.3 );
		}
	}
	
	// ------------------------------------------------------------
	// Override CreateCharacter
	// ------------------------------------------------------------
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	void SetRandomHealth(EntityAI itemEnt)
	{
		if (itemEnt)
		{
			float rndHlt = Math.RandomFloat( 0.50, 0.85 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override void StartingEquipSetup( PlayerBase player, bool clothesChosen )
	{
		if ( !GetExpansionSettings().GetSpawn().StartingClothing.EnableCustomClothing )
		{
			EntityAI itemClothing;
			EntityAI itemEnt;
			ItemBase itemBs;
			float rand;
			
			// top
			itemClothing = player.FindAttachmentBySlotName( "Body" );
			if ( itemClothing )
			{
				SetRandomHealth( itemClothing );

				itemEnt = itemClothing.GetInventory().CreateInInventory( "Rag" );
				if ( Class.CastTo( itemBs, itemEnt ) )
					itemBs.SetQuantity( 4 );
				player.SetQuickBarEntityShortcut( itemEnt, 0 );

				SetRandomHealth( itemEnt );
				
				itemEnt = itemClothing.GetInventory().CreateInInventory( "RoadFlare" );
				SetRandomHealth( itemEnt );
				itemEnt = itemClothing.GetInventory().CreateInInventory( "RoadFlare" );
				SetRandomHealth( itemEnt );
				player.SetQuickBarEntityShortcut( itemEnt, 1 );
			}

			// pants
			itemClothing = player.FindAttachmentBySlotName( "Legs" );
			if ( itemClothing )
			{
				SetRandomHealth( itemClothing );

				itemEnt = itemClothing.GetInventory().CreateInInventory( "Heatpack" );
				SetRandomHealth( itemEnt );

				int throwDice = Math.RandomInt( 0, 2 );
				if ( throwDice == 0 )
					itemEnt = itemClothing.GetInventory().CreateInInventory( "dzn_tool_watch" );
				else
					itemEnt = itemClothing.GetInventory().CreateInInventory( "dzn_tool_watch2" );
				player.SetQuickBarEntityShortcut( itemEnt, 2 );
			}

			// shoes
			itemClothing = player.FindAttachmentBySlotName( "Feet" );
			if ( itemClothing )
			{
				SetRandomHealth( itemClothing );
			}

			// bump fresh spawn water and energy values (to compensate for the frozen food and harder-to-get wells)
			player.GetStatWater().Set( 900 );
			player.GetStatEnergy().Set( 1100 );
		}
	}
};
  
Mission CreateCustomMission(string path)
{
	return new CustomMission();
};
