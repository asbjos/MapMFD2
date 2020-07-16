#pragma once

//#include <fstream>
#include <iostream>
#include <string>

enum PROJECTION { EQUIRECTANGULAR, MILLER, MERCATOR, TRANSVERSEMERCATOR, EQUALEARTH, MOLLWEIDE, ORTELIUSOVAL, WINKELTRIPEL, RECTANGULARPOLYCONIC, AZIMUTHALEQUIDISTANT, LAMBERTAZIMUTHAL, CASSINI, LASTENTRYPROJECTION };
enum MAPFEATURE { BOX, CROSS, RINGS };
enum CONFIGSELECT { CONFIGTRACKMODE, CONFIGRADAR, CONFIGSHOWVESSELS, CONFIGPROJECTION, CONFIGFLIPPOLE, CONFIGRESETMAP, CONFIGGRIDSEP, CONFIGGRIDRES, CONFIGMAPRES, CONFIGMAPAUTOSIZE, CONFIGTRACKANGLEDELTA, CONFIGTRACKNUMORBITS, CONFIGPLANETVIEWSEGMENTS, CONFIGRESETALL, CONFIGDEBUGINFO, LASTENTRYCONFIG };
enum TRACKMODE { NOTRACK, LONGTRACK, LATLONGTRACK, LASTENTRYTRACK };
enum TARGETEXPANDMODES { EXPANDSPACEPORTS, EXPANDSPACECRAFT, EXPANDMOONS, EXPANDNONE = -1};

const int GROUND_TRACK_ITERATION_MAX_STEPS = int(2e4); // 2e4 is arbitrary max limit of what I'll allow. If you want a longer plot, there's no problem to increase time delta in config
const int GROUND_TRACK_HISTORY_SIZE = int(1e3); // Number of datapoints recorded to position history.
const int MAX_ZOOM = 256; // should be power of 2

struct
{
	int TRACK_MODE = 1;
	int PROJECTION = 0;
	int GRID_ANGLE_SEP = 30;
	int GRID_RESOLUTION = 2;
	int LINES_AMOUNT = 5000;
	double GROUNDTRACK_STEP = 0.1;
	double GROUNDTRACK_ORBITS = 3.5;
	int VIEW_CIRCLE_RESOLUTION = 90;
	bool ELEVATION_RADAR = false;
	bool SHOW_VESSELS = false;
} DEFAULT_VALUES;

typedef struct ReferenceObjectType {
	int index;
	OBJHANDLE handle;
	char name[20];
	double sunDist;

	// Now supporting up to 20 moons
	int moonCount = 0;
	OBJHANDLE moonHandle[20];
	int moonIndex[20];
	char moonName[20][20]; // 20 moons, 20 chars each
} PLANET;

