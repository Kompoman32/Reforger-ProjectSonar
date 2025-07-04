class RT_PS_CallToRadioDialog: SCR_ConfigurableDialogUi
{
	ref array<ref RT_PS_CustomRadioStation> m_aStations= {};

	protected const ResourceName DIALOGS_CONFIG = "{814FCA3CB7851F6B}Configs/Dialogs/CommonDialogs.conf";
	protected const ResourceName DIALOGS_PRESET = "call_to_radio";

	//------------------------------------------------------------------------------------------------
	void RT_PS_CallToRadioDialog(array<ref RT_PS_CustomRadioStation> radioStations)
	{				
		foreach (RT_PS_CustomRadioStation x : radioStations)
		{
			m_aStations.Insert(x);
		}

		SCR_ConfigurableDialogUi.CreateFromPreset(DIALOGS_CONFIG, DIALOGS_PRESET, this);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen(SCR_ConfigurableDialogUiPreset preset)
	{
			
		ButtonWidget selectButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("RadioStationsList"));
		SCR_ComboBoxComponent combobox= SCR_ComboBoxComponent.Cast(selectButton.FindHandler(SCR_ComboBoxComponent));

		combobox.m_aElementNames.Clear();
		
		foreach (RT_PS_CustomRadioStation x : m_aStations)
		{
			combobox.m_aElementNames.Insert(x.m_sRadiostationName);
		}
		
		combobox.SetCurrentItem(0);
		
		
		ButtonWidget confirmButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("ConfirmButton"));
		SCR_InputButtonComponent confirmButtonHandler= SCR_InputButtonComponent.Cast(confirmButton.FindHandler(SCR_InputButtonComponent));
		BindButtonConfirm(confirmButtonHandler);
		
		ButtonWidget cancelButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("CancelButton"));
		SCR_InputButtonComponent cancelButtonHandler= SCR_InputButtonComponent.Cast(cancelButton.FindHandler(SCR_InputButtonComponent));
		BindButtonConfirm(cancelButtonHandler);
	}
	
	int GetSelectedStationIndex()
	{
		ButtonWidget selectButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("RadioStationsList"));
		SCR_ComboBoxComponent combobox= SCR_ComboBoxComponent.Cast(selectButton.FindHandler(SCR_ComboBoxComponent));
		
		return combobox.m_iSelectedItem;
	}
}
