class COM_ObjectMenu extends PopupMenu
{
	protected TextListboxWidget m_classList;
	protected EditBoxWidget m_SearchBox;
	protected ButtonWidget m_btnSpawnGround;
	protected ButtonWidget m_btnSpawnCursor;
	protected ButtonWidget m_btnSpawnInventory;
	protected ButtonWidget m_btnCancel;
	protected EditBoxWidget m_QuantityItem;
	protected ButtonWidget m_btnDOJToggle;
	protected ButtonWidget m_btnDOJPrecision;

	private ItemPreviewWidget m_item_widget;
	protected EntityAI previewItem;
	private int m_characterRotationX;
	private int m_characterRotationY; // Borrowed from inspectmenu
	private int m_characterScaleDelta;
	private vector m_characterOrientation;

	protected EditBoxWidget m_editBox;
	
	void COM_ObjectMenu()
	{
	}

	void ~COM_ObjectMenu()
	{
		if ( previewItem ) 
		{
			GetGame().ObjectDelete( previewItem );
		}
	}

	void Init()
	{
		m_classList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "classlist" ) );
		m_SearchBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "search_input" ) );
		m_btnSpawnGround = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_ground" ) );
		m_btnSpawnCursor = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_cursorpos" ) );
		m_btnSpawnInventory = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_inventory" ) );
		m_btnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );
		m_btnDOJToggle = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_doj_toggle" ) );
		m_btnDOJPrecision = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_doj_precision" ) );

		m_QuantityItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "quantity_items" ) );
		
		m_editBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("className_spawner_box") );
	}

	override void OnShow()
	{
		UpdateList( "All" );
	}

	override void OnHide() 
	{
		if ( previewItem ) 
		{
			GetGame().ObjectDelete( previewItem );
		}
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( w == m_SearchBox )
		{
			UpdateList( "All" );
			return true;
		}

		return false;
	}

	bool OnMouseEnter( Widget w , int x, int y ) 
	{
		if ( w == m_SearchBox ) 
		{
			GetPlayer().GetInputController().OverrideMovementSpeed( true, 0 );
		}
		return false;
	}

	bool OnMouseLeave( Widget w, Widget enterW, int x, int y ) 
	{
		if ( w == m_SearchBox ) 
		{
			GetPlayer().GetInputController().OverrideMovementSpeed( false, 0 );
		}
		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		string strSelection = GetCurrentSelection();
		bool ai = false;

		int quantity = 0;
		string text = "";
		ItemBase oItem = NULL;
		
		if ( strSelection == "" ) 
		{
			strSelection = GetEditBoxInput();
		}

		if( strSelection != "" )
		{
			strSelection.ToLower();
			ObjectEditor obEditor = ObjectEditor.Cast( GetCOMModuleManager().GetModule( ObjectEditor ) );

			if ( GetGame().IsKindOf( strSelection, "DZ_LightAI" ) ) 
			{
				ai = true;
			}

			if( w == m_btnSpawnCursor )
			{
				EntityAI oCursorObj = EntityAI.Cast( g_Game.CreateObject( strSelection, GetCursorPos(), true, ai ) );
				obEditor.addObject( oCursorObj );
				ObjectInfoMenu.listBox.AddItem(oCursorObj.GetType(), oCursorObj, 0);

				if ( oCursorObj.IsInherited( ItemBase ) )
				{
					oItem = ItemBase.Cast( oCursorObj );
					SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );

					quantity = 0;
					text = m_QuantityItem.GetText();
					text.ToUpper();

					if (text == "MAX")
					{
						quantity = oItem.GetQuantityMax();
					} else
					{
						quantity = text.ToInt();
					}
					oItem.SetQuantity(quantity);

					return true;
				}
				oCursorObj.PlaceOnSurface();
				float random_pitch = Math.RandomFloat(1, 359);
				oCursorObj.SetOrientation( Vector(random_pitch, 0, 0) );
			}
			else if ( w == m_btnSpawnGround )
			{
				EntityAI oObj = EntityAI.Cast( g_Game.CreateObject( strSelection, GetGame().GetPlayer().GetPosition(), false, ai ) );
 				obEditor.addObject( oObj );
			 	ObjectInfoMenu.listBox.AddItem(oObj.GetType(), oObj, 0);

				if ( oObj.IsInherited( ItemBase ) )
				{
					oItem = ItemBase.Cast( oObj );
					SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );
					
					quantity = 0;
					text = m_QuantityItem.GetText();
					text.ToUpper();
					
					if (text == "MAX")
					{
						quantity = oItem.GetQuantityMax();
					} else
					{
						quantity = text.ToInt();
					}
					oItem.SetQuantity(quantity);

					return true;
				}
				oObj.PlaceOnSurface();
			}
			else if ( w == m_btnSpawnInventory )
			{
				EntityAI oInvItem = g_Game.GetPlayer().GetInventory().CreateInInventory( strSelection );

				oInvItem.SetHealth( oInvItem.GetMaxHealth() );

				if ( oInvItem.IsInherited( ItemBase ) )
				{
					oItem = ItemBase.Cast( oObj );
					SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );

					quantity = 0;
					text = m_QuantityItem.GetText();
					text.ToUpper();
					
					if (text == "MAX")
					{
						quantity = oItem.GetQuantityMax();
					} else
					{
						quantity = text.ToInt();
					}
					oItem.SetQuantity(quantity);
				}
				return true;
			}
		}

		if ( w.GetName().Contains( "btn_filter" ) ) 
		{
			string buttonName = w.GetName();
			buttonName.Replace("btn_filter_", "");
			UpdateList( buttonName );

			return true;
		}
		
		if ( w == m_btnDOJToggle)
		{
			COMEditorMenu menu = COMEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
			if ( menu )
			{
				if ( !menu.GetDOJBoolState() )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "Randomized Rotation Toggle" ), new StringLocaliser( "Randomized Rotation mode enabled" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );

					menu.SetDOJBool( true );
				}
				else
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "Randomized Rotation Toggle" ), new StringLocaliser( "Randomized Rotation mode disabled" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );

					menu.SetDOJBool( false );
				}
			}
			
			return true;
		}

		if ( w == m_btnDOJPrecision)
		{
			COMEditorMenu menu2 = COMEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );
			if ( menu2 )
			{
				if ( !menu2.GetDOJPrecision() )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "Precision Toggle" ), new StringLocaliser( "Precision mode enabled" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );

					menu2.SetDOJPrecision( true );
				}
				else 
				{					
					GetNotificationSystem().CreateNotification( new StringLocaliser( "Precision Toggle" ), new StringLocaliser( "Precision mode disabled" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );

					menu2.SetDOJPrecision( false );
				}
			}
			
			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w == m_classList ) 
		{
			string strSelection = GetCurrentSelection();
			m_characterOrientation = vector.Zero;

			if ( !m_item_widget )
			{
				Widget preview_panel = layoutRoot.FindAnyWidget("preview_pnl");

				if ( preview_panel ) 
				{
					float width;
					float height;
					preview_panel.GetSize(width, height);

					m_item_widget = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_panel) );
				}
			}

			if ( previewItem ) 
			{
				GetGame().ObjectDelete( previewItem );
			}

			if ( CheckItemCrash(strSelection) )
				return false;

			previewItem = EntityAI.Cast( GetGame().CreateObject( strSelection, vector.Zero, false ) );

			m_item_widget.SetItem( previewItem );
			m_item_widget.SetModelPosition( Vector(0,0,0.5) );
			m_item_widget.SetModelOrientation( m_characterOrientation );

			float itemx, itemy;		
			m_item_widget.GetPos(itemx, itemy);

			m_item_widget.SetSize( 1.5, 1.5 );

			// align to center 
			m_item_widget.SetPos( -0.225, -0.225 );
		}

		return true;
	}
	
	private static ref array< string > items =
	{
		"quickiebow",
		"recurvebow",
		"gp25base",
		"gp25",
		"gp25_standalone",
		"m203base",
		"m203",
		"m203_standalone",
		"red9",
		"pvcbow",
		"crossbow",
		"augsteyr",
		"m249",
		"undersluggrenadem4",
		"groza",
		"pm73rak",
		"trumpet",
		"lawbase",
		"law",
		"rpg7base",
		"rpg7",
		"dartgun",
		"shockpistol",
		"shockpistol_black",
		"derringer_black",
		"derringer_pink",
		"derringer_grey",
		"fnx45_arrow",
		"longhorn",
		"p1",
		"makarovpb",
		"mp133shotgun_pistolgrip",

		"largetentbackpack",
		"splint_applied",
		"leatherbelt_natural",
		"leatherbelt_beige",
		"leatherbelt_brown",
		"leatherbelt_black",
		"leatherknifeshealth",

		"itemoptics",
		"fx",
		"expansionsoundproxybase",
		"expansionvehiclebasesoundproxybase",
		"expansionhelicopterscript",
		"expansionvodnikaudio",
		"expansionlhdaudio",
		"expansionutilityaudio",
		"expansionzodiacaudio",
		"expansiongyrocopteraudio",
		"expansionmerlinaudio",
		"expansionmh6audio",
		"expansionuh1haudio",
		"expansionvehicleplanebase",
		"expansionvehiclehelicopterbase",
		"expansionvehicleboatbase",
		"expansionvehiclebikebase",
		"expansionvehiclecarbase",
		"expansionvehiclevehiclebase"
	};

	private static ref array< string > itemsList =
	{
		"placing",
		"debug",
		"fx",
		"proxy",
		"audio"
	};

	private bool CheckItemCrash( string name )
	{
		if ( items.Find( name ) > -1 )
		{
			return true;
		}

		for ( int i = 0; i < itemsList.Count(); ++i )
		{
			if ( name.Contains( itemsList[i] ) )
			{
				return true;
			}
		}
		return false;
	}

	override bool OnMouseButtonDown( Widget w, int x, int y, int button ) 
	{
		if (w == m_item_widget)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_characterRotationX, m_characterRotationY);
			return true;
		}
		return false;
	}

	override bool OnMouseWheel( Widget w, int x, int y, int wheel ) 
	{
		if ( w == m_item_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}

	void UpdateScale(int mouse_x, int mouse_y, int wheel, bool is_dragging) // Borrowed from inspect menu
	{
		float w, h, x, y;		
		m_item_widget.GetPos(x, y);
		m_item_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 4);
		h = h + ( m_characterScaleDelta / 4 );
		if ( w > 0.5 && w < 4 )
		{
			m_item_widget.SetSize( w, h );
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - ( m_characterScaleDelta / 8 );
			float new_y = y - ( m_characterScaleDelta / 8 );
			m_item_widget.SetPos( new_x, new_y );
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging) // Borrowed from inspect menu
	{
		vector o = m_characterOrientation;
		o[0] = o[0] + (m_characterRotationY - mouse_y);
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		
		m_item_widget.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_characterOrientation = o;
		}
	}

	void UpdateList( string classFilter ) // All default
	{
		m_classList.ClearItems();

		TStringArray configs = new TStringArray;
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

		string strSearch = m_SearchBox.GetText();

		strSearch.ToLower();

		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
		{
			string strConfigPath = configs.Get( nConfig );

			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

			for ( int nClass = 0; nClass < nClasses; ++nClass )
			{
				string strName;

				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

				string strNameLower = strName;

				strNameLower.ToLower();

				if ( GetGame().IsKindOf( strNameLower, classFilter ) || classFilter == "All" ) // Fix for weapon_base not being child of "All"
				{
					if ( (strSearch != "" && (!strNameLower.Contains( strSearch ))) ) 
					{
						continue;
					}

					if ( CheckItemCrash(strNameLower) )
						continue;

					m_classList.AddItem( strName, NULL, 0 );
				}
			}
		}
	}

	string GetCurrentSelection()
	{
		if ( m_classList.GetSelectedRow() != -1 )
		{
			string result;
			m_classList.GetItemText( m_classList.GetSelectedRow(), 0, result );
			return result;
		}

		return "";
	}
	
	string GetEditBoxInput() 
	{
		return m_editBox.GetText();
	}
}

// DZ_LightAI
// House?
// Transport
// Weapon_Base
// Edible_Base
// Clothing_Base