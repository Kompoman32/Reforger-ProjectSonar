class RT_PS_TransformToPortableAction: ScriptedUserAction
{	
	[Attribute("", UIWidgets.EditBox)]
	ResourceName m_sPortableRadioPrefab;

	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;

	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{		
		if (!m_sPortableRadioPrefab) return false;
		
		RT_PS_SettingsConfig settings = ArmaReforgerScripted.RT_PS_GetSettingsConfig();
		if (!settings.m_bStaticRadioConvertion) {
			return false;
		}
		
		return true;
	}
	
	
	override bool CheckOnServerFirstScript() {
		return true;
	}
	
	protected IEntity GenerateEntity() {		
		Resource loaded = Resource.Load(m_sPortableRadioPrefab);
		if (!loaded) return null;

		// Creating New Entity
		EntitySpawnParams params = new EntitySpawnParams;
		params.TransformMode = ETransformMode.WORLD;
		GetOwner().GetTransform(params.Transform);
		
		IEntity itemEntity = GetGame().SpawnEntityPrefabEx(m_sPortableRadioPrefab, false, GetGame().GetWorld(), params);
		
		RplComponent rpl = RplComponent.Cast(itemEntity.FindComponent(RplComponent));
		
		if (!rpl) return null;
		if (!rpl.Id() || rpl.Id() == RplId.Invalid()) return null;
		
		return itemEntity; 
	};
	

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		SCR_InventoryStorageManagerComponent pInventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		
		if (!pInventoryManager) return;
		
		IEntity newRadio = GenerateEntity();		
		
		if (!newRadio) return;
		
		if (pInventoryManager.CanInsertItem(newRadio, EStoragePurpose.PURPOSE_DEPOSIT))
		{
			SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(pUserEntity);
			char.RT_PS_PickupRadioAfterTransform_S(newRadio);			
			
			SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
		} else {
			SCR_EntityHelper.DeleteEntityAndChildren(newRadio);
		}
	}
}
