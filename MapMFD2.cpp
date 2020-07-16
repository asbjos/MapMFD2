/*
	Map MFD 2, a project by Asbj�rn 'asbjos' Kr�ger, 2020.

	The intention is to create a new and hopefully better Map MFD experience
	than the default version in Orbiter Space Flight Simulator.

	This will not and can not replace the original, but will hopefully be of use.

	A basic roadmap and wishlist:
		Multibase select
		Multitarget select
		Different map projection
		Possibly optimised for zoom (plot lines selectively)
		Show distance to base AND vessel. Both TDist (true distance) and GDist (ground distance - orthodome).
*/

#define STRICT
#define ORBITER_MODULE

#include "orbitersdk.h"
#include "MapMFD2.h"


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
	else
	{
		MapScreen(skp);
	}

	return true;
}

bool MapMFD::ConsumeButton(int bt, int event)
{
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
		else return false;
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
		if (bt == 6 && trackPosition != LATLONGTRACK)
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
		}
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
	bool NumberOrbitsDisplayed(void* id, char* str, void* data);

	if (configScreen)
	{
		switch (key)
		{
		case OAPI_KEY_MINUS:
			configSelection = CONFIGSELECT((int(configSelection) + LASTENTRYCONFIG - 1) % int(LASTENTRYCONFIG)); // + 9 instead of -1, which results in negative result
			return true;
		case OAPI_KEY_EQUALS:
			configSelection = CONFIGSELECT((int(configSelection) + 1) % int(LASTENTRYCONFIG));
			return true;
		case OAPI_KEY_M:

			switch (configSelection)
			{
			case CONFIGTRACKMODE:
				orbitTrackGround = !orbitTrackGround;
				return true;
			case CONFIGRADAR:
				displayElevationRadar = !displayElevationRadar;
				return true;
			case CONFIGSHOWVESSELS:
				showVessels = !showVessels;
				return true;
			case CONFIGPROJECTION:
				proj = PROJECTION((int(proj) + 1) % int(LASTENTRYPROJECTION));
				return true;
			case CONFIGFLIPPOLE:
				azimuthalEquidistantNortPole = !azimuthalEquidistantNortPole;
				return true;
			case CONFIGRESETMAP:
				centreLat = 0.0;
				centreLong = 0.0;
				centreZoom = 1;
				trackPosition = NOTRACK;
				return true;
			case CONFIGGRIDSEP:
				oapiOpenInputBox("Grid separation (\u00B0):", GridSeparation, 0, 20, (void*)this);
				return true;
			case CONFIGGRIDRES:
				oapiOpenInputBox("Grid resolution (\u00B0):", GridResolution, 0, 20, (void*)this);
				return true;
			case CONFIGMAPRES:
				skipEveryNLines = (skipEveryNLines + 1) % (skipEveryNLinesMax + 1);
				if (skipEveryNLines == skipEveryNLinesMax) autoResolution = true;
				else autoResolution = false;
				return true;
			case CONFIGMAPAUTOSIZE:
				defaultMapLinesAmount = (defaultMapLinesAmount % 10000) + 1000; // allow up to 1e4 lines, and at least 1e3
				return true;
			case CONFIGTRACKANGLEDELTA:
				// Valid values are 0.01, 0.05, 0.1, 0.5, 1.0
				if (orbitTrackAngleDelta == 0.1) orbitTrackAngleDelta = 0.5;
				else if (orbitTrackAngleDelta == 0.5) orbitTrackAngleDelta = 1.0;
				else if (orbitTrackAngleDelta == 1.0) orbitTrackAngleDelta = 0.01;
				else if (orbitTrackAngleDelta == 0.01) orbitTrackAngleDelta = 0.05;
				else orbitTrackAngleDelta = 0.1;
				return true;
			case CONFIGTRACKNUMORBITS:
				oapiOpenInputBox("Number of orbits to show:", NumberOrbitsDisplayed, 0, 20, (void*)this);
				return true;
			case CONFIGPLANETVIEWSEGMENTS:
				if (viewCircleResolution == 60) viewCircleResolution = 90;
				else if (viewCircleResolution == 90) viewCircleResolution = 120;
				else if (viewCircleResolution == 120) viewCircleResolution = 180;
				else if (viewCircleResolution == 180) viewCircleResolution = 360;
				else viewCircleResolution = 60;
				return true;
			case CONFIGRESETALL:
				resetCommand = true;
				return true;
			case CONFIGDEBUGINFO:
				debugInformation = !debugInformation;
				return true;
			default:
				sprintf(oapiDebugString(), "ERROR! Invalid Configselect setting. Debug %.3f", oapiGetSimTime());
				return false;
			}

			return true;
		case OAPI_KEY_D:
			// Set active value to default
			switch (configSelection)
			{
			case CONFIGTRACKMODE:
				orbitTrackGround = true;
				return true;
			case CONFIGRADAR:
				displayElevationRadar = false;
				return true;
			case CONFIGSHOWVESSELS:
				showVessels = false;
				return true;
			case CONFIGPROJECTION:
				proj = EQUIRECTANGULAR;
				return true;
			case CONFIGFLIPPOLE:
				azimuthalEquidistantNortPole = true;
				return true;
			case CONFIGRESETMAP:
				centreLat = 0.0;
				centreLong = 0.0;
				centreZoom = 1;
				trackPosition = NOTRACK;
				return true;
			case CONFIGGRIDSEP:
				gridAngleSeparation = 30;
				return true;
			case CONFIGGRIDRES:
				gridResolution = 2;
				return true;
			case CONFIGMAPRES:
				skipEveryNLines = skipEveryNLinesMax;
				autoResolution = true;
				return true;
			case CONFIGMAPAUTOSIZE:
				defaultMapLinesAmount = 5000;
				return true;
			case CONFIGTRACKANGLEDELTA:
				orbitTrackAngleDelta = 0.1;
				return true;
			case CONFIGTRACKNUMORBITS:
				orbitTrackOrbitsNumber = 3.5;
				return true;
			case CONFIGPLANETVIEWSEGMENTS:
				viewCircleResolution = 60;
				return true;
			case CONFIGRESETALL:
				resetCommand = false;
				return true;
			default:
				sprintf(oapiDebugString(), "ERROR! Invalid configselect reset. Debug %.3f", oapiGetSimTime());
				return false;
			}
		case OAPI_KEY_O:
			// Back to main map display
			configScreen = false;
			InvalidateButtons();
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
			return true;
		case OAPI_KEY_EQUALS:
			referenceSelection = (referenceSelection + 1) % numberReferenceChoices;
			if (referenceSelection < currentReferenceTopListed) currentReferenceTopListed = referenceSelection;
			if (referenceSelection > currentReferenceTopListed + entriesPerPage) currentReferenceTopListed = referenceSelection - entriesPerPage;
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
			return true;
		case OAPI_KEY_D: // auto and return
			SetNewReference(v->GetSurfaceRef());
			referenceListScreen = false;
			InvalidateButtons();
			return true;
		case OAPI_KEY_T:
			oapiOpenInputBox("Reference planet:", ReferencePlanet, 0, 20, (void*)this);

			// DEBUG! See if this has to be moved to SetReferencePlanet function.
			referenceListScreen = false;
			InvalidateButtons();
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

		int entriesPerPage = 13;

		switch (key)
		{
		case OAPI_KEY_MINUS:
			targetSelection = (targetSelection + numberTargetChoices - 1) % numberTargetChoices; // + 9 instead of -1, which results in negative result
			if (targetSelection < currentTargetTopListed) currentTargetTopListed = targetSelection;
			if (targetSelection > currentTargetTopListed + entriesPerPage) currentTargetTopListed = targetSelection - entriesPerPage;
			return true;
		case OAPI_KEY_EQUALS:
			targetSelection = (targetSelection + 1) % numberTargetChoices;
			if (targetSelection < currentTargetTopListed) currentTargetTopListed = targetSelection;
			if (targetSelection > currentTargetTopListed + entriesPerPage) currentTargetTopListed = targetSelection - entriesPerPage;
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
				else
				{
					sprintf(oapiDebugString(), "ERROR! DEBUG! Vessel index was not valid! %.2f", oapiGetSimTime());
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

			sprintf(oapiDebugString(), "ERROR! DEBUG! Invalid select press. %.2f", oapiGetSimTime());
			return false; // we should have been through a return true before now
		case OAPI_KEY_O: // select and return home
			targetListScreen = false;
			InvalidateButtons();
			return true;
		case OAPI_KEY_T:
			oapiOpenInputBox("Target object:", TargetObject, 0, 20, (void*)this);

			// Allow us to see map for potential coordinates
			targetListScreen = false;
			InvalidateButtons();
			return true;
		default:
			return false;
		}
	}
	else
	{
		switch (key)
		{
		case OAPI_KEY_R:
			// New "fancy" reference select screen
			referenceListScreen = true;
			InvalidateButtons();
			//oapiOpenInputBox("Reference planet:", ReferencePlanet, 0, 20, (void*)this);
			return true;
		case OAPI_KEY_T:
			// New "fancy" target select screen
			targetListScreen = true;
			InvalidateButtons();
			//oapiOpenInputBox("Target object:", TargetObject, 0, 20, (void*)this);
			return true;
		case OAPI_KEY_X:
			centreZoom /= 2;

			if (centreZoom < 1) centreZoom = 1;
			return true;
		case OAPI_KEY_Z:
			//panZoom *= 2.0;
			centreZoom *= 2;

			if (centreZoom > MAX_ZOOM) centreZoom = MAX_ZOOM;
			return true;
		case OAPI_KEY_K:
			trackPosition = TRACKMODE((int(trackPosition) + 1) % int(LASTENTRYTRACK));
			if (trackPosition == LONGTRACK) centreLat = 0.0; // set to zero, so that it's centered. But it is allowed to change this later.
			InvalidateButtons();
			return true;
		case OAPI_KEY_MINUS:
			//panLat += 1.0 * RAD / panZoom;
			centreLat += 15.0 * RAD / double(centreZoom);
			return true;
		case OAPI_KEY_EQUALS:
			//panLat -= 1.0 * RAD / panZoom;
			centreLat -= 15.0 * RAD / double(centreZoom);
			return true;
		case OAPI_KEY_LBRACKET:
			//panLong += 1.0 * RAD / panZoom;
			centreLong -= 15.0 * RAD / double(centreZoom);
			return true;
		case OAPI_KEY_RBRACKET:
			//panLong -= 1.0 * RAD / panZoom;
			centreLong += 15.0 * RAD / double(centreZoom);
			return true;
		case OAPI_KEY_M:
			numTargets -= 1;
			if (numTargets < 0) numTargets = 0;
			return true;
		case OAPI_KEY_P:
			proj = PROJECTION((int(proj) + 1) % int(LASTENTRYPROJECTION));
			return true;
		case OAPI_KEY_1:
			referenceListScreen = true;
			InvalidateButtons();
			return true;
		case OAPI_KEY_2:
			targetListScreen = true;
			InvalidateButtons();
			return true;
		case OAPI_KEY_C:
			// Config display
			configScreen = true;
			InvalidateButtons();
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

bool MapMFD::ConsumeKeyImmediate(char* kstate)
{
	//if ()
	return false;
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

bool NumberOrbitsDisplayed(void* id, char* str, void* data)
{
	return ((MapMFD*)data)->SetNumberOrbitsDisplayed(str);
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

			if (strPos != NULL) // found two values, i.e. a coordinate in 'long lat' format, both in degrees.
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
		else if (numTargets < 10)
		{
			targets[numTargets] = userTarget;
			numTargets += 1;
			return true;
		}

		return false;
	}
	else if (numTargets < 10)
	{
		targets[numTargets] = userTarget;
		numTargets += 1;

		//HUDPARAM surfHud;
		//surfHud.HUDorbit = { userTarget };
		//oapiSetHUDMode(HUD_SURFACE, &surfHud);
		return true;
	}

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

void MapMFD::StoreStatus() const
{
	MapMFDState.autoResolution = autoResolution;
	MapMFDState.azimuthalEquidistantNortPole = azimuthalEquidistantNortPole;
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

	MapMFDState.orbitTrackAngleDelta = orbitTrackAngleDelta;
	MapMFDState.orbitTrackOrbitsNumber = orbitTrackOrbitsNumber;
	MapMFDState.displayElevationRadar = displayElevationRadar;
	for (int i = 0; i < GROUND_TRACK_HISTORY_SIZE; i++)
	{
		MapMFDState.shipHistory[i][0] = shipHistory[i][0];
		MapMFDState.shipHistory[i][1] = shipHistory[i][1];
		MapMFDState.shipHistory[i][2] = shipHistory[i][2];
	}
	MapMFDState.shipHistoryIndex = shipHistoryIndex;
	MapMFDState.shipHistoryLength = shipHistoryLength;
}

void MapMFD::RecallStatus()
{
	if (oapiGetObjectType(MapMFDState.ref) != OBJTP_INVALID && !resetCommand) // reset command asks us to not recall, so that we set to default
	{
		autoResolution = MapMFDState.autoResolution;
		azimuthalEquidistantNortPole = MapMFDState.azimuthalEquidistantNortPole;
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
		orbitTrackOrbitsNumber = MapMFDState.orbitTrackOrbitsNumber;
		displayElevationRadar = MapMFDState.displayElevationRadar;
		for (int i = 0; i < GROUND_TRACK_HISTORY_SIZE; i++)
		{
			shipHistory[i][0] = MapMFDState.shipHistory[i][0];
			shipHistory[i][1] = MapMFDState.shipHistory[i][1];
			shipHistory[i][2] = MapMFDState.shipHistory[i][2];
		}
		shipHistoryIndex = MapMFDState.shipHistoryIndex;
		shipHistoryLength = MapMFDState.shipHistoryLength;
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
	double simt = oapiGetSimTime(); // sim time

	//sprintf(oapiDebugString(), "In: %i, out: %i. SkipFactor: %i. Auto: %i", pointsInside, pointsOutside, int(58800 / defaultMapLinesAmount / centreZoom) + 1, int(autoResolution));
	pointsInside = 0;
	pointsOutside = 0;

	// Title
	char refName[30], mfdTitle[100];
	oapiGetObjectName(ref, refName, 30);

	sprintf(mfdTitle, "Map: ");
	strcat(mfdTitle, refName); // Add name of mapped body

	Title(skp, mfdTitle);	// Draws the MFD title

	int textPos = 1;
	char cbuf[100];

	// Draw track and zoom info
	if (trackPosition != NOTRACK) sprintf(cbuf, "TRK");
	else sprintf(cbuf, "   ");

	if (centreZoom > 1) sprintf(cbuf, "%s  ZM %i", cbuf, centreZoom);
	skp->Text(textDX * 60, 0, cbuf, strlen(cbuf));

	sprintf(cbuf, GetProjectionName());
	skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
	textPos++;

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
	const int sunCircleResolution = 180;
	double sunLong, sunLat, sunRad;
	GetObjectEquPos(oapiGetGbodyByIndex(0), &sunLong, &sunLat, &sunRad); // I here expect ALL systems to have the Sun (or light giving star) as first body in config.
	double transformedSunLong, transformedSunLat;
	TransformPoint(sunLong, sunLat, &transformedSunLong, &transformedSunLat, proj);
	skp->SetBrush(sunIcon);
	skp->SetPen(terminatorLine);
	int sunSize = 10;
	skp->Ellipse(int(W / 2 + transformedSunLong / PI * W / 2) - sunSize, int(H / 2 - transformedSunLat / PI05 * H / 4) - sunSize, int(W / 2 + transformedSunLong / PI * W / 2) + sunSize, int(H / 2 - transformedSunLat / PI05 * H / 4) + sunSize);
	skp->SetBrush(NULL); // disable autofilling
	// Calculate the angular viewable area, and calculate points for all different bearings. From https://www.movable-type.co.uk/scripts/latlong.html
	double sunAngleView = PI05; // just set it like this, and save computation power, although it really is 0.0024 degrees off for the Earth.
	double firstLo, firstLa, previousLo, previousLa;
	skp->SetPen(terminatorLine);
	if (proj == EQUIRECTANGULAR)
	{
		oapi::IVECTOR2 sunFill[sunCircleResolution + 4]; // normal resolution + (e.g.) upper left, upper right, terminatorPointRight, ..., terminatorPointLeft

		bool hasCrossedMapEdge

		for (int i = 0; i < sunCircleResolution; i++)
		{
			double bearing = double(i) / double(sunCircleResolution) * PI2;

			double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));
			double longPoint = normangle(sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint)));

			if (sunLat < 0.0) // overlaps South Pole
			{
				if (longPoint )
			}
			else // fills entire North Pole
			{

			}

			//double transformedLat, transformedLong;
			//TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

		}
	}
	else // generic line around terminator
	{
		for (int i = 0; i < sunCircleResolution; i++) // around entire circle
		{
			double bearing = double(i) / double(sunCircleResolution) * PI2;

			double latPoint = asin(sin(sunLat) * cos(sunAngleView) + cos(sunLat) * sin(sunAngleView) * cos(bearing));
			double longPoint = normangle(sunLong + atan2(sin(bearing) * sin(sunAngleView) * cos(sunLat), cos(sunAngleView) - sin(sunLat) * sin(latPoint)));

			//double transformedLat, transformedLong;
			//TransformPoint(longPoint, latPoint, &transformedLong, &transformedLat, proj);

			if (i != 0)
			{
				DrawLine(previousLo, previousLa, longPoint, latPoint, skp);
			}
			else
			{
				firstLa = latPoint;
				firstLo = longPoint;
			}

			previousLo = longPoint;
			previousLa = latPoint;
		}

		// Wrap around
		DrawLine(previousLo, previousLa, firstLo, firstLa, skp);
	}
	
	// Draw grid lines for map
	skp->SetPen(gridLines);
	double safetyValue = 0.9999;
	// Longitude lines (vertical)
	for (int lo = -180; lo < 180; lo += gridAngleSeparation)
	{
		for (int k = 0; k < 180 / gridResolution; k++)
		{
			int la = -90 + k * gridResolution;
			DrawLine(lo * RAD, la * RAD, lo * RAD, la * RAD + gridResolution * RAD, skp, false);
		}
	}
	// Latitude lines (horizontal)
	for (int la = -90; la <= 90; la += gridAngleSeparation)
	{
		for (int k = 0; k < 360 / gridResolution; k++)
		{
			int lo = -180 + k * gridResolution;
			DrawLine(lo * RAD, la * RAD, lo * RAD + gridResolution * RAD, la * RAD, skp, false);
		}
	}

	// Draw map
	if (updateCache)
	{
		char filePath[100];
		bool successfullyFoundVector = false;
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
			sprintf(cbuf, "Mode: %i, handle: %i", fileMode, (int)vectorFile);
			skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
			textPos++;
		}

		if (vectorFile != 0)
		{
			// Cache settings
			mapExists = true;

			// Other stuff
			successfullyFoundVector = true;
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
			//sprintf(cacheMap[lineNr], line);
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

						//if (abs(longitude) >= 180.0) longitude = longitude / abs(longitude) * longitude * 0.99999; // no overflow, please
						//if (abs(latitude) >= 90.0) latitude = latitude / abs(latitude) * latitude * 0.99999; // no overflow, please

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
				sprintf(cbuf, "Last string: %s", line);
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				sprintf(cbuf, "Last long: %.2f\u00B0, last lat: %.2f\u00B0", longitude, latitude);
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				if (vertexCounter == totalVertices)
				{
					sprintf(cbuf, "Read %i of %i vertices. Success!", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
				else
				{
					sprintf(cbuf, "Read %i of %i vertices. Failed!", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
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
						//char* strPos;
						//strPos = strchr(line, ' ');
						//double longitude = atof(line);
						//double latitude = atof(line + int(strPos - line + 1));

						//if (abs(longitude) >= 180.0) longitude = longitude / abs(longitude) * longitude * 0.99999; // no overflow, please
						//if (abs(latitude) >= 90.0) latitude = latitude / abs(latitude) * latitude * 0.99999; // no overflow, please

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
				sprintf(cbuf, "Using cache!");
				skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
				textPos++;

				if (vertexCounter == totalVertices)
				{
					sprintf(cbuf, "Read %i of %i vertices. Success!", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
				else
				{
					sprintf(cbuf, "Read %i of %i vertices. Failed!", vertexCounter, totalVertices);
					skp->Text(textDX, textDY * textPos, cbuf, strlen(cbuf));
					textPos++;
				}
			}
		}
	}

	// Draw surface bases
	skp->SetPen(baseBox);
	skp->SetTextColor(0xF0F0F0);
	for (int i = 0; i < (int)oapiGetBaseCount(ref); i++)
	{
		double baseLatitude, baseLongitude;
		OBJHANDLE baseRef = oapiGetBaseByIndex(ref, i);
		oapiGetBaseEquPos(baseRef, &baseLongitude, &baseLatitude);
		if (centreZoom >= 8)
		{
			char label[30];
			oapiGetObjectName(baseRef, label, 30);
			DrawFeature(baseLongitude, baseLatitude, 3, BOX, skp, label);
		}
		else DrawFeature(baseLongitude, baseLatitude, 3, BOX, skp, "");
	}

	// Draw vessels
	if (showVessels)
	{
		for (int i = 0; i < (int)oapiGetVesselCount(); i++)
		{
			OBJHANDLE vesselH = oapiGetVesselByIndex(i);
			VESSEL* veI = oapiGetVesselInterface(vesselH);

			if (veI->GetSurfaceRef() == ref)
			{
				if (!ObjectAlreadyInTarget(vesselH))
				{
					double objLong, objLat, objRad;
					GetObjectEquPos(vesselH, &objLong, &objLat, &objRad);

					// Fetch object name
					char nameString[20];
					oapiGetObjectName(vesselH, nameString, 20);

					// Finally, draw current position, so that it's on top.
					skp->SetPen(targetPosition);
					skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
					skp->SetTextColor(0x00FFFF);
					DrawFeature(objLong, objLat, 10, CROSS, skp, nameString);
				}
			}
		}
	}

	// Draw orbit track of targets
	int infoLinesDrawn = 0;
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
			char nameString[20];
			oapiGetObjectName(targets[i], nameString, 20);

			// Finally, draw current position, so that it's on top.
			skp->SetPen(targetPosition);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
			skp->SetTextColor(0x00FFFF);
			DrawFeature(currentLongTarget, currentLatTarget, 10, CROSS, skp, nameString);
			skp->SetPen(targetOrbitTrack);
			DrawFeature(currentLongTarget, currentLatTarget, 6, RINGS, skp, "");
			DrawFeature(currentLongTarget, currentLatTarget, 10, RINGS, skp, "");

			char trueDist[20], orthoDist[20];
			// Find true distance to input coordinate
			VECTOR3 coordPos, vesselPos;
			v->GetRelativePos(ref, vesselPos); // first get vector to planet centre.
			vesselPos = Ecl2Equ(vesselPos); // then convert to equatorial.
			double coordRadius = refRad + oapiSurfaceElevation(ref, currentLongTarget, currentLatTarget); // distance to centre of planet from coordinate (adding surface elevation).
			double rotationAngle = oapiGetPlanetCurrentRotation(ref); // get the angle to add.
			coordPos = _V(cos(currentLongTarget + rotationAngle) * cos(currentLatTarget), sin(currentLatTarget), sin(currentLongTarget + rotationAngle) * cos(currentLatTarget)) * coordRadius;
			FormatValue(trueDist, 20, length(vesselPos - coordPos));

			// Find ground distance. Luckily, that is easier.
			FormatValue(orthoDist, 20, oapiOrthodome(currentLongTarget, currentLatTarget, currentLong, currentLat)* refRad);

			// And also remember to draw target info
			char altStr[20];
			FormatValue(altStr, 20, currentRadTarget - refRad);
			sprintf(cbuf, "TGT: %s [%s, Alt%s, Dst%s(%s)]", nameString, GetCoordinateString(currentLongTarget, currentLatTarget), altStr, trueDist, orthoDist + 1); // FormatValue returns an annoying space in the beginning.
			skp->SetFont(configFont);
			skp->SetTextColor(0x00FFFF);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
			infoLinesDrawn++;
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
			double coordRadius = refRad + oapiSurfaceElevation(ref, longi, lati); // distance to centre of planet from coordinate (adding surface elevation).
			double rotationAngle = oapiGetPlanetCurrentRotation(ref); // get the angle to add.
			coordPos = _V(cos(longi + rotationAngle) * cos(lati), sin(lati), sin(longi + rotationAngle) * cos(lati)) * coordRadius;
			FormatValue(trueDist, 20, length(vesselPos - coordPos));

			// Find ground distance. Luckily, that is easier.
			FormatValue(orthoDist, 20, oapiOrthodome(longi, lati, currentLong, currentLat)* refRad);

			// Bearing to base
			double baseBearing = atan2(sin(longi - currentLong) * cos(lati), cos(currentLat) * sin(lati) - sin(currentLat) * cos(lati) * cos(longi - currentLong));
			baseBearing = posangle(baseBearing); // 0 ... 360 deg, reflecting information on surface HUD

			// And also remember to draw target info
			char nameStr[20];
			oapiGetObjectName(targets[i], nameStr, 20);
			sprintf(cbuf, "BSE: %s [%s, Dst%s(%s), Brg %05.1f\u00B0]", nameStr, GetCoordinateString(longi, lati), trueDist, orthoDist + 1, baseBearing * DEG);
			//sprintf(cbuf, "TGT: %s [ %s, Alt: %s]", "basibas", GetCoordinateString(longi, lati), "altialt");
			skp->SetFont(configFont);
			skp->SetTextColor(0x00FFFF);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
			infoLinesDrawn++;
		}
		else if (targets[i] == NULL) // user specified coordinate
		{
			double longiDeg, latiDeg, longi, lati;
			longiDeg = targetCoord[i][0];
			latiDeg = targetCoord[i][1];
			longi = longiDeg * RAD;
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
			double coordRadius = refRad + oapiSurfaceElevation(ref, longi, lati); // distance to centre of planet from coordinate (adding surface elevation).
			double rotationAngle = oapiGetPlanetCurrentRotation(ref); // get the angle to add.
			coordPos = _V(cos(longi + rotationAngle) * cos(lati), sin(lati), sin(longi + rotationAngle) * cos(lati)) * coordRadius;
			FormatValue(trueDist, 20, length(vesselPos - coordPos));

			// Find ground distance. Luckily, this is easier.
			FormatValue(orthoDist, 20, oapiOrthodome(longi, lati, currentLong, currentLat) * refRad);

			// Bearing to base
			double baseBearing = atan2(sin(longi - currentLong) * cos(lati), cos(currentLat) * sin(lati) - sin(currentLat) * cos(lati) * cos(longi - currentLong));
			baseBearing = posangle(baseBearing); // 0 ... 360 deg, reflecting information on surface HUD

			// Write info
			sprintf(cbuf, "COOR: [%s, Dst%s(%s), Brg %05.1f\u00B0]", GetCoordinateString(longi, lati), trueDist, orthoDist + 1, baseBearing * DEG);
			skp->SetFont(configFont);
			skp->SetTextColor(0x00FFFF);
			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);
			skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
			infoLinesDrawn++;
		}
	}

	// Draw ship historic track, but only if in ground track view
	if (orbitTrackGround)
	{
		if (shipHistoryLength == 0) // first fill with one value
		{
			shipHistory[shipHistoryIndex][0] = currentLong;
			shipHistory[shipHistoryIndex][1] = currentLat;
			shipHistory[shipHistoryIndex][2] = simt;
			shipHistoryLength = 1; // i.e. ++;
		}

		double historyTimeDelta = 10.0;
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
			//linesDrawnHistory++;

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

	// Draw periapsis and apoapsis points in ground track (if they exist)
	if (orbitTrackGround && el.e < 1.0 && !(v->GroundContact()) && !(prm.PeD < refRad && posangle(prm.MnA) > PI)) // we have an apogee, and we're not suborbital AND past apogee
	{
		double apoLong, apoLat, apoSpeed;
		GetEquPosInXSeconds(prm.ApT, el, prm, currentLong, &apoLong, &apoLat, &apoSpeed);
		skp->SetTextColor(0x00FF00);
		DrawFeature(apoLong, apoLat, 5, BOX, skp, "Ap");
	}

	if (orbitTrackGround && prm.PeD > refRad && (el.e < 1.0 || normangle(prm.TrA) < 0.0)) // we have an upcoming perigee that is above surface
	{
		double periLong, periLat, periSpeed;
		GetEquPosInXSeconds(prm.PeT, el, prm, currentLong, &periLong, &periLat, &periSpeed);
		if (prm.PeT < 0.0) sprintf(oapiDebugString(), "Warning, we're plotting perigee, but it has occured! Debug %.2f", simt);
		skp->SetTextColor(0x00FF00);
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

	// Altitude fill, a kind of landing radar
	if (displayElevationRadar)
	{
		const int radialSteps = 10;
		const int angularSteps = 16;
		const double metrePerColourStep = 10.0; // 10 m gives max range 1280 m in each axis

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

				double elevation = oapiSurfaceElevation(ref, longPoint, latPoint);

				// Set colour to middle intenisty (128) for currentElevation, and scale to metrePerColourStep
				//double colHue = 35.0;
				//double colourValue = 0.75;
				//double colourSaturation = 1.0 - abs(elevation - currentElevation) / metrePerColourStep * 1e-2;
				//// Limit to valid colour range
				//if (colourSaturation < 0) colourSaturation = 0;
				//else if (colourSaturation > 100) colourSaturation = 100;

				//double Cval = colourValue * colourSaturation;
				//double Xval = Cval * (1.0 - abs(fmod(colHue / 60.0, 2.0) - 1.0));
				//double mVal = colourValue - Cval;
				//double Rval = Cval;
				//double Gval = Xval;
				//double Bval = 0.0;

				//int valR = int((Rval + mVal) * 255.0);
				//int valG = int((Gval + mVal) * 255.0);
				//int valB = int((Bval + mVal) * 255.0);

				//DWORD elevColour = oapiGetColour(valB, valG, valR);
				int colIntentsity = 128 + int((elevation - currentElevation) / metrePerColourStep);
				if (colIntentsity < 0) colIntentsity = 0;
				else if (colIntentsity > 255) colIntentsity = 255;
				sprintf(oapiDebugString(), "colInt: %i", colIntentsity);
				DWORD elevColour = oapiGetColour(colIntentsity, colIntentsity, colIntentsity);


				// The four edges of our polygon
				double spanCoords[4][2]; // four edges of a polygon, inLeft, outLeft, outRight, inRight
				spanCoords[0][1] = asin(sin(currentLat) * cos(radialViewInner) + cos(currentLat) * sin(radialViewInner) * cos(bearingLeft));
				spanCoords[0][0] = normangle(currentLong + atan2(sin(bearingLeft) * sin(radialViewInner) * cos(currentLat), cos(radialViewInner) - sin(currentLat) * sin(spanCoords[0][1])));
				spanCoords[1][1] = asin(sin(currentLat) * cos(radialViewOuter) + cos(currentLat) * sin(radialViewOuter) * cos(bearingLeft));
				spanCoords[1][0] = normangle(currentLong + atan2(sin(bearingLeft) * sin(radialViewOuter) * cos(currentLat), cos(radialViewOuter) - sin(currentLat) * sin(spanCoords[1][1])));
				spanCoords[2][1] = asin(sin(currentLat) * cos(radialViewOuter) + cos(currentLat) * sin(radialViewOuter) * cos(bearingRight));
				spanCoords[2][0] = normangle(currentLong + atan2(sin(bearingRight) * sin(radialViewOuter) * cos(currentLat), cos(radialViewOuter) - sin(currentLat) * sin(spanCoords[2][1])));
				spanCoords[3][1] = asin(sin(currentLat) * cos(radialViewInner) + cos(currentLat) * sin(radialViewInner) * cos(bearingRight));
				spanCoords[3][0] = normangle(currentLong + atan2(sin(bearingRight) * sin(radialViewInner) * cos(currentLat), cos(radialViewInner) - sin(currentLat) * sin(spanCoords[3][1])));

				oapi::IVECTOR2 spanPixels[4];
				for (int j = 0; j < 4; j++)
				{
					double lonTra, latTra;
					TransformPoint(spanCoords[j][0], spanCoords[j][1], &lonTra, &latTra, proj);
					spanPixels[j].x = int(W / 2 + lonTra / PI * W / 2);
					spanPixels[j].y = int(H / 2 - latTra / PI05 * W / 4);
				}

				//oapi::Pen *elevPen = oapiCreatePen(1, 10, elevColour);
				oapi::Brush* elevBrush = oapiCreateBrush(elevColour);
				//skp->SetPen(elevPen);
				skp->SetPen(mainOrbitTrack);
				skp->SetBrush(elevBrush);
				skp->Polygon(spanPixels, 4);
				//oapiReleasePen(elevPen);
				oapiReleaseBrush(elevBrush);

				// DEBUG!
				skp->SetPen(mainOrbitTrack);
				skp->SetTextColor(0x00FF00);
				char lab[10];
				sprintf(lab, "%.1f m", elevation - currentElevation);
				DrawFeature(longPoint, latPoint, 2, CROSS, skp, lab);

			}
		}
	}

	// Fianlly, draw current position, so that it's on top.
	skp->SetPen(mainPosition);
	skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BOTTOM);
	skp->SetTextColor(0x00FF00);
	DrawFeature(currentLong, currentLat, 10, CROSS, skp, v->GetName());

	// And also remember to draw our own info
	char altStr[20];
	FormatValue(altStr, 20, currentRad - refRad);
	sprintf(cbuf, "SHP: %s [%s, Alt%s, Crs %05.1f\u00B0]", v->GetName(), GetCoordinateString(currentLong, currentLat), altStr, courseDirection * DEG);
	skp->SetFont(configFont);
	skp->SetTextColor(0x00FF00);
	skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);	
	skp->Text(textDX, textDY * (20 - infoLinesDrawn), cbuf, strlen(cbuf));
	infoLinesDrawn++;
}

void MapMFD::ConfigScreen(oapi::Sketchpad* skp)
{
	// Title
	char mfdTitle[50];
	sprintf(mfdTitle, "Map: Display parameters");
	Title(skp, mfdTitle); // Draws the MFD title

	int textX0 = W / 80;
	int textY0 = H / 20 * 2;
	int dY = H / 16;
	int secondRowIndent = 60;

	char cbuf[100];
	skp->SetFont(configFont); // Set to the correct font

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

	sprintf(cbuf, "Projection");
	skp->Text(textX0, textY0 + int(CONFIGPROJECTION) * dY, cbuf, strlen(cbuf));
	sprintf(cbuf, GetProjectionName());
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGPROJECTION) * dY, cbuf, strlen(cbuf));

	sprintf(cbuf, "Azimuth. Equidist. pole");
	skp->Text(textX0, textY0 + int(CONFIGFLIPPOLE) * dY, cbuf, strlen(cbuf));
	if (azimuthalEquidistantNortPole) sprintf(cbuf, "North");
	else sprintf(cbuf, "South");
	skp->Text(textX0 * secondRowIndent, textY0 + int(CONFIGFLIPPOLE) * dY, cbuf, strlen(cbuf));

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
	skp->SetFont(configFont); // Set to the correct font

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
	skp->SetFont(configFont); // Set to the correct font

	int lineNumber = -currentTargetTopListed;

	// Display bases on reference object
	sprintf(cbuf, "Spaceports >");
	skp->Text(textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
	lineNumber++;

	DWORD defaultColour = 0xFFFFFF; // white

	if (targetExpand == EXPANDSPACEPORTS)
	{
		for (int i = 0; i < (int)oapiGetBaseCount(ref); i++)
		{
			OBJHANDLE objectHandle = oapiGetBaseByIndex(ref, i);
			oapiGetObjectName(objectHandle, cbuf, 20);
			if (ObjectAlreadyInTarget(objectHandle)) skp->SetTextColor(0x00FFFF);
			skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
			skp->SetTextColor(defaultColour);
			lineNumber++;
		}
	}

	// Display vessels around reference object
	sprintf(cbuf, "Spacecraft >");
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
				oapiGetObjectName(listVessel, cbuf, 20);
				if (ObjectAlreadyInTarget(listVessel)) skp->SetTextColor(0x00FFFF);
				skp->Text(4 * textX0, textY0 + dY * lineNumber, cbuf, strlen(cbuf));
				skp->SetTextColor(defaultColour);
				lineNumber++;
			}
		}
	}

	// Display moons around reference object
	sprintf(cbuf, "Moons >");
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
					if (ObjectAlreadyInTarget(sortedPlanetsCache[i].moonHandle[k])) skp->SetTextColor(0x00FFFF);
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

