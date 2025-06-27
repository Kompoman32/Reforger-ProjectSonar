class RT_PS_CustomRadioComponentClass: ScriptComponentClass
{
}

class RT_PS_CustomRadioComponent: ScriptComponent
{	
	[RplProp()]
	int m_iRadioStationIndex = 0;	
	[RplProp()]
	protected bool m_bState;
	[RplProp()]
	protected float m_fVolume = 50;

	RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	RT_PS_CustomRadioStation m_RadioStation;
	
	float m_fSignalInterionValue = 0;
	float m_fSignalRoomSizeValue = 0;

	protected IEntity m_Owner;
	protected vector m_vOwnerTransform[4];

	protected SignalsManagerComponent m_SignalManager;

	protected float m_fMaxAudibaleDistance = 200;
	protected bool m_bLastIsAudiable = false;
	
	protected AudioHandle m_CurrentTrack = AudioHandle.Invalid;
	protected float m_fVolumeToSignalDivider = 50;
		
	static const string SOUND_EVENT_NAME = "SOUND_CUSTOM_RADIO";
	static const string DJ_INDEX_SIGNAL= "DJTrackID";
	static const string MUSIC_INDEX_SIGNAL= "MusicTrackID";
	static const string OFFSET_SIGNAL= "Offset";
	static const string BROADCAST_TYPE_SIGNAL= "BroadcastType";
	static const string VOLUME_SIGNAL= "RadioVolume";
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{		
		m_Owner = owner;
		m_SignalManager = SignalsManagerComponent.Cast(m_Owner.FindComponent(SignalsManagerComponent));
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
		
		m_Owner.GetTransform(m_vOwnerTransform);
		CalculateInteriorSignalValues();
		
		EOnActivate(owner);
	}
		
	//------------------------------------------------------------------------------------------------
	protected void CalculateInteriorSignalValues() 
	{
		SoundWorld m_SoundWorld;
		ChimeraWorld chimeraWorld = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (chimeraWorld)
		{
			m_SoundWorld = chimeraWorld.GetSoundWorld();
		}
		
		if (m_SoundWorld) 
		{				
			RT_PS_CustomRadioInteriorRequestCallback rirc = new RT_PS_CustomRadioInteriorRequestCallback(this);
			
			m_SoundWorld.CalculateInterirorAt(m_vOwnerTransform, rirc);	
		}			
	}
	
