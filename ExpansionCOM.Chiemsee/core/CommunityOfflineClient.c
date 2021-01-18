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

		/*
		Object obj = CarScript.GetAll()[0];
		if ( obj )
		{
			for ( int j = 0; j < debugObjs.Count(); j++ )
			{
				GetGame().ObjectDelete( debugObjs[j] );
			}

			debugObjs.Clear();

			vector boundingBox[2];
			obj.GetCollisionBox( boundingBox );

			vector headingTransform[4];
			GetHeadingTransform( obj, headingTransform );

			float l = boundingBox[0][0];
			float r = boundingBox[1][0];
			float d = boundingBox[0][1];
			float u = boundingBox[1][1];
			float b = boundingBox[0][2];
			float f = boundingBox[1][2];

			Object dbg = NULL;

			vector bbTransFL[4];
			GetBoundingPositionTransform( obj, Vector( l, d, f ), headingTransform, bbTransFL );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransFL );

			vector bbTransFR[4];
			GetBoundingPositionTransform( obj, Vector( r, d, f ), headingTransform, bbTransFR );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransFR );

			vector bbTransBL[4];
			GetBoundingPositionTransform( obj, Vector( l, d, b ), headingTransform, bbTransBL );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransBL );

			vector bbTransBR[4];
			GetBoundingPositionTransform( obj, Vector( r, d, b ), headingTransform, bbTransBR );
			CreateDebugObject( "ExpansionDebugBox_Red", bbTransBR );
		}
		*/
	}

	override void OfflineMissionStart()
	{
		// COTIsActive = true;

		vector pPos = "0 0 0";

		pPos = "8948.92 16.8124 4822.77";

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
			item = item.GetInventory().CreateAttachment("Expansion_ANPEQ15_Green");
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
		ExpansionMissionModule missionModule;
		if (Class.CastTo(missionModule, GetModuleManager().GetModule(ExpansionMissionModule)))
		{
			missionModule.SetMissionConstructor(COMMissionConstructor);
		}

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

		/*
		if (key == KeyCode.KC_K)
		{
			Print( "START ITEMS" );

			TStringArray configs = new TStringArray;
			configs.Insert( CFG_VEHICLESPATH );
			configs.Insert( CFG_WEAPONSPATH );
			configs.Insert( CFG_MAGAZINESPATH );

			for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
			{
				string strConfigPath = configs.Get( nConfig );

				int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

				for ( int nClass = 0; nClass < nClasses; ++nClass )
				{
					string strName;

					g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

					int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

					if ( scope != 2 )
					{
						continue;
					}

					if ( strName == "Mag_Scout_5Rnd") continue; // fix crash for this dumb item. dont spawn it

					if ( strName.Contains("Fx") )continue; // fix crash for this dumb item. dont spawn it

					if ( strName == "ItemOptics" ) 
					{
						continue; // Fix crash
					}

					if ( strName.Contains("Fx") ) 
					{
					//	continue; // Fix crash
					}

					Print( "" + strName );
				}
			}

			Print( "END ITEMS" );
		}
		*/
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