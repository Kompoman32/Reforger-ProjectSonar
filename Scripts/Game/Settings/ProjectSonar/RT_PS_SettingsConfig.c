//------------------------------------------------------------------------------------------------
//! Base container for all settings
[BaseContainerProps(configRoot: true)]
class RT_PS_SettingsConfig
{
	
	[Attribute(desc: "Stations that should not be available to use (Station names)")]
	ref array<string> m_aBlacklist;
	
	[Attribute(desc: "Stations that should be available to use (Station names). If filled in, it is used as a list of used stations")]
	ref array<string> m_aWhitelist;
	
	[Attribute("0", desc: "Hides action to call to the radio on telephones")]
	bool m_bHideTelephoneAction;
	
	[Attribute("0", desc: "Randomization method applied to the track list at stations", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(E_RT_PS_RadiostationRandomizationMethod))]
	E_RT_PS_RadiostationRandomizationMethod m_RandomizationMethod;	
	
	void SetFromHeader(notnull RT_PS_MissionHeaderSettings settings)
	{		
		if (settings.m_aBlacklist)
		{
			m_aBlacklist = settings.m_aBlacklist;
		}
		
		if (settings.m_aWhitelist)
		{
			m_aWhitelist = settings.m_aWhitelist;
		}
		
		m_bHideTelephoneAction = settings.m_bHideTelephoneAction;
		m_RandomizationMethod = settings.m_RandomizationMethod;
	}
}