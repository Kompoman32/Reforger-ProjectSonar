class RT_PS_CustomRadioAntennaSystem: GameSystem
{
	[Attribute("", UIWidgets.Object)]
	ref array<ref RT_PS_CustomRadioStation> m_aRadiostations;

	[RplProp()]
	protected ref array<ref RT_PS_CustomRadioStationTrackInfo> m_aRadiostationsTracks = {};
	
	[RplProp()]
	protected ref array<ref RT_PS_CustomRadioStationTrackInfoTimestampWrapper> m_aRadiostationsTimesWrapers = {};
	
	[RplProp()]
	bool m_bAllowRadios = true;
	
	[RplProp()]
	bool m_bAllowFixRadios = false;
	
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
		Print(string.Format("Antenna on Server Inited %1", this), LogLevel.NORMAL);

		array<string> names = {};
		
		foreach (RT_PS_CustomRadioStation x: m_aRadiostations)
		{
			names.Insert(x.m_sRadiostationName);
		}
		
		Print(string.Format("RadioStations: %1", names), LogLevel.NORMAL);
		
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gm)
		{
			ScriptInvokerBase<SCR_BaseGameMode_PlayerId> invoker = gm.GetOnPlayerConnected();	
			if (invoker) 
				invoker.Insert(OnPlayerConnected);
		}
		
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Insert(OnVehicleDamaged);
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
			Print(string.Format("UpdateTrack %1:(%2) - tInd: %3, tLen: %4", pIndex, radiostation.m_sRadiostationName, newTrack.m_iTrackIndex, newTrack.m_iTrackSize), LogLevel.NORMAL);
		} else {
			Print(string.Format("UpdateTrack %1:(%2) - tInd: null, tLen: null", pIndex, radiostation.m_sRadiostationName), LogLevel.NORMAL);
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
		array<WorldTimestamp> newRadiostationsTimes = {};
		
		foreach (RT_PS_CustomRadioStationTrackInfoTimestampWrapper x: pRadiostationsTimes)
		{
			if (x) {
				newRadiostationsTimes.Insert(x.m_Timestamp);
			} else {
				newRadiostationsTimes.Insert(null);
			}
		}
		
		auto oldRadiostationsTimes= m_aRadiostationsTimes;
		
		m_aRadiostationsTracks = pRadiostationsTracks;
		m_aRadiostationsTimesWrapers = pRadiostationsTimes;
		m_aRadiostationsTimes = newRadiostationsTimes;
		
		// Debug
		string radioStationsText = "";
		
		foreach (int i,RT_PS_CustomRadioStation x: m_aRadiostations)
		{
			if (x) {
				RT_PS_CustomRadioStationTrackInfo track;
				string trackText;
				
				if (i < pRadiostationsTracks.Count())
				{
					track = pRadiostationsTracks[i];
				}
				
				if (track) 
				{
					trackText = string.Format("%1 (%2s/%3s)", track.m_iTrackIndex, GetRadioStationTrackTimeLeft(i), track.m_iTrackSize);
				} else {
					trackText = "null";
				}
				
				WorldTimestamp time;
				if (i < newRadiostationsTimes.Count())
				{
					time = newRadiostationsTimes[i];
				}
				
				radioStationsText += string.Format("    %1: [track: %2, serverTime: %3]", x.m_sRadiostationName, trackText, time) + "\n";
			} else {
				radioStationsText += "    Station null: [null]\n";
			}
		}
		
		Print(string.Format("Tracks Updated.\nStations:\n[\n%1]", radioStationsText), LogLevel.NORMAL);
		
		UpdateConnectedRadios(oldRadiostationsTimes);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnVehicleDamaged(SCR_VehicleDamageManagerComponent pDamageManager)
	{
		if (pDamageManager.GetState() != EDamageState.DESTROYED)
			return;
		
		IEntity vehicle = pDamageManager.GetOwner();
	
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(vehicle.FindComponent(SlotManagerComponent));
		if (!slotManager) return;
		
		EntitySlotInfo slot = slotManager.GetSlotByName("RADIO");
		if (!slot) 
		{
			slot = slotManager.GetSlotByName("radio");
		};
		
		if (!slot) return;
		
		IEntity attachedEntity = slot.GetAttachedEntity();
		if (!attachedEntity) return;

		RT_PS_CustomRadioComponent radio = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
	
		if (!radio) return;
	
		radio.Disable();
		radio.ResetPlay();
	}
	
	//------------------------------------------------------------------------------------------------
	int GetRadiostaionsCount() 
	{
		return m_aRadiostations.Count();
	}
	
	RT_PS_CustomRadioStation GetRadioStation(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count())	 {
			return null;
		}
		
		return m_aRadiostations[pStationIndex];
	}
	
	int GetRadioStationTrackIndex(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count())	 {
			return null;
		}
		
		if (!m_aRadiostationsTracks[pStationIndex]) {
			return null;
		}
		
		return m_aRadiostationsTracks[pStationIndex].m_iTrackIndex;
	}
	
	//------------------------------------------------------------------------------------------------
	RT_PS_CustomRadioStationTrackInfo GetRadioStationTrack(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count())	 {
			return null;
		}
		
		return m_aRadiostationsTracks[pStationIndex];
	}
	
	
	//------------------------------------------------------------------------------------------------
	float GetRadioStationTrackOffset(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count() || !m_aRadiostationsTracks[pStationIndex])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_aRadiostationsTracks[pStationIndex].m_iTrackSize;
		WorldTimestamp time = m_aRadiostationsTimes[pStationIndex];
		
		return trackSize*1000 - time.DiffMilliseconds(timeNow_s);
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRadioStationTrackTimeLeft(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count() || !m_aRadiostationsTracks[pStationIndex])	 {
			return 0;
		}
		
		// float timeNow_s = m_owner.GetWorld().GetWorldTime();
		WorldTimestamp timeNow_s = ChimeraWorld.CastFrom(GetGame().GetWorld()).GetServerTimestamp();
		float trackSize = m_aRadiostationsTracks[pStationIndex].m_iTrackSize;
		WorldTimestamp time = m_aRadiostationsTimes[pStationIndex];
		
		return time.DiffMilliseconds(timeNow_s) / 1000;
		//return (timeNow_s - (time*1000 - trackSize*1000));
	}
	
	//------------------------------------------------------------------------------------------------
	string GetRadioStationTrackName(int pStationIndex) {
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostations.Count())	 {
			return string.Empty;
		}
		
		if (pStationIndex < 0 || pStationIndex >= m_aRadiostationsTracks.Count())	 {
			return string.Empty;
		}
				
		RT_PS_CustomRadioStation radioStation = m_aRadiostations[pStationIndex];
		RT_PS_CustomRadioStationTrackInfo track = m_aRadiostationsTracks[pStationIndex];
		
		return radioStation.GetTrackName(track.m_iTrackIndex, track.m_bIsDJ);
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateConnectedRadios(array<WorldTimestamp> pRadiostationsTimes) 
	{		
		foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
		{
			int radioStationIndex = radio.m_iRadioStationIndex;
			
			auto time = pRadiostationsTimes[radioStationIndex];
			
			if (Replication.IsServer()) {
				radio.ResetPlay();
			}
			
			// 5 second may fix track jumping when we have a lot of radios, maybe a better solution can be found later
			if (Math.AbsFloat(time.DiffMilliseconds(m_aRadiostationsTimes[radioStationIndex])) > 5000) {
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
	void Debug_On_Off_radios()
	{		
		Rpc(RPC_DoDebug_OffOnRadios);
		RPC_DoDebug_OffOnRadios();
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_DoDebug_OffOnRadios()
	{		
		m_bAllowRadios = !m_bAllowRadios;
		
		if (Replication.IsServer()) 
		{
			ForceUpdateConnectedRadios();	
		} 
		else 
		{
			foreach (EntityID radioId, RT_PS_CustomRadioComponent radio: m_activeRadios)
			{
				if (m_bAllowRadios)
				{
					radio.StartPlay();
				} else {
					radio.StopPlay();
				}
			}
		}
		

	}
	
	//------------------------------------------------------------------------------------------------
	void Debug_AllowFixRadios()
	{		
		Rpc(RPC_DoDebug_AllowFixRadios);
		RPC_DoDebug_AllowFixRadios();
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_DoDebug_AllowFixRadios()
	{		
		m_bAllowFixRadios = !m_bAllowFixRadios;
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
	
	void ~RT_PS_CustomRadioAntennaSystem() 
	{
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Remove(OnVehicleDamaged);
	}
}