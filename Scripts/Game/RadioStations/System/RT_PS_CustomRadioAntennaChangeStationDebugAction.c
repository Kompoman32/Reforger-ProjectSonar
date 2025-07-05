class RT_PS_CustomRadioAntennaChangeStationDebugAction extends SCR_AdjustSignalAction 
{
	int m_iSelectedRadioIndex = 0;

	protected IEntity p_OwnerEntity;
	protected RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
	
		p_OwnerEntity = pOwnerEntity;
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
	}

	//------------------------------------------------------------------------------------------------
	override protected bool OnLoadActionData(ScriptBitReader reader)
	{		
		super.OnLoadActionData(reader);
		
		m_iSelectedRadioIndex = m_fTargetValue;
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (SCR_ConfigurableDialogUi.GetCurrentDialog()) return false;
		
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
		
		if (m_RadioSystem.GetRadiostaionsCount() == 1)
		{
			m_sCannotPerformReason = "#RT_PS-CannotPerform_OneStation";
			return false;
		}
		
		return true;	
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		if (!m_RadioSystem) return;
		
		m_fTargetValue = m_iSelectedRadioIndex;
	}
	
	//------------------------------------------------------------------------------------------------
	override void HandleAction(float value) 
	{
		if (!m_RadioSystem) return;
		
		super.HandleAction(value);
		
		m_iSelectedRadioIndex = m_fTargetValue;
		
		if (m_iSelectedRadioIndex >= m_RadioSystem.GetRadiostaionsCount()) 
		{
			m_iSelectedRadioIndex = 0;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{		
		if (!m_RadioSystem) return false;
		
		RT_PS_CustomRadioStation station = m_RadioSystem.GetRadioStation(m_iSelectedRadioIndex);
		
		if (!station) return false;
		
		outName = GetUIInfo().GetName() + ": " + station.m_sRadiostationName;
		
		return true;
	}
	
}