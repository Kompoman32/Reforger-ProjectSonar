class RT_PS_Utils {
	static string ArrayJoinStringInt(array<int> arr) {
		int count = arr.Count();
	
		string result = "";
		
		foreach (int i, int value: arr)
		{
			if (i < count - 1) 
			{
				result += string.Format("%1, ", value);
			} else {
				result += string.Format("%1", value);
			}
		}
		
		return result;
	}
	
	static Managed ArrayGet(array<Managed> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) return null;
		
		return arr.Get(index);
	}
	
	static string ArrayGet(array<string> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) {
			return string.Empty;
		} 
		
		return arr.Get(index);
	}
	
	static int ArrayGet(array<int> arr, int index)
	{
		if (index  < 0 || index >= arr.Count()) {
			return null;
		} 
		
		return arr.Get(index);
	}
}