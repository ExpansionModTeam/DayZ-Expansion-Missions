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

		Positions.Insert( "Airfield", "5636.620605 74.012505 3661.829590" );
		Positions.Insert( "Stonigton", "6875.350098 38.832481 2005.443115" );
		Positions.Insert( "Paris Island", "2483.801270 22.462500 3354.750000" );
		Positions.Insert( "Westbrook", "3264.520752 22.368082 6273.124023" );
		Positions.Insert( "RCFI", "3779.281738 16.012457 9045.755859" );
		Positions.Insert( "Sandy Bay", "5472.219238 2.622500 7733.409180" );
		Positions.Insert( "Mountainville", "8639.942383 7.414711 8931.773438" );
		Positions.Insert( "Mount Katahdin", "7233.665039 601.595398 11729.587891" );
		Positions.Insert( "Race Circuit", "10016.242188 21.812498 10993.663086" );
		Positions.Insert( "Military Island", "13100.995117 12.922499 9738.477539" );
		Positions.Insert( "Devasted Swamps", "14350.466797 1.875131 6612.347656" );
		Positions.Insert( "Mine", "10933.339844 46.742496 2304.359131" );
		Positions.Insert( "Camo Bear", "8885.958008 22.195593 5981.856445" );
		Positions.Insert( "Oceanville", "10242.033203 41.685661 4880.735352" );
		Positions.Insert( "Smallvile", "10389.737305 9.088058 4355.288574" );
		Positions.Insert( "Camp Eagle", "8832.635742 45.432487 3794.614746" );
		Positions.Insert( "West Brook", "3262.582520 19.473124 6394.934082" );
		Positions.Insert( "Portland", "6046.375488 5.892501 14436.607422" );
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