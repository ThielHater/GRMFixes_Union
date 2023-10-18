// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	template <class T>
	T zLerp(const float t, const T& a, const T& b) {
		return a + (b - a) * t;
	}

	float zSinusEase(float t)
	{
		const float _PI = 3.14159265358979323846F;
		return float((sinf(t * _PI - _PI / 2) + 1) / 2);
	};

	void zClamp(float& value, float bl, float bh)
	{
		if (value < bl)
			value = bl;
		else if (value > bh)
			value = bh;
	}

	void zClamp01(float& f)
	{
		if ((*(zDWORD*)&f) >= (zDWORD)0x80000000)
			f = 0.f;
		else if ((*(zDWORD*)&f) > (zDWORD)0x3f800000)
			f = 1.0F;
	}

	float zMin(const float a, const float b)
	{
		if (a < b)
			return a;
		else
			return b;
	}

	float zMax(const float a, const float b)
	{
		if (a > b)
			return a;
		else
			return b;
	}

	float GetIntensity(float r, float g, float b)
	{
		return (0.299F) * r + (0.587F) * g + (0.114F) * b;
	};

	float GetIntensity(zCOLOR col)
	{
		return GetIntensity(col.r, col.g, col.b);
	};

	float GetIntensity(zVEC3 col)
	{
		return GetIntensity(col[0], col[1], col[2]);
	};
	
	float GetSkyFadeWeight(zCZoneZFog* fogZone)
	{
		float dist = fogZone->GetCamPosWeight();
		if (dist < fogZone->innerRangePerc)
			dist = 0;
		else
			dist = (dist - fogZone->innerRangePerc) / (1.0F - fogZone->innerRangePerc);
		return dist;
	};

	zVEC3 g_overrideColor = zVEC3(0);
	BOOL g_bOverrideColorFlag = FALSE;
	zCOLOR g_resultFogColor = zCOLOR(0);
	zCOLOR g_resultFogColorOverride = zCOLOR(0);

	// 0x0060C090 public: virtual void __thiscall zCZoneZFog::ProcessZoneList(class zCArraySort<class zCZone *> const &,class zCArraySort<class zCZone *> const &,class zCWorld *)
	void __fastcall zCZoneZFog_ProcessZoneList(zCZoneZFog* _this, void* vtable, zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* world);
	CInvoke<void(__thiscall*)(zCZoneZFog* _this, zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* homeWorld)> Ivk_zCZoneZFog_ProcessZoneList(GothicMemoryLocations::zCZoneZFog::ProcessZoneList, &zCZoneZFog_ProcessZoneList);

	void __fastcall zCZoneZFog_ProcessZoneList(zCZoneZFog* _this, void* vtable, zCArraySort<zCZone*> const& zoneList, zCArraySort<zCZone*> const& zoneDeactivateList, zCWorld* world)
	{
		if ((zoneList.GetNum() == 0) && (zoneDeactivateList.GetNum() == 0))
			return;

		float fadeOutSkyWeight = 0;
		float fadeOutColorWeight = 0;
		zCOLOR backGroundColor = g_resultFogColor;
		float dayWeight = (GetIntensity(backGroundColor) / 255.0f) * 3;
		float avgRed = 0;
		float avgGreen = 0;
		float avgBlue = 0;
		int numFogZonesModColor = 0;

		zCZone* zoneDefault = world->SearchZoneDefaultByClass(zCZoneZFogDefault::classDef);
		zClamp(dayWeight, 0.1f, 1.0f);

		float defaultRange = zoneDefault
			? ((zCZoneZFog*)zoneDefault)->fogRangeCenter
			: world->GetActiveSkyControler()->GetFarZ();

		float range = 0;
		if (zoneList.GetNum() <= 0)
		{
			range = defaultRange;
		}
		else
		{
			for (int i = 0; i < zoneList.GetNum(); i++)
			{
				zCZoneZFog* zone = ((zCZoneZFog*)zoneList[i]);
				range += zone->GetActiveRange(defaultRange);
				float zoneWeight = GetSkyFadeWeight(zone);

				numFogZonesModColor++;
				float zw = 1 - zoneWeight;
				avgRed += zw * (float)zone->fogColor.r;
				avgGreen += zw * (float)zone->fogColor.g;
				avgBlue += zw * (float)zone->fogColor.b;
				fadeOutColorWeight += zw;
			};

			range /= (float)zoneList.GetNum();
		};

		world->GetActiveSkyControler()->SetFarZ(range);

		if (numFogZonesModColor >= 1)
		{
			zVEC3 origColor(backGroundColor.r, backGroundColor.g, backGroundColor.b);
			zVEC3 overrideColor((avgRed * dayWeight) / numFogZonesModColor, (avgGreen * dayWeight) / numFogZonesModColor, (avgBlue * dayWeight) / numFogZonesModColor);
			g_overrideColor = zLerp(zSinusEase(fadeOutColorWeight / numFogZonesModColor), origColor, overrideColor);
			g_bOverrideColorFlag = TRUE;
		}
		else
		{
			g_bOverrideColorFlag = FALSE;
		}
	}

	// 0x005C01A0 private: void __thiscall zCSkyControler_Outdoor::CalcFog(void)
	void __fastcall zCSkyControler_Outdoor_CalcFog(zCSkyControler_Outdoor* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyControler_Outdoor* _this)> Ivk_zCSkyControler_Outdoor_CalcFog(GothicMemoryLocations::zCSkyControler_Outdoor::CalcFog, &zCSkyControler_Outdoor_CalcFog);

	void __fastcall zCSkyControler_Outdoor_CalcFog(zCSkyControler_Outdoor* _this, void* vtable)
	{
		if (_this->GetUnderwaterFX())
		{
			_this->resultFogColor = _this->underwaterColor;
			g_resultFogColor = _this->resultFogColor;
			g_resultFogColorOverride = _this->resultFogColor;
			_this->resultFogSkyFar = 1000.0F;
			_this->resultFogSkyNear = 0.0F;
			_this->resultFogFar = _this->underwaterFarZ;
			_this->resultFogNear = 0.0F;
			return;
		};

		float userFogMid = _this->userFogFar * 0.4F;
		float userFogDelta = _this->userFogFar - userFogMid;
		float fogTimeScale = _this->masterState.fogDist;
		float ypos = zCCamera::activeCam->connectedVob->GetPositionWorld()[VY];
		float fogYScale = _this->heightFogMaxY - _this->heightFogMinY != 0
			? (ypos - _this->heightFogMinY) / (_this->heightFogMaxY - _this->heightFogMinY)
			: 0;
		zClamp01(fogYScale);

		float resScale = zMax(fogTimeScale, fogYScale);
		_this->resultFogScale = resScale;
		resScale = 1.0F - resScale;

		_this->resultFogFar = userFogMid + resScale * userFogDelta;
		_this->resultFogFar *= _this->userFarZScalability;
		_this->resultFogNear = 0.3f * _this->resultFogFar;

		if (_this->rainFX.outdoorRainFXWeight > 0)
		{
			static float nearestFog = -2500.0f;
			_this->resultFogNear = zLerp(_this->rainFX.outdoorRainFXWeight, _this->resultFogNear, nearestFog);
		};

		float skyUserFar = zMin(40000.0F, _this->resultFogFar * 1.5F) * 0.5F;
		float skyUserMid = zMax(10000.0F, _this->resultFogNear * 1.5F) * 0.5F;
		_this->resultFogSkyFar = skyUserMid + resScale * (skyUserFar - skyUserMid);
		_this->resultFogSkyNear = 0.25F * _this->resultFogSkyFar;

		if ((_this->rainFX.outdoorRainFXWeight > 0))
		{
			_this->resultFogSkyNear = 0.10F * _this->resultFogSkyFar;
			_this->resultFogSkyFar *= 0.8F;
		};

		float intens = GetIntensity(_this->masterState.fogColor);
		if (intens > 120.0F)
			intens = 120.0F;
		zVEC3 grey = zVEC3(intens);
		float s = _this->resultFogScale * 0.5F;
		zVEC3 col = _this->masterState.fogColor * (1.0F - s) + grey * s;

		if (_this->rainFX.outdoorRainFXWeight > 0)
			col = zLerp(_this->rainFX.outdoorRainFXWeight, col, grey);

		g_resultFogColor.r = col[0];
		g_resultFogColor.g = col[1];
		g_resultFogColor.b = col[2];

		_this->resultFogColor = g_resultFogColor;

		intens = GetIntensity(g_overrideColor);
		if (intens > 120.0F)
			intens = 120.0F;
		grey = zVEC3(intens);
		col = g_overrideColor * (1.0F - s) + grey * s;

		if (_this->rainFX.outdoorRainFXWeight > 0)
			col = zLerp(_this->rainFX.outdoorRainFXWeight, col, grey);

		g_resultFogColorOverride.r = col[0];
		g_resultFogColorOverride.g = col[1];
		g_resultFogColorOverride.b = col[2];

		if (g_bOverrideColorFlag)
		{
			// Hooking of RenderSetup(), where the switch between resultFogColor and resultFogColorOverride resides, was not successful for some reason. Therefore I override the value here.
			_this->resultFogColor = g_resultFogColorOverride;
		}
	}
}
