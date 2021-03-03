
// r3dwin.h -- engine core header

// debug flag //

// omit from release version
#define DEBUG_MODE

// defines //

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800

// system includes //

#include<objbase.h>
#include<windows.h>
#include<windowsx.h>
#include<winuser.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include<direct.h>

// directx includes

#include<dxerr8.h>
#include<d3d8.h>
#include<d3d8types.h>
#include<d3dx8math.h>
#include<dmusici.h>
#include<dinput.h>

// structures //

typedef struct tagDISPLAYMODE
{
	// resolution
	DWORD dwX,dwY;
	// format
	D3DFORMAT fmt;
	// windowed flag
	BOOL bWindowed;
	// lockable flag
	BOOL bLockable;
	// back buffer count
	DWORD dwBackBufferCount;
} DISPLAYMODE,*LPDISPLAYMODE;

typedef struct tagDATAFILEHEADER
{
	// size of data block
	DWORD dwSize;
	// number of references
	DWORD dwNumRef;
	// position of offset table
	DWORD dwOffsetPos;
	// position of size table
	DWORD dwSizePos;
	// position of data table
	DWORD dwDataPos;
	// encryption codes
	BYTE cEncCode[4];
} DATAFILEHEADER,*LPDATAFILEHEADER;

typedef struct tagDATAFILEREF
{
	// pointer to data
	LPVOID lpData;
	// size of data
	DWORD dwSize;
} DATAFILEREF,*LPDATAFILEREF;

// constants //

#define EPSILON 0.000001F

#define PI 3.141592653F

#define BLOCK_ALLOC_INC 32

#define RANDOM_NUMBERS 521 // a prime number, to prevent patterns

#define MAX_FRAME_SKIP 15

#define KEY_BUFFER_SIZE 32

// standard dialog pushbuttons
#define BTN_OK		0xFFFF
#define BTN_CANCEL	0xFFFE

#define FLASH_TIMER_ID		 1
#define FLASH_TIMER_DURATION 1000

#define MSEC_PER_FRAME 17
#define MSEC_PER_SEC   1000

// engine includes //

#include"r3ddxg.h" // DirectX Graphics wrapper
#include"r3ddxa.h" // DirectX Audio wrapper
#include"r3ddxi.h" // DirectInput wrapper
#include"r3dapp.h" // primiary application

// globals //

// handle to application window
extern HWND g_hWnd;
// handle to application instance
extern HINSTANCE g_hInst;
// handle to duplication mutex
extern HANDLE g_hMutex;
// paused flag
extern BOOL g_bIdle;
// closing flag
extern BOOL g_bClosing;
// display modes
extern DISPLAYMODE g_dmApp;
extern DISPLAYMODE g_dmSys;
// window size
extern int g_nXSize;
extern int g_nYSize;
// window position
extern int g_nXPos;
extern int g_nYPos;
// frame skip
extern DWORD g_dwFrameSkip;
// frames per second
extern DWORD g_dwFPS;
// updates per second
extern DWORD g_dwUPS;
// application timer
extern DWORD g_dwAppTimer;
// timer counters
extern DWORD g_dwNumTimer;
extern DWORD g_dwMaxTimer;
// timer data
extern LPDWORD g_lpdwTimerLast;
extern LPDWORD g_lpdwTimerInc;
// random number tables
extern float* g_lpfRand;
extern int* g_lpnRand;
// resource counters
extern DWORD g_dwNumCOMRes;
extern DWORD g_dwNumWinRes;
extern DWORD g_dwNumMemRes;
extern DWORD g_dwNumClassRes;
// message strings
extern char g_lpMsg[];
extern WCHAR g_lpWMsg[];
// registry keys
extern HKEY g_hSoftKey;
extern HKEY g_hAppKey;
// script file handles
#ifdef DEBUG_MODE
extern HANDLE g_hScriptFile;
#endif
extern HANDLE g_hCScriptFile;
// script file data
extern LPBYTE g_lpCScriptData;
extern DWORD g_dwCScriptDataPtr;
// system mouse info
extern BOOL g_bWinMouse;
extern int g_nMouseX;
extern int g_nMouseY;
extern int g_nMousePX;
extern int g_nMousePY;
extern int g_nMouseDX;
extern int g_nMouseDY;
extern BOOL g_bMouseBtnL;
extern BOOL g_bMouseBtnR;
extern BOOL g_bMousePBtnL;
extern BOOL g_bMousePBtnR;
extern BOOL g_bMouseClkL;
extern BOOL g_bMouseClkR;
#ifdef DEBUG_MODE
extern BOOL g_bMouseBtnS;
extern BOOL g_bMouseBtnC;
extern BOOL g_bMousePBtnS;
extern BOOL g_bMousePBtnC;
extern BOOL g_bMouseClkS;
extern BOOL g_bMouseClkC;
#endif
// dialog box counters
extern DWORD g_dwNumDlg;
extern DWORD g_dwMaxDlg;
// dialog box data
extern LPBYTE* g_lplpDlgData;
extern LPDWORD g_lpdwDlgDataSize;
extern LPDWORD g_lpdwDlgDataPtr;
extern HWND g_hCurrDlg;
// accelerator keys
extern HACCEL g_hAccel;
extern ACCEL g_acAltEnter;
// pause bitmap data
extern BITMAPINFOHEADER g_bih;
extern LPBYTE g_lpPauseBmpData;
// data file counters
extern DWORD g_dwNumDataFileRef;
extern DWORD g_dwMaxDataFileRef;
// data file data
extern HANDLE g_hDataFile;
extern LPBYTE g_lpDataFile;
#ifdef DEBUG_MODE
extern LPSTR* g_lplpDataFileRefName;
extern LPBOOL g_lpbDataFileRefUsed;
#endif
extern DWORD g_dwDataFileOffsetPos;
extern DWORD g_dwDataFileSizePos;
extern DWORD g_dwDataFileDataPos;
// COM flag
extern BOOL g_bCOMReady;

