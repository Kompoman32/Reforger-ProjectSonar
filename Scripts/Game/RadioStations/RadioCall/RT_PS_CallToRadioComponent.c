class RT_PS_CallToRadioComponentClass: ScriptComponentClass
{
}

class RT_PS_CallToRadioComponent: ScriptComponent {
	RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{				
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}	
	}
		
	void OpenDialog()
	{
		Rpc(RPC_DoOpenDialog);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RPC_DoOpenDialog()
	{
		RT_PS_CallToRadioDialog menu = RT_PS_CallToRadioDialog.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.CallToTheRadio));
		
		if (menu) 
		{
			menu.m_OnConfirm.Insert(OnConfirm);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnConfirm(RT_PS_CallToRadioDialog dlg)
	{		
		Rpc(RPC_AskUpdateTrackOnRadios, dlg.GetSelectedStationIndex());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RPC_AskUpdateTrackOnRadios(int selectedRadioIndex) 
	{				
		PrintFormat("RPC_AskUpdateTrackOnRadios %1", selectedRadioIndex);
		
		m_RadioSystem.Debug_UpdateTrack(selectedRadioIndex);
		
		GetGame().GetCallqueue().CallLater(m_RadioSystem.Debug_UpdateTrack_2, 100, false, selectedRadioIndex );		
	}
}