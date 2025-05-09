// ubuntu is sooooooooooooooooooooooooooooooo ez
#pragma once

#if !defined(RENDERDOC_NO_STDINT)
#include <stdint.h>
#endif

#if defined(WIN32)
	#define RENDERDOC_CC __cdecl
#elif defined(__linux__)
	#define RENDERDOC_CC
#else
	#error "Unknown platform"
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
//////////////////////////////////////////////////////////////////////////////////////////////////
// RenderDoc capture options
// 

typedef enum
{
	// Allow the application to enable vsync
	//
	// Default - enabled
	//
	// 1 - The application can enable or disable vsync at will
	// 0 - vsync is force disabled
	eRENDERDOC_Option_AllowVSync = 0,
	
	// Allow the application to enable fullscreen
	//
	// Default - enabled
	//
	// 1 - The application can enable or disable fullscreen at will
	// 0 - fullscreen is force disabled
	eRENDERDOC_Option_AllowFullscreen = 1,

	// Record API debugging events and messages 
	//
	// Default - disabled
	//
	// 1 - Enable built-in API debugging features and records the results into
	//     the capture logfile, which is matched up with events on replay
	// 0 - no API debugging is forcibly enabled
	eRENDERDOC_Option_DebugDeviceMode = 2,
	
	// Capture CPU callstacks for API events 
	//
	// Default - disabled
	//
	// 1 - Enables capturing of callstacks
	// 0 - no callstacks are captured
	eRENDERDOC_Option_CaptureCallstacks = 3,
	
	// When capturing CPU callstacks, only capture them from drawcalls.
	// This option does nothing without the above option being enabled
	//
	// Default - disabled
	//
	// 1 - Only captures callstacks for drawcall type API events.
	//     Ignored if CaptureCallstacks is disabled
	// 0 - Callstacks, if enabled, are captured for every event.
	eRENDERDOC_Option_CaptureCallstacksOnlyDraws = 4,

	// Specify a delay in seconds to wait for a debugger to attach, after
	// creating or injecting into a process, before continuing to allow it to run.
	//
	// 0 indicates no delay, and the process will run immediately after injection
	//
	// Default - 0 seconds
	//
	eRENDERDOC_Option_DelayForDebugger = 5,
	
	// Verify any writes to mapped buffers, by checking the memory after the
	// bounds of the returned pointer to detect any modification.
	//
	// Default - disabled
	//
	// 1 - Verify any writes to mapped buffers
	// 0 - No verification is performed, and overwriting bounds may cause
	//     crashes or corruption in RenderDoc
	eRENDERDOC_Option_VerifyMapWrites = 6,
	
	// Hooks any system API calls that create child processes, and injects
	// RenderDoc into them recursively with the same options.
	//
	// Default - disabled
	//
	// 1 - Hooks into spawned child processes
	// 0 - Child processes are not hooked by RenderDoc
	eRENDERDOC_Option_HookIntoChildren = 7,

	// By default RenderDoc only includes resources in the final logfile necessary
	// for that frame, this allows you to override that behaviour.
	//
	// Default - disabled
	//
	// 1 - all live resources at the time of capture are included in the log
	//     and available for inspection
	// 0 - only the resources referenced by the captured frame are included
	eRENDERDOC_Option_RefAllResources = 8,

	// By default RenderDoc skips saving initial states for resources where the
	// previous contents don't appear to be used, assuming that writes before
	// reads indicate previous contents aren't used.
	//
	// Default - disabled
	//
	// 1 - initial contents at the start of each captured frame are saved, even if
	//     they are later overwritten or cleared before being used.
	// 0 - unless a read is detected, initial contents will not be saved and will
	//     appear as black or empty data.
	eRENDERDOC_Option_SaveAllInitials = 9,

	// In APIs that allow for the recording of command lists to be replayed later,
	// RenderDoc may choose to not capture command lists before a frame capture is
	// triggered, to reduce overheads. This means any command lists recorded once
	// and replayed many times will not be available and may cause a failure to
	// capture.
	//
	// Note this is only true for APIs where multithreading is difficult or
	// discouraged. Newer APIs like Vulkan and D3D12 will ignore this option
	// and always capture all command lists since the API is heavily oriented
	// around it and the overheads have been reduced by API design.
	//
	// 1 - All command lists are captured from the start of the application
	// 0 - Command lists are only captured if their recording begins during
	//     the period when a frame capture is in progress.
	eRENDERDOC_Option_CaptureAllCmdLists = 10,

	// Mute API debugging output when the debug device mode option is enabled 
	//
	// Default - enabled
	//
	// 1 - Mute any API debug messages from being displayed or passed through
	// 0 - API debugging is displayed as normal
	eRENDERDOC_Option_DebugOutputMute = 11,

} RENDERDOC_CaptureOption;

