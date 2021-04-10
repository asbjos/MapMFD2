/*
	Map MFD 2, a project by Asbjørn 'asbjos' Krüger, 2020.

	The intention is to create a new and hopefully better Map MFD experience
	than the default version in Orbiter Space Flight Simulator 2016.

	This will not and can not replace the original, but will hopefully be of use.

	A basic roadmap and wishlist:
		Multibase select
		Multitarget select
		Different map projection
		Possibly optimised for zoom (plot lines selectively)
		Show distance to base AND vessel. Both TDist (true distance) and GDist (ground distance - orthodome).

	This source code is released under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
	For other use, please contact me (I'm username 'asbjos' on Orbiter-Forum).
*/

#define STRICT
#define ORBITER_MODULE

#include "orbitersdk.h"
#include "MapMFD2.h"

//#include <string>
//#include <iostream>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
//using namespace std;
namespace fs = std::experimental::filesystem;


bool MapMFD::Update(oapi::Sketchpad* skp)
{
	if (configScreen)
	{
		ConfigScreen(skp);
	}
	else if (referenceListScreen)
	{
		ReferenceListScreen(skp);
	}
	else if (targetListScreen)
	{
		TargetListScreen(skp);
	}
	else if (markerListScreen)
	{
		MarkerListScreen(skp);
	}
	else
	{
		MapScreen(skp);
	}

	return true;
}

bool MapMFD::ConsumeButton(int bt, int event)
{
	// The centreLong/centreLat pan buttons are immediate, so process entire pressed event
	if (!configScreen && !referenceListScreen && !targetListScreen && !markerListScreen) // in map screen
	{
		bool newPress = false;
		if (event & PANEL_MOUSE_LBDOWN) newPress = true; // new click
		else if (event & PANEL_MOUSE_LBPRESSED) newPress = false; // holding click
		else return false; // no click (at least not of the ones we care about)

		if (bt == 6 && trackPosition != LATLONGTRACK)
		{
			return ConsumeKeyImmediate(OAPI_KEY_MINUS, newPress);
		}
		if (bt == 7 && trackPosition != LATLONGTRACK)
		{
			return ConsumeKeyImmediate(OAPI_KEY_EQUALS, newPress);
		}
		if (bt == 8 && trackPosition == NOTRACK)
		{
			return ConsumeKeyImmediate(OAPI_KEY_LBRACKET, newPress);
		}
		if (bt == 9 && trackPosition == NOTRACK)
		{
			return ConsumeKeyImmediate(OAPI_KEY_RBRACKET, newPress);
		}
	}

	// If nothing happens, or not a keydown event, then don't do anything
	if (!(event & PANEL_MOUSE_LBDOWN)) return false;

	if (configScreen)
	{
		if (bt == 0)
		{
			return ConsumeKeyBuffered(OAPI_KEY_MINUS);
		}
		if (bt == 1)
		{
			return ConsumeKeyBuffered(OAPI_KEY_EQUALS);
		}
		if (bt == 2)
		{
			return ConsumeKeyBuffered(OAPI_KEY_M);
		}
		if (bt == 3)
		{
			return ConsumeKeyBuffered(OAPI_KEY_O);
		}
		if (bt == 4)
		{
			return ConsumeKeyBuffered(OAPI_KEY_D);
		}
		else return false;
	}
	else if (referenceListScreen)
	{
		if (bt == 0)
		{
			return ConsumeKeyBuffered(OAPI_KEY_MINUS);
		}
		if (bt == 1)
		{
			return ConsumeKeyBuffered(OAPI_KEY_EQUALS);
		}
		if (bt == 2)
		{
			return ConsumeKeyBuffered(OAPI_KEY_M);
		}
		if (bt == 3)
		{
			return ConsumeKeyBuffered(OAPI_KEY_O);
		}
		if (bt == 4)
		{
			return ConsumeKeyBuffered(OAPI_KEY_D);
		}
		if (bt == 5)
		{
			return ConsumeKeyBuffered(OAPI_KEY_T);
		}
		else return false;
	}
	else if (targetListScreen)
	{
		if (bt == 0)
		{
			return ConsumeKeyBuffered(OAPI_KEY_MINUS);
		}
		if (bt == 1)
		{
			return ConsumeKeyBuffered(OAPI_KEY_EQUALS);
		}
		if (bt == 2)
		{
			return ConsumeKeyBuffered(OAPI_KEY_M);
		}
		if (bt == 3)
		{
			return ConsumeKeyBuffered(OAPI_KEY_S);
		}
		if (bt == 4)
		{
			return ConsumeKeyBuffered(OAPI_KEY_O);
		}
		if (bt == 5)
		{
			return ConsumeKeyBuffered(OAPI_KEY_T);
		}
		if (bt == 6)
		{
			return ConsumeKeyBuffered(OAPI_KEY_N);
		}
		else return false;
	}
	else if (markerListScreen)
	{
		if (bt == 0)
		{
			return ConsumeKeyBuffered(OAPI_KEY_MINUS);
		}
		if (bt == 1)
		{
			return ConsumeKeyBuffered(OAPI_KEY_EQUALS);
		}
		if (bt == 2)
		{
			return ConsumeKeyBuffered(OAPI_KEY_M);
		}
		if (bt == 3)
		{
			return ConsumeKeyBuffered(OAPI_KEY_O);
		}
	}
	else
	{
		if (bt == 0)
		{
			return ConsumeKeyBuffered(OAPI_KEY_R);
		}
		if (bt == 1)
		{
			return ConsumeKeyBuffered(OAPI_KEY_T);
		}
		if (bt == 2)
		{
			return ConsumeKeyBuffered(OAPI_KEY_X);
		}
		if (bt == 3)
		{
			return ConsumeKeyBuffered(OAPI_KEY_Z);
		}
		if (bt == 4)
		{
			return ConsumeKeyBuffered(OAPI_KEY_K);
		}
		if (bt == 5)
		{
			return ConsumeKeyBuffered(OAPI_KEY_C);
		}
		/*if (bt == 6 && trackPosition != LATLONGTRACK)
		{
			return ConsumeKeyBuffered(OAPI_KEY_MINUS);
		}
		if (bt == 7 && trackPosition != LATLONGTRACK)
		{
			return ConsumeKeyBuffered(OAPI_KEY_EQUALS);
		}
		if (bt == 8 && trackPosition == NOTRACK)
		{
			return ConsumeKeyBuffered(OAPI_KEY_LBRACKET);
		}
		if (bt == 9 && trackPosition == NOTRACK)
		{
			return ConsumeKeyBuffered(OAPI_KEY_RBRACKET);
		}*/
		if (bt == 10)
		{
			return ConsumeKeyBuffered(OAPI_KEY_M);
		}
		if (bt == 11)
		{
			return ConsumeKeyBuffered(OAPI_KEY_P);
		}
		else return false;
	}
}

bool MapMFD::ConsumeKeyBuffered(DWORD key)
{
	bool ReferencePlanet(void* id, char* str, void* data);
	bool TargetObject(void* id, char* str, void* data);
	bool GridSeparation(void* id, char* str, void* data);
	bool GridResolution(void* id, char* str, void* data);
	bool NumberMaxOrbitPeriodFraction(void* id, char* str, void* data);
	bool NumberOrbitsDisplayed(void* id, char* str, void* data);
	bool SpecificAltitudeSelection(void* id, char* str, void* data);

	int entriesPerPage = 13; // for list displays

	if (configScreen)
	{
		switch (key)
		{
		case OAPI_KEY_MINUS:
			configSelection = CONFIGSELECT((int(configSelection) + LASTENTRYCONFIG - 1) % int(LASTENTRYCONFIG)); // + 9 instead of -1, which results in negative result
			if (int(configSelection) < currentTopListed) currentTopListed = int(configSelection);
			if (int(configSelection) > currentTopListed + entriesPerPage) currentTopListed = int(configSelection) - entriesPerPage;

			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_EQUALS:
			configSelection = CONFIGSELECT((int(configSelection) + 1) % int(LASTENTRYCONFIG));
			if (int(configSelection) < currentTopListed) currentTopListed = int(configSelection);
			if (int(configSelection) > currentTopListed + entriesPerPage) currentTopListed = int(configSelection) - entriesPerPage;

			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_M:

			switch (configSelection)
			{
			case CONFIGTRACKMODE:
				orbitTrackGround = !orbitTrackGround;
				break;
			case CONFIGRADAR:
				displayElevationRadar = !displayElevationRadar;
				break;
			case CONFIGSHOWVESSELS:
				showVessels = !showVessels;
				break;
			case CONFIGDRAWSPECIFICALT:
				oapiOpenInputBox("Altitude (0 = off):", SpecificAltitudeSelection, 0, 20, (void*)this);
				break;
			case CONFIGSHOWHISTORY:
				showHistory = !showHistory;
				if (!showHistory) shipHistoryLength = 0; // reset if turned off
				break;
			case CONFIGPROJECTION:
				proj = PROJECTION((int(proj) + 1) % int(LASTENTRYPROJECTION));
				break;
			/*case CONFIGFLIPPOLE:
				azimuthalEquidistantNortPole = !azimuthalEquidistantNortPole;
				break;*/
			case CONFIGRESETMAP:
				centreLat = 0.0;
				centreLong = 0.0;
				centreZoom = 1;
				trackPosition = NOTRACK;
				break;
			case CONFIGGRIDSEP:
				oapiOpenInputBox("Grid separation (\u00B0):", GridSeparation, 0, 20, (void*)this);
				break;
			case CONFIGGRIDRES:
				oapiOpenInputBox("Grid resolution (\u00B0):", GridResolution, 0, 20, (void*)this);
				break;
			case CONFIGMAPRES:
				skipEveryNLines = (skipEveryNLines + 1) % (skipEveryNLinesMax + 1);
				if (skipEveryNLines == skipEveryNLinesMax) autoResolution = true;
				else autoResolution = false;
				break;
			case CONFIGMAPAUTOSIZE:
				defaultMapLinesAmount = (defaultMapLinesAmount % 10000) + 1000; // allow up to 1e4 lines, and at least 1e3
				break;
			case CONFIGTRACKANGLEDELTA:
				// Valid values are 0.01, 0.05, 0.1, 0.5, 1.0
				if (orbitTrackAngleDelta == 0.1) orbitTrackAngleDelta = 0.5;
				else if (orbitTrackAngleDelta == 0.5) orbitTrackAngleDelta = 1.0;
				else if (orbitTrackAngleDelta == 1.0) orbitTrackAngleDelta = 0.01;
				else if (orbitTrackAngleDelta == 0.01) orbitTrackAngleDelta = 0.05;
				else orbitTrackAngleDelta = 0.1;
				break;
			case CONFIGTRACKMAXPERIODFRAC:
				oapiOpenInputBox("Max time step period fraction:", NumberMaxOrbitPeriodFraction, 0, 20, (void*)this);
				break;
			case CONFIGTRACKNUMORBITS:
				oapiOpenInputBox("Number of orbits to show:", NumberOrbitsDisplayed, 0, 20, (void*)this);
				break;
			case CONFIGPLANETVIEWSEGMENTS:
				if (viewCircleResolution == 60) viewCircleResolution = 90;
				else if (viewCircleResolution == 90) viewCircleResolution = 120;
				else if (viewCircleResolution == 120) viewCircleResolution = 180;
				else if (viewCircleResolution == 180) viewCircleResolution = 360;
				else viewCircleResolution = 60;
				break;
			case CONFIGMARKERS:
				markerListScreen = true;
				configScreen = false; // byebye
				InvalidateButtons();
				//InvalidateDisplay();
				break;
			case CONFIGRESETALL:
				resetCommand = true;
				break;
			case CONFIGDEBUGINFO:
				debugInformation = !debugInformation;
				break;
			default:
				sprintf(oapiDebugString(), "ERROR! Invalid Configselect setting. Debug %.3f", oapiGetSimTime());
				return false;
			}

			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_D:
			// Set active value to default
			switch (configSelection)
			{
			case CONFIGTRACKMODE:
				orbitTrackGround = true;
				break;
			case CONFIGRADAR:
				displayElevationRadar = false;
				break;
			case CONFIGSHOWVESSELS:
				showVessels = false;
				break;
			case CONFIGDRAWSPECIFICALT:
				drawSpecificAlt = 0.0;
				break;
			case CONFIGSHOWHISTORY:
				showHistory = true;
				break;
			case CONFIGPROJECTION:
				proj = EQUIRECTANGULAR;
				break;
			/*case CONFIGFLIPPOLE:
				azimuthalEquidistantNortPole = true;
				break;*/
			case CONFIGRESETMAP:
				centreLat = 0.0;
				centreLong = 0.0;
				centreZoom = 1;
				trackPosition = NOTRACK;
				break;
			case CONFIGGRIDSEP:
				gridAngleSeparation = 30;
				break;
			case CONFIGGRIDRES:
				gridResolution = 2;
				break;
			case CONFIGMAPRES:
				skipEveryNLines = skipEveryNLinesMax;
				autoResolution = true;
				break;
			case CONFIGMAPAUTOSIZE:
				defaultMapLinesAmount = 5000;
				break;
			case CONFIGTRACKANGLEDELTA:
				orbitTrackAngleDelta = 0.1;
				break;
			case CONFIGTRACKMAXPERIODFRAC:
				maxPeriodFraction = 100.0;
				break;
			case CONFIGTRACKNUMORBITS:
				orbitTrackOrbitsNumber = 3.5;
				break;
			case CONFIGPLANETVIEWSEGMENTS:
				viewCircleResolution = 60;
				break;
			case CONFIGRESETALL:
				resetCommand = false;
				break;
			default:
				sprintf(oapiDebugString(), "ERROR! Invalid configselect reset. Debug %.3f", oapiGetSimTime());
				return false;
			}

			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_O:
			// Back to main map display
			configScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		default:
			return false;
		}
	}
	else if (referenceListScreen)
	{
		OBJHANDLE newReference = 0; // used in OAPI_KEY_O, but must declare before switch
		int numberReferenceChoices = totalPlanets;
		if (referenceExpand >= 0) numberReferenceChoices += sortedPlanetsCache[referenceExpand].moonCount;
		int entriesPerPage = 13;

		switch (key)
		{
		case OAPI_KEY_MINUS:
			referenceSelection = (referenceSelection + numberReferenceChoices - 1) % numberReferenceChoices; // + 9 instead of -1, which results in negative result
			if (referenceSelection < currentReferenceTopListed) currentReferenceTopListed = referenceSelection;
			if (referenceSelection > currentReferenceTopListed + entriesPerPage) currentReferenceTopListed = referenceSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_EQUALS:
			referenceSelection = (referenceSelection + 1) % numberReferenceChoices;
			if (referenceSelection < currentReferenceTopListed) currentReferenceTopListed = referenceSelection;
			if (referenceSelection > currentReferenceTopListed + entriesPerPage) currentReferenceTopListed = referenceSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_M:
			if (referenceExpand == referenceSelection) // selected same object that was previously expanded
			{
				referenceExpand = -1; // close
				if (totalPlanets <= entriesPerPage) currentReferenceTopListed = 0; // don't need to scroll if we can fit everything
			}
			else
			{
				if (referenceExpand >= 0 && referenceSelection > referenceExpand) // we have selected an object in expanded or later
				{
					if (referenceSelection > referenceExpand + sortedPlanetsCache[referenceExpand].moonCount) // we're after the expanded object (e.g. selected Mars after expanded Earth->Moon)
					{
						referenceExpand = referenceSelection - sortedPlanetsCache[referenceExpand].moonCount;
						if (totalPlanets + sortedPlanetsCache[referenceExpand].moonCount <= entriesPerPage) currentReferenceTopListed = 0; // don't need to scroll if we can fit everything
					}
					else // we're inside moon list
					{
						// I don't know of any systems where we have moons orbiting moons. So I didn't bother to implement it.
						referenceSelection = referenceExpand; // reset box back to parent
						referenceExpand = -1; // close
						if (totalPlanets <= entriesPerPage) currentReferenceTopListed = 0; // don't need to scroll if we can fit everything
					}
				}
				else // before list
				{
					referenceExpand = referenceSelection;
					if (totalPlanets + sortedPlanetsCache[referenceExpand].moonCount <= entriesPerPage) currentReferenceTopListed = 0; // don't need to scroll if we can fit everything
				}
			}

			InvalidateDisplay();
			return true;
		case OAPI_KEY_O: // select and return
			if (referenceExpand >= 0 && referenceSelection > referenceExpand) // we have selected an object in expanded or later
			{
				if (referenceSelection > referenceExpand + sortedPlanetsCache[referenceExpand].moonCount) // we're after the expanded object (e.g. selected Mars after expanded Earth->Moon)
				{
					newReference = sortedPlanetsCache[referenceSelection - sortedPlanetsCache[referenceExpand].moonCount].handle;
				}
				else // we're inside moon list
				{
					newReference = sortedPlanetsCache[referenceExpand].moonHandle[referenceSelection - referenceExpand - 1]; // -1 as we're e.g. 4-Mars, 5-Phobos, which is refSel 5 - refExp 4 = 1, but moon nr. 0. 
				}
			}
			else
			{
				newReference = sortedPlanetsCache[referenceSelection].handle;
			}

			if (oapiGetObjectType(newReference) != OBJTP_INVALID) SetNewReference(newReference);
			else sprintf(oapiDebugString(), "ERROR! Debug! Something went wrong as the selected handle was invalid! %.2f", oapiGetSimTime());
			referenceListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_D: // auto and return
			SetNewReference(v->GetSurfaceRef());
			referenceListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_T:
			oapiOpenInputBox("Reference planet:", ReferencePlanet, 0, 20, (void*)this);

			referenceListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		default:
			return false;
		}
	}
	else if (targetListScreen)
	{
		OBJHANDLE newTarget = 0; // used in OAPI_KEY_O, but must declare before switch
		int numberTargetChoices = 3; // spaceports, spacecraft, moons

		switch (targetExpand)
		{
		case EXPANDSPACEPORTS:
			numberTargetChoices += oapiGetBaseCount(ref);
			break;
		case EXPANDSPACECRAFT:
			for (int i = 0; i < (int)oapiGetVesselCount(); i++)
			{
				OBJHANDLE listVessel = oapiGetVesselByIndex(i);
				VESSEL* ves = oapiGetVesselInterface(listVessel);
				if (ves->GetSurfaceRef() == ref)
				{
					numberTargetChoices++;
				}
			}
			break;
		case EXPANDMOONS:
			for (int i = 0; i < totalPlanets; i++)
			{
				if (sortedPlanetsCache[i].handle == ref)
				{
					// We found our planet.
					numberTargetChoices += sortedPlanetsCache[i].moonCount;
					break;
				}
			}
			break;
		case EXPANDNONE:
			break;
		}

		switch (key)
		{
		case OAPI_KEY_MINUS:
			targetSelection = (targetSelection + numberTargetChoices - 1) % numberTargetChoices; // + 9 instead of -1, which results in negative result
			if (targetSelection < currentTargetTopListed) currentTargetTopListed = targetSelection;
			if (targetSelection > currentTargetTopListed + entriesPerPage) currentTargetTopListed = targetSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_EQUALS:
			targetSelection = (targetSelection + 1) % numberTargetChoices;
			if (targetSelection < currentTargetTopListed) currentTargetTopListed = targetSelection;
			if (targetSelection > currentTargetTopListed + entriesPerPage) currentTargetTopListed = targetSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_M:
			if (int(targetExpand) == targetSelection) // selected same object that was previously expanded
			{
				targetExpand = EXPANDNONE; // close
			}
			else if (targetExpand == EXPANDNONE) // nothing is expanded, and user then want to expand the selected item
			{
				targetExpand = TARGETEXPANDMODES(targetSelection);
			}
			else
			{
				if (targetSelection > int(targetExpand)) // we have selected an object in expanded or later
				{
					if (targetSelection > int(targetExpand) + numberTargetChoices - 3) // we're after the expanded list (e.g. selected Spacecraft after expanded Spaceports)
					{
						targetExpand = TARGETEXPANDMODES(targetSelection - (numberTargetChoices - 3));
						targetSelection = int(targetExpand); // reset box to new parent, where we had our box. The label has moved, so move box too.
					}
					else // we're inside the expanded list
					{
						targetSelection = int(targetExpand); // reset box back to parent
						targetExpand = EXPANDNONE; // close
					}
				}
				else // before list
				{
					targetExpand = TARGETEXPANDMODES(targetSelection);
				}
			}

			currentTargetTopListed = 0; // don't need to scroll as we can fit everything

			InvalidateDisplay();

			return true;
		case OAPI_KEY_S: // select (or deselct) as target
			if (targetExpand == EXPANDSPACEPORTS)
			{
				if (0 < targetSelection && targetSelection < numberTargetChoices - 2) // between spaceports header and before spacecrafts header (inside spaceports list)
				{
					OBJHANDLE targetedObject = oapiGetBaseByIndex(ref, targetSelection - 1); // -1 as we start from 1 (0 being the spaceports header)

					if (oapiGetObjectType(targetedObject) != OBJTP_SURFBASE)
					{
						sprintf(oapiDebugString(), "ERROR! DEBUG! Base targeted was not valid OBJHANDLE. %.2f", oapiGetSimTime());
					}
					else
					{
						return AddOrRemoveTarget(targetedObject);
					}
				}
			}
			else if (targetExpand == EXPANDSPACECRAFT)
			{
				if (1 < targetSelection && targetSelection < numberTargetChoices - 1) // between spacecrafts header and before moons header (inside spacecrafts list)
				{
					int vesselToSelect = targetSelection - 2; // -2 as we start from 2 (0 and 1 being spaceports/spacecrafts headers)
					
					int vesselIndex = 0;

					for (int i = 0; i < (int)oapiGetVesselCount(); i++)
					{
						OBJHANDLE listVessel = oapiGetVesselByIndex(i);
						VESSEL* ves = oapiGetVesselInterface(listVessel);
						if (ves->GetSurfaceRef() == ref)
						{
							if (vesselIndex == vesselToSelect) // we found the vessel
							{
								if (oapiGetObjectType(listVessel) != OBJTP_VESSEL)
								{
									sprintf(oapiDebugString(), "ERROR! DEBUG! Vessel targeted was not valid OBJHANDLE. %.2f", oapiGetSimTime());
									return false;
								}
								else
								{
									return AddOrRemoveTarget(listVessel);
								}
							}

							vesselIndex++;
						}
					}
				}
			}
			else if (targetExpand == EXPANDMOONS)
			{
				if (2 < targetSelection && targetSelection < numberTargetChoices) // between moons header and before end of list (inside moons list)
				{
					OBJHANDLE targetedMoon = NULL;
					for (int i = 0; i < totalPlanets; i++)
					{
						if (sortedPlanetsCache[i].handle == ref)
						{
							int moonIndex = targetSelection - 3; // -3 as we start from 3 (0, 1, 2 being spaceports/spacecrafts/moons headers)
							if (0 <= moonIndex && moonIndex < sortedPlanetsCache[i].moonCount) // safety check that we have a valid index
							{
								targetedMoon = sortedPlanetsCache[i].moonHandle[moonIndex];

								if (oapiGetObjectType(targetedMoon) != OBJTP_PLANET)
								{
									sprintf(oapiDebugString(), "ERROR! DEBUG! Moon targeted was not valid OBJHANDLE. %.2f", oapiGetSimTime());
								}
								else
								{
									return AddOrRemoveTarget(targetedMoon);
								}
							}
							else
							{
								sprintf(oapiDebugString(), "ERROR! DEBUG! Moon targeted was not valid index. %.2f", oapiGetSimTime());
							}
						}
					}
				}
			}

			return false; // we should have been through a return true before now
		case OAPI_KEY_O: // select and return home
			targetListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_T:
			oapiOpenInputBox("Target object:", TargetObject, 0, 20, (void*)this);

			// Allow us to see map for potential coordinates
			targetListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_N:
			// Select nearest object in expanded list

			double closestDistance;
			closestDistance = 1e20; // "infinity"
			OBJHANDLE closestObject;
			closestObject = NULL;

			switch (targetExpand)
			{
			case EXPANDSPACEPORTS:
				for (int i = 0; i < int(oapiGetBaseCount(ref)); i++)
				{
					OBJHANDLE surfBase = oapiGetBaseByIndex(ref, i);
					VECTOR3 pos;
					v->GetRelativePos(surfBase, pos);
					if (length(pos) < closestDistance)
					{
						closestDistance = length(pos);
						closestObject = surfBase;
					}
				}

				if (oapiGetObjectType(closestObject) == OBJTP_SURFBASE) AddOrRemoveTarget(closestObject); // add or remove nearest object


				InvalidateDisplay();
				return true;
			case EXPANDSPACECRAFT:
				for (int i = 0; i < (int)oapiGetVesselCount(); i++)
				{
					OBJHANDLE listVessel = oapiGetVesselByIndex(i);
					VESSEL* ves = oapiGetVesselInterface(listVessel);
					if (ves->GetSurfaceRef() == ref && listVessel != v->GetHandle())
					{
						VECTOR3 pos;
						v->GetRelativePos(listVessel, pos);

						if (length(pos) < closestDistance)
						{
							closestDistance = length(pos);
							closestObject = listVessel;
						}
					}
				}

				if (oapiGetObjectType(closestObject) == OBJTP_VESSEL) AddOrRemoveTarget(closestObject);


				InvalidateDisplay();
				return true;
			case EXPANDMOONS:
				for (int i = 0; i < totalPlanets; i++)
				{
					if (sortedPlanetsCache[i].handle == ref)
					{
						// We found our planet.
						for (int j = 0; j < sortedPlanetsCache[i].moonCount; j++)
						{
							VECTOR3 pos;
							v->GetRelativePos(sortedPlanetsCache[i].moonHandle[j], pos);
							if (length(pos) < closestDistance)
							{
								closestDistance = length(pos);
								closestObject = sortedPlanetsCache[i].moonHandle[j];
							}
						}

						if (oapiGetObjectType(closestObject) == OBJTP_CBODY || oapiGetObjectType(closestObject) == OBJTP_PLANET) AddOrRemoveTarget(closestObject);;
						
						return true;
					}
				}


				InvalidateDisplay();
				return true;
			case EXPANDNONE:
			default:
				return false;
			}

			return true;
		default:
			return false;
		}
	}
	else if (markerListScreen)
	{
		char refName[50];
		oapiGetObjectName(ref, refName, 50);
		char folderPath[100];
		sprintf(folderPath, "Config\\%s\\Marker", refName);
		const int TOTAL_MARKERS_THIS_PLANET = 50;
		char availableMarkers[TOTAL_MARKERS_THIS_PLANET][200]; // allow 50 markers. No name should be longer than 200 characters.
		int markersInFolder = 0;

		for (const auto& entry : fs::directory_iterator(folderPath))
		{
			std::string myString{ entry.path().u8string() };
			sprintf(availableMarkers[markersInFolder], myString.c_str());
			markersInFolder++;
		}

		int numberMarkerChoices = markersInFolder;
		switch (key)
		{
		case OAPI_KEY_MINUS:
			markerSelection = (markerSelection + numberMarkerChoices - 1) % numberMarkerChoices; // + 9 instead of -1, which results in negative result
			if (markerSelection < currentMarkerTopListed) currentMarkerTopListed = markerSelection;
			if (markerSelection > currentMarkerTopListed + entriesPerPage) currentMarkerTopListed = markerSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_EQUALS:
			markerSelection = (markerSelection + 1) % numberMarkerChoices;
			if (markerSelection < currentMarkerTopListed) currentMarkerTopListed = markerSelection;
			if (markerSelection > currentMarkerTopListed + entriesPerPage) currentMarkerTopListed = markerSelection - entriesPerPage;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_M:
			// If selected marker in enabled -> disable
			// else -> enable
			return AddOrRemoveMarker(availableMarkers[markerSelection]);

			InvalidateDisplay();
			return true;
		case OAPI_KEY_O: // return home
			markerListScreen = false;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		default:
			return false;
		}
	}
	else // default map screen
	{
		switch (key)
		{
		case OAPI_KEY_R:
			// New "fancy" reference select screen
			referenceListScreen = true;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			//oapiOpenInputBox("Reference planet:", ReferencePlanet, 0, 20, (void*)this);
			return true;
		case OAPI_KEY_T:
			// New "fancy" target select screen
			targetListScreen = true;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			//oapiOpenInputBox("Target object:", TargetObject, 0, 20, (void*)this);
			return true;
		case OAPI_KEY_X:
			centreZoom /= 2;
			if (centreZoom < 1) centreZoom = 1;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_Z:
			centreZoom *= 2;
			if (centreZoom > MAX_ZOOM) centreZoom = MAX_ZOOM;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_K:
			trackPosition = TRACKMODE((int(trackPosition) + 1) % int(LASTENTRYTRACK));
			if (trackPosition == LONGTRACK) centreLat = 0.0; // set to zero, so that it's centered. But it is allowed to change this later.
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		// Pan buttons are moved to ConsumeKeyImmediate
		/*case OAPI_KEY_MINUS:
			centreLat += 15.0 * RAD / double(centreZoom);
			if (centreLat > PI05) centreLat = PI05;
			return true;
		case OAPI_KEY_EQUALS:
			centreLat -= 15.0 * RAD / double(centreZoom);
			if (centreLat < -PI05) centreLat = -PI05;
			return true;
		case OAPI_KEY_LBRACKET:
			centreLong = normangle(centreLong - 15.0 * RAD / double(centreZoom));
			return true;
		case OAPI_KEY_RBRACKET:
			centreLong = normangle(centreLong + 15.0 * RAD / double(centreZoom));
			return true;*/
		case OAPI_KEY_M:
			numTargets -= 1;
			if (numTargets < 0) numTargets = 0;

			InvalidateDisplay();
			return true;
		case OAPI_KEY_P:
			proj = PROJECTION((int(proj) + 1) % int(LASTENTRYPROJECTION));

			{
				bool foundValidProjection = false;
				int checkedProjections = 0;
				while (!foundValidProjection && checkedProjections < int(LASTENTRYPROJECTION)) // cycle through projections until we find a valid one
				{
					// Check if the current projection is in the block-list
					bool selectionIsBlocked = false;
					//int numberChecked = 0;
					int i = 0;
					while (!selectionIsBlocked && i < int(LASTENTRYPROJECTION))
					{
						if (DEFAULT_VALUES.BLOCK_PROJECTIONS[i] == int(proj)) // found the current projection in the block-list
						{
							selectionIsBlocked = true; // stop search
							proj = PROJECTION((int(proj) + 1) % int(LASTENTRYPROJECTION)); // and go to next projection
						}

						i++;
					}

					if (!selectionIsBlocked) // there was no blocking of current projection
					{
						foundValidProjection = true; // success! Allow it to be selected
					}

					checkedProjections++;
				}
			}

			InvalidateDisplay();
			
			return true;
		case OAPI_KEY_1:
			referenceListScreen = true;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_2:
			targetListScreen = true;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_C:
			// Config display
			configScreen = true;
			InvalidateButtons();
			InvalidateDisplay(); // also a fitting occation to update display, so that we don't have to wait for that to happen.
			return true;
		case OAPI_KEY_3:
			// Clear history track
			shipHistoryLength = 0;
			return true;
		case OAPI_KEY_4:
			// Remove all targets
			numTargets = 0;
			return true;
		default:
			return false;
		}
	}

	return false;
}

