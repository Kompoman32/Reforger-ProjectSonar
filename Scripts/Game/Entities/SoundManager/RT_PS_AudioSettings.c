modded class SCR_AudioSettings 
{
	[Attribute(defvalue: "100", uiwidget: UIWidgets.Slider, params: "0 300 5", desc: "Boost/Reduce radio volume.")]
	float m_fRadioVolumeBoost;
}

modded class SCR_AudioSettingsSubMenu 
{
	override void OnTabCreate(Widget menuRoot, ResourceName buttonsLayout, int index)
	{
		super.OnTabCreate(menuRoot, buttonsLayout, index);
		
		m_aSettingsBindings.Insert(new SCR_SettingBindingGameplay("SCR_AudioSettings", "m_fRadioVolumeBoost", "RadioVolumeBoost"));
		LoadSettings();
	}
}

modded class SCR_SoundManagerModule
{
	override void OnInit()
	{	
		super.OnInit();
		
		GetGame().OnUserSettingsChangedInvoker().Insert(SetRadioVolumeBoostSignalValue);
	}

	protected void SetRadioVolumeBoostSignalValue()
	{			
		BaseContainer settings = GetGame().GetGameUserSettings().GetModule("SCR_AudioSettings");    		
		if (!settings)
			return;
		
		GetGame().GetCallqueue().Remove(UpdateConnectedRadios);
		GetGame().GetCallqueue().CallLater(UpdateConnectedRadios, 300, false);
	}
	
	protected void UpdateConnectedRadios()
	{
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		
		if (!world) return;			
		
		RT_PS_CustomRadioAntennaSystem radioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		
		if (!radioSystem) return;

		radioSystem.ForceUpdateConnectedRadios();			
	}
}