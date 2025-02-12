


class MyRadioStationComponentClass: ScriptComponentClass
{
	static override array<typename> Requires(IEntityComponentSource src)
	{
		return {MyRadioAntennaComponent};
	}
}

class MyRadioStationComponent: ScriptComponent 
{
	protected IEntity m_owner;

		
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
	
	override void OnPostInit(IEntity owner)
	{
		m_owner = owner;
	}
	
	MyRadioStationTrackInfo GetNewTrack() {		
		if (!m_TracksLengths || m_TracksLengths.Count() == 0 && m_djTracksLengths.Count() == 0) {
			PrintFormat("Get new track (%1): %2", m_radiostationName, null);
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
			PrintFormat("Get new track  (%1): %2", m_radiostationName, null);
			return null;
		}
		
		auto trackIndex = Math.RandomInt(0, tracks.Count());
		auto trackLength = tracks.Get(trackIndex);
		
		MyRadioStationTrackInfo newTrack = new MyRadioStationTrackInfo();
		
		newTrack.m_projectFile = m_stationAudioProject;
		newTrack.m_trackIndex = trackIndex;
		newTrack.m_trackSize = trackLength;
		newTrack.b_isDJ = isDJ;
		
		
		PrintFormat("Get new track  (%1): %2", m_radiostationName, newTrack);
		return newTrack;
	}	
}