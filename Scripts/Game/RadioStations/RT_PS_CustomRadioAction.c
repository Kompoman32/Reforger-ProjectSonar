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
	
	[Attribute("0", UIWidgets.CheckBox)]
	bool m_bInStaticProp;
	
	[Attribute("CustomRadio", UIWidgets.EditBox)]
	string m_sSlotName;

	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	protected RT_PS_CustomRadioComponent m_RadioComponent;
	protected IEntity p_OwnerEntity;
	
	static const ref array<string> PossibleSlotName = {"CustomRadio", "RADIO", "radio"};

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		p_OwnerEntity = pOwnerEntity;

		m_RadioComponent = RT_PS_CustomRadioComponent.Cast(pOwnerEntity.FindComponent(RT_PS_CustomRadioComponent));
		
		if (!m_bInStaticProp) {m_RadioComponent = null;}
		
		if (!m_RadioComponent && !m_bInStaticProp) 
		{
			m_RadioComponent = RT_PS_CustomRadioAction.FindRadioComponent(pOwnerEntity, m_sSlotName);
		};
		
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
	protected bool Enabled()
	{
		if (!m_RadioComponent) 
			return false;
		
		return m_RadioComponent.Enabled();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{		
		if (!m_RadioComponent && !m_bInStaticProp) {
			m_RadioComponent = RT_PS_CustomRadioAction.FindRadioComponent(p_OwnerEntity, m_sSlotName);
		};
		
		if (!m_RadioComponent || !m_RadioSystem || !m_RadioSystem.m_bAllowRadios) {
			return m_eActionType == RT_PS_ECustomRadioAction.TURN_ON_OFF;
		}
		
		if (m_eActionType == RT_PS_ECustomRadioAction.RESET && !m_RadioSystem.m_bAllowFixRadios) return false;
		
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
		if (!m_RadioComponent)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Radio";
			return false;
		}
		
		if (!m_RadioSystem || !m_RadioSystem.m_bAllowRadios)
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
		
		if (m_eActionType == RT_PS_ECustomRadioAction.TURN_ON_OFF && Enabled())
		{
			return true;
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
		
		if (!m_RadioComponent || !m_RadioSystem || !m_RadioSystem.m_bAllowRadios)
		{
			return true;
		}
		
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
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioComponent FindRadioComponent(IEntity pOwnerEntity, string pSlotName = "CustomRadio")
	{		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pOwnerEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return null;
		
		EntitySlotInfo slot = FindRadioSlot(pOwnerEntity, pSlotName);		
		if (!slot) return null;
		
		IEntity attachedEntity = slot.GetAttachedEntity();
		if (!attachedEntity) return null;

		return RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	static EntitySlotInfo FindRadioSlot(IEntity pOwnerEntity, string pSlotName = "CustomRadio")
	{
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pOwnerEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return null;
		
		EntitySlotInfo slot = slotManager.GetSlotByName(pSlotName);
		
		if (slot && slot.IsEnabled()) return slot;
		
		foreach (string possibleName : RT_PS_CustomRadioAction.PossibleSlotName)
		{
			slot = slotManager.GetSlotByName(possibleName);
			
			if (slot && slot.IsEnabled()) return slot;
		}
		
		return null;
	}
}
