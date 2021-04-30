ref DeparturePointMenu m_DeparturePointMenu;


class DeparturePointMenu extends UIScriptedMenu
{
	static string 			m_LayoutPath = "MapLink/gui/layouts/TransferMenu.layout";
	
	protected MapLinkDepaturePoint m_MapLinkDepaturePoint;
	
	protected ref Timer CountDownTimer
	protected int CountDownTimeRemaining;
	
	protected Widget m_ArrivalPoints;
	
	protected ButtonWidget m_Cancel;
	
	protected Widget m_TransferingTimeFrame;
	
	protected TextWidget m_TranferingCountDown;
	protected RichTextWidget m_Heading;
	
	protected bool m_ShouldChangeSize = true;
	protected ref SizeToChild			m_ContentResize;
	
	protected autoptr array<autoptr DeparturePointWidget> dPointWidgets;
	
	protected string m_TravelTo_ArrivalPoint;
	protected string m_TravelTo_ServerName;
	
	override Widget Init()
    {
		CountDownTimer = new Timer();
		layoutRoot 				= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_LayoutPath));
		
		m_ArrivalPoints 		= Widget.Cast(layoutRoot.FindAnyWidget("ArrivalPoints_Grid"));
		
		//Print(m_ContentResize);
		//m_ArrivalPoints.GetScript( m_ContentResize );
		//Print(m_ContentResize);
		m_Cancel 				= ButtonWidget.Cast(layoutRoot.FindAnyWidget("Cancel"));
	 
		m_TransferingTimeFrame 	= Widget.Cast(layoutRoot.FindAnyWidget("TransferingTimeFrame"));
		
		m_TranferingCountDown 	= TextWidget.Cast(layoutRoot.FindAnyWidget("TranferingCountDown"));
		
		m_Heading				= RichTextWidget.Cast(layoutRoot.FindAnyWidget("Heading"));
		
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CreateChildern, 50);
		return layoutRoot;
	}
	
	void ~DeparturePointMenu(){
		MLUnLockControls();
		if (CountDownTimer.IsRunning()){
			CountDownTimer.Stop();
			m_TransferingTimeFrame.Show(false);
		}
		delete dPointWidgets;
		delete CountDownTimer;
	}
	
	void SetDeparturePoint(MapLinkDepaturePoint dPoint){
		m_MapLinkDepaturePoint = MapLinkDepaturePoint.Cast(dPoint);
		MLLockControls();
	}
	
	
	void CreateChildern(){
		if (!dPointWidgets){
			 dPointWidgets = new array<autoptr DeparturePointWidget>;
		}
		for (int i = 0; i < m_MapLinkDepaturePoint.ArrivalPoints.Count(); i++){
			MapLinkArrivalPoint arrivalPoint = MapLinkArrivalPoint.Cast(m_MapLinkDepaturePoint.ArrivalPoints.Get(i).Get());
			for (int j = 0; j < arrivalPoint.SpawnPoints.Count(); j++){
				if (arrivalPoint.SpawnPoints.Get(j).ServerName != UApiConfig().ServerID){
					dPointWidgets.Insert(new DeparturePointWidget(m_ArrivalPoints, this, m_MapLinkDepaturePoint.ArrivalPoints.Get(i), arrivalPoint.SpawnPoints.Get(j)));
				}
			}
		}
	}
	
	
	void InitTravel(string arrivalPoint, int CountDown, string serverName){
		
		m_TravelTo_ArrivalPoint = arrivalPoint;
		m_TravelTo_ServerName = serverName;
		CountDownTimeRemaining = CountDown;
		CountDownTimer.Run(1, this, "StepCountDown", NULL, true);
		m_TranferingCountDown.SetText(CountDownTimeRemaining.ToString());
		m_TransferingTimeFrame.Show(true);
	}
	
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (w == m_Cancel){
			Cancel();
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
	
	void StepCountDown(){
		m_TranferingCountDown.SetText(string.ToString(--CountDownTimeRemaining));
		if (CountDownTimeRemaining <= 0){
			CountDownTimer.Stop();
			DoTravel();
		}
	}
	
	void DoTravel(){
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player){
			player.UApiRequestTravel(m_TravelTo_ArrivalPoint, m_TravelTo_ServerName);
		}
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.CloseSelf, 1);
	}
	
	void CloseSelf(){
		GetGame().GetUIManager().CloseMenu(MAPLINK_DEPARTUREPOINTMENU);
	}
	
	void Cancel(){
		if (CountDownTimer.IsRunning()){
			CountDownTimer.Stop();
			m_TransferingTimeFrame.Show(false);
		} else {
			m_TravelTo_ArrivalPoint = "";
			m_TravelTo_ServerName = "";
			GetGame().GetUIManager().CloseMenu(MAPLINK_DEPARTUREPOINTMENU);
		}
	}
	
	protected void MLLockControls() {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        GetGame().GetUIManager().ShowUICursor(true);
    }

    protected void MLUnLockControls() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
    }
}