// Sets an option that controls how RenderDoc behaves on capture.
//
// Returns 1 if the option and value are valid
// Returns 0 if either is invalid and the option is unchanged
typedef int (RENDERDOC_CC *pRENDERDOC_SetCaptureOptionU32)(RENDERDOC_CaptureOption opt, uint32_t val);
typedef int (RENDERDOC_CC *pRENDERDOC_SetCaptureOptionF32)(RENDERDOC_CaptureOption opt, float val);

// Gets the current value of an option as a uint32_t
//
// If the option is invalid, 0xffffffff is returned
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_GetCaptureOptionU32)(RENDERDOC_CaptureOption opt);

// Gets the current value of an option as a float
//
// If the option is invalid, -FLT_MAX is returned
typedef float (RENDERDOC_CC *pRENDERDOC_GetCaptureOptionF32)(RENDERDOC_CaptureOption opt);

typedef enum
{
	// '0' - '9' matches ASCII values
	eRENDERDOC_Key_0 = 0x30,
	eRENDERDOC_Key_1 = 0x31,
	eRENDERDOC_Key_2 = 0x32,
	eRENDERDOC_Key_3 = 0x33,
	eRENDERDOC_Key_4 = 0x34,
	eRENDERDOC_Key_5 = 0x35,
	eRENDERDOC_Key_6 = 0x36,
	eRENDERDOC_Key_7 = 0x37,
	eRENDERDOC_Key_8 = 0x38,
	eRENDERDOC_Key_9 = 0x39,

	// 'A' - 'Z' matches ASCII values
	eRENDERDOC_Key_A = 0x41,
	eRENDERDOC_Key_B = 0x42,
	eRENDERDOC_Key_C = 0x43,
	eRENDERDOC_Key_D = 0x44,
	eRENDERDOC_Key_E = 0x45,
	eRENDERDOC_Key_F = 0x46,
	eRENDERDOC_Key_G = 0x47,
	eRENDERDOC_Key_H = 0x48,
	eRENDERDOC_Key_I = 0x49,
	eRENDERDOC_Key_J = 0x4A,
	eRENDERDOC_Key_K = 0x4B,
	eRENDERDOC_Key_L = 0x4C,
	eRENDERDOC_Key_M = 0x4D,
	eRENDERDOC_Key_N = 0x4E,
	eRENDERDOC_Key_O = 0x4F,
	eRENDERDOC_Key_P = 0x50,
	eRENDERDOC_Key_Q = 0x51,
	eRENDERDOC_Key_R = 0x52,
	eRENDERDOC_Key_S = 0x53,
	eRENDERDOC_Key_T = 0x54,
	eRENDERDOC_Key_U = 0x55,
	eRENDERDOC_Key_V = 0x56,
	eRENDERDOC_Key_W = 0x57,
	eRENDERDOC_Key_X = 0x58,
	eRENDERDOC_Key_Y = 0x59,
	eRENDERDOC_Key_Z = 0x5A,

	// leave the rest of the ASCII range free
	// in case we want to use it later
	eRENDERDOC_Key_NonPrintable = 0x100,

	eRENDERDOC_Key_Divide,
	eRENDERDOC_Key_Multiply,
	eRENDERDOC_Key_Subtract,
	eRENDERDOC_Key_Plus,

	eRENDERDOC_Key_F1,
	eRENDERDOC_Key_F2,
	eRENDERDOC_Key_F3,
	eRENDERDOC_Key_F4,
	eRENDERDOC_Key_F5,
	eRENDERDOC_Key_F6,
	eRENDERDOC_Key_F7,
	eRENDERDOC_Key_F8,
	eRENDERDOC_Key_F9,
	eRENDERDOC_Key_F10,
	eRENDERDOC_Key_F11,
	eRENDERDOC_Key_F12,

	eRENDERDOC_Key_Home,
	eRENDERDOC_Key_End,
	eRENDERDOC_Key_Insert,
	eRENDERDOC_Key_Delete,
	eRENDERDOC_Key_PageUp,
	eRENDERDOC_Key_PageDn,

	eRENDERDOC_Key_Backspace,
	eRENDERDOC_Key_Tab,
	eRENDERDOC_Key_PrtScrn,
	eRENDERDOC_Key_Pause,

	eRENDERDOC_Key_Max,
} RENDERDOC_InputButton;