bool MapMFD::DrawLine(double long0, double lat0, double long1, double lat1, oapi::Sketchpad* skp, bool safetyCheck)
{
	double transformedLongitude0 = -PI05, transformedLatitude0 = -PI05 / 2.0, transformedLongitude1 = PI05, transformedLatitude1 = PI05 / 2.0;

	TransformPoint(long0, lat0, &transformedLongitude0, &transformedLatitude0, proj);
	TransformPoint(long1, lat1, &transformedLongitude1, &transformedLatitude1, proj);

	int pxLong0 = int(W / 2 + transformedLongitude0 / PI * W / 2);
	int pxLat0 = int(H / 2 - transformedLatitude0 / PI05 * W / 4);
	int pxLong1 = int(W / 2 + transformedLongitude1 / PI * W / 2);
	int pxLat1 = int(H / 2 - transformedLatitude1 / PI05 * W / 4);

	// Finally, before plotting, ensure that the line is visible. We do this by finding the distance from the centre of the screen to the parametric line.
	// If the distance is more than the diagonal distance to the edge (hypothenuse), it must be completely outside, and not worth plotting.
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

	bool bothPointsOutside = false;
	if (((pxLong0 < 0 || pxLong0 > W) || (pxLat0 < 0 || pxLat0 > H)) && // point 0 outside
		((pxLong1 < 0 || pxLong1 > W) || (pxLat1 < 0 || pxLat1 > H))) // point 1 outside
	{
		bothPointsOutside = true;
	}

	if (bothPointsOutside) // don't add safetycheck, as no line out of bounds is useful. I'm desciding that.
	{
		// Don't plot, as the two anchor points are outside of the screen.
		pointsOutside += 1;
		return false;
	}
	else if (safetyCheck && (pow(transformedLongitude0 - transformedLongitude1, 2.0) + pow(transformedLatitude0 - transformedLatitude1, 2.0)) > 90.0 * RAD * double(centreZoom)) // multiply by centreZoom, so that we don't delete zoomed objects
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

	switch (feature)
	{
	case BOX:
		skp->Rectangle(pxX - size, pxY - size, pxX + size, pxY + size);
		skp->Text(pxX, pxY, label, strlen(label));
		return true;
	case CROSS:
		skp->Line(pxX - size, pxY, pxX + size, pxY);
		skp->Line(pxX, pxY - size, pxX, pxY + size);
		skp->Text(pxX, pxY, label, strlen(label));
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
	double futureLong, futureLat, futureSpeed;
	double previousLong = currentLong, previousLat = currentLat; // initialise to current pos, in case first step is below ground.

	if (orbitTrackGround)
	{
		double timeLimit;
		if (el.e > 1.0) timeLimit = 1e6; // orbit period not defined for hyperbolic orbit
		else timeLimit = min(1e6, prm.T * orbitTrackOrbitsNumber); // 1e6 seconds equals 11.5 days.
		// I want to have an adaptive track time step. This is especially useful for hyperbolic, but also highly eccentric elliptical orbits.
		// One such solution is to set the time step length inversely proportional to the ratio between orbital speed and maximum (perigee) speed.
		//double currentSpeed = sqrt(refMu * (2.0 * (1.0 + el.e * cos(prm.TrA)) / (el.a * (1.0 - el.e * el.e)) - 1.0 / el.a));
		//double maxSpeed;
		//if (el.e > 1.0 && prm.MnA > 0.0) maxSpeed = currentSpeed; // we're past perigee, and will always go slower than now
		//else if (prm.PeD < refRad) maxSpeed = sqrt(refMu * (2.0 / refRad - 1.0 / el.a)); // we're going to crash into the planet, so use max attained speed (crashing speed)
		//else maxSpeed = sqrt(refMu / el.a * (1.0 + el.e) / (1.0 - el.e));
		//double timeDelta = double(orbitTrackTimeDelta) * maxSpeed / currentSpeed;
		//double time = 0.0;

		double stepTrA = prm.TrA;
		double time = 0.0;
		const double angleDelta = orbitTrackAngleDelta * RAD;

		bool aboveSurface = true;
		int n = 0;

		while (aboveSurface && time < timeLimit && n < GROUND_TRACK_ITERATION_MAX_STEPS)
		{
			aboveSurface = GetEquPosInXSeconds(time, el, prm, currentLong, &futureLong, &futureLat, &futureSpeed);

			if (n > 0 && aboveSurface) // must be above surface, as we don't want to draw a line of it falling inside the planet
			{
				DrawLine(previousLong, previousLat, futureLong, futureLat, skp);

				previousLong = futureLong;
				previousLat = futureLat;
			}

			//timeDelta = double(orbitTrackTimeDelta) * maxSpeed / futureSpeed;

			stepTrA += angleDelta;
			//double EccAn = EccentricAnomaly(el.e, stepTrA);
			double futureMnA = TrA2MnA(stepTrA, el.e);
			if (el.e > 1.0)
			{
				//EccAn = 2.0 * atanh(sqrt((el.e - 1.0) / (el.e + 1.0)) * tan(stepTrA / 2.0));
				//futureMnA = el.e * sinh(EccAn) - EccAn;
				double meanMotion = sqrt(refMu / pow(-el.a, 3.0));
				time = (futureMnA - prm.MnA) / meanMotion;

				//if (tan(stepTrA / 2.0) * sqrt((el.e - 1.0) / (el.e + 1.0)) > 1.0) time = timeLimit * 2.0; // TrA can max become this certain value for a hyperbolic (physical fact, not my limitation). Then force stop loop by setting time over limit
			}
			else
			{
				double meanMotion = PI2 / prm.T;
				time = posangle(futureMnA - prm.MnA) / meanMotion + floor((stepTrA - prm.TrA) / PI2) * prm.T;
			}

			//time += timeDelta;
			n++;
		}

		if (!aboveSurface)
		{
			DrawFeature(previousLong, previousLat, 3, BOX, skp, "");
		}
	}
	else
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

	longitude = normangle(longitude - centreLong);

	switch (projection)
	{
	case EQUIRECTANGULAR:
		// https://en.wikipedia.org/wiki/Equirectangular_projection
		transLong = longitude/* * cos(centreLat)*/;
		transLat = latitude - centreLat;
		break;
	case MILLER:
		// https://en.wikipedia.org/wiki/Miller_cylindrical_projection
		transLong = longitude;
		transLat = 1.25 * log(tan(PI / 4.0 + 0.4 * latitude)) - 1.25 * log(tan(PI / 4.0 + 0.4 * centreLat));
		break;
	case MERCATOR:
		// https://en.wikipedia.org/wiki/Mercator_projection
		transLong = longitude;
		transLat = log(tan(PI / 4.0 + latitude / 2.0)) - log(tan(PI / 4.0 + centreLat / 2.0));;
		break;
	case TRANSVERSEMERCATOR:
		// https://en.wikipedia.org/wiki/Transverse_Mercator_projection
		theta = sin(longitude) * cos(latitude);
		k0 = 1.0;

		if (theta == 1.0) theta *= 0.9999; // avoid division by zero

		transLong = 0.5 * k0 * log((1.0 + theta) / (1.0 - theta));
		transLat = k0 * atan2(sin(latitude), cos(latitude) * cos(longitude)) - centreLat;
		break;
	case EQUALEARTH:
		// https://en.wikipedia.org/wiki/Equal_Earth_projection
		A1 = 1.340264, A2 = -0.081106, A3 = 0.000893, A4 = 0.003796;
		theta = asin(sqrt(3.0) / 2.0 * sin(latitude));

		transLong = 2.0 * sqrt(3.0) * longitude * cos(theta) / (3.0 * (9.0 * A4 * pow(theta, 8.0) + 7.0 * A3 * pow(theta, 6.0) + 3.0 * A2 * pow(theta, 2.0) + A1));
		transLat = A4 * pow(theta, 9.0) + A3 * pow(theta, 7.0) + A2 * pow(theta, 3.0) + A1 * (theta);
		// Now find new theta for centreLat, and subtract that from transLat.
		theta = asin(sqrt(3.0) / 2.0 * sin(centreLat));
		transLat -= A4 * pow(theta, 9.0) + A3 * pow(theta, 7.0) + A2 * pow(theta, 3.0) + A1 * (theta);
		break;
	case MOLLWEIDE: // NOT done. No centreLat.
		// https://en.wikipedia.org/wiki/Mollweide_projection
		theta = latitude;
		if (abs(latitude) > PI05 * 0.999) // should be == PI05, but due to floating point error, we have to set to a close value. This value could be something like 1 - 1e-10, but we set to 0.999. Reason: none. Ask me. 
			theta = latitude / abs(latitude) * PI05; // set to sgn(lat) * PI05.
		else
			for (int i = 0; i < 5; i++) theta = theta - (2.0 * theta + sin(2.0 * theta) - PI * sin(latitude)) / (2.0 + 2.0 * cos(2.0 * theta)); // iterate to solve equation. Using 5 legs now. Slower convergence at poles.

		transLong = 2.0 * sqrt(2.0) / PI * longitude * cos(theta);
		transLat = sqrt(2.0) * sin(theta);

		// now find new theta for centreLat, and subtract that from transLat.
		theta = centreLat;
		if (abs(centreLat) > PI05 * 0.999) // should be == PI05, but due to floating point error, we have to set to a close value. This value could be something like 1 - 1e-10, but we set to 0.999. Reason: none. Ask me. 
			theta = centreLat / abs(centreLat) * PI05; // set to sgn(lat) * PI05.
		else
			for (int i = 0; i < 5; i++) theta = theta - (2.0 * theta + sin(2.0 * theta) - PI * sin(centreLat)) / (2.0 + 2.0 * cos(2.0 * theta)); // iterate to solve equation. Using 5 legs now. Slower convergence at poles.
		transLat -= sqrt(2.0) * sin(theta);

		break;
	case ORTELIUSOVAL: // NOT done. Has centreLong, but no centreLat. Also check the first if statement, if we are to include the -centreLong term there.
		// https://en.wikipedia.org/wiki/Ortelius_oval_projection
		if (longitude == 0.0)
		{
			transLat = latitude - centreLat;
			transLong = longitude; // = 0.0
		}
		else
		{
			theta = 0.5 * (PI * PI / (4.0 * abs(longitude)) + abs(longitude));
			transLat = latitude;
			if (abs(longitude) < PI05) transLong = abs(longitude) - theta + sqrt(theta * theta - pow(transLat, 2.0));
			else transLong = sqrt(PI * PI / 4.0 - latitude * latitude) + abs(longitude) - PI05;
			if (longitude < 0.0) transLong = -abs(transLong);
			else transLong = abs(transLong);
			
			transLat -= centreLat;
		}
		break;
	case WINKELTRIPEL: // NOT done
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
	case RECTANGULARPOLYCONIC: // done, but check if atan2 is needed due to division by sin in k0
		// https://en.wikipedia.org/wiki/Rectangular_polyconic_projection
		if (latitude == 0.0) latitude = 1e-5; // avoid division by 0

		if (centreLat == 0.0) k0 = 0.5 * longitude;
		else k0 = tan(0.5 * longitude * sin(centreLat)) / sin(centreLat);

		theta = 2.0 * atan(k0 * sin(latitude));

		transLong = sin(theta) / tan(latitude);
		transLat = normangle(latitude - centreLat) + (1.0 - cos(theta)) / tan(latitude); // weigh up for the global centreLat subtraction
		break;
	case AZIMUTHALEQUIDISTANT: // done (I think. Alternavely check the atan2)
		// https://en.wikipedia.org/wiki/Azimuthal_equidistant_projection
		// Offset centreLat by PI05, so that we begin centering on the North Pole
		if (azimuthalEquidistantNortPole) // centre North
		{
			if (centreLat + PI05 == PI05)
			{
				theta = longitude;
				k0 = PI05 - latitude;
			}
			else
			{
				theta = -atan2(cos(latitude) * sin(longitude), cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
				k0 = -acos(sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude));
			}

			transLong = (k0)*sin(theta);
			transLat = -(k0)*cos(theta);
		}
		else // centre South
		{
			if (centreLat + PI05 == PI05)
			{
				theta = longitude;
				k0 = -PI05 - latitude;
			}
			else
			{
				theta = atan2(cos(latitude) * sin(longitude), cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
				k0 = acos(sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude));
			}

			transLong = (k0)*sin(-theta);
			transLat = -(k0)*cos(theta);
		}
		break;
	case LAMBERTAZIMUTHAL: // done
		// https://mathworld.wolfram.com/LambertAzimuthalEqual-AreaProjection.html
		if (abs(longitude) == PI) longitude *= 0.999; // avoid division by zero
		theta = sqrt(2.0 / (1.0 + sin(centreLat) * sin(latitude) + cos(centreLat) * cos(latitude) * cos(longitude)));

		transLong = theta * cos(latitude) * sin(longitude);
		transLat = theta * (cos(centreLat) * sin(latitude) - sin(centreLat) * cos(latitude) * cos(longitude));
		break;
	case CASSINI: // NOT done, but check atan2
		// https://en.wikipedia.org/wiki/Cassini_projection
		transLong = asin(cos(latitude) * sin(longitude));
		transLat = atan2(sin(latitude), cos(latitude) * cos(longitude)) - centreLat;
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

bool MapMFD::GetEquPosInXSeconds(double t, ELEMENTS el, ORBITPARAM prm, double currentLongitude, double* longitude, double* latitude, double *currentSpeed)
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
	if (el.e > 1.0) M = M0 + sqrt(refMu / pow(-el.a, 3.0)) * t; // http://control.asu.edu/Classes/MAE462/462Lecture05.pdf page 37.
	else M = fmod(M0 + PI2 * t / prm.T, PI2);
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
	*currentSpeed = sqrt(refMu * (2.0 * (1.0 + el.e * cos(TrA)) / (el.a * (1.0 - el.e * el.e)) - 1.0 / el.a)); // for dynamic updating of orbit track

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


	// Now we have state vectors in equatorial frame. Now calculate KOST.
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
	double period = PI2 * sqrt(pow(SMa, 3.0) / refMu);
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
	prm->T = period;
	//if (el.e > 1.0) prm->T = -1.0; // give it a sane value, for calculations sake
	prm->MnA = MnA;
}

char* MapMFD::GetProjectionName(void)
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

char* MapMFD::GetCoordinateString(double longitude, double latitude)
{
	char coordinate[50];
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
