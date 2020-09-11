#pragma once
#include <algorithm>

/* 
Header file for MapMFD2 addon for Orbiter Space Flight Simulator 2016.
Addon by Asbj�rn 'asbjos' Kr�ger, 2020.

This source code is released under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
For other use, please contact me (I'm username 'asbjos' on Orbiter-Forum).
*/

enum PROJECTION { EQUIRECTANGULAR, MILLER, MERCATOR, TRANSVERSEMERCATOR, EQUALEARTH, MOLLWEIDE, ORTELIUSOVAL, WINKELTRIPEL, RECTANGULARPOLYCONIC, AZIMUTHALEQUIDISTANT, LAMBERTAZIMUTHAL, CASSINI, LASTENTRYPROJECTION };
enum MAPFEATURE { BOX, CROSS, RINGS };
enum CONFIGSELECT { CONFIGTRACKMODE, CONFIGRADAR, CONFIGSHOWVESSELS, CONFIGDRAWSPECIFICALT, CONFIGSHOWHISTORY, CONFIGPROJECTION, CONFIGFLIPPOLE, CONFIGRESETMAP, CONFIGGRIDSEP, CONFIGGRIDRES, CONFIGMAPRES, CONFIGMAPAUTOSIZE, CONFIGTRACKANGLEDELTA, CONFIGTRACKNUMORBITS, CONFIGPLANETVIEWSEGMENTS, CONFIGRESETALL, CONFIGDEBUGINFO, LASTENTRYCONFIG };
enum TRACKMODE { NOTRACK, LONGTRACK, LATLONGTRACK, LASTENTRYTRACK };
enum TARGETEXPANDMODES { EXPANDSPACEPORTS, EXPANDSPACECRAFT, EXPANDMOONS, EXPANDNONE = -1}; // set EXPANDNONE to -1, so that we can do a >= 0 check to see if something is expanded

const int GROUND_TRACK_ITERATION_MAX_STEPS = int(2e4); // 2e4 is arbitrary max limit of what I'll allow. If you want a longer plot, there's no problem to increase time delta in config
const int GROUND_TRACK_HISTORY_SIZE = int(1e3); // Number of datapoints recorded to position history.
const int MAX_ZOOM = 256; // should be power of 2
static char MFD_NAME[10] = "Map2";

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
	bool SHOW_HISTORY = true;
	double DRAW_SPECIFIC_ALT = 0.0; // off
	int BLOCK_PROJECTIONS[int(LASTENTRYPROJECTION)]; // list of projection indices to block, initialise to block none (index -1 does not exist).
} DEFAULT_VALUES;

// Initialise projection list

