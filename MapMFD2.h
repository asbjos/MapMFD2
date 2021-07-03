#pragma once
//#include <algorithm>

/* 
Header file for MapMFD2 addon for Orbiter Space Flight Simulator 2016.
Addon by Asbjørn 'asbjos' Krüger, 2020.

This source code is released under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
For other use, please contact me (I'm username 'asbjos' on Orbiter-Forum).
*/
// NEW: VANDERGRINTEN, HAMMER, GALLPETERS, HOBODYER, STEREOGRAPHIC, GNOMONIC, BERGHAUSSTAR, LASKOWSKI, LOXIMUTHAL, ROBINSON.
//enum PROJECTION { EQUIRECTANGULAR, MILLER, MERCATOR, VANDERGRINTEN, TRANSVERSEMERCATOR, ROBINSON, EQUALEARTH, MOLLWEIDE, AITOFF, HAMMER, LOXIMUTHAL, LASKOWSKI, ORTELIUSOVAL, WINKELTRIPEL, RECTANGULARPOLYCONIC, AZIMUTHALEQUIDISTANT, LAMBERTAZIMUTHAL, STEREOGRAPHIC, GNOMONIC, BERGHAUSSTAR, CASSINI, GALLPETERS, HOBODYER, LASTENTRYPROJECTION };
enum PROJECTION { EQUIRECTANGULAR, MERCATOR, VANDERGRINTEN, TRANSVERSEMERCATOR, ROBINSON, EQUALEARTH, MOLLWEIDE, HAMMER, LOXIMUTHAL, LASKOWSKI, WINKELTRIPEL, AZIMUTHALEQUIDISTANT, LAMBERTAZIMUTHAL, STEREOGRAPHIC, GALLPETERS, LASTENTRYPROJECTION };
enum MAPFEATURE { BOX, CROSS, RINGS };
enum CONFIGSELECT { CONFIGTRACKMODE, CONFIGRADAR, CONFIGSHOWVESSELS, CONFIGDRAWSPECIFICALT, CONFIGSHOWHISTORY, CONFIGPROJECTION/*, CONFIGFLIPPOLE*/, CONFIGRESETMAP, CONFIGGRIDSEP, CONFIGGRIDRES, CONFIGMAPRES, CONFIGMAPAUTOSIZE, CONFIGNUMERICVSANALYTIC, CONFIGCONSIDERTERRAIN, CONFIGTRACKANGLEDELTA, CONFIGTRACKMAXPERIODFRAC, CONFIGTRACKNUMORBITS, CONFIGPLANETVIEWSEGMENTS, CONFIGMARKERS, CONFIGRESETALL, CONFIGDEBUGINFO, LASTENTRYCONFIG };
enum TRACKMODE { NOTRACK, LONGTRACK, LATLONGTRACK, LASTENTRYTRACK };
enum TARGETEXPANDMODES { EXPANDSPACEPORTS, EXPANDSPACECRAFT, EXPANDMOONS, EXPANDNONE = -1}; // set EXPANDNONE to -1, so that we can do a >= 0 check to see if something is expanded

const int GROUND_TRACK_ITERATION_MAX_STEPS = int(2e4); // 2e4 is arbitrary max limit of what I'll allow. If you want a longer plot, there's no problem to increase time delta in config
const int GROUND_TRACK_HISTORY_SIZE = int(1e3); // Number of datapoints recorded to position history.
const int MAX_ZOOM = 32768; // should be power of 2
static char MFD_NAME[10] = "Map2";
const int COLOUR_BIT_DEPTH = 256; // we have 256 shades from red to green. Declared as constant for better readability.
const int TOTAL_MARKERS_ALLOWED_TO_BE_ENABLED = 50;
const int TOTAL_MARKERS_PER_PLANET = 50;
const int MAX_MOONS_PER_BODY = 20; // the number of moons for a body in the ref/target list.

