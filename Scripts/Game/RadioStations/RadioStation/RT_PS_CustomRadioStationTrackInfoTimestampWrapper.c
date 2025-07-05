class RT_PS_CustomRadioStationTrackInfoTimestampWrapper {
	WorldTimestamp m_Timestamp;	

	//------------------------------------------------------------------------------------------------	
	static bool Extract(RT_PS_CustomRadioStationTrackInfoTimestampWrapper instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Fill a snapshot with values from an instance.
		snapshot.SerializeBytes(instance.m_Timestamp, 8);
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, RT_PS_CustomRadioStationTrackInfoTimestampWrapper instance)
	{
		// Fill an instance with values from snapshot.
		snapshot.SerializeBytes(instance.m_Timestamp, 8);
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		// Read values from snapshot, encode them into smaller representation, then
		// write them into packet.
		snapshot.Serialize(packet, 8);
	}

	//------------------------------------------------------------------------------------------------	
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Read values from packet, decode them into their original representation,
		// then write them into snapshot.
		snapshot.Serialize(packet, 8);
		return true;
	}

	//------------------------------------------------------------------------------------------------	
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
	{
		// Compare two snapshots and determine whether they are the same.
		return lhs.CompareSnapshots(rhs, 8);
	}

	//------------------------------------------------------------------------------------------------	
	static bool PropCompare(RT_PS_CustomRadioStationTrackInfoTimestampWrapper instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		// Determine whether current values in instance are sufficiently different from
		// an existing snapshot that it's worth creating new one.
		// For float or vector values, it is possible to use some threshold to avoid creating too
		// many snapshots due to tiny changes in these values.
		return snapshot.Compare(instance.m_Timestamp, 8);
	}
	
	
}