// Sets which key or keys can be used to toggle focus between multiple windows
//
// If keys is NULL or num is 0, toggle keys will be disabled
typedef void (RENDERDOC_CC *pRENDERDOC_SetFocusToggleKeys)(RENDERDOC_InputButton *keys, int num);

// Sets which key or keys can be used to capture the next frame
//
// If keys is NULL or num is 0, captures keys will be disabled
typedef void (RENDERDOC_CC *pRENDERDOC_SetCaptureKeys)(RENDERDOC_InputButton *keys, int num);

typedef enum
{
	// This single bit controls whether the overlay is enabled or disabled globally
	eRENDERDOC_Overlay_Enabled = 0x1,

	// Show the average framerate over several seconds as well as min/max
	eRENDERDOC_Overlay_FrameRate = 0x2,

	// Show the current frame number
	eRENDERDOC_Overlay_FrameNumber = 0x4,

	// Show a list of recent captures, and how many captures have been made
	eRENDERDOC_Overlay_CaptureList = 0x8,

	// Default values for the overlay mask
	eRENDERDOC_Overlay_Default =
	       (eRENDERDOC_Overlay_Enabled|
	        eRENDERDOC_Overlay_FrameRate|
	        eRENDERDOC_Overlay_FrameNumber|
	        eRENDERDOC_Overlay_CaptureList),

	// Enable all bits
	eRENDERDOC_Overlay_All = ~0U,

	// Disable all bits
	eRENDERDOC_Overlay_None = 0,
} RENDERDOC_OverlayBits;

// returns the overlay bits that have been set
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_GetOverlayBits)();
// sets the overlay bits with an and & or mask
typedef void (RENDERDOC_CC *pRENDERDOC_MaskOverlayBits)(uint32_t And, uint32_t Or);

// this function will attempt to shut down RenderDoc.
//
// Note: that this will only work correctly if done immediately after
// the dll is loaded, before any API work happens. RenderDoc will remove its
// injected hooks and shut down. Behaviour is undefined if this is called
// after any API functions have been called.
typedef void (RENDERDOC_CC *pRENDERDOC_Shutdown)();

// This function will unload RenderDoc's crash handler.
//
// If you use your own crash handler and don't want RenderDoc's handler to
// intercede, you can call this function to unload it and any unhandled
// exceptions will pass to the next handler.
typedef void (RENDERDOC_CC *pRENDERDOC_UnloadCrashHandler)();

// Sets the logfile path template
//
// logfile is a UTF-8 string that gives a template for how captures will be named
// and where they will be saved.
//
// Any extension is stripped off the path, and captures are saved in the directory
// specified, and named with the filename and the frame number appended. If the
// directory does not exist it will be created, including any parent directories.
//
// If pathtemplate is NULL, the template will remain unchanged
//
// Example:
//
// SetLogFilePathTemplate("my_captures/example");
//
// Capture #1 -> my_captures/example_frame123.rdc
// Capture #2 -> my_captures/example_frame456.rdc
typedef void (RENDERDOC_CC *pRENDERDOC_SetLogFilePathTemplate)(const char *pathtemplate);

