//------------------------------------------------------------------------------------------------
//! Base container for all settings
[BaseContainerProps(configRoot: true)]
class RT_PS_SettingsConfig
{
	
	[Attribute(desc: "Stations that should not be available to use (Station names)")]
	ref array<string> m_aBlacklist;
	
	[Attribute(desc: "Stations that should be available to use (Station names). If filled in, it is used as a list of used stations")]
	ref array<string> m_aWhitelist;
	
	[Attribute("0", desc: "Disables radio use for passengers if the pilot's seat is occupied")]
	bool m_bPilotOnlyRadio;
	
	[Attribute("0", desc: "Hides action to call to the radio on telephones")]
	bool m_bHideTelephoneAction;
	
	[Attribute("0", desc: "Randomization method applied to the track list at stations", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(E_RT_PS_RadiostationRandomizationMethod))]
	E_RT_PS_RadiostationRandomizationMethod m_RandomizationMethod = E_RT_PS_RadiostationRandomizationMethod.SHUFFLE_ONCE;
	
	[Attribute("0", desc: "The minimum number of MUSIC tracks that need to be played to play another DJ track")]
	int m_iDJCooldown = 0;
	
	[Attribute("0", desc: "Allows Portable Radio to play inside inventory")]
	bool m_bPortableRadioPlaysFromInventory;
	
	[Attribute("1", desc: "Allows Portable Radio in arsenals")]
	bool m_bPortableRadioArsenalAvailable = true;
	
	[Attribute("0", desc: "Allows Portable Radio only in arsenals")]
	bool m_bPortableRadioOnlyCIVArsenal;
	
	[Attribute("1", desc: "Enables conversion of static radios to portable versions (Pickup Action)")]
	bool m_bStaticRadioConvertion = true;
	
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
		
		m_bPilotOnlyRadio = settings.m_bPilotOnlyRadio;
		m_bHideTelephoneAction = settings.m_bHideTelephoneAction;
		m_RandomizationMethod = settings.m_RandomizationMethod;
		m_iDJCooldown = settings.m_iDJCooldown;
		m_bPortableRadioPlaysFromInventory = settings.m_bPortableRadioPlaysFromInventory;
		m_bPortableRadioArsenalAvailable = settings.m_bPortableRadioArsenalAvailable;
		m_bPortableRadioOnlyCIVArsenal = settings.m_bPortableRadioOnlyCIVArsenal;
		m_bStaticRadioConvertion = settings.m_bStaticRadioConvertion;
	}
}