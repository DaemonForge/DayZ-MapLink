class MapLinkCountTimerWidget  extends ScriptedWidgetEventHandler {
	
	static string 				m_LayoutPath = "MapLink/gui/layouts/UnderProtectionInfo.layout";
		
	ref Widget					m_Root;
	
	ref TextWidget 				m_Time;
	
	ref Timer 						m_Timer;
	int 						m_CurrentTime = 0;
	
	void MapLinkCountTimerWidget(){
		CreateWidget();
	}
	
	protected void CreateWidget(){
		m_Root	= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath));
		m_Time	= TextWidget.Cast(m_Root.FindAnyWidget("Timer"));
		m_Root.SetHandler(this);
	}
	
	void ~MapLinkCountTimerWidget(){
		if (m_Root){
			m_Root.Show(false);
		}
		if (m_Timer ){
			if ( m_Timer.IsRunning()){
				m_Timer.Stop();
			}
		}
	}
	
	void Start(int time){
		if (!m_Root) {
			CreateWidget();
		}
		if (!m_Timer) {
			m_Timer = new Timer();
		}
		if (m_Timer.IsRunning()) {
			m_Timer.Stop();
		}
		m_CurrentTime = time;
		m_Time.SetText(m_CurrentTime.ToString());
		m_Root.Show(true);
		m_Timer.Run(1, this, "StepTimer", NULL, true);
	}
	
	protected void StepTimer(){
		m_Time.SetText(string.ToString(--m_CurrentTime));
		if (m_CurrentTime < 0){
			m_Timer.Stop();
			m_Root.Show(false);
		}
	}
	
	void Stop(){
		m_Timer.Stop();
		m_Root.Show(false);
	}
}