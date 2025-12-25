//------------------------------------------------------------------------------------------------
//! Main game instance.
//! Created when the game starts (not when a world starts!) and persists until the game is closed.
modded class ArmaReforgerScripted : ChimeraGame
{
	static protected ref RT_PS_SettingsConfig s_RT_PS_SettingsConfig;
	static protected bool s_bRT_PS_WasMissionHeaderApplied = false;
	
	//------------------------------------------------------------------------------------------------
	override void OnAfterInit(BaseWorld world)
	{
		super.OnAfterInit(world);
		
		if (Replication.IsServer() && !s_RT_PS_SettingsConfig)
		{
			s_RT_PS_SettingsConfig = SCR_ConfigHelperT<RT_PS_SettingsConfig>.GetConfigObject("{6C4790D1DF17E74B}Configs/PS/Settings.conf");
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnMissionSet(MissionHeader mission)
	{
		super.OnMissionSet(mission);
		
		if (!Replication.IsServer() || s_bRT_PS_WasMissionHeaderApplied)
			return;
		
		SCR_MissionHeader scriptedMission = SCR_MissionHeader.Cast(mission);
		if (!scriptedMission)
			return;
		
		RT_PS_SetSettingFromHeader(scriptedMission);		
		s_bRT_PS_WasMissionHeaderApplied = true;
	}
	
	//------------------------------------------------------------------------------------------------
	void RT_PS_SetSettingFromHeader(SCR_MissionHeader pMissionHeader)
	{
		RT_PS_SettingsConfig config = RT_PS_GetSettingsConfig();
		if (config && pMissionHeader)
			config.SetFromHeader(pMissionHeader.m_ProjectSonar_Settings);
	}
	
	//------------------------------------------------------------------------------------------------
	static RT_PS_SettingsConfig RT_PS_GetSettingsConfig()
	{
		return s_RT_PS_SettingsConfig;
	}
}