class MapMFD : public MFD2
{
public:
	MapMFD(DWORD w, DWORD h, VESSEL* vessel);
	~MapMFD(void);
	char* ButtonLabel(int bt);
	int ButtonMenu(const MFDBUTTONMENU** menu) const;
	bool Update(oapi::Sketchpad* skp);
	static int MsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);
	bool ConsumeButton(int bt, int event);
	bool ConsumeKeyBuffered(DWORD key);
	bool ConsumeKeyImmediate(char* kstate);
	void StoreStatus(void) const;
	void RecallStatus(void);
	void WriteStatus(FILEHANDLE scn) const;
	void ReadStatus(FILEHANDLE scn);

	void MapScreen(oapi::Sketchpad* skp);
	void ConfigScreen(oapi::Sketchpad* skp);
	void ReferenceListScreen(oapi::Sketchpad* skp);
	void TargetListScreen(oapi::Sketchpad* skp);

	void BuildReferenceCache(void);
	void SetNewReference(OBJHANDLE hRef);
	bool AddOrRemoveTarget(OBJHANDLE hRef);
	bool ObjectAlreadyInTarget(OBJHANDLE hRef);

	bool DrawLine(double long0, double lat0, double long1, double lat1, oapi::Sketchpad* skp, bool safetyCheck = true);
	bool DrawFeature(double longitude, double latitude, int size, MAPFEATURE feature, oapi::Sketchpad* skp, char *label);
	void DrawOrbitTrack(double currentLong, double currentLat, ELEMENTS el, ORBITPARAM prm, oapi::Sketchpad* skp);

	bool TransformPoint(double longitude, double latitude, double *transformedLongitude, double *transformedLatitude, PROJECTION projection);
	char* GetProjectionName(void);
	char* GetCoordinateString(double longitude, double latitude);

	bool GetEquPosInXSeconds(double t, ELEMENTS el, ORBITPARAM prm, double currentLongitude, double* longitude, double* latitude, double *currentSpeed); // return false if altitude below surface
	void GetObjectEquPos(OBJHANDLE tgt, double* longitude, double* latitude, double *radius);
	void GetObjectRelativeElements(OBJHANDLE tgt, ELEMENTS& el, ORBITPARAM* prm);

	// Key input functions
	bool SetReferencePlanet(char* rstr);
	bool SetTargetObject(char* rstr);
	bool SetGridSeparation(char* rstr);
	bool SetGridResolution(char* rstr);
	bool SetNumberOrbitsDisplayed(char* rstr);

	// Some basic mathematical functions
	double MnA2TrA(double MnA, double Ecc);
	double TrA2MnA(double TrA, double Ecc);
	double EccentricAnomaly(double ecc, double TrA);
	VECTOR3 Ecl2Equ(VECTOR3 Ecl);

	void myStrncpy(char* writeTo, const char* readFrom, int len);

private:
	OBJHANDLE ref; // planet reference
	double refMu;
	double refRad;

	OBJHANDLE targets[10]; // object targets, support up to 10
	int numTargets = 0;
	float targetCoord[10][2]; // specifically for setting target user coordinate. Is most of the time NULL, unless set by a user, then index number numTargets is overloaded. The corresponding targets entry is NULL. Is in degrees!

	bool configScreen = false;
	CONFIGSELECT configSelection = CONFIGSELECT(0); // initialise to first entry

	bool referenceListScreen = false;
	bool targetListScreen = false;
	int referenceSelection = 0;
	int currentReferenceTopListed = 0;
	int referencePlanetSelection = 0; // planet nr. selected
	//int numberReferenceChoices = 1;
	int referenceExpand = -1; // expand planet nr. this to show moons
	bool updateReferenceCache = true;
	PLANET sortedPlanetsCache[30];
	int totalPlanets = 0;

	TARGETEXPANDMODES targetExpand = EXPANDNONE;
	int targetSelection = 0;
	int currentTargetTopListed = 0;

	VESSEL* v; // vessel instance
	int W, H; // screen width and height in pixels
	int textDX, textDY; // Delta X & Y for text lines
	PROJECTION proj = PROJECTION(DEFAULT_VALUES.PROJECTION); // map projection

	int gridAngleSeparation = DEFAULT_VALUES.GRID_ANGLE_SEP; // degrees
	int gridResolution = DEFAULT_VALUES.GRID_RESOLUTION; // degrees
	int skipEveryNLines = 10; // skip every N lines when drawing from map. 6 is auto. Max 5 else.
	int skipEveryNLinesMax = 10; // should initially be the same as skipEveryNLines
	bool autoResolution = true; // automatically control skipEveryNLines value
	int defaultMapLinesAmount = DEFAULT_VALUES.LINES_AMOUNT; // used for autoResolution when desciding what the max map lines to display is
	double orbitTrackAngleDelta = DEFAULT_VALUES.GROUNDTRACK_STEP; // TrA angle delta per track leg. Set to 0.01, 0.05, 0.1, 0.5, 1.0
	bool orbitTrackGround = DEFAULT_VALUES.TRACK_MODE; // ground track mode. If false, set to orbit track mode.
	int viewCircleResolution = DEFAULT_VALUES.VIEW_CIRCLE_RESOLUTION; // number of segments in the view circle ("what is visible").
	double orbitTrackOrbitsNumber = DEFAULT_VALUES.GROUNDTRACK_ORBITS; // number of orbits to show of ground track (limited by max 1e6 seconds).
	bool displayElevationRadar = DEFAULT_VALUES.ELEVATION_RADAR; // wether to show elevation radar or not. It can be FPS hungry.

	// Debug
	int pointsOutside = 0;
	int pointsInside = 0;

	double centreLat = 0.0, centreLong = 0.0;
	int centreZoom = 1; // no floating point errors, please
	bool azimuthalEquidistantNortPole = true;
	TRACKMODE trackPosition = NOTRACK;

	bool showVessels = DEFAULT_VALUES.SHOW_VESSELS;

	//char cacheMap[100000][35];
	float cacheMap[100000][2];
	bool updateCache = true; // never change this other than when caching or changing reference. If memory is lost, the update call is also reactivated.
	bool coastMap, mapExists;

	double shipHistory[GROUND_TRACK_HISTORY_SIZE][3]; // record ship coordinates, and last is time stamp
	int shipHistoryIndex = 0; // index to record to in shipHistory
	int shipHistoryLength = 0; // history record length
};

bool resetCommand = false; // inform user and RecallStatus that we want to reset
bool debugInformation = false;

// Static memory for Store/Recall Status
struct
{
	OBJHANDLE ref;
	double refMu;
	double refRad;

	OBJHANDLE targets[10];
	int numTargets;
	float targetCoord[10][2]; // new

	bool configScreen;
	CONFIGSELECT configSelection;

	PROJECTION proj;

	int gridAngleSeparation;
	int gridResolution;
	int skipEveryNLines;
	int skipEveryNLinesMax;
	bool autoResolution;
	int defaultMapLinesAmount;
	double orbitTrackAngleDelta; // new
	bool orbitTrackGround;
	double orbitTrackOrbitsNumber; // new
	bool displayElevationRadar; // new


	double centreLat, centreLong;
	int centreZoom;
	bool azimuthalEquidistantNortPole;
	TRACKMODE trackPosition;

	double shipHistory[GROUND_TRACK_HISTORY_SIZE][3]; // new
	int shipHistoryIndex; // new
	int shipHistoryLength; // new

	bool showVessels;
} MapMFDState;

// Create pens and brushes
oapi::Pen* coastLines;
oapi::Pen* contourLines;
oapi::Pen* mainOrbitTrack;
oapi::Pen* mainOrbitTrackHistory;
oapi::Pen* mainPosition;
oapi::Pen* targetOrbitTrack;
oapi::Pen* targetPosition;
oapi::Pen* gridLines;
oapi::Pen* baseBox;
oapi::Brush* sunlitSide;
oapi::Pen* terminatorLine;
oapi::Pen* groundCoverageLine;
oapi::Pen* targetGroundCoverageLine;
oapi::Brush* sunIcon;

// Text pens and fonts
DWORD configTextColour = 0xFFFFFF;
DWORD inactiveConfigTextColour = 0x707070;
oapi::Font* defaultFont;
oapi::Font* configFont;
oapi::Font* mapObjectFont;

int g_MFDmode; // identifier for new MFD mode
//HINSTANCE hInst;

// ==============================================================
// MFD class implementation

