class CommunityOfflineClient : MissionGameplay
{
	ref array<Object> m_CustomObjects = new array<Object>;
	
	void CommunityOfflineClient()
	{
		NewCOMModuleManager();
	}

	void ~CommunityOfflineClient()
	{
		Print("CommunityOfflineClient::~CommunityOfflineClient()");
	}

	ref array< Object > debugObjs = new array< Object >;

	void CreateDebugObject( string type, vector trans[4] )
	{
		Object dbg = GetGame().CreateObjectEx( "ExpansionDebugBox_Red", "0 0 0", ECE_CREATEPHYSICS );
		dbg.SetTransform( trans );
		debugObjs.Insert( dbg );
	}

	void GetHeadingTransform( Object obj, out vector trans[4] )
	{
		float heading = obj.GetOrientation()[0] * Math.DEG2RAD;

		trans[1] = "0 1 0";
		trans[2] = Vector( Math.Sin( heading ), 0, Math.Cos( heading ) );
		trans[0] = -(trans[2] * trans[1]);

		trans[3] = obj.GetPosition();
	}

	void GetBoundingPositionTransform( Object obj, vector position, vector wsTrans[4], out vector trans[4] )
	{
		vector bbTrans[4];
		Math3D.YawPitchRollMatrix( "0 0 0", bbTrans );
		bbTrans[3] = position;

		Math3D.MatrixMultiply4( wsTrans, bbTrans, trans );

		vector contact_pos;
		vector contact_dir
		vector hitNormal;
		int contactComponent;
		float hitFraction;
		Object hitObject;
		
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER;
		DayZPhysics.RayCastBullet( trans[3] + "0 10 0", trans[3] - "0 10 0", collisionLayerMask, obj, hitObject, contact_pos, hitNormal, hitFraction );
		trans[3] = contact_pos;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		GetCOMModuleManager().OnUpdate(timeslice);
	}

