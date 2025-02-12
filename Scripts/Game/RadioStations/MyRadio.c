class MyRadioComponentClass: ScriptComponentClass
{
}

class MyRadioComponent: ScriptComponent 
{
	static string SOUND_EVENT_NAME = "SOUND_RADIO";
		
	static string DJ_INDEX_SIGNAL= "DJTrackID";
	static string MUSIC_INDEX_SIGNAL= "MusicTrackID";
	static string OFFSET_SIGNAL= "Offset";
	static string BROADCAST_TYPE_SIGNAL= "BroadcastType";
	static string VOLUME_SIGNAL= "RadioVolume";
	
	protected IEntity m_owner;
	protected vector m_ownerTransform[4];
	protected AudioHandle m_currentTrack = AudioHandle.Invalid;
	MyRadioStationComponent m_radioStation;
	[RplProp()]
	int m_radioStationIndex = 0;
	
	protected SignalsManagerComponent m_signalManager;
	
	[RplProp(onRplName: "onRplSetEnableDisable")]
	protected bool b_state;
	[RplProp(onRplName: "onRplSetVolume")]
	protected float m_volume = 50;
	
	
	protected float m_volumeToSignalDivider = 100;
	
	protected void ~MyRadioComponent() 
	{
		EOnDeactivate(m_owner);
	}
	
	override void OnPostInit(IEntity owner)
	{		
		m_owner = owner;
		
		m_signalManager = SignalsManagerComponent.Cast(m_owner.FindComponent(SignalsManagerComponent));
		
		PrintFormat("Radio Inited %1: %2", owner.GetName(), owner);
		
		EOnActivate(owner);
	}
	
	override void EOnActivate(IEntity owner) 
	{		
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Insert(OnVehicleDamaged);
		

		if (!MyRadioAntennaComponent.s_Instance) return;
		
		if (b_state) {
			MyRadioAntennaComponent.s_Instance.Connect(this);
			StartPlay();
		}
	}
	
	override void EOnDeactivate(IEntity owner) 
	{		
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Remove(OnVehicleDamaged);
		
		if (!MyRadioAntennaComponent.s_Instance) return;
		
		MyRadioAntennaComponent.s_Instance.Disconnect(this);
		StopPlay();
	}
	
	override void EOnFrame(IEntity owner, float timeSlice) 
	{
		if (!Enabled() || !MyRadioAntennaComponent.s_Instance) return;
		
		m_owner.GetTransform(m_ownerTransform);
		AudioSystem.SetSoundTransformation(m_currentTrack, m_ownerTransform);	
	}
	
	bool Enabled() {
		return b_state;
	}
	