struct
{
	DWORD COAST = 0xFF8080;
	DWORD CONTOUR = 0x0070C0;
	DWORD MAINTRACK = 0x00FF00;
	DWORD MAINTRACKHISTORY = 0x337F33;
	DWORD TARGETTRACK = 0x00FFFF;
	DWORD GRID = 0x505050;
	DWORD BASE = 0xF0F0F0;
	DWORD SUNFILL = 0x303030;
	DWORD TERMINATOR = 0xC0C0C0;
	DWORD SUNICON = 0x13B8FD;
	DWORD MAINVIEW = 0x40A040;
	DWORD TARGETVIEW = 0x00A0A0;
} DEFAULT_COLOURS;

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

	// Map screen functions. Used to clean up the MapScreen function.
	void MakeSunLight(oapi::Sketchpad* skp);
	void MakeGridLines(oapi::Sketchpad* skp);
	void MakeMap(oapi::Sketchpad* skp, const char* refName, int* txtPos);
	void MakeSurfaceBasesAndVessels(oapi::Sketchpad* skp);
	void MakeTargets(oapi::Sketchpad* skp, double currentLong, double currentLat, int *infoLinesDrwn);
	void MakeShip(oapi::Sketchpad* skp, double currentLong, double currentLat, double currentRad, int* infoLinesDrwn);

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

	bool GetEquPosInXSeconds(double t, ELEMENTS el, ORBITPARAM prm, double currentLongitude, double* longitude, double* latitude); // return false if altitude below surface
	void GetObjectEquPos(OBJHANDLE tgt, double* longitude, double* latitude, double *radius);
	void GetObjectRelativeElements(OBJHANDLE tgt, ELEMENTS& el, ORBITPARAM* prm);

	// Key input functions
	bool SetReferencePlanet(char* rstr);
	bool SetTargetObject(char* rstr);
	bool SetGridSeparation(char* rstr);
	bool SetGridResolution(char* rstr);
	bool SetNumberOrbitsDisplayed(char* rstr);
	bool SetSpecificAltitudeSelect(char* rstr);

	// Some basic mathematical functions
	double MnA2TrA(double MnA, double Ecc);
	double TrA2MnA(double TrA, double Ecc);
	double EccentricAnomaly(double ecc, double TrA);
	VECTOR3 Ecl2Equ(VECTOR3 Ecl);
	VECTOR3 Coord2Vector(double longitude, double latitude);

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
	int currentTopListed = 0;

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
	bool orbitTrackGround = bool(DEFAULT_VALUES.TRACK_MODE); // ground track mode. If false, set to orbit track mode.
	int viewCircleResolution = DEFAULT_VALUES.VIEW_CIRCLE_RESOLUTION; // number of segments in the view circle ("what is visible").
	double orbitTrackOrbitsNumber = DEFAULT_VALUES.GROUNDTRACK_ORBITS; // number of orbits to show of ground track (limited by max 1e6 seconds).
	bool displayElevationRadar = DEFAULT_VALUES.ELEVATION_RADAR; // wether to show elevation radar or not. It can be FPS hungry.
	double drawSpecificAlt = DEFAULT_VALUES.DRAW_SPECIFIC_ALT; // if not 0.0, you can set a specific altitude to show where in orbit you are at that altitude. Uses same format as "Ap" and "Pe" drawing.

	// Debug
	int pointsOutside = 0;
	int pointsInside = 0;

	double centreLat = 0.0, centreLong = 0.0;
	int centreZoom = 1; // no floating point errors, please
	bool azimuthalEquidistantNortPole = true;
	TRACKMODE trackPosition = NOTRACK;

	bool showVessels = DEFAULT_VALUES.SHOW_VESSELS;
	bool showHistory = DEFAULT_VALUES.SHOW_HISTORY;

	float cacheMap[100000][2];
	bool updateCache = true; // never change this other than when caching or changing reference. If memory is lost, the update call is also reactivated.
	bool coastMap, mapExists;

	double shipHistory[GROUND_TRACK_HISTORY_SIZE][3]; // record ship coordinates, and last is time stamp
	int shipHistoryIndex = 0; // index to record to in shipHistory
	int shipHistoryLength = 0; // history record length

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
	oapi::Brush* black; // used to plot darkness on sunlit side, always black
	oapi::Pen* terminatorLine;
	oapi::Pen* groundCoverageLine;
	oapi::Pen* targetGroundCoverageLine;
	oapi::Brush* sunIcon;
	//oapi::Font* defaultFont;
	//oapi::Font* configFont;
	oapi::Font* mapObjectFont;
};

static bool resetCommand = false; // inform user and RecallStatus that we want to reset
static bool debugInformation = false;

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
	double drawSpecificAlt;

	double centreLat, centreLong;
	int centreZoom;
	bool azimuthalEquidistantNortPole;
	TRACKMODE trackPosition;

	double shipHistory[GROUND_TRACK_HISTORY_SIZE][3]; // new
	int shipHistoryIndex; // new
	int shipHistoryLength; // new

	bool showVessels;
	bool showHistory;
} MapMFDState;

// Text pens and fonts
static DWORD configTextColour = 0xFFFFFF;
static DWORD inactiveConfigTextColour = 0x707070;

int g_MFDmode; // identifier for new MFD mode

// ==============================================================
// MFD class implementation

