static const int MLERROR = 0;
static const int MLVERBOSE = 1;
static const int MLINFO = 2;
static const int MLDEBUG = 3;


class MLLog extends Managed {
	
	protected static ref MLLogFileInstance m_MapLinkLogFileInstance;
	
	static MLLogFileInstance GetInstance(){
		if (!m_MapLinkLogFileInstance){m_MapLinkLogFileInstance = new MLLogFileInstance;}
		return m_MapLinkLogFileInstance;
	}
	
	static void Log(string text, int level = 1) {
		GetInstance().DoLog(text,level);
	}
	
	static void Info(string text){
		GetInstance().DoLog(text, MLINFO);
	}
	
	static void Debug(string text){
		GetInstance().DoLog(text, MLDEBUG);
	}

	static void Err(string text){
		Error2("[MapLink] Error", text);
		GetInstance().DoLog(text, MLERROR);
	}
	
	static void SetLogLevels(int level, int apiLevel = -99){
		if (apiLevel == -99){
			apiLevel = level;
		}
		GetInstance().SetLogLevel(level);
		GetInstance().SetApiLogLevel(apiLevel);
	}
	
};
class MLLogFileInstance extends Managed {
	
	protected int				m_LogLevel	= 3;
	protected int				m_LogToApiLevel = 3;
	protected bool 			m_isInit = false;
	
	protected static string LogDir = "$profile:";
	protected FileHandle		m_FileHandle;
	
	void MLLogFileInstance(int level = 4) {	
		m_LogLevel = level;	
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
	
	void SetLogLevel(int level){
		m_LogLevel = level;
	}
	
	void SetApiLogLevel(int level){
		m_LogToApiLevel = level;
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
		Error2("[MapLink] Error", "Unable to create" + path + " file in Profile.");
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
	
	
	void DoLog(string text, int level = 1)
	{	
		if (level == 2 && m_LogLevel >= level) {
			GetGame().AdminLog("[MapLink]" + GetTag(level) + text);
		}
		if (m_isInit && m_LogLevel >= level){
			//Print("[MapLink] " + GetTag(level) + GetTimeStamp() + " | " + text);
			string towrite = GetTag(level)  + GetTimeStamp() + " | " + " " + text;
			FPrintln(m_FileHandle, towrite);
		} else if (m_LogLevel >= level) {
			Print("[MapLink]" + GetTag(level) + " " + text);
		}
		if (m_LogToApiLevel >= level){
			UApi().Rest().Log(GetJsonObject(text, level));
		}
	}
	
	protected static string GetTag(int level){
		switch ( level ) {
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
	
	static string GetJsonObject(string text, int level) {
		string sLevel = "INFO";
		
		switch ( level ) {
			case MLERROR:
				sLevel = "ERROR";
				break;
			case MLVERBOSE:
				sLevel =  "VERBOSE";
				break;
			case MLDEBUG:
				sLevel =  "DEBUG";
				break;
			case MLINFO:
				sLevel =  "INFO";
				break;
			default:
				sLevel =  "INFO";
				break;
		}
		autoptr MapLinkLogObject obj = new MapLinkLogObject(text, sLevel);
		return obj.ToJson();
	}
}

class MapLinkLogObject extends UApiObject_Base {
	
	string Message;
	string Level;
	
	void MapLinkLogObject(string text, string level){
		Message = text;
		Level = level;
		
	}
	
	override string ToJson(){
		string jsonString = JsonFileLoader<MapLinkLogObject>.JsonMakeData(this);
		return jsonString;
	}
	
}