bool MapMFD::ConsumeKeyImmediate(DWORD key, bool newPress)
{
	double syst = oapiGetSysTime();
	double sysdt = oapiGetSysStep();

	if (newPress) immediateKeyStart = syst;

	double angDelta = 1.0 * RAD + (syst - immediateKeyStart) * (syst - immediateKeyStart) * 7.5 * RAD; // speed up motion over time with a quadratic law.
	angDelta = min(angDelta, 360.0 * RAD); // don't allow faster than 360 deg/s. Currently takes sqrt((360 - 1) / 7.5) = 6.9 seconds to hit limit.
	// And then finally adjust for zoom:
	angDelta /= double(centreZoom);

	switch (key)
	{
	case OAPI_KEY_MINUS:
		centreLat += angDelta * sysdt;
		if (centreLat > PI05) centreLat = PI05;
		break;
	case OAPI_KEY_EQUALS:
		centreLat -= angDelta * sysdt;
		if (centreLat < -PI05) centreLat = -PI05;
		break;
	case OAPI_KEY_LBRACKET:
		centreLong = normangle(centreLong - angDelta * sysdt);
		break;
	case OAPI_KEY_RBRACKET:
		centreLong = normangle(centreLong + angDelta * sysdt);
		break;
	default:
		sprintf(oapiDebugString(), "WARNING! ConsumeKeyImmediate got wrong button! %.2f", oapiGetSimTime());
		return false;
	}

	InvalidateDisplay(); // give impression of fluid display. Default Map MFD does this.

	return false; // Don't return anything, unless we want to inhibit Orbiter key handling. Which we don't need.
}

bool ReferencePlanet(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetReferencePlanet(str);
}

bool TargetObject(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetTargetObject(str);
}

bool GridSeparation(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetGridSeparation(str);
}

bool GridResolution(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetGridResolution(str);
}

bool NumberMaxOrbitPeriodFraction(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetMaxPeriodFraction(str);
}

bool NumberOrbitsDisplayed(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetNumberOrbitsDisplayed(str);
}

bool SpecificAltitudeSelection(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetSpecificAltitudeSelect(str);
}

bool MapMFD::SetReferencePlanet(char* rstr)
{
	OBJHANDLE userRef = oapiGetObjectByName(rstr);

	if (!strcmp(rstr, "a") || !strcmp(rstr, "A")) // auto-select
	{
		SetNewReference(v->GetSurfaceRef());
		return true;
	}
	else if (userRef == NULL || oapiGetObjectType(userRef) != OBJTP_PLANET) // If object not is a planet, return false
	{
		return false;
	}
	else
	{
		SetNewReference(userRef);
		return true;
	}
}

bool MapMFD::SetTargetObject(char* rstr)
{
	OBJHANDLE userTarget = oapiGetObjectByName(rstr);

	if (userTarget == NULL) // If object not is a celestial body / vessel, return false
	{
		// Could still be a base, though
		userTarget = oapiGetBaseByName(ref, rstr);
		if (userTarget == NULL) // no base
		{
			// Could still be a long-lat pair, though
			char* strPos;
			strPos = strchr(rstr, ' ');

			if (strPos != NULL && numTargets < 10) // found two values, i.e. a coordinate in 'long lat' format, both in degrees. And we have room for new target.
			{
				char longText[20];
				char latText[20];
				strncpy(longText, rstr, strPos - rstr + 1); // gets the first part (before space)
				targetCoord[numTargets][0] = (float)atof(longText);
				strncpy(latText, rstr + int(strPos - rstr + 1), strlen(rstr) - int(strPos - rstr + 1)); // gets the second part (after space)
				targetCoord[numTargets][1] = (float)atof(latText);
				targets[numTargets] = NULL;
				numTargets += 1;

				return true;
			}

			// Still nothing recognized. Set false.
			return false;
		}
		else return AddOrRemoveTarget(userTarget); // this will return false if full target list (>10), else true, and will either set new target or deselect old.

		return false;
	}
	else return AddOrRemoveTarget(userTarget); // this will return false if full target list (>10), else true, and will either set new target or deselect old.

	return false;
}

bool MapMFD::SetGridSeparation(char* rstr)
{
	int inputValue = atoi(rstr);

	if (!strcmp(rstr, "a") || !strcmp(rstr, "A")) // auto-select
	{
		gridAngleSeparation = 30; // default value
		return true;
	}
	else if (inputValue < 1)
	{
		return false; // must be positive at least 1 degree per line
	}
	else
	{
		gridAngleSeparation = inputValue;
		return true;
	}
	return false;
}

bool MapMFD::SetGridResolution(char* rstr)
{
	int inputValue = atoi(rstr);

	if (!strcmp(rstr, "a") || !strcmp(rstr, "A")) // auto-select
	{
		gridResolution = 2; // default value
		return true;
	}
	else if (inputValue < 1)
	{
		return false; // must be positive at least 1 degree line length
	}
	else
	{
		gridResolution = inputValue;
		return true;
	}
	return false;
}

bool MapMFD::SetMaxPeriodFraction(char* rstr)
{
	double inputValue = atof(rstr);

	if (inputValue > 0.0)
	{
		maxPeriodFraction = inputValue;
		return true;
	}

	return false;
}

bool MapMFD::SetNumberOrbitsDisplayed(char* rstr)
{
	double inputValue = atof(rstr);

	if (!strcmp(rstr, "a") || !strcmp(rstr, "A")) // auto-select
	{
		orbitTrackOrbitsNumber = 3.5; // default value
		return true;
	}
	else if (inputValue < 0.0)
	{
		return false; // must be positive
	}
	else
	{
		orbitTrackOrbitsNumber = inputValue;
		return true;
	}
	return false;
}

bool MapMFD::SetSpecificAltitudeSelect(char* rstr)
{
	double factor = 1.0;
	if (!_strnicmp(rstr + strlen(rstr) - 1, "k", 1))
		factor = 1e3;
	else if (!_strnicmp(rstr + strlen(rstr) - 1, "M", 1))
		factor = 1e6;

	drawSpecificAlt = atof(rstr) * factor;
	return true;
}

void MapMFD::StoreStatus() const
{
	MapMFDState.autoResolution = autoResolution;
	//MapMFDState.azimuthalEquidistantNortPole = azimuthalEquidistantNortPole;
	MapMFDState.centreLat = centreLat;
	MapMFDState.centreLong = centreLong;
	MapMFDState.centreZoom = centreZoom;
	MapMFDState.configScreen = configScreen;
	MapMFDState.configSelection = configSelection;
	MapMFDState.defaultMapLinesAmount = defaultMapLinesAmount;
	MapMFDState.gridAngleSeparation = gridAngleSeparation;
	MapMFDState.gridResolution = gridResolution;
	MapMFDState.numTargets = numTargets;
	MapMFDState.proj = proj;
	MapMFDState.ref = ref;
	MapMFDState.refMu = refMu;
	MapMFDState.refRad = refRad;
	MapMFDState.skipEveryNLines = skipEveryNLines;
	MapMFDState.skipEveryNLinesMax = skipEveryNLinesMax;
	for (int i = 0; i < numTargets; i++)
	{
		MapMFDState.targets[i] = targets[i];
		MapMFDState.targetCoord[i][0] = targetCoord[i][0];
		MapMFDState.targetCoord[i][1] = targetCoord[i][1];
	}
	MapMFDState.trackPosition = trackPosition;
	MapMFDState.orbitTrackGround = orbitTrackGround;
	MapMFDState.orbitTrackAngleDelta = orbitTrackAngleDelta;
	MapMFDState.maxPeriodFraction = maxPeriodFraction;
	MapMFDState.orbitTrackOrbitsNumber = orbitTrackOrbitsNumber;
	MapMFDState.displayElevationRadar = displayElevationRadar;
	MapMFDState.drawSpecificAlt = drawSpecificAlt;
	for (int i = 0; i < GROUND_TRACK_HISTORY_SIZE; i++)
	{
		MapMFDState.shipHistory[i][0] = shipHistory[i][0];
		MapMFDState.shipHistory[i][1] = shipHistory[i][1];
		MapMFDState.shipHistory[i][2] = shipHistory[i][2];
	}
	MapMFDState.shipHistoryIndex = shipHistoryIndex;
	MapMFDState.shipHistoryLength = shipHistoryLength;
	MapMFDState.showVessels = showVessels;
	MapMFDState.showHistory = showHistory;
}

void MapMFD::RecallStatus()
{
	if (oapiGetObjectType(MapMFDState.ref) != OBJTP_INVALID && !resetCommand) // reset command asks us to not recall, so that we set to default
	{
		autoResolution = MapMFDState.autoResolution;
		centreLat = MapMFDState.centreLat;
		centreLong = MapMFDState.centreLong;
		centreZoom = MapMFDState.centreZoom;
		configScreen = MapMFDState.configScreen;
		configSelection = MapMFDState.configSelection;
		defaultMapLinesAmount = MapMFDState.defaultMapLinesAmount;
		gridAngleSeparation = MapMFDState.gridAngleSeparation;
		gridResolution = MapMFDState.gridResolution;
		numTargets = MapMFDState.numTargets;
		proj = MapMFDState.proj;
		ref = MapMFDState.ref;
		refMu = MapMFDState.refMu;
		refRad = MapMFDState.refRad;
		skipEveryNLines = MapMFDState.skipEveryNLines;
		skipEveryNLinesMax = MapMFDState.skipEveryNLinesMax;
		for (int i = 0; i < numTargets; i++)
		{
			targets[i] = MapMFDState.targets[i];
			targetCoord[i][0] = MapMFDState.targetCoord[i][0];
			targetCoord[i][1] = MapMFDState.targetCoord[i][1];
		}
		trackPosition = MapMFDState.trackPosition;

		orbitTrackAngleDelta = MapMFDState.orbitTrackAngleDelta;
		maxPeriodFraction = MapMFDState.maxPeriodFraction;
		orbitTrackGround = MapMFDState.orbitTrackGround;
		orbitTrackOrbitsNumber = MapMFDState.orbitTrackOrbitsNumber;
		displayElevationRadar = MapMFDState.displayElevationRadar;
		drawSpecificAlt = MapMFDState.drawSpecificAlt;
		for (int i = 0; i < GROUND_TRACK_HISTORY_SIZE; i++)
		{
			shipHistory[i][0] = MapMFDState.shipHistory[i][0];
			shipHistory[i][1] = MapMFDState.shipHistory[i][1];
			shipHistory[i][2] = MapMFDState.shipHistory[i][2];
		}
		shipHistoryIndex = MapMFDState.shipHistoryIndex;
		shipHistoryLength = MapMFDState.shipHistoryLength;
		showVessels = MapMFDState.showVessels;
		showHistory = MapMFDState.showHistory;
	}
	else if (resetCommand)
	{
		resetCommand = false; // Set back to default, after we've flushed the MFD.
	}
}