// Constructor
MapMFD::MapMFD(DWORD w, DWORD h, VESSEL* vessel)
	: MFD2(w, h, vessel)
{
	SetNewReference(vessel->GetSurfaceRef());

	char cbuf[30], cbuff[30];
	oapiGetObjectName(ref, cbuff, 30);
	FormatValue(cbuf, 30, refMu);
	oapiWriteLogV("Debug, ref: %s, mu: %s, rad: %.2f km", cbuff, cbuf + 1, refRad / 1e3);

	v = vessel;

	W = int(w);
	H = int(h);

	textDX = W / 80;
	textDY = H / 20;

	defaultFont = oapiCreateFont(H / 30, false, "Fixed", FONT_NORMAL, 0);
	configFont = oapiCreateFont(H / 25, true, "Sans", FONT_NORMAL, 0);
	mapObjectFont = oapiCreateFont(H / 50, true, "Sans", FONT_NORMAL, 0);

	coastLines = oapiCreatePen(1, 1, 0xFF8080);
	contourLines = oapiCreatePen(1, 1, 0x0070C0);
	mainOrbitTrack = oapiCreatePen(1, 1, 0x00FF00);
	mainOrbitTrackHistory = oapiCreatePen(1, 1, 0x337F33);
	mainPosition = oapiCreatePen(1, 3, 0x00FF00);
	targetOrbitTrack = oapiCreatePen(1, 1, 0x00FFFF);
	targetPosition = oapiCreatePen(1, 3, 0x00FFFF);
	gridLines = oapiCreatePen(1, 1, 0x505050);
	baseBox = oapiCreatePen(1, 1, 0xF0F0F0);
	sunlitSide = oapiCreateBrush(0x303030);
	//terminatorLine = oapiCreatePen(1, 1, 0xC0C0C0);
	terminatorLine = oapiCreatePen(1, 1, 0x13B8FD);
	groundCoverageLine = oapiCreatePen(1, 1, 0x40A040);
	targetGroundCoverageLine = oapiCreatePen(1, 1, 0x00A0A0);
	sunIcon = oapiCreateBrush(0x13B8FD);
}

// Destructor
MapMFD::~MapMFD()
{
	oapiReleaseFont(configFont);
	oapiReleaseFont(mapObjectFont);

	// Release pens
	oapiReleasePen(coastLines);
	oapiReleasePen(contourLines);
	oapiReleasePen(mainOrbitTrack);
	oapiReleasePen(mainOrbitTrackHistory);
	oapiReleasePen(mainPosition);
	oapiReleasePen(targetOrbitTrack);
	oapiReleasePen(targetPosition);
	oapiReleasePen(gridLines);
	oapiReleasePen(baseBox);
	oapiReleaseBrush(sunlitSide);
	oapiReleasePen(terminatorLine);
	oapiReleasePen(groundCoverageLine);
	oapiReleasePen(targetGroundCoverageLine);
	oapiReleaseBrush(sunIcon);

	oapiWriteLog("MapMFD2 Destructing");
}


