//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_PS_ToggleRadioContextAction : SCR_SelectedEntitiesContextAction
{	
	override bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		bool isValid = false;
		RT_PS_CustomRadioComponent radio = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(selectedEntity, isValid, radio);

		return isValid && radio.IsActive();
	}
	
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{		
		return true;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		bool isValid = false;
		RT_PS_CustomRadioComponent radio = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(selectedEntity, isValid, radio);
		
		if (!isValid) return;
		
		radio.ActionEnableDisable(!radio.Enabled());
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_PS_ChangeRadiostationContextAction : RT_PS_ToggleRadioContextAction
{		
	override bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
	{
		bool isValid = false;
		RT_PS_CustomRadioComponent radio = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(selectedEntity, isValid, radio);
		
		if (!isValid) return false;
		
		return radio.Enabled() && RT_PS_CustomRadioAntennaSystem.GetInstance().GetRadiostaionsCount() > 1;
	}
	
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		bool isValid = false;
		RT_PS_CustomRadioComponent radio = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(selectedEntity, isValid, radio);
		
		if (!isValid) return;
		
		radio.ActionChangeStation();
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
class RT_PS_FixRadioContextAction : RT_PS_ToggleRadioContextAction
{		
	override void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
	{		
		bool isValid = false;
		RT_PS_CustomRadioComponent radio = null;
		RT_PS_Utils.GetRadioComponentFromAttribute(selectedEntity, isValid, radio);
		
		if (!isValid) return;
		
		radio.ActionReset();
	}
};