void MapMFD::WriteStatus(FILEHANDLE scn) const
{
	char cbuf[100];

	oapiGetObjectName(ref, cbuf, 100);
	oapiWriteScenario_string(scn, "REF", cbuf);

	for (int i = 0; i < numTargets; i++)
	{
		if (targets[i] != NULL)
		{
			oapiGetObjectName(targets[i], cbuf, 100);
			oapiWriteScenario_string(scn, "TARGET", cbuf);
		}
		else
		{
			sprintf(cbuf, "%.2f %.2f", targetCoord[i][0], targetCoord[i][1]);
			oapiWriteScenario_string(scn, "TARGET", cbuf);
		}
	}

	oapiWriteScenario_int(scn, "ZOOM", centreZoom);

	if (trackPosition == NOTRACK)
	{
		sprintf(cbuf, "%.2f %.2f", centreLong * DEG, centreLat * DEG);
		oapiWriteScenario_string(scn, "POS", cbuf);
	}

	oapiWriteScenario_int(scn, "PROJ", int(proj));
}

void MapMFD::ReadStatus(FILEHANDLE scn)
{
	char* cbuf;
	bool posSet = false;

	while (oapiReadScenario_nextline(scn, cbuf))
	{
		if (!_strnicmp(cbuf, "REF", 3))
		{
			OBJHANDLE scnRef = oapiGetGbodyByName(cbuf + 3);
			if (oapiGetObjectType(scnRef) == OBJTP_PLANET) SetNewReference(scnRef);
		}
		else if (!_strnicmp(cbuf, "TARGET", 6))
		{
			char* targetName = cbuf + 7;
			OBJHANDLE scnTgt = oapiGetVesselByName(targetName);
			if (scnTgt == NULL) scnTgt = oapiGetBaseByName(ref, targetName);

			if (oapiGetObjectType(scnTgt) == OBJTP_SURFBASE || oapiGetObjectType(scnTgt) == OBJTP_VESSEL)
			{
				AddOrRemoveTarget(scnTgt);
			}
			else
			{
				// Could still be a long-lat pair, though
				char* strPos;
				strPos = strchr(targetName, ' ');

				if (strPos != NULL) // found two values, i.e. a coordinate in 'long lat' format, both in degrees.
				{
					char longText[20];
					char latText[20];
					myStrncpy(longText, targetName, strPos - targetName + 1); // gets the first part (before space)
					targetCoord[numTargets][0] = (float)atof(longText);
					myStrncpy(latText, targetName + int(strPos - targetName + 1), strlen(targetName) - int(strPos - targetName + 1)); // gets the second part (after space)
					targetCoord[numTargets][1] = (float)atof(latText);
					targets[numTargets] = NULL;
					numTargets += 1;
				}
			}
		}
		else if (!_strnicmp(cbuf, "ZOOM", 4))
		{
			int scnZoom = atoi(cbuf + 4);

			if (scnZoom >= 1 && scnZoom <= MAX_ZOOM) centreZoom = scnZoom;
		}
		else if (!_strnicmp(cbuf, "POS", 3))
		{
			// A long lat pair
			char* strPos;
			char* rstr = cbuf + 4;
			strPos = strchr(rstr, ' ');

			if (strPos != NULL) // found two values, i.e. a coordinate in 'long lat' format, both in degrees.
			{
				char longText[20];
				char latText[20];
				myStrncpy(longText, rstr, strPos - rstr + 1); // gets the first part (before space)
				centreLong = atof(longText) * RAD;
				myStrncpy(latText, rstr + int(strPos - rstr + 1), strlen(rstr) - int(strPos - rstr + 1)); // gets the second part (after space)
				centreLat = atof(latText) * RAD;
			}

			posSet = true;
		}
		else if (!_strnicmp(cbuf, "PROJ", 4))
		{
			proj = PROJECTION(atoi(cbuf + 5));
		}
		else if (!_strnicmp(cbuf, "END_MFD", 7))
		{
			// For some strange reason, Orbiter only stops oapiReadScenario_nextline when "END" is reached, but a MFD stops with "END_MFD", so tell it to stop manually.

			// Use the occation to active tracking if no POS has been set. This is consistent with default MapMFD
			if (!posSet) trackPosition = LATLONGTRACK;

			return;
		}
	}
}

void MapMFD::MapScreen(oapi::Sketchpad* skp)
{
	int textPos = 2;

	// Title
	char refName[30], mfdTitle[100];
	oapiGetObjectName(ref, refName, 30);

	// Get our vessel current frame coordinates. This is used all throughout.
	double currentLong, currentLat, currentRad;
	GetObjectEquPos(v->GetHandle(), &currentLong, &currentLat, &currentRad);

	// Set centre to position if tracking is on
	if (trackPosition != NOTRACK)
	{
		if (trackPosition == LATLONGTRACK)
		{
			centreLat = currentLat;
			centreLong = currentLong;
		}
		else if (trackPosition == LONGTRACK)
		{
			centreLong = currentLong;
		}
	}

	// ===========================
	// Starting map plotting here!

	// First fill Sun-lit side
	MakeSunLight(skp);
	
	// Draw grid lines for map
	MakeGridLines(skp);

	// Draw map
	MakeMap(skp, refName, &textPos);

	// Draw markers (data files in body folder [e.g. Config\Moon\Marker\Miscellaneous.mkr])
	MakeMarkers(skp, refName);

	// Draw surface bases and vessels
	MakeSurfaceBasesAndVessels(skp);

	int infoLinesDrawn = 0;

	// Draw targets and their information
	MakeTargets(skp, currentLong, currentLat, &infoLinesDrawn);

	// Draw all our own ship info and data
	MakeShip(skp, currentLong, currentLat, currentRad, &infoLinesDrawn);

	// ==========================
	// End of map plotting.

	// Write text in the end, to get it on top
	skp->SetFont(GetDefaultFont(0)); // Set back to default
	skp->SetTextAlign(); // back to default
	char cbuf[100];
	if (debugInformation)
	{
		sprintf(cbuf, "In: %i, out: %i.", pointsInside, pointsOutside);
		skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
		textPos++;

		sprintf(cbuf, "SkipFactor: % i.Auto : % i", int(58800 / defaultMapLinesAmount / centreZoom) + 1, int(autoResolution));
		skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
		textPos++;
	}
	pointsInside = 0;
	pointsOutside = 0;

	sprintf(mfdTitle, "Map: ");
	strcat(mfdTitle, refName); // Add name of mapped body

	Title(skp, mfdTitle);	// Draws the MFD title

	// Draw track and zoom info
	if (trackPosition != NOTRACK) sprintf(cbuf, "TRK");
	else sprintf(cbuf, "   ");

	if (centreZoom > 1) sprintf(cbuf, "%s  ZM %i", cbuf, centreZoom);
	skp->Text(textDX * 53, 0, cbuf, strlen(cbuf));

	//sprintf(cbuf, GetProjectionName());
	sprintf(cbuf, GetSpecificProjectionName(proj));
	skp->Text(textDX, textDY * 1, cbuf, strlen(cbuf));
}

void MapMFD::ConfigScreen(oapi::Sketchpad* skp)
{
	// Title
	char mfdTitle[50];
	sprintf(mfdTitle, "Map: Display parameters");
	Title(skp, mfdTitle); // Draws the MFD title

	int textX0 = W / 80;
	int dY = H / 16;
	int textY0 = H / 20 * 2 - currentTopListed * dY;
	int secondRowIndent = 60;

	char cbuf[100];
	skp->SetFont(GetDefaultFont(1)); // Set to the correct font
	skp->SetTextColor(configTextColour); // Set to default colour

	sprintf(cbuf, "Orbit lines");
	skp->Text(textX0, textY0 + int(CONFIGTRACKMODE) * dY, cbuf, strlen(cbuf));
	if (orbitTrackGround) sprintf(cbuf, "Groundtrack");
	else sprintf(cbuf, "Orbit plane");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGTRACKMODE) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Elevation radar");
	skp->Text(textX0, textY0 + int(CONFIGRADAR) * dY, cbuf, strlen(cbuf));
	if (displayElevationRadar) sprintf(cbuf, "On");
	else sprintf(cbuf, "Off");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGRADAR) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Display other vessels");
	skp->Text(textX0, textY0 + int(CONFIGSHOWVESSELS) * dY, cbuf, strlen(cbuf));
	if (showVessels) sprintf(cbuf, "On");
	else sprintf(cbuf, "Off");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGSHOWVESSELS) * dY, cbuf, strlen(cbuf));

	if (!orbitTrackGround) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects ground track mode, so indicate that by graying out if not in that
	sprintf(cbuf, "Display specific altitude");
	skp->Text(textX0, textY0 + int(CONFIGDRAWSPECIFICALT) * dY, cbuf, strlen(cbuf));
	if (drawSpecificAlt == 0.0) sprintf(cbuf, " Off"); // Add space, as format value does it, and we later remove that space
	else FormatValue(cbuf, 20, drawSpecificAlt);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGDRAWSPECIFICALT) * dY, cbuf + 1, strlen(cbuf + 1)); // Format value creates an ugly space before number, so remove it here
	if (!orbitTrackGround) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	if (!orbitTrackGround) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects ground track mode, so indicate that by graying out if not in that
	sprintf(cbuf, "Display history track");
	skp->Text(textX0, textY0 + int(CONFIGSHOWHISTORY) * dY, cbuf, strlen(cbuf));
	if (showHistory) sprintf(cbuf, "On");
	else sprintf(cbuf, "Off");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGSHOWHISTORY) * dY, cbuf, strlen(cbuf));
	if (!orbitTrackGround) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	sprintf(cbuf, "Projection");
	skp->Text(textX0, textY0 + int(CONFIGPROJECTION) * dY, cbuf, strlen(cbuf));
	//sprintf(cbuf, GetProjectionName());
	sprintf(cbuf, GetSpecificProjectionName(proj));
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGPROJECTION) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Reset map pan");
	skp->Text(textX0, textY0 + int(CONFIGRESETMAP) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%.1f\u00B0N %.1f\u00B0E", centreLat * DEG, centreLong * DEG);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGRESETMAP) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Grid angle sep.");
	skp->Text(textX0, textY0 + int(CONFIGGRIDSEP) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%i\u00B0", gridAngleSeparation);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGGRIDSEP) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Grid angle res.");
	skp->Text(textX0, textY0 + int(CONFIGGRIDRES) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%i\u00B0", gridResolution);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGGRIDRES) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Map skip every N line");
	skp->Text(textX0, textY0 + int(CONFIGMAPRES) * dY, cbuf, strlen(cbuf));
	if (autoResolution) sprintf(cbuf, "AUTO");
	else sprintf(cbuf, "%i", skipEveryNLines);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGMAPRES) * dY, cbuf, strlen(cbuf));

	if (!autoResolution) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects auto resolution state, so indicate that by graying out if not in auto
	sprintf(cbuf, "Map auto resol. lines");
	skp->Text(textX0, textY0 + int(CONFIGMAPAUTOSIZE) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%i", defaultMapLinesAmount);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGMAPAUTOSIZE) * dY, cbuf, strlen(cbuf));
	if (!autoResolution) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	if (!orbitTrackGround) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects ground track, so indicate that by graying out if not in that mode
	sprintf(cbuf, "Orbit track time step");
	skp->Text(textX0, textY0 + int(CONFIGTRACKANGLEDELTA) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%.3f\u00B0", orbitTrackAngleDelta);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGTRACKANGLEDELTA) * dY, cbuf, strlen(cbuf));
	if (!orbitTrackGround) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	if (!orbitTrackGround) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects ground track, so indicate that by graying out if not in that mode
	sprintf(cbuf, "Orbit track max period fraction");
	skp->Text(textX0, textY0 + int(CONFIGTRACKMAXPERIODFRAC) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%.1f", maxPeriodFraction);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGTRACKMAXPERIODFRAC) * dY, cbuf, strlen(cbuf));
	if (!orbitTrackGround) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	if (!orbitTrackGround) skp->SetTextColor(inactiveConfigTextColour); // this setting only affects ground track, so indicate that by graying out if not in that mode
	sprintf(cbuf, "Max orbits displayed");
	skp->Text(textX0, textY0 + int(CONFIGTRACKNUMORBITS) * dY, cbuf, strlen(cbuf));
	double iterationStop = GROUND_TRACK_ITERATION_MAX_STEPS / (360.0 / orbitTrackAngleDelta); // iteration stops sooner if too high resolution.
	if (iterationStop < orbitTrackOrbitsNumber) sprintf(cbuf, "%.2f (%.2f)", orbitTrackOrbitsNumber, iterationStop);
	else sprintf(cbuf, "%.2f", orbitTrackOrbitsNumber);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGTRACKNUMORBITS) * dY, cbuf, strlen(cbuf));
	if (!orbitTrackGround) skp->SetTextColor(configTextColour); // switch back to normal text colour if we were in inactive state.

	sprintf(cbuf, "Circle of view resolution");
	skp->Text(textX0, textY0 + int(CONFIGPLANETVIEWSEGMENTS) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "%i", viewCircleResolution);
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGPLANETVIEWSEGMENTS) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Select markers");
	skp->Text(textX0, textY0 + int(CONFIGMARKERS) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, "Press MOD");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGMARKERS) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Reset MFD");
	skp->Text(textX0, textY0 + int(CONFIGRESETALL) * dY, cbuf, strlen(cbuf));
	if (resetCommand) sprintf(cbuf, "Press F8");
	else sprintf(cbuf, "OFF");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGRESETALL) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Show debug info");
	skp->Text(textX0, textY0 + int(CONFIGDEBUGINFO) * dY, cbuf, strlen(cbuf));
	if (debugInformation) sprintf(cbuf, "ON");
	else sprintf(cbuf, "OFF");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGDEBUGINFO) * dY, cbuf, strlen(cbuf));

	// Create selection box
	skp->SetPen(mainOrbitTrack); // re-use old pen.
	skp->Rectangle(textX0 - 5, textY0 + int(configSelection) * dY, W - textX0, textY0 + dY + int(configSelection) * dY);
}

void MapMFD::ReferenceListScreen(oapi::Sketchpad* skp)
{
	// Title
	char mfdTitle[50];
	sprintf(mfdTitle, "Map: Reference select");
	Title(skp, mfdTitle); // Draws the MFD title

	int textX0 = W / 80;
	int textY0 = H / 20 * 2;
	int dY = H / 16;
	int secondRowIndent = 60;

	char cbuf[100];
	skp->SetFont(GetDefaultFont(1)); // Set to the correct font

	if (updateReferenceCache) BuildReferenceCache(); // create list if it doesn't already exist.

	// Finally print list in sorted order

	int lineNumber = -currentReferenceTopListed;

	for (int i = 0; i < totalPlanets; i++)
	{
		sprintf(cbuf, sortedPlanetsCache[i].name);
		skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
		lineNumber++;

		if (i == referenceExpand)
		{
			for (int k = 0; k < sortedPlanetsCache[i].moonCount; k++)
			{
				sprintf(cbuf, sortedPlanetsCache[i].moonName[k]);
				skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
				lineNumber++;
			}
		}
	}

	// Create selection box
	skp->SetPen(mainOrbitTrack); // re-use old pen.
	skp->Rectangle(textX0 - 5, textY0 + (referenceSelection - currentReferenceTopListed) * dY, W - textX0, textY0 + dY + (referenceSelection - currentReferenceTopListed) * dY);
}

void MapMFD::TargetListScreen(oapi::Sketchpad* skp)
{
	// Title
	char mfdTitle[50];
	sprintf(mfdTitle, "Map: Target select. Number: %i", numTargets);
	Title(skp, mfdTitle); // Draws the MFD title

	int textX0 = W / 80;
	int textY0 = H / 20 * 2;
	int dY = H / 16;
	int secondRowIndent = 60;

	char cbuf[100];
	skp->SetFont(GetDefaultFont(1)); // Set to the correct font

	int lineNumber = -currentTargetTopListed;

	// Display bases on reference object
	if (targetExpand == EXPANDSPACEPORTS) sprintf(cbuf, "Spaceports <");
	else sprintf(cbuf, "Spaceports >");
	skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
	lineNumber++;

	DWORD defaultColour = 0xFFFFFF; // white

	if (targetExpand == EXPANDSPACEPORTS)
	{
		for (int i = 0; i < (int)oapiGetBaseCount(ref); i++)
		{
			OBJHANDLE objectHandle = oapiGetBaseByIndex(ref, i);
			oapiGetObjectName(objectHandle, cbuf, 20);
			if (ObjectAlreadyInTarget(objectHandle)) skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
			skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
			skp->SetTextColor(defaultColour);
			lineNumber++;
		}
	}

	// Display vessels around reference object
	if (targetExpand == EXPANDSPACECRAFT) sprintf(cbuf, "Spacecraft <");
	else sprintf(cbuf, "Spacecraft >");
	skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
	lineNumber++;

	if (targetExpand == EXPANDSPACECRAFT)
	{
		for (int i = 0; i < (int)oapiGetVesselCount(); i++)
		{
			OBJHANDLE listVessel = oapiGetVesselByIndex(i);
			VESSEL* ves = oapiGetVesselInterface(listVessel);
			if (ves->GetSurfaceRef() == ref)
			{
				oapiGetObjectName(listVessel, cbuf, 60);
				if (ObjectAlreadyInTarget(listVessel)) skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
				skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
				skp->SetTextColor(defaultColour);
				lineNumber++;
			}
		}
	}

	// Display moons around reference object
	if (targetExpand == EXPANDMOONS) sprintf(cbuf, "Moons <");
	else sprintf(cbuf, "Moons >");
	skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
	lineNumber++;

	if (targetExpand == EXPANDMOONS)
	{
		if (updateReferenceCache) BuildReferenceCache();

		for (int i = 0; i < totalPlanets; i++)
		{
			if (sortedPlanetsCache[i].handle == ref)
			{
				// We found our planet. Now write out the moons, and then break
				for (int k = 0; k < sortedPlanetsCache[i].moonCount; k++)
				{
					sprintf(cbuf, sortedPlanetsCache[i].moonName[k]);
					if (ObjectAlreadyInTarget(sortedPlanetsCache[i].moonHandle[k])) skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
					skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
					skp->SetTextColor(defaultColour);
					lineNumber++;
				}
				break;
			}
		}
	}

	// Create selection box
	skp->SetPen(mainOrbitTrack); // re-use old pen.
	skp->Rectangle(textX0 - 5, textY0 + (targetSelection - currentTargetTopListed) * dY, W - textX0, textY0 + dY + (targetSelection - currentTargetTopListed) * dY);
}

void MapMFD::MarkerListScreen(oapi::Sketchpad* skp)
{
	// Stock Map MFD has this in config screen. 
	// To make matters easier for myself (I did not expect a dynamic config screen), 
	// and for readability (Venus has 23 marker files),
	// fork out the marker list to a new list, launched from the config screen.

	// Title
	char mfdTitle[50];
	sprintf(mfdTitle, "Markers");
	Title(skp, mfdTitle); // Draws the MFD title

	int textX0 = W / 80;
	int textY0 = H / 20 * 2;
	int dY = H / 16;
	int secondRowIndent = 60;

	char cbuf[100];
	skp->SetFont(GetDefaultFont(1)); // Set to the correct font

	int lineNumber = -currentMarkerTopListed;

	char folderPath[100];
	char refName[40];
	oapiGetObjectName(ref, refName, 40);
	sprintf(folderPath, "Config\\%s\\Marker", refName);
	const int TOTAL_MARKERS_THIS_PLANET = 50;
	char availableMarkers[TOTAL_MARKERS_THIS_PLANET][200]; // allow 50 markers. No name should be longer than 200 characters.
	int markersInFolder = 0;

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string myString{ entry.path().u8string() };
		char thisMarkerName[200];
		sprintf(thisMarkerName, myString.c_str());

		// Get marker name (without path [before \\] and extension [after .])
		char* lastSlashPosWild = strchr(thisMarkerName, '\\');
		char* lastSlashPos = "uwu"; // dank
		while (lastSlashPosWild != NULL)
		{
			lastSlashPosWild = strchr(lastSlashPosWild + 1, '\\');
			if (lastSlashPosWild != NULL) lastSlashPos = lastSlashPosWild; // save the value if actual position.
		}

		int pathLen = int(lastSlashPos - thisMarkerName);
		char subThisMarkerName[100];
		strcpy(subThisMarkerName, thisMarkerName + pathLen + 1); // "Config\\Earth\\Marker\\thisMarker.mkr", +pathLen removes "Config\\...\\Marker\\", where pathLen is length of path.
		char* periodLoc;
		periodLoc = strchr(subThisMarkerName, '.');

		char actualName[100];
		// Found a period
		if (periodLoc != NULL)
		{
			myStrncpy(actualName, subThisMarkerName, periodLoc - subThisMarkerName); // "___.mkr"
		}

		// Print marker name
		sprintf(cbuf, actualName);
		skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
		sprintf(cbuf, "DISABLED");
		for (int i = 0; i < numMarkers; i++)
		{
			if (strcmp(thisMarkerName, enabledMarkers[i]) == 0)
			{
				sprintf(cbuf, "ENABLED");
				break;
			}
		}
		skp->Text(textX0 * secondRowIndent, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
		lineNumber++;

		markersInFolder++;
	}

	// Create selection box
	skp->SetPen(mainOrbitTrack); // re-use old pen.
	skp->Rectangle(textX0 - 5, textY0 + (markerSelection - currentMarkerTopListed) * dY, W - textX0, textY0 + dY + (markerSelection - currentMarkerTopListed) * dY);
}

