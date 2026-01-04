modded class SCR_TurnOnAction : TurnOnUserAction
{
	override bool CanBeShownScript(IEntity user)
	{
		return false;
	}
}