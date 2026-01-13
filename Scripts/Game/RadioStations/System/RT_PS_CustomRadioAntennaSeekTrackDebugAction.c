class RT_PS_CustomRadioAntennaSeekTrackDebugAction extends SCR_AdjustSignalAction 
{
	float m_fSelectedSeekValue = 0;

	protected IEntity m_Owner;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	protected RT_PS_CustomRadioAntennaChangeStationDebugAction m_CurrentSelectedRadioStationAction;
	
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
	override protected bool OnLoadActionData(ScriptBitReader reader)
	{		
		bool result = super.OnLoadActionData(reader);
		
		if(Replication.IsServer())
		{
			m_fSelectedSeekValue = SCR_GetCurrentValue();
		}
		
		return result;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{		
		if (!m_RadioSystem) return false;
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_RadioSystem) 
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Antenna";
			return false;
		}	
		
		if (m_RadioSystem.GetRadiostaionsCount() == 0)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_Station";
			return false;
		}
		
		return true;	
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		if(Replication.IsServer())
		{
			m_fSelectedSeekValue = 0;
		}
		
		SetSignalValue(m_fSelectedSeekValue);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionCanceled(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		super.OnActionCanceled(pOwnerEntity, pUserEntity);
		
		m_fTargetValue = 0;
		
		if (!m_RadioSystem) return;
		if (!m_fSelectedSeekValue) return;
		
		m_RadioSystem.Debug_SeekCurrentTrack(GetCurrentSelectedRadioStationIndex(), m_fSelectedSeekValue);
	}
	
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{		
		if (!m_RadioSystem) return false;
		
		outName = string.Format("%1: %2s", GetUIInfo().GetName(), m_fTargetValue);
		
		return true;
	}
	
	protected int GetCurrentSelectedRadioStationIndex()
	{
		if (!m_CurrentSelectedRadioStationAction)
		{
			m_CurrentSelectedRadioStationAction = RT_PS_Utils.GetDebugActionCurrentSelectedRadioStation(m_RadioSystem, m_Owner);
		}
		
		if (!m_RadioSystem || !m_CurrentSelectedRadioStationAction) return null;
		
		return m_CurrentSelectedRadioStationAction.m_iSelectedRadioIndex;
	}
	
}