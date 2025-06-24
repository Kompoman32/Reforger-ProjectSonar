class CustomRadioStationTrackInfo {
	ResourceName m_projectFile;
	int m_trackIndex;
	int m_trackSize;
	bool b_isDJ;

	void CustomRadioStationTrackInfo() {}
	
	
	// ## Extract/Inject
	// Extracting data from instance into snapshot, and injecting data from snapshot to instance.
	// Snapshot is meant to be fast to work with, so values are left uncompressed to avoid extra work when accessing these values.

	// ## Encode/Decode
	// Encoding snapshot into a packet and decoding snapshot from a packet.
	// Packets need to be as small as possible, so this process tries to reduce the size as much as it can.
	// Knowing what range of values can certain variable have and encoding that range in minimum number of bits required is key.
	// If it is to assume the full range of values is needed, helpers that already implement those for different types can be used.

	static bool Extract(CustomRadioStationTrackInfo instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		if (!instance) return true;
		
		// Fill a snapshot with values from an instance.
		snapshot.SerializeString(instance.m_projectFile);
		snapshot.SerializeInt(instance.m_trackIndex);
		snapshot.SerializeInt(instance.m_trackSize);
		snapshot.SerializeBool(instance.b_isDJ);
		return true;
	}

	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, CustomRadioStationTrackInfo instance)
	{
		if (!instance) return true;
		
		// Fill an instance with values from snapshot.
		snapshot.SerializeString(instance.m_projectFile);
		snapshot.SerializeInt(instance.m_trackIndex);
		snapshot.SerializeInt(instance.m_trackSize);
		snapshot.SerializeBool(instance.b_isDJ);
		return true;
	}

	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		// Read values from snapshot, encode them into smaller representation, then
		// write them into packet.
		snapshot.EncodeString(packet);	// m_projectFile
		snapshot.EncodeInt(packet);		// m_trackIndex
		snapshot.EncodeInt(packet);	// m_trackSize
		snapshot.EncodeBool(packet);	// b_isDJ
	}

	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Read values from packet, decode them into their original representation,
		// then write them into snapshot.
		snapshot.DecodeString(packet);	// m_projectFile
		snapshot.DecodeInt(packet);		// m_trackIndex
		snapshot.DecodeInt(packet);	// m_trackSize
		snapshot.DecodeBool(packet);	// b_isDJ
		return true;
	}

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

	static bool PropCompare(CustomRadioStationTrackInfo instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		if (!instance) return true;
		
		// Determine whether current values in instance are sufficiently different from
		// an existing snapshot that it's worth creating new one.
		// For float or vector values, it is possible to use some threshold to avoid creating too
		// many snapshots due to tiny changes in these values.
		return snapshot.CompareString(instance.m_projectFile)
				&& snapshot.CompareInt(instance.m_trackIndex)
				&& snapshot.CompareInt(instance.m_trackSize)
				&& snapshot.CompareBool(instance.b_isDJ);
	}
	
	
}