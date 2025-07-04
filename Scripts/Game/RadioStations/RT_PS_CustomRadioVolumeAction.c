class RT_PS_CustomRadioVolumeAction extends SCR_AdjustSignalAction 
{
	[Attribute("0", UIWidgets.CheckBox)]
	bool m_bInStaticProp;

	IEntity p_OwnerEntity;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	protected RT_PS_CustomRadioComponent m_RadioComponent;
	
	protected float m_fVolumeMultiplier = 5;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
	
		p_OwnerEntity = pOwnerEntity;	
		m_RadioComponent = RT_PS_CustomRadioComponent.Cast(pOwnerEntity.FindComponent(RT_PS_CustomRadioComponent));
		
		if (!m_RadioComponent && !m_bInStaticProp) FindRadioComponent(pOwnerEntity);
		
		if (m_RadioComponent) 
		{
			m_fTargetValue = m_RadioComponent.GetVolume() / m_fVolumeMultiplier;
		}
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
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

		m_RadioComponent = RT_PS_CustomRadioComponent.Cast(attachedEntity.FindComponent(RT_PS_CustomRadioComponent));
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
	
	protected void HandleLoadActionData(float pNewTargetValue) 
	{
		float oldTargetValue = m_fTargetValue;
		
		if (oldTargetValue != pNewTargetValue) {
			m_fTargetValue = pNewTargetValue;
			SetSignalValue(pNewTargetValue);
			
			m_RadioComponent.ActionSetVolume(m_fTargetValue * m_fVolumeMultiplier);
		}
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		if (SCR_ConfigurableDialogUi.GetCurrentDialog()) return false;
		if (!m_RadioComponent && !m_bInStaticProp) FindRadioComponent(p_OwnerEntity);
		if (!m_RadioComponent || !m_RadioSystem || !m_RadioSystem.m_bAllowRadios) return false;
		
		return m_RadioComponent.Enabled();
	}
	
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		if (m_RadioComponent)
		{
			m_fTargetValue = m_RadioComponent.GetVolume() / m_fVolumeMultiplier;
		}
	}
	
	override void HandleAction(float value) 
	{
		if (!m_RadioComponent) return;
		
		super.HandleAction(value);
		
		m_RadioComponent.ActionSetVolume(m_fTargetValue * m_fVolumeMultiplier);
	}
	
	override void HandleActionDecrease(float value) 
	{
		if (!m_RadioComponent) return;

		super.HandleActionDecrease(value);
		

		m_RadioComponent.ActionSetVolume(m_fTargetValue * m_fVolumeMultiplier);
	}
	
	override bool GetActionNameScript(out string outName)
	{		
		if (!m_RadioComponent) return false;
		
		outName = GetUIInfo().GetName()+": " + m_RadioComponent.GetVolume().ToString() + "%";
		return true;
	}
	
}