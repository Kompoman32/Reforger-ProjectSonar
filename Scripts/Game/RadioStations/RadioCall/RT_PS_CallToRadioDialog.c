modded enum ChimeraMenuPreset
{
	CallToTheRadio
}

class RT_PS_CallToRadioDialog: MenuBase
{
	RT_PS_CustomRadioAntennaSystem m_RadioSystem;
	ref array<ref RT_PS_CustomRadioStation> m_aStations= {};
	
	SCR_ComboBoxComponent m_Combobox;
	
	ref ScriptInvoker m_OnConfirm = new ScriptInvoker();
	ref ScriptInvoker m_OnCancel = new ScriptInvoker();

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		const ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		if (world) 
		{
			m_RadioSystem = RT_PS_CustomRadioAntennaSystem.Cast(world.FindSystem(RT_PS_CustomRadioAntennaSystem));
		}
		
		if (m_RadioSystem)
		{
			foreach (RT_PS_CustomRadioStation x : m_RadioSystem.m_aRadiostations)
			{
				m_aStations.Insert(x);
			}
		}
			
		ButtonWidget selectButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("RadioStationsList"));
		m_Combobox = SCR_ComboBoxComponent.Cast(selectButton.FindHandler(SCR_ComboBoxComponent));
		
		if (!m_Combobox) return;

		m_Combobox.m_aElementNames.Clear();
		
		foreach (RT_PS_CustomRadioStation x : m_aStations)
		{
			m_Combobox.m_aElementNames.Insert(x.m_sRadiostationName);
		}
		
		m_Combobox.SetCurrentItem(0);
		
		ButtonWidget confirmButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("ConfirmButton"));
		SCR_InputButtonComponent confirmButtonHandler = SCR_InputButtonComponent.Cast(confirmButton.FindHandler(SCR_InputButtonComponent));
		confirmButtonHandler.m_OnActivated.Insert(OnConfirm);

		
		ButtonWidget cancelButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("CancelButton"));
		SCR_InputButtonComponent cancelButtonHandler= SCR_InputButtonComponent.Cast(cancelButton.FindHandler(SCR_InputButtonComponent));
		cancelButtonHandler.m_OnActivated.Insert(OnCancel);
	}
	
	int GetSelectedStationIndex()
	{		
		if (!m_Combobox) return 0;
		
		return m_Combobox.m_iSelectedItem;
	}
	
	//----------------------------------------------------------------------------------------
	protected void OnConfirm()
	{
		m_OnConfirm.Invoke(this);
		Close();
	}
	
	
	//----------------------------------------------------------------------------------------
	protected void OnCancel()
	{
		m_OnCancel.Invoke(this);
		Close();
	}
}