void MapMFD::MakeSunLight(oapi::Sketchpad *skp)
{
	const int sunCircleResolution = 360 / gridResolution;
	double sunLong, sunLat, sunDist;
	GetObjectEquPos(oapiGetGbodyByIndex(0), &sunLong, &sunLat, &sunDist); // I here expect ALL systems to have the Sun (or light giving star) as first body in config.
	// Calculate the angular viewable area, and calculate points for all different bearings. From https://www.movable-type.co.uk/scripts/latlong.html
	double sunAngleView = PI05; // just set it like this, and save computation power, although it really is 0.0024 degrees off for the Earth.
	// But ACKCHYUALLY, the angle should be 90.26 degrees for Earth, as the "0.0024 degrees" from above considers point light, but as the Sun has a non-zero extent
	// in the sky, the equation for the sunlight angle view (considering just first contact with the Sun's disk, is given by PI05 + asin((sunRad + refRad) / sunDist),
	// which for the Earth is about 90.26 degrees, and not 89.9976 degrees (point light), or 90 degrees (approx).
	// For Mercury, the angle is 90.67 degrees.
	// But turns out that it's MUCH easier to get a nice-looking result using the PI05 simplification, so we don't get the opportunity to be pedantic.

	//// Add icon for sun position first. If the later daylight fill works, this will be overwritten. If it doesn't this shows up as a backup
	//double transformedSunLong, transformedSunLat;
	//TransformPoint(sunLong, sunLat, &transformedSunLong, &transformedSunLat, proj);

	//skp->SetBrush(sunIcon);
	//skp->SetPen(NULL);
	//int sunSize = 10;
	//skp->Ellipse(int(W / 2 + transformedSunLong / PI * W / 2) - sunSize, int(H / 2 - transformedSunLat / PI05 * H / 4) - sunSize, int(W / 2 + transformedSunLong / PI * W / 2) + sunSize, int(H / 2 - transformedSunLat / PI05 * H / 4) + sunSize);
	skp->SetBrush(NULL); // disable autofilling

	// For debug-text:
	skp->SetTextAlign(oapi::Sketchpad::CENTER, oapi::Sketchpad::BASELINE);
	skp->SetFont(mapObjectFont);

	oapi::IVECTOR2 sunFill[1000];
	int fillElements;
	double transformedLong, transformedLat; // create here, instead of creating it three (or more) times

	double bearingToEdge = 0.0; // this value tells where the terminator starts, and is used in fill, and later when we create the terminator
	int direction = 1; // this value tells which direction the terminator goes from the starting bearing, and is used in fill, and later when we create the terminator

	bool completelySunFilled = true;

	switch (proj)
	{
	case EQUIRECTANGULAR:
	//case MILLER:
	case MERCATOR:
	case VANDERGRINTEN:
	case ROBINSON:
	case EQUALEARTH:
	case MOLLWEIDE:
	case HAMMER:
	case LOXIMUTHAL:
	case LASKOWSKI:
	//case ORTELIUSOVAL:
	case WINKELTRIPEL:
	//case RECTANGULARPOLYCONIC:
	case GALLPETERS:
		fillElements = 0; // switch-case doesn't allow for declaring with value, so first declare, then assign
		double leftmostLongitude, leftmostTerminatorLatitude; // longitude of left edge of map
		leftmostLongitude = normangle(centreLong - PI); // switch-case doesn't allow for declaring with value, so first declare, then assign
		leftmostTerminatorLatitude = atan(tan(sunLat + PI05) * sin(leftmostLongitude - sunLong + PI05)); // source: https://math.stackexchange.com/questions/804301/what-is-the-approximation-equation-for-making-the-day-night-wave for inspiration, and some playing/thinking in GeoGebra
		bearingToEdge = atan2(sin(leftmostLongitude - sunLong) * cos(leftmostTerminatorLatitude), cos(sunLat) * sin(leftmostTerminatorLatitude) - sin(sunLat) * cos(leftmostTerminatorLatitude) * cos(leftmostLongitude - sunLong));

		direction = 1;
		if (sunLat < 0.0) direction = -1; // switch direction at equinox, so that points are always plotted from left to right, coming back left at one of the poles.

		for (int i = 0; i <= sunCircleResolution; i++) // around entire circle
		{
			double bearing = -double(direction * i) / double(sunCircleResolution) * PI2 + bearingToEdge; // Add an offset so that first bearing is leftmost part of screen, and last bearing is rightmost part of screen

			double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));
			double longPoint = sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint));

			if (i == 0) longPoint += 1e-5; // force point to lie exactly on the left side
			else if (i == sunCircleResolution) longPoint -= 1e-5; // force point to lie exactly on the right side
			
			TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

			sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
			sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

			if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

			if (debugInformation) // show index at vertices
			{
				char debugLabel[4];
				sprintf(debugLabel, "%i", fillElements);
				skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
			}

			fillElements++;
		}

		// And now add the edge coordinates
		// First the right side
		// Find which latitude to start from. Example: leftmostTerminatorLatitude = 3.8 deg and bottom edge -> start from 2 deg (if gridResolution is 2 deg).
		// Example: leftmostTerminatorLatitude = -3.8 and bottom edge -> start from -4 deg.
		int latitudeStart;
		latitudeStart = ceil(leftmostTerminatorLatitude * DEG / int(gridResolution)) * gridResolution;
		if (sunLat < 0.0) latitudeStart = floor(leftmostTerminatorLatitude * DEG / int(gridResolution)) * gridResolution;

		double decrementValue;
		decrementValue = 0.999;
		// Make right up/down
		for (int i = latitudeStart; abs(i) < 90; i += direction * gridResolution)
		{
			TransformPoint(PI * decrementValue + centreLong, i * RAD, &transformedLong, &transformedLat, proj);
			sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
			sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

			if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

			if (debugInformation) // show index at vertices
			{
				char debugLabel[4];
				sprintf(debugLabel, "%i", fillElements);
				skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
			}

			fillElements++;
		}
		// Make top/bottom
		//for (int i = 180; i >= -180; i -= gridResolution) 
		for (int i = 180; i >= -180; i -= 360) // the poles are in our projections always either flat sides (e.g. equirectangular) or singular points (e.g. Mollweide), so we only need two points, +180 and -180 (for Mollweide we actually only need one)
		{
			if (i == 180) TransformPoint(i * RAD + centreLong - 1e-5, direction * PI05 * decrementValue, &transformedLong, &transformedLat, proj);
			else if (i == -180) TransformPoint(i * RAD + centreLong + 1e-5, direction * PI05 * decrementValue, &transformedLong, &transformedLat, proj);
			else TransformPoint(i * RAD + centreLong, direction * PI05 * decrementValue, &transformedLong, &transformedLat, proj);

			sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
			sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

			if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

			if (debugInformation) // show index at vertices
			{
				char debugLabel[4];
				sprintf(debugLabel, "%i", fillElements);
				skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
			}
			fillElements++;
		}

		// Make left down/up
		for (int i = direction * (90 - gridResolution); direction * i >= direction * latitudeStart; i -= direction * gridResolution)
		{
			TransformPoint(-PI * decrementValue + centreLong, i * RAD, &transformedLong, &transformedLat, proj);
			sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
			sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

			if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

			if (debugInformation) // show index at vertices
			{
				char debugLabel[4];
				sprintf(debugLabel, "%i", fillElements);
				skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
			}
			fillElements++;
		}

		skp->SetBrush(sunlitSide);
		skp->SetPen(NULL); // add terminator line later
		if (completelySunFilled) // there are no vertices on screen
		{
			char debugLabel[4];
			if (oapiOrthodome(centreLong, centreLat, sunLong, sunLat) < sunAngleView) // within sunView, i.e. entire displayed map is lighted up.
			{
				// Omit the flickering problem, and just fill with light
				oapi::IVECTOR2 sunSquare[4];
				sunSquare[0].x = 0 + 1; sunSquare[0].y = 0 + 1;
				sunSquare[1].x = W - 1; sunSquare[1].y = 0 + 1;
				sunSquare[2].x = W - 1; sunSquare[2].y = H - 1;
				sunSquare[3].x = 0 + 1; sunSquare[3].y = H - 1;
				skp->Polygon(sunSquare, 4);

				sprintf(debugLabel, "Simple fill square light");
			}
			else sprintf(debugLabel, "Simple fill square dark"); // All dark, don't plot at all
			
			if (debugInformation) skp->Text(W / 2, H / 2 + 20, debugLabel, strlen(debugLabel)); // display info
		}
		else skp->Polygon(sunFill, fillElements); // normal plotting, as we have a complex polygon view

		break;
	case AZIMUTHALEQUIDISTANT:
	case LAMBERTAZIMUTHAL:
	case STEREOGRAPHIC:
	//case GNOMONIC: // Gnomic features all geodesics as straight lines. Which is a problem, as the terminator is a geodesic. DEBUG: make a specific implementation of Gnomic sunfill.
		/* These are azimuthal projections, where the entire Sun-lit area may be undivided. This occurs if sunLat/sunLong is less than sunAngleView from centreLat/centreLong
		There are two cases 
			- Entire Sun-lit in one piece (sunPos - centrePos < 90 deg)
			- Band of Sun all around, with dark patch in middle (else) */

		if (oapiOrthodome(sunLong, sunLat, centreLong, centreLat) < sunAngleView) // entire Sun in one piece.
		{
			fillElements = 0; // initialise
			for (int i = 0; i < 360; i += gridResolution)
			{
				double bearing = double(i) * RAD;

				double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));
				double longPoint = sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint));

				TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

				sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
				sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

				if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

				if (debugInformation) // show index at vertices
				{
					char debugLabel[4];
					sprintf(debugLabel, "%i", fillElements);
					skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
				}

				fillElements++;
			}

			skp->SetBrush(sunlitSide);
			skp->SetPen(NULL); // add terminator line later
			if (completelySunFilled) // there are no vertices on screen.
			{
				char debugLabel[4];
				if (oapiOrthodome(centreLong, centreLat, sunLong, sunLat) < sunAngleView) // within sunView, i.e. entire displayed map is lighted up.
				{
					// Omit the flickering problem, and just fill with light
					oapi::IVECTOR2 sunSquare[4];
					sunSquare[0].x = 0 + 1; sunSquare[0].y = 0 + 1;
					sunSquare[1].x = W - 1; sunSquare[1].y = 0 + 1;
					sunSquare[2].x = W - 1; sunSquare[2].y = H - 1;
					sunSquare[3].x = 0 + 1; sunSquare[3].y = H - 1;
					skp->Polygon(sunSquare, 4);

					sprintf(debugLabel, "Simple fill square light");
				}
				else sprintf(debugLabel, "Simple fill square dark"); // All dark, don't plot at all

				if (debugInformation) skp->Text(W / 2, H / 2 + 20, debugLabel, strlen(debugLabel)); // display info
			}
			else skp->Polygon(sunFill, fillElements); // normal plotting, as we have a complex polygon view
		}
		else // light strip around. So instead do the opposite of above: first fill entire map with light, and then plot darkness.
		{
			// First plot light
			fillElements = 0; // initialise
			decrementValue = 0.999;
			for (int i = 0; i < 360; i += gridResolution)
			{
				double bearing = double(i) * RAD;

				double latPoint = asin(sin(centreLat) * cos(PI * decrementValue) + cos(centreLat) * sin(PI * decrementValue) * cos(bearing));
				double longPoint = centreLong + atan2(sin(bearing) * sin(PI * decrementValue) * cos(centreLat), cos(PI * decrementValue) - sin(centreLat) * sin(latPoint));

				TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

				sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
				sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

				if (debugInformation) // show index at vertices
				{
					char debugLabel[4];
					sprintf(debugLabel, "%i", fillElements);
					skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
				}

				fillElements++;
			}

			skp->SetBrush(sunlitSide);
			skp->SetPen(NULL); // add terminator line later
			skp->Polygon(sunFill, fillElements);

			fillElements = 0; // reset
			// Then plot darkness, where the centre is the antipode of sunPos, which is (-sunLat, sunLong + 180 deg)
			for (int i = 0; i < 360; i += gridResolution)
			{
				double bearing = double(i) * RAD;

				double latPoint = asin(sin(-sunLat) * cos(sunAngleView) + cos(-sunLat) * sin(sunAngleView) * cos(bearing));
				double longPoint = sunLong + PI + atan2(sin(bearing) * sin(sunAngleView) * cos(-sunLat), cos(sunAngleView) - sin(-sunLat) * sin(latPoint));

				TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

				sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
				sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

				if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

				if (debugInformation) // show index at vertices
				{
					char debugLabel[4];
					sprintf(debugLabel, "%i", fillElements);
					skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
				}

				fillElements++;
			}

			skp->SetBrush(black);
			skp->SetPen(NULL); // add terminator line later
			if (completelySunFilled && oapiOrthodome(centreLong, centreLat, sunLong, sunLat) < sunAngleView) // there are no vertices on screen, and we're within sunView, i.e. entire displayed map is lighted up.
			{
				// We already have made sunFill, as we're in azimuthal projection
				if (debugInformation) // display info
				{
					char debugLabel[4];
					sprintf(debugLabel, "Simple fill square");
					skp->Text(W / 2, H / 2 + 20, debugLabel, strlen(debugLabel));
				}
			}
			else skp->Polygon(sunFill, fillElements); // normal plotting, as we have a complex polygon view
		}

		break;
	case TRANSVERSEMERCATOR:
		// These are transverse projections. Therefore the daylight will always fill either the left or right side (just like the cylindrical projections [e.g. equirectanguar] have always one pole in light).
		fillElements = 0; // switch-case doesn't allow for declaring with value, so first declare, then assign
		double terminatorEquatorLongitude; // longitude of the terminator where it crosses the Equator
		terminatorEquatorLongitude = sunLong - PI05;
		if (abs(normangle(centreLong - terminatorEquatorLongitude)) < PI05) terminatorEquatorLongitude += PI; // push the value to the longitude that is on the map edge, which is more than 90 deg from centreLong

		if (normangle(sunLong - centreLong) < 0.0) direction = -1;
		else direction = 1;

		bearingToEdge = atan2(sin(terminatorEquatorLongitude - sunLong) * cos(0.0), cos(sunLat) * sin(0.0) - sin(sunLat) * cos(0.0) * cos(terminatorEquatorLongitude - sunLong));

		for (int i = 0; i <= sunCircleResolution; i++) // around entire circle
		{
			double bearing = -double(direction * i) / double(sunCircleResolution) * PI2 + bearingToEdge; // Add an offset so that first bearing is top part of screen, and last bearing is bottom part of screen

			double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));

			if (i == 0) latPoint += 1e-5; // force point to lie exactly on the top side
			else if (i == sunCircleResolution) latPoint -= 1e-5; // force point to lie exactly on the bottom side

			double longPoint = sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint));

			TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

			sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
			sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

			if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

			if (debugInformation) // show index at vertices
			{
				char debugLabel[4];
				sprintf(debugLabel, "%i", fillElements);
				skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
			}

			fillElements++;
		}

		// And now add the edge coordinates
		double edgeLongitude;
		if (direction == 1) // go right
			edgeLongitude = centreLong + PI05 + 1e-4;
		else // go left
			edgeLongitude = centreLong - PI05 - 1e-4;

		// Bottom edge, either left or right
		TransformPoint(edgeLongitude, -1e-8, &transformedLong, &transformedLat, proj);
		sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
		sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

		if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

		if (debugInformation) // show index at vertices
		{
			char debugLabel[4];
			sprintf(debugLabel, "%i", fillElements);
			skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
		}
		fillElements++;

		// Top edge, either left or right
		TransformPoint(edgeLongitude, +1e-8, &transformedLong, &transformedLat, proj);
		sunFill[fillElements].x = int(W / 2 + transformedLong / PI * W / 2);
		sunFill[fillElements].y = int(H / 2 - transformedLat / PI05 * W / 4);

		if (completelySunFilled && (0 < sunFill[fillElements].x && sunFill[fillElements].x < W) && (0 < sunFill[fillElements].y && sunFill[fillElements].y < H)) completelySunFilled = false;

		if (debugInformation) // show index at vertices
		{
			char debugLabel[4];
			sprintf(debugLabel, "%i", fillElements);
			skp->Text(sunFill[fillElements].x, sunFill[fillElements].y, debugLabel, strlen(debugLabel));
		}
		fillElements++;

		skp->SetBrush(sunlitSide);
		skp->SetPen(NULL); // add terminator line later
		if (completelySunFilled) // there are no vertices on screen.
		{
			char debugLabel[4];
			if (oapiOrthodome(centreLong, centreLat, sunLong, sunLat) < sunAngleView) // within sunView, i.e. entire displayed map is lighted up.
			{
				// Omit the flickering problem, and just fill with light
				oapi::IVECTOR2 sunSquare[4];
				sunSquare[0].x = 0 + 1; sunSquare[0].y = 0 + 1;
				sunSquare[1].x = W - 1; sunSquare[1].y = 0 + 1;
				sunSquare[2].x = W - 1; sunSquare[2].y = H - 1;
				sunSquare[3].x = 0 + 1; sunSquare[3].y = H - 1;
				skp->Polygon(sunSquare, 4);

				sprintf(debugLabel, "Simple fill square light");
			}
			else sprintf(debugLabel, "Simple fill square dark"); // All dark, don't plot at all

			if (debugInformation) skp->Text(W / 2, H / 2 + 20, debugLabel, strlen(debugLabel)); // display info
		}
		else skp->Polygon(sunFill, fillElements); // normal plotting, as we have a complex polygon view

		break;
	default:
		break;
	}

	// Add the terminator line
	//// Longitude of left edge of map
	//double leftmostLongitude = normangle(centreLong - PI); // switch-case doesn't allow for declaring with value, so first declare, then assign
	//double leftmostTerminatorLatitude = atan(tan(sunLat + PI05) * sin(leftmostLongitude - sunLong + PI05)); // source: https://math.stackexchange.com/questions/804301/what-is-the-approximation-equation-for-making-the-day-night-wave for inspiration, and some playing/thinking in GeoGebra
	//double bearingToEdge = atan2(sin(leftmostLongitude - sunLong) * cos(leftmostTerminatorLatitude), cos(sunLat) * sin(leftmostTerminatorLatitude) - sin(sunLat) * cos(leftmostTerminatorLatitude) * cos(leftmostLongitude - sunLong));
	//int direction = 1;
	//if (sunLat < 0.0) direction = -1; // switch direction at equinox, so that points are always plotted from left to right, coming back left at one of the poles.

	skp->SetPen(terminatorLine);
	double previousLo, previousLa;

	for (int i = 0; i <= sunCircleResolution; i++) // around entire circle, and <= instead of < to get the same point as leftmost, just on the right side.
	{
		double bearing = -double(direction * i) / double(sunCircleResolution) * PI2 + bearingToEdge;

		double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));
		double longPoint = normangle(sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint)));

		if (i == 0)
		{
			longPoint += 1e-5; // force point to be leftmost of map
			latPoint += 1e-5; // force point to be top of map (for transverse maps)
		}
		else
		{
			if (i == sunCircleResolution)
			{
				longPoint -= 1e-5; // force point to be rightmost of map
				latPoint -= 1e-5; // force point to be bottom of map (for transverse maps)
			}
			DrawLine(previousLo, previousLa, longPoint, latPoint, skp, false);
		}

		previousLo = longPoint;
		previousLa = latPoint;
	}
}

