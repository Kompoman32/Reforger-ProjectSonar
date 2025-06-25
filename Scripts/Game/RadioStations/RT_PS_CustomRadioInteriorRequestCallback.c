class RT_PS_CustomRadioInteriorRequestCallback : InteriorRequestCallback
{
	protected RT_PS_CustomRadioComponent m_Radio;
	
	//------------------------------------------------------------------------------------------------	
	override void OnCompleted(float interior, float roomSize)
	{
		m_Radio.m_fSignalInterionValue = interior;
		m_Radio.m_fSignalRoomSizeValue = roomSize;
	}
	
	//------------------------------------------------------------------------------------------------	
	void RT_PS_CustomRadioInteriorRequestCallback(RT_PS_CustomRadioComponent pRadio)
	{
		m_Radio = pRadio;
	}
}