class RT_PS_CustomRadioInteriorRequestCallback : InteriorRequestCallback
{
	protected RT_PS_CustomRadioComponent m_radio;
	
	//------------------------------------------------------------------------------------------------	
	override void OnCompleted(float interior, float roomSize)
	{
		m_radio.m_signalInterionValue = interior;
		m_radio.m_signalRoomSizeValue = roomSize;
	}
	
	//------------------------------------------------------------------------------------------------	
	void RT_PS_CustomRadioInteriorRequestCallback(RT_PS_CustomRadioComponent radio)
	{
		m_radio = radio;
	}
}