// prototypes //

// window handling functions

extern LRESULT CALLBACK WindowProc(HWND hWnd,
								   UINT Msg,
								   WPARAM wParam,
								   LPARAM lParam);

extern void PrimaryMessageLoop(void);

extern void EnterIdleMode(void);

extern void ExitIdleMode(void);

extern void SendCloseMessage(void);

extern BOOL LockApplication(HINSTANCE hInstance);

extern void UnlockApplication(void);

extern BOOL AddApplicationWindowClass(void);

extern void RemoveApplicationWindowClass(void);

extern BOOL CreateApplicationWindow(void);

extern void DestroyApplicationWindow(void);

extern DISPLAYMODE GetSystemDisplayMode(void);

extern void SetSystemDisplayMode(DWORD dwX=0,
								 DWORD dwY=0);

extern void CalculateWindowMetrics(void);

extern BOOL SetDataPath(LPCSTR lpDir);

extern BOOL ProcessPendingMessages(void);

extern void ProcessMouseTracking(void);

extern void BeginExclusiveMode(void);

extern void EndExclusiveMode(void);

extern void MinimizeApplicationWindow(void);

extern void UpdateWindowMetrics(void);

extern void RestoreApplicationWindow(void);

extern BOOL InitAcceleratorTable(void);

extern void ReleaseAcceleratorTable(void);

extern void ShowWindowsCursor(void);

extern void HideWindowsCursor(void);

extern BOOL CapturePauseBitmap(D3DCOLOR clrFilter);

extern void ReleasePauseBitmap(void);

// object handling functions

extern BOOL InitializeAllModules(void);

extern void ReleaseAllModules(void);

extern BOOL InitializeCOM(void);

extern void ReleaseCOM(void);

// error handling functions

extern void DisplayErrorMessage(LPSTR lpError,
								LPSTR lpSolution=NULL,
								BOOL bExtInfo=TRUE);

extern void DisplayFatalErrorMessage(LPSTR lpError,
									 LPSTR lpSolution=NULL,
									 BOOL bExtInfo=TRUE);

extern void DisplayDXErrorMessage(LPSTR lpError,
								  LPSTR lpSolution=NULL,
								  HRESULT hError=NULL);

extern void DisplayFatalDXErrorMessage(LPSTR lpError,
									   LPSTR lpSolution=NULL,
									   HRESULT hError=NULL);

extern void DisplayInfoMessage(LPSTR lpError);

// misc functions

extern void SwapPtrs(LPVOID* lplpPtr1,
					 LPVOID* lplpPtr2);

extern BOOL AllocMem(LPVOID* lplpPtr,
					 DWORD dwSize);

extern BOOL ExpandMem(LPVOID* lplpPtr,
	 			      DWORD dwSize,
 				      DWORD dwExpAmt);

extern BOOL CompressMem(LPVOID* lplpPtr,
	 			        DWORD dwSize);

extern void FreeMem(LPVOID* lplpPtr);

extern BOOL InitRandomNumbers(void);

extern float GetRandomFloat(BOOL bPrevious=FALSE);

