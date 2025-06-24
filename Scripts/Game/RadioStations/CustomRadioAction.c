enum CustomRadioActionEnum 
{
	TurnOnOff = 0,
	Change = 1,
	Reset = 2,
}

class CustomRadioAction: ScriptedUserAction
{
	IEntity p_OwnerEntity;
	CustomRadioAntennaSystem m_RadioSystem;
	CustomRadioComponent m_RadioComponent;
	
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(CustomRadioActionEnum))]
	CustomRadioActionEnum m_eActionType;
	
	[Attribute("1", UIWidgets.CheckBox)]
	bool m_inVehicle;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		p_OwnerEntity = pOwnerEntity;

		m_RadioComponent = CustomRadioComponent.Cast(pOwnerEntity.FindComponent(CustomRadioComponent));
		
		if (m_inVehicle) {m_RadioComponent = null;}
		
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(pOwnerEntity);
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = CustomRadioAntennaSystem.Cast(world.FindSystem(CustomRadioAntennaSystem));
		}
	}
	
	override bool CanBroadcastScript()
	{
		return m_eActionType != CustomRadioActionEnum.Reset;
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

		m_RadioComponent = CustomRadioComponent.Cast(attachedEntity.FindComponent(CustomRadioComponent));
	}
	
	void FindRadioSystem()
	{
		
	}
	
	bool Enabled() {
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.Enabled();
	}
	
	override bool CanBeShownScript(IEntity user)
	{	
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(p_OwnerEntity);
		
		if (!m_RadioComponent || !m_RadioSystem) {
			return m_eActionType == CustomRadioActionEnum.TurnOnOff;
		}
		
		if (!Enabled())
		{
			if (m_eActionType == CustomRadioActionEnum.Change )
				return false;
		}
		
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		if (m_eActionType == CustomRadioActionEnum.TurnOnOff && Enabled())
		{
			return true;
		}
		
		if (!m_RadioComponent) 
		{
			m_sCannotPerformReason = "#Custom_Radio-CannotPerform_Radio";
			return false;
		}
		
		if (!m_RadioSystem)
		{
			m_sCannotPerformReason = "#Custom_Radio-CannotPerform_Antenna";
			return false;
		}	
		
		if (m_RadioSystem.GetRadiostaionsCount() == 0)
		{
			m_sCannotPerformReason = "#Custom_Radio-CannotPerform_Station";
			return false;
		}
		
		if (m_RadioComponent.Enabled() && m_eActionType == CustomRadioActionEnum.Change && m_RadioSystem.GetRadiostaionsCount() == 1)
		{
			m_sCannotPerformReason = "#Custom_Radio-CannotPerform_OneStation";
			return false;
		}
		
		return true;	
	}

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {			
		if (!m_RadioComponent) return;		
		
		switch (m_eActionType)
		{
			case CustomRadioActionEnum.TurnOnOff:
			{				
				m_RadioComponent.ActionEnableDisable(!Enabled());
				break;
			}
			case CustomRadioActionEnum.Change:
			{
				m_RadioComponent.ActionChangeStation();
				break;
			}
			case CustomRadioActionEnum.Reset:
			{
				m_RadioComponent.ActionReset();
				break;
			}
		}
	}
	
	override bool GetActionNameScript(out string outName)
	{		
		outName = GetUIInfo().GetName();
		
		switch (m_eActionType)
		{
			case CustomRadioActionEnum.TurnOnOff:
			{			
				if (Enabled())
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";

				return true;
			}
			case CustomRadioActionEnum.Change:
			{
				if (!m_RadioComponent || !m_RadioComponent.m_radioStation) return false;
				
				outName+= ": " + m_RadioComponent.m_radioStation.m_radiostationName;
				
				return true;
			}
		}

		return false;
	}
}
