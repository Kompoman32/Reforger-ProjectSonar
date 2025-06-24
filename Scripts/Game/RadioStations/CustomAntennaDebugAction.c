enum CustomRadioAntennaDebugActionEnum 
{
	UpdateRadios = 1,
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
		switch (m_eActionType)
		{
			case CustomRadioAntennaDebugActionEnum.UpdateRadios:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.AskTo_UpdateTracks();
				}
			}
		}
	}
}