extern int GetRandomInt(BOOL bPrevious=FALSE);

extern void ReleaseRandomNumbers(void);

extern BOOL VerifyClassObject(LPVOID lpPtr);

#ifdef DEBUG_MODE
extern void DisplayResourceUsage(void);
#endif

extern void ConfirmResourcesReleased(void);

extern BOOL DivideString(LPSTR lpStr);

extern LPSTR GetStringDivision(LPSTR lpStr,
							   DWORD dwIndex);

extern int DisplayMessageBox(HWND hWnd,
							 LPSTR lpText,
							 LPSTR lpCaption,
							 UINT uType);

extern BOOL BeginSystemOutput(void);

extern void EndSystemOutput(BOOL bFlag);

extern DWORD GetFileLength(LPSTR lpFile);

extern void PurgeSystemObjects(void);

extern void ForceImmediateExit(void);

extern void CapitalizeString(LPSTR lpStr);

extern DWORD BinaryToDWORD(LPSTR lpStr);

// registry functions

extern BOOL OpenRegistry(void);

extern void CloseRegistry(void);

extern void SetRegistryKey(LPCSTR lpName,
						   LPSTR lpData);

extern LPSTR GetRegistryKey(LPCSTR lpName);

extern void FreeRegistryKey(LPSTR lpData);

extern void DeleteRegistryKey(LPCSTR lpName);

// system mouse functions

extern void EnableMouseTracking(void);

extern void DisableMouseTracking(void);

// timer functions

extern DWORD GetNextTimer(void);

extern DWORD AddTimer(DWORD dwFramesPerTick);

extern void UpdateTimers(void);

extern DWORD CheckTimer(DWORD dwIndex);

extern void PurgeTimers(void);

// script functions

#ifdef DEBUG_MODE
extern BOOL OpenScriptCompiler(LPCSTR lpDestFile,
							   LPCSTR lpSrcFile);

extern char ReadScriptChar(void);

extern long ReadScriptLong(void);

extern float ReadScriptFloat(void);

extern BOOL ReadScriptString(LPSTR lpStr);

extern BOOL SaveScriptLong(long lVal);

extern BOOL SaveScriptFloat(float fVal);

extern BOOL SaveScriptString(LPSTR lpStr);

extern BOOL CopyScriptLong(void);

extern BOOL CopyScriptFloat(void);

extern BOOL CopyScriptString(void);

extern void CloseScriptCompiler(void);

extern BOOL OpenScriptCreator(LPCSTR lpFile);

extern BOOL CreateScriptString(LPSTR lpStr);

extern void CloseScriptCreator(void);

extern BOOL IsValidScriptChar(char c);
#endif

extern BOOL OpenCompiledScript(LPCSTR lpFile);

extern char ReadCScriptChar(void);

extern void ReadCScriptBytes(LPVOID lpDest,
							 DWORD dwNumBytes);

extern long ReadCScriptLong(void);

extern float ReadCScriptFloat(void);

extern void ReadCScriptString(LPSTR lpStr);

extern void CloseCompiledScript(void);

// dialog box functions

extern DWORD GetNextDialog(void);

extern DWORD AddDialog(short sXSize,
					   short sYSize,
					   LPCSTR lpTitle);

extern void ExpandDialog(DWORD dwIndex);

extern void AlignDialogWORD(DWORD dwIndex);

extern void AlignDialogDWORD(DWORD dwIndex);

extern void AddDialogDWORD(DWORD dwIndex,
						   DWORD dwVal);

extern void AddDialogWORD(DWORD dwIndex,
						  WORD wVal);

extern void AddDialogBYTE(DWORD dwIndex,
						  BYTE cVal);

extern void AddDialogShort(DWORD dwIndex,
						   short sVal);

extern void AddDialogCoords(DWORD dwIndex,
						    short sX,
							short sY);

extern void AddDialogString(DWORD dwIndex,
						    LPSTR lpStr);

extern void AddDialogPtr(DWORD dwIndex,
						 LPVOID lpPtr);

extern int ExecuteDialog(DWORD dwIndex,
						 DLGPROC lpDlgProc);

extern void PurgeDialogs(void);

extern void AddDialogControl(DWORD dwIndex);

extern void AddDialogPushButton(DWORD dwIndex,
								short sXPos,
								short sYPos,
								short sXSize,
								short sYSize,
								LPCSTR lpText,
								WORD wID,
								BOOL bDefBtn=FALSE);

extern void AddDialogCheckBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
							  LPCSTR lpText,
							  WORD wID);

