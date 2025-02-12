enum MyAntennaDebugActionEnum 
{
	Clear = 0,
	UpdateRadios = 1
}

class MyAntennaDebugAction: ScriptedUserAction
{
	MyRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(MyAntennaDebugActionEnum))]
	EAudioSourceConfigurationFlag m_eActionType;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);	
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
			case MyAntennaDebugActionEnum.Clear:
			{
				if (MyRadioAntennaComponent.s_Instance) 
				{
					MyRadioAntennaComponent.s_Instance.SetEventMask(MyRadioAntennaComponent.s_Instance.m_owner, EntityEvent.INIT);
					
					Print("Instance Cleared");
				}
				
				MyRadioAntennaComponent.s_Instance = null;
				
				break;
			}
			
			case MyAntennaDebugActionEnum.UpdateRadios:
			{
				if (MyRadioAntennaComponent.s_Instance) 
				{
					MyRadioAntennaComponent.s_Instance.AskTo_UpdateTracks();
				}
			}
		}
	}
}
