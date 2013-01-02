/////////////////////////////////////////////////////////////////////////////
// sdinterf.h : Definition of Sigma Designs proprietary interfaces.
// SDWDM DirectShow Filter for WDM
// Created by Jacques MAHE
// Copyright Sigma Designs Inc
// Sigma Designs Proprietary and confidential
// Created on 09/09/98
// This file contains the CLSID (use #include "initguid.h" in one of the file
// of your project before including this header file") and the Interfaces 
// definition of all the Sigma Decoder Filter exported interfaces, except 
// IID_IRmHardware & IRmHardware (You will need "imhard.h").

/////////////////////////////////////////////////////////////////////////////
// MPEG DirectShow Decoder Filter (RMMPEG2.AX) CLSID

// {4E3ABD41-458E-11d1-917e-00001B4F006F}
DEFINE_GUID(CLSID_RM_MPEG2_VIDEO_SUBPICTURE,
0x4e3abd41, 0x458e, 0x11d1, 0x91, 0x7e, 0x0, 0x0, 0x1b, 0x4f, 0x0, 0x6f);

/////////////////////////////////////////////////////////////////////////////
// MPEG DirectShow Decoder Filter Interface IDs.

// {6EB47900-4815-11d2-93DC-0080ADB67301}
DEFINE_GUID(IID_IVideoAdjust, 
0x6eb47900, 0x4815, 0x11d2, 0x93, 0xdc, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// {FC5F1820-527B-11d2-A86E-0040054237F6}
DEFINE_GUID(IID_IRegionCodeControl, 
0xfc5f1820, 0x527b, 0x11d2, 0xa8, 0x6e, 0x0, 0x40, 0x5, 0x42, 0x37, 0xf6);

// {AEB23DA0-54BB-11d2-BD98-0080ADB67301}
DEFINE_GUID(IID_IAudioOutput, 
0xaeb23da0, 0x54bb, 0x11d2, 0xbd, 0x98, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// {4246FA00-52E2-11d2-BD98-0080ADB67301}
DEFINE_GUID(IID_IREALmagicProfile, 
0x4246fa00, 0x52e2, 0x11d2, 0xbd, 0x98, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// {F1906E01-D722-11d2-B2AC-00400542D89C}
DEFINE_GUID(IID_IOverlayConfig, 
0xf1906e01, 0xd722, 0x11d2, 0xb2, 0xac, 0x0, 0x40, 0x5, 0x42, 0xd8, 0x9c);

// {34860D40-488F-11d3-9E5F-0090272CFCC9}
DEFINE_GUID(IID_ITvVgaOutput, 
0x34860d40, 0x488f, 0x11d3, 0x9e, 0x5f, 0x0, 0x90, 0x27, 0x2c, 0xfc, 0xc9);

// {0229D3E0-162E-11d5-9E5F-0090272CFCC9}
DEFINE_GUID(IID_ITvVgaOutput2,
0x229d3e0, 0x162e, 0x11d5, 0x9e, 0x5f, 0x0, 0x90, 0x27, 0x2c, 0xfc, 0xc9);

// {7425EEB6-FE67-4984-875D-2B3D939F8773}
DEFINE_GUID(IID_ITvVgaOutput3, 
0x7425eeb6, 0xfe67, 0x4984, 0x87, 0x5d, 0x2b, 0x3d, 0x93, 0x9f, 0x87, 0x73);

// {28029D11-F764-11d4-9C1A-0090272CFCC9}
DEFINE_GUID(IID_IMPEGTimeInfo, 
0x28029d11, 0xf764, 0x11d4, 0x9c, 0x1a, 0x0, 0x90, 0x27, 0x2c, 0xfc, 0xc9);

/////////////////////////////////////////////////////////////////////////////
// Decoder filter property pages CLSIDs.

// {5F687880-3CF2-11d2-93DC-0080ADB67301}
DEFINE_GUID(CLSID_SdConfigPropertyPage, 
0x5f687880, 0x3cf2, 0x11d2, 0x93, 0xdc, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// {34EE23E0-71CE-11d3-9643-00400542D89C}
DEFINE_GUID(CLSID_SdTvOutPropertyPage, 
0x34ee23e0, 0x71ce, 0x11d3, 0x96, 0x43, 0x0, 0x40, 0x5, 0x42, 0xd8, 0x9c);

// {5F687880-3CF2-11d2-93DC-0080ADB67301}
DEFINE_GUID(CLSID_SdOverlayPropertyPage, 
0x5f687881, 0x3cf2, 0x11d2, 0x93, 0xdc, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

/////////////////////////////////////////////////////////////////////////////
// CLSID not in relation with the decoder filter (WDM related only).

// {D2CB58A0-5567-11d2-BD98-0080ADB67301}
DEFINE_GUID(QUASAR_VIDEO_ADJUSTMENT, 
0xd2cb58a0, 0x5567, 0x11d2, 0xbd, 0x98, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// {5A564AA1-0D38-11d2-93DC-0080ADB67301}
DEFINE_GUID(QUASAR_DEVICE_PROPERTY,
0x5a564aa1, 0xd38, 0x11d2, 0x93, 0xdc, 0x0, 0x80, 0xad, 0xb6, 0x73, 0x1);

// GUID we created to support audio under NT 4.0 when no sound card!

// {020FB9A0-CC3E-11d3-9643-00400542D89C}
DEFINE_GUID(MEDIATYPE_SigmaAudio, 
0x20fb9a0, 0xcc3e, 0x11d3, 0x96, 0x43, 0x0, 0x40, 0x5, 0x42, 0xd8, 0x9c);

// GUID Created to support MPEG4 format

// {8AB4D3D1-C812-11d5-BE17-00A0C90AA8A1}
DEFINE_GUID(MEDIASUBTYPE_SDMPEG4_Video, 
0x8ab4d3d1, 0xc812, 0x11d5, 0xbe, 0x17, 0x0, 0xa0, 0xc9, 0xa, 0xa8, 0xa1);

// {8AB4D3D2-C812-11d5-BE17-00A0C90AA8A1}
DEFINE_GUID(MEDIASUBTYPE_SDMPEG4_PCMAudio, 
0x8ab4d3d2, 0xc812, 0x11d5, 0xbe, 0x17, 0x0, 0xa0, 0xc9, 0xa, 0xa8, 0xa1);

// {8AB4D3D0-C812-11d5-BE17-00A0C90AA8A1}
DEFINE_GUID (CLSID_SDMP4FileSource,
0x8ab4d3d0, 0xc812, 0x11d5, 0xbe, 0x17, 0x0, 0xa0, 0xc9, 0xa, 0xa8, 0xa1);

// {C49DBDC0-C81A-11d5-BE17-00A0C90AA8A1}
DEFINE_GUID (CLSID_SDMPEG4_System, 
0xc49dbdc0, 0xc81a, 0x11d5, 0xbe, 0x17, 0x0, 0xa0, 0xc9, 0xa, 0xa8, 0xa1);

// {325222F8-8C6C-4a39-A76A-A50356D207DA}
DEFINE_GUID(FORMAT_SigmaDataFormat, 
0x325222f8, 0x8c6c, 0x4a39, 0xa7, 0x6a, 0xa5, 0x3, 0x56, 0xd2, 0x7, 0xda);

/////////////////////////////////////////////////////////////////////////////
// Interface definition (No more CLSIDs)

#ifndef __sdinterf_h
#define __sdinterf_h

#include "mpegcmn.h"

// Hardware flags
#define SDCAP_ANALOG_OVERLAY			0x00000001
#define SDCAP_AUDIO_DAC					0x00000002
#define SDCAP_AUDIO_SPDIFF				0x00000004
#define SDCAP_TV_OUT					0x00000008
#define SDCAP_TV_AND_VGA_OUT			0x00000010
#define SDCAP_EEPROM					0x00000020
#define SDCAP_BSC_ADJUST				0x00000040	// Brightness/Contrast/Saturation
#define SDCAP_TV_NTSC_ONLY				0x00000080	// IGS special
#define SDCAP_TV_PAL_ONLY				0x00000100	// IGS special
#define SDCAP_TV_PAL60_ONLY				0x00000200	// IGS special
#define SDCAP_TV_FORCED_STANDARD_MASK	0x00000380
#define SDCAP_AUTO_CALIBRATE_POSITION	0x00000400
#define SDCAP_HDTV_OUT					0x00000800
#define SDCAP_COMPONENT_RGB_SUPPORT		0x00001000
#define SDCAP_COMPONENT_YUV_SUPPORT		0x00002000
#define SDCAP_480P						0x00004000	// Ventura 2000 only (VGA connector)

#define SDCAP_TV_OUT_OPTIONS_MASK		0x00000088	// No TV out property sheet when 0

// Structure used to retrieve system resources
#ifndef _Quasar_KSResources
#define _Quasar_KSResources
typedef struct _SYSTEM_RESOURCES
{
	ULONG PhysicalAddress;
	ULONG LinearAddress;
	ULONG HardwareFlags;
    ULONG BoardId;
	ULONG UserAddress;
	ULONG VideoDmaPhysicalAddress;
	ULONG VideoDmaLinearAddress;
	ULONG VideoDmaBufferSize;
	ULONG AudioDmaPhysicalAddress;
	ULONG AudioDmaLinearAddress;
	ULONG AudioDmaBufferSize;
	ULONG SubpictureDmaPhysicalAddress;
	ULONG SubpictureDmaLinearAddress;
	ULONG SubpictureDmaBufferSize;
} SYSTEM_RESOURCES, *PSYSTEM_RESOURCES;
#endif

#ifndef _Quasar_KSProperty
#define _Quasar_KSProperty
typedef enum 
{
    QUASAR_KSPROPERTY_RESOURCES,
	QUASAR_KSPROPERTY_DISPLAYRESOLUTION,
	QUASAR_KSPROPERTY_ENCRYPTEDTITLEKEY,
	QUASAR_KSPROPERTY_CHECK_SOFTICE,
	QUASAR_KSPROPERTY_SAVE_ORIGINAL_DATA,
	QUASAR_KSPROPERTY_GET_DESP_FUNC_TABLE,
	QUASAR_KSPROPERTY_GET_MONITOR_VALUE,
	QUASAR_KSPROPERTY_END_TAMPER,
	QUASAR_KSPROPERTY_EEPROM_SET_ADDRESS,
	QUASAR_KSPROPERTY_EEPROM_ACCESS,
	QUASAR_KSPROPERTY_AUDIO_OUT,
	QUASAR_KSPROPERTY_MAP_SIDT,
	QUASAR_KSPROPERTY_UNMAP_SIDT,
	QUASAR_KSPROPERTY_MEMMAP_THE_MEMORY,
	QUASAR_KSPROPERTY_MEM_UNMAP_THE_MEMORY,
	QUASAR_KSPROPERTY_CALC_FREQUENCY,
	QUASAR_KSPROPERTY_HARDWARE_OWNER
} QUASAR_KSPROPERTY;
#endif _Quasar_KSProperty

// {44F84FC0-D7A7-11d2-B2AC-00400542D89C}
DEFINE_GUID(QUASAR_OVERLAY_CONFIG, 
0x44f84fc0, 0xd7a7, 0x11d2, 0xb2, 0xac, 0x0, 0x40, 0x5, 0x42, 0xd8, 0x9c);

#ifndef _Quasar_KSOverlayMode
#define _Quasar_KSOverlayMode
typedef enum 
{
    KSPROPERTY_OVERLAY_MODE,
	KSPROPERTY_OVERLAY_VIDEOPOSITION,
	KSPROPERTY_OVERLAY_TOP_BORDER,
	KSPROPERTY_OVERLAY_LEFT_BORDER,
	KSPROPERTY_OVERLAY_VGA_CORRECTION,
	KSPROPERTY_OVERLAY_CALIBRATION_PARAMETERS,
	KSPROPERTY_OVERLAY_JITTER_ADJUSTMENT,
	KSPROPERTY_OVERLAY_VGA_FINE_ADJUSTMENT,
	KSPROPERTY_OVERLAY_START_CALIBRATION,
	KSPROPERTY_OVERLAY_STOP_CALIBRATION,
	KSPROPERTY_OVERLAY_CALIBRATE_TOP_BORDER,
	KSPROPERTY_OVERLAY_CALIBRATE_LEFT_BORDER,
	KSPROPERTY_OVERLAY_CALIBRATE_CORRECTION,
	KSPROPERTY_OVERLAY_CALIBRATE_COLOR_LIMITS
} OVERLAY_CONFIG_KSPROPERTY;
#endif

// Flag for the Analog Overlay Modes
typedef enum _OVERLAY_MODE
{
    OverlayModeNone = 1,
    OverlayModeRectangle,
    OverlayModeOverlay
} OVERLAY_MODE;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Min and max values for Brightness, Saturation and Contrast. Defaut is middle range.
#ifndef MIN_BRIGHTNESS

#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 1000
#define MIN_CONTRAST 0
#define MAX_CONTRAST 1000
#define MIN_SATURATION 0
#define MAX_SATURATION 1000

#endif

/////////////////////////////////////////////////////////////////////////////
// interface IVideoAdjust used for brightness, saturation and contrast.

interface IVideoAdjust : public IUnknown
{
public:
    /* IVideoAdjust methods */
    STDMETHOD(put_Brightness)(THIS_ long lBrightness) PURE;
    STDMETHOD(get_Brightness)(THIS_ long FAR* plBrightness) PURE;
    STDMETHOD(put_Saturation)(THIS_ long lSaturation) PURE;
    STDMETHOD(get_Saturation)(THIS_ long FAR* plSaturation) PURE;
    STDMETHOD(put_Contrast)(THIS_ long lContrast) PURE;
    STDMETHOD(get_Contrast)(THIS_ long FAR* plContrast) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// interface IRegionCodeControl used for region code.

// Return Codes
#define NO_EEPROM					0xAA
#define EEPROM_TAMPERED				0xEE

// Region Code Functions
#define SET_REGION					0x01
#define GET_REGION					0x02
#define GET_COUNTER_A				0x03
#define GET_COUNTER_B				0x04
#define RESET_BOARD					0x05

interface IRegionCodeControl: public IUnknown
{
public:
    /* IRegionCodeControl methods */
    STDMETHOD(RegionCodeControl)	(THIS_ DWORD dwFunctionCode, BYTE *pbReturnCode) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// interface IREALmagicProfile used to save / retrieve REALmagic settings.

interface IREALmagicProfile : public IUnknown
{
public:
    STDMETHOD(GetInt)(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault, int *pValue) PURE;
	STDMETHOD(WriteInt)(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue) PURE;
	STDMETHOD(GetString)(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, LPSTR szString, int iSize) PURE;
	STDMETHOD(WriteString)(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue) PURE;
    STDMETHOD(GetREALmagicID)() PURE;
    STDMETHOD(GetHardwareFlags)() PURE;
    STDMETHOD(UpdateDisplaySize)() PURE;			// Update display resolution
    STDMETHOD(GetDisplayInt)(LPCTSTR lpszEntry, int nDefault, int *pValue) PURE;
	STDMETHOD(WriteDisplayInt)(LPCTSTR lpszEntry, int nValue) PURE;
    STDMETHOD(CalibrationDone)(BOOL *bDone) PURE;	// Check if calibration has been done for the current resolution
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// The different audio output format
#ifndef _Quasar_AudioOutputType
#define _Quasar_AudioOutputType
typedef enum
{
	DIGITAL_AUDIO,		// Through On-board DAC
	PCM_AUDIO,			// Through Quasar SpDiff channel, no AC-3
	AC3_AUDIO			// Through Quasar SpDiff channel
} AUDIO_OUTPUT, *PAUDIO_OUTPUT;
#endif

// Audio channel
#ifndef _Quasar_AudioOutputMode
#define _Quasar_AudioOutputMode
typedef enum 
{
	STEREO,
	LEFT_ONLY,			// Right channel output on both speakers
	RIGHT_ONLY			// Left channel output on both speakers
} AUDIO_CHANNEL, *PAUDIO_CHANNEL;
#endif

/////////////////////////////////////////////////////////////////////////////
// interface IAudioOutput used to get/set audio output (Digital, PCM or AC3)

interface IAudioOutput : public IUnknown
{
public:
    STDMETHOD(GetOutput)(PAUDIO_OUTPUT pAudioOutput) PURE;
	STDMETHOD(SetOutput)(AUDIO_OUTPUT AudioOutput) PURE;
	STDMETHOD(GetChannel)(PAUDIO_CHANNEL pAudioChannel) PURE;
	STDMETHOD(SetChannel)(AUDIO_CHANNEL AudioChannel) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// interface IOverlayConfig used to adjust overlay parameters

interface IOverlayConfig : public IUnknown
{
public:
	// Set and get overlay mode
    STDMETHOD(put_OverlayMode)(THIS_ OVERLAY_MODE OverlayMode) PURE;
    STDMETHOD(get_OverlayMode)(THIS_ OVERLAY_MODE FAR* pOverlayMode) PURE;

	// Set and get overlay position
    STDMETHOD(put_OverlayPosition)(THIS_ RECT rcDestination) PURE;
    STDMETHOD(get_OverlayPosition)(THIS_ RECT FAR* prcDestination) PURE;

	// Methods to set and get the different settings
    STDMETHOD(put_TopBorder)(THIS_ ULONG ulYOffset) PURE;
    STDMETHOD(get_TopBorder)(THIS_ ULONG FAR* pulYOffset) PURE;
    STDMETHOD(put_LeftBorder)(THIS_ ULONG ulXOffset) PURE;
    STDMETHOD(get_LeftBorder)(THIS_ ULONG FAR* pulXOffset) PURE;
    STDMETHOD(put_VgaCorrection)(THIS_ ULONG ulVgaCorrection) PURE;
    STDMETHOD(get_VgaCorrection)(THIS_ ULONG FAR* pulVgaCorrection) PURE;

    STDMETHOD(put_JitterAdjustment)(THIS_ ULONG ulJitterAdjustment) PURE;
    STDMETHOD(get_JitterAdjustment)(THIS_ ULONG FAR* pulJitterAdjustment) PURE;
    STDMETHOD(put_VgaFineAdjustment)(THIS_ ULONG ulVgaFineAdjustment) PURE;
    STDMETHOD(get_VgaFineAdjustment)(THIS_ ULONG FAR* pulVgaFineAdjustment) PURE;

	// Let WDM know the current resolution
	STDMETHOD(UpdateResolution)() PURE;

	// Methods to auto-calibrate
    STDMETHOD(AutoCalibrate)(THIS_ IREALmagicProfile *pProfile) PURE;

    STDMETHOD(StartAutoCalibration)() PURE;
    STDMETHOD(StopAutoCalibration)() PURE;
    STDMETHOD(CalibrateTopBorder)() PURE;
    STDMETHOD(CalibrateLeftBorder)() PURE;
    STDMETHOD(CalibrateCorrection)() PURE;
    STDMETHOD(CalibrateColorLimits)(THIS_ PCOLOR_LIMITS pColorLimits) PURE;

    STDMETHOD(put_CalibrationParameters)(THIS_ CALIBRATION_PARAMETERS FAR* pCalParams) PURE;
    STDMETHOD(get_CalibrationParameters)(THIS_ CALIBRATION_PARAMETERS FAR* pCalParams) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Flags for the different TV out formats
#ifndef SET_VGA
#define SET_VGA				0x0000	// Unused. Need to mask with ~SET_TV
#define SET_TV				0x0001	// Forces output to TV (instead of VGA)
#define SET_HDTV			0x0020

#define SET_NTSC			0x0000	// Unused. Need to mask with ~(SET_PAL | SET_PAL60)
#define SET_PAL 			0x0002	// Set TV output to PAL (720*576*25fps)
#define SET_PAL60			0x0008	// Set TV output to PAL60 (720*480*30fps)
#define SET_PALM			0x000A	// Not supported yet
#define SET480P				0x0100	// Ouput on Component RGB special connector

#define SET_ONETOONE		0x0000	// Unused. Need to mask with ~SET_SCALE
#define SET_SCALE			0x0004

#define SET_TV_AS_SOURCE	0x0010	// Override SET_NTSC and SET_PALxxx flags

#define COMPOSITE			0x0000	// Ouput on Composite/SVideo connector
#define COMPONENT_YUV		0x0080	// Ouput on Component YUV special connector
#define COMPONENT_RGB		0x00C0	// Ouput on Component RGB special connector
#define COMPONENT_RGB_SCART	0x00C0	// Ouput on RGB Scart Connector

#endif

// Enumeration of the different video format
typedef enum
{
	TVSRC_UNDEFINED,		// No file opened or not PAL/NTSC
	TVSRC_NTSC,				// Video is 352*240 or a multiple, 29.97 frames per sec
	TVSRC_PAL				// Video is 352*288 or a multiple, 25 frames per sec
} TV_SOURCE_FORMATS, *PTV_SOURCE_FORMATS;

/////////////////////////////////////////////////////////////////////////////
// interface ITvVgaOutput used to set output to VGA or TV in different 
// TV standards (PAL, NTSC....). DirectShow maintains its own TV output
// settings which are different then the one from MCI or DVD station. The
// only common settings between the 3 is TV standard and TV output (Connector
// type).

interface ITvVgaOutput : public IUnknown
{
public:
    STDMETHOD(GetTvOutput)(THIS_ PDWORD pdwOutFlags) PURE;
	STDMETHOD(SetTvOutput)(THIS_ DWORD dwOutFlags) PURE;

    STDMETHOD(GetTvSource)(THIS_ PTV_SOURCE_FORMATS pTvSrcFormat) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// interface ITvVgaOutput2 used to set output to VGA or TV in different 
// TV standards (PAL, NTSC....) or in HDTV mode. The first 3 methods are
// 100% compatible with the ITvVgaOutput interface. 
// The HDTV added functionality is only used to set Video through the VGA 
// Sub-D connector and is different than the component RGB coming out of the 
// TV connector. The dwRegistryHDTVMode parameter used to set the HDTV mode 
// is using the HDTV mode list found in:
//    HKEY_LOCAL_MACHINE\Software\Sigma Designs\REALmagic\xx\Progressive
// (Mode0 through Mode7).
// Be extremly cautious when modifying this list since wrong values may
// hang the system and eventually cause damage to external HDTV devices.
// To exit HDTV mode, simply use the SetTvOutput.

interface ITvVgaOutput2 : public IUnknown
{
public:
    STDMETHOD(GetTvOutput)(THIS_ PDWORD pdwOutFlags) PURE;
	STDMETHOD(SetTvOutput)(THIS_ DWORD dwOutFlags) PURE;
	
    STDMETHOD(GetTvSource)(THIS_ PTV_SOURCE_FORMATS pTvSrcFormat) PURE;

	STDMETHOD(GetHDTVMode)(THIS_ PDWORD pdwRegistryHDTVMode) PURE;
	STDMETHOD(SetHDTVMode)(THIS_ DWORD dwRegistryHDTVMode) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// interface ITvVgaOutput3 used to set output to VGA or TV in different 
// TV standards (PAL, NTSC....) or in HDTV mode. The first 3 methods are
// 100% compatible with the ITvVgaOutput interface and the last 2 with 
// ITvVgaOutput2 interface.
// We added support for Preferred TV format when not provided on the video
// input pin using the MPEG2VIDEOINFO structure (FORMAT_MPEG2Video format).
// Note that the MPEG2VIDEOINFO structure will superseed this default 
// settings.

// Enumeration of the different video format
typedef enum
{
	TVMODE_NORMAL_OR_WIDE = 0,		// 4:3 -> 4:3 and 16:9 -> 16:9
	TVMODE_PAN_SCAN,				// 4:3 -> 4:3 and 16:9 -> 4:3 pan & scan
	TVMODE_LETTER_BOX				// 4:3 -> 4:3 and 16:9 -> 4:3 letter box
} TV_PREFERRED_MODES, *PTV_PREFERRED_MODES;

interface ITvVgaOutput3 : public IUnknown
{
public:
    STDMETHOD(GetTvOutput)(THIS_ PDWORD pdwOutFlags) PURE;
	STDMETHOD(SetTvOutput)(THIS_ DWORD dwOutFlags) PURE;
	
    STDMETHOD(GetTvSource)(THIS_ PTV_SOURCE_FORMATS pTvSrcFormat) PURE;

	STDMETHOD(GetTvPreferredMode)(THIS_ PTV_PREFERRED_MODES pTvPrefMode) PURE;
	STDMETHOD(SetTvPreferredMode)(THIS_ TV_PREFERRED_MODES TvPrefMode) PURE;
	
	STDMETHOD(GetHDTVMode)(THIS_ PDWORD pdwRegistryHDTVMode) PURE;
	STDMETHOD(SetHDTVMode)(THIS_ DWORD dwRegistryHDTVMode) PURE;

	STDMETHOD(GetHDTVSubDMode)(THIS_ PDWORD pdwRegistryHDTVMode) PURE;
	STDMETHOD(SetHDTVSubDMode)(THIS_ DWORD dwRegistryHDTVMode) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// interface IMPEGTimeInfo used to retrieve information about the PTSs and 
// STC.

interface IMPEGTimeInfo : public IUnknown
{
public:
    STDMETHOD(GetSTC)(THIS_ MPEG_SYSTEM_TIME *pmstSTC) PURE;

	STDMETHOD(GetNextPTSToDecode)(THIS_ MPEG_STREAM_TYPE StreamType, 
		THIS_ MPEG_SYSTEM_TIME *pmstSTC) PURE;
	STDMETHOD(GetDisplayedPTS)(THIS_ MPEG_STREAM_TYPE StreamType,
		THIS_ MPEG_SYSTEM_TIME *pmsPTS) PURE;
    STDMETHOD(GetLastPTSReceived)(THIS_ MPEG_STREAM_TYPE StreamType,
		THIS_ MPEG_SYSTEM_TIME *pmsPTS) PURE;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif
