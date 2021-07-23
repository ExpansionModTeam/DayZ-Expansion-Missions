
static CarScript com_lastSpawnedCar;

class GameMenu extends PopupMenu
{
	TextListboxWidget m_gameScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_gameScriptButton;

	protected ref map< string, string > checkBoxMap = new map< string, string >; // store widget name

	string checkboxLayout = "$CurrentDir:\\Missions\\ExpansionCOM.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\CheckboxTemplate.layout";

	void GameMenu()
	{
	}

	void ~GameMenu()
	{
	}

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("game_checkbox_panel");
		m_gameScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("game_list_box"));
		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));

		m_gameScriptList.AddItem( "Spawn Hatchback",		new Param1<string>("SpawnHatchback"),					0 );
		m_gameScriptList.AddItem( "Spawn Gunter",			new Param1<string>("SpawnHatchback_02"),				0 );
		m_gameScriptList.AddItem( "Spawn Sedan", 			new Param1<string>("SpawnSedan"), 	 					0 );
		m_gameScriptList.AddItem( "Spawn Sarka", 			new Param1<string>("SpawnSedan_02"), 	 				0 );
		m_gameScriptList.AddItem( "Spawn M3S Covered",		new Param1<string>("SpawnTruck_01_Covered"), 			0 );
		//m_gameScriptList.AddItem( "Spawn M3S Chassis",		new Param1<string>("SpawnTruck_01_Chassis"), 			0 );
		//m_gameScriptList.AddItem( "Spawn M3S Cargo",		new Param1<string>("SpawnTruck_01_Cargo"), 				0 );
		//m_gameScriptList.AddItem( "Spawn M3S Command",		new Param1<string>("SpawnTruck_01_Command"), 				0 );
		//m_gameScriptList.AddItem( "Spawn Utility",			new Param1<string>("SpawnTruck_02"), 	  				0 );
		//m_gameScriptList.AddItem( "Spawn Van",				new Param1<string>("SpawnVan_01"), 	  					0 );
		m_gameScriptList.AddItem( "Spawn UAZ",				new Param1<string>("SpawnExpansionUAZ"),				0 );
		m_gameScriptList.AddItem( "Spawn UAZ Roofless",		new Param1<string>("SpawnExpansionUAZRoofless"),		0 );
		m_gameScriptList.AddItem( "Spawn UAZ Cargo Roofless",	new Param1<string>("SpawnExpansionUAZCargoRoofless"),	0 );
		m_gameScriptList.AddItem( "Spawn UAZ Cargo",			new Param1<string>("SpawnExpansionUAZCargo"),			0 );
		m_gameScriptList.AddItem( "Spawn Vodnik",			new Param1<string>("SpawnExpansionVodnik"),			0 );
		m_gameScriptList.AddItem( "Spawn Bus",			 	new Param1<string>("SpawnExpansionBus"),				0 );
		//! m_gameScriptList.AddItem( "Spawn Forklift",		new Param1<string>("SpawnExpansionForklift"),			0 );
		m_gameScriptList.AddItem( "Spawn Tractor",			new Param1<string>("SpawnExpansionTractor"),			0 );
		m_gameScriptList.AddItem( "Spawn Helicopter Merlin",			new Param1<string>("SpawnExpansionMerlin"),			0 );
		m_gameScriptList.AddItem( "Spawn Helicopter Mh6",				new Param1<string>("SpawnExpansionMh6"),				0 );
		m_gameScriptList.AddItem( "Spawn Helicopter Uh1h",				new Param1<string>("SpawnExpansionUh1h"),				0 );
		//m_gameScriptList.AddItem( "Spawn Helicopter Mi8",				new Param1<string>("SpawnExpansionMi8"),				0 );
		m_gameScriptList.AddItem( "Spawn Plane AN2",				new Param1<string>("SpawnExpansionAN2"),				0 );
		m_gameScriptList.AddItem( "Spawn Plane C130J",			new Param1<string>("SpawnExpansionC130J"),			0 );
		//! m_gameScriptList.AddItem( "Spawn Plane Su34",			new Param1<string>("SpawnExpansionSu34"),				0 );
		m_gameScriptList.AddItem( "Spawn Boat PBX",		new Param1<string>("SpawnExpansionZodiacBoat"),		0 );
		m_gameScriptList.AddItem( "Spawn Boat RHIB",		new Param1<string>("SpawnExpansionUtilityBoat"),		0 );
		//! m_gameScriptList.AddItem( "Spawn Boat Wheeled RHIB",	new Param1<string>("SpawnExpansionAmphibiaBoat"),		0 );

		CheckBoxWidget checkBoxGodmode = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxGodmode.SetName( "Godmode" );
		checkBoxGodmode.SetText( "Godmode" );

		checkBoxMap.Insert( checkBoxGodmode.GetName(), "ToggleGodMode" );
	}

	override void OnShow()
	{
		// Update checkbox checked
		UpdateCheckBoxes();
	}

	override void OnHide() 
	{
		
	}

	void UpdateCheckBoxes() 
	{
		foreach( string widgetName, string function : checkBoxMap ) 
		{
			Widget widget = m_checkboxPanel.FindWidget( widgetName );

			if ( widget.IsInherited( CheckBoxWidget )) 
			{
				CheckBoxWidget checkbox = CheckBoxWidget.Cast( widget );
				bool checked;
				GetGame().GameScript.CallFunction( this , function , checked, NULL );
				checkbox.SetChecked( checked );
			}
		}
	}

	void Update() 
	{
	}

	bool ToggleGodMode( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			m_comGodMode = !m_comGodMode;

			if ( m_comGodMode )
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "Godmode Toggle" ), new StringLocaliser( "Godmode enabled" ), EXPANSION_NOTIFICATION_ICON_SHIELD, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );
			} else {
				GetNotificationSystem().CreateNotification( new StringLocaliser( "Godmode Toggle" ), new StringLocaliser( "Godmode disabled" ), EXPANSION_NOTIFICATION_ICON_SHIELD, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );
			}

			GetPlayer().SetAllowDamage( !m_comGodMode );
		}
		return m_comGodMode;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		string param;
		Param1<string> param1;

		if ( w == m_gameScriptButton ) 
		{
			int selectRow = m_gameScriptList.GetSelectedRow();

			if ( selectRow == -1 ) return false;

			m_gameScriptList.GetItemData( selectRow, 0, param1 );

			if ( param1 ) 
			{
				GetGame().GameScript.CallFunction( this , param1.param1 , NULL, 0 );
			}
		}

		if ( w.IsInherited( CheckBoxWidget ) ) 
		{
			param = checkBoxMap.Get( w.GetName() );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param , NULL, CheckBoxWidget.Cast( w ) );
			}
		}

		return false;
	}

	static CarScript SpawnHatchback() 
	{
		TStringArray attArr = {
		"HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel",
		"CarBattery","CarRadiator","SparkPlug","HatchbackHood",
		"HatchbackTrunk","HatchbackDoors_Driver","HatchbackDoors_CoDriver",
		};

		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a ADA 4x4" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "OffroadHatchback", attArr );
	}

	static CarScript SpawnHatchback_02() 
	{
		TStringArray attArr = {
		"Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel",
		"CarBattery","CarRadiator","SparkPlug","Hatchback_02_Hood",
		"Hatchback_02_Trunk","Hatchback_02_Door_1_1",
		"Hatchback_02_Door_1_2","Hatchback_02_Door_2_1",
		"Hatchback_02_Door_2_2",
		};

		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Gunter" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Hatchback_02", attArr );
	}

	static CarScript SpawnSedan() 
	{
		TStringArray attArr = {
		"CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel",
		"CarBattery", "CarRadiator", "SparkPlug","CivSedanHood",
		"CivSedanTrunk", "CivSedanDoors_Driver",
		"CivSedanDoors_CoDriver", "CivSedanDoors_BackLeft", 
		"CivSedanDoors_BackRight",
		};

		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Olga" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "CivilianSedan", attArr );
	}

	static CarScript SpawnSedan_02() 
	{
		TStringArray attArr = {
		"Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel",
		"CarBattery","CarRadiator","SparkPlug","Sedan_02_Hood",
		"Sedan_02_Trunk","Sedan_02_Door_1_1",
		"Sedan_02_Door_1_2","Sedan_02_Door_2_1",
		"Sedan_02_Door_2_2",
		};

		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Sarka" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Sedan_02", attArr );
	}

	static CarScript SpawnTruck_01_Chassis() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","Truck_01_Hood","Truck_01_Door_1_1",
		"Truck_01_Door_2_1",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a M3S Chassis" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Truck_01_Chassis", attArr );
	}

	static CarScript SpawnTruck_01_Cargo() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","Truck_01_Hood","Truck_01_Door_1_1",
		"Truck_01_Door_2_1",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a M3S Cargo" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Truck_01_Cargo", attArr );
	}

	static CarScript SpawnTruck_01_Covered() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","Truck_01_Hood","Truck_01_Door_1_1",
		"Truck_01_Door_2_1",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a M3S Covered" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Truck_01_Covered", attArr );
	}

	static CarScript SpawnTruck_01_Command() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","Truck_01_Hood","Truck_01_Door_1_1",
		"Truck_01_Door_2_1",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a M3S Command" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Truck_01_Command", attArr );
	}

	static CarScript SpawnTruck_02() 
	{
		TStringArray attArr = {
		"Truck_02_Wheel","Truck_02_Wheel", "Truck_02_Wheel","Truck_02_Wheel",
		"TruckBattery","GlowPlug",
		"Truck_02_Door_1_1","Truck_02_Door_2_1",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Compact Truck" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Truck_02", attArr );
	}

	static CarScript SpawnExpansionMerlin()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionMerlinFrontWheel");
		attArr.Insert("ExpansionMerlinFrontWheel");
		attArr.Insert("ExpansionMerlinBackWheel");
		attArr.Insert("ExpansionMerlinBackWheel");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Merlin Helicopter" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionMerlin", attArr );
	}

	static CarScript SpawnExpansionMi8()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionMi8FrontWheel");
		attArr.Insert("ExpansionMi8FrontWheel");
		attArr.Insert("ExpansionMi8BackWheel");
		attArr.Insert("ExpansionMi8BackWheel");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Mi8 Helicopter" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionMi8", attArr );

	}

	static CarScript SpawnExpansionMh6()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Expansion_Mh6_Door_1_1");
		attArr.Insert("Expansion_Mh6_Door_2_1");
		attArr.Insert("Expansion_Mh6_Door_1_2");
		attArr.Insert("Expansion_Mh6_Door_2_2");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Mh6 Helicopter" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionMh6", attArr );
	}

	static CarScript SpawnExpansionUh1h()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUh1hDoor_1_1");
		attArr.Insert("ExpansionUh1hDoor_1_2");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Uh1h Helicopter" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUh1h", attArr );
	}

	static CarScript SpawnExpansionQuad()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Quad" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionQuad", attArr );
	}

	static CarScript SpawnExpansionUAZ()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a UAZ" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUAZ", attArr );
	}

	static CarScript SpawnExpansionUAZRoofless()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a UAZ Roofless" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUAZRoofless", attArr );
	}

	static CarScript SpawnExpansionUAZCargoRoofless()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a UAZ Cargo Roofless" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUAZCargoRoofless", attArr );
	}

	static CarScript SpawnExpansionUAZCargo()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a UAZ Cargo" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUAZCargo", attArr );
	}

	static CarScript SpawnExpansionForklift()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Forklift" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionForklift", attArr );
	}

	static CarScript SpawnExpansionTractor()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("ExpansionTractorDoorsDriver");
		attArr.Insert("ExpansionTractorDoorsCodriver");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Tractor" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionTractor", attArr );
	}

	static CarScript SpawnExpansionVodnik()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionVodnikDoorDriver");
		attArr.Insert("ExpansionVodnikDoorCoDriver");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Vodnik" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionVodnik", attArr );
	}

	static CarScript SpawnExpansionBus( vector position = "0 0 0" )
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionBusWheel");
		attArr.Insert("ExpansionBusWheel");
		attArr.Insert("ExpansionBusWheelDouble");
		attArr.Insert("ExpansionBusWheelDouble");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a (Expansion) Bus" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionBus", attArr, position );
	}

	static CarScript SpawnExpansionZodiacBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a PBX Boat" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionZodiacBoat", attArr );
	}

	static CarScript SpawnExpansionUtilityBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a RHIB Boat" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionUtilityBoat", attArr );
	}

	static CarScript SpawnExpansionAmphibiaBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Amphibia Boat" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionAmphibiaBoat", attArr );
	}

	static CarScript SpawnExpansionSu34()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheelSmall");
		attArr.Insert("ExpansionAnWheelSmall");
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Su34 Plane" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionSu34", attArr );
	}

	static CarScript SpawnExpansionAN2()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheelSmall");
		attArr.Insert("ExpansionAnWheelSmall");
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a An2 Plane" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionAn2", attArr );
	}

	static CarScript SpawnExpansionC130J()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a C130J Plane" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "ExpansionC130J", attArr );
	}

	static CarScript SpawnBus() 
	{
		TStringArray attArr = {
		"TransitBusWheel","TransitBusWheel", "TransitBusWheelDouble","TransitBusWheelDouble",
		"TruckBattery","GlowPlug","BusHood",
		"BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Transit Bus" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "TransitBus", attArr );
	}

	static CarScript SpawnVan_01() 
	{
		TStringArray attArr = {
			"Van_01_Wheel","Van_01_Wheel","Van_01_Wheel","Van_01_Wheel",
			"CarBattery","CarRadiator","SparkPlug",
			"Van_01_Door_1_1","Van_01_Door_2_1","Van_01_Door_2_2",
			"Van_01_Trunk_1", "Van_01_Trunk_2",
		};
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Vehicle Spawner" ), new StringLocaliser( "Spawned a Blue Van" ), EXPANSION_NOTIFICATION_ICON_CAR, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

		return SpawnVehicle( "Van_01", attArr );
	}

	static CarScript SpawnVehicle( string vehicle, TStringArray attachments, vector position = "0 0 0" ) 
	{
		if ( position == "0 0 0" )
			position = GetCursorPos();
		com_lastSpawnedCar = CarScript.Cast( GetGame().CreateObject( vehicle, position, false, false ) );

		for (int j = 0; j < attachments.Count(); j++) { com_lastSpawnedCar.GetInventory().CreateAttachment( attachments.Get(j) ); }

		com_lastSpawnedCar.Fill( CarFluid.FUEL, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.OIL, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.BRAKE, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.COOLANT, 1000 );

		//com_lastSpawnedCar.EngineStart();
		//com_lastSpawnedCar.ToggleHeadlights();

		return com_lastSpawnedCar;
	}
}