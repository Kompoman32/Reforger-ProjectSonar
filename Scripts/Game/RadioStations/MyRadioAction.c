enum MyRadioActionEnum 
{
	TurnOnOff = 0,
	Change = 1
}

class MyRadioAction: ScriptedUserAction
{
	MyRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(MyRadioActionEnum))]
	MyAntennaDebugActionEnum m_eActionType;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);

		m_RadioComponent = MyRadioComponent.Cast(pOwnerEntity.FindComponent(MyRadioComponent));		
	}
	
	bool Enabled() {
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.Enabled();
	}
	
	override bool CanBeShownScript(IEntity user)
	{	
		if (m_RadioComponent && !Enabled())
		{
			if (m_eActionType == MyRadioActionEnum.Change )
				return false;
		}
		
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		if (m_eActionType == MyRadioActionEnum.TurnOnOff && Enabled())
		{
			return true;
		}
		
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
		
		if (m_RadioComponent.Enabled() && m_eActionType == MyRadioActionEnum.Change && MyRadioAntennaComponent.s_Instance.GetRadiostaionsCount() == 1)
		{
			m_sCannotPerformReason = "#RT_Radio_CannotPerform_OneStation";
			return false;
		}
		
		return true;	
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
				m_RadioComponent.ActionEnableDisable(!Enabled());
				break;
			}
			case MyRadioActionEnum.Change:
			{
				m_RadioComponent.ChangeStation();
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
				if (Enabled())
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";
				
				outName+= " " + MyRadioAntennaComponent.s_Instance.GetRadioStationTrackTimeLeft(m_RadioComponent.m_radioStationIndex);

				return true;
			}
			case MyRadioActionEnum.Change:
			{
				if (!m_RadioComponent || !m_RadioComponent.m_radioStation) return false;
				
				outName+= ": " + m_RadioComponent.m_radioStation.m_radiostationName;
				
				return true;
			}
		}

		return false;
	}
}
