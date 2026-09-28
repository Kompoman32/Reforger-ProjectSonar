
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

modded class SCR_ChimeraCharacter: ChimeraCharacter
{
	protected int m_iCheckGeneratedRadioCounter;
	protected const int CheckGeneratedRadioMaxCounter = 20;
	protected const int MaxDistanceToAbortPickupRadio = 25; // 5m
	
	void RT_PS_PickupRadioAfterTransform_S(IEntity radio)
	{		
		RplComponent rpl = RplComponent.Cast(radio.FindComponent(RplComponent));
		
		Rpc(RT_PS_PickupRadioAfterTransform_O, rpl.Id())
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RT_PS_PickupRadioAfterTransform_O(RplId radioRplId)
	{
		GetGame().GetCallqueue().Remove(RT_PS_PickupRadioAfterTransform_O_Delayed);
		
		SCR_InventoryStorageManagerComponent pInventoryManager = SCR_InventoryStorageManagerComponent.Cast(FindComponent(SCR_InventoryStorageManagerComponent));
		if (!pInventoryManager) return;
		
		pInventoryManager.SetReturnCodeDefault();
		
		m_iCheckGeneratedRadioCounter = 0;
		
		GetGame().GetCallqueue().CallLater(RT_PS_PickupRadioAfterTransform_O_Delayed, 100, true, pInventoryManager, radioRplId);
	}
	
	protected void RT_PS_PickupRadioAfterTransform_O_Delayed(SCR_InventoryStorageManagerComponent pInventoryManager, RplId radioRplId)
	{
		if (m_iCheckGeneratedRadioCounter > CheckGeneratedRadioMaxCounter) {
			GetGame().GetCallqueue().Remove(RT_PS_PickupRadioAfterTransform_O_Delayed);
		}
		
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(radioRplId));
		if (!rpl || rpl == RplId.Invalid()) return;

		if (vector.DistanceSq(rpl.GetEntity().GetOrigin(), GetOrigin()) > MaxDistanceToAbortPickupRadio
			|| !pInventoryManager.CanInsertItem(rpl.GetEntity(), EStoragePurpose.PURPOSE_DEPOSIT)
		) {
			GetGame().GetCallqueue().Remove(RT_PS_PickupRadioAfterTransform_O_Delayed);
			return;
		}

		if (!pInventoryManager.IsAnimationReady())
			return;

		pInventoryManager.InsertItem(rpl.GetEntity());
		
		GetGame().GetCallqueue().Remove(RT_PS_PickupRadioAfterTransform_O_Delayed);
	}
}