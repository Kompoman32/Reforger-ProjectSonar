enum MyRadioActionEnum 
{
	TurnOnOff = 0,
	Change = 1,
	Reset = 2,
}

class MyRadioAction: ScriptedUserAction
{
	IEntity p_OwnerEntity;
	MyRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(MyRadioActionEnum))]
	MyAntennaDebugActionEnum m_eActionType;
	
	[Attribute("1", UIWidgets.CheckBox)]
	bool m_inVehicle;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		p_OwnerEntity = pOwnerEntity;

		m_RadioComponent = MyRadioComponent.Cast(pOwnerEntity.FindComponent(MyRadioComponent));
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(pOwnerEntity);
	}
	
	override bool CanBroadcastScript()
	{
		return m_eActionType != MyRadioActionEnum.Reset;
	}
	
	void FindRadioComponent(IEntity pOwnerEntity)
	{		
		if (m_RadioComponent) return;
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pOwnerEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return;
		EntitySlotInfo slot = slotManager.GetSlotByName("RADIO");
		if (!slot) {
			slot = slotManager.GetSlotByName("radio");
		};
		if (!slot) return;
		IEntity attachedEntity = slot.GetAttachedEntity();
		if (!attachedEntity) return;

		m_RadioComponent = MyRadioComponent.Cast(attachedEntity.FindComponent(MyRadioComponent));
	}
	
	bool Enabled() {
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.Enabled();
	}
	
	override bool CanBeShownScript(IEntity user)
	{	
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(p_OwnerEntity);
		
		if (!m_RadioComponent || !MyRadioAntennaComponent.s_Instance) {
			return m_eActionType == MyRadioActionEnum.TurnOnOff;
		}
		
		if (!Enabled())
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
			case MyRadioActionEnum.Reset:
			{
				m_RadioComponent.Ask_Reset();
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
