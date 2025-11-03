// Enabled
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_PS_Radio_Attribute_Enabled: SCR_BaseFloatValueHolderEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(radio.Enabled());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return;
		
		radio.ActionEnableDisable(var.GetBool());
	}
	
	//------------------------------------------------------------------------------------------------
	override void UpdateInterlinkedVariables(SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, bool isInit = false)
	{
		if (isInit)
		{
			manager.SetAttributeAsSubAttribute(RT_PS_Radio_Attribute_Volume);
			manager.SetAttributeAsSubAttribute(RT_PS_Radio_Attribute_RadioStations);
		}
				
		manager.SetAttributeEnabled(RT_PS_Radio_Attribute_Volume, var && var.GetBool());
		manager.SetAttributeEnabled(RT_PS_Radio_Attribute_RadioStations, var && var.GetBool());
	}
}

// Volume
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_PS_Radio_Attribute_Volume: SCR_BaseValueListEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{	
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return null;
		
		return SCR_BaseEditorAttributeVar.CreateInt(radio.GetVolume());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return;
		
		radio.ActionSetVolume(var.GetInt());
	}
}

// Radiostations
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class RT_PS_Radio_Attribute_RadioStations: SCR_BaseFloatValueHolderEditorAttribute
{	
	RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{		
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return null;
		
		m_RadioSystem = system;
				
		return SCR_BaseEditorAttributeVar.CreateInt(index);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		bool isValid = false;
		RT_PS_CustomRadioAntennaSystem system = null;
		RT_PS_CustomRadioComponent radio = null;
		int index = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(item, isValid, system, radio, index);
		
		if (!isValid) return;
		
		radio.ActionChangeStation(var.GetInt());
	}
	
	//------------------------------------------------------------------------------------------------
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		FillValues();
		outEntries.Insert(new SCR_BaseEditorAttributeFloatStringValues(m_aValues));
		return outEntries.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FillValues()
	{				
		if (!m_RadioSystem) return;
		
		m_aValues.Clear();
				
		SCR_EditorAttributeFloatStringValueHolder value;
		
		foreach (int i, RT_PS_CustomRadioStation radioStation: m_RadioSystem.m_aRadiostations)
		{
			value = new SCR_EditorAttributeFloatStringValueHolder();
			value.SetName(radioStation.m_sRadiostationName);
			value.SetFloatValue(i);
			
			m_aValues.Insert(value);
		}
	}
}