// Constructor
MapMFD::MapMFD(DWORD w, DWORD h, VESSEL* vessel)
	: MFD2(w, h, vessel)
{
	SetNewReference(vessel->GetSurfaceRef());

	v = vessel;

	W = int(w);
	H = int(h);

	textDX = W / 80;
	textDY = H / 20;

	mapObjectFont = oapiCreateFont(12, true, "Sans", FONT_NORMAL, 0); // set forced to 10, so that text is also visible on small MFD sizes

	coastLines = oapiCreatePen(1, 1, DEFAULT_COLOURS.COAST);
	contourLines = oapiCreatePen(1, 1, DEFAULT_COLOURS.CONTOUR);
	mainOrbitTrack = oapiCreatePen(1, 1, DEFAULT_COLOURS.MAINTRACK);
	mainOrbitTrackHistory = oapiCreatePen(1, 1, DEFAULT_COLOURS.MAINTRACKHISTORY);
	mainPosition = oapiCreatePen(1, 3, DEFAULT_COLOURS.MAINTRACK);
	targetOrbitTrack = oapiCreatePen(1, 1, DEFAULT_COLOURS.TARGETTRACK);
	targetPosition = oapiCreatePen(1, 3, DEFAULT_COLOURS.TARGETTRACK);
	gridLines = oapiCreatePen(1, 1, DEFAULT_COLOURS.GRID);
	baseBox = oapiCreatePen(1, 1, DEFAULT_COLOURS.BASE);
	sunlitSide = oapiCreateBrush(DEFAULT_COLOURS.SUNFILL);
	black = oapiCreateBrush(0x000000);
	terminatorLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.TERMINATOR);
	groundCoverageLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.MAINVIEW);
	targetGroundCoverageLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.TARGETVIEW);
	sunIcon = oapiCreateBrush(DEFAULT_COLOURS.SUNICON);
}

// Destructor
MapMFD::~MapMFD()
{
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
	oapiReleaseBrush(black);
	oapiReleasePen(terminatorLine);
	oapiReleasePen(groundCoverageLine);
	oapiReleasePen(targetGroundCoverageLine);
	oapiReleaseBrush(sunIcon);
}


