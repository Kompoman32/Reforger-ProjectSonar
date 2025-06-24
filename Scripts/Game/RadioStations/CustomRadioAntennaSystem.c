class CustomRadioAntennaSystem: GameSystem
{
//	protected ref array<SCR_VehicleDustPerWheel> m_Components = {};
	
		
	IEntity m_owner;
	
	[RplProp()]
	protected ref array<ref CustomRadioStationTrackInfo> m_radiostationsTracks = {};
	
	
	[RplProp()]
	protected ref array<ref CustomRadioStationTrackInfoTimestampWrapper> m_radiostationsTimesWrapers = {};
	protected ref array<WorldTimestamp> m_radiostationsTimes = {};
	protected int m_radiostationsCount = 0;
	
	ref map<EntityID, CustomRadioComponent> m_activeRadios = new map<EntityID, CustomRadioComponent>;
	
	[Attribute("", UIWidgets.Object)]
	ref array<ref CustomRadioStation> m_radiostations;
	

	override static void InitInfo(WorldSystemInfo outInfo)
	{
		super.InitInfo(outInfo);
		outInfo.SetAbstract(true)/*.SetUnique(true)*/;
	}
	
	override protected void OnUpdate(ESystemPoint point)
	{
		super.OnUpdate();
		
		const RplId systemRplid = Replication.FindId(this);
		const RplNode systemRplNode = Replication.FindNode(systemRplid);
		
		if (systemRplNode.GetRole() == RplRole.Proxy) return;
		
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		
		bool somethingUpdated = false;
		
		for (int i = 0; i < m_radiostationsCount; ++i)
		{
			if (timeNow_s.Greater(m_radiostationsTimes[i])) {
				UpdateTrack(i);
				
				if (m_radiostationsTracks[i])
				{
					somethingUpdated  = true;
					m_radiostationsTimes[i] = timeNow_s.PlusSeconds(m_radiostationsTracks[i].m_trackSize);	
				} else {
					timeNow_s.PlusSeconds(10);
				}
			}
		}
		
		if (somethingUpdated)
		{			
			SendTimesToClients();
		}
	}
	
	override event protected void OnStarted()
	{
		InitRadioStations();
		
		if (Replication.IsServer()) 
		{
			GetGame().GetCallqueue().CallLater(InitOnServer, 200, false);
		} else {
			GetGame().GetCallqueue().CallLater(InitOnClient, 2000, false);
		}
	}
	
	override event protected void OnStopped()
	{
//		if (s_Instance == this)
//		{
//			s_Instance = null;
//			
//			if (Replication.IsServer()) {
//				SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
//				if (gm)
//				{
//					ScriptInvokerBase<SCR_BaseGameMode_PlayerId> invoker = gm.GetOnPlayerConnected();	
//					if (invoker) 
//						invoker.Remove(OnPlayerConnected);
//				}
//			}
//			
//			foreach (EntityID radioId, CustomRadioComponent radio: m_activeRadios)
//			{
//				radio.StopPlay();
//			}
//		}
	}
	
	void InitOnServer()
	{		
		PrintFormat("Antenna on Server Inited %1", this);

		array<string> names = {};
		
		foreach (CustomRadioStation x: m_radiostations)
		{
			names.Insert(x.m_radiostationName);
		}
		
		PrintFormat("RadioStations: %1", names);
		
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gm)
		{
			ScriptInvokerBase<SCR_BaseGameMode_PlayerId> invoker = gm.GetOnPlayerConnected();	
			if (invoker) 
				invoker.Insert(OnPlayerConnected);
		}
	}
	
	void InitOnClient()
	{
				
	}
	
	protected void InitRadioStations()
	{								
		ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		WorldTimestamp t;
		
		if (world) {
			t = world.GetServerTimestamp()
		}
		
		foreach (auto x: m_radiostations)
		{
			m_radiostationsTimes.Insert(t);
			m_radiostationsTracks.Insert(null);
		}
		
		m_radiostationsCount = m_radiostations.Count();
	}
	
	protected void OnPlayerConnected(int playerId)
	{
		SendTimesToClients();
	}
	
	void UpdateTrack(int index) 
	{
		CustomRadioStation radiostation = m_radiostations[index];
		CustomRadioStationTrackInfo newTrack = radiostation.GetNewTrack();
		
		if (newTrack){
			PrintFormat("UpdateTrack %1:(%2) - tInd: %3, tLen: %4", index, radiostation.m_radiostationName, newTrack.m_trackIndex, newTrack.m_trackSize);
		} else {
			PrintFormat("UpdateTrack %1:(%2) - tInd: null, tLen: null", index, radiostation.m_radiostationName);
		}
		
		m_radiostationsTracks.Set(index, newTrack);
	}
	
	void SendTimesToClients()
	{
		array<ref CustomRadioStationTrackInfoTimestampWrapper> arr = {};
		
		for (int i = 0; i < m_radiostationsTimes.Count(); ++i)
		{
			CustomRadioStationTrackInfoTimestampWrapper wrapper = new CustomRadioStationTrackInfoTimestampWrapper();
			
			wrapper.m_timestamp = m_radiostationsTimes[i];
			arr.Insert(wrapper);
		}
		
		
		RpcDo_UpdateTracks(m_radiostationsTracks, arr);
		Rpc(RpcDo_UpdateTracks, m_radiostationsTracks, arr);
		
	}
	
	void OnRplRpcDo_UpdateTracks()
	{
		RpcDo_UpdateTracks(m_radiostationsTracks, m_radiostationsTimesWrapers)
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_UpdateTracks(array<ref CustomRadioStationTrackInfo> radiostationsTracks, array<ref CustomRadioStationTrackInfoTimestampWrapper> radiostationsTimes)
	{		
		array<WorldTimestamp> arr = {};
		
		foreach (CustomRadioStationTrackInfoTimestampWrapper x: radiostationsTimes)
		{
			arr.Insert(x.m_timestamp);
		}
		
		auto oldRadiostationsTimes= m_radiostationsTimes;
		
		m_radiostationsTracks = radiostationsTracks;
		m_radiostationsTimesWrapers = radiostationsTimes;
		m_radiostationsTimes = arr;
		
		PrintFormat("Tracks Updated. Times: %1", m_radiostationsTimes);
		
		UpdateConnectedRadios(oldRadiostationsTimes);
	}
	
	int GetRadiostaionsCount() 
	{
		return m_radiostations.Count();
	}
	
	CustomRadioStation GetRadioStation(int index) {
		if (index < 0 || index >= m_radiostations.Count())	 {
			return null;
		}
		
		return m_radiostations[index];
	}
	
	CustomRadioStationTrackInfo GetRadioStationTrack(int index) {
		if (index < 0 || index >= m_radiostations.Count())	 {
			return null;
		}
		
		return m_radiostationsTracks[index];
	}
	
	float GetRadioStationTrackOffset(int index) {
		if (index < 0 || index >= m_radiostations.Count() || !m_radiostationsTracks[index])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_radiostationsTracks[index].m_trackSize;
		WorldTimestamp time = m_radiostationsTimes[index];
		
		return trackSize*1000 - time.DiffMilliseconds(timeNow_s);
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	float GetRadioStationTrackTimeLeft(int index) {
		if (index < 0 || index >= m_radiostations.Count() || !m_radiostationsTracks[index])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_radiostationsTracks[index].m_trackSize;
		WorldTimestamp time = m_radiostationsTimes[index];
		
		return time.DiffMilliseconds(timeNow_s) / 1000;
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	void UpdateConnectedRadios(array<WorldTimestamp> radiostationsTimes) 
	{		
		foreach (EntityID radioId, CustomRadioComponent radio: m_activeRadios)
		{
			int radioStationIndex = radio.m_radioStationIndex;
			
			auto time = radiostationsTimes[radioStationIndex];
			
			if (Math.AbsFloat(time.DiffMilliseconds(m_radiostationsTimes[radioStationIndex])) > 0.1) {
				radio.ResetPlay();
			}			
		}
	}
	
	void ForceUpdateConnectedRadios()
	{
		foreach (EntityID radioId, CustomRadioComponent radio: m_activeRadios)
		{
			radio.ResetPlay();
		}
	}
	
	void Connect(CustomRadioComponent radio) 
	{
		m_activeRadios.Set(radio.GetOwner().GetID(), radio);
	}
	
	void Disconnect(CustomRadioComponent radio) 
	{
		m_activeRadios.Remove(radio.GetOwner().GetID());
	}
	
	
	// -------------- DEBUG -------------- //
	
	void Debug_UpdateRadios()
	{
		SendTimesToClients();
	}
	
	void Debug_UpdateTrack(int radioStationIndex)
	{
		m_radiostationsTimes[radioStationIndex] = null;
	}
	
	void Debug_UpdateTrack_2(int radioStationIndex)
	{		
		foreach (EntityID radioId, CustomRadioComponent radio: m_activeRadios)
		{			
			if (radioStationIndex != radio.m_radioStationIndex) continue;
			
			radio.ActionReset();	
		}
	}
}