extern void AddDialogRadioButton(DWORD dwIndex,
								 short sXPos,
								 short sYPos,
								 short sXSize,
								 short sYSize,
								 LPCSTR lpText,
								 WORD wID);

extern void AddDialogGroupBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
							  LPCSTR lpText);

extern void AddDialogDropDownListBox(DWORD dwIndex,
									 short sXPos,
									 short sYPos,
									 short sXSize,
									 short sYSize,
									 WORD wID);

extern void AddDialogEditControl(DWORD dwIndex,
								 short sXPos,
								 short sYPos,
								 short sXSize,
								 short sYSize,
								 WORD wID);

extern void AddDialogListBox(DWORD dwIndex,
							 short sXPos,
							 short sYPos,
							 short sXSize,
							 short sYSize,
							 WORD wID);

extern void AddDialogStaticControl(DWORD dwIndex,
								   short sXPos,
								   short sYPos,
								   short sXSize,
								   short sYSize,
								   LPCSTR lpText);

extern void AddDialogGroup(DWORD dwIndex);

extern BOOL SetCurrentDialogCheckBox(WORD wID,
									 BOOL bVal);

extern BOOL GetCurrentDialogCheckBox(WORD wID);

extern BOOL SetCurrentDialogEditText(WORD wID,
									 LPSTR lpText);

extern BOOL GetCurrentDialogEditText(WORD wID,
									 LPSTR lpText,
									 int nMaxCount);

extern BOOL SetCurrentDialogEditFloat(WORD wID,
									  float fVal);

extern float GetCurrentDialogEditFloat(WORD wID);

extern BOOL SetCurrentDialogEditLong(WORD wID,
									 long lVal);

extern long GetCurrentDialogEditLong(WORD wID);

extern void SetCurrentDialogBox(HWND hDlg);

extern BOOL GetCurrentDialogDropDownSelection(DWORD dwID,
											  LPSTR lpText);

extern BOOL ResetCurrentDialogDropDownSelections(DWORD dwID);

extern BOOL AddCurrentDialogDropDownSelection(DWORD dwID,
											  LPSTR lpText);

extern BOOL RemoveCurrentDialogDropDownSelection(DWORD dwID);

extern BOOL HighlightCurrentDialogDropDownSelection(DWORD dwID,
													LPSTR lpText);

// data file functions

#ifdef DEBUG_MODE
extern BOOL OpenDataCompiler(LPCSTR lpFile);

extern DWORD AddDataFileRef(LPSTR lpFile);

extern void CloseDataCompiler(void);
#endif

extern BOOL OpenDataFile(LPCSTR lpFile);

extern DATAFILEREF FindDataFileRef(LPSTR lpFile);

extern void CloseDataFile(void);

// macros //

#define AddCOMResource   { g_dwNumCOMRes++; }
#define AddWinResource   { g_dwNumWinRes++; }
#define AddMemResource   { g_dwNumMemRes++; }
#define AddClassResource { g_dwNumClassRes++; }

#define AddCOMResources(n) { g_dwNumCOMRes+=n; }

#define RemoveCOMResource   { g_dwNumCOMRes--; }
#define RemoveWinResource   { g_dwNumWinRes--; }
#define RemoveMemResource   { g_dwNumMemRes--; }
#define RemoveClassResource { g_dwNumClassRes--; }

#define ReleaseCOMPtr(a) { if(a) { a->Release(); a=NULL; RemoveCOMResource; } }

#define AllocClassObject(a,b) { a=new b; if(VerifyClassObject((LPVOID)(a))) AddClassResource; }
#define FreeClassObject(a)    { if(a) { delete a; a=NULL; RemoveClassResource; } }

#define InitStruct(a) { ZeroMemory(&a,sizeof(a)); }

#define IsNearEqual(a,b) (fabsf(((float)(a))-((float)(b)))<EPSILON)
#define IsNearZero(a)    IsNearEqual((a),0)

#define DegToRad(a) ((float)((((float)(a))/360)*(2*PI)))
#define RadToDeg(a) ((float)((((float)(a))/(2*PI))*360))

#define Squared(a) ((a)*(a))

#define IsKeyDown(a) (GetAsyncKeyState(a)&0x8000)

#define IsMouseInRect(r) (g_nMouseX>=r.left&&g_nMouseX<=r.right&&g_nMouseY>=r.top&&g_nMouseY<=r.bottom)

#define MakeWideString(d,s) { MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,s,-1,d,_MAX_PATH); }

// messages

#define IDM_TOGGLEFULLSCREEN WM_USER+100
	
// eof //