	void ActionEnableDisable(bool enable)
	{
		b_state = enable;
		onRplSetEnableDisable();
		//	Rpc(RpcAsk_ActionEnableDisable, enable)
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAsk_ActionEnableDisable(bool enable)
	{
		if (b_state == enable) { return; }
		
		b_state = enable;
		
		Replication.BumpMe();
	}
	
	/** Enable Radio / Executed on Server **/
	void Enable() {		
		if (!MyRadioAntennaComponent.s_Instance) 
		{
			return;
		}
		
		b_state = true;
		
		if (m_radioStation == null) {
			m_radioStation = MyRadioAntennaComponent.s_Instance.GetRadioStation(m_radioStationIndex);
		}
		
		MyRadioAntennaComponent.s_Instance.Connect(this);
		
		StartPlay();
		
		SetEventMask(m_owner, EntityEvent.FRAME);
	}
	
	/** Disable Radio / Executed on Server **/
	void Disable() {
		b_state = false;	

		MyRadioAntennaComponent.s_Instance.Disconnect(this);
	
		StopPlay();
		SetEventMask(m_owner, EntityEvent.INIT);
	}
	
	/** on state change after BumpMe **/
	protected void onRplSetEnableDisable() 
	{
		if (b_state)
		{
			Enable();
		} else {
			Disable();
		}
	}
	
	void ChangeStation() {
		if (!b_state || !MyRadioAntennaComponent.s_Instance) {
			return;
		}
				
		StopPlay();
		
		++m_radioStationIndex;
		
		if (m_radioStationIndex >= MyRadioAntennaComponent.s_Instance.GetRadiostaionsCount()) 
		{
			m_radioStationIndex = 0;
		}
		
		m_radioStation = MyRadioAntennaComponent.s_Instance.GetRadioStation(m_radioStationIndex);
		
		m_owner.GetTransform(m_ownerTransform);
		PlayChangeSound();
		StartPlay();			
	}
	
	void PlayChangeSound()
	{
		AudioSystem.PlayEvent("{0373401D5C6607B1}Sounds/RadioBroadcast/RadioBroadcast.acp", "SOUND_RADIO_TURN_ON", m_ownerTransform, {}, {});
	}
	
	void StartPlay() 
	{
		if (m_radioStation == null || !MyRadioAntennaComponent.s_Instance) 
		{
			return;
		}
		
		MyRadioStationTrackInfo trackInfo = MyRadioAntennaComponent.s_Instance.GetRadioStationTrack(m_radioStationIndex);
		
		if (!trackInfo)		
		{
			PrintFormat("Cant get track info. RadioStation index: %1", m_radioStationIndex);
			return;
		}
		
		array<string> signalNames = {};
		array<float> signalValues = {};
		
		GetPlaySignals(trackInfo, signalNames, signalValues);
		
		m_owner.GetTransform(m_ownerTransform);
		// Play event
		m_currentTrack = AudioSystem.PlayEvent(trackInfo.m_projectFile, SOUND_EVENT_NAME, m_ownerTransform, signalNames, signalValues);
	}
	
	protected void GetPlaySignals(MyRadioStationTrackInfo trackInfo, out array<string> signalNames, out array<float> signalValues) 
	{
		float trackOffset = MyRadioAntennaComponent.s_Instance.GetRadioStationTrackOffset(m_radioStationIndex);

		signalNames.Insert(BROADCAST_TYPE_SIGNAL);
		signalNames.Insert(OFFSET_SIGNAL);
		
		if (trackInfo.b_isDJ)
		{
			signalNames.Insert(DJ_INDEX_SIGNAL);
			signalValues.Insert(0);
		}
		else 
		{
			signalNames.Insert(MUSIC_INDEX_SIGNAL);
			signalValues.Insert(1);
		}
		
		signalValues.Insert(trackOffset);
		signalValues.Insert(trackInfo.m_trackIndex);	
		
		signalNames.Insert(VOLUME_SIGNAL);
		signalValues.Insert(GetVolume() / m_volumeToSignalDivider);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		GameSignalsManager gameSignalsManager = GetGame().GetSignalsManager();
		
		signalNames.Insert(SCR_SoundManagerEntity.G_INTERIOR_SIGNAL_NAME);
		signalNames.Insert(SCR_SoundManagerEntity.G_CURR_VEHICLE_COVERAGE_SIGNAL_NAME);
		signalNames.Insert(SCR_SoundManagerEntity.G_IS_THIRD_PERSON_CAM_SIGNAL_NAME);
		signalNames.Insert(SCR_SoundManagerEntity.G_ROOM_SIZE);
		
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetGInteriorSignalIdx()));
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetGCurrVehicleCoverageSignalIdx()));
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetGIsThirdPersonCamSignalIdx()));
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetRoomSizeIdx()));	
	}
	
	void StopPlay() {
		AudioSystem.TerminateSound(m_currentTrack);
	}
	
	void ResetPlay()
	{
		StopPlay();

		if (!b_state || !MyRadioAntennaComponent.s_Instance)
		{
			return;
		}
		
		StartPlay();
	}
	
	float GetVolume()
	{
		return m_volume;
	}
	
	void ActionSetVolume(float volume)
	{
		volume = Math.Clamp(volume, 0, 100);
		
		if (volume == m_volume) return;
		
		m_volume = volume;		

		ResetPlay();
	}
	
	void onRplSetVolume()
	{	
		ResetPlay();
	}
	
	protected void OnVehicleDamaged(SCR_VehicleDamageManagerComponent damageManager)
	{
		Print(damageManager.GetOwner().GetID());
	
		//Vehicle was repaired.
		if (damageManager.GetState() != EDamageState.DESTROYED)
			return;
		
		IEntity vehicle = damageManager.GetOwner();
	
		ActionsManagerComponent actionsManager = ActionsManagerComponent.Cast(vehicle.FindComponent(ActionsManagerComponent));
	
		if (!actionsManager) return;
	
		//array<EntitySlotInfo> vehicleSlotInfos = {};
		UserActionContext context = actionsManager.GetContext("CustomRadio");
	
		if (!context) return;
	
		array<BaseUserAction> outActions = {};
	
		context.GetActionsList(outActions);		
	
		MyRadioAction foundedAction;
	
		foreach (BaseUserAction action: outActions)
		{
			if (action.Type() == MyRadioAction) 
			{
				foundedAction = MyRadioAction.Cast(action);
				break;
			}
		}
	
		if (!foundedAction || foundedAction.m_RadioComponent != this) return;
		
		StopPlay();
	}
}