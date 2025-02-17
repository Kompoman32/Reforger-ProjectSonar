enum MyAntennaDebugActionEnum 
{
	UpdateRadios = 1,
}

class MyAntennaDebugAction: ScriptedUserAction
{
	MyRadioAntennaSystem m_RadioSystem;
	MyRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(MyAntennaDebugActionEnum))]
	EAudioSourceConfigurationFlag m_eActionType;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = MyRadioAntennaSystem.Cast(world.FindSystem(MyRadioAntennaSystem));
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
			case MyAntennaDebugActionEnum.UpdateRadios:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.AskTo_UpdateTracks();
				}
			}
		}
	}
}
