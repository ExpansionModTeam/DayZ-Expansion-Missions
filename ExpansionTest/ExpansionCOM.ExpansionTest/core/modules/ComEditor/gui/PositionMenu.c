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

		Positions.Insert( "Airstrip", "1072.486816 43.562496 1101.662109" );
		Positions.Insert( "Watchtower", "932.087341 80.662506 1399.989014" );
		Positions.Insert( "Village", "794.725647 43.832951 1165.473389" );
		Positions.Insert( "Port", "928.760437 6.862098 792.848206" );
		Positions.Insert( "Lake", "885.015503 44.101490 1263.654053" );
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