struct
{
	int TRACK_MODE = 1;
	int PROJECTION = 0;
	int GRID_ANGLE_SEP = 30;
	int GRID_RESOLUTION = 2;
	int LINES_AMOUNT = 5000;
	double GROUNDTRACK_STEP = 0.1;
	double GROUNDTRACK_MAXTIMEFRAC = 100.0;
	double GROUNDTRACK_ORBITS = 3.5;
	int VIEW_CIRCLE_RESOLUTION = 90;
	bool ELEVATION_RADAR = false;
	bool SHOW_VESSELS = false;
	bool SHOW_HISTORY = true;
	double DRAW_SPECIFIC_ALT = 0.0; // off
	bool GROUNDTRACK_NUMERIC = true;
	int BLOCK_PROJECTIONS[int(LASTENTRYPROJECTION)]; // list of projection indices to block, initialise to block none (index -1 does not exist).
	bool CONSIDER_TERRAIN_GROUNDTRACK = false;
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
	DWORD MARKER[6] = { 0x00FFFF, 0xFFFF00, 0x4040FF, 0xFF00FF, 0x40FF40, 0xFF8080 };
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
	OBJHANDLE moonHandle[MAX_MOONS_PER_BODY];
	int moonIndex[MAX_MOONS_PER_BODY];
	char moonName[MAX_MOONS_PER_BODY][20]; // 20 moons, 20 chars each
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
	bool ConsumeKeyImmediate(DWORD key, bool newPress); // Should normally have char *kstate as variable, but as this is only called in ConsumeButton, and is not run by Orbiter, I give myself the liberty to give similar function as KeyBuffered.
	void StoreStatus(void) const;
	void RecallStatus(void);
	void WriteStatus(FILEHANDLE scn) const;
	void ReadStatus(FILEHANDLE scn);

	void MapScreen(oapi::Sketchpad* skp);
	void ConfigScreen(oapi::Sketchpad* skp);
	void ReferenceListScreen(oapi::Sketchpad* skp);
	void TargetListScreen(oapi::Sketchpad* skp);
	void MarkerListScreen(oapi::Sketchpad* skp);

	// Map screen functions. Used to clean up the MapScreen function.
	void MakeSunLight(oapi::Sketchpad* skp);
	void MakeGridLines(oapi::Sketchpad* skp);
	void MakeMap(oapi::Sketchpad* skp, const char* refName, int* txtPos);
	void MakeMarkers(oapi::Sketchpad* skp, const char* refName);
	void MakeSurfaceBasesAndVessels(oapi::Sketchpad* skp);
	void MakeTargets(oapi::Sketchpad* skp, double currentLong, double currentLat, int *infoLinesDrwn);
	void MakeShip(oapi::Sketchpad* skp, double currentLong, double currentLat, double currentRad, int* infoLinesDrwn);

	void BuildReferenceCache(void);
	void SetNewReference(OBJHANDLE hRef);
	bool AddOrRemoveTarget(OBJHANDLE hRef);
	bool ObjectAlreadyInTarget(OBJHANDLE hRef);
	bool AddOrRemoveMarker(const char *fileName);

	bool DrawLine(double long0, double lat0, double long1, double lat1, oapi::Sketchpad* skp, bool safetyCheck = true);
	bool DrawFeature(double longitude, double latitude, int size, MAPFEATURE feature, oapi::Sketchpad* skp, char *label);
	void DrawOrbitTrackAnalytic(double currentLong, double currentLat, ELEMENTS el, ORBITPARAM prm, oapi::Sketchpad* skp);
	void DrawOrbitTrackNumeric(VECTOR3 statePos, VECTOR3 stateVel, ELEMENTS el, ORBITPARAM prm, oapi::Sketchpad* skp); // attempted improvement of above function.

	bool TransformPoint(double longitude, double latitude, double *transformedLongitude, double *transformedLatitude, PROJECTION projection);
	//char* GetProjectionName(void);
	char* GetCoordinateString(double longitude, double latitude);

	bool GetEquPosInXSecondsAnalytical(double t, ELEMENTS el, ORBITPARAM prm, double* longitude, double* latitude); // return false if altitude below surface
	//bool GetEquPosInXSeconds2(double t, ELEMENTS el, ORBITPARAM prm, double currentLongitude, double* longitude, double* latitude, int perturbation); // attempted improvement of above function.
	void GetNextStateVector(double dt, VECTOR3 pos0, VECTOR3 vel0, VECTOR3* pos, VECTOR3* vel);
	VECTOR3 GravitationalAcceleration(VECTOR3 r);
	void GetObjectEquPos(OBJHANDLE tgt, double* longitude, double* latitude, double *radius);
	void GetObjectRelativeElements(OBJHANDLE tgt, ELEMENTS& el, ORBITPARAM* prm, VECTOR3 *pos, VECTOR3 *vel);

