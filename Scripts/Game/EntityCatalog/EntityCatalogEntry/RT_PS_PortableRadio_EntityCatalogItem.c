[BaseContainerProps(), SCR_BaseContainerCustomInventoryCatalogEntry("m_sEntityPrefab", "m_aEntityDataList", "m_bEnabled")]
class RT_PS_PortableRadio_EntityCatalogItem: SCR_EntityCatalogInventoryItem  {
	[Attribute("0", desc: "Do this EntityCatalogItem in the CIV catalog?")]
	bool m_bIsCIV;
	
	RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	override void PostInitEntry(SCR_EntityCatalog catalog)
	{
		super.PostInitEntry(catalog);
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) {
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem))
		}		
	}
	
	override SCR_BaseEntityCatalogData GetEntityDataOfType(typename dataType)
	{			
		if (m_RadioSystem)
		{
			if (!m_RadioSystem.m_bPortableRadioArsenalAvailable) return null;
			
			if (m_RadioSystem.m_bPortableRadioOnlyCIVArsenal && !m_bIsCIV) return null;
		}
		
		return super.GetEntityDataOfType(dataType);
	}
}