[EntityEditorProps(category: "GameScripted/Components", description: "")]
class RT_PS_PortalbeRadioPlaceableItemComponentClass: SCR_PlaceableItemComponentClass {}

class RT_PS_PortalbeRadioPlaceableItemComponent: SCR_PlaceableItemComponent
{
	override bool OverrideStartPlaceAction(notnull SCR_ItemPlacementComponent caller, out bool skipItemUsage = false)
	{
		skipItemUsage = true;
		return true;
	}
	
	override bool OverrideOnPlacingEnded(notnull SCR_ItemPlacementComponent caller)
	{
		caller.OnPlacingEndedForPlaceableItem();
		
		ChimeraCharacter character = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
		if (!character)
			return false;
		
		SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
		if (!characterController)
			return false;
		
		characterController.SetDisableWeaponControls(false);
		characterController.SetDisableMovementControls(false);
		
		return false;
	}
}