	// Key input functions
	bool SetReferencePlanet(char* rstr);
	bool SetTargetObject(char* rstr);
	bool SetGridSeparation(char* rstr);
	bool SetGridResolution(char* rstr);
	bool SetMaxPeriodFraction(char* rstr);
	bool SetNumberOrbitsDisplayed(char* rstr);
	bool SetSpecificAltitudeSelect(char* rstr);

	// Some basic mathematical functions
	double MnA2TrA(double MnA, double Ecc);
	double TrA2MnA(double TrA, double Ecc);
	double EccentricAnomaly(double ecc, double TrA);
	VECTOR3 Ecl2Equ(VECTOR3 Ecl);
	VECTOR3 Coord2Vector(double longitude, double latitude);
	double AngleSameTurn(double inputAngle, const double refAngle);

	//void myStrncpy(char* writeTo, const char* readFrom, int len);

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
	bool markerListScreen = false;
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

	int markerSelection = 0;
	int currentMarkerTopListed = 0;

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
	double maxPeriodFraction = DEFAULT_VALUES.GROUNDTRACK_MAXTIMEFRAC; // TrA angle max time step, related to reference rotation period.
	bool orbitTrackGround = bool(DEFAULT_VALUES.TRACK_MODE); // ground track mode. If false, set to orbit track mode.
	int viewCircleResolution = DEFAULT_VALUES.VIEW_CIRCLE_RESOLUTION; // number of segments in the view circle ("what is visible").
	double orbitTrackOrbitsNumber = DEFAULT_VALUES.GROUNDTRACK_ORBITS; // number of orbits to show of ground track (limited by max 1e6 seconds).
	bool displayElevationRadar = DEFAULT_VALUES.ELEVATION_RADAR; // wether to show elevation radar or not. It can be FPS hungry.
	double drawSpecificAlt = DEFAULT_VALUES.DRAW_SPECIFIC_ALT; // if not 0.0, you can set a specific altitude to show where in orbit you are at that altitude. Uses same format as "Ap" and "Pe" drawing.
	bool groundtrackUseTerrain = DEFAULT_VALUES.CONSIDER_TERRAIN_GROUNDTRACK; // use terrain for impact prediction or not.

	// Debug
	int pointsOutside = 0;
	int pointsInside = 0;

	double centreLat = 0.0, centreLong = 0.0;
	int centreZoom = 1; // no floating point errors, please
	//bool azimuthalEquidistantNortPole = true;
	TRACKMODE trackPosition = NOTRACK;

	bool showVessels = DEFAULT_VALUES.SHOW_VESSELS;
	bool showHistory = DEFAULT_VALUES.SHOW_HISTORY;

	bool groundtrackNumeric = DEFAULT_VALUES.GROUNDTRACK_NUMERIC; // allow user to switch between numeric and analytic ground track calculation. \
	Analytic breaks down for eccentricity close to 0.0 or close to 1.0. \
	Analytic lacks J2 non-spherical term implementation, although the J2 in numeric still doesn't perfectly predict even in LEO. \
	Ap/Pe and altitude points are calculated analytically, so especially with non-spheircal gravity enabled, the points may not line up with ground track if in numeric mode. \
	Performance should be roughly similar. Maybe numeric is slightly faster, due to less trigonometric calculation.

	float cacheMap[100000][2];
	bool updateCache = true; // never change this other than when caching or changing reference. If memory is lost, the update call is also reactivated.
	bool coastMap, mapExists;

	double immediateKeyStart = 0.0; // the sysTime for start holding a pan key (centreLat/centreLong adjustment). Note that sysTime is always positive.

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
	oapi::Pen* markerPen[6]; // 6 different pen colours (set by ColourIdx 0-5 in marker file)
	oapi::Brush* sunlitSide;
	oapi::Brush* black; // used to plot darkness on sunlit side, always black
	oapi::Pen* terminatorLine;
	oapi::Pen* groundCoverageLine;
	oapi::Pen* targetGroundCoverageLine;
	oapi::Brush* sunIcon;
	//oapi::Font* defaultFont;
	//oapi::Font* configFont;
	oapi::Font* mapObjectFont;
	oapi::Pen* gradientRedGreen[COLOUR_BIT_DEPTH];

	// Markers
	int totalMarkersInReference = 0;
};

