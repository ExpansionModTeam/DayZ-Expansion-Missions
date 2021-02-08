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

		Positions.Insert( "Herreninsel", "7027.956055 10.402502 3514.430664" );
		Positions.Insert( "Osternach", "3914.968506 29.584784 2703.943115" );
		Positions.Insert( "Baustelle", "4018.841553 7.807599 5078.954102" );
		Positions.Insert( "Hochstatt", "2623.289551 24.616463 5887.574707" );
		Positions.Insert( "Sagewerk", "4222.124512 12.522442 6658.396484" );
		Positions.Insert( "Gstadt Industrie", "8231.880859 4.878265 6337.642090" );
		Positions.Insert( "Gollenhausen", "9335.285156 6.277999 8797.252930" );
		Positions.Insert( "Eggstatt", "5296.567871 2.072500 8390.075195" );
		Positions.Insert( "Hemhof", "3345.000244 4.323145 8686.267578" );
		Positions.Insert( "Jolling", "1140.620239 19.302500 9485.746094" );
		Positions.Insert( "Soll", "8409.345703 45.072495 9785.223633" );
		Positions.Insert( "Stein", "4630.960449 74.067032 7582.009766" );
		Positions.Insert( "AltSchwanstein", "3954.993896 50.788231 1267.142090" );
		Positions.Insert( "Alm", "1405.421875 315.157776 1758.245361" );
		Positions.Insert( "Hub", "2136.849609 502.716614 538.320679" );
		Positions.Insert( "TedValley", "881.134155 27.349918 1320.070313" );
		Positions.Insert( "Fraueninsel", "8944.375977 16.812483 4821.572266" );
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

				return true;
			}

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = SnapToGround( vPlayerPos );

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