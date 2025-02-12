class MyRadioVolumeAction extends SCR_AdjustSignalAction 
{
	MyRadioComponent m_radioComp;
	
	protected float m_volumeMultiplier = 5;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		super.Init(pOwnerEntity, pManagerComponent);
		
		m_radioComp = MyRadioComponent.Cast(pOwnerEntity.FindComponent(MyRadioComponent));
		
		if (m_radioComp) 
		{
			m_fTargetValue = m_radioComp.GetVolume() / m_volumeMultiplier;
		}
	}

	override protected bool OnLoadActionData(ScriptBitReader reader)
	{		
		if (!m_radioComp) return true;

		float oldTargetValue = m_fTargetValue;
		float newTargetValue;
		
		reader.ReadFloat(newTargetValue);
		
		GetGame().GetCallqueue().Remove(HandleLoadActionData);
		GetGame().GetCallqueue().CallLater(HandleLoadActionData, 500, false, newTargetValue);
		return true;
	}
	
	protected void HandleLoadActionData(float newTargetValue) 
	{
		float oldTargetValue = m_fTargetValue;
		
		if (oldTargetValue != newTargetValue) {
			m_fTargetValue = newTargetValue;
			SetSignalValue(newTargetValue);
			
			m_radioComp.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
		}
	}
	
	override bool CanBeShownScript(IEntity user)
	{
		if (!m_radioComp) return false;
		
		return m_radioComp.Enabled();
	}
	
	override void OnActionStart(IEntity pUserEntity)
	{
		super.OnActionStart(pUserEntity);
		
		m_fTargetValue = m_radioComp.GetVolume() / m_volumeMultiplier;
	}
	
	override void HandleAction(float value) 
	{
		if (!m_radioComp) return;
		
		super.HandleAction(value);
		
		m_radioComp.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
	}
	
	override void HandleActionDecrease(float value) 
	{
		if (!m_radioComp) return;

		super.HandleActionDecrease(value);
		

		m_radioComp.ActionSetVolume(m_fTargetValue * m_volumeMultiplier);
	}
	
	override bool GetActionNameScript(out string outName )
	{		
		outName = GetUIInfo().GetName()+": " + m_radioComp.GetVolume().ToString() + "%";
		return true;
	}
	
}