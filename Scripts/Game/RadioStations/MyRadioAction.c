enum MyRadioActionEnum 
{
	TurnOnOff = 0,
	Change = 1
}

class MyRadioAction: ScriptedUserAction
{
	MyRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(MyRadioActionEnum))]
	EAudioSourceConfigurationFlag m_eActionType;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);

		m_RadioComponent = MyRadioComponent.Cast(pOwnerEntity.FindComponent(MyRadioComponent));		
	}
	
	bool GetState() {
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.GetState();
	}
	
	void SetState(bool enable) {
		if (!m_RadioComponent)
			return;
		
		if (GetState())
			m_RadioComponent.Disable();
		else
			m_RadioComponent.Enable();
		
	}
	
	override bool CanBeShownScript(IEntity user)
	{	
		if (m_RadioComponent && !m_RadioComponent.GetState())
		{
			if (m_eActionType == MyRadioActionEnum.Change )
				return false;
		}
		
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_RadioComponent) 
		{
			m_sCannotPerformReason = "#RT_Radio_CannotPerform_Radio";
			return false;
		}
		
		if (!MyRadioAntennaComponent.s_Instance)
		{
			m_sCannotPerformReason = "#RT_Radio_CannotPerform_Antenna";
			return false;
		}	
		
		if (MyRadioAntennaComponent.s_Instance.GetRadiostaionsCount() == 0)
		{
			m_sCannotPerformReason = "#RT_Radio_CannotPerform_Station";
			return false;
		}
		
		if (m_RadioComponent.GetState() && m_eActionType == MyRadioActionEnum.Change && MyRadioAntennaComponent.s_Instance.GetRadiostaionsCount() == 1)
		{
			m_sCannotPerformReason = "#RT_Radio_CannotPerform_OneStation";
			return false;
		}
		
		return true;	
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
		if (!m_RadioComponent) return;
		
		
		switch (m_eActionType)
		{
			case MyRadioActionEnum.TurnOnOff:
			{
				SetState(!GetState());
				break;
			}
			case MyRadioActionEnum.Change:
			{
				m_RadioComponent.Change();
				break;
			}
		}
	}
	
	override bool GetActionNameScript(out string outName)
	{		
		outName = GetUIInfo().GetName();
		
		switch (m_eActionType)
		{
			case MyRadioActionEnum.TurnOnOff:
			{			
				if (GetState())
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";
				return true;
			}
			case MyRadioActionEnum.Change:
			{
				if (!m_RadioComponent) return false;
				
				outName+= ": " + m_RadioComponent.m_radioStation.m_radiostationName;
				
				return true;
			}
		}

		return false;
	}
}
