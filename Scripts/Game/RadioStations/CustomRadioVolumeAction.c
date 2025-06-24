class CustomRadioVolumeAction extends SCR_AdjustSignalAction 
{
	IEntity p_OwnerEntity;
	CustomRadioComponent m_RadioComponent;
	
	protected float m_volumeMultiplier = 5;
	
	[Attribute("1", UIWidgets.CheckBox)]
	bool m_inVehicle;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
	
		p_OwnerEntity = pOwnerEntity;	
		m_RadioComponent = CustomRadioComponent.Cast(pOwnerEntity.FindComponent(CustomRadioComponent));
		
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(pOwnerEntity);
		
		if (m_RadioComponent) 
		{
			m_fTargetValue = m_RadioComponent.GetVolume() / m_volumeMultiplier;
		}
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
	

	override protected bool OnLoadActionData(ScriptBitReader reader)
	{		
		if (!m_RadioComponent) return true;

		float oldTargetValue = m_fTargetValue;
		float newTargetValue;
		
		reader.ReadFloat(newTargetValue);
		
		GetGame().GetCallqueue().Remove(HandleLoadActionData);
		GetGame().GetCallqueue().CallLater(HandleLoadActionData, 500, false, newTargetValue);
		return true;
	}
	
	protected void HandleLoadActionData(float newTargetValue) 
	{
		float oldTargetValue = m_fTargetValue;
		
		if (oldTargetValue != newTargetValue) {
			m_fTargetValue = newTargetValue;
			SetSignalValue(newTargetValue);
			
			m_RadioComponent.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
		}
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		if (!m_RadioComponent && m_inVehicle) FindRadioComponent(p_OwnerEntity);
		if (!m_RadioComponent) return false;
		
		return m_RadioComponent.Enabled();
	}
	
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		if (m_RadioComponent)
		{
			m_fTargetValue = m_RadioComponent.GetVolume() / m_volumeMultiplier;
		}
	}
	
	override void HandleAction(float value) 
	{
		if (!m_RadioComponent) return;
		
		super.HandleAction(value);
		
		m_RadioComponent.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
	}
	
	override void HandleActionDecrease(float value) 
	{
		if (!m_RadioComponent) return;

		super.HandleActionDecrease(value);
		

		m_RadioComponent.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
	}
	
	override bool GetActionNameScript(out string outName )
	{		
		if (!m_RadioComponent) return false;
		
		outName = GetUIInfo().GetName()+": " + m_RadioComponent.GetVolume().ToString() + "%";
		return true;
	}
	
}