	//------------------------------------------------------------------------------------------------
	bool Enabled() {
		return m_bState;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SetRadioStationVar() {
		if (!m_RadioSystem) return;
		
		m_RadioStation = m_RadioSystem.GetRadioStation(m_iRadioStationIndex);
	}
	
	//------------------------------------------------------------------------------------------------
	void ActionEnableDisable(bool pEnable)
	{		
		if (Replication.IsClient()) {
			return;
		}			
		
		m_bState = pEnable;

		ActionReset();
	}
	
	//------------------------------------------------------------------------------------------------
	void ActionChangeStation() {		
		if (Replication.IsClient()) {
			return;
		}

		
		if (!m_bState || !m_RadioSystem) {
			return;
		}
		
		++m_iRadioStationIndex;
		
		if (m_iRadioStationIndex >= m_RadioSystem.GetRadiostaionsCount()) 
		{
			m_iRadioStationIndex = 0;
		}
		
		ActionReset();
	}
	
	//------------------------------------------------------------------------------------------------
	void ActionReset()
	{			
		if (Replication.IsClient()) {
			return;
		}		

		Rpc(RpcDo_Reset, m_bState, m_fVolume, m_iRadioStationIndex);
		RpcDo_Reset(m_bState, m_fVolume, m_iRadioStationIndex);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_Reset(bool pState, float pVolume, int pRadioStationIndex)
	{			
		m_bState = pState;
		m_fVolume = pVolume;
		m_iRadioStationIndex = pRadioStationIndex;
		SetRadioStationVar();
	
		StopPlay();
	
		if (m_bState)
		{
			Enable();
		} else {
			Disable();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ActionSetVolume(float pVolume)
	{		
		pVolume = Math.Clamp(pVolume, 0, 100);
		
		if (pVolume == m_fVolume) return;
		
		m_fVolume = pVolume;
		
		ResetPlay();
		
		GetGame().GetCallqueue().Remove(ActionReset);
		GetGame().GetCallqueue().CallLater(ActionReset, 300, false);
	}
	
	//------------------------------------------------------------------------------------------------
	void Enable() {		
		if (!m_RadioSystem) 
		{
			return;
		}
		
		m_bState = true;	
	
		SetRadioStationVar();
		
		m_RadioSystem.Connect(this);
		
		StartPlay();
		
		SetEventMask(m_Owner, EntityEvent.FRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	void Disable() {
		m_bState = false;	
		m_bLastIsAudiable = false;

		m_RadioSystem.Disconnect(this);
	
		StopPlay();
		SetEventMask(m_Owner, EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void PlayChangeSound()
	{
		AudioSystem.PlayEvent("{0373401D5C6607B1}Sounds/RadioBroadcast/RadioBroadcast.acp", "SOUND_RADIO_TURN_ON", m_vOwnerTransform, {}, {});
	}
	
	//------------------------------------------------------------------------------------------------
	void StartPlay() 
	{
		if (!m_RadioSystem || !m_RadioSystem || !m_RadioSystem.m_bAllowRadios) 
		{
			return;
		}
	
		SetRadioStationVar();
	
		//if (Replicati1on.IsClient()) {
			GetGame().GetCallqueue().Remove(CheckSoundDistance);
			GetGame().GetCallqueue().CallLater(CheckSoundDistance, 1000, true);
		//}
		
		RT_PS_CustomRadioStationTrackInfo trackInfo = m_RadioSystem.GetRadioStationTrack(m_iRadioStationIndex);
		
		if (!trackInfo)		
		{
			PrintFormat("Cant get track info. RadioStation index: %1", m_iRadioStationIndex);
			return;
		}
		
		array<string> signalNames = {};
		array<float> signalValues = {};
		
		GetPlaySignals(trackInfo, signalNames, signalValues);
		
		m_Owner.GetTransform(m_vOwnerTransform);
	
		// Play event
		m_CurrentTrack = AudioSystem.PlayEvent(trackInfo.m_sProjectFile, SOUND_EVENT_NAME, m_vOwnerTransform, signalNames, signalValues);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void GetPlaySignals(RT_PS_CustomRadioStationTrackInfo pTrackInfo, out array<string> pSignalNames, out array<float> pSignalValues) 
	{
		float trackOffset = m_RadioSystem.GetRadioStationTrackOffset(m_iRadioStationIndex);

		pSignalNames.Insert(BROADCAST_TYPE_SIGNAL);
		pSignalNames.Insert(DJ_INDEX_SIGNAL);	
		pSignalNames.Insert(MUSIC_INDEX_SIGNAL);
		
		if (pTrackInfo.m_bIsDJ)
		{
			pSignalValues.Insert(0);
			pSignalValues.Insert(pTrackInfo.m_iTrackIndex);
			pSignalValues.Insert(0);
		}
		else 
		{
			pSignalValues.Insert(1);
			pSignalValues.Insert(0);
			pSignalValues.Insert(pTrackInfo.m_iTrackIndex);
		}
	
		pSignalNames.Insert(OFFSET_SIGNAL);
		pSignalValues.Insert(trackOffset);
		
		pSignalNames.Insert(VOLUME_SIGNAL);
		
		float volume = GetVolume();
		volume /= m_fVolumeToSignalDivider;
		
		
		BaseContainer settings = GetGame().GetGameUserSettings().GetModule("SCR_AudioSettings");    		
		if (settings) 
		{
			float settingsVolume;
			settings.Get("m_fRadioVolumeBoost", settingsVolume);
			settingsVolume /= 100;
			volume *= settingsVolume;			
		}
		
		pSignalValues.Insert(volume);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		GameSignalsManager gameSignalsManager = GetGame().GetSignalsManager();	
	
		pSignalNames.Insert(SCR_SoundManagerEntity.G_INTERIOR_SIGNAL_NAME);
		pSignalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetGInteriorSignalIdx()));
	
		pSignalNames.Insert(SCR_SoundManagerEntity.G_ROOM_SIZE);
		pSignalValues.Insert(gameSignalsManager.GetSignalValue(soundManagerEntity.GetRoomSizeIdx()));
		
		pSignalNames.Insert(SCR_AudioSource.INTERIOR_SIGNAL_NAME);
		pSignalValues.Insert(gameSignalsManager.GetSignalValue(m_fSignalInterionValue));
	
		pSignalNames.Insert(SCR_AudioSource.ROOM_SIZE_SIGNAL_NAME);
		pSignalValues.Insert(gameSignalsManager.GetSignalValue(m_fSignalRoomSizeValue));		
	}
	
	//------------------------------------------------------------------------------------------------
	void StopPlay() {
		AudioSystem.TerminateSound(m_CurrentTrack);
	}
	
	//------------------------------------------------------------------------------------------------
	void ResetPlay()
	{	
		StopPlay();

		if (!m_bState || !m_RadioSystem)
		{
			return;
		}
		
		StartPlay();
	}
	
	//------------------------------------------------------------------------------------------------
	float GetVolume()
	{
		return m_fVolume;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CheckSoundDistance()
	{
		if (!m_bState) return;

		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		GameSignalsManager gameSignalsManager = GetGame().GetSignalsManager();
	
		if (m_fVolume == 0) return;
	
		RT_PS_CustomRadioStationTrackInfo trackInfo = m_RadioSystem.GetRadioStationTrack(m_iRadioStationIndex);
	
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
		m_Owner.GetWorldTransform(radioPos);
	
		float distance = vector.Distance(radioPos[3], playerPos);
		bool isAudible = distance < m_fMaxAudibaleDistance;	
	
		//PrintFormat("CheckSoundDistance: l:%1 = %2 (%3) (%4 | %5)", m_bLastIsAudiable , isAudible, distance, radioPos[3], playerPos);
		//PrintFormat("CheckSoundDistance: l:%1 = %2 (%3) t:%4 %5", m_bLastIsAudiable , isAudible, distance, m_CurrentTrack, AudioSystem.IsSoundPlayed(m_CurrentTrack));
	
		if (!m_bLastIsAudiable && isAudible) {
			ResetPlay();
		}
	
		m_bLastIsAudiable  = isAudible;
	
		if (m_CurrentTrack == AudioHandle.Invalid) {
			m_bLastIsAudiable = false;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteBool(m_bState);
		writer.WriteInt(m_iRadioStationIndex);
		writer.WriteInt(m_fVolume);
		
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

	//------------------------------------------------------------------------------------------------
	override void EOnActivate(IEntity owner) 
	{		
		if (!m_RadioSystem) return;
		
		if (m_bState) {			
			m_RadioSystem.Connect(this);
			StartPlay();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnDeactivate(IEntity owner) 
	{				
		if (!m_RadioSystem) return;
		
		m_RadioSystem.Disconnect(this);
		StopPlay();
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice) 
	{
		if (!Enabled() || !m_RadioSystem) return;
		
		m_Owner.GetTransform(m_vOwnerTransform);
		AudioSystem.SetSoundTransformation(m_CurrentTrack, m_vOwnerTransform);
	}

	//------------------------------------------------------------------------------------------------
	protected void ~RT_PS_CustomRadioComponent() 
	{
		EOnDeactivate(m_Owner);
	}
}