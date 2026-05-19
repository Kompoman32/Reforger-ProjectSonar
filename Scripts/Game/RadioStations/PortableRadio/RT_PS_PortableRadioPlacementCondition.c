[BaseContainerProps()]
class RT_PS_PortableRadioPlacementCondition : SCR_AvailableActionCondition
{
	[Attribute(desc: "Check if placement mode is enabled")]
	protected bool m_bCheckIsPlacementModeEnabled;

	//------------------------------------------------------------------------------------------------
	override bool IsAvailable(SCR_AvailableActionsConditionData data)
	{
		if (!data)
			return false;

		RT_PS_PortableRadioGadgetComponent gadgetComponent = RT_PS_PortableRadioGadgetComponent.Cast(data.GetHeldGadgetComponent());
		if (!gadgetComponent)
			return GetReturnResult(false);
		
		if (m_bCheckIsPlacementModeEnabled && !gadgetComponent.GetIsPlacementModeEnabled())
			return GetReturnResult(false);

		return GetReturnResult(true);
	}
}