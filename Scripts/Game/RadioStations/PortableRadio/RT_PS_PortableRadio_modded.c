
modded class SCR_ItemPlacementComponent : ScriptComponent
{
	void TogglePreviewPublic(IEntity weapon)
	{
		TogglePreview(weapon)
	}
	
	bool GetIsPreviewEnabled()
	{
		return m_PreviewEntity != null;
	}
			
	void OnPlacingEndedForPlaceableItem()
	{
		if (!SCR_PlaceableItemComponent.Cast(m_PlacedItem.FindComponent(SCR_PlaceableItemComponent))) return;
		
		ChimeraCharacter controlledEntity = ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		if (!controlledEntity)
			return;
		
		SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(controlledEntity.GetCharacterController());
		if (!characterController)
			return;
		
		RplComponent characterRplComp = controlledEntity.GetRplComponent();
		if (!characterRplComp)
			return;
		
		SCR_InventoryStorageManagerComponent storageManager = SCR_InventoryStorageManagerComponent.Cast(characterController.GetInventoryStorageManager());
		if (!storageManager)
			return;
		
		RplComponent rplComponent = RplComponent.Cast(m_PlacedItem.FindComponent(RplComponent));
		if (!rplComponent)
			return;
		
		SCR_PlacementCallback cb = new SCR_PlacementCallback();
		cb.m_PlaceableId = rplComponent.Id();
		cb.m_TargetId = m_TargetId;
		cb.m_iNodeId = m_iTargetEntityNodeID;
		cb.m_bIsBeingAttachedToEntity = m_bIsBeingAttachedToEntity;
		
		storageManager.TryRemoveItemFromInventory(m_PlacedItem, null, cb);		
		
		Rpc(RPC_AskSetPlacementPosition_PlaceableItem, m_vCurrentMat[0], m_vCurrentMat[1], m_vCurrentMat[2], m_vCurrentMat[3], rplComponent.Id(), characterRplComp.Id());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RPC_AskSetPlacementPosition_PlaceableItem(vector right, vector up, vector forward, vector position, RplId placeableId, RplId characterId)
	{
		GetGame().GetCallqueue().CallLater(RPC_AskSetPlacementPosition_PlaceableItem_Delayed, 300, false, right, up, forward, position, placeableId, characterId);
	}
	
	void RPC_AskSetPlacementPosition_PlaceableItem_Delayed(vector right, vector up, vector forward, vector position, RplId placeableId, RplId characterId)
	{
		RplComponent rplComponent = RplComponent.Cast(Replication.FindItem(placeableId));
		if (!rplComponent)
			return;

		IEntity item = rplComponent.GetEntity();
		if (!item) return;

		vector transform[4] = {right, up, forward, position};
		
		item.SetTransform(transform);
		item.Update();
		
		Rpc(RPC_AskSetPlacementPosition_PlaceableItem_Broadcast, right, up, forward, position, placeableId, characterId);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_AskSetPlacementPosition_PlaceableItem_Broadcast(vector right, vector up, vector forward, vector position, RplId placeableId, RplId characterId)
	{
		RplComponent rplComponent = RplComponent.Cast(Replication.FindItem(placeableId));
		if (!rplComponent)
			return;

		IEntity item = rplComponent.GetEntity();
		if (!item) return;

		vector transform[4] = {right, up, forward, position};
		
		item.SetTransform(transform);
		item.Update();
	}
}


modded class SCR_AvailableActionsConditionData
{
	//------------------------------------------------------------------------------------------------
	protected override void FetchCurrentlyPlacedItemName()
	{
		super.FetchCurrentlyPlacedItemName();
		
		RT_PS_PortableRadioGadgetComponent gadgetComponent = RT_PS_PortableRadioGadgetComponent.Cast(m_GadgetManager.GetHeldGadgetComponent());
		if (!gadgetComponent)
			return;

		IEntity item = gadgetComponent.GetOwner();
		if (!item)
			return;

		if (item != m_LastPlacedItem)
		{
			InventoryItemComponent iic = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
			if (!iic)
				return;

			UIInfo info = iic.GetUIInfo();
			if (!info)
				return;

			m_sCurrentlyPlacedItemName = info.GetName();
			m_LastPlacedItem = item;
		}
	}
}