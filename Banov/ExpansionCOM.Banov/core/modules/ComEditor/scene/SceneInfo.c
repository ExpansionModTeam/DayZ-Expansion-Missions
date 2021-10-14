/**
\brief Stores loaded scene data information from json file. ie object positions, loot positions, duration etc

*/
class SceneInfo
{

	//ref map< ObjectData, array< LootSpot >>  m_objects = new map< ObjectData, array< LootSpot >>;
	string name;
	ref array< ObjectData > m_objects = new array< ObjectData >;
	ref array< LootSpot > m_lootSpots = new array< LootSpot >;

	bool persistent = false;
	int duration = 0; // in seconds

	void SceneInfo( string sceneName ) 
	{
		name = sceneName;
	}

	void ~SceneInfo() 
	{

	}

	string GetName() 
	{
		return name;
	}

	void AddObject( Object object, vector pos ) 
	{
		m_objects.Insert( new ObjectData( object.GetType(), pos, object.GetOrientation() ));
	}

	void AddLootSpot( vector pos, LootType type ) 
	{
		m_lootSpots.Insert( new LootSpot( pos, type ) );
	}
}

// create scene (set origin point to player position)
// create object containers with loot spots
// add object container to scene