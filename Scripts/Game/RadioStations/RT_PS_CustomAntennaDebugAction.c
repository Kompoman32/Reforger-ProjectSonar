enum RT_PS_ECustomRadioAntennaDebugAction 
{
	UPDATE_RADIOS = 1,
	UPDATE_TRACK = 2,
}

class RT_PS_CustomRadioAntennaDebugAction: ScriptedUserAction
{
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(RT_PS_ECustomRadioAntennaDebugAction))]
	RT_PS_ECustomRadioAntennaDebugAction m_eActionType;

	protected IEntity m_Owner;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		m_Owner = pOwnerEntity;
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
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

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {
		if (Replication.IsClient()) return;
		
					
		switch (m_eActionType)
		{
			case RT_PS_ECustomRadioAntennaDebugAction.UPDATE_RADIOS:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.Debug_UpdateRadios();
				}
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.UPDATE_TRACK:
			{
				UpdateTrackOnRadios();
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateTrackOnRadios() 
	{
		if (!m_RadioSystem) return;
		
		ActionsManagerComponent am = ActionsManagerComponent.Cast(m_Owner.FindComponent(ActionsManagerComponent));
		
		if (!am) return;
		
		RT_PS_CustomRadioAntennaChangeStationDebugAction action = RT_PS_CustomRadioAntennaChangeStationDebugAction.Cast(am.FindAction(1));
		
		if (!action) return;
				
		m_RadioSystem.Debug_UpdateTrack(action.m_iSelectedRadioIndex);
		
		GetGame().GetCallqueue().CallLater(m_RadioSystem.Debug_UpdateTrack_2, 100, false, action.m_iSelectedRadioIndex);		
	}
}