void MapMFD::MakeGridLines(oapi::Sketchpad* skp)
{
	skp->SetPen(gridLines);

	// Draw encircling shape / border for the map graphic, but only for non-rectangular maps with poles on the sides (so called pseudocylindrical projections).
	switch (proj)
	{
	case EQUIRECTANGULAR:
	//case MILLER:
	case MERCATOR:
	case TRANSVERSEMERCATOR:
	case GALLPETERS:
	//case GNOMONIC: // gnomonic and stereographic are actually in same class as azimuthal ones, but they have an infinite span, so we never have a border to display
	case STEREOGRAPHIC: // stereographic is actually in same class as azimuthal ones, but it has an infinite span, so we never have a border to display
		break;
	case VANDERGRINTEN:
	case ROBINSON:
	case EQUALEARTH:
	case MOLLWEIDE:
	case HAMMER:
	case LOXIMUTHAL:
	case LASKOWSKI:
	//case ORTELIUSOVAL:
	case WINKELTRIPEL:
	//case RECTANGULARPOLYCONIC:
		double decrementValue;
		decrementValue = 0.999;
		for (int i = -180; i < 180; i += gridResolution)
		{
			//double lo = i * 0.999;
			DrawLine(i * RAD + centreLong, -PI05 * decrementValue, i * RAD + gridResolution * RAD + centreLong, -PI05 * decrementValue, skp, false); // bottom
			DrawLine(i * RAD + centreLong, PI05 * decrementValue, i * RAD + gridResolution * RAD + centreLong, PI05 * decrementValue, skp, false); // top
		}

		for (int i = -90; i < 90; i += gridResolution)
		{
			//double la = i * 0.999;
			DrawLine(-PI * decrementValue + centreLong, i * RAD, -PI * decrementValue + centreLong, i * RAD + gridResolution * RAD, skp, false); // left
			DrawLine(PI * decrementValue + centreLong, i * RAD, PI * decrementValue + centreLong, i * RAD + gridResolution * RAD, skp, false); // right
		}

		break;
	case AZIMUTHALEQUIDISTANT:
	case LAMBERTAZIMUTHAL:
		// These projections are so-called azimuthal, so the boundary is the great-circle 180 deg distance from the centre point.
		// The encircling shape is thus a 180 deg angular distance for all bearings from centreLong/centreLat, much like the "viewable content"-circle and terminator line.
		decrementValue = 0.999;
		double firstLong, firstLat, previousLong, previousLat;
		for (int i = 0; i <= 360; i += gridResolution)
		{
			double bearing = double(i) * RAD;

			double latPoint = asin(sin(centreLat) * cos(PI * decrementValue) + cos(centreLat) * sin(PI * decrementValue) * cos(bearing));
			double longPoint = normangle(centreLong + atan2(sin(bearing) * sin(PI * decrementValue) * cos(centreLat), cos(PI * decrementValue) - sin(centreLat) * sin(latPoint)));

			if (i != 0)
			{
				DrawLine(previousLong, previousLat, longPoint, latPoint, skp);
			}
			else
			{
				firstLong = longPoint;
				firstLat = latPoint;
			}

			previousLat = latPoint;
			previousLong = longPoint;
		}
		DrawLine(previousLong, previousLat, firstLong, firstLat, skp);

		break;
	default:
		sprintf(oapiDebugString(), "ERROR! Default in MakeGridLines! %.2f", oapiGetSimTime());
		break;
	}

	// Draw grid lines for map
	// Longitude lines (vertical)
	for (int lo = -180; lo < 180; lo += gridAngleSeparation) // for every longitude
	{
		for (int k = 0; k < 180 / gridResolution; k++) // draw latitude meridian
		{
			int la = -90 + k * gridResolution;
			if (!((proj == TRANSVERSEMERCATOR) && abs(normangle(lo * RAD - centreLong)) > PI05 && (la == -gridResolution))) // line crossing equator more than 90 deg from centreLong in Cassini and Transverse Mercator moves over entire display, so don't display that.
				DrawLine(lo * RAD, la * RAD, lo * RAD, la * RAD + gridResolution * RAD, skp, false);
		}
	}
	// Latitude lines (horizontal)
	for (int la = -90; la <= 90; la += gridAngleSeparation)
	{
		for (int k = 0; k < 360 / gridResolution; k++)
		{
			int lo = -180 + k * gridResolution;
			if (!(normangle(lo * RAD - centreLong) > 0.0 && normangle(lo * RAD + gridResolution * RAD - centreLong) < 0.0) || proj == AZIMUTHALEQUIDISTANT || proj == LAMBERTAZIMUTHAL || proj == STEREOGRAPHIC || proj == TRANSVERSEMERCATOR) // line always propagates right, so if line crosses screen, then don't plot. Unless azimuthal/transverse plot.
				DrawLine(lo * RAD, la * RAD, lo * RAD + gridResolution * RAD, la * RAD, skp, false);
		}
	}
}

