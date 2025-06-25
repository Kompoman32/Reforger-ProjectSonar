enum RT_PS_ECustomRadioAction 
{
	TURN_ON_OFF = 0,
	CHANGE = 1,
	RESET = 2,
}

class RT_PS_CustomRadioAction: ScriptedUserAction
{
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(RT_PS_ECustomRadioAction))]
	RT_PS_ECustomRadioAction m_eActionType;
	
	[Attribute("1", UIWidgets.CheckBox)]
	bool m_bInVehicle;

	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	protected RT_PS_CustomRadioComponent m_RadioComponent;
	protected IEntity p_OwnerEntity;

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		p_OwnerEntity = pOwnerEntity;

		m_RadioComponent = RT_PS_CustomRadioComponent.Cast(pOwnerEntity.FindComponent(RT_PS_CustomRadioComponent));
		
		if (m_bInVehicle) {m_RadioComponent = null;}
		
		if (!m_RadioComponent && m_bInVehicle) FindRadioComponent(pOwnerEntity);
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
		return m_eActionType != RT_PS_ECustomRadioAction.RESET;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FindRadioComponent(IEntity pOwnerEntity)
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

		m_RadioComponent = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool Enabled()
	{
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.Enabled();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{	
		if (!m_RadioComponent && m_bInVehicle) FindRadioComponent(p_OwnerEntity);
		
		if (!m_RadioComponent || !m_RadioSystem) {
			return m_eActionType == RT_PS_ECustomRadioAction.TURN_ON_OFF;
		}
		
		if (!Enabled())
		{
			if (m_eActionType == RT_PS_ECustomRadioAction.CHANGE )
				return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		if (m_eActionType == RT_PS_ECustomRadioAction.TURN_ON_OFF && Enabled())
		{
			return true;
		}
		
		if (!m_RadioComponent) 
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Radio";
			return false;
		}
		
		if (!m_RadioSystem)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Antenna";
			return false;
		}	
		
		if (m_RadioSystem.GetRadiostaionsCount() == 0)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Station";
			return false;
		}
		
		if (m_RadioComponent.Enabled() && m_eActionType == RT_PS_ECustomRadioAction.CHANGE && m_RadioSystem.GetRadiostaionsCount() == 1)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_OneStation";
			return false;
		}
		
		return true;	
	}

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{			
		if (!m_RadioComponent) return;		
		
		switch (m_eActionType)
		{
			case RT_PS_ECustomRadioAction.TURN_ON_OFF:
			{				
				m_RadioComponent.ActionEnableDisable(!Enabled());
				break;
			}
			case RT_PS_ECustomRadioAction.CHANGE:
			{
				m_RadioComponent.ActionChangeStation();
				break;
			}
			case RT_PS_ECustomRadioAction.RESET:
			{
				m_RadioComponent.ActionReset();
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{		
		outName = GetUIInfo().GetName();
		
		switch (m_eActionType)
		{
			case RT_PS_ECustomRadioAction.TURN_ON_OFF:
			{			
				if (Enabled())
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";

				return true;
			}
			case RT_PS_ECustomRadioAction.CHANGE:
			{
				if (!m_RadioComponent || !m_RadioComponent.m_RadioStation) return false;
				
				outName+= ": " + m_RadioComponent.m_RadioStation.m_sRadiostationName;
				
				return true;
			}
		}

		return false;
	}
}
