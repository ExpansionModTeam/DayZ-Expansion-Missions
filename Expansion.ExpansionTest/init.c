void main()
{
	Entity playerEnt = GetGame().CreatePlayer(NULL, "SurvivorM_Elias", "250 0 250", 0, "NONE");
	PlayerBase player = PlayerBase.Cast(playerEnt) ;
	GetGame().SelectPlayer(NULL, player);
}