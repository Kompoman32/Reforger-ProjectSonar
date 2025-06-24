enum CustomRadioAntennaDebugActionEnum 
{
	UpdateRadios = 1,
	UpdateTrack = 2,
}

class CustomRadioAntennaDebugAction: ScriptedUserAction
{
	IEntity m_owner;
	
	CustomRadioAntennaSystem m_RadioSystem;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(CustomRadioAntennaDebugActionEnum))]
	EAudioSourceConfigurationFlag m_eActionType;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		m_owner = pOwnerEntity;
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = CustomRadioAntennaSystem.Cast(world.FindSystem(CustomRadioAntennaSystem));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return false;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
		return true;
	}

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {
		if (Replication.IsClient()) return;
		
					
		switch (m_eActionType)
		{
			case CustomRadioAntennaDebugActionEnum.UpdateRadios:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.Debug_UpdateRadios();
				}
				break;
			}
			
			case CustomRadioAntennaDebugActionEnum.UpdateTrack:
			{
				UpdateTrackOnRadios();
				break;
			}
		}
	}
	
	void UpdateTrackOnRadios() 
	{
		if (!m_RadioSystem) return;
		
		ActionsManagerComponent am = ActionsManagerComponent.Cast(m_owner.FindComponent(ActionsManagerComponent));
		
		if (!am) return;
		
		CustomRadioAntennaChangeStationDebugAction action = CustomRadioAntennaChangeStationDebugAction.Cast(am.FindAction(1));
		
		if (!action) return;
				
		m_RadioSystem.Debug_UpdateTrack(action.m_selectedRadioIndex);
		
		GetGame().GetCallqueue().CallLater(m_RadioSystem.Debug_UpdateTrack_2, 100, false, action.m_selectedRadioIndex);		
	}
}
