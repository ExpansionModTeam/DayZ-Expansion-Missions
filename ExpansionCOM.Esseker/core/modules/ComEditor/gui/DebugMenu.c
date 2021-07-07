class DebugMenu extends PopupMenu
{
	TextListboxWidget m_debugScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_debugScriptButton;

	ref array<string> IconArr;
	ref array<int> ColorArr;
	ref array<string> ColorNameArr;

	//! Locator
	protected autoptr ExpansionLocatorUI m_UICallback;

	//! Markers
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;

	void DebugMenu()
	{
		m_UICallback = null;

		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );

		
		IconArr = new ref array<string>;
		IconArr.Insert("Arrow");
		IconArr.Insert("Error");
		IconArr.Insert("Airdrop");
		IconArr.Insert("Car");
		IconArr.Insert("Deliver");
		IconArr.Insert("Radiation");
		IconArr.Insert("Trader");
		IconArr.Insert("Water 1");
		IconArr.Insert("Water 2");
		IconArr.Insert("Infected 1");
		IconArr.Insert("Infected 2");
		IconArr.Insert("Skull 1");
		IconArr.Insert("Skull 2");
		IconArr.Insert("Skull 3");
		IconArr.Insert("Helicopter");
		IconArr.Insert("Base");
		IconArr.Insert("Boat");
		IconArr.Insert("Fishing");
		IconArr.Insert("Territory");
		IconArr.Insert("Bear");
		IconArr.Insert("Claw");
		IconArr.Insert("Drip");
		IconArr.Insert("Ear");
		IconArr.Insert("Eye");
		IconArr.Insert("Fireplace");
		IconArr.Insert("Heart");
		IconArr.Insert("Hook");
		IconArr.Insert("Info");
		IconArr.Insert("Knife");
		IconArr.Insert("Map");
		IconArr.Insert("Marker");
		IconArr.Insert("Map Marker");
		IconArr.Insert("Orientation");
		IconArr.Insert("Menu");
		IconArr.Insert("Pen");
		IconArr.Insert("Persona");
		IconArr.Insert("Pill");
		IconArr.Insert("Questionmark");
		IconArr.Insert("Moon");
		IconArr.Insert("Star");
		IconArr.Insert("Sun");
		IconArr.Insert("Ellipse");
		IconArr.Insert("Tent");
		IconArr.Insert("Thermometer");
		IconArr.Insert("Book 1");
		IconArr.Insert("Book 2");
		IconArr.Insert("Note");
		IconArr.Insert("Grab");
		IconArr.Insert("Open Hand");
		IconArr.Insert("Radio");
		IconArr.Insert("Shield");
		IconArr.Insert("Snow");
		IconArr.Insert("Group");
		IconArr.Insert("Vehicle Crash");
		IconArr.Insert("Animal Skull");
		IconArr.Insert("Apple");
		IconArr.Insert("Apple Core");
		IconArr.Insert("Arrows");
		IconArr.Insert("Axe");
		IconArr.Insert("Backpack");
		IconArr.Insert("Bandage");
		IconArr.Insert("Batteries");
		IconArr.Insert("Berries");
		IconArr.Insert("Kitchen Knife Big");
		IconArr.Insert("Binoculars");
		IconArr.Insert("Bolt");
		IconArr.Insert("Bonfire");
		IconArr.Insert("Bottle");
		IconArr.Insert("Bow");
		IconArr.Insert("Broken Lighter");
		IconArr.Insert("Can Of Beans Big");
		IconArr.Insert("Can Of Beans Small");
		IconArr.Insert("Car Keys");
		IconArr.Insert("Carrot");
		IconArr.Insert("Chain Saw");
		IconArr.Insert("Chicken");
		IconArr.Insert("Chocolate");
		IconArr.Insert("Cigarets");
		IconArr.Insert("Cloth");
		IconArr.Insert("Compass");
		IconArr.Insert("Corn");
		IconArr.Insert("Crowbar");
		IconArr.Insert("Cow");
		IconArr.Insert("Dinosaur Skull");
		IconArr.Insert("Dry Wood");
		IconArr.Insert("Eatable Flowers");
		IconArr.Insert("Electrical Tape");
		IconArr.Insert("Empty Can");
		IconArr.Insert("Fish");
		IconArr.Insert("Flare");
		IconArr.Insert("Flare Gun");
		IconArr.Insert("Flare Gun Ammo");
		IconArr.Insert("Flashlight");
		IconArr.Insert("Fox");
		IconArr.Insert("Frying Pan");
		IconArr.Insert("Gas");
		IconArr.Insert("Gas Mask");
		IconArr.Insert("Golf Club");
		IconArr.Insert("Goose");
		IconArr.Insert("Grenade");
		IconArr.Insert("Guitar");
		IconArr.Insert("Gun");
		IconArr.Insert("Gun Bullets");
		IconArr.Insert("Hammer");
		IconArr.Insert("Herbal Medicine");
		IconArr.Insert("Home Made Grenade");
		IconArr.Insert("Human Skull");
		IconArr.Insert("Insect");
		IconArr.Insert("Kitchen Knife");
		IconArr.Insert("Ladder");
		IconArr.Insert("Lantern");
		IconArr.Insert("Lighter");
		IconArr.Insert("Machette");
		IconArr.Insert("Paper Map");
		IconArr.Insert("Matches");
		IconArr.Insert("Medic Box");
		IconArr.Insert("Mushrooms");
		IconArr.Insert("Nails");
		IconArr.Insert("Paper");
		IconArr.Insert("Pills");
		IconArr.Insert("Pipe Wrench");
		IconArr.Insert("Powder");
		IconArr.Insert("Pumpkin");
		IconArr.Insert("Rabbit");
		IconArr.Insert("Raccon");
		IconArr.Insert("Radio");
		IconArr.Insert("Rat");
		IconArr.Insert("Rock 1");
		IconArr.Insert("Rock 2");
		IconArr.Insert("Rope");
		IconArr.Insert("Saw");
		IconArr.Insert("Scrap Metal");
		IconArr.Insert("Screwdriver");
		IconArr.Insert("Shotgun");
		IconArr.Insert("Shotgun Bullets");
		IconArr.Insert("Shovel");
		IconArr.Insert("Soda");
		IconArr.Insert("Tent Small");
		IconArr.Insert("Walkie Talkie");
		IconArr.Insert("Water Jug");
		IconArr.Insert("Wild Pork");
		IconArr.Insert("Worms");

		ColorArr = new ref array<int>;
		ColorArr.Insert(COLOR_EXPANSION_ITEM_HIGHLIGHT_TEXT);
		ColorArr.Insert(COLOR_EXPANSION_ITEM_NORMAL_TEXT);
		ColorArr.Insert(COLOR_EXPANSION_ITEM_HIGHLIGHT_ELEMENT);
		ColorArr.Insert(COLOR_EXPANSION_ITEM_NORMAL_ELEMENT);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_INFO);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_ERROR);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_ORANGE);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_ASPHALT);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_AMETHYST);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_TURQUOISE);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_EXPANSION);
		ColorArr.Insert(COLOR_EXPANSION_NOTIFICATION_MISSION);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().SystemChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().AdminChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().GlobalChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().DirectChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().TransportChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().PartyChatColor);
		ColorArr.Insert(GetExpansionSettings().GetGeneral().TransmitterChatColor);

		ColorNameArr = new ref array<string>;
		ColorNameArr.Insert("ITEM_HIGHLIGHT_TEXT");
		ColorNameArr.Insert("ITEM_NORMAL_TEXT");
		ColorNameArr.Insert("ITEM_HIGHLIGHT_ELEMENT");
		ColorNameArr.Insert("ITEM_NORMAL_ELEMENT");
		ColorNameArr.Insert("NOTIFICATION_INFO");
		ColorNameArr.Insert("NOTIFICATION_ERROR");
		ColorNameArr.Insert("NOTIFICATION_SUCCSESS");
		ColorNameArr.Insert("NOTIFICATION_ORANGE");
		ColorNameArr.Insert("NOTIFICATION_ASPHALT");
		ColorNameArr.Insert("NOTIFICATION_AMETHYST");
		ColorNameArr.Insert("NOTIFICATION_TURQUOISE");
		ColorNameArr.Insert("NOTIFICATION_ORANGEVILLE");
		ColorNameArr.Insert("NOTIFICATION_EXPANSION");
		ColorNameArr.Insert("NOTIFICATION_MISSION");
		ColorNameArr.Insert("DEFAULT_COLOUR");
		ColorNameArr.Insert("GAME_TEXT_COLOUR");
		ColorNameArr.Insert("ADMIN_TEXT_COLOUR");
		ColorNameArr.Insert("DIRECT_TEXT_COLOUR");
		ColorNameArr.Insert("GLOBAL_TEXT_COLOUR");
		ColorNameArr.Insert("PAS_TEXT_COLOUR");
		ColorNameArr.Insert("TRANSPORT_COLOUR");
		ColorNameArr.Insert("TEAM_TEXT_COLOUR");
	}

	void ~DebugMenu()
	{
		delete ColorArr;
		delete ColorNameArr;
		delete IconArr;
	}

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("debug_checkbox_panel");
		m_debugScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("debug_list_box"));
		m_debugScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("debug_script_button"));

		m_debugScriptList.AddItem( "Random Notification",							new Param1<string>("ExecEXPNotification"),	0 );

		m_debugScriptList.AddItem( "Locator Title",									new Param1<string>("ExecEXPLocator1"),		0 );
		m_debugScriptList.AddItem( "Locator Title/Desc",							new Param1<string>("ExecEXPLocator2"),		0 );
		m_debugScriptList.AddItem( "Locator Title/Desc/Time",						new Param1<string>("ExecEXPLocator3"),		0 );
		m_debugScriptList.AddItem( "Locator Title/Desc/Time/FadeIn",				new Param1<string>("ExecEXPLocator4"),		0 );
		m_debugScriptList.AddItem( "Locator Title/Desc/Time/FadeIn/FadeOut",		new Param1<string>("ExecEXPLocator5"),		0 );
		
		m_debugScriptList.AddItem( "Random Local Chat Msg",							new Param1<string>("ExecEXPLocalChat"),		0 );
		m_debugScriptList.AddItem( "Random Global Chat Msg",						new Param1<string>("ExecEXPGlobalChat"),	0 );
		m_debugScriptList.AddItem( "Random Admin Chat Msg",							new Param1<string>("ExecEXPAdminChat"),		0 );
		m_debugScriptList.AddItem( "Random Game Chat Msg",							new Param1<string>("ExecEXPGameChat"),		0 );

		m_debugScriptList.AddItem( "Random Server Marker",							new Param1<string>("ExecEXPServerMarker"),	0 );
	}

	override void OnShow()
	{
		m_UICallback = new ExpansionLocatorUI();
	}

	override void OnHide() 
	{
		
	}

	void Update() 
	{

	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		string param;
		Param1<string> param1;

		if ( w == m_debugScriptButton ) 
		{
			int selectRow = m_debugScriptList.GetSelectedRow();

			if ( selectRow == -1 ) return false;

			m_debugScriptList.GetItemData( selectRow, 0, param1 );

			if ( param1 ) 
			{
				GetGame().GameScript.CallFunction( this , param1.param1 , NULL, 0 );
			}
		}

		return false;
	}

	void ExecEXPNotification() 
	{
		string SelectedIcon = IconArr.GetRandomElement();
		int id = Math.RandomInt( 0, ColorArr.Count() );
		int SelectedColor = ColorArr[id];
		string SelectedColorName = ColorNameArr[id];

		GetNotificationSystem().CreateNotification( new StringLocaliser( SelectedIcon ), new StringLocaliser( SelectedColorName ), ExpansionIcons.GetPath(SelectedIcon), SelectedColor, 7 );

		//GetNotificationSystem().CreateNotification( new StringLocaliser( "My Title" ), new StringLocaliser( "My Description" ), MyIcon, MyARGBColor, time );
	}

	void ExecEXPLocator1() 
	{
		m_UICallback.OnShowCityClient("Name to display");
	}

	void ExecEXPLocator2() 
	{
		m_UICallback.OnShowCityClient("Name to display","Description to display");
	}

	void ExecEXPLocator3() 
	{
		m_UICallback.OnShowCityClient("Name to display","Stay on screen for 2 seconds", 2000);
	}

	void ExecEXPLocator4() 
	{
		m_UICallback.OnShowCityClient("Name to display","FadeIn very quickly", 5000, 0.25);
	}

	void ExecEXPLocator5() 
	{
		m_UICallback.OnShowCityClient("Name to display","Slow FadeIn, Fast FadeOut, stay longer on screen", 6000, 0.015, 0.05);
	}

	void ExecEXPLocalChat() 
	{
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCDirect, "", "My Local Message !", "" ) );
	}

	void ExecEXPGlobalChat() 
	{
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCGlobal, "", "My Global Message !", "" ) );
	}

	void ExecEXPAdminChat() 
	{
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCAdmin, "", "My Admin Message !", "" ) );
	}

	void ExecEXPGameChat() 
	{
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCSystem, "", "My Game Message !", "" ) );
	}

	void ExecEXPServerMarker() 
	{
		if ( !m_MarkerModule )
			return;

		string SelectedIcon = IconArr.GetRandomElement();
		int id = Math.RandomInt( 0, ColorArr.Count() );
		int SelectedColor = ColorArr[id];
		string SelectedColorName = ColorNameArr[id];

		m_ServerMarker = m_MarkerModule.CreateServerMarker( SelectedIcon, ExpansionIcons.GetPath(SelectedIcon), GetGame().GetPlayer().GetPosition(), SelectedColor, true );

		// m_MarkerModule.CreateServerMarker( "My Name", MyIconName, GetPosition(), MyARGBIcon, Is_3DEnabled );
	}
}