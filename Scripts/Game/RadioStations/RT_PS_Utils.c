class RT_PS_Utils {
	static string ArrayJoinStringInt(array<int> arr) {
		int count = arr.Count();
	
		string result = "";
		
		foreach (int i, int value: arr)
		{
			if (i < count - 1) 
			{
				result += string.Format("%1, ", value);
			} else {
				result += string.Format("%1", value);
			}
		}
		
		return result;
	}
	
	static Managed ArrayGet(array<Managed> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) return null;
		
		return arr.Get(index);
	}
	
	static string ArrayGet(array<string> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) {
			return string.Empty;
		} 
		
		return arr.Get(index);
	}
	
	static int ArrayGet(array<int> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) {
			return null;
		} 
		
		return arr.Get(index);
	}

	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioComponent FindRadioComponent(IEntity pEntity, string pSlotName = "CustomRadio")
	{		
		if (!pEntity) return null;
				
		RT_PS_CustomRadioComponent radioComp;
		
		radioComp = RT_PS_Utils.FindRadioComponentInEntity(pEntity);
		if (radioComp) return radioComp;
		
		radioComp = RT_PS_Utils.FindRadioComponentBySlotName(pEntity, pSlotName);
		if (radioComp) return radioComp;
		
		radioComp = RT_PS_Utils.FindRadioComponentInAnySlot(pEntity);
		if (radioComp) return radioComp;
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioComponent FindRadioComponentInEntity(IEntity pEntity)
	{
		return RT_PS_CustomRadioComponent.Cast(pEntity.FindComponent(RT_PS_CustomRadioComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	static void FindAllRadioComponentsInSlots(IEntity pEntity, notnull out array<RT_PS_CustomRadioComponent> radioComps)
	{		
		if (!pEntity) return;
		
		RT_PS_CustomRadioComponent radioComp;
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return;
		
		IEntity attachedEntity;
		
		array<EntitySlotInfo> slots = {};		
		slotManager.GetSlotInfos(slots);
		
		foreach (EntitySlotInfo slot : slots)
		{			
			attachedEntity = slot.GetAttachedEntity();
			
			if (!attachedEntity) continue;
			
			radioComp = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
			
			if (!radioComp) continue;
			
			radioComps.Insert(radioComp);
		}
		
		return;
	}
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioComponent FindRadioComponentBySlotName(IEntity pEntity, string pSlotName = "CustomRadio")
	{	
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return null;
		
		RT_PS_CustomRadioComponent radio;
		IEntity attachedEntity;
		
		EntitySlotInfo slot = slotManager.GetSlotByName(pSlotName);
		
		if (slot && slot.IsEnabled()) {
			attachedEntity = slot.GetAttachedEntity();
			
			if (attachedEntity) {
				radio = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
				
				if (radio) return radio;
			}
		};
				
		foreach (string possibleName : RT_PS_CustomRadioAction.PossibleSlotName)
		{
			slot = slotManager.GetSlotByName(possibleName);
			
			if (!slot || !slot.IsEnabled()) continue;			
			
			attachedEntity = slot.GetAttachedEntity();
			
			if (!attachedEntity) continue;
						
			radio = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
			
			if (radio) return radio;
		}

		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioComponent FindRadioComponentInAnySlot(IEntity pEntity)
	{	
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(pEntity.FindComponent(SlotManagerComponent));
		if (!slotManager) return null;
		
		RT_PS_CustomRadioComponent radio;
		IEntity attachedEntity;
		
		array<EntitySlotInfo> slots = {};		
		slotManager.GetSlotInfos(slots);
		
		foreach (EntitySlotInfo slot : slots)
		{
			if (!slot.IsEnabled()) continue;
			
			attachedEntity = slot.GetAttachedEntity();
			
			if (!attachedEntity) continue;
						
			radio = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
			
			if (radio) return radio;
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	static void GetRadioComponentFromAttribute(Managed item, out bool isValid = false, out RT_PS_CustomRadioAntennaSystem pSystem = null, out RT_PS_CustomRadioComponent pRadio = null, out int pCurrentRadioIndex = -1)
	{
		SCR_EditableEntityComponent comp = SCR_EditableEntityComponent.Cast(item);
		if (!comp) return;		
		
		IEntity entity = comp.GetOwner();
		
		if (!entity) return;
		
		RT_PS_CustomRadioComponent radio = RT_PS_Utils.FindRadioComponent(entity);
		if (!radio) return;
		
		RT_PS_CustomRadioAntennaSystem system = RT_PS_CustomRadioAntennaSystem.GetInstance();
		if (!system) return;
		
		isValid = true;
		pSystem = system;
		pRadio = radio;
		
		int index = system.m_aRadiostations.Find(radio.m_RadioStation);
		if (index < 0) return;
		
		pCurrentRadioIndex = index;
	}
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_CustomRadioAntennaChangeStationDebugAction GetDebugActionCurrentSelectedRadioStation(RT_PS_CustomRadioAntennaSystem pRadioSystem, IEntity pOwnerEntity)
	{		
		if (!pRadioSystem) return null;
		
		ActionsManagerComponent am = ActionsManagerComponent.Cast(pOwnerEntity.FindComponent(ActionsManagerComponent));
		
		if (!am) return null;
		
		array<BaseUserAction> actions = {};				
		am.GetActionsList(actions);
		
		foreach(BaseUserAction action: actions)
		{
			RT_PS_CustomRadioAntennaChangeStationDebugAction csAction = RT_PS_CustomRadioAntennaChangeStationDebugAction.Cast(action);
			if (csAction) 
			{
				return csAction;
			}
		}
		
		return null;
	}
}