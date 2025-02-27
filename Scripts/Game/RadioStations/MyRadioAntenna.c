class MyRadioAntennaComponentClass: ScriptComponentClass
{
	static override bool DependsOn(string className)
	{
		if (className == "MyRadioStationComponentClass")
			return true;

		return false;
	}
}

class MyRadioAntennaComponent: ScriptComponent 
{
	[RplProp(onRplName: "OnInstanceUpdated")]
	static MyRadioAntennaComponent s_Instance;
	
	protected IEntity m_owner;
	protected ref array<MyRadioStationComponent> m_radiostations = {};
	protected ref array<ref MyRadioStationTrackInfo> m_radiostationsTracks = {};
	
	protected ref array<float> m_radiostationsTimes = {};
	protected int m_radiostationsCount = 0;
	
	protected ref map<EntityID, MyRadioComponent> m_activeRadios = new map<EntityID, MyRadioComponent>;
	
	
	
	void ~MyRadioAntennaComponent()
	{
		if (s_Instance == this)
		{
			s_Instance = null;
			Rpc(RpcDo_OnInstanceDestroy);
		}
	}
	
	override void OnPostInit(IEntity owner)
	{
		m_owner = owner;
		
		if (s_Instance) {
			Print("Only one instance of MyRadioAntennaComponent is allowed in the world!", LogLevel.WARNING);
			//delete owner;
			return;
		}
		
		s_Instance = this;	
		
		OnInstanceUpdated(s_Instance);
		
		Rpc(RpcAsk_Authority_InitOnServer);
		//RpcAsk_Authority_InitOnServer();
	}
	
	/** Executed on Server  */
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAsk_Authority_InitOnServer()
	{		
		Print("Server Inited");
		Print(this);

		array<string> names = {};
		
		foreach (MyRadioStationComponent x: m_radiostations)
		{
			names.Insert(x.m_radiostationName);
		}
		Print(names);
		
		SetEventMask(m_owner, EntityEvent.FRAME);
	}
	
	/** Executed on Clients  */
	protected void OnInstanceUpdated(MyRadioAntennaComponent instance)
	{
		s_Instance = instance;
		
		array<Managed> radiostations = {};
		instance.m_owner.FindComponents(MyRadioStationComponent, radiostations);
		
		foreach (auto x: radiostations)
		{
			auto antenna = MyRadioStationComponent.Cast(x);
			m_radiostations.Insert(antenna);
			m_radiostationsTimes.Insert(0);
			m_radiostationsTracks.Insert(null);
		}
		
		m_radiostationsCount = m_radiostations.Count();
	}
	
	/** Executed on Clients  */
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_OnInstanceDestroy()
	{
		s_Instance = null;
	}
	
	override void EOnFrame(IEntity owner, float timeSlice) 
	{		
		float timeNow_s = m_owner.GetWorld().GetWorldTime() / 1000;	
		
		bool somethingUpdated = false;
		
		for (int i = 0; i < m_radiostationsCount; ++i)
		{
			if (m_radiostationsTimes[i] < timeNow_s - 1) {
				UpdateTrack(i);
				
				if (m_radiostationsTracks[i])
				{
					somethingUpdated  = true;
					m_radiostationsTimes[i] = timeNow_s + m_radiostationsTracks[i].m_trackSize;	
				}
			}
		}
		
		if (somethingUpdated)
		{
			Print("Tracks Updated");
			Print(m_radiostationsTimes);			
			
			Rpc(RpcDo_UpdateTracks, m_radiostationsTracks, m_radiostationsTimes);
			RpcDo_UpdateTracks(m_radiostationsTracks, m_radiostationsTimes);
		}
	}
	
	void UpdateTrack(int index) 
	{
		MyRadioStationComponent radiostation = m_radiostations[index];
		MyRadioStationTrackInfo newTrack = radiostation.GetNewTrack();
		
		m_radiostationsTracks.Set(index, newTrack);
	}
	
	[RplRpc(RplChannel.Unreliable, RplRcver.Broadcast)]
	void RpcDo_UpdateTracks(array<ref MyRadioStationTrackInfo> radiostationsTracks, array<float> radiostationsTimes)
	{
		m_radiostationsTracks = radiostationsTracks;
		m_radiostationsTimes = radiostationsTimes;
		
		UpdateConnectedRadios();
	}
	
	int GetRadiostaionsCount() 
	{
		return m_radiostations.Count();
	}
	
	MyRadioStationComponent GetRadioStation(int index) {
		if (index < 0 || index >= m_radiostations.Count())	 {
			return null;
		}
		
		return m_radiostations[index];
	}
	
	MyRadioStationTrackInfo GetRadioStationTrack(int index) {
		if (index < 0 || index >= m_radiostations.Count())	 {
			return null;
		}
		
		return m_radiostationsTracks[index];
	}
	
	float GetRadioStationTrackOffset(int index) {
		if (index < 0 || index >= m_radiostations.Count() || !m_radiostationsTracks[index])	 {
			return 0;
		}
		
		float timeNow_s = m_owner.GetWorld().GetWorldTime();
		float trackSize = m_radiostationsTracks[index].m_trackSize;
		float time = m_radiostationsTimes[index];
		
		
		return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	void Connect(MyRadioComponent radio) 
	{
		m_activeRadios.Set(radio.GetOwner().GetID(), radio);
	}
	
	void Disconnect(MyRadioComponent radio) 
	{
		m_activeRadios.Remove(radio.GetOwner().GetID());
	}
	
	void UpdateConnectedRadios() 
	{
		foreach (EntityID radioId, MyRadioComponent radio: m_activeRadios)
		{
			radio.Reset();
		}
	}
}