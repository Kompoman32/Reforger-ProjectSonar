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

modded class SCR_SoundManagerEntity
{
	override void EOnInit(IEntity owner)
	{	
		super.EOnInit(owner);
		
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
	
	private void UpdateConnectedRadios()
	{
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		MyRadioAntennaSystem radioSystem;
		
		if (world) 
		{
			radioSystem = MyRadioAntennaSystem.Cast(world.FindSystem(MyRadioAntennaSystem));
			radioSystem.ForceUpdateConnectedRadios();
		}			
	}
}