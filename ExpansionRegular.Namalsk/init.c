void main()
{
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
		11/1 -> 11/30
		keep in mind that gameplay features are tied to the mission date (stored in the storage) and that it SHOULD remain this period!
	   while using:
		day accelerated 6 times (serverTimeAcceleration=6), resulting in an average 78 min of day-time (RL)
		night accelerated 24 times (serverNightTimeAcceleration=4), resulting in an average of 26 min of night-time (RL)
	*/
	int year, month, day, hour, minute;
	GetGame().GetWorld().GetDate( year, month, day, hour, minute );

    if ( ( month < 11 ) || ( month >= 12 ) )
    {
    	year = 2011;
        month = 11;
        day = 1;
		
		GetGame().GetWorld().SetDate( year, month, day, hour, minute );
	}
};

class CustomMission: MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		// this piece of code is recommended otherwise event system is switched on automatically and runs from default values
		// comment this whole block if NOT using Namalsk Survival
		if ( m_EventManagerServer )
		{
			// enable/disable event system, min time between events, max time between events
            m_EventManagerServer.OnInitServer( true, 600, 800 );
            // Register possible events along with their probability (0..1)
	    // any custom events MUST inherit from EventBase, otherwise they will fail to load!
            m_EventManagerServer.RegisterEvent( Aurora, 1.0 );
            m_EventManagerServer.RegisterEvent( Blizzard, 0.5 );
            m_EventManagerServer.RegisterEvent( ExtremeCold, 0.4 );
            m_EventManagerServer.RegisterEvent( Snowfall, 0.8 );
            m_EventManagerServer.RegisterEvent( EVRStorm, 0.25 );
		}
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
};
  
Mission CreateCustomMission(string path)
{
	return new CustomMission();
};
