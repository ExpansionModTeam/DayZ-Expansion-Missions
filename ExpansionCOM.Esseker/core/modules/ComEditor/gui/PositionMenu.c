class COM_PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	autoptr map< string, vector > Positions = new map< string, vector >;

	protected bool m_bOverCurrentPos;

	void COM_PositionMenu()
	{
		m_bOverCurrentPos = false;

		Positions.Insert( "Adela Airfield", "10039.871094 73.472443 4777.682617" );
		Positions.Insert( "Lower Esseker", "10857.405273 62.710972 5395.772461" );
		Positions.Insert( "Esseker City Center", "10095.385742 70.660225 5625.604980" );
		Positions.Insert( "Esseker Industrial Zone", "8298.370117 82.849060 5418.161621" );
		Positions.Insert( "Old Esseker", "8759.964844 85.869080 5495.525879" );
		Positions.Insert( "Matija Weis College", "7382.024414 98.622505 5292.684082" );
		Positions.Insert( "Neptune Resort", "6409.291504 121.945671 5510.816406" );
		Positions.Insert( "Gulash", "6286.080566 114.962029 6191.152344" );
		Positions.Insert( "Kula", "4860.313965 172.735016 5087.645020" );
		Positions.Insert( "Lug", "6193.166992 206.140717 6939.369629|" );
		Positions.Insert( "posestra", "7724.583496 133.466949 6915.709961" );
		Positions.Insert( "Unnamed Military Base 01", "7672.477539 245.702499 7599.828613" );
		Positions.Insert( "Unnamed Military Base 02", "10943.205078 244.472504 6581.646484" );
		Positions.Insert( "Pilana Sawmil", "8997.279297 230.092438 7872.799805" );
		Positions.Insert( "Chardak Resort", "10116.057617 129.872452 7928.936035" );
		Positions.Insert( "Novi Grad", "11877.045898 82.444305 7946.409180" );
		Positions.Insert( "Kamen Quarry", "11147.742188 134.652481 9074.303711" );
		Positions.Insert( "Borosh", "10081.470703 72.855202 9860.635742" );
		Positions.Insert( "Igman Military Base", "7646.713867 124.260368 10152.380859" );
		Positions.Insert( "Tuk", "7123.677246 275.866638 9327.558594" );
	}

	void ~COM_PositionMenu()
	{
	}

	void Init()
	{
		m_LstPositionList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_cancel") );

		for ( int nPosition = 0; nPosition < Positions.Count(); nPosition++ )
		{
			m_LstPositionList.AddItem( Positions.GetKey( nPosition ), NULL, 0 );
		}
	}

	override void OnShow()
	{
		vector player_pos = GetGame().GetPlayer().GetPosition();

		m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}

	override void OnHide()
	{

	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
		{
			m_bOverCurrentPos = true;
		}

		return false;
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
		{
			m_bOverCurrentPos = false;
		}

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		if ( m_bOverCurrentPos )
		{
			m_TxtSelectedX.SetText( "" );
			m_TxtSelectedY.SetText( "" );
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
			float pos_x = 0;
			float pos_y = 0;

			if( ( m_TxtSelectedX.GetText() != "" ) && ( m_TxtSelectedY.GetText() != "" ) )
			{
				pos_x = m_TxtSelectedX.GetText().ToFloat();
				pos_y = m_TxtSelectedY.GetText().ToFloat();
			}
			else if( ( m_TxtCurrentX.GetText() != "" ) && ( m_TxtCurrentY.GetText() != "" ) )
			{
				pos_x = m_TxtCurrentX.GetText().ToFloat();
				pos_y = m_TxtCurrentY.GetText().ToFloat();
			}
			else
			{
				PlayerBase oPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
				oPlayer.MessageStatus( "Invalid teleportation position!" );

				GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Teleportation" ), new StringLocaliser( "Invalid teleportation position !" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7 );

				return true;
			}

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = SnapToGround( vPlayerPos );

			GetNotificationSystem().CreateNotification( new StringLocaliser( "COM Teleportation" ), new StringLocaliser( "Teleported player at selected location !" ), EXPANSION_NOTIFICATION_ICON_PERSONA, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7 );

			GetGame().GetPlayer().SetPosition( vPlayerPos );

			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";

		if( !Positions.Find( GetCurrentPositionName(), position ) )
		{
			position = "0 0 0";
		}

		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );

		return true;
	}

	string GetCurrentPositionName()
	{
		if ( m_LstPositionList.GetSelectedRow() != -1 )
		{
			string position_name;
			m_LstPositionList.GetItemText( m_LstPositionList.GetSelectedRow(), 0, position_name );
			return position_name;
		}

		return "";
	}
}