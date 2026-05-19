[ComponentEditorProps(category: "GameScripted/Gadgets", description: "")]
class RT_PS_PortableRadioGadgetComponentClass : SCR_GadgetComponentClass
{
}

class RT_PS_PortableRadioGadgetComponent : SCR_GadgetComponent
{
	protected RT_PS_PortalbeRadioPlaceableItemComponent m_PlaceableItem;
	
	protected bool m_bPlaceableModeEnabled;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{		
		m_PlaceableItem = RT_PS_PortalbeRadioPlaceableItemComponent.Cast(owner.FindComponent(RT_PS_PortalbeRadioPlaceableItemComponent));
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
		if (!GetOwner()) return;
		
		if (mode == EGadgetMode.ON_GROUND || mode == EGadgetMode.IN_HAND)
		{
			RT_PS_CustomRadioComponent radio = RT_PS_CustomRadioComponent.Cast(GetOwner().FindComponent(RT_PS_CustomRadioComponent));	
			
			if (!radio || !radio.m_RadioSystem || radio.IsActive()) return;	
			
			radio.Activate(GetOwner());
		}
		
		if (mode == EGadgetMode.IN_STORAGE)
		{
			RT_PS_CustomRadioComponent radio = RT_PS_CustomRadioComponent.Cast(GetOwner().FindComponent(RT_PS_CustomRadioComponent));	
			
			if (!radio || !radio.m_RadioSystem) return;	
			
			if (radio.m_RadioSystem.m_bPortableRadioPlaysFromInventory)
			{
				if (!radio.IsActive()) {
					radio.Activate(GetOwner());
				}	
			}
			else 
			{
				if (radio.IsActive()) {
					radio.Deactivate(GetOwner());
				}	
			}
			
		}
		
		if (mode == EGadgetMode.IN_HAND)
		{
			//SetPlacementMode();
			GetGame().GetInputManager().AddActionListener("GadgetActivate", EActionTrigger.DOWN, TogglePlacementMode);
		}
		else {
			GetGame().GetInputManager().RemoveActionListener("GadgetActivate", EActionTrigger.DOWN, TogglePlacementMode);
			DisablePlacementMode();
		}
	}
	
	bool GetIsPlacementModeEnabled()
	{
		return m_bPlaceableModeEnabled;
	}
	
	void TogglePlacementMode()
	{
		if (!m_PlaceableItem) return;
		
		PlayerController controller = GetGame().GetPlayerController();
		if (!controller) return;
		
		SCR_ItemPlacementComponent placementComp = SCR_ItemPlacementComponent.Cast(controller.FindComponent(SCR_ItemPlacementComponent));
		if (!placementComp) return;
		
		if (placementComp.GetIsPreviewEnabled()) {
			DisablePlacementMode();
		} else {
			EnablePlacementMode();
		}

	}
	
	void EnablePlacementMode()
	{
		if (!m_PlaceableItem) return;
		
		PlayerController controller = GetGame().GetPlayerController();
		if (!controller) return;
		
		SCR_ItemPlacementComponent placementComp = SCR_ItemPlacementComponent.Cast(controller.FindComponent(SCR_ItemPlacementComponent));
		if (!placementComp) return;
		
		m_bPlaceableModeEnabled = true;
		
		placementComp.SetPlacedItem(m_PlaceableItem.GetOwner());
		placementComp.TogglePreviewPublic(m_PlaceableItem.GetOwner());
	}
	
	void DisablePlacementMode()
	{
		m_bPlaceableModeEnabled = false;
		
		if (!m_PlaceableItem) return;
		
		PlayerController controller = GetGame().GetPlayerController();
		if (!controller) return;
		
		SCR_ItemPlacementComponent placementComp = SCR_ItemPlacementComponent.Cast(controller.FindComponent(SCR_ItemPlacementComponent));
		if (!placementComp) return;
		
		placementComp.SetPlacedItem(null);
		placementComp.DisablePreview();
	}
}