// Outside, so that we can populate in config reading.
char enabledMarkers[TOTAL_MARKERS_ALLOWED_TO_BE_ENABLED][300]; // at most 50 marker files enabeled. As of now, Orbiter has 7 for Earth and 6 for Moon, so I don't think more should be necessary. But Venus has 23! Obsobs.
int numMarkers = 0;

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
	double maxPeriodFraction; // NEW
	bool orbitTrackGround;
	double orbitTrackOrbitsNumber; // new
	bool displayElevationRadar; // new
	double drawSpecificAlt;
	bool groundtrackNumeric; // newnewnew
	bool groundtrackUseTerrain; // newnewnew

	double centreLat, centreLong;
	int centreZoom;
	//bool azimuthalEquidistantNortPole;
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
	for (int i = 0; i < 6; i++)
	{
		markerPen[i] = oapiCreatePen(1, 1, DEFAULT_COLOURS.MARKER[i]);
	}
	sunlitSide = oapiCreateBrush(DEFAULT_COLOURS.SUNFILL);
	black = oapiCreateBrush(0x000000);
	terminatorLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.TERMINATOR);
	groundCoverageLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.MAINVIEW);
	targetGroundCoverageLine = oapiCreatePen(1, 1, DEFAULT_COLOURS.TARGETVIEW);
	sunIcon = oapiCreateBrush(DEFAULT_COLOURS.SUNICON);

	for (int i = 0; i < COLOUR_BIT_DEPTH; i++)
	{
		//gradientRedGreen[i] = oapziCreatePen(1, 5, oapiGetColour(0, int(sqrt(255 * 255 - i * i)), i)); // see this video for why I square and sqrt the colour: https://www.youtube.com/watch?v=LKnqECcg6Gw (TL;DW: brightness)
		gradientRedGreen[i] = oapiCreatePen(1, 5, oapiGetColour(0, 255 - i, i)); // not correcting for gamma, as the yellow and green are nearly indistinguishable. In this linear way, 0 altitude is "gray", while positive or negative altitude are brighter, standing more out than just in colour.
	}
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
	for (int i = 0; i < 6; i++) oapiReleasePen(markerPen[i]);
	oapiReleaseBrush(sunlitSide);
	oapiReleaseBrush(black);
	oapiReleasePen(terminatorLine);
	oapiReleasePen(groundCoverageLine);
	oapiReleasePen(targetGroundCoverageLine);
	oapiReleaseBrush(sunIcon);

	for (int i = 0; i < COLOUR_BIT_DEPTH; i++)
	{
		oapiReleasePen(gradientRedGreen[i]);
	}
}


