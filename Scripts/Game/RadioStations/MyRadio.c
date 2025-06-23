class MyRadioComponentClass: ScriptComponentClass
{
}

class MyRadioComponent: ScriptComponent
{	
	static string SOUND_EVENT_NAME = "SOUND_CUSTOM_RADIO";
		
	static string DJ_INDEX_SIGNAL= "DJTrackID";
	static string MUSIC_INDEX_SIGNAL= "MusicTrackID";
	static string OFFSET_SIGNAL= "Offset";
	static string BROADCAST_TYPE_SIGNAL= "BroadcastType";
	static string VOLUME_SIGNAL= "RadioVolume";
	
	protected float m_volumeToSignalDivider = 50;
	protected float m_maxAudibaleDistance = 200;
	
	
	protected SignalsManagerComponent m_signalManager;
	
	protected IEntity m_owner;
	protected vector m_ownerTransform[4];
	protected bool b_lastIsAudiable = false;
	protected AudioHandle m_currentTrack = AudioHandle.Invalid;
	
	MyRadioAntennaSystem m_radioSystem;
	CustomRadioStation m_radioStation;
	
	[RplProp()]
	int m_radioStationIndex = 0;	
	[RplProp()]
	protected bool b_state;
	[RplProp()]
	protected float m_volume = 50;
	
	
	float m_signalInterionValue = 0;
	float m_signalRoomSizeValue = 0;
	
	protected void ~MyRadioComponent() 
	{
		EOnDeactivate(m_owner);
	}
	
	override void OnPostInit(IEntity owner)
	{		
		m_owner = owner;
		m_signalManager = SignalsManagerComponent.Cast(m_owner.FindComponent(SignalsManagerComponent));
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_radioSystem = MyRadioAntennaSystem.Cast(world.FindSystem(MyRadioAntennaSystem));
		}
		
		m_owner.GetTransform(m_ownerTransform);
		calculateInteriorSignalValues();
		