// Return button labels
const int NUMBER_BUTTONS_DEFAULT = 12;
const int NUMBER_BUTTONS_CONFIG = 5;
const int NUMBER_BUTTONS_REFERENCE = 6;
const int NUMBER_BUTTONS_TARGET = 7;
char* MapMFD::ButtonLabel(int bt)
{
	// The labels for the two buttons used by our MFD mode
	static char* label[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", "<", ">", "TG-", "PRJ" };
	static char* labelTrkLon[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", " ", " ", "TG-", "PRJ" };
	static char* labelTrkLonLat[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", " ", " ", " ", " ", "TG-", "PRJ" };

	static char* labelConfig[NUMBER_BUTTONS_CONFIG] = { "UP", "DN", "MOD", "OK", "DEF" };

	static char* labelReference[NUMBER_BUTTONS_REFERENCE] = { "UP", "DN", "MOD", "OK", "DEF", "TXT" };
	static char* labelTarget[NUMBER_BUTTONS_TARGET] = { "UP", "DN", "MOD", "SEL", "BCK", "TXT", "NEA" };

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
		{"Text input", 0, 'T'},
		{"Nearest object", 0, 'N'}
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

	return 0; // just in case we haven't returned until now
}

// MFD message parser
int MapMFD::MsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and return a pointer to it.
		return (int)(new MapMFD(LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam));
	}
	return 0;
}

// ==============================================================
// API interface

void ReadUserColour(FILEHANDLE cfgFile, char *label, DWORD *colour)
{
	char colourString[10];
	bool couldRead = oapiReadItem_string(cfgFile, label, colourString);
	if (couldRead && strlen(colourString) == 6)
	{
		*colour = strtoul(colourString, NULL, 16);
	}
	else
	{
		oapiWriteLogV("MapMFD2 ERROR! Could not find/understand %s colour in config file!", label);
	}
}

char* GetSpecificProjectionName(int proj)
{
	// Write projection
	switch (proj)
	{
	case EQUIRECTANGULAR:
		return "Equirectangular";
	case MILLER:
		return "Miller";
	case MERCATOR:
		return "Mercator";
	case TRANSVERSEMERCATOR:
		return "Transverse Mercator";
	case EQUALEARTH:
		return "Equal Earth";
	case MOLLWEIDE:
		return "Mollweide";
	case ORTELIUSOVAL:
		return "Ortelius Oval";
	case WINKELTRIPEL:
		return "Winkel Tripel";
	case RECTANGULARPOLYCONIC:
		return "Rectangular Polyconic";
	case AZIMUTHALEQUIDISTANT:
		return "Azimuthal Equidistant";
	case LAMBERTAZIMUTHAL:
		return "Lambert Azimuthal Equal-area";
	case CASSINI:
		return "Cassini";
	default:
		return "ERROR";
	}
	return "ERROR";
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
	MFDMODESPECEX spec;
	spec.name = MFD_NAME;
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
	oapiReadItem_bool(cfgFile, "DefShowHistoryTrack", DEFAULT_VALUES.SHOW_HISTORY);
	oapiReadItem_float(cfgFile, "DefDrawSpecificAltitude", DEFAULT_VALUES.DRAW_SPECIFIC_ALT);

	// Logic for disabling specific projections
	char blockProjections[200];
	oapiReadItem_string(cfgFile, "DisableSpecificProjections", blockProjections);
	std::fill_n(DEFAULT_VALUES.BLOCK_PROJECTIONS, int(LASTENTRYPROJECTION), -1); // initalise array to -1 (don't block any)
	char* comma = blockProjections;
	int commaNr = 0;
	while (comma != NULL && commaNr < int(LASTENTRYPROJECTION))
	{
		int blockedProjection = atoi(comma); // find first coming integer

		if (blockedProjection >= 0 && blockedProjection < int(LASTENTRYPROJECTION)) // if valid index
		{
			DEFAULT_VALUES.BLOCK_PROJECTIONS[commaNr] = blockedProjection; // add to array of block list
			oapiWriteLogV("MapMFD2: disabled %s (%i) projection", GetSpecificProjectionName(blockedProjection), blockedProjection); // tell user which projection is blocked
		}

		commaNr++; // update array index

		char* nextComma = strchr(comma + 1, ',');
		if (nextComma != NULL) comma = nextComma + 1;
		else comma = NULL;
	}

	// Get colours
	ReadUserColour(cfgFile, "ColourCoast", &DEFAULT_COLOURS.COAST);
	ReadUserColour(cfgFile, "ColourContour", &DEFAULT_COLOURS.CONTOUR);
	ReadUserColour(cfgFile, "ColourMainTrack", &DEFAULT_COLOURS.MAINTRACK);
	ReadUserColour(cfgFile, "ColourMainTrackHistory", &DEFAULT_COLOURS.MAINTRACKHISTORY);
	ReadUserColour(cfgFile, "ColourTargetTrack", &DEFAULT_COLOURS.TARGETTRACK);
	ReadUserColour(cfgFile, "ColourGrid", &DEFAULT_COLOURS.GRID);
	ReadUserColour(cfgFile, "ColourBase", &DEFAULT_COLOURS.BASE);
	ReadUserColour(cfgFile, "ColourSunFill", &DEFAULT_COLOURS.SUNFILL);
	ReadUserColour(cfgFile, "ColourTerminator", &DEFAULT_COLOURS.TERMINATOR);
	ReadUserColour(cfgFile, "ColourSunIcon", &DEFAULT_COLOURS.SUNICON);
	ReadUserColour(cfgFile, "ColourMainView", &DEFAULT_COLOURS.MAINVIEW);
	ReadUserColour(cfgFile, "ColourTargetView", &DEFAULT_COLOURS.TARGETVIEW);

	oapiCloseFile(cfgFile, FILE_IN);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode(g_MFDmode);

	resetCommand = true; // tell MFD that we have locked Orbiter, and don't want to recall from RecallStatus
}

// Other

// Because strncpy sucks, I have to make my own >:(
void MapMFD::myStrncpy(char* writeTo, const char* readFrom, int len)
{
	int i = 0;
	while (i < len && readFrom[i] != '\0')
	{
		writeTo[i] = readFrom[i];
		i++;
	}
	writeTo[i] = '\0';
}