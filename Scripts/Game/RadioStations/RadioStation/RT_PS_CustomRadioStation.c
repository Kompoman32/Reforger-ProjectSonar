[BaseContainerProps()]
class RT_PS_CustomRadioStation
{		
	[Attribute("", UIWidgets.EditBox, "Radiostation name")]
	string m_sRadiostationName;
	
	[Attribute("", UIWidgets.FileNamePicker, "Ref to .acp file", "acp")]
	ResourceName m_StationAudioProject;
	
	[Attribute("", UIWidgets.Slider, "Tracks Lengths", "1 3600 1")]
	ref array<int> m_aTracksLengths;
	
	[Attribute("", UIWidgets.EditBox, "Tracks Names (WIP just for future maybe)")]
	ref array<string> m_aTracksNames;
	
	[Attribute("0.2", UIWidgets.Slider, "DJ Tracks probability", "0 1 0.1")]
	float m_fDjProbability;
	
	[Attribute("", UIWidgets.Slider, "DJ Tracks Lengths", "1 3600 1")]
	ref array<int> m_aDjTracksLengths;
	
	[Attribute("", UIWidgets.EditBox, "DJ Tracks Names (WIP just for future maybe)")]
	ref array<string> m_aDjTracksNames;
	
	protected int m_lLastTrackIndex = -1;
	protected int m_lLastDJTrackIndex = -1;
	
	ref array<int> m_aTracksIndexes = {};
	ref array<int> m_aDjTracksIndexes = {};
	
	void RT_PS_CustomRadioStation() {
		if (Replication.IsServer()) 
		{		
			foreach (int i, int value : m_aTracksLengths)
			{
				m_aTracksIndexes.Insert(i);
			}
			
			foreach (int i, int value : m_aDjTracksLengths)
			{
				m_aDjTracksIndexes.Insert(i);
			}
			
			SCR_ArrayHelperT<int>.Shuffle(m_aTracksIndexes, 1);
			SCR_ArrayHelperT<int>.Shuffle(m_aDjTracksIndexes, 1);
			
			// Debug
			string tracksOrderText =  string.Format("Music [%1]", RT_PS_Utils.ArrayJoinStringInt(m_aTracksIndexes));
			string tracksDjOrderText =  string.Format("DJ [%1]", RT_PS_Utils.ArrayJoinStringInt(m_aDjTracksIndexes));
			
			Print(string.Format("[SONAR] Station %1 tracks order: %2 %3", m_sRadiostationName, tracksOrderText, tracksDjOrderText), LogLevel.DEBUG)			
		}
	}

	//------------------------------------------------------------------------------------------------	
	RT_PS_CustomRadioStationTrackInfo GetNewTrack() {		
		if (!m_aTracksLengths || m_aTracksLengths.Count() == 0 && m_aDjTracksLengths.Count() == 0) {
			return null;
		}
		
		array<int> tracks = m_aTracksLengths;
		array<int> tracksIndexes = m_aTracksIndexes;
		bool isDJ = false;
		
		float djProbability = m_fDjProbability;
		
		if (m_aTracksLengths.Count() == 0) djProbability = 1;
		if (m_aDjTracksLengths.Count() == 0) djProbability = 0;
		
		if (Math.RandomFloat01() < m_fDjProbability) 
		{
			isDJ = true;
			tracks = m_aDjTracksLengths;
			tracksIndexes = m_aDjTracksIndexes;
		}
		
		if (tracks.Count() == 0) 
		{
			return null;
		}
		
		auto trackIndex = GetNewTrackIndex(isDJ);
		auto trackLength = tracks.Get(tracksIndexes.Get(trackIndex));
		
		if (isDJ) 
		{
			m_lLastDJTrackIndex = trackIndex;
		}
		else 
		{
			m_lLastTrackIndex = trackIndex;
		}
		
		
		RT_PS_CustomRadioStationTrackInfo newTrack = new RT_PS_CustomRadioStationTrackInfo();
		
		newTrack.m_sProjectFile = m_StationAudioProject;
		newTrack.m_iTrackIndex = RT_PS_Utils.ArrayGet(tracksIndexes, trackIndex);
		newTrack.m_iTrackSize = trackLength;
		newTrack.m_bIsDJ = isDJ;
		
		return newTrack;
	}	
	
	//------------------------------------------------------------------------------------------------	
	protected int GetNewTrackIndex(bool pIsDj)
	{
		array<int> tracks = m_aTracksIndexes;
		
		int lastTrackIndex = m_lLastTrackIndex;
		
		if (pIsDj)
		{
			tracks = m_aDjTracksIndexes;
			lastTrackIndex = m_lLastDJTrackIndex;
		}
		
		if (tracks.Count() == 1) return 0;
		
		if (tracks.Count() == 2) {
			if (lastTrackIndex == -1) return 0;
			
			return 2 - lastTrackIndex - 1;
		}
		
		/*
		int index = Math.RandomInt(0, tracks.Count());
		
		while(index == lastTrackIndex) 
		{
			index = Math.RandomInt(0, tracks.Count());
		}
		*/
		
		int index = lastTrackIndex + 1;
		if (index > tracks.Count() - 1) {
			index = 0;
		}
		
		return index;		
	}
	
	//------------------------------------------------------------------------------------------------	
	string GetTrackName(int index, bool isDJ)
	{
		array<string> tracksNames = m_aTracksNames;
		
		if (isDJ)
		{
			tracksNames = m_aDjTracksNames;
		}
		
		return RT_PS_Utils.ArrayGet(tracksNames, index);	
	}
	
	
}