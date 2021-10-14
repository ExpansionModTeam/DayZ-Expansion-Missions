class ObjectInfoMenu extends PopupMenu
{
	static EditBoxWidget infoPosX;
	static EditBoxWidget infoPosY;
	static EditBoxWidget infoPosZ;

	static EditBoxWidget infoPosYaw;
	static EditBoxWidget infoPosPitch;
	static EditBoxWidget infoPosRoll;

	static TextListboxWidget listBox;

	private ref COMWidgetStore widgetStore;

	void ObjectInfoMenu()
	{
	}

	void ~ObjectInfoMenu()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Remove( Update );
	}

	static void UpdateObjectList()
	{
		listBox.ClearItems();

		ref array<Object> objects = ((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).m_Objects;

		foreach( Object obj : objects )
		{
			listBox.AddItem(obj.GetType(), obj, 0); // store object ref into list?
		}
	}

	Object GetSelectedRowObject()
	{
		if ( listBox.GetSelectedRow() != -1 )
		{
			Object result;
			listBox.GetItemData( listBox.GetSelectedRow(), 0, result );
			return result;
		}

		return NULL;
	}

	override void Init( )
	{

		widgetStore = new COMWidgetStore( layoutRoot );

		listBox 		= widgetStore.GetListboxWidget("object_editor_info_list");

		infoPosX 		= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_pos_x" ) );
		infoPosY 		= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_pos_y" ) );
		infoPosZ 		= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_pos_z" ) );
		infoPosYaw 		= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_yaw_input" ) );
		infoPosPitch 	= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_pitch_input" ) );
		infoPosRoll 	= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "object_info_roll_input" ) );
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w.GetName() == "object_editor_info_list")
		{
			Object selected = GetSelectedRowObject();
			if ( selected )
			{
				((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).SelectObject( selected );
			}
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w.GetName() == "object_editor_info_export")
		{
			((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).ExportScene();
			GetNotificationSystem().CreateNotification( new StringLocaliser( "Object Editor" ), new StringLocaliser( "Copied data to clipboard." ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );
		}
		if ( w.GetName() == "object_editor_info_save")
		{
			((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).SaveScene();
			GetNotificationSystem().CreateNotification( new StringLocaliser( "Object Editor" ), new StringLocaliser( "Saved scene." ), EXPANSION_NOTIFICATION_ICON_HOME, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );
		}
		if ( w.GetName() == "object_editor_info_clear")
		{
			ref array< Object> objects = ((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).m_Objects;
			GetNotificationSystem().CreateNotification( new StringLocaliser( "Object Editor" ), new StringLocaliser( "Cleared all objects from current scene." ), EXPANSION_NOTIFICATION_ICON_T_Empty_Can, COLOR_EXPANSION_NOTIFICATION_INFO, 7 );

			foreach( Object obj : objects )
			{
				GetGame().ObjectDelete( obj );
			}
			objects.Clear();
			UpdateObjectList();
		}
		if ( w.GetName() == "object_editor_info_dumplods" )
		{
			string toCopy = "";
			array<LOD> lods = new array<LOD>;
			Object object = ((ObjectEditor) GetCOMModuleManager().GetModule(ObjectEditor)).m_SelectedObject;
			object.GetLODS(lods);

			foreach( LOD lod : lods )
			{
				toCopy = toCopy + object.GetLODName( lod ) + "\n";
				array<Selection> selections = new array<Selection>;
				lod.GetSelections( selections );
				foreach ( Selection selection : selections )
				{
					toCopy = toCopy + " " + selection.GetName() + "\n";
				}
			}
			GetGame().CopyToClipboard(toCopy);
			// Message( "Dumped LODs to clipboard");
		}
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if ( !GetSelectedObject() )
		{
			return false;
		}
		vector orientation = GetSelectedObject().GetOrientation();
		vector position = GetSelectedObjectPosition();

		bool up = wheel < 0;
		int value = 1;

		COMEditorMenu editor_menu = COMEditorMenu.Cast( GetGame().GetUIManager().GetMenu() );

		if ( up ) value = -1;

		if ( w == infoPosYaw )
		{
			orientation[0] = orientation[0] + value;

			if( Math.AbsFloat( orientation[0] ) < 0.001 )
			{
				orientation[0] = 0;
			}

			GetSelectedObject().SetOrientation( orientation );
			infoPosYaw.SetText( orientation[0].ToString() );
		}
		if ( w == infoPosPitch )
		{
			if ( orientation[0] > 0 ) // seemless pitch change
			{
				value = -value;
			}

			orientation[1] = orientation[1] + value;

			if( Math.AbsFloat( orientation[1] ) < 0.001 )
			{
				orientation[1] = 0;
			}

			GetSelectedObject().SetOrientation( orientation );
			infoPosPitch.SetText( orientation[1].ToString() );

		}
		if ( w == infoPosRoll )
		{
			orientation[2] = orientation[2] + value;

			if( Math.AbsFloat( orientation[2] ) < 0.001 )
			{
				orientation[2] = 0;
			}

			GetSelectedObject().SetOrientation( orientation );
			infoPosRoll.SetText( orientation[2].ToString() );
		}
		if ( w == infoPosY )
		{
			if ( editor_menu && editor_menu.GetDOJPrecision() )
			{
				position[1] = position[1] + value * 0.001;
			}
			else
			{
				position[1] = position[1] + value * 0.05;
			}
			SetSelectedObjectTransform( position, GetSelectedObject().GetOrientation() );
			infoPosY.SetText( position[1].ToString() );
		}
		if ( w == infoPosX )
		{
			if ( editor_menu && editor_menu.GetDOJPrecision() )
			{
				position[0] = position[0] + value * 0.001;
			}
			else
			{
				position[0] = position[0] + value * 0.05;
			}

			SetSelectedObjectTransform( position, GetSelectedObject().GetOrientation() );
			infoPosX.SetText( position[0].ToString() );
		}
		if ( w == infoPosZ )
		{
			if ( editor_menu && editor_menu.GetDOJPrecision() )
			{
				position[2] = position[2] + value * 0.001;
			}
			else
			{
				position[2] = position[2] + value * 0.05;
			}

			SetSelectedObjectTransform( position, GetSelectedObject().GetOrientation() );
			infoPosZ.SetText( position[2].ToString() );
		}
		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) // finished = press enter
	{
		if ( !w.IsInherited( EditBoxWidget ))
		{
			return false;
		}

		vector pos = GetSelectedObjectPosition();
		vector orientation = GetSelectedObject().GetOrientation();

		EditBoxWidget editWidget = EditBoxWidget.Cast( w );
		string text = editWidget.GetText();

		float value = text.ToFloat();

//		Print("float value = text.ToFloat() = " + value);

		if ( editWidget == infoPosYaw )
		{
			orientation[0] = value;
		}
		if ( editWidget == infoPosPitch )
		{
			orientation[1] = value;
		}
		if ( editWidget == infoPosRoll )
		{
			orientation[2] = value;
		}
		if ( editWidget == infoPosX )
		{
			pos[0] = value;
		}
		if ( editWidget == infoPosY )
		{
			pos[1] = value;
		}
		if ( editWidget == infoPosZ )
		{
			pos[2] = value;
		}
		SetSelectedObjectTransform( pos, orientation );

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		SetFocus( layoutRoot );
		return false;
	}

	override void OnShow()
	{
		UpdateObjectList();
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 100, true );
	}

	void Update()
	{
		Object selectedObject = GetSelectedObject();
		string text = "No object Selected";
		TStringArray paths = new TStringArray;
		string color = "normal";

		bool selected = false;

		Widget focusedWidget = GetFocus();

		if ( selectedObject )
		{
			infoPosX.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.ClearFlags( WidgetFlags.IGNOREPOINTER );

			text = selectedObject.GetType();
			color = "info";
			if (text == "")
			{
				text = "Object Selected but no Name";
				color = "warning";
			}
		} 
		else
		{
			infoPosX.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.SetFlags( WidgetFlags.IGNOREPOINTER );
		}

		TextWidget selectedObjectWidget = layoutRoot.FindAnyWidget( "object_editor_info_select_input" );
		selectedObjectWidget.SetText( text );
		
		if (color == "normal")
		{
			selectedObjectWidget.SetColor(ARGB(255,255,255,255));
		}
		else if (color == "warning")
		{
			selectedObjectWidget.SetColor(ARGB(255,255,0,0));
		}
		else if (color == "info")
		{
			selectedObjectWidget.SetColor(ARGB(255,0,255,0));
		}
	}

	vector GetSelectedObjectPosition()
	{
		Object obj = GetSelectedObject();
		Object parent = Object.Cast( obj.GetParent() );
		if ( parent )
		{
			return parent.WorldToModel( obj.GetPosition() );
		}

		return obj.GetPosition();
	}

	void SetSelectedObjectTransform( vector position, vector orientation )
	{
		Object obj = GetSelectedObject();

		if ( dBodyIsActive( obj ) && dBodyIsDynamic( obj ) )
		{
			vector transform[4];
			obj.GetTransform( transform );
			float distance = vector.Distance( transform[3], position );
			float time = 1.0 / 40.0;

			Math3D.YawPitchRollMatrix( orientation, transform );
			transform[3] = position;
			dBodySetTargetMatrix( obj, transform, time );

			return;
		}

		Object parent = Object.Cast( obj.GetParent() );
		if ( parent )
		{
			obj.SetPosition( parent.ModelToWorld( position ) );
			obj.SetOrientation( orientation );
			ForceTargetCollisionUpdate( obj );
		} else
		{
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			ForceTargetCollisionUpdate( obj );
		}
	}

	Object GetSelectedObject()
	{
		return ObjectEditor.Cast(GetCOMModuleManager().GetModule( ObjectEditor )).m_SelectedObject;
	}
}
