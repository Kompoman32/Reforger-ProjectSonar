class RT_PS_CustomRadioStationTrackInfo {
	ResourceName m_sProjectFile;
	int m_iTrackIndex;
	int m_iTrackSize;
	bool m_bIsDJ;	

	//------------------------------------------------------------------------------------------------	
	static bool Extract(RT_PS_CustomRadioStationTrackInfo instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		if (!instance) return true;
		
		// Fill a snapshot with values from an instance.
		snapshot.SerializeString(instance.m_sProjectFile);
		snapshot.SerializeInt(instance.m_iTrackIndex);
		snapshot.SerializeInt(instance.m_iTrackSize);
		snapshot.SerializeBool(instance.m_bIsDJ);
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, RT_PS_CustomRadioStationTrackInfo instance)
	{
		if (!instance) return true;
		
		// Fill an instance with values from snapshot.
		snapshot.SerializeString(instance.m_sProjectFile);
		snapshot.SerializeInt(instance.m_iTrackIndex);
		snapshot.SerializeInt(instance.m_iTrackSize);
		snapshot.SerializeBool(instance.m_bIsDJ);
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		// Read values from snapshot, encode them into smaller representation, then
		// write them into packet.
		snapshot.EncodeString(packet);	// m_sProjectFile
		snapshot.EncodeInt(packet);		// m_iTrackIndex
		snapshot.EncodeInt(packet);	// m_iTrackSize
		snapshot.EncodeBool(packet);	// m_bIsDJ
	}

	//------------------------------------------------------------------------------------------------	
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Read values from packet, decode them into their original representation,
		// then write them into snapshot.
		snapshot.DecodeString(packet);	// m_sProjectFile
		snapshot.DecodeInt(packet);		// m_iTrackIndex
		snapshot.DecodeInt(packet);	// m_iTrackSize
		snapshot.DecodeBool(packet);	// m_bIsDJ
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
	{
		// Compare two snapshots and determine whether they are the same.
		return 
			lhs.CompareStringSnapshots(rhs)	
			&& lhs.CompareSnapshots(rhs, 4)
			&& lhs.CompareSnapshots(rhs, 4)
			&& lhs.CompareSnapshots(rhs, 1);
		;
	}

	//------------------------------------------------------------------------------------------------	
	static bool PropCompare(RT_PS_CustomRadioStationTrackInfo instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		if (!instance) return true;
		
		// Determine whether current values in instance are sufficiently different from
		// an existing snapshot that it's worth creating new one.
		// For float or vector values, it is possible to use some threshold to avoid creating too
		// many snapshots due to tiny changes in these values.
		return snapshot.CompareString(instance.m_sProjectFile)
				&& snapshot.CompareInt(instance.m_iTrackIndex)
				&& snapshot.CompareInt(instance.m_iTrackSize)
				&& snapshot.CompareBool(instance.m_bIsDJ);
	}
}