// returns the current logfile template, see SetLogFileTemplate above, as a UTF-8 string
typedef const char* (RENDERDOC_CC *pRENDERDOC_GetLogFilePathTemplate)();

// returns the number of captures that have been made
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_GetNumCaptures)();

// This function returns the details of a capture, by index. New captures are added
// to the end of the list.
//
// logfile will be filled with the absolute path to the capture file, as a UTF-8 string
// pathlength will be written with the length in bytes of the logfile string
// timestamp will be written with the time of the capture, in seconds since the Unix epoch
//
// Any of the parameters can be NULL and they'll be skipped.
//
// The function will return 1 if the capture index is valid, or 0 if the index is invalid
// If the index is invalid, the values will be unchanged
//
// Note: when captures are deleted in the UI they will remain in this list, so the
// logfile path may not exist anymore.
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_GetCapture)(uint32_t idx, char *logfile, uint32_t *pathlength, uint64_t *timestamp);

// capture the next frame on whichever window and API is currently considered active
typedef void (RENDERDOC_CC *pRENDERDOC_TriggerCapture)();

// returns 1 if the RenderDoc UI is connected to this application, 0 otherwise
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_IsRemoteAccessConnected)();

// This function will launch the Replay UI associated with the RenderDoc library injected
// into the running application.
//
// if connectRemoteAccess is 1, the Replay UI will be launched with a command line parameter
// to connect to this application
// cmdline is the rest of the command line, as a UTF-8 string. E.g. a captures to open
// if cmdline is NULL, the command line will be empty.
//
// returns the PID of the replay UI if successful, 0 if not successful.
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_LaunchReplayUI)(uint32_t connectRemoteAccess, const char *cmdline);

// RenderDoc can return a higher version than requested if it's backwards compatible,
// this function returns the actual version returned. If a parameter is NULL, it will be
// ignored and the others will be filled out.
typedef void (RENDERDOC_CC *pRENDERDOC_GetAPIVersion)(int *major, int *minor, int *patch);

//////////////////////////////////////////////////////////////////////////
// Capturing functions
//

// A device pointer is a pointer to the API's root handle.
//
// This would be an ID3D11Device, HGLRC/GLXContext, ID3D12Device, etc
typedef void* RENDERDOC_DevicePointer;

// A window handle is the OS's native window handle
//
// This would be an HWND, GLXDrawable, etc
typedef void* RENDERDOC_WindowHandle;

// This sets the RenderDoc in-app overlay in the API/window pair as 'active' and it will
// respond to keypresses. Neither parameter can be NULL
typedef void (RENDERDOC_CC *pRENDERDOC_SetActiveWindow)(RENDERDOC_DevicePointer device, RENDERDOC_WindowHandle wndHandle);

// When choosing either a device pointer or a window handle to capture, you can pass NULL.
// Passing NULL specifies a 'wildcard' match against anything. This allows you to specify
// any API rendering to a specific window, or a specific API instance rendering to any window,
// or in the simplest case of one window and one API, you can just pass NULL for both.
//
// In either case, if there are two or more possible matching (device,window) pairs it
// is undefined which one will be captured.
//
// Note: for headless rendering you can pass NULL for the window handle and either specify
// a device pointer or leave it NULL as above.

// Immediately starts capturing API calls on the specified device pointer and window handle.
//
// If there is no matching thing to capture (e.g. no supported API has been initialised),
// this will do nothing.
//
// The results are undefined (including crashes) if two captures are started overlapping,
// even on separate devices and/oror windows.
typedef void (RENDERDOC_CC *pRENDERDOC_StartFrameCapture)(RENDERDOC_DevicePointer device, RENDERDOC_WindowHandle wndHandle);

// Returns whether or not a frame capture is currently ongoing anywhere.
//
// This will return 1 if a capture is ongoing, and 0 if there is no capture running
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_IsFrameCapturing)();

// Ends capturing immediately.
//
// This will return 1 if the capture succeeded, and 0 if there was an error capturing.
typedef uint32_t (RENDERDOC_CC *pRENDERDOC_EndFrameCapture)(RENDERDOC_DevicePointer device, RENDERDOC_WindowHandle wndHandle);

