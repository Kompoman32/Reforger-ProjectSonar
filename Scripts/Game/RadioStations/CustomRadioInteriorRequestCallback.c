class CustomRadioInteriorRequestCallback : InteriorRequestCallback
{
	protected CustomRadioComponent m_radio;
	
	//------------------------------------------------------------------------------------------------	
	override void OnCompleted(float interior, float roomSize)
	{
		m_radio.m_signalInterionValue = interior;
		m_radio.m_signalRoomSizeValue = roomSize;
	}
	
	//------------------------------------------------------------------------------------------------	
	void CustomRadioInteriorRequestCallback(CustomRadioComponent radio)
	{
		m_radio = radio;
	}
}