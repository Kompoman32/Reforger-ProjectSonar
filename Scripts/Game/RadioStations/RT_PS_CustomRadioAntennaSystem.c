class RT_PS_CustomRadioAntennaSystem: GameSystem
{
	[Attribute("", UIWidgets.Object)]
	ref array<ref RT_PS_CustomRadioStation> m_aRadiostations;

	[RplProp()]
	protected ref array<ref RT_PS_CustomRadioStationTrackInfo> m_aRadiostationsTracks = {};
	
	[RplProp()]
	protected ref array<ref RT_PS_CustomRadioStationTrackInfoTimestampWrapper> m_aRadiostationsTimesWrapers = {};
	
	IEntity m_owner;

	protected int m_iRadiostationsCount = 0;
	protected ref array<WorldTimestamp> m_aRadiostationsTimes = {};
	protected ref map<EntityID, RT_PS_CustomRadioComponent> m_activeRadios = new map<EntityID, RT_PS_CustomRadioComponent>;

	//------------------------------------------------------------------------------------------------
	override static void InitInfo(WorldSystemInfo outInfo)
	{
		super.InitInfo(outInfo);
		outInfo.SetAbstract(true);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnUpdate(ESystemPoint point)
	{
		super.OnUpdate();
		
		const RplId systemRplid = Replication.FindId(this);
		const RplNode systemRplNode = Replication.FindNode(systemRplid);
		
		if (systemRplNode.GetRole() == RplRole.Proxy) return;
		
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		
		bool somethingUpdated = false;
		
		for (int i = 0; i < m_iRadiostationsCount; ++i)
		{
			if (timeNow_s.Greater(m_aRadiostationsTimes[i])) {
				UpdateTrack(i);
				
				if (m_aRadiostationsTracks[i])
				{
					somethingUpdated  = true;
					m_aRadiostationsTimes[i] = timeNow_s.PlusSeconds(m_aRadiostationsTracks[i].m_iTrackSize);	
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
	
	//------------------------------------------------------------------------------------------------
	protected void InitOnServer()
	{		
		PrintFormat("Antenna on Server Inited %1", this);

		array<string> names = {};
		
		foreach (RT_PS_CustomRadioStation x: m_aRadiostations)
		{
			names.Insert(x.m_sRadiostationName);
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
	
	//------------------------------------------------------------------------------------------------
	protected void InitOnClient()
	{
				
	}
	
	//------------------------------------------------------------------------------------------------
	protected void InitRadioStations()
	{								
		ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		WorldTimestamp t;
		
		if (world) {
			t = world.GetServerTimestamp()
		}
		
		foreach (auto x: m_aRadiostations)
		{
			m_aRadiostationsTimes.Insert(t);
			m_aRadiostationsTracks.Insert(null);
		}
		
		m_iRadiostationsCount = m_aRadiostations.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnPlayerConnected(int playerId)
	{
		SendTimesToClients();
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateTrack(int pIndex) 
	{
		RT_PS_CustomRadioStation radiostation = m_aRadiostations[pIndex];
		RT_PS_CustomRadioStationTrackInfo newTrack = radiostation.GetNewTrack();
		
		if (newTrack){
			PrintFormat("UpdateTrack %1:(%2) - tInd: %3, tLen: %4", pIndex, radiostation.m_sRadiostationName, newTrack.m_iTrackIndex, newTrack.m_iTrackSize);
		} else {
			PrintFormat("UpdateTrack %1:(%2) - tInd: null, tLen: null", pIndex, radiostation.m_sRadiostationName);
		}
		
		m_aRadiostationsTracks.Set(pIndex, newTrack);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SendTimesToClients()
	{
		array<ref RT_PS_CustomRadioStationTrackInfoTimestampWrapper> arr = {};
		
		for (int i = 0; i < m_aRadiostationsTimes.Count(); ++i)
		{
			RT_PS_CustomRadioStationTrackInfoTimestampWrapper wrapper = new RT_PS_CustomRadioStationTrackInfoTimestampWrapper();
			
			wrapper.m_Timestamp = m_aRadiostationsTimes[i];
			arr.Insert(wrapper);
		}
		
		
		RpcDo_UpdateTracks(m_aRadiostationsTracks, arr);
		Rpc(RpcDo_UpdateTracks, m_aRadiostationsTracks, arr);
		
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_UpdateTracks(array<ref RT_PS_CustomRadioStationTrackInfo> pRadiostationsTracks, array<ref RT_PS_CustomRadioStationTrackInfoTimestampWrapper> pRadiostationsTimes)
	{		
		array<string> radioStationNames = {};
		array<int> tracksIndexes= {};
		array<WorldTimestamp> newRadiostationsTimes = {};
		
		foreach (RT_PS_CustomRadioStationTrackInfoTimestampWrapper x: pRadiostationsTimes)
		{
			if (x) {
				newRadiostationsTimes.Insert(x.m_Timestamp);
			} else {
				newRadiostationsTimes.Insert(null);
			}
		}

		foreach (RT_PS_CustomRadioStationTrackInfo x: pRadiostationsTracks)
		{
			if (x) {
				tracksIndexes.Insert(x.m_iTrackIndex);
			} else {
				tracksIndexes.Insert(-1);
			}
		}
		
		foreach (RT_PS_CustomRadioStation x: m_aRadiostations)
		{
			if (x) {
				radioStationNames.Insert(x.m_sRadiostationName);
			} else {
				radioStationNames.Insert("null");
			}
		}
		
		auto oldRadiostationsTimes= m_aRadiostationsTimes;
		
		m_aRadiostationsTracks = pRadiostationsTracks;
		m_aRadiostationsTimesWrapers = pRadiostationsTimes;
		m_aRadiostationsTimes = newRadiostationsTimes;
		
		PrintFormat("Tracks Updated. Stations: %1 | Tracks: %2 | Times: %3", radioStationNames, tracksIndexes, newRadiostationsTimes);
		
		UpdateConnectedRadios(oldRadiostationsTimes);
	}
	
	//------------------------------------------------------------------------------------------------
	int GetRadiostaionsCount() 
	{
		return m_aRadiostations.Count();
	}
	
	RT_PS_CustomRadioStation GetRadioStation(int pIndex) {
		if (pIndex < 0 || pIndex >= m_aRadiostations.Count())	 {
			return null;
		}
		
		return m_aRadiostations[pIndex];
	}
	
	//------------------------------------------------------------------------------------------------
	RT_PS_CustomRadioStationTrackInfo GetRadioStationTrack(int pIndex) {
		if (pIndex < 0 || pIndex >= m_aRadiostations.Count())	 {
			return null;
		}
		
		return m_aRadiostationsTracks[pIndex];
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRadioStationTrackOffset(int pIndex) {
		if (pIndex < 0 || pIndex >= m_aRadiostations.Count() || !m_aRadiostationsTracks[pIndex])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_aRadiostationsTracks[pIndex].m_iTrackSize;
		WorldTimestamp time = m_aRadiostationsTimes[pIndex];
		
		return trackSize*1000 - time.DiffMilliseconds(timeNow_s);
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRadioStationTrackTimeLeft(int pIndex) {
		if (pIndex < 0 || pIndex >= m_aRadiostations.Count() || !m_aRadiostationsTracks[pIndex])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_aRadiostationsTracks[pIndex].m_iTrackSize;
		WorldTimestamp time = m_aRadiostationsTimes[pIndex];
		
		return time.DiffMilliseconds(timeNow_s) / 1000;
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateConnectedRadios(array<WorldTimestamp> pRadiostationsTimes) 
	{		
		foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
		{
			int radioStationIndex = radio.m_iRadioStationIndex;
			
			auto time = pRadiostationsTimes[radioStationIndex];
			
			if (Math.AbsFloat(time.DiffMilliseconds(m_aRadiostationsTimes[radioStationIndex])) > 0.1) {
				radio.ResetPlay();
			}			
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ForceUpdateConnectedRadios()
	{
		foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
		{
			radio.ResetPlay();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void Connect(RT_PS_CustomRadioComponent pRadio) 
	{
		m_activeRadios.Set(pRadio.GetOwner().GetID(), pRadio);
	}
	
	//------------------------------------------------------------------------------------------------
	void Disconnect(RT_PS_CustomRadioComponent pRadio) 
	{
		m_activeRadios.Remove(pRadio.GetOwner().GetID());
	}
	
	
	// -------------- DEBUG -------------- //
	
	//------------------------------------------------------------------------------------------------
	void Debug_UpdateRadios()
	{
		SendTimesToClients();
	}
	
	//------------------------------------------------------------------------------------------------
	void Debug_UpdateTrack(int pRadioStationIndex)
	{
		m_aRadiostationsTimes[pRadioStationIndex] = null;
	}
	
	//------------------------------------------------------------------------------------------------
	void Debug_UpdateTrack_2(int pRadioStationIndex)
	{		
		foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
		{			
			if (pRadioStationIndex != radio.m_iRadioStationIndex) continue;
			
			radio.ActionReset();	
		}
	}


	//------------------------------------------------------------------------------------------------
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
	
	//------------------------------------------------------------------------------------------------
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
//			foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
//			{
//				radio.StopPlay();
//			}
//		}
	}
}