		EOnActivate(owner);
	}
	
	override void EOnActivate(IEntity owner) 
	{		
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Insert(OnVehicleDamaged);
		

		if (!m_radioSystem) return;
		
		if (b_state) {			
			m_radioSystem.Connect(this);
			StartPlay();
		}
	}
	
	override void EOnDeactivate(IEntity owner) 
	{		
		ScriptInvoker onVehicleDamageStateChanged = SCR_VehicleDamageManagerComponent.GetOnVehicleDamageStateChanged();
		if (onVehicleDamageStateChanged)
			onVehicleDamageStateChanged.Remove(OnVehicleDamaged);
		
		if (!m_radioSystem) return;
		
		m_radioSystem.Disconnect(this);
		StopPlay();
	}
	
	override void EOnFrame(IEntity owner, float timeSlice) 
	{
		if (!Enabled() || !m_radioSystem) return;
		
		m_owner.GetTransform(m_ownerTransform);
		AudioSystem.SetSoundTransformation(m_currentTrack, m_ownerTransform);
	}
		
	protected void calculateInteriorSignalValues() 
	{
		SoundWorld m_SoundWorld;
		ChimeraWorld chimeraWorld = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (chimeraWorld)
		{
			m_SoundWorld = chimeraWorld.GetSoundWorld();
		}
		
		if (m_SoundWorld) 
		{				
			RT_RadioInteriorRequestCallback rirc = new RT_RadioInteriorRequestCallback(this);
			
			m_SoundWorld.CalculateInterirorAt(m_ownerTransform, rirc);	
		}			
	}
	
	bool Enabled() {
		return b_state;
	}
	
	void SetRadioStationVar() {
		if (!m_radioSystem) return;
		
		m_radioStation = m_radioSystem.GetRadioStation(m_radioStationIndex);
	}
	
	void ActionEnableDisable(bool enable)
	{		
		if (Replication.IsClient()) {
			return;
		}			
		
		b_state = enable;

		ActionReset();
	}
	
	void ActionChangeStation() {		
		if (Replication.IsClient()) {
			return;
		}

		
		if (!b_state || !m_radioSystem) {
			return;
		}
		
		++m_radioStationIndex;
		
		if (m_radioStationIndex >= m_radioSystem.GetRadiostaionsCount()) 
		{
			m_radioStationIndex = 0;
		}
		
		ActionReset();
	}
	
	void ActionReset()
	{			
		if (Replication.IsClient()) {
			return;
		}		
		

		Rpc(RpcDo_Reset, b_state, m_volume, m_radioStationIndex);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_Reset(bool state, float volume, int radioStationIndex)
	{			
		b_state = state;
		m_volume = volume;
		m_radioStationIndex = radioStationIndex;
		SetRadioStationVar();
	
		StopPlay();
	
		if (b_state)
		{
			Enable();
		} else {
			Disable();
		}
	}
	
	void ActionSetVolume(float volume)
	{		
		volume = Math.Clamp(volume, 0, 100);
		
		if (volume == m_volume) return;
		
		m_volume = volume;
		
		if (Replication.IsClient()) {
			ResetPlay();			
		}
		
		GetGame().GetCallqueue().Remove(ActionReset);
		GetGame().GetCallqueue().CallLater(ActionReset, 300, false);
	}
	
	void Enable() {		
		if (!m_radioSystem) 
		{
			return;
		}
		
		b_state = true;	
	
		SetRadioStationVar();
		
		m_radioSystem.Connect(this);
		
		StartPlay();
		
		SetEventMask(m_owner, EntityEvent.FRAME);
	}
	
	void Disable() {
		b_state = false;	
		b_lastIsAudiable = false;

		m_radioSystem.Disconnect(this);
	
		StopPlay();
		SetEventMask(m_owner, EntityEvent.INIT);
	}
	
	void PlayChangeSound()
	{
		AudioSystem.PlayEvent("{0373401D5C6607B1}Sounds/RadioBroadcast/RadioBroadcast.acp", "SOUND_RADIO_TURN_ON", m_ownerTransform, {}, {});
	}
	
	void StartPlay() 
	{
		if (!m_radioSystem) 
		{
			return;
		}
	
		SetRadioStationVar();
	
		if (Replication.IsClient()) {
	
			GetGame().GetCallqueue().Remove(CheckSoundDistance);
			GetGame().GetCallqueue().CallLater(CheckSoundDistance, 1000, true);
		}
		
		MyRadioStationTrackInfo trackInfo = m_radioSystem.GetRadioStationTrack(m_radioStationIndex);
		
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
		float trackOffset = m_radioSystem.GetRadioStationTrackOffset(m_radioStationIndex);

		signalNames.Insert(BROADCAST_TYPE_SIGNAL);
		signalNames.Insert(DJ_INDEX_SIGNAL);	
		signalNames.Insert(MUSIC_INDEX_SIGNAL);
		
		if (trackInfo.b_isDJ)
		{
			signalValues.Insert(0);
			signalValues.Insert(trackInfo.m_trackIndex);
			signalValues.Insert(0);
		}
		else 
		{
			signalValues.Insert(1);
			signalValues.Insert(0);
			signalValues.Insert(trackInfo.m_trackIndex);
		}
	
		signalNames.Insert(OFFSET_SIGNAL);
		signalValues.Insert(trackOffset);
		
		signalNames.Insert(VOLUME_SIGNAL);
		signalValues.Insert(GetVolume() / m_volumeToSignalDivider);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		GameSignalsManager gameSignalsManager = GetGame().GetSignalsManager();	
	
		signalNames.Insert(SCR_SoundManagerEntity.G_INTERIOR_SIGNAL_NAME);
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetGInteriorSignalIdx()));
	
	
		signalNames.Insert(SCR_SoundManagerEntity.G_ROOM_SIZE);
		signalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetRoomSizeIdx()));
	
		
		signalNames.Insert(SCR_AudioSource.INTERIOR_SIGNAL_NAME);
		signalValues.Insert(gameSignalsManager.GetSignalValue(m_signalInterionValue));
	
		signalNames.Insert(SCR_AudioSource.ROOM_SIZE_SIGNAL_NAME);
		signalValues.Insert(gameSignalsManager.GetSignalValue(m_signalRoomSizeValue));		
	}
	
	void StopPlay() {
		AudioSystem.TerminateSound(m_currentTrack);
	}
	
	void ResetPlay()
	{	
		StopPlay();

		if (!b_state || !m_radioSystem)
		{
			return;
		}
		
		StartPlay();
	}
	
	float GetVolume()
	{
		return m_volume;
	}
	
	protected void OnVehicleDamaged(SCR_VehicleDamageManagerComponent damageManager)
	{
		if (damageManager.GetState() != EDamageState.DESTROYED)
			return;
		
		IEntity vehicle = damageManager.GetOwner();
	
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(vehicle.FindComponent(SlotManagerComponent));
		if (!slotManager) return;
				EntitySlotInfo slot = slotManager.GetSlotByName("RADIO");
		if (!slot) {
			slot = slotManager.GetSlotByName("radio");
		};
		if (!slot) return;
		IEntity attachedEntity = slot.GetAttachedEntity();
		if (!attachedEntity) return;

		MyRadioComponent radio = MyRadioComponent.Cast(attachedEntity.FindComponent(MyRadioComponent));
	
		if (!radio) return;
	
		Disable();
	}
	
	protected void CheckSoundDistance()
	{
		if (!b_state) return;
	
		if (m_volume == 0) return;
	
		MyRadioStationTrackInfo trackInfo = m_radioSystem.GetRadioStationTrack(m_radioStationIndex);
	
		if (!trackInfo) return;
	
		vector playerPos;
	
		SCR_EditorManagerEntity localEditorManager = SCR_EditorManagerEntity.GetInstance();
		SCR_ManualCamera camera = SCR_CameraEditorComponent.GetCameraInstance();
	
		if (localEditorManager && localEditorManager.GetPlayerID() == SCR_PlayerController.GetLocalPlayerId() && camera) {
			vector gmPos[4];
			camera.GetWorldTransform(gmPos);
	
			playerPos = gmPos[3];	
		} else {
			ChimeraCharacter playerEntity = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
			if (!playerEntity) return;
		
			SCR_EditableCharacterComponent ec = SCR_EditableCharacterComponent.Cast(playerEntity.FindComponent(SCR_EditableCharacterComponent));
		
			if (!ec) return;

			
			ec.GetPos(playerPos);
		}	
	
		vector radioPos[4];
		m_owner.GetWorldTransform(radioPos);
	
		float distance = vector.Distance(radioPos[3], playerPos);
		bool isAudible = distance < m_maxAudibaleDistance;	
	
		//PrintFormat("CheckSoundDistance: l:%1 = %2 (%3) (%4 | %5)", b_lastIsAudiable , isAudible, distance, radioPos[3], playerPos);
		//PrintFormat("CheckSoundDistance: l:%1 = %2 (%3) t:%4 %5", b_lastIsAudiable , isAudible, distance, m_currentTrack, AudioSystem.IsSoundPlayed(m_currentTrack));
	
		if (!b_lastIsAudiable && isAudible) {
			ResetPlay();
		}
	
		b_lastIsAudiable  = isAudible;
	
		if (m_currentTrack == AudioHandle.Invalid) {
			b_lastIsAudiable = false;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteBool(b_state);
		writer.WriteInt(m_radioStationIndex);
		writer.WriteInt(m_volume);
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool RplLoad(ScriptBitReader reader)
	{
		bool state;
		int radioStationIndex, volume;
		bool aloneInGroup;
		
		reader.ReadBool(state);
		reader.ReadInt(radioStationIndex);
		reader.ReadInt(volume);
	
	
		GetGame().GetCallqueue().Remove(RpcDo_Reset);
		GetGame().GetCallqueue().CallLater(RpcDo_Reset, 1000, false, state, volume, radioStationIndex);
		
		return true;
	}
}