//////////////////////////////////////////////////////////////////////////////////////////////////
// RenderDoc API versions
// 

// RenderDoc uses semantic versioning (http://semver.org/).
// 
// MAJOR version is incremented when incompatible API changes happen.
// MINOR version is incremented when functionality is added in a backwards-compatible manner.
// PATCH version is incremented when backwards-compatible bug fixes happen.
//
// Note that this means the API returned can be higher than the one you might have requested.
// e.g. if you are running against a newer RenderDoc that supports 1.0.1, it will be returned
// instead of 1.0.0. You can check this with the GetAPIVersion entry point
typedef enum
{
	eRENDERDOC_API_Version_1_0_0 = 10000, // RENDERDOC_API_1_0_0 = 1 00 00
	eRENDERDOC_API_Version_1_0_1 = 10001, // RENDERDOC_API_1_0_1 = 1 00 01
} RENDERDOC_Version;

// API version changelog:
//
// 1.0.0 - initial release
// 1.0.1 - Bugfix: IsFrameCapturing() was returning false for captures that were triggered
//         by keypress or TriggerCapture, instead of Start/EndFrameCapture.

// eRENDERDOC_API_Version_1_0_1
typedef struct
{
	pRENDERDOC_GetAPIVersion              GetAPIVersion;

	pRENDERDOC_SetCaptureOptionU32        SetCaptureOptionU32;
	pRENDERDOC_SetCaptureOptionF32        SetCaptureOptionF32;

	pRENDERDOC_GetCaptureOptionU32        GetCaptureOptionU32;
	pRENDERDOC_GetCaptureOptionF32        GetCaptureOptionF32;

	pRENDERDOC_SetFocusToggleKeys         SetFocusToggleKeys;
	pRENDERDOC_SetCaptureKeys             SetCaptureKeys;

	pRENDERDOC_GetOverlayBits             GetOverlayBits;
	pRENDERDOC_MaskOverlayBits            MaskOverlayBits;

	pRENDERDOC_Shutdown                   Shutdown;
	pRENDERDOC_UnloadCrashHandler         UnloadCrashHandler;

	pRENDERDOC_SetLogFilePathTemplate     SetLogFilePathTemplate;
	pRENDERDOC_GetLogFilePathTemplate     GetLogFilePathTemplate;

	pRENDERDOC_GetNumCaptures             GetNumCaptures;
	pRENDERDOC_GetCapture                 GetCapture;

	pRENDERDOC_TriggerCapture             TriggerCapture;

	pRENDERDOC_IsRemoteAccessConnected    IsRemoteAccessConnected;
	pRENDERDOC_LaunchReplayUI             LaunchReplayUI;

	pRENDERDOC_SetActiveWindow            SetActiveWindow;

	pRENDERDOC_StartFrameCapture          StartFrameCapture;
	pRENDERDOC_IsFrameCapturing           IsFrameCapturing;
	pRENDERDOC_EndFrameCapture            EndFrameCapture;
} RENDERDOC_API_1_0_1;

typedef RENDERDOC_API_1_0_1 RENDERDOC_API_1_0_0;

//////////////////////////////////////////////////////////////////////////////////////////////////
// RenderDoc API entry point
// 
// This entry point can be obtained via GetProcAddress/dlsym if RenderDoc is available.
// 
// The name is the same as the typedef - "RENDERDOC_GetAPI"
//
// This function is not thread safe, and should not be called on multiple threads at once.
// Ideally, call this once as early as possible in your application's startup, before doing
// any API work, since some configuration functionality etc has to be done also before
// initialising any APIs.
//
// Parameters:
//   version is a single value from the RENDERDOC_Version above.
//
//   outAPIPointers will be filled out with a pointer to the corresponding struct of function
//   pointers.
//
// Returns:
//   1 - if the outAPIPointers has been filled with a pointer to the API struct requested
//   0 - if the requested version is not supported or the arguments are invalid.
//
typedef int (RENDERDOC_CC *pRENDERDOC_GetAPI)(RENDERDOC_Version version, void **outAPIPointers);

#ifdef __cplusplus
}  // extern "C"
#endif