// Return button labels
const int NUMBER_BUTTONS_DEFAULT = 12;
const int NUMBER_BUTTONS_CONFIG = 5;
const int NUMBER_BUTTONS_REFERENCE = 6;
const int NUMBER_BUTTONS_TARGET = 7;
const int NUMBER_BUTTONS_MARKER = 4;
char* MapMFD::ButtonLabel(int bt)
{
	// The labels for the two buttons used by our MFD mode
	static char* label[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", "<", ">", "TG-", "PRJ" };
	static char* labelTrkLon[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", "UP", "DN", " ", " ", "TG-", "PRJ" };
	static char* labelTrkLonLat[NUMBER_BUTTONS_DEFAULT] = { "REF", "TGT", "ZM-", "ZM+", "TRK", "CFG", " ", " ", " ", " ", "TG-", "PRJ" };

	static char* labelConfig[NUMBER_BUTTONS_CONFIG] = { "UP", "DN", "MOD", "OK", "DEF" };

	static char* labelReference[NUMBER_BUTTONS_REFERENCE] = { "UP", "DN", "MOD", "OK", "DEF", "TXT" };
	static char* labelTarget[NUMBER_BUTTONS_TARGET] = { "UP", "DN", "MOD", "SEL", "BCK", "TXT", "NEA" };
	static char* labelMarker[NUMBER_BUTTONS_MARKER] = { "UP", "DN", "MOD", "BCK" };

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
	else if (markerListScreen)
	{
		return (bt < NUMBER_BUTTONS_MARKER ? labelMarker[bt] : 0);
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

	// The menu descriptions in tgtList view
	static const MFDBUTTONMENU tgtMnu[NUMBER_BUTTONS_TARGET] = {
		{"Move selection up", 0, '-'},
		{"Move selection down", 0, '='},
		{"Modify selected parameter", 0, 'M'},
		{"Select", 0, 'S'},
		{"Return to map", 0, 'O'},
		{"Text input", 0, 'T'},
		{"Nearest object", 0, 'N'}
	};

	// The menu descriptions in mkrList view
	static const MFDBUTTONMENU mkrMnu[NUMBER_BUTTONS_MARKER] = {
		{"Move selection up", 0, '-'},
		{"Move selection down", 0, '='},
		{"Modify selected marker", 0, 'M'},
		{"Return to map", 0, 'O'}
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
		else if (markerListScreen)
		{
			*menu = mkrMnu;
			return NUMBER_BUTTONS_MARKER;
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

char* GetSpecificProjectionName(int proj)
{
	// Write projection
	switch (proj)
	{
	case EQUIRECTANGULAR:
		return "Equirectangular";
		//case MILLER:
			//return "Miller";
	case MERCATOR:
		return "Mercator";
	case VANDERGRINTEN:
		return "Van der Grinten";
	case TRANSVERSEMERCATOR:
		return "Transverse Mercator";
	case ROBINSON:
		return "Robinson";
	case EQUALEARTH:
		return "Equal Earth";
	case MOLLWEIDE:
		return "Mollweide";
		/*case OBLIQUEMOLLWEIDE:
			return "Oblique Mollweide";*/
	case HAMMER:
		return "Hammer";
	case LOXIMUTHAL:
		return "Loximuthal";
	case LASKOWSKI:
		return "Laskowski";
		//case ORTELIUSOVAL:
			//return "Ortelius Oval";
	case WINKELTRIPEL:
		return "Winkel Tripel";
		//case RECTANGULARPOLYCONIC:
			//return "Rectangular Polyconic";
	case AZIMUTHALEQUIDISTANT:
		return "Azimuthal Equidistant";
	case LAMBERTAZIMUTHAL:
		return "Lambert Azimuthal Equal-area";
	case STEREOGRAPHIC:
		return "Stereographic";
		//case GNOMONIC:
			//return "Gnomonic";
	case GALLPETERS:
		return "Gall-Peters";
	default:
		return "ERROR";
	}
	return "ERROR";
}

// Because strncpy sucks, I have to make my own >:(
void myStrncpy(char* writeTo, const char* readFrom, int len)
{
	int i = 0;
	while (i < len && readFrom[i] != '\0')
	{
		writeTo[i] = readFrom[i];
		i++;
	}
	writeTo[i] = '\0';
}

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

void myReadInt(FILEHANDLE cfg, char* tag, int& value)
{
	int newValue;
	bool readSuccess = oapiReadItem_int(cfg, tag, newValue);
	if (readSuccess)
	{
		value = newValue;
		//oapiWriteLogV("MapMFD2 successfully read tag %s from config. Set to %i.", tag, value);
	}
	else oapiWriteLogV("MapMFD2 failed to read tag %s from config. Value hardcoded to %i.", tag, value);
}

void myReadBool(FILEHANDLE cfg, char* tag, bool& value)
{
	bool newValue;
	bool readSuccess = oapiReadItem_bool(cfg, tag, newValue);
	if (readSuccess)
	{
		value = newValue;
		//oapiWriteLogV("MapMFD2 successfully read tag %s from config. Set to %s.", tag, value ? "TRUE" : "FALSE");
	}
	else oapiWriteLogV("MapMFD2 failed to read tag %s from config. Value hardcoded to %s.", tag, value ? "TRUE" : "FALSE");
}

void myReadFloat(FILEHANDLE cfg, char* tag, double& value)
{
	double newValue;
	bool readSuccess = oapiReadItem_float(cfg, tag, newValue);
	if (readSuccess)
	{
		value = newValue;
		//oapiWriteLogV("MapMFD2 successfully read tag %s from config. Set to %f.", tag, value);
	}
	else oapiWriteLogV("MapMFD2 failed to read tag %s from config. Value hardcoded to %f.", tag, value);
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

	FILEHANDLE cfgFile = oapiOpenFile("MFD\\MapMFD2.cfg", FILE_IN_ZEROONFAIL, CONFIG);
	if (cfgFile == 0) oapiWriteLog("MapMFD2 could not find cfg file!");
	else
	{
		myReadInt(cfgFile, "DefTrackMode", DEFAULT_VALUES.TRACK_MODE);
		myReadInt(cfgFile, "DefProjection", DEFAULT_VALUES.PROJECTION);
		myReadInt(cfgFile, "DefGridAngleSep", DEFAULT_VALUES.GRID_ANGLE_SEP);
		myReadInt(cfgFile, "DefGridResolution", DEFAULT_VALUES.GRID_RESOLUTION);
		myReadInt(cfgFile, "DefDefaultLinesAmount", DEFAULT_VALUES.LINES_AMOUNT);
		myReadFloat(cfgFile, "DefGroundTrackStep", DEFAULT_VALUES.GROUNDTRACK_STEP);
		myReadFloat(cfgFile, "DefGroundTrackMaxTimePeriodFraction", DEFAULT_VALUES.GROUNDTRACK_MAXTIMEFRAC);
		myReadFloat(cfgFile, "DefTrackOrbitsNumber", DEFAULT_VALUES.GROUNDTRACK_ORBITS);
		myReadInt(cfgFile, "DefViewCircleResolution", DEFAULT_VALUES.VIEW_CIRCLE_RESOLUTION);
		myReadBool(cfgFile, "DefShowAltitudeRadar", DEFAULT_VALUES.ELEVATION_RADAR);
		myReadBool(cfgFile, "DefShowOtherVessels", DEFAULT_VALUES.SHOW_VESSELS);
		myReadBool(cfgFile, "DefShowHistoryTrack", DEFAULT_VALUES.SHOW_HISTORY);
		myReadFloat(cfgFile, "DefDrawSpecificAltitude", DEFAULT_VALUES.DRAW_SPECIFIC_ALT);
		myReadBool(cfgFile, "DefGroundtrackNumeric", DEFAULT_VALUES.GROUNDTRACK_NUMERIC);
		myReadBool(cfgFile, "DefConsiderTerrainForGroundTrack", DEFAULT_VALUES.CONSIDER_TERRAIN_GROUNDTRACK);

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
		for (int i = 0; i < 6; i++)
		{
			char ColourName[20];
			sprintf(ColourName, "ColourMarker%i", i);
			ReadUserColour(cfgFile, ColourName, &DEFAULT_COLOURS.MARKER[i]);
		}
		ReadUserColour(cfgFile, "ColourSunFill", &DEFAULT_COLOURS.SUNFILL);
		ReadUserColour(cfgFile, "ColourTerminator", &DEFAULT_COLOURS.TERMINATOR);
		ReadUserColour(cfgFile, "ColourSunIcon", &DEFAULT_COLOURS.SUNICON);
		ReadUserColour(cfgFile, "ColourMainView", &DEFAULT_COLOURS.MAINVIEW);
		ReadUserColour(cfgFile, "ColourTargetView", &DEFAULT_COLOURS.TARGETVIEW);

		// Enable default markers
		char defaultMarkers[2000]; // many full paths can get long.
		char markerFilePath[200];
		oapiReadItem_string(cfgFile, "DefaultMarkers", defaultMarkers); // read entire string

		int startPath = 0; // initialise at beginning.
		int endPath = strlen(defaultMarkers); // initialise at end of line
		comma = "hello world"; // give some non-NULL value
		while (comma != NULL && strlen(defaultMarkers) > 10) // contents in default
		{
			comma = strchr(defaultMarkers + startPath, ',');
			if (comma != NULL) endPath = comma - defaultMarkers;
			myStrncpy(markerFilePath, defaultMarkers + startPath, endPath - startPath);

			while (markerFilePath[0] == ' ') myStrncpy(markerFilePath, markerFilePath + 1, strlen(markerFilePath)); // remove leading spaces
			while (markerFilePath[strlen(markerFilePath) - 1] == ' ') myStrncpy(markerFilePath, markerFilePath, strlen(markerFilePath) - 1); // remove trailing spaces

			sprintf(enabledMarkers[numMarkers], markerFilePath);
			numMarkers++;
			oapiWriteLogV("MapMFD2: default marker %i <%s>", numMarkers, markerFilePath);

			startPath = comma - defaultMarkers + 1; // update start path
			endPath = strlen(defaultMarkers); // assume until end of string, unless a new comma is found.
		}
	}


	oapiCloseFile(cfgFile, FILE_IN);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode(g_MFDmode);

	resetCommand = true; // tell MFD that we have locked Orbiter, and don't want to recall from RecallStatus
}
