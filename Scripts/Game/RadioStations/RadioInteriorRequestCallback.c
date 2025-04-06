class RT_RadioInteriorRequestCallback : InteriorRequestCallback
{
	protected MyRadioComponent m_radio;
	
	//------------------------------------------------------------------------------------------------	
	override void OnCompleted(float interior, float roomSize)
	{
		m_radio.m_signalInterionValue = interior;
		m_radio.m_signalRoomSizeValue = roomSize;
	}
	
	//------------------------------------------------------------------------------------------------	
	void RT_RadioInteriorRequestCallback(MyRadioComponent radio)
	{
		m_radio = radio;
	}
}