class RT_PS_CallToRadioAction: ScriptedUserAction
{
	protected IEntity m_Owner;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		m_Owner = pOwnerEntity;
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
		return false;
	}	
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{	
		if (!m_RadioSystem) return false;
		
		return !m_RadioSystem.m_bSettingsHideTelephoneAction;
	}
	

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {
		if (!m_RadioSystem) return;
		
		RplIdentity pIdentity = Replication.FindOwner(RplComponent.Cast(pUserEntity.FindComponent(RplComponent)).Id());
		
		if (Replication.IsServer())
		{
			RplComponent rplComp= RplComponent.Cast(m_Owner.FindComponent(RplComponent));
			rplComp.Give(pIdentity);
			
			RT_PS_CallToRadioComponent comp = RT_PS_CallToRadioComponent.Cast(m_Owner.FindComponent(RT_PS_CallToRadioComponent));
			comp.OpenDialog();
		}
	}
}
