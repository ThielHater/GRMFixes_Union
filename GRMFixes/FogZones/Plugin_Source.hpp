// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	float g_farZ = 15000.0f;

	float SinusEase(float t)
	{
		const float _PI = 3.14159265358979323846F;
		return float((sinf(t * _PI - _PI / 2) + 1) / 2);
	};

	zVEC3 ColorLerp(zVEC3 pC1, zVEC3 pC2, float s)
	{
		zVEC3 result = { pC1[0] + s * (pC2[0] - pC1[0]), pC1[1] + s * (pC2[1] - pC1[1]), pC1[2] + s * (pC2[2] - pC1[2]) };
		return result;
	}

	float GetCamPosWeight(zCZoneZFog* zone, zVEC3 posCam)
	{
		zVEC3 rel = posCam - zone->GetPositionWorld();
		zVEC3 scale = zone->bbox3D.mins - zone->bbox3D.maxs;
		scale *= 0.5f;
		scale[0] = 1.0f / scale[0];
		scale[1] = 1.0f / scale[1];
		scale[2] = 1.0f / scale[2];
		zVEC3 dot = { rel[0] * scale[0], rel[1] * scale[1], rel[2] * scale[2] };
		float dist = dot.Length();
		if (dist > 1.0f)
			dist = 1.0f;
		return dist;
	};

	float GetSkyFadeWeight(zCZoneZFog* zone, zVEC3 posCam)
	{
		float dist = GetCamPosWeight(zone, posCam);
		if (dist < zone->innerRangePerc)
			dist = 0;
		else
			dist = (dist - zone->innerRangePerc) / (1.0f - zone->innerRangePerc);
		return dist;
	};

	float GetActiveRange(zCZoneZFog* zone, zVEC3 posCam, float fogRangeDefault)
	{
		float dist = GetCamPosWeight(zone, posCam);
		if (dist < zone->innerRangePerc)
			dist = 0;
		else
			dist = (dist - zone->innerRangePerc) / (1.0f - zone->innerRangePerc);
		float range = zone->fogRangeCenter + dist * (fogRangeDefault - zone->fogRangeCenter);
		return range;
	};

	extern "C" __declspec(dllexport) void SetFogColorAndRange()
	{
		if (gameMan && gameMan->gameSession && gameMan->gameSession->world && gameMan->gameSession->world->skyControlerOutdoor && gameMan->gameSession->camVob)
		{
			zCSkyControler_Outdoor* sky = (zCSkyControler_Outdoor*)gameMan->gameSession->world->skyControlerOutdoor;
			zCZone** activeZones = gameMan->gameSession->world->zoneActiveList.GetArray();
			int activeZonesNum = gameMan->gameSession->world->zoneActiveList.GetNum();

			// Bestimmung der Kameraposition
			zVEC3 posCam = gameMan->gameSession->camVob->GetPositionWorld();

			// Berechnung der Uhrzeit
			float skyTime = sky->masterTime;
			long totalSecs = (long)(skyTime / 0.00001157407f);
			int hours = totalSecs / 3600;
			int minutes = (totalSecs % 3600) / 60;
			if (skyTime >= 0.5f) hours -= 12; else hours += 12;
			int totalMinutes = hours * 60 + minutes;

			// Standardsichtweite ermitteln
			float fogRange = 0.0f;
			float fogRangeDefault = 15000.0f;
			for (int i = 0; i < activeZonesNum; i++)
			{
				if (*(int*)activeZones[i] == 0x7DC28C) // zCZoneZFogDefault
				{
					fogRangeDefault = ((zCZoneZFogDefault*)activeZones[i])->fogRangeCenter;
					break;
				}
			}

			// SkyStates zwischen denen interpoliert wird
			const int maxSkyStates = 12;
			struct mySkyState skyStates[maxSkyStates] =
			{
				{ 0, 210,{ 5, 5, 20 } }, // 00:00 - 03:30
				{ 210, 75,{ 5, 5, 20 } }, // 03:30 - 04:45
				{ 285, 75,{ 78, 63, 96 } }, // 04:45 - 06:00
				{ 360, 630,{ 116, 93, 81 } }, // 06:00 - 16:30
				{ 990, 30,{ 116, 93, 81 } }, // 16:30 - 17:00
				{ 1020, 30,{ 116, 116, 116 } }, // 17:00 - 17:30
				{ 1050, 30,{ 116, 93, 81 } }, // 17:30 - 18:00
				{ 1080, 60,{ 116, 93, 81 } }, // 18:00 - 19:00
				{ 1140, 60,{ 162, 84, 71 } }, // 19:00 - 20:00
				{ 1200, 90,{ 73, 43, 62 } }, // 20:00 - 21:30
				{ 1290, 90,{ 17, 17, 48 } }, // 21:30 - 23:00
				{ 1380, 60,{ 5, 5, 20 } } // 23:00 - 00:00
			};

			// Grundton berechnen
			zVEC3 fogColorDefault;
			for (int i = 0; i < maxSkyStates; i++)
			{
				int j = (i + 1 < maxSkyStates) ? i + 1 : 0;
				if ((totalMinutes >= skyStates[i].start) && (totalMinutes <= skyStates[i].start + skyStates[i].duration))
				{
					int duration = (totalMinutes - skyStates[i].start);
					float backgroundWeight = (float)duration / (float)skyStates[i].duration;
					fogColorDefault = ColorLerp(skyStates[i].color, skyStates[j].color, backgroundWeight);
					break;
				}
			}

			// zusätzliche Farbanteile der Fogzones berechnen
			zVEC3 fogColor;
			float avgRed = 0.0f, avgGreen = 0.0f, avgBlue = 0.0f;
			float sumFogZoneWeight = 0.0f;
			int countFogZones = 0;

			// alle Zones durchlaufen
			for (int i = 0; i < activeZonesNum; i++)
			{
				if (*(int*)activeZones[i] == 0x7DC64C) // zCZoneFog
				{
					zCZoneZFog* zone = (zCZoneZFog*)activeZones[i];

					// Gewichtung der Zone berechnen
					float zoneWeight = GetSkyFadeWeight(zone, posCam);

					// Ist der Spieler außerhalb der Zone?
					if (zoneWeight == 1.0f)
						continue;

					// Farbanteile und Gewichtung aufaddieren, Anzahl der Zonen inkrementieren
					float zw = 1 - zoneWeight;
					avgRed += zw * zone->fogColor.r;
					avgGreen += zw * zone->fogColor.g;
					avgBlue += zw * zone->fogColor.b;
					sumFogZoneWeight += zw;
					countFogZones++;

					// Sichweite aufaddieren
					fogRange += GetActiveRange(zone, posCam, fogRangeDefault);
				}
			}

			// Ist der Spieler in keiner Fogzone?
			if (countFogZones == 0)
			{
				// Grundton und Standardsichtweite verwenden
				fogColor = fogColorDefault;
				fogRange = fogRangeDefault;
			}
			else
			{
				// Grundton mit den zusätzlichen Farbanteilen mischen und Sichtweite berechnen
				zVEC3 avgColor = { avgRed / countFogZones, avgGreen / countFogZones, avgBlue / countFogZones };
				fogColor = ColorLerp(fogColorDefault, avgColor, SinusEase(sumFogZoneWeight / countFogZones));
				fogRange = fogRange / countFogZones;
			}

			// Farbe anwenden
			if (sky->state0)
				sky->state0->fogColor = fogColor;
			if (sky->state1)
				sky->state1->fogColor = fogColorDefault;

			// Sichtweite merken
			g_farZ = fogRange;
		}
	}

	// 0x00716F60 public: virtual void __thiscall zCRnd_D3D::BeginFrame(void)
	void __fastcall zCRnd_D3D_BeginFrame(zCRnd_D3D* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCRnd_D3D * _this)> Ivk_zCRnd_D3D_BeginFrame(GothicMemoryLocations::zCRnd_D3D::BeginFrame, &zCRnd_D3D_BeginFrame);

	void __fastcall zCRnd_D3D_BeginFrame(zCRnd_D3D* _this, void* vtable)
	{
		SetFogColorAndRange();
		Ivk_zCRnd_D3D_BeginFrame(_this);
	}

	// 0x005C0540 public: virtual void __thiscall zCSkyControler_Outdoor::SetFarZ(float)
	void __fastcall zCSkyControler_Outdoor_SetFarZ(zCSkyControler_Outdoor* _this, void* vtable, float farZ);
	CInvoke<void(__thiscall*)(zCSkyControler_Outdoor * _this, float farZ)> Ivk_zCSkyControler_Outdoor_SetFarZ(GothicMemoryLocations::zCSkyControler_Outdoor::SetFarZ, &zCSkyControler_Outdoor_SetFarZ);

	void __fastcall zCSkyControler_Outdoor_SetFarZ(zCSkyControler_Outdoor* _this, void* vtable, float farZ)
	{
		Ivk_zCSkyControler_Outdoor_SetFarZ(_this, g_farZ);
	}
}
