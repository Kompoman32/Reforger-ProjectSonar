class MyRadioStationTrackInfoTimestampWrapper {
	WorldTimestamp m_timestamp;

	void MyRadioStationTrackInfoTimestampWrapper() {}
	
	
	// ## Extract/Inject
	// Extracting data from instance into snapshot, and injecting data from snapshot to instance.
	// Snapshot is meant to be fast to work with, so values are left uncompressed to avoid extra work when accessing these values.

	// ## Encode/Decode
	// Encoding snapshot into a packet and decoding snapshot from a packet.
	// Packets need to be as small as possible, so this process tries to reduce the size as much as it can.
	// Knowing what range of values can certain variable have and encoding that range in minimum number of bits required is key.
	// If it is to assume the full range of values is needed, helpers that already implement those for different types can be used.

	static bool Extract(MyRadioStationTrackInfoTimestampWrapper instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Fill a snapshot with values from an instance.
		snapshot.SerializeBytes(instance.m_timestamp, 8);
		return true;
	}

	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, MyRadioStationTrackInfoTimestampWrapper instance)
	{
		// Fill an instance with values from snapshot.
		snapshot.SerializeBytes(instance.m_timestamp, 8);
		return true;
	}

	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		// Read values from snapshot, encode them into smaller representation, then
		// write them into packet.
		snapshot.Serialize(packet, 8);
	}

	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		// Read values from packet, decode them into their original representation,
		// then write them into snapshot.
		snapshot.Serialize(packet, 8);
		return true;
	}

	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
	{
		// Compare two snapshots and determine whether they are the same.
		return lhs.CompareSnapshots(rhs, 8);
	}

	static bool PropCompare(MyRadioStationTrackInfoTimestampWrapper instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		// Determine whether current values in instance are sufficiently different from
		// an existing snapshot that it's worth creating new one.
		// For float or vector values, it is possible to use some threshold to avoid creating too
		// many snapshots due to tiny changes in these values.
		return snapshot.Compare(instance.m_timestamp, 8);
	}
	
	
}