// Return button labels
const int NUMBER_BUTTONS_DEFAULT = 12;
const int NUMBER_BUTTONS_CONFIG = 5;
const int NUMBER_BUTTONS_REFERENCE = 6;
const int NUMBER_BUTTONS_TARGET = 6;
char* MapMFD::ButtonLabel(int bt)
{
	// The labels for the two buttons used by our MFD mode
	static char* label[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", "<", ">", "TG-", "PRJ" };
	static char* labelTrkLon[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", " ", " ", "TG-", "PRJ" };
	static char* labelTrkLonLat[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", " ", " ", " ", " ", "TG-", "PRJ" };
	//static char* labelSecondary[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "PG", " ", " ", "CFG", "CLH", "CLT", "PRJ" };

	static char* labelConfig[NUMBER_BUTTONS_CONFIG] = { "UP", "DN", "MOD", "OK", "DEF" };

	static char* labelReference[NUMBER_BUTTONS_REFERENCE] = { "UP", "DN", "MOD", "OK", "DEF", "TXT" };
	static char* labelTarget[NUMBER_BUTTONS_TARGET] = { "UP", "DN", "MOD", "SEL", "BCK", "TXT" };

	if (configScreen)
	{
		return (bt < NUMBER_BUTTONS_CONFIG ? labelConfig[bt] : 0);
	}
	else if (referenceListScreen)
	{
		return (bt < NUMBER_BUTTONS_REFERENCE ? labelReference[bt] : 0);
	}
	else if (targetListScreen)
	{
		return (bt < NUMBER_BUTTONS_TARGET ? labelTarget[bt] : 0);
	}
	else
	{
		switch (trackPosition)
		{
		case LONGTRACK:
			return (bt < NUMBER_BUTTONS_DEFAULT ? labelTrkLon[bt] : 0);
		case LATLONGTRACK:
			return (bt < NUMBER_BUTTONS_DEFAULT ? labelTrkLonLat[bt] : 0);
		case NOTRACK: // do the same as default
		default:
			return (bt < NUMBER_BUTTONS_DEFAULT ? label[bt] : 0);
		}
	}
}

// Return button menus
int MapMFD::ButtonMenu(const MFDBUTTONMENU** menu) const
{
	// The menu descriptions for the buttons in main view
	static const MFDBUTTONMENU defaultMnu[NUMBER_BUTTONS_DEFAULT] = {
		{"Map reference", 0, 'R'},
		{"Select target", 0, 'T'},
		{"Zoom out", 0, 'X'},
		{"Zoom in", 0, 'Z'},
		{"Track mode on/off", 0, 'K'},
		{"Config screen", 0, 'C'},
		{"Scroll up", 0, '-'},
		{"Scroll down", 0, '='},
		{"Scroll left", 0, '['},
		{"Scroll right", 0, ']'},
		{"Remove last target", 0, 'M'},
		{"Toggle projection", 0, 'P'}
	};

	// Main view, tracking longitude
	static const MFDBUTTONMENU defaultMnuTrkLon[NUMBER_BUTTONS_DEFAULT] = {
		{"Map reference", 0, 'R'},
		{"Select target", 0, 'T'},
		{"Zoom out", 0, 'X'},
		{"Zoom in", 0, 'Z'},
		{"Track mode on/off", 0, 'K'},
		{"Config screen", 0, 'C'},
		{"Scroll up", 0, '-'},
		{"Scroll down", 0, '='},
		{" ", 0, ' '},
		{" ", 0, ' '},
		{"Remove last target", 0, 'M'},
		{"Toggle projection", 0, 'P'}
	};

	// Main view, tracking both longitude and latitude
	static const MFDBUTTONMENU defaultMnuTrkLonLat[NUMBER_BUTTONS_DEFAULT] = {
		{"Map reference", 0, 'R'},
		{"Select target", 0, 'T'},
		{"Zoom out", 0, 'X'},
		{"Zoom in", 0, 'Z'},
		{"Track mode on/off", 0, 'K'},
		{"Config screen", 0, 'C'},
		{" ", 0, ' '},
		{" ", 0, ' '},
		{" ", 0, ' '},
		{" ", 0, ' '},
		{"Remove last target", 0, 'M'},
		{"Toggle projection", 0, 'P'}
	};

	//// Secondary buttons
	//static const MFDBUTTONMENU defaultMnuSecondary[NUMBER_BUTTONS_DEFAULT] = {
	//	{"Map reference", 0, 'R'},
	//	{"Select target", 0, 'T'},
	//	{"Zoom out", 0, 'X'},
	//	{"Zoom in", 0, 'Z'},
	//	{"Track mode on/off", 0, 'K'},
	//	{"Primary buttons", 0, 'D'},
	//	{"Reference list screen", 0, '1'},
	//	{"Target list screen", 0, '2'},
	//	{"Config screen", 0, 'C'},
	//	{"Clear history track", 0, '3'},
	//	{"Clear all targets", 0, '4'},
	//	{"Toggle projection", 0, 'P'}
	//};

	// The menu descriptions in config view
	static const MFDBUTTONMENU configMnu[NUMBER_BUTTONS_CONFIG] = {
		{"Move selection up", 0, '-'},
		{"Move selection down", 0, '='},
		{"Modify selected parameter", 0, 'M'},
		{"Return to map", 0, 'O'},
		{"Set to default value", 0, 'D'}
	};

	// The menu descriptions in refList view
	static const MFDBUTTONMENU refMnu[NUMBER_BUTTONS_REFERENCE] = {
		{"Move selection up", 0, '-'},
		{"Move selection down", 0, '='},
		{"Modify selected parameter", 0, 'M'},
		{"Select and return", 0, 'O'},
		{"Set to default value", 0, 'D'},
		{"Text input", 0, 'T'}
	};

	// The menu descriptions in refList view
	static const MFDBUTTONMENU tgtMnu[NUMBER_BUTTONS_TARGET] = {
		{"Move selection up", 0, '-'},
		{"Move selection down", 0, '='},
		{"Modify selected parameter", 0, 'M'},
		{"Select", 0, 'S'},
		{"Return to map", 0, 'O'},
		{"Text input", 0, 'T'}
	};

	if (menu)
	{
		if (configScreen)
		{
			*menu = configMnu;
			return NUMBER_BUTTONS_CONFIG;
		}
		else if (referenceListScreen)
		{
			*menu = refMnu;
			return NUMBER_BUTTONS_REFERENCE;
		}
		else if (targetListScreen)
		{
			*menu = tgtMnu;
			return NUMBER_BUTTONS_TARGET;
		}
		else
		{
			switch (trackPosition)
			{
			case LONGTRACK:
				*menu = defaultMnuTrkLon;
				break;
			case LATLONGTRACK:
				*menu = defaultMnuTrkLonLat;
				break;
			case NOTRACK: // do the same as default
			default:
				*menu = defaultMnu;
				break;
			}

			return NUMBER_BUTTONS_DEFAULT; // all tracking modes all have same ammount of keys
		}
	}
}

// MFD message parser
int MapMFD::MsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and
		// return a pointer to it.
		return (int)(new MapMFD(LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam));
	}
	return 0;
}

// ==============================================================
// API interface

DLLCLBK void InitModule(HINSTANCE hDLL)
{
	static char* name = "Map2";   // MFD mode name
	MFDMODESPECEX spec;
	spec.name = name;
	spec.key = OAPI_KEY_M;                // MFD mode selection key
	spec.context = NULL;
	spec.msgproc = MapMFD::MsgProc;  // MFD mode callback function

	// Register the new MFD mode with Orbiter
	g_MFDmode = oapiRegisterMFDMode(spec);

	FILEHANDLE cfgFile = oapiOpenFile("MFD\\MapMFD2.cfg", FILE_IN, CONFIG);

	oapiReadItem_int(cfgFile, "DefTrackMode", DEFAULT_VALUES.TRACK_MODE);
	oapiReadItem_int(cfgFile, "DefProjection", DEFAULT_VALUES.PROJECTION);
	oapiReadItem_int(cfgFile, "DefGridAngleSep", DEFAULT_VALUES.GRID_ANGLE_SEP);
	oapiReadItem_int(cfgFile, "DefGridResolution", DEFAULT_VALUES.GRID_RESOLUTION);
	oapiReadItem_int(cfgFile, "DefDefaultLinesAmount", DEFAULT_VALUES.LINES_AMOUNT);
	oapiReadItem_float(cfgFile, "DefGroundTrackStep", DEFAULT_VALUES.GROUNDTRACK_STEP);
	oapiReadItem_float(cfgFile, "DefTrackOrbitsNumber", DEFAULT_VALUES.GROUNDTRACK_ORBITS);
	oapiReadItem_int(cfgFile, "DefViewCircleResolution", DEFAULT_VALUES.VIEW_CIRCLE_RESOLUTION);
	oapiReadItem_bool(cfgFile, "DefShowAltitudeRadar", DEFAULT_VALUES.ELEVATION_RADAR);
	oapiReadItem_bool(cfgFile, "DefShowOtherVessels", DEFAULT_VALUES.SHOW_VESSELS);

	oapiCloseFile(cfgFile, FILE_IN);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode(g_MFDmode);

	resetCommand = true; // tell MFD that we have locked Orbiter, and don't want to recall from RecallStatus
	oapiWriteLog("MapMFD2 Exiting");
}

// Other

void MapMFD::myStrncpy(char* writeTo, const char* readFrom, int len)
{
	// Because strncpy sucks, I have to make my own >:(
	int i = 0;
	while (i < len && readFrom[i] != '\0')
	{
		writeTo[i] = readFrom[i];
		i++;
	}
	writeTo[i] = '\0';
}