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
}