	override void OfflineMissionStart()
	{
		// COTIsActive = true;

		vector pPos = "0 0 0";

		pPos = "9664.391602 79.323418 5552.737793";

		PlayerBase player;
		Class.CastTo( player, GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), pPos, 0, "NONE" ) );
		GetGame().SelectPlayer( NULL, player );

		if (player)
		{
			EntityAI item = NULL;

			item = player.GetInventory().CreateInInventory("Mich2001Helmet");
			item = item.GetInventory().CreateAttachment("NVGoggles");
			item.GetInventory().CreateAttachment("Battery9V");
			item = player.GetInventory().CreateInInventory("M65Jacket_Black");
			item = player.GetInventory().CreateInInventory("OMNOGloves_Brown");
			item = player.GetInventory().CreateInInventory("BDUPants");
			item = player.GetInventory().CreateInInventory("MilitaryBoots_Black");
			item = item.GetInventory().CreateAttachment("CombatKnife");
			
			item = player.GetInventory().CreateInInventory("DryBag_Blue");
			
			item = player.GetInventory().CreateInInventory("Shovel");

			item = player.GetInventory().CreateInInventory("Expansion_M16");
			item.GetInventory().CreateAttachment("Battery9V");

			player.COTSetGodMode(true);
		}

		pPos[1] = GetGame().SurfaceY(pPos[0], pPos[2]);
		GetGame().GetPlayer().SetPosition(pPos);

		SpawnItem( GetPlayer(), "ExpansionAdminHammer" );
		SpawnItem( GetPlayer(), "CrowBar" );

		SpawnItem( GetPlayer(), "ExpansionStairKit" );
		SpawnItem( GetPlayer(), "ExpansionStairKit" );

		SpawnItem( GetPlayer(), "ExpansionFloorKit" );
		SpawnItem( GetPlayer(), "ExpansionFloorKit" );
		SpawnItem( GetPlayer(), "ExpansionFloorKit" );

		SpawnItem( GetPlayer(), "ExpansionWallKit" );
		SpawnItem( GetPlayer(), "ExpansionWallKit" );
		SpawnItem( GetPlayer(), "ExpansionWallKit" );

		SpawnItem( GetPlayer(), "ExpansionRampKit" );
		SpawnItem( GetPlayer(), "ExpansionRampKit" );
	}

	override void OnInit()
	{
		super.OnInit();

		SetupWeather();

		SetupDateAndTime();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		GetCOMModuleManager().OnInit();
		GetCOMModuleManager().OnMissionStart();

		GetGame().GetUIManager().CloseMenu(MENU_INGAME);
	}

	override void OnMissionFinish()
	{
		GetCOMModuleManager().OnMissionFinish();

		CloseAllMenus();

		DestroyAllMenus();

		GetGame().GetUIManager().CloseMenu(MENU_INGAME);

		super.OnMissionFinish();
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		GetCOMModuleManager().OnMissionLoaded();
	}

	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);

		if (key == KeyCode.KC_PERIOD)
		{
			if (GetUIManager().IsMenuOpen(MENU_GESTURES))
			{
				GesturesMenu.CloseMenu();
			}
		}
		if (key == KeyCode.KC_C)
		{
			Camera currentCam = Camera.GetCurrentCamera();
			int year, month, day, hour, minute;
			float overcast, rain, fog, windspeed;
			GetGame().GetWorld().GetDate(year, month, day, hour, minute);
			string date = year.ToString() + ", " + month.ToString() + ", " + day.ToString() + ", " + hour.ToString() + ", " + minute.ToString();
			overcast = g_Game.GetWeather().GetOvercast().GetActual();
			rain = g_Game.GetWeather().GetRain().GetActual();
			fog = g_Game.GetWeather().GetFog().GetActual();
			windspeed = g_Game.GetWeather().GetWindSpeed();
			string weather = "\nOvercast: " + overcast.ToString() + "\nRain: " + rain.ToString() + "\nFog: " + fog.ToString() + "\nWindSpeed: " + windspeed.ToString();
			string clipboard = "CameraPOS: " + currentCam.GetPosition() + "\nCameraORI: " + currentCam.GetOrientation() + "\nCharPOS: " + GetPlayer().GetPosition() + "\nCharORI: " + GetPlayer().GetOrientation() + "\nFOV: " + currentCam.GetCurrentFOV() + "\nDate: " + date + weather;
			GetGame().CopyToClipboard(clipboard);
		}
	}

	static void SetupWeather()
	{
		Weather weather = g_Game.GetWeather();

		weather.GetOvercast().SetLimits(0.0, 2.0);
		weather.GetRain().SetLimits(0.0, 2.0);
		weather.GetFog().SetLimits(0.0, 2.0);

		weather.GetOvercast().SetForecastChangeLimits(0.0, 0.0);
		weather.GetRain().SetForecastChangeLimits(0.0, 0.0);
		weather.GetFog().SetForecastChangeLimits(0.0, 0.0);

		weather.GetOvercast().SetForecastTimeLimits(1800, 1800);
		weather.GetRain().SetForecastTimeLimits(600, 600);
		weather.GetFog().SetForecastTimeLimits(600, 600);

		weather.GetOvercast().Set(0.0, 0, 0);
		weather.GetRain().Set(0.0, 0, 0);
		weather.GetFog().Set(0.0, 0, 0);

		weather.SetWindMaximumSpeed( 20 );
		weather.SetWindFunctionParams( 0.1, 1.0, 20 );
		weather.SetWindSpeed( 0.1 );
	}

	static void SetupDateAndTime()
	{
		int year, month, day, hour, minute;
		int reset_month = 9, reset_day = 20;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		if ((month == reset_month) && (day < reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month == reset_month + 1) && (day > reset_day))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
			else
			{
				if ((month < reset_month) || (month > reset_month + 1))
				{
					GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
				}
			}
		}
	}

	Object SpawnObjectCustom(string type, vector position, vector orientation, bool insertInArray = true)
	{
		Object obj = GetGame().CreateObject(type, position);
		if (obj)
		{
			obj.SetPosition(position);
			obj.SetOrientation(orientation);
			vector roll = obj.GetOrientation();
			roll[2] = roll[2] - 1;
			obj.SetOrientation(roll);
			roll[2] = roll[2] + 1;
			obj.SetOrientation(roll);

			if (insertInArray)
				m_CustomObjects.Insert(obj);
		}

		return obj;
	}

	void RemoveAttachment(PlayerBase player, string slot_name)
	{
		int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
		if (slot_id == InventorySlots.INVALID)
			return;

		Object oitm = player.GetInventory().FindAttachment(slot_id);
		if (oitm)
		{
			GetGame().ObjectDelete(oitm);
		}
	}

	void SpawnItem(PlayerBase player, string item)
	{
		Object oitm = player.GetInventory().CreateInInventory(item);
		if (!oitm)
		{
			vector position = player.GetPosition();
			oitm = GetGame().CreateObject(item, position);

			if (oitm)
			{
				position[0] = position[0] + Math.RandomFloatInclusive(-5, 5);
				position[2] = position[2] + Math.RandomFloatInclusive(-5, 5);
				position[1] = GetGame().SurfaceY(position[0], position[2]);
				oitm.SetPosition(position);
			}
		}
	}

	void SpawnKit(string kit)
	{
		SpawnItem(PlayerBase.Cast( GetGame().GetPlayer() ), kit);
		SpawnItem(PlayerBase.Cast( GetGame().GetPlayer() ), "Nail");
		SpawnItem(PlayerBase.Cast( GetGame().GetPlayer() ), "WoodenPlank");
	}
}