void MapMFD::MakeMap(oapi::Sketchpad* skp, const char *refName, int *txtPos)
{
	int textPos = *txtPos;
	char cbuf[100];

	if (updateCache)
	{
		char filePath[100];
		//bool successfullyFoundVector = false;
		sprintf(filePath, "%s\\Data\\coast.vec", refName);
		FILEHANDLE vectorFile = oapiOpenFile(filePath, FILE_IN_ZEROONFAIL, CONFIG);
		int fileMode = 0;
		if (vectorFile == 0)
		{
			sprintf(filePath, "%s\\Data\\contour.vec", refName);
			vectorFile = oapiOpenFile(filePath, FILE_IN_ZEROONFAIL, CONFIG);
			fileMode = 1;
		}

		if (debugInformation)
		{
			skp->SetFont(GetDefaultFont(0));
			skp->SetTextAlign();
			sprintf(cbuf, "Mode: %i, handle: %i", fileMode, (int)vectorFile);
			skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
			textPos++;
		}

		if (vectorFile != 0)
		{
			// Cache settings
			mapExists = true;

			// Other stuff
			//successfullyFoundVector = true;
			char* line;

			int lineNr = 0;
			// Get total number of vertices (first line of vec), to later ensure successful aqusition.
			oapiReadScenario_nextline(vectorFile, line); // We're not using a scenario, but Orbiter does not have a oapiReadFile function, so this will have to suffice. Thank you to indy91 for the method, from https://www.orbiter-forum.com/showthread.php?t=40452
			int totalVertices = atoi(line);
			cacheMap[lineNr][0] = (float)totalVertices;
			lineNr++;

			// Get total number of segments, so that we know how long to iterate
			oapiReadScenario_nextline(vectorFile, line);
			int totalSegments = atoi(line);
			cacheMap[lineNr][0] = (float)totalSegments;
			lineNr++;

			// Set the ratio of lines to plot
			int skipLineFactor = skipEveryNLines; // copy setting from config screen

			if (autoResolution) // instead control skipLineFactor automagically
			{
				skipLineFactor = int(totalVertices / defaultMapLinesAmount / centreZoom); // use int casting to floor result (according to stackoverflow user, it's faster)
				// Set defaultMapLinesAmount as arbitrary amount which is OK to plot
			}

			// Switch to pen for creating coast or contour lines
			if (fileMode == 0)
			{
				skp->SetPen(coastLines);
				coastMap = true;
			}
			else
			{
				skp->SetPen(contourLines);
				coastMap = false;
			}

			int vertexCounter = 0;
			for (int i = 0; i < totalSegments; i++)
			{
				oapiReadScenario_nextline(vectorFile, line);
				int numVerticesInSegment = atoi(line);
				cacheMap[lineNr][0] = (float)numVerticesInSegment;
				lineNr++;

				double previousLongitude, previousLatitude;
				double firstLongitude, firstLatitude;
				int numSegmentsDrawn = 0;

				for (int k = 0; k < numVerticesInSegment; k++)
				{
					// We're now getting all vertex data
					oapiReadScenario_nextline(vectorFile, line);
					char* strPos;
					strPos = strchr(line, ' ');
					double longitude = atof(line);
					double latitude = atof(line + int(strPos - line + 1));
					cacheMap[lineNr][0] = (float)longitude;
					cacheMap[lineNr][1] = (float)latitude;
					lineNr++;

					if ((k % (skipLineFactor + 1)) == 0)
					{
						// We'll work in radians here
						longitude *= RAD;
						latitude *= RAD;

						// No overflow, please.
						longitude = normangle(longitude);
						//latitude = normangle(latitude);

						// Draw segment
						if (k != 0) // must have two vertices
						{
							DrawLine(previousLongitude, previousLatitude, longitude, latitude, skp);
							numSegmentsDrawn += 1;
						}
						else
						{
							firstLongitude = longitude;
							firstLatitude = latitude;
						}

						previousLongitude = longitude;
						previousLatitude = latitude;
					}

					vertexCounter++;
				}

				// We're now finished making one segment. Connect loose ends.
				if (numSegmentsDrawn > 1 && (previousLatitude != firstLatitude || previousLongitude != firstLongitude)) // a real object, and an actual loose end
				{
					DrawLine(previousLongitude, previousLatitude, firstLongitude, firstLatitude, skp);
				}
			}

			char* strPos;
			strPos = strchr(line, ' ');
			double longitude = atof(line);
			double latitude = atof(line + int(strPos - line + 1));

			if (debugInformation)
			{
				skp->SetFont(GetDefaultFont(0));
				skp->SetTextAlign();
				sprintf(cbuf, "Last string: %s", line);
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				sprintf(cbuf, "Last long: %.2f\u00B0, last lat: %.2f\u00B0", longitude, latitude);
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				if (vertexCounter == totalVertices)
				{
					sprintf(cbuf, "Read %i of %i vertices.", vertexCounter, totalVertices);;
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;

					sprintf(cbuf, "Success!");
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
				else
				{
					sprintf(cbuf, "Read %i of %i vertices.", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;

					sprintf(cbuf, "Failed!");
					skp->Text(textDX, textDY* textPos, cbuf, strlen(cbuf));
					textPos++;
				}
			}
		}
		else mapExists = false;

		oapiCloseFile(vectorFile, FILE_IN_ZEROONFAIL); // close file
		updateCache = false;
	}
	else
	{
		if (mapExists)
		{
			//char* line;
			int lineNr = 0;

			// Get total number of vertices (first line of vec), to later ensure successful aqusition.
			//line = cacheMap[lineNr];
			//int totalVertices = atoi(line);
			int totalVertices = int(cacheMap[lineNr][0]);
			lineNr++;

			// Get total number of segments, so that we know how long to iterate
			//line = cacheMap[lineNr];
			int totalSegments = int(cacheMap[lineNr][0]);// atoi(line);
			lineNr++;

			// Set the ratio of lines to plot
			int skipLineFactor = skipEveryNLines; // copy setting from config screen

			if (autoResolution) // instead control skipLineFactor automagically
			{
				skipLineFactor = int(totalVertices / defaultMapLinesAmount / centreZoom); // use int casting to floor result (according to stackoverflow user, it's faster)
				// Set defaultMapLinesAmount as arbitrary amount which is OK to plot
			}

			// Switch to pen for creating coast or contour lines
			if (coastMap) skp->SetPen(coastLines);
			else skp->SetPen(contourLines);

			int vertexCounter = 0;
			for (int i = 0; i < totalSegments; i++)
			{
				//line = cacheMap[lineNr];
				int numVerticesInSegment = int(cacheMap[lineNr][0]); //atoi(line);
				lineNr++;

				double previousLongitude, previousLatitude;
				double firstLongitude, firstLatitude;
				int numSegmentsDrawn = 0;

				for (int k = 0; k < numVerticesInSegment; k++)
				{
					// We're now getting all vertex data
					//line = cacheMap[lineNr];
					double longitude = cacheMap[lineNr][0];
					double latitude = cacheMap[lineNr][1];
					lineNr++;

					if ((k % (skipLineFactor + 1)) == 0)
					{
						// We'll work in radians here
						longitude *= RAD;
						latitude *= RAD;

						// No overflow, please.
						longitude = normangle(longitude);
						//latitude = normangle(latitude);

						// Draw segment
						if (k != 0) // must have two vertices
						{
							DrawLine(previousLongitude, previousLatitude, longitude, latitude, skp);
							numSegmentsDrawn += 1;
						}
						else
						{
							firstLongitude = longitude;
							firstLatitude = latitude;
						}

						previousLongitude = longitude;
						previousLatitude = latitude;
					}

					vertexCounter++;
				}

				// We're now finished making one segment. Connect loose ends.
				if (numSegmentsDrawn > 1 && (previousLatitude != firstLatitude || previousLongitude != firstLongitude)) // a real object, and an actual loose end
				{
					DrawLine(previousLongitude, previousLatitude, firstLongitude, firstLatitude, skp);
				}
			}

			if (debugInformation)
			{
				skp->SetFont(GetDefaultFont(0));
				skp->SetTextAlign();
				sprintf(cbuf, "Using cache!");
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				if (vertexCounter == totalVertices)
				{
					sprintf(cbuf, "Read %i of %i vertices.", vertexCounter, totalVertices);;
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;

					sprintf(cbuf, "Success!");
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
				else
				{
					sprintf(cbuf, "Read %i of %i vertices.", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;

					sprintf(cbuf, "Failed!");
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
			}
		}
	}

	*txtPos = textPos; // update counter
}

void MapMFD::MakeMarkers(oapi::Sketchpad* skp, const char* refName)
{
	skp->SetPen(markerPen[1]); // give a default marker (stock Map MFD defaults to 1)
	skp->SetTextColor(DEFAULT_COLOURS.MARKER[1]); // give default colour
	skp->SetTextAlign(); // default text alignement (left, top)

	// As the marker file(s) are in the same config structure as the coast/contour map itself, we reuse much of the same code as in the MakeMap() function.

	// Iterate over all files in Marker folder, using std::filesystem (https://stackoverflow.com/a/37494654).
	// If file name is in EnabeledMarkers, then draw markers.


	//if (__cplusplus == 201703L) sprintf(oapiDebugString(), "C++17");
	//else if (__cplusplus == 201402L) sprintf(oapiDebugString(), "C++14"); // <-- we are here, so must use experimental folder library, as it's in C++17.
	//else if (__cplusplus == 201103L) sprintf(oapiDebugString(), "C++11");
	//else if (__cplusplus == 199711L) sprintf(oapiDebugString(), "C++98");
	//else sprintf(oapiDebugString(), "Pre-standard C++");

	char cbuf[200];

	char folderPath[100];
	sprintf(folderPath, "Config\\%s\\Marker", refName);
	const int TOTAL_MARKERS_THIS_PLANET = 50;
	char availableMarkers[TOTAL_MARKERS_THIS_PLANET][200]; // allow 50 markers. No name should be longer than 200 characters.
	int markersInFolder = 0;

	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		std::string myString{ entry.path().u8string() };
		sprintf(availableMarkers[markersInFolder], myString.c_str());
		markersInFolder++;
	}

	int debugMarkersDisplaying = 0;
	if (markersInFolder > TOTAL_MARKERS_THIS_PLANET)
	{
		sprintf(oapiDebugString(), "ERROR! Total %i markers, exceeds limit of %i. %.2f", markersInFolder, TOTAL_MARKERS_THIS_PLANET, oapiGetSimTime());
	}
	else if (markersInFolder > 0)
	{
		//sprintf(oapiDebugString(), "Marker folder %s. Marker files: %i, first <%s>", folderPath, markersInFolder, availableMarkers[0]);

		// Go through all available markers, and check if they are in the ones selected to display.
		for (int i = 0; i < markersInFolder; i++)
		{
			for (int j = 0; j < TOTAL_MARKERS_ALLOWED_TO_BE_ENABLED; j++)
			{
				if (strcmp(availableMarkers[i], enabledMarkers[j]) == 0) // we want to display one of the markers.
				{
					char* markerPath = availableMarkers[i] + 7; // +7 to remove "Config\", which is 7 characters.
					FILEHANDLE markerFile = oapiOpenFile(markerPath, FILE_IN_ZEROONFAIL, CONFIG);
					if (markerFile != 0) // could open marker file. Now read and draw.
					{
						int lineNr = 0;
						int markersDrawn = 0;
						char* line;

						bool reading = true;
						bool dataStarted = false; // file opens with comments and metadata, which we don't parse.

						while (reading)
						{
							reading = oapiReadScenario_nextline(markerFile, line);
							
							if (!_strnicmp(line, "ColourIdx", 9))
							{
								int colourIdx = atoi(line + 9);

								if (0 <= colourIdx && colourIdx <= 5) // valid number. 
								{
									skp->SetPen(markerPen[colourIdx]);
									skp->SetTextColor(DEFAULT_COLOURS.MARKER[colourIdx]);
								}
							}
							else if (strcmp(line, "BEGIN_DATA") == 0) dataStarted = true;
							else if (dataStarted) // the lines after "BEGIN_DATA"
							{
								if (strlen(line) > 5) // line may be empty, so don't do anything
								{
									// Line has format "long   lat  : name"
									double markerLong = atof(line) * RAD; // first comes longitude, which is in degrees, so convert to radians.
									char* spaceLoc = strchr(line, ' ');
									if (int(spaceLoc - line) > 14 || spaceLoc == NULL) // for some reason, some marker files use tabs, and some spaces. So if no early space is found, search for tabs. 15 is min length of a tab-input with spaces "+0.00\t+0.00\t:\tA A"
									{
										spaceLoc = strchr(line, '\t');
									}

									double markerLat = atof(line + int(spaceLoc - line + 1)) * RAD; // "  lat  : name"
									char* colonLoc = strchr(line, ':');
									char* markerLabel = line + int(colonLoc - line + 2); // "name"

									int markerSize = 3;
									if (centreZoom < 16) markerLabel = "";
									if (centreZoom < 4) markerSize = 1;

									DrawFeature(markerLong, markerLat, markerSize, RINGS, skp, markerLabel);
									markersDrawn++;

									if (debugInformation)
									{
										sprintf(cbuf, "<%s> %.1f, %.1f, <%s>", line, markerLong * DEG, markerLat * DEG, markerLabel);
										skp->Text(120, 40 + markersDrawn * 30, cbuf, strlen(cbuf));
									}
								}
							}


							lineNr++;
						}
					}

					oapiCloseFile(markerFile, FILE_IN_ZEROONFAIL); // close file

					debugMarkersDisplaying++;
					if (debugInformation)
					{
						sprintf(cbuf, "%iDraw marker %s, handle: %i", debugMarkersDisplaying, availableMarkers[i], int(markerFile));
						skp->Text(80, 30 + 30 * debugMarkersDisplaying, cbuf, strlen(cbuf));
					}
					break; // no possibility that a marker is enabled twice, so stop innermost for loop, and go to next marker in folder.
				}

			}
		}
	}
	else
	{
		//sprintf(oapiDebugString(), "Marker folder %s. No markers present!", folderPath);
	}
}

void MapMFD::MakeSurfaceBasesAndVessels(oapi::Sketchpad* skp)
{
	// Draw bases
	skp->SetPen(baseBox);
	skp->SetTextColor(DEFAULT_COLOURS.BASE);
	skp->SetTextAlign(); // default text alignement (left, top)
	for (int i = 0; i < (int)oapiGetBaseCount(ref); i++)
	{
		double baseLatitude, baseLongitude;
		OBJHANDLE baseRef = oapiGetBaseByIndex(ref, i);
		oapiGetBaseEquPos(baseRef, &baseLongitude, &baseLatitude);
		if (centreZoom >= 8)
		{
			char label[50];
			oapiGetObjectName(baseRef, label, 45);
			DrawFeature(baseLongitude, baseLatitude, 3, BOX, skp, label);
		}
		else
		{
			DrawFeature(baseLongitude, baseLatitude, 3, BOX, skp, "");
		}
	}

	// Draw all vessels at planet (if user wants it)
	if (showVessels)
	{
		for (int i = 0; i < (int)oapiGetVesselCount(); i++)
		{
			OBJHANDLE vesselH = oapiGetVesselByIndex(i);
			VESSEL* veI = oapiGetVesselInterface(vesselH);

			if (veI->GetSurfaceRef() == ref && !ObjectAlreadyInTarget(vesselH))
			{
				double objLong, objLat, objRad;
				GetObjectEquPos(vesselH, &objLong, &objLat, &objRad);

				// Fetch object name
				char nameString[50];
				oapiGetObjectName(vesselH, nameString, 45);

				// Finally, draw current position, so that it's on top.
				skp->SetPen(targetPosition);
				skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
				skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
				DrawFeature(objLong, objLat, 10, CROSS, skp, nameString);
			}
		}
	}
}

void MapMFD::MakeTargets(oapi::Sketchpad* skp, double currentLong, double currentLat, int *infoLinesDrwn)
{
	char cbuf[100];
	int infoLinesDrawn = *infoLinesDrwn;

	// Draw orbit track of targets
	//int infoLinesDrawn = 0; // this is instead handeled in MapScreen (the parent function)
	for (int i = 0; i < numTargets; i++)
	{
		int type = oapiGetObjectType(targets[i]); // the object type we have

		if (type == OBJTP_VESSEL || type == OBJTP_PLANET || type == OBJTP_STAR)
		{
			ELEMENTS el;
			ORBITPARAM prm;

			GetObjectRelativeElements(targets[i], el, &prm);

			double currentLongTarget, currentLatTarget, currentRadTarget;
			GetObjectEquPos(targets[i], &currentLongTarget, &currentLatTarget, &currentRadTarget);

			skp->SetPen(targetOrbitTrack);

			DrawOrbitTrack(currentLongTarget, currentLatTarget, el, prm, skp); // target orbit track

			// Draw circle of "viewable content", i.e. "what portion of the planet is in view?".
			// Calculate the angular viewable area, and calculate points for all different bearings. From https://www.movable-type.co.uk/scripts/latlong.html
			double angleView = acos(refRad / (currentRadTarget - oapiSurfaceElevation(ref, currentLongTarget, currentLatTarget))); // subtract ground elevation, so that landed on a mountain will show zero view
			double firstLong = 0.0, firstLat = 0.0, previousLong = 0.0, previousLat = 0.0;
			skp->SetPen(targetGroundCoverageLine);
			for (int i = 0; i < viewCircleResolution; i++) // around entire circle with 6 deg steps
			{
				double bearing = double(i) / double(viewCircleResolution) * PI2;

				double latPoint = asin(sin(currentLatTarget) * cos(angleView) + cos(currentLatTarget) * sin(angleView) * cos(bearing));
				double longPoint = normangle(currentLongTarget + atan2(sin(bearing) * sin(angleView) * cos(currentLatTarget), cos(angleView) - sin(currentLatTarget) * sin(latPoint)));

				if (i != 0)
				{
					DrawLine(previousLong, previousLat, longPoint, latPoint, skp);
				}
				else
				{
					firstLong = longPoint;
					firstLat = latPoint;
				}

				previousLat = latPoint;
				previousLong = longPoint;
			}
			DrawLine(previousLong, previousLat, firstLong, firstLat, skp);

			// Fetch object name
			char nameString[50];
			oapiGetObjectName(targets[i], nameString, 45);

			// Finally, draw current position, so that it's on top.
			skp->SetPen(targetPosition);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
			skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
			DrawFeature(currentLongTarget, currentLatTarget, 10, CROSS, skp, nameString);
			skp->SetPen(targetOrbitTrack);
			DrawFeature(currentLongTarget, currentLatTarget, 6, RINGS, skp, "");
			DrawFeature(currentLongTarget, currentLatTarget, 10, RINGS, skp, "");

			char trueDist[20], orthoDist[20];
			// Find true distance to target object
			VECTOR3 tgtPos;
			v->GetRelativePos(targets[i], tgtPos);
			FormatValue(trueDist, 20, length(tgtPos));

			// Find ground distance.
			FormatValue(orthoDist, 20, oapiOrthodome(currentLongTarget, currentLatTarget, currentLong, currentLat) * refRad);

			// And also remember to draw target info
			char altStr[20];
			FormatValue(altStr, 20, currentRadTarget - refRad);
			skp->SetFont(GetDefaultFont(1));
			skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			sprintf(cbuf, "TGT: %s [%s, Alt%s, Dst%s(%s)]", nameString, GetCoordinateString(currentLongTarget, currentLatTarget), altStr, trueDist, orthoDist + 1); // FormatValue returns an annoying space in the beginning.
			if (int(skp->GetTextWidth(cbuf)) > W) // if text too wide to fit display
			{
				// We print from the bottom, so last part first
				sprintf(cbuf, "  Alt%s, Dst%s(%s)]", altStr, trueDist, orthoDist + 1);
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
				sprintf(cbuf, "TGT: %s [%s,", nameString, GetCoordinateString(currentLongTarget, currentLatTarget));
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
			}
			else
			{
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
			}
		}
		else if (type == OBJTP_SURFBASE)
		{
			double longi, lati;
			oapiGetBaseEquPos(targets[i], &longi, &lati);
			skp->SetPen(targetOrbitTrack);
			DrawFeature(longi, lati, 6, RINGS, skp, "");
			DrawFeature(longi, lati, 10, RINGS, skp, "");

			char trueDist[20], orthoDist[20];
			// Find true distance to input coordinate
			VECTOR3 coordPos, vesselPos;
			v->GetRelativePos(ref, vesselPos); // first get vector to planet centre.
			vesselPos = Ecl2Equ(vesselPos); // then convert to equatorial.
			coordPos = Coord2Vector(longi, lati);
			FormatValue(trueDist, 20, length(vesselPos - coordPos));

			// Find ground distance. Luckily, that is easier.
			FormatValue(orthoDist, 20, oapiOrthodome(longi, lati, currentLong, currentLat) * refRad);

			// Bearing to base
			double baseBearing = atan2(sin(longi - currentLong) * cos(lati), cos(currentLat) * sin(lati) - sin(currentLat) * cos(lati) * cos(longi - currentLong));
			baseBearing = posangle(baseBearing); // 0 ... 360 deg, reflecting information on surface HUD

			// And also remember to draw target info
			char nameStr[20];
			oapiGetObjectName(targets[i], nameStr, 20);
			skp->SetFont(GetDefaultFont(1));
			skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			sprintf(cbuf, "BSE: %s [%s, Dst%s(%s), Brg %05.1f\u00B0]", nameStr, GetCoordinateString(longi, lati), trueDist, orthoDist + 1, baseBearing * DEG);
			if (int(skp->GetTextWidth(cbuf)) > W)
			{
				// Draw from the bottom, so last part first
				sprintf(cbuf, "  Dst%s(%s), Brg %05.1f\u00B0]", trueDist, orthoDist + 1, baseBearing * DEG);
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
				sprintf(cbuf, "BSE: %s [%s,", nameStr, GetCoordinateString(longi, lati));
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
			}
			else
			{
				skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
				infoLinesDrawn++;
			}
		}
		else if (targets[i] == NULL) // user specified coordinate
		{
			double longiDeg, latiDeg, longi, lati;
			longiDeg = targetCoord[i][0];
			latiDeg = targetCoord[i][1];
			longi = normangle(longiDeg * RAD);
			lati = latiDeg * RAD;
			skp->SetPen(targetOrbitTrack);
			DrawFeature(longi, lati, 6, RINGS, skp, "");
			DrawFeature(longi, lati, 10, RINGS, skp, "");

			// And also remember to draw target info
			char trueDist[20], orthoDist[20];

			// Find true distance to input coordinate
			VECTOR3 coordPos, vesselPos;
			v->GetRelativePos(ref, vesselPos); // first get vector to planet centre.
			vesselPos = Ecl2Equ(vesselPos); // then convert to equatorial.
			coordPos = Coord2Vector(longi, lati); // get the vector from centre to coordinate
			FormatValue(trueDist, 20, length(vesselPos - coordPos));

			// Find ground distance. Luckily, this is easier.
			FormatValue(orthoDist, 20, oapiOrthodome(longi, lati, currentLong, currentLat) * refRad);

			// Bearing to base
			double baseBearing = atan2(sin(longi - currentLong) * cos(lati), cos(currentLat) * sin(lati) - sin(currentLat) * cos(lati) * cos(longi - currentLong));
			baseBearing = posangle(baseBearing); // 0 ... 360 deg, reflecting information on surface HUD

			// Write info
			sprintf(cbuf, "COOR: [%s, Dst%s(%s), Brg %05.1f\u00B0]", GetCoordinateString(longi, lati), trueDist, orthoDist + 1, baseBearing * DEG);
			skp->SetFont(GetDefaultFont(1));
			skp->SetTextColor(DEFAULT_COLOURS.TARGETTRACK);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
			infoLinesDrawn++;
		}
	}

	*infoLinesDrwn = infoLinesDrawn;
}

void MapMFD::MakeShip(oapi::Sketchpad* skp, double currentLong, double currentLat, double currentRad, int *infoLinesDrwn)
{
	double simt = oapiGetSimTime(); // sim time
	char cbuf[100];
	int infoLinesDrawn = *infoLinesDrwn;

	// Draw ship historic track, but only if in ground track view
	if (orbitTrackGround && showHistory)
	{
		if (shipHistoryLength == 0) // first fill with one value
		{
			shipHistory[shipHistoryIndex][0] = currentLong;
			shipHistory[shipHistoryIndex][1] = currentLat;
			shipHistory[shipHistoryIndex][2] = simt;
			shipHistoryLength = 1; // i.e. ++;
		}

		const double historyTimeDelta = 10.0;
		if (simt > shipHistory[shipHistoryIndex][2] + historyTimeDelta && !(v->GroundContact()))
		{
			shipHistoryIndex = (shipHistoryIndex + 1) % GROUND_TRACK_HISTORY_SIZE;

			shipHistory[shipHistoryIndex][0] = currentLong;
			shipHistory[shipHistoryIndex][1] = currentLat;
			shipHistory[shipHistoryIndex][2] = simt;

			if (shipHistoryLength < GROUND_TRACK_HISTORY_SIZE) shipHistoryLength++;
		}

		skp->SetPen(mainOrbitTrackHistory);
		for (int i = 0; i < shipHistoryLength; i++) // don't draw around in circle, therefore subtract one (e.g. 4 segments from 5 points)
		{
			if (i == 0) // Draw from currPos to last save
			{
				DrawLine(shipHistory[shipHistoryIndex][0], shipHistory[shipHistoryIndex][1], currentLong, currentLat, skp);
			}
			else
			{
				int indexPrevious = (shipHistoryIndex - i + GROUND_TRACK_HISTORY_SIZE) % GROUND_TRACK_HISTORY_SIZE;
				int indexCurrent = (shipHistoryIndex - i + 1 + GROUND_TRACK_HISTORY_SIZE) % GROUND_TRACK_HISTORY_SIZE;

				if (shipHistory[indexPrevious][2] > simt - (GROUND_TRACK_HISTORY_SIZE * historyTimeDelta)) // only draw last 1e4 seconds (if time acc, we may have older saves)
					DrawLine(shipHistory[indexPrevious][0], shipHistory[indexPrevious][1], shipHistory[indexCurrent][0], shipHistory[indexCurrent][1], skp);
			}

		}
	}

	// Draw orbit track
	ELEMENTS el;
	ORBITPARAM prm;
	//GetObjectRelativeElements(, el, &prm);
	v->GetElements(ref, el, &prm, 0.0, FRAME_EQU);

	skp->SetPen(mainOrbitTrack);
	DrawOrbitTrack(currentLong, currentLat, el, prm, skp); // main orbit track

	// Draw marker for a pre-determined altitude. Useful if you want to know where you go below e.g. 60 km during reentry.
	double radiusToDraw = drawSpecificAlt + refRad;
	if (orbitTrackGround && drawSpecificAlt != 0.0 && radiusToDraw > prm.PeD) // in orbit track, have set an altitude, and it is above perigee
	{
		if (el.e > 1.0 || (el.e < 1.0 && radiusToDraw < prm.ApD)) // hyperbolic, or elliptical orbit and within Pe and Ap (Pe check above)
		{
			// The TrA at where the intersections occur
			double TrAatAlt1 = acos((el.a / radiusToDraw * (1.0 - el.e * el.e) - 1.0) / el.e);
			//if (el.e > 1.0) TrAatAlt1 = acos((el.a / radiusToDraw * (el.e * el.e - 1.0) - 1.0) / el.e);
			double TrAatAlt2 = PI2 - TrAatAlt1; // happens two times per orbit (up and down crossing)

			// The MnA at where the intersections occur
			double futureMnA1 = TrA2MnA(TrAatAlt1, el.e);
			double futureMnA2 = TrA2MnA(TrAatAlt2, el.e);

			if (el.e < 1.0)
			{
				// Make them happen in the future
				if (futureMnA1 < prm.MnA) futureMnA1 += PI2;
				if (futureMnA2 < prm.MnA) futureMnA2 += PI2;
			}

			double meanMotion = PI2 / prm.T;
			if (el.e > 1.0) meanMotion = sqrt(refMu / pow(-el.a, 3));
			double timeToThat1 = (futureMnA1 - prm.MnA) / meanMotion;
			double timeToThat2 = (futureMnA2 - prm.MnA) / meanMotion;

			// Draw info
			double altLong, altLat;
			skp->SetTextColor(DEFAULT_COLOURS.MAINTRACK);
			char altChar[20];
			FormatValue(altChar, 20, drawSpecificAlt);
			// First occurance:
			if (timeToThat1 > 0.0 && !(prm.PeT < timeToThat1 && prm.PeD < refRad)) // in the future (hyperbolic), and don't display the upgoing point if hitting the ground first
			{
				GetEquPosInXSeconds(timeToThat1, el, prm, currentLong, &altLong, &altLat);
				DrawFeature(altLong, altLat, 5, BOX, skp, altChar);
			}
			// And second occurrance:
			if (timeToThat2 > 0.0 && !(prm.PeT < timeToThat2 && prm.PeD < refRad)) // in the future (hyperbolic), and don't display the upgoing point if hitting the ground first
			{
				GetEquPosInXSeconds(timeToThat2, el, prm, currentLong, &altLong, &altLat);
				DrawFeature(altLong, altLat, 5, BOX, skp, altChar);
			}
		}
	}

	// Draw periapsis and apoapsis points in ground track (if they exist)
	if (orbitTrackGround && el.e < 1.0 && !(v->GroundContact()) && !(prm.PeD < refRad && posangle(prm.MnA) > PI)) // we have an apogee, and we're not suborbital AND past apogee
	{
		double apoLong, apoLat;
		GetEquPosInXSeconds(prm.ApT, el, prm, currentLong, &apoLong, &apoLat);
		skp->SetTextColor(DEFAULT_COLOURS.MAINTRACK);
		DrawFeature(apoLong, apoLat, 5, BOX, skp, "Ap");
	}

	if (orbitTrackGround && prm.PeD > refRad && (el.e < 1.0 || normangle(prm.TrA) < 0.0)) // we have an upcoming perigee that is above surface
	{
		double periLong, periLat;
		GetEquPosInXSeconds(prm.PeT, el, prm, currentLong, &periLong, &periLat);
		if (prm.PeT < 0.0) sprintf(oapiDebugString(), "ERROR! We're plotting perigee, but it has occured! Debug %.2f", simt);
		skp->SetTextColor(DEFAULT_COLOURS.MAINTRACK);
		DrawFeature(periLong, periLat, 5, BOX, skp, "Pe");
	}

	// Draw circle of "viewable content", i.e. "what portion of the planet is in view?".
	// Calculate the angular viewable area, and calculate points for all different bearings. From https://www.movable-type.co.uk/scripts/latlong.html
	double currentElevation = oapiSurfaceElevation(ref, currentLong, currentLat); // create variable, for use later in surface radar
	double angleView = acos(refRad / (currentRad - currentElevation)); // subtract ground elevation, so that landed on a mountain will show zero view
	double firstLong = 0.0, firstLat = 0.0, previousLong = 0.0, previousLat = 0.0;
	skp->SetPen(groundCoverageLine);
	for (int i = 0; i < viewCircleResolution; i++) // around entire circle with 6 deg steps
	{
		double bearing = double(i) / double(viewCircleResolution) * PI2;

		double latPoint = asin(sin(currentLat) * cos(angleView) + cos(currentLat) * sin(angleView) * cos(bearing));
		double longPoint = normangle(currentLong + atan2(sin(bearing) * sin(angleView) * cos(currentLat), cos(angleView) - sin(currentLat) * sin(latPoint)));

		if (i != 0)
		{
			DrawLine(previousLong, previousLat, longPoint, latPoint, skp);
		}
		else
		{
			firstLong = longPoint;
			firstLat = latPoint;
		}

		previousLat = latPoint;
		previousLong = longPoint;
	}
	DrawLine(previousLong, previousLat, firstLong, firstLat, skp);

	// This is used later, in ship info string, but create here, as I need it for adjusting radar after ship course.
	double courseDirection;
	VECTOR3 groundVec;
	v->GetGroundspeedVector(FRAME_HORIZON, groundVec);
	courseDirection = posangle(atan2(groundVec.x, groundVec.z));

	// Elevation fill, a kind of landing radar
	if (displayElevationRadar)
	{
		const int radialSteps = 10;
		const int angularSteps = 16;
		//double previousAngularElevation[angularSteps] = { -PI05 }; // save the previous surface elevation angle, so that we stop displaying if the outside point is behind the mountain in front
		const double metrePerColourStep = 5.0; // 10 m gives max range 1280 m in each axis
		skp->SetTextColor(0x00FF00); // for info text

		for (int i = 1; i < radialSteps + 1; i++) // radial iteration, starting from 1, as 0 is our location
		{
			double radialView = angleView / 2.0 * double(i) / (double(radialSteps) + 1.0); // +1 to not get up to border of angleView circle, but be within
			double radialViewOuter = angleView / 2.0 * (double(i) + 0.5) / (double(radialSteps) + 1.0); // 4-gon with this outer angular radius
			double radialViewInner = angleView / 2.0 * (double(i) - 0.5) / (double(radialSteps) + 1.0); // 4-gon with this inner angular radius

			for (int k = 0; k < angularSteps; k++) // angular iteration
			{
				double bearing = posangle(double(k) / double(angularSteps) * PI2 + courseDirection);
				double bearingRight = posangle(bearing - 0.5 * PI2 / angularSteps);
				double bearingLeft = posangle(bearing + 0.5 * PI2 / angularSteps);

				double latPoint = asin(sin(currentLat) * cos(radialView) + cos(currentLat) * sin(radialView) * cos(bearing));
				double longPoint = normangle(currentLong + atan2(sin(bearing) * sin(radialView) * cos(currentLat), cos(radialView) - sin(currentLat) * sin(latPoint)));

				double elevation = oapiSurfaceElevation(ref, longPoint, latPoint); // surface elevation

				int colourElev = 128 + int((elevation - currentElevation) / metrePerColourStep);

				if (colourElev < 0) colourElev = 0;
				else if (colourElev > 255) colourElev = 255;

				//DWORD elevColour = oapiGetColour(0, 255 - colourElev, colourElev); // use red for positive elevation (crash into mountain), green for negative elevation (good clearance)
				skp->SetPen(gradientRedGreen[colourElev]);
				//char debugElev[10];
				//sprintf(debugElev, "%i", colourElev);
				DrawFeature(longPoint, latPoint, 10, CROSS, skp, ""); // text doesn't scale nicely, so don't display any

			}
		}
	}

	if (v->GroundContact() && v->GetSurfaceRef() == ref) // heading doesn't work for non-surface ref.
	{
		skp->SetPen(mainOrbitTrack);
		//double headingLong, headingLat;
		double heading;
		oapiGetHeading(v->GetHandle(), &heading);
		double headingDistance = 1000 / refRad; // 1 km long arrow
		double headingLat = asin(sin(currentLat) * cos(headingDistance) + cos(currentLat) * sin(headingDistance) * cos(heading));
		double headingLong = currentLong + atan2(sin(heading) * sin(headingDistance) * cos(currentLat), cos(headingDistance) - sin(currentLat) * sin(headingLat));

		DrawLine(currentLong, currentLat, headingLong, headingLat, skp, false);
	}

	// Fianlly, draw current position, so that it's on top.
	skp->SetPen(mainPosition);
	skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
	skp->SetTextColor(DEFAULT_COLOURS.MAINTRACK);
	DrawFeature(currentLong, currentLat, 10, CROSS, skp, v->GetName());

	// And also remember to draw our own info
	char altStr[20];
	FormatValue(altStr, 20, currentRad - refRad);
	skp->SetFont(GetDefaultFont(1));
	skp->SetTextColor(DEFAULT_COLOURS.MAINTRACK);
	skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
	sprintf(cbuf, "SHP: %s [%s, Alt%s, Crs %05.1f\u00B0]", v->GetName(), GetCoordinateString(currentLong, currentLat), altStr, courseDirection * DEG);
	if (int(skp->GetTextWidth(cbuf)) > W) // if text too wide to fit display
	{
		// We draw from the bottom, so print last part first
		sprintf(cbuf, "  Alt%s, Crs %05.1f\u00B0]", altStr, courseDirection * DEG);
		skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
		infoLinesDrawn++;
		sprintf(cbuf, "SHP: %s [%s,", v->GetName(), GetCoordinateString(currentLong, currentLat));
		skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
		infoLinesDrawn++;
	}
	else
	{
		skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
		infoLinesDrawn++;
	}

	*infoLinesDrwn = infoLinesDrawn;
}

void MapMFD::BuildReferenceCache(void)
{
	totalPlanets = 0;
	PLANET planetBody[30];

	OBJHANDLE centralStar = oapiGetGbodyByIndex(0);

	for (int i = 0; i < (int)oapiGetGbodyCount(); i++)
	{
		OBJHANDLE currentBody = oapiGetGbodyByIndex(i);

		// Gravitational force from the Sun is greater than from the Earth for the Moon. But we all know Moon is orbiting Earth, so manually set parent. This is an inelegant solution, but I can't find any better way
		if (currentBody == oapiGetGbodyByName("Moon"))
		{
			OBJHANDLE parentHandle = oapiGetGbodyByName("Earth");

			for (int k = 0; k < i; k++)
			{
				if (planetBody[k].handle == parentHandle)
				{
					int moonIndex = planetBody[k].moonCount; // clean up code by giving this value a new (shorter) name

					planetBody[k].moonIndex[moonIndex] = i;
					planetBody[k].moonHandle[moonIndex] = currentBody;
					oapiGetObjectName(planetBody[k].moonHandle[moonIndex],
						planetBody[k].moonName[moonIndex],
						20);
					planetBody[k].moonCount++;
				}
			}

		}
		else
		{
			double parentForce = 0.0;
			OBJHANDLE parentHandle = centralStar; // initialise

			for (int k = 0; k < i; k++) // as the Gbody index is ordered in after mass in descending order, we only have to check up to our own index, as nothing orbits a lighter object
			{
				OBJHANDLE parentBody = oapiGetGbodyByIndex(k);
				double parentMass = oapiGetMass(parentBody);
				VECTOR3 parentPos;
				oapiGetRelativePos(currentBody, parentBody, &parentPos);
				double parentDist2 = length2(parentPos);
				double force = parentMass / parentDist2;
				if (parentForce < force) // Parent body is the body with largest gravitational force on selected object
				{
					parentHandle = parentBody;
					parentForce = force;
				}
			}

			if (parentHandle != centralStar) // parent is not central body (Sun)
			{
				for (int k = 0; k < i; k++)
				{
					if (planetBody[k].handle == parentHandle)
					{
						int moonIndex = planetBody[k].moonCount; // clean up code by giving this value a new (shorter) name

						planetBody[k].moonIndex[moonIndex] = i;
						planetBody[k].moonHandle[moonIndex] = currentBody;
						oapiGetObjectName(planetBody[k].moonHandle[moonIndex],
							planetBody[k].moonName[moonIndex],
							20);
						planetBody[k].moonCount++;
					}
				}
			}
			else // is planet
			{
				planetBody[totalPlanets].handle = currentBody;
				planetBody[totalPlanets].index = i;

				oapiGetObjectName(planetBody[totalPlanets].handle, planetBody[totalPlanets].name, 20);

				// We're orbiting the Sun. Find distance to it.
				VECTOR3 sunPos;
				oapiGetRelativePos(planetBody[totalPlanets].handle, centralStar, &sunPos);
				planetBody[totalPlanets].sunDist = length(sunPos); // this will list Neptune outside Pluto sometimes. Oh well, who cares.
				totalPlanets++;
			}
		}
	}

	// Sort noParent objects (planets) after distance, with moons under their respective parent.
	// Implement a sorting algorithm.
	for (int i = 0; i < totalPlanets; i++)
	{
		double ourDistance = planetBody[i].sunDist;
		int numberCloser = 0;
		for (int k = 0; k < totalPlanets; k++)
		{
			double objectDist = planetBody[k].sunDist;
			if (i != k && objectDist < ourDistance) numberCloser++; // put index higher
		}

		sortedPlanetsCache[numberCloser] = planetBody[i];

		// Also use the occation to add indicators to planets having moons
		if (sortedPlanetsCache[numberCloser].moonCount != 0)
		{
			strcat(sortedPlanetsCache[numberCloser].name, " >");
		}
	}
}

void MapMFD::SetNewReference(OBJHANDLE hRef)
{
	ref = hRef;
	refMu = oapiGetMass(ref) * GGRAV;
	refRad = oapiGetSize(ref);
	updateCache = true;
	shipHistoryLength = 0; // reset history as we have a new reference
}

// You have the obligation to pass a valid handle!
bool MapMFD::AddOrRemoveTarget(OBJHANDLE hRef)
{
	bool targetObjectExists = false;

	for (int i = 0; i < numTargets; i++)
	{
		if (hRef == targets[i]) // we already have it as a target, so remove it
		{
			targets[i] = NULL;
			targetObjectExists = true;
		}
		else if (targetObjectExists) // push the rest of the targets one step down
		{
			targets[i - 1] = targets[i];

			targetCoord[i - 1][0] = targetCoord[i][0];
			targetCoord[i - 1][1] = targetCoord[i][1];
		}
	}

	if (targetObjectExists) // we have found and deleted the target
	{
		numTargets -= 1; // reduce list count (do it here, as we must first iterate over all previous targets)
		return true; // successfully removed object from targets!
	}
	else // we want to set it as a new target
	{
		if (numTargets < 10)
		{
			targets[numTargets] = hRef;
			numTargets += 1;
			return true; // successsfully added object to targets!
		}
	}

	return false; // probably have already full target list
}

bool MapMFD::ObjectAlreadyInTarget(OBJHANDLE hRef)
{
	for (int i = 0; i < numTargets; i++)
	{
		if (hRef == targets[i]) return true; // found
	}

	return false; // did not find
}

bool MapMFD::AddOrRemoveMarker(const char* fileName)
{
	bool markerObjectExists = false;
	for (int i = 0; i < numMarkers; i++)
	{
		if (strcmp(fileName, enabledMarkers[i]) == 0) // we already have it enabled, so disable
		{
			sprintf(enabledMarkers[i], "");
			markerObjectExists = true;
		}
		else if (markerObjectExists) // push the rest of the markers one step down
		{
			sprintf(enabledMarkers[i - 1], enabledMarkers[i]);
		}
	}

	if (markerObjectExists) // we have found and disabled the marker
	{
		numMarkers -= 1; // reduce list count (do it here, as we must first iterate over all previous markers)
		return true; // successfully removed file from markers!
	}
	else // we want to set it as a new marker
	{
		if (numMarkers < TOTAL_MARKERS_ALLOWED_TO_BE_ENABLED)
		{
			sprintf(enabledMarkers[numMarkers], fileName);
			numMarkers += 1;
			return true; // successsfully added file to markers!
		}
	}

	return false; // probably have already full marker list
}

bool MapMFD::DrawLine(double long0, double lat0, double long1, double lat1, oapi::Sketchpad* skp, bool safetyCheck)
{
	double transformedLongitude0 = -PI05, transformedLatitude0 = -PI05 / 2.0, transformedLongitude1 = PI05, transformedLatitude1 = PI05 / 2.0;

	TransformPoint(long0, lat0, &transformedLongitude0, &transformedLatitude0, proj);
	TransformPoint(long1, lat1, &transformedLongitude1, &transformedLatitude1, proj);

	int pxLong0 = int(W / 2 + transformedLongitude0 / PI * W / 2);
	int pxLat0 = int(H / 2 - transformedLatitude0 / PI05 * W / 4);
	int pxLong1 = int(W / 2 + transformedLongitude1 / PI * W / 2);
	int pxLat1 = int(H / 2 - transformedLatitude1 / PI05 * W / 4);

	// Don't write over the MFD border, so if a line is one end inside and other outside, then truncate the outside to where it goes out of the MFD borders.
	bool point0inside = true, point1inside = true;
	if ((pxLong0 < 0 || pxLong0 > W) || (pxLat0 < 0 || pxLat0 > H)) point0inside = false; // point 0 outside
	if ((pxLong1 < 0 || pxLong1 > W) || (pxLat1 < 0 || pxLat1 > H)) point1inside = false; // point 1 outside

	// Finally, before plotting, ensure that the line is visible. We do this by finding the distance from the centre of the screen to the parametric line.
	// If the distance is more than the diagonal distance to the edge (hypotenuse), it must be completely outside, and not worth plotting.
	//double shortestDistanceLineToCentre = W * H / PI2 * abs(transformedLongitude0 * transformedLatitude1 - transformedLongitude1 * transformedLatitude0) / sqrt(H * H * pow(transformedLatitude0 - transformedLatitude1, 2.0) + W * W * pow(transformedLongitude1 - transformedLongitude0, 2.0));
	//bool lineOutsideScreen = shortestDistanceLineToCentre > sqrt(W * W / 4 + H * H / 4); // further distance than screen diagonal
	//
	//if (lineOutsideScreen)
	//{
	//	// Line is far away from screen. Don't plot.
	//	return false;
	//}
	//else
	//{
	//	// Line may be outside screen, but aligned towards centre, so that the condition above tricks us to believe that it is dislayed.
	//	// Now check if the line ever appears on the screen.
	//	/*double vertexToCentreDistance0 = sqrt(pow(pxLong0 - W / 2, 2) + pow(pxLat0 - H / 2, 2));
	//	double vertexToCentreDistance1 = sqrt(pow(pxLong1 - W / 2, 2) + pow(pxLat1 - H / 2, 2));*/
	//	

	if (!point0inside && !point1inside) // don't add safetycheck, as no line out of bounds is useful. I'm desciding that.
	{
		// Don't plot, as the two anchor points are outside of the screen.
		pointsOutside += 1;
		return false;
	}
	else if (safetyCheck && ((transformedLongitude0 - transformedLongitude1) * (transformedLongitude0 - transformedLongitude1) + (transformedLatitude0 - transformedLatitude1) * (transformedLatitude0 - transformedLatitude1)) > 90.0 * RAD * double(centreZoom)) // multiply by centreZoom, so that we don't delete zoomed objects
	{
		// Don't plot, as the line is too long to look good, unless so specified by disabling safetyCheck
		return false;
	}
	else
	{
		skp->Line(pxLong0, pxLat0, pxLong1, pxLat1);
		pointsInside += 1;
		return true; // success!
	}
}

bool MapMFD::DrawFeature(double longitude, double latitude, int size, MAPFEATURE feature, oapi::Sketchpad* skp, char *label)
{
	double transformedLongitude = 0.0, transformedLatitude = 0.0;

	TransformPoint(longitude, latitude, &transformedLongitude, &transformedLatitude, proj);

	int pxX = int(W / 2 + transformedLongitude / PI * W / 2);
	int pxY = int(H / 2 - transformedLatitude / PI05 * W / 4);

	skp->SetFont(mapObjectFont);
	skp->SetBrush(NULL); // make inner fill transparent

	// Label
	if (strlen(label) > 0) skp->Text(pxX, pxY, label, strlen(label));

	switch (feature)
	{
	case BOX:
		skp->Rectangle(pxX - size, pxY - size, pxX + size, pxY + size);
		return true;
	case CROSS:
		skp->Line(pxX - size, pxY, pxX + size, pxY);
		skp->Line(pxX, pxY - size, pxX, pxY + size);
		return true;
	case RINGS:
		skp->Ellipse(pxX - size, pxY - size, pxX + size, pxY + size);
		return true;
	}

	return false;
}

void MapMFD::DrawOrbitTrack(double currentLong, double currentLat, ELEMENTS el, ORBITPARAM prm, oapi::Sketchpad* skp)
{
	// Plot the orbit track of a vessel
	double futureLong, futureLat;
	double previousLong = currentLong, previousLat = currentLat; // initialise to current pos, in case first step is below ground.

	if (orbitTrackGround)
	{
		double timeLimit;
		if (el.e > 1.0) timeLimit = 1e6; // orbit period not defined for hyperbolic orbit
		else timeLimit = min(1e6, prm.T * orbitTrackOrbitsNumber); // 1e6 seconds equals 11.5 days.

		double stepTrA = prm.TrA;
		double time = 0.0;
		const double angleDelta = orbitTrackAngleDelta * RAD;
		const double maxTimeStep = oapiGetPlanetPeriod(ref) / maxPeriodFraction;

		int debugCountOversteps = 0;

		bool aboveSurface = true;
		int n = 0;

		while (aboveSurface && time < timeLimit && n < GROUND_TRACK_ITERATION_MAX_STEPS)
		{
			aboveSurface = GetEquPosInXSeconds(time, el, prm, currentLong, &futureLong, &futureLat);

			if (n > 0 && aboveSurface) // must be above surface, as we don't want to draw a line of it falling inside the planet
			{
				DrawLine(previousLong, previousLat, futureLong, futureLat, skp);

				previousLong = futureLong;
				previousLat = futureLat;
			}

			stepTrA += angleDelta;
			double futureMnA = TrA2MnA(stepTrA, el.e);
			if (el.e > 1.0) // hyperbolic
			{
				double meanMotion = sqrt(refMu / pow(-el.a, 3));
				double desiredTime = (futureMnA - prm.MnA) / meanMotion;

				if (desiredTime - time > maxTimeStep) // new time is too large step from previous time.
				{
					time += maxTimeStep;

					// And now set back stepTrA to what we have in time
					futureMnA = time * meanMotion + prm.MnA;
					stepTrA = MnA2TrA(futureMnA, el.e);

					debugCountOversteps++;
				}
				else time = desiredTime;
			}
			else // elliptic
			{
				double meanMotion = PI2 / prm.T;
				double desiredTime = posangle(futureMnA - prm.MnA) / meanMotion + floor((stepTrA - prm.TrA) / PI2) * prm.T;
				
				if (desiredTime - time > maxTimeStep) // new time is too large step from previous time.
				{
					time += maxTimeStep;

					// And now set back stepTrA to what we have in time
					futureMnA = time * meanMotion + prm.MnA;
					double newSetTrA = posangle(MnA2TrA(futureMnA, el.e));
					while (newSetTrA < stepTrA - PI05) newSetTrA += PI2;
					stepTrA = newSetTrA;

					debugCountOversteps++;
				}
				else time = desiredTime;
			}

			n++;
		}

		if (!aboveSurface)
		{
			DrawFeature(previousLong, previousLat, 3, BOX, skp, "");
		}

		if (debugInformation)
		{
			char debugLabel[100];
			sprintf(debugLabel, "Oversteps: %i, time: %.1f", debugCountOversteps, time);
			skp->Text(W / 2, H / 2 + 50, debugLabel, strlen(debugLabel)); // display info
		}
	}
	else // orbit track plane mode
	{
		double firstLong = 0.0, firstLat = 0.0;
		for (int k = 0; k < W; k++)
		{
			// Here follows a modification of GetEquPosInXSeconds, but with no nonspherical perturbions and no rotation of planet
			double LAN = el.theta;
			double APe = el.omegab - LAN;

			// This method is partly from NTRS document 20160000809
			double M0 = prm.MnA;
			double M = M0;

			// TrA in x seconds
			double TrA = MnA2TrA(M, el.e);
			TrA = fmod(TrA + PI2 * double(k) / double(W), PI2);
			double TrA0 = MnA2TrA(M0, el.e);

			double u = APe + TrA;
			double u0 = APe + TrA0;
			double alpha = atan2(cos(u) * sin(LAN) + sin(u) * cos(LAN) * cos(el.i), cos(u) * cos(LAN) - sin(u) * sin(LAN) * cos(el.i));
			double alpha0 = atan2(cos(u0) * sin(LAN) + sin(u0) * cos(LAN) * cos(el.i), cos(u0) * cos(LAN) - sin(u0) * sin(LAN) * cos(el.i));
			alpha -= alpha0;

			double longi = alpha + currentLong;
			longi = normangle(longi);

			double lati = asin(sin(u) * sin(el.i));

			if (k > 0)
			{
				DrawLine(previousLong, previousLat, longi, lati, skp);
			}
			else
			{
				firstLong = longi;
				firstLat = lati;
			}

			previousLong = longi;
			previousLat = lati;
		}

		// Connect the final and first points
		DrawLine(firstLong, firstLat, previousLong, previousLat, skp);
	}
}

bool MapMFD::TransformPoint(double longitude, double latitude, double* transformedLongitude, double* transformedLatitude, PROJECTION projection)
{
	double transLong, transLat;

	double k0 = 1.0; // used for transverse Mercator, and several others as miscellanous constant
	double A1 = 1.340264, A2 = -0.081106, A3 = 0.000893, A4 = 0.003796; // Coeffs used by Equal Earth
	double theta; // parameter with various values used in Equal Earth and Mollweide
	const double RobinsonCoeffs[19][2] = { // Robinson coeffs every 5 deg latitude, from 0 to 90
			{1.0000, 0.0000},
			{0.9986, 0.0620},
			{0.9954, 0.1240},
			{0.9900, 0.1860},
			{0.9822, 0.2480},
			{0.9730, 0.3100},
			{0.9600, 0.3720},
			{0.9427, 0.4340},
			{0.9216, 0.4958},
			{0.8962, 0.5571},
			{0.8679, 0.6176},
			{0.8350, 0.6769},
			{0.7986, 0.7346},
			{0.7597, 0.7903},
			{0.7186, 0.8435},
			{0.6732, 0.8936},
			{0.6213, 0.9394},
			{0.5722, 0.9761},
			{0.5322, 1.0000}
	};
	const int BerghausStarLobes = 5; // number of arms in Berghaus Star projection.


	longitude = normangle(longitude - centreLong);

	switch (projection)
	{
	case EQUIRECTANGULAR:
		// https://en.wikipedia.org/wiki/Equirectangular_projection
		transLong = longitude;
		transLat = latitude - centreLat;
		break;
	//case MILLER:
	//	// https://en.wikipedia.org/wiki/Miller_cylindrical_projection
	//	transLong = longitude;
	//	transLat = 1.25 * log(tan(PI / 4.0 + 0.4 * latitude)) - 1.25 * log(tan(PI / 4.0 + 0.4 * centreLat));
	//	break;
	case MERCATOR:
		// https://en.wikipedia.org/wiki/Mercator_projection
		transLong = longitude;
		if (abs(latitude) == PI05) latitude *= 0.9999; // avoid log(+-inf)
		transLat = log(tan(PI / 4.0 + latitude / 2.0)) - log(tan(PI / 4.0 + centreLat / 2.0));
		break;
	case VANDERGRINTEN:
		// https://en.wikipedia.org/wiki/Van_der_Grinten_projection
		A1 = 0.5 * abs(PI / longitude - longitude / PI); // A
		theta = asin(abs(2.0 * latitude / PI)); // theta
		A2 = cos(theta) / (sin(theta) + cos(theta) - 1.0); // G
		A3 = A2 * (2.0 / sin(theta) - 1.0); // P
		A4 = A1 * A1 + A2; // Q

		transLong = PI * (A1 * (A2 - A3 * A3) + sqrt(A1 * A1 * (A2 - A3 * A3) * (A2 - A3 * A3) - (A3 * A3 + A1 * A1) * (A2 * A2 - A3 * A3))) / (A3 * A3 + A1 * A1);
		transLat = PI * (A3 * A4 - A1 * sqrt((A1 * A1 + 1.0) * (A3 * A3 + A1 * A1) - A4 * A4)) / (A3 * A3 + A1 * A1);
		if (longitude < 0.0) transLong *= -1.0;
		if (latitude < 0.0) transLat *= -1.0;

		if (latitude == 0.0)
		{
			transLong = longitude;
			transLat = 0.0;
		}

		if (abs(longitude) < 1e-4 || abs(latitude) == PI05) // actually a longitude == 0.0 condition, but due to compuational math, we have a somewhat loose definition of "0".
		{
			transLong = 0.0;
			transLat = PI * tan(theta / 2.0);

			if (latitude < 0.0) transLat *= -1.0;
		}


		// Subtract centreLat
		theta = asin(abs(2.0 * centreLat / PI)); // theta
	
		// For centreLat subtraction, we now have that the central longitude is 0, so this condition will always be true. Implement beneath.
		k0 = PI * tan(theta / 2.0);
		if (centreLat < 0.0) k0 *= -1.0;

		transLat -= k0;
		break;
	case TRANSVERSEMERCATOR:
		// https://en.wikipedia.org/wiki/Transverse_Mercator_projection
		theta = sin(longitude) * cos(latitude);
		k0 = 1.0;

		if (theta == 1.0) theta *= 0.9999; // avoid division by zero

		transLong = 0.5 * k0 * log((1.0 + theta) / (1.0 - theta));
		transLat = k0 * atan2(sin(latitude), cos(latitude) * cos(longitude)) - centreLat;
		break;
	case ROBINSON:
		int idx;
		idx = max(0, min(18, int(abs(latitude) / (5.0 * RAD))));
		if (idx >= 18) theta = RobinsonCoeffs[18][0];
		else // 0 ... 17
		{
			A1 = (abs(latitude) / (5.0 * RAD) - double(idx)); // weight of interpolation
			theta = RobinsonCoeffs[idx][0] * (1.0 - A1) + RobinsonCoeffs[idx + 1][0] * A1; // weighted average of coefficients longitude
			k0 = RobinsonCoeffs[idx][1] * (1.0 - A1) + RobinsonCoeffs[idx + 1][1] * A1; // weighted average of coefficients latitude
		}

		transLong = 0.8487 * theta * longitude;
		transLat = 1.3523 * k0;
		if (latitude < 0.0) transLat *= -1.0;

		// centreLat
		idx = max(0, min(18, int(abs(centreLat) / (5.0 * RAD))));
		if (idx >= 18) theta = RobinsonCoeffs[18][0];
		else // 0 ... 17
		{
			A1 = (abs(centreLat) / (5.0 * RAD) - double(idx)); // weight of interpolation
			theta = RobinsonCoeffs[idx][0] * (1.0 - A1) + RobinsonCoeffs[idx + 1][0] * A1; // weighted average of coefficients longitude
			k0 = RobinsonCoeffs[idx][1] * (1.0 - A1) + RobinsonCoeffs[idx + 1][1] * A1; // weighted average of coefficients latitude
		}
		theta = 1.3523 * k0;
		if (centreLat < 0.0) theta *= -1.0;
		transLat -= theta;

		break;
	case EQUALEARTH:
		// https://en.wikipedia.org/wiki/Equal_Earth_projection
		A1 = 1.340264, A2 = -0.081106, A3 = 0.000893, A4 = 0.003796;
		theta = asin(sqrt(3.0) / 2.0 * sin(latitude));

		double powtheta2, powtheta3, powtheta4, powtheta5, powtheta6, powtheta7, powtheta8, powtheta9; // improve performance, as pow is insanely slow
		powtheta2 = theta * theta;
		powtheta3 = powtheta2 * theta;
		powtheta4 = powtheta3 * theta;
		powtheta5 = powtheta4 * theta;
		powtheta6 = powtheta5 * theta;
		powtheta7 = powtheta6 * theta;
		powtheta8 = powtheta7 * theta;
		powtheta9 = powtheta8 * theta;
		transLong = 2.0 * sqrt(3.0) * longitude * cos(theta) / (3.0 * (9.0 * A4 * powtheta8 + 7.0 * A3 * powtheta6 + 3.0 * A2 * powtheta2 + A1));
		transLat = A4 * powtheta9 + A3 * powtheta7 + A2 * powtheta3 + A1 * (theta);
		// Now find new theta for centreLat, and subtract that from transLat.
		theta = asin(sqrt(3.0) / 2.0 * sin(centreLat));
		powtheta3 = theta * theta * theta; // update relevant powers of theta
		powtheta7 = powtheta3 * powtheta3 * theta;
		powtheta9 = powtheta7 * theta * theta;
		transLat -= A4 * powtheta9 + A3 * powtheta7 + A2 * powtheta3 + A1 * (theta);

		break;
	case MOLLWEIDE:
		// https://en.wikipedia.org/wiki/Mollweide_projection
		theta = latitude - 0.2 * sin(latitude); // approximation for Newton's method, so that convergence is faster 
		if (abs(latitude) > PI05 * 0.999) // should be == PI05, but due to floating point error, we have to set to a close value. This value could be something like 1 - 1e-10, but we set to 0.999. Reason: none. Ask me. 
			theta = latitude / abs(latitude) * PI05; // set to sgn(lat) * PI05.
		else
			for (int i = 0; i < 2; i++) theta = theta - (2.0 * theta + sin(2.0 * theta) - PI * sin(latitude)) / (2.0 + 2.0 * cos(2.0 * theta)); // iterate to solve equation. Using 5 legs now. Slower convergence at poles.

		//A1 = 1.5553; // pol. factors
		//A2 = -2.9641;
		//A3 = 2.3413;
		//A4 = 0.205; // sin factor

		//// My own approximation of Mollweide to omit iteration. The whole problem is to solve 2*theta + sin(2*theta) = pi * sin(latitude) for theta
		//k0 = abs(latitude); // helping number to reduce abs calculations
		//if (k0 < 1.2) theta = latitude - A4 * sin(latitude);
		//else theta = latitude / k0 * (A1 * k0 * k0 + A2 * k0 + A3); // sgn(latitude) * (x^2 ...)

		transLong = 2.0 * sqrt(2.0) / PI * longitude * cos(theta);
		transLat = sqrt(2.0) * sin(theta);

		// now find new theta for centreLat, and subtract that from transLat.
		theta = centreLat - 0.2 * sin(centreLat);
		if (abs(centreLat) > PI05 * 0.999) // should be == PI05, but due to floating point error, we have to set to a close value. This value could be something like 1 - 1e-10, but we set to 0.999. Reason: none. Ask me. 
			theta = centreLat / abs(centreLat) * PI05; // set to sgn(lat) * PI05.
		else
			for (int i = 0; i < 2; i++) theta = theta - (2.0 * theta + sin(2.0 * theta) - PI * sin(centreLat)) / (2.0 + 2.0 * cos(2.0 * theta)); // iterate to solve equation. Using 5 legs now. Slower convergence at poles.
		//
		//// My own approximation of Mollweide to omit iteration. The whole problem is to solve 2*theta + sin(2*theta) = pi * sin(latitude) for theta
		//k0 = abs(centreLat); // helping number to reduce abs calculations
		//if (k0 < 1.2) theta = centreLat - A4 * sin(centreLat);
		//else theta = centreLat / k0 * (A1 * k0 * k0 + A2 * k0 + A3); // sgn(latitude) * (x^2 ...)

		transLat -= sqrt(2.0) * sin(theta);

		break;
	case HAMMER:
		// https://en.wikipedia.org/wiki/Hammer_projection
		transLong = 2.0 * sqrt(2.0) * cos(latitude) * sin(longitude / 2.0) / sqrt(1.0 + cos(latitude) * cos(longitude / 2.0));
		transLat = sqrt(2.0) * sin(latitude) / sqrt(1.0 + cos(latitude) * cos(longitude / 2.0));
		transLat -= sqrt(2.0) * sin(centreLat) / sqrt(1.0 + cos(centreLat)); // subtract centreLat
		//transLat -= centreLat;
		break;
	case LOXIMUTHAL:
		transLong = longitude * (latitude - centreLat) / log(tan(PI / 4.0 + latitude / 2.0) / tan(PI / 4.0 + centreLat / 2.0));
		transLat = latitude - centreLat;

		if (abs(transLat) < 1e-4) transLong = longitude * cos(centreLat);

		break;
	case LASKOWSKI:
		transLong = longitude * (0.975534 + latitude * latitude * (-0.119161 + longitude * longitude * -0.0143059 + latitude * latitude * -0.0547009));
		transLat = latitude * (1.00384 + longitude * longitude * (0.0802894 + latitude * latitude * -0.02855 + longitude * longitude * 0.000199025) + latitude * latitude * (0.0998909 + latitude * latitude * -0.0491032));
		transLat -= centreLat * (1.00384 + centreLat * centreLat * (0.0998909 + centreLat * centreLat * -0.0491032));
		break;
	//case ORTELIUSOVAL:
	//	// https://en.wikipedia.org/wiki/Ortelius_oval_projection
	//	if (longitude == 0.0)
	//	{
	//		transLat = latitude - centreLat;
	//		transLong = longitude; // = 0.0
	//	}
	//	else
	//	{
	//		theta = 0.5 * (PI * PI / (4.0 * abs(longitude)) + abs(longitude));
	//		transLat = latitude;
	//		if (abs(longitude) < PI05) transLong = abs(longitude) - theta + sqrt(theta * theta - pow(transLat, 2));
	//		else transLong = sqrt(PI * PI / 4.0 - latitude * latitude) + abs(longitude) - PI05;
	//		if (longitude < 0.0) transLong = -abs(transLong);
	//		else transLong = abs(transLong);
	//		
	//		transLat -= centreLat;
	//	}
	//	break;
	case WINKELTRIPEL:
		// https://en.wikipedia.org/wiki/Winkel_tripel_projection
		k0 = acos(2.0 / PI);
		theta = acos(cos(latitude) * cos(longitude / 2.0));
		
		if (theta == 0.0) // division by zero
		{
			transLong = 0.5 * (longitude * cos(k0) + 2.0 * cos(latitude) * sin(longitude / 2.0));
			transLat = 0.5 * (latitude + sin(latitude)) - centreLat;
		}
		else
		{
			transLong = 0.5 * (longitude * cos(k0) + 2.0 * cos(latitude) * sin(longitude / 2.0) / sin(theta) * theta);
			transLat = 0.5 * (latitude + sin(latitude) / sin(theta) * theta) - centreLat;
		}
		break;
	//case RECTANGULARPOLYCONIC:
	//	// https://en.wikipedia.org/wiki/Rectangular_polyconic_projection
	//	if (latitude == 0.0) latitude = 1e-5; // avoid division by 0

	//	if (centreLat == 0.0) k0 = 0.5 * longitude;
	//	else k0 = tan(0.5 * longitude * sin(centreLat)) / sin(centreLat);

	//	theta = 2.0 * atan(k0 * sin(latitude));

	//	transLong = sin(theta) / tan(latitude);
	//	transLat = normangle(latitude - centreLat) + (1.0 - cos(theta)) / tan(latitude); // weigh up for the global centreLat subtraction

	//	// I have actually messed up the implementation in the above code (changing the latitude with correct scale to follow centreLat). But I like the look, so I've kept it.
	//	// For an implementation consistent with other implemented projections (like Mollweide), then try the version below.
	//	//if (latitude == 0.0) latitude = 1e-5; // avoid division by 0

	//	//k0 = 0.5 * longitude;
	//	////else k0 = tan(0.5 * longitude * sin(0.0)) / sin(0.0);

	//	//theta = 2.0 * atan(k0 * sin(latitude));

	//	//transLong = sin(theta) / tan(latitude);
	//	//transLat = normangle(latitude - centreLat) + (1.0 - cos(theta)) / tan(latitude); // weigh up for the global centreLat subtraction
	//	break;
	case AZIMUTHALEQUIDISTANT:
		// https://en.wikipedia.org/wiki/Azimuthal_equidistant_projection
		theta = sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude); // cos(great distance)
		k0 = acos(theta) / sqrt(1.0 - theta * theta);
		if (theta > 0.9999) k0 = 1.0;
		transLong = k0 * cos(latitude) * sin(longitude);
		transLat = k0 * (cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));


		break;
	case LAMBERTAZIMUTHAL:
		// https://mathworld.wolfram.com/LambertAzimuthalEqual-AreaProjection.html
		if (abs(longitude) == PI) longitude *= 0.999; // avoid division by zero
		theta = sqrt(2.0 / (1.0 + sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude)));

		transLong = theta * cos(latitude) * sin(longitude);
		transLat = theta * (cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
		break;
	case STEREOGRAPHIC:
		// https://en.wikipedia.org/wiki/Stereographic_projection_in_cartography
		theta = sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude); // cos(great distance)

		k0 = 2.0 / (1.0 + theta);
		transLong = k0 * cos(latitude) * sin(longitude);
		transLat = k0 * (cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
		break;
	//case GNOMONIC:
	//	// https://mathworld.wolfram.com/GnomonicProjection.html
	//	theta = sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude); // cos(great distance)

	//	if (theta <= 0.2) k0 = 3.0 * acos(theta) / sqrt(1.0 - theta * theta);
	//	else k0 = 1.0 / theta;

	//	transLong = k0 * cos(latitude) * sin(longitude);
	//	transLat = k0 * (cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
	//	break;
	case GALLPETERS:
		transLong = longitude;
		transLat = 2.0 * sin(latitude);
		transLat -= 2.0 * sin(centreLat);
		break;
	default:
		sprintf(oapiDebugString(), "ERROR, default in TransformPoint! %.2f", oapiGetSimTime());
		return false;
		break;
	}

	*transformedLongitude = double(centreZoom) * transLong;
	*transformedLatitude = double(centreZoom) * transLat;

	return true;
}

bool MapMFD::GetEquPosInXSeconds(double t, ELEMENTS el, ORBITPARAM prm, double currentLongitude, double* longitude, double* latitude)
{
	double LAN = el.theta;
	double APe = el.omegab - LAN;
	//// This gives bullshit results for high eccentricity (also for suborbital flights), so I'll simply disable it.
	//if (v->NonsphericalGravityEnabled() && el.e < 1.0) // Take J2 effects into consideration. Perturbs LAN and APe. But we only implement it for elliptical orbits, as I have no equation for orbits with no prm.T defined (i.e. hyperbolic orbits)
	//{
	//	double J2 = oapiGetPlanetJCoeff(ref, 0); // 0 gives J2.
	//	APe += 3.0 * PI2 / prm.T / 4.0 * pow(refRad / el.a, 2.0) * (5.0 * cos(el.i) * cos(el.i) - 1.0) / pow(1.0 - el.e * el.e, 2.0) * t * J2;
	//	LAN += -3.0 * PI2 / prm.T / 2.0 * pow(refRad / el.a, 2.0) * cos(el.i) / pow(1.0 - el.e * el.e, 2.0) * t * J2;
	//}

	// This method is partly from NTRS document 20160000809
	double LPe = APe + LAN;
	double M0 = prm.MnA;
	double M = M0;

	// TrA in x seconds
	if (el.e > 1.0) M = M0 + sqrt(refMu / pow(-el.a, 3)) * t; // http://control.asu.edu/Classes/MAE462/462Lecture05.pdf page 37.
	else M = posangle(M0 + PI2 * t / prm.T);
	double TrA = MnA2TrA(M, el.e);
	double TrA0 = prm.TrA;

	double u = LPe - LAN + TrA;
	double u0 = LPe - LAN + TrA0;
	double alpha = atan2(cos(u) * sin(LAN) + sin(u) * cos(LAN) * cos(el.i), cos(u) * cos(LAN) - sin(u) * sin(LAN) * cos(el.i));
	double alpha0 = atan2(cos(u0) * sin(LAN) + sin(u0) * cos(LAN) * cos(el.i), cos(u0) * cos(LAN) - sin(u0) * sin(LAN) * cos(el.i));
	alpha -= alpha0;

	double longi = alpha + currentLongitude - PI2 / oapiGetPlanetPeriod(ref) * t;
	longi = normangle(longi);

	double lati = asin(sin(u) * sin(el.i));

	*longitude = longi;
	*latitude = lati;

	double orbitRad = el.a * (1.0 - el.e * el.e) / (1.0 + el.e * cos(TrA));
	if (orbitRad > refRad) return true;
	else return false;
}

void MapMFD::GetObjectEquPos(OBJHANDLE tgt, double* longitude, double* latitude, double *radius)
{
	VECTOR3 tgtPos;
	oapiGetRelativePos(tgt, ref, &tgtPos);
	VECTOR3 Equ = Ecl2Equ(tgtPos);
	double dist = length(Equ);
	*latitude = asin(Equ.y / dist);
	*longitude = normangle(atan2(Equ.z, Equ.x) - oapiGetPlanetCurrentRotation(ref));
	*radius = dist;
}

void MapMFD::GetObjectRelativeElements(OBJHANDLE tgt, ELEMENTS& el, ORBITPARAM* prm)
{
	VECTOR3 statePos, stateVel;
	oapiGetRelativePos(tgt, ref, &statePos);
	oapiGetRelativeVel(tgt, ref, &stateVel);
	MATRIX3 rot;
	oapiGetPlanetObliquityMatrix(ref, &rot); // Ecl2Equ, but do it here explecitely, so that we only call the matrix once.
	statePos = tmul(rot, statePos);
	stateVel = tmul(rot, stateVel);

	// Orbiter has a wrong reference system. Swap y and z.
	double buffer = statePos.z;
	statePos.z = statePos.y;
	statePos.y = buffer;

	buffer = stateVel.z;
	stateVel.z = stateVel.y;
	stateVel.y = buffer;


	// Now we have state vectors in equatorial frame. Then calculate KOST.
	// Sources are https://downloads.rene-schwarz.com/download/M002-Cartesian_State_Vectors_to_Keplerian_Orbit_Elements.pdf for elliptical orbits,
	// and https://en.wikipedia.org/wiki/Hyperbolic_trajectory for hyperbolic orbits.
	double radius = length(statePos);
	double speed = length(stateVel);
	double radialSpeed = dotp(statePos, stateVel) / radius;
	VECTOR3 angularMomentumVector = crossp(statePos, stateVel);
	double inclination = acos(angularMomentumVector.z / length(angularMomentumVector));
	VECTOR3 normalVector = crossp(_V(0.0, 0.0, 1.0), angularMomentumVector);
	double LAN = acos(normalVector.x / length(normalVector));
	if (normalVector.y < 0.0)
		LAN = PI2 - LAN;
	VECTOR3 eccentricityVector = (statePos * (speed * speed - refMu / radius) - stateVel * radius * radialSpeed) / refMu;
	double eccentricity = length(eccentricityVector);
	double APe = acos(dotp(unit(normalVector), unit(eccentricityVector)));
	if (eccentricityVector.z < 0.0)
		APe = PI2 - APe;
	double TrA = acos(dotp(eccentricityVector, statePos) / eccentricity / radius);
	if (radialSpeed < 0.0)
		TrA = PI2 - TrA;
	double eccentricAnomaly = 2.0 * atan(tan(TrA / 2.0) / sqrt((1.0 + eccentricity) / (1.0 - eccentricity)));
	if (eccentricity > 1.0) eccentricAnomaly = 2.0 * atanh(tan(TrA / 2.0) / sqrt((eccentricity + 1.0) / (eccentricity - 1.0))); // hyperbolic equation of motion

	double energy = length2(stateVel) / 2.0 - refMu / length(statePos);
	double SMa = -refMu / (2.0 * energy);
	double period = PI2 * sqrt(pow(SMa, 3) / refMu);
	double MnA = eccentricAnomaly - eccentricity * sin(eccentricAnomaly);
	if (eccentricity > 1.0) MnA = eccentricity * sinh(eccentricAnomaly) - eccentricAnomaly;

	el.a = SMa;
	el.e = eccentricity;
	el.i = inclination;
	el.omegab = posangle(LAN + APe);
	el.L = posangle(el.omegab + MnA);
	el.theta = LAN;

	prm->EcA = eccentricAnomaly;
	prm->TrA = TrA;
	prm->T = period; // this will be erranous for hyperbolic orbits, but it won't bother us.
	prm->MnA = MnA;
}

char* MapMFD::GetCoordinateString(double longitude, double latitude)
{
	static char coordinate[20];
	if (longitude < 0.0) sprintf(coordinate, "%.2f\u00B0W", -longitude * DEG);
	else sprintf(coordinate, "%.2f\u00B0E", longitude * DEG);

	if (latitude < 0.0) sprintf(coordinate, "%s  %.2f\u00B0S", coordinate, -latitude * DEG);
	else sprintf(coordinate, "%s  %.2f\u00B0N", coordinate, latitude * DEG);

	return coordinate;
}

// MnA in radians
inline double MapMFD::MnA2TrA(double MnA, double Ecc)
{
	//double TrA = MnA + (2.0 * Ecc - pow(Ecc, 3.0) / 4.0) * sin(MnA) + 5.0 / 4.0 * pow(Ecc, 2.0) * sin(2.0 * MnA) + 13.0 / 12.0 * pow(Ecc, 3.0) * sin(3.0 * MnA);
	// Old Taylor series diverges for high eccentricities. Instead, we now first convert from MnA to EccAnomaly (using Newton's method), and then analytical conversion to TrA
	// While old Taylor required an Ecc < 0.4, the new method requires Ecc < 0.98, so much much better.

	// And for hyperbolic orbits, we are now featuring another method, using the hyperbolic orbit equations.
	if (Ecc < 1.0)
	{
		double EccAnom = MnA; // initial guess

		const int maxIterationLength = 15;
		double previousResult = EccAnom;
		int i = 0;
		while (i < maxIterationLength)
		{
			EccAnom -= (EccAnom - Ecc * sin(EccAnom) - MnA) / (1.0 - Ecc * cos(EccAnom));
			i++;

			if (abs(EccAnom - previousResult) < 1e-8) break; // Converged

			previousResult = EccAnom;
		}

		double TrA = 2.0 * atan(sqrt((1.0 + Ecc) / (1.0 - Ecc)) * tan(EccAnom / 2.0));

		return TrA;
	}
	else
	{
		// Hyperbolic equations, from https://en.wikipedia.org/wiki/Hyperbolic_trajectory

		double HypAnom = asinh(MnA / 2.0); // initial guess, hyperbolic anomaly, which has an exponential dependency

		const int maxIterationLength = 30;
		double previousResult = HypAnom;
		int i = 0;
		while (i < maxIterationLength)
		{
			HypAnom -= (HypAnom + MnA - Ecc * sinh(HypAnom)) / (1.0 - Ecc * cosh(HypAnom));
			i++;

			if (abs(HypAnom - previousResult) < 1e-8) break; // Converged

			previousResult = HypAnom;
		}

		double TrA = 2.0 * atan(sqrt((Ecc + 1.0) / (Ecc - 1.0)) * tanh(HypAnom / 2.0));

		return TrA;
	}

}

inline double MapMFD::TrA2MnA(double TrA, double Ecc)
{
	//double MnA = TrA - 2.0 * Ecc * sin(TrA) + (3.0 / 4.0 * pow(Ecc, 2.0) + pow(Ecc, 4.0) / 8.0) * sin(2.0 * TrA) - pow(Ecc, 3.0) / 3.0 * sin(3.0 * TrA) + 5.0 / 32.0 * pow(Ecc, 4.0) * sin(4.0 * TrA);
	// Old Taylor series diverges for high Ecc and is in general shit, when we can use analytical result.

	double EccAnom = EccentricAnomaly(Ecc, TrA);
	if (Ecc > 1.0) return Ecc * sinh(EccAnom) - EccAnom; // hyperbolic
	else return EccAnom - Ecc * sin(EccAnom); // elliptic
}

// TrA in radians
inline double MapMFD::EccentricAnomaly(double ecc, double TrA)
{
	if (ecc > 1.0) return 2.0 * atanh(sqrt((ecc - 1.0) / (ecc + 1.0)) * tan(TrA / 2.0)); // hyperbolic
	else return 2.0 * atan(sqrt((1.0 - ecc) / (1.0 + ecc)) * tan(TrA / 2.0)); // elliptic
}

inline VECTOR3 MapMFD::Ecl2Equ(VECTOR3 Ecl)
{
	MATRIX3 rot;
	oapiGetPlanetObliquityMatrix(ref, &rot);
	VECTOR3 Equ = tmul(rot, Ecl);
	return Equ;
}

// Gives the equatorial frame vector from reference centre to a coordinate. Includes surface elevation.
inline VECTOR3 MapMFD::Coord2Vector(double longitude, double latitude)
{
	double coordRadius = refRad + oapiSurfaceElevation(ref, longitude, latitude); // distance to centre of planet from coordinate (adding surface elevation).
	double rotationAngle = oapiGetPlanetCurrentRotation(ref); // get the angle to add.
	return _V(cos(longitude + rotationAngle) * cos(latitude), sin(latitude), sin(longitude + rotationAngle) * cos(latitude)) * coordRadius;
}

