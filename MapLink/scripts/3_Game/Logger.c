static const int MLERROR = 0;
static const int MLVERBOSE = 1;
static const int MLINFO = 2;
static const int MLDEBUG = 3;


class MLLog extends Managed {
	
	static ref MLLogFileInstance m_MapLinkLogFileInstance;
	
	static MLLogFileInstance GetInstance(){
		if (!m_MapLinkLogFileInstance){m_MapLinkLogFileInstance = new MLLogFileInstance;}
		return m_MapLinkLogFileInstance;
	}
	
	static void Log(string text, int loglevel = 1) {
		GetInstance().DoLog(text,loglevel);
	}
	
	static void Info(string text){
		GetInstance().DoLog(text, MLINFO);
	}
	
	static void Debug(string text){
		GetInstance().DoLog(text, MLDEBUG);
	}

	static void Err(string text){
		Error("[MapLink] [ERROR]" + text);
		GetInstance().DoLog(text, MLERROR);
	}
	
};
class MLLogFileInstance extends Managed {
	
	protected int				m_LogLevel	= 3;
	protected bool 			m_isInit = false;
	protected bool				m_LogToApi = true;
	
	protected static string LogDir = "$profile:";
	protected FileHandle		m_FileHandle;
	
	void MLLogFileInstance(int LogLevel = 4) {	
		m_LogLevel = LogLevel;	
		if ( !GetGame().IsServer() || GetGame().IsClient() ){
			return;	
		}
		m_FileHandle = CreateFile(LogDir + "MapLink_" + GetDateStampFile() + ".log");
		if (m_FileHandle != 0){
			m_isInit = true;
		}
	}
	
	void ~MLLogFileInstance() {
		if ( m_isInit ) {
			CloseFile(m_FileHandle);
		}
	}
	
	void SetLogLevel(int LogLevel){
		m_LogLevel = LogLevel;
	}
	
	protected FileHandle CreateFile(string path) {
		if ( !GetGame().IsServer() || GetGame().IsClient() ){
			return null;	
		}
		
		FileHandle fHandle = OpenFile(path, FileMode.WRITE);
		if (fHandle != 0) {
			FPrintln(fHandle, "MapLink Log Started: " + GetDateStamp() + " " + GetTimeStamp() );
			return fHandle;
		}
		Error("[MapLink][Error] Unable to create" + path + " file in Profile.");
		return fHandle;
	}
	
	protected static string GetDateStamp() {
		int yr, mth, day;
		GetYearMonthDay(yr, mth, day);
		string sday = day.ToString();
		if (sday.Length() == 1){
			sday = "0" + sday;
		}
		
		string smth = mth.ToString();
		if (smth.Length() == 1){
			smth = "0" + mth.ToString();
		}
		
		return yr.ToString() + "-" + smth + "-" + sday;
	}
	
	protected static string GetDateStampFile() {
		int hr, min, sec;
		GetHourMinuteSecond(hr, min, sec);
		
		string ssec = sec.ToString();
		if (ssec.Length() == 1){
			ssec = "0" + ssec;
		}
		string smin = min.ToString();
		if (smin.Length() == 1){
			smin = "0" + smin;
		}
		string shr = hr.ToString();
		if (shr.Length() == 1) {
			shr = "0" + shr;
		}
		
		return  GetDateStamp() + "_" + shr + "-" + smin + "-" + ssec;
	}
	
	protected static string GetTimeStamp() {
		int hr, min, sec;
		GetHourMinuteSecond(hr, min, sec);
		
		string ssec = sec.ToString();
		if (ssec.Length() == 1){
			ssec = "0" + ssec;
		}
		string smin = min.ToString();
		if (smin.Length() == 1){
			smin = "0" + smin;
		}
		string shr = hr.ToString();
		if (shr.Length() == 1) {
			shr = "0" + shr;
		}
		
		return  shr + ":" + smin + ":" + ssec;
	}
	
	
	void DoLog(string text, int loglevel = 1)
	{	
		if (m_LogLevel < loglevel){
			return;
		}
		if (loglevel == 2) {
			GetGame().AdminLog("[MapLink]" + GetTag(loglevel) + text);
		}
		if (m_isInit){
			//Print("[MapLink] " + GetTag(loglevel) + GetTimeStamp() + " | " + text);
			string towrite = GetTag(loglevel)  + GetTimeStamp() + " | " + " " + text;
			FPrintln(m_FileHandle, towrite);
		} else {
			Print("[MapLink]" + GetTag(loglevel) + " " + text);
		}
		if (m_LogToApi){
			UApi().Rest().Log(GetJsonObject(text, loglevel));
		}
	}
	
	protected static string GetTag(int loglevel){
		switch ( loglevel ) {
			case MLERROR:
				return "[ERROR] ";
				break;
			case MLVERBOSE:
				return "[VERBOSE] ";
				break;
			case MLDEBUG:
				return "[DEBUG] ";
				break;
			case MLINFO:
				return "[INFO] ";
				break;
			default:
				return "[INFO] ";
				break;
		}
		return "[NULL] ";
	}
	
	static string GetJsonObject(string text, int loglevel) {
		string Level = "INFO";
		
		switch ( loglevel ) {
			case MLERROR:
				Level = "ERROR";
				break;
			case MLVERBOSE:
				Level =  "VERBOSE";
				break;
			case MLDEBUG:
				Level =  "DEBUG";
				break;
			case MLINFO:
				Level =  "INFO";
				break;
			default:
				Level =  "INFO";
				break;
		}
		autoptr MapLinkLogObject obj = new MapLinkLogObject(text, Level);
		return obj.ToJson();
	}
}

class MapLinkLogObject extends UApiObject_Base {
	
	string Message;
	string Level;
	string Server;
	
	void MapLinkLogObject(string text, string loglevel){
		Message = text;
		Level = loglevel;
		Server = UApiConfig().ServerID;
		
	}
	
	override string ToJson(){
		string jsonString = JsonFileLoader<MapLinkLogObject>.JsonMakeData(this);
		return jsonString;
	}
	
}