
[BaseContainerProps()]
class CustomRadioStation
{		
	private int m_lastTrackIndex = -1;
	
	[Attribute("", UIWidgets.EditBox, "Radiostation name")]
	string m_radiostationName;
	
	[Attribute("", UIWidgets.FileNamePicker, "Ref to .acp file", "acp")]
	ResourceName m_stationAudioProject;
	
	[Attribute("", UIWidgets.Slider, "Tracks Lengths", "1 3600 1")]
	ref array<int> m_TracksLengths;
	
	[Attribute("", UIWidgets.Slider, "DJ Tracks probability", "0 1 0.1")]
	float m_djProbability = 0.2;
	
	[Attribute("", UIWidgets.Slider, "DJ Tracks Lengths", "1 3600 1")]
	ref array<int> m_djTracksLengths;
	
	MyRadioStationTrackInfo GetNewTrack() {		
		if (!m_TracksLengths || m_TracksLengths.Count() == 0 && m_djTracksLengths.Count() == 0) {
			return null;
		}
		
		array<int> tracks = m_TracksLengths;
		bool isDJ = false;
		
		float djProbability = m_djProbability;
		
		if (m_TracksLengths.Count() == 0) djProbability = 1;
		if (m_djTracksLengths.Count() == 0) djProbability = 0;
		
		if (Math.RandomFloat01() < m_djProbability) 
		{
			isDJ = true;
			tracks = m_djTracksLengths;
		}
		
		if (tracks.Count() == 0) 
		{
			return null;
		}
		
		auto trackIndex = GetNewTrackIndex(isDJ);
		auto trackLength = tracks.Get(trackIndex);
		
		m_lastTrackIndex = trackIndex;
		
		MyRadioStationTrackInfo newTrack = new MyRadioStationTrackInfo();
		
		newTrack.m_projectFile = m_stationAudioProject;
		newTrack.m_trackIndex = trackIndex;
		newTrack.m_trackSize = trackLength;
		newTrack.b_isDJ = isDJ;
		
		return newTrack;
	}	
	
	int GetNewTrackIndex(bool isDJ)
	{
		array<int> tracks = m_TracksLengths;
		
		if (isDJ) tracks = m_djTracksLengths;
		
		if (tracks.Count() == 1) return 0;
		
		if (tracks.Count() == 2) return 2 - m_lastTrackIndex - 1;
		
		int index = Math.RandomInt(0, tracks.Count());
		
		while(index == m_lastTrackIndex) 
		{
			index = Math.RandomInt(0, tracks.Count());
		}
		
		return index;		
	}
	
	
}