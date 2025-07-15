enum RT_PS_ECustomRadioAntennaDebugAction 
{
	UPDATE_RADIOS,
	ONOFF_RADIOS,
	ALLOW_FIX_RADIOS,
	UPDATE_TRACK,
}

class RT_PS_CustomRadioAntennaDebugAction: ScriptedUserAction
{
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(RT_PS_ECustomRadioAntennaDebugAction))]
	RT_PS_ECustomRadioAntennaDebugAction m_eActionType;

	protected IEntity m_Owner;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		m_Owner = pOwnerEntity;
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{		
		if (!m_RadioSystem) return false;
		
		outName = GetUIInfo().GetName();
		
							
		switch (m_eActionType)
		{
			case RT_PS_ECustomRadioAntennaDebugAction.UPDATE_TRACK:
			{				
				int selectedRadioIndex = GetCurrentSelectedRadioStationIndex();
				
				if (selectedRadioIndex != 0 && !selectedRadioIndex) return false;
				
				int trackIndex = m_RadioSystem.GetRadioStationTrackIndex(selectedRadioIndex);
				
				if (trackIndex != 0 && !trackIndex) return false;
				
				RT_PS_CustomRadioStationTrackInfo track = m_RadioSystem.GetRadioStationTrack(selectedRadioIndex);
				
				if (!track) return false;
				
				bool isDj = track.m_bIsDJ;
				float timeLeft = m_RadioSystem.GetRadioStationTrackTimeLeft(selectedRadioIndex);

				string trackName = m_RadioSystem.GetRadioStationTrackName(selectedRadioIndex);	
				
				if (trackName)
				{
					trackName = string.Format("Track %1", trackName);
				} 
				else 
				{
					trackName = string.Format("Track № %1", trackIndex);
				}
				
				if (isDj)
				{
					trackName = string.Format("DJ %1", trackName);
				}

				outName += string.Format(" %1 - %2s", trackName, timeLeft);
				
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.ONOFF_RADIOS:
			{		
				if (m_RadioSystem.m_bAllowRadios)
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";
				
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.ALLOW_FIX_RADIOS:
			{		
				if (m_RadioSystem.m_bAllowFixRadios)
					outName += ": #AR-UserAction_State_Off";
				else
					outName += ": #AR-UserAction_State_On";
				
				break;
			}
		}		
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) {
		if (Replication.IsClient()) return;
		
					
		switch (m_eActionType)
		{
			case RT_PS_ECustomRadioAntennaDebugAction.UPDATE_RADIOS:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.Debug_UpdateRadios();
				}
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.UPDATE_TRACK:
			{
				UpdateTrackOnRadios();
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.ONOFF_RADIOS:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.Debug_On_Off_radios();
				}
				break;
			}
			
			case RT_PS_ECustomRadioAntennaDebugAction.ALLOW_FIX_RADIOS:
			{
				if (m_RadioSystem) 
				{
					m_RadioSystem.Debug_AllowFixRadios();
				}
				break;
			}
		}
	}
	
	private void OnConfirm(SCR_ConfigurableDialogUi dlg)
	{
		dlg.Close();
	}
	
	private void OnCancel(SCR_ConfigurableDialogUi dlg)
	{
		dlg.Close();
	}
	
	protected int GetCurrentSelectedRadioStationIndex()
	{
		if (!m_RadioSystem) return null;
		
		ActionsManagerComponent am = ActionsManagerComponent.Cast(m_Owner.FindComponent(ActionsManagerComponent));
		
		if (!am) return null;
		
		array<BaseUserAction> actions = {};				
		am.GetActionsList(actions);
		
		foreach(BaseUserAction action: actions)
		{
			RT_PS_CustomRadioAntennaChangeStationDebugAction csAction = RT_PS_CustomRadioAntennaChangeStationDebugAction.Cast(action);
			if (csAction) 
			{
				return csAction.m_iSelectedRadioIndex;
			}
		}
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateTrackOnRadios() 
	{
		int selectedRadioIndex = GetCurrentSelectedRadioStationIndex();
		
		if (selectedRadioIndex != 0 && !selectedRadioIndex) return;
				
		m_RadioSystem.Debug_UpdateTrack(selectedRadioIndex);
		
		GetGame().GetCallqueue().CallLater(m_RadioSystem.Debug_UpdateTrack_2, 100, false, selectedRadioIndex );		
	}
}
