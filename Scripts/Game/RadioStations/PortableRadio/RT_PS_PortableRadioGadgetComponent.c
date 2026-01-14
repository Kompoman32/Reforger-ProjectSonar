[ComponentEditorProps(category: "GameScripted/Gadgets", description: "")]
class RT_PS_PortableRadioGadgetComponentClass : SCR_GadgetComponentClass
{
}

class RT_PS_PortableRadioGadgetComponent : SCR_GadgetComponent
{
	protected IEntity m_Owner;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{		
		m_Owner = owner;
		super.OnPostInit(owner);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void ModeSwitch(EGadgetMode mode, IEntity charOwner)
	{
		super.ModeSwitch(mode, charOwner);
		
		GetGame().GetCallqueue().Remove(ModeSwitchDelayed);
		GetGame().GetCallqueue().CallLater(ModeSwitchDelayed, 300, false, mode, charOwner);
	}

	//------------------------------------------------------------------------------------------------
	protected void ModeSwitchDelayed(EGadgetMode mode, IEntity charOwner)
	{
		if (!m_Owner) return;
		
		if (mode == EGadgetMode.ON_GROUND || mode == EGadgetMode.IN_HAND)
		{
			RT_PS_CustomRadioComponent radio = RT_PS_CustomRadioComponent.Cast(m_Owner.FindComponent(RT_PS_CustomRadioComponent));	
			
			if (!radio || radio.IsActive()) return;	
			
			radio.Activate(m_Owner);
			
			return;
		}
		
		if (mode == EGadgetMode.IN_STORAGE)
		{
			RT_PS_CustomRadioComponent radio = RT_PS_CustomRadioComponent.Cast(m_Owner.FindComponent(RT_PS_CustomRadioComponent));	
			
			if (!radio || !radio.IsActive()) return;	
			
			radio.Deactivate(m_Owner);
		}
	}
}