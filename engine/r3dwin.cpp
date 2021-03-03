
// r3dwin.cpp -- engine core

// includes //

#include"r3dwin.h"

// initialize globals //

// handle to application window
HWND g_hWnd=(HWND)INVALID_HANDLE_VALUE;
// handle to application instance
HINSTANCE g_hInst=(HINSTANCE)INVALID_HANDLE_VALUE;
// handle to duplication mutex
HANDLE g_hMutex=INVALID_HANDLE_VALUE;
// paused flag
BOOL g_bIdle=FALSE;
// closing flag
BOOL g_bClosing=FALSE;
// display modes
DISPLAYMODE g_dmApp;
DISPLAYMODE g_dmSys;
// window size
int g_nXSize=0;
int g_nYSize=0;
// window position
int g_nXPos=0;
int g_nYPos=0;
// frame skip (must be 1)
DWORD g_dwFrameSkip=1;
// frames per second
DWORD g_dwFPS=0;
// updates per second
DWORD g_dwUPS=0;
// application timer
DWORD g_dwAppTimer=0;
// timer counters
DWORD g_dwNumTimer=0;
DWORD g_dwMaxTimer=0;
// timer data
LPDWORD g_lpdwTimerLast=NULL;
LPDWORD g_lpdwTimerInc=NULL;
// random number tables
float* g_lpfRand=NULL;
int* g_lpnRand=NULL;
// resource counters
DWORD g_dwNumCOMRes=0;
DWORD g_dwNumWinRes=0;
DWORD g_dwNumMemRes=0;
DWORD g_dwNumClassRes=0;
// message strings
char g_lpMsg[_MAX_PATH];
WCHAR g_lpWMsg[_MAX_PATH];
// registry keys
HKEY g_hSoftKey=(HKEY)INVALID_HANDLE_VALUE;
HKEY g_hAppKey=(HKEY)INVALID_HANDLE_VALUE;
// script file handles
#ifdef DEBUG_MODE
HANDLE g_hScriptFile=INVALID_HANDLE_VALUE;
#endif
HANDLE g_hCScriptFile=INVALID_HANDLE_VALUE;
// script file data
LPBYTE g_lpCScriptData=NULL;
DWORD g_dwCScriptDataPtr=0;
// system mouse info
BOOL g_bWinMouse=FALSE;
int g_nMouseX=0;
int g_nMouseY=0;
int g_nMousePX=0;
int g_nMousePY=0;
int g_nMouseDX=0;
int g_nMouseDY=0;
BOOL g_bMouseBtnL=FALSE;
BOOL g_bMouseBtnR=FALSE;
BOOL g_bMousePBtnL=FALSE;
BOOL g_bMousePBtnR=FALSE;
BOOL g_bMouseClkL=FALSE;
BOOL g_bMouseClkR=FALSE;
#ifdef DEBUG_MODE
BOOL g_bMouseBtnS=FALSE;
BOOL g_bMouseBtnC=FALSE;
BOOL g_bMousePBtnS=FALSE;
BOOL g_bMousePBtnC=FALSE;
BOOL g_bMouseClkS=FALSE;
BOOL g_bMouseClkC=FALSE;
#endif
// dialog box counters
DWORD g_dwNumDlg=0;
DWORD g_dwMaxDlg=0;
// dialog box data
LPBYTE* g_lplpDlgData=NULL;
LPDWORD g_lpdwDlgDataSize=NULL;
LPDWORD g_lpdwDlgDataPtr=NULL;
HWND g_hCurrDlg=(HWND)INVALID_HANDLE_VALUE;
// accelerator keys
HACCEL g_hAccel=(HACCEL)INVALID_HANDLE_VALUE;
ACCEL g_acAltEnter;
// pause bitmap data
BITMAPINFOHEADER g_bih;
LPBYTE g_lpPauseBmpData=NULL;
// data file counters
DWORD g_dwNumDataFileRef=0;
DWORD g_dwMaxDataFileRef=0;
// data file data
HANDLE g_hDataFile=INVALID_HANDLE_VALUE;
LPBYTE g_lpDataFile=NULL;
#ifdef DEBUG_MODE
LPSTR* g_lplpDataFileRefName=NULL;
LPBOOL g_lpbDataFileRefUsed=NULL;
#endif
DWORD g_dwDataFileOffsetPos=0;
DWORD g_dwDataFileSizePos=0;
DWORD g_dwDataFileDataPos=0;
// COM flag
BOOL g_bCOMReady=FALSE;

// functions //

// WinMain()

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	// prevent duplicate instances
	if(!LockApplication(hInstance))
		return(0);

	// initialize & execute application
	if(InitializeAllModules())
		PrimaryMessageLoop();

	// release all allocated resources
	ReleaseAllModules();

	// release duplication lock
	UnlockApplication();

	// confirm removal of all resources
	ConfirmResourcesReleased();

	// return to OS
	return(0);
}

// WindowProc()

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT Msg,
							WPARAM wParam,
							LPARAM lParam)
{
	// device context
	HDC hDC;
	
	// check for messages that we are interested in
	switch(Msg)
	{
	// process activate message
	case(WM_ACTIVATE):

		// check new state
		if(LOWORD(wParam)==WA_INACTIVE)
			EnterIdleMode();
		else
			ExitIdleMode();
		
		// ok
		return(NULL);
		
	// process close message
	case(WM_CLOSE):

		// set flag
		g_bClosing=TRUE;
		
		// quit application
		PostQuitMessage(NULL);

		// ok
		return(NULL);

	// display mode changed
	case(WM_DISPLAYCHANGE):

		// update mode if idle
		if(g_bIdle)
			SetSystemDisplayMode((DWORD)LOWORD(lParam),
								 (DWORD)HIWORD(lParam));
		
		// ok
		break;

	// process enable message
	case(WM_ENABLE):

		// check flag
		if(!wParam)
			EnterIdleMode();
		else
			ExitIdleMode();
		
		// ok
		return(NULL);
		
	// user has ended session
	case(WM_ENDSESSION):

		// check flags
		if((BOOL)wParam==TRUE||
		   (BOOL)lParam==TRUE)
			ForceImmediateExit();
		
		// ok
		return(NULL);

	// focus is lost
	case(WM_ENTERMENULOOP):
	case(WM_ENTERSIZEMOVE):
	case(WM_KILLFOCUS):

		// enter idle
		EnterIdleMode();

		// ok
		return(NULL);

	// system entering standby
	case(WM_POWERBROADCAST):

		// check flag
		if((DWORD)wParam==PBT_APMSUSPEND)
		{
			// enter idle
			EnterIdleMode();
			
			// ok
			return(TRUE);
		}

		// ok
		break;
		
	// focus is restored
	case(WM_EXITMENULOOP):
	case(WM_EXITSIZEMOVE):
	case(WM_SETFOCUS):

		// exit idle
		ExitIdleMode();

		// ok
		return(NULL);

	// timer message received
	case(WM_TIMER):

		// check id
		if((UINT)wParam==FLASH_TIMER_ID)
		{
			// flash window
			FlashWindow(g_hWnd,
						TRUE);
			
			// ok
			return(NULL);
		}

	// command message
	case(WM_COMMAND):

		// check identifier
		switch(LOWORD(wParam))
		{
		// toggle full-screen command
		case(IDM_TOGGLEFULLSCREEN):
			
			// toggle if necessary
			if(ApplicationToggleFullScreen()&&
			   g_bWindowedOK)
				ResetRenderDevice();

			// ok
			return(NULL);
		}
		
		// ok
		break;

	// paint message
	case(WM_PAINT):

		// get device context
		hDC=GetDC(g_hWnd);

		// check value
		if(hDC)
		{
			// add resource
			AddWinResource;
		
			// update client area
			UpdateClientArea();
			
			// display bitmap if available
			if(g_lpPauseBmpData)
			{
				// copy bitmap
				SetDIBitsToDevice(hDC,
								  0,0,
								  g_rClient.dwX,
								  g_rClient.dwY,
								  0,0,
								  0,
								  g_rClient.dwY,
								  (LPVOID)g_lpPauseBmpData,
								  (LPBITMAPINFO)&g_bih,
								  DIB_RGB_COLORS);
			}
			else
			{
				// select black brush
				SelectObject(hDC,
							 GetStockObject(BLACK_BRUSH));

				// paint window
				PatBlt(hDC,
					   0,0,
					   (int)g_rClient.dwX,
					   (int)g_rClient.dwY,
					   PATCOPY);

				// de-select object
				SelectObject(hDC,
							 NULL);
			}

			// delete device context
			DeleteDC(hDC);
			
			// remove resource
			RemoveWinResource;
		}

		// ok
		break;

	// mouse events
	case(WM_MOUSEMOVE):
		if(g_bWinMouse)
		{
			// get positions
			float fX=(float)(LOWORD(lParam));
			float fY=(float)(HIWORD(lParam));

			// set scaled positions
			g_nMouseX=(int)((fX/g_rClient.fX)*640);
			g_nMouseY=(int)((fY/g_rClient.fY)*480);

			// ok
			return(NULL);
		}

		// ok
		break;
	}

	// let windows process all other messages
	return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

// PrimaryMessageLoop()

void PrimaryMessageLoop(void)
{
	// frame target tick count
	DWORD dwFrameTickCount;

	// frame carry-over
	DWORD dwFrameCarryOver;
	
	// fps target tick count
	DWORD dwFPSTickCount;
	
	// fps frame counter
	DWORD dwFPSFrameCount;
	
	// ups frame counter
	DWORD dwUPSFrameCount;
	
	// timer refresh flag
	BOOL bTimerRefresh=TRUE;

	// loop until function returns
	while(TRUE)
	{
		// process pending messages
		if(!ProcessPendingMessages())
			return;
			
		// do nothing if device is lost
		if(FAILED(g_lpD3DDevice->TestCooperativeLevel()))
		{
			// attempt to recover device
			ResetRenderDevice();

			// reset timers
			bTimerRefresh=TRUE;
		}
		else
		{
			// check execution flags
			if(!g_bIdle&&
			   !g_bClosing)
			{
				// refresh timers if necessary
				if(bTimerRefresh)
				{
					// refresh tick counts
					dwFrameTickCount=GetTickCount()+MSEC_PER_FRAME;
					dwFPSTickCount=GetTickCount()+MSEC_PER_SEC;

					// reset other counters
					dwFrameCarryOver=0;
					dwFPSFrameCount=0;
					dwUPSFrameCount=0;
					g_dwFrameSkip=1;
					g_dwFPS=0;
					g_dwUPS=0;

					// reset timer reset flag
					bTimerRefresh=FALSE;
				}
				
				// do not allow large frame skips
				if(g_dwFrameSkip>MAX_FRAME_SKIP)
					g_dwFrameSkip=MAX_FRAME_SKIP;

				// call application loop if necessary
				if(g_dwFrameSkip)
				{
					// process mouse
					ProcessMouseTracking();

					// execute application
					if(!ApplicationLoop())
						SendCloseMessage();

					// increment counters
					dwFPSFrameCount++;
					dwUPSFrameCount+=g_dwFrameSkip;
				}

				// save current tick count
				DWORD dwTickCount=GetTickCount();
				
				// perform timing tasks
				if(dwTickCount<dwFrameTickCount)
				{
					// do not advance until frame tick
					g_dwFrameSkip=0;
				}
				else
				{
					// reset frame counter
					g_dwFrameSkip=1;

					// compute frame carry-over
					dwFrameCarryOver+=dwTickCount-dwFrameTickCount;

					// skip frames as needed
					while(dwFrameCarryOver>=MSEC_PER_FRAME)
					{
						// increment frame skip
						g_dwFrameSkip++;

						// update frame carry-over
						dwFrameCarryOver-=MSEC_PER_FRAME;
					}

					// increment counter
					dwFrameTickCount=dwTickCount+MSEC_PER_FRAME;
				}
				
				// perform frame rate tasks
				if(dwTickCount>dwFPSTickCount)
				{
					// store frame rates
					g_dwFPS=dwFPSFrameCount;
					g_dwUPS=dwUPSFrameCount;

					// reset frame counters
					dwFPSFrameCount=0;
					dwUPSFrameCount=0;

					// reset tick count
					dwFPSTickCount=dwTickCount+MSEC_PER_SEC;
				}

				// update timers
				UpdateTimers();
			}
			else
			{
				// make sure timers are reset after returning from idle
				bTimerRefresh=TRUE;
			}
		}
	}
}

// DisplayErrorMessage()

void DisplayErrorMessage(LPSTR lpError,
						 LPSTR lpSolution,
						 BOOL bExtInfo)
{
	// check error string
	if(lpError)
	{
		// add error message
		strcpy(g_lpMsg,lpError);
	}
	else
	{
		// add undefined error
		strcpy(g_lpMsg,"Undefined error.");
	}

	// check solution string
	if(lpSolution)
	{
		// add line break
		strcat(g_lpMsg,"\n\n");

		// add possible solutions
		strcat(g_lpMsg,lpSolution);
	}
	
	// check extended info flag
	if(bExtInfo)
	{
		// add extended info
		strcat(g_lpMsg,"\n\nSystem Message: ");

		// add system message
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
					  NULL,
					  GetLastError(),
					  NULL,
					  g_lpMsg+strlen(g_lpMsg),
					  (_MAX_PATH-1)-strlen(g_lpMsg),
					  NULL);
	}

	// display message box
	DisplayMessageBox(g_hWnd,
					  g_lpMsg,
					  (LPSTR)ApplicationName(),
					  MB_OK|MB_ICONEXCLAMATION);
}

// DisplayFatalErrorMessage()

void DisplayFatalErrorMessage(LPSTR lpError,
							  LPSTR lpSolution,
							  BOOL bExtInfo)
{
	// display error message
	DisplayErrorMessage(lpError,lpSolution,bExtInfo);

#ifdef DEBUG_MODE
	// query user
	if(DisplayMessageBox(g_hWnd,
						 "Continue execution?",
						 (LPSTR)ApplicationName(),
					     MB_YESNO|MB_ICONQUESTION)==IDYES)
		return;
#endif

	// end execution
	ForceImmediateExit();
}

// DisplayDXErrorMessage()

void DisplayDXErrorMessage(LPSTR lpError,
						   LPSTR lpSolution,
						   HRESULT hError)
{
	// check error flag
	if(lpError)
	{
		// add error message
		strcpy(g_lpMsg,lpError);
	}
	else
	{
		// add undefined error
		strcpy(g_lpMsg,"Undefined error.");
	}

	// check solution string
	if(lpSolution)
	{
		// add line break
		strcat(g_lpMsg,"\n\n");

		// add possible solutions
		strcat(g_lpMsg,lpSolution);
	}
	
	// check for extended info
	if(hError)
	{
		// add extended info
		strcat(g_lpMsg,"\n\nSystem Message: ");

		// copy dx error description
		strncpy(g_lpMsg+strlen(g_lpMsg),
				DXGetErrorDescription8(hError),
				(_MAX_PATH-1)-strlen(g_lpMsg));

		// set null terminator
		g_lpMsg[_MAX_PATH-1]=0;
	}

	// display message box
	DisplayMessageBox(g_hWnd,
					  g_lpMsg,
					  (LPSTR)ApplicationName(),
					  MB_OK|MB_ICONEXCLAMATION);
}

// DisplayFatalDXErrorMessage()

void DisplayFatalDXErrorMessage(LPSTR lpError,
							    LPSTR lpSolution,
							    HRESULT hError)
{
	// display error message
	DisplayDXErrorMessage(lpError,lpSolution,hError);

#ifdef DEBUG_MODE
	// query user
	if(DisplayMessageBox(g_hWnd,
						 "Continue execution?",
						 (LPSTR)ApplicationName(),
					     MB_YESNO|MB_ICONQUESTION)==IDYES)
		return;
#endif

	// end execution
	ForceImmediateExit();
}

// DisplayInfoMessage()

void DisplayInfoMessage(LPSTR lpError)
{
	// check error string
	if(lpError)
	{
		// display error message
		DisplayMessageBox(g_hWnd,
						  lpError,
						  (LPSTR)ApplicationName(),
						  MB_OK|MB_ICONEXCLAMATION);
	}
	else
	{
		// display undefined message
		DisplayMessageBox(g_hWnd,
						  "Unknown message.",
						  (LPSTR)ApplicationName(),
						  MB_OK|MB_ICONEXCLAMATION);
	}
}

// InitializeAllModules()

BOOL InitializeAllModules(void)
{
	// begin exclusive mode
	BeginExclusiveMode();

	// set data path
	if(!SetDataPath(ApplicationDataPath()))
		return(FALSE);

	// create application window
	if(!CreateApplicationWindow())
		return(FALSE);

	// initialize misc items
	if(!InitAcceleratorTable())
		return(FALSE);
	if(!InitRandomNumbers())
		return(FALSE);
	
	// enable mouse tracking
	EnableMouseTracking();

	// initialize primary modules
	if(!DXGraphicsInit())
		return(FALSE);
	if(!DXAudioInit())
		return(FALSE);
	if(!DXInputInit())
		return(FALSE);
	if(!ApplicationInit())
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseAllModules()

void ReleaseAllModules(void)
{
	// exit idle mode
	ExitIdleMode();

	// release primary modules
	ApplicationRelease();
	DXInputRelease();
	DXAudioRelease();
	DXGraphicsRelease();

	// release misc items
	ReleaseAcceleratorTable();
	ReleaseRandomNumbers();

	// purge objects
	PurgeSystemObjects();

	// destroy window & associated resources
	DestroyApplicationWindow();

	// end exclusive mode
	EndExclusiveMode();
}

// EnterIdleMode()

void EnterIdleMode(void)
{
	// check flags
	if(!g_bIdle&&
	   !g_bClosing)
	{
		// set flag
		g_bIdle=TRUE;

		// end exclusive mode
		EndExclusiveMode();

		// show cursor
		ShowWindowsCursor();

		// pause application
		D3DCOLOR clrFilter=ApplicationPause();

		// pause audio
		PauseAudioOutput();

		// capture pause bitmap
		CapturePauseBitmap(clrFilter);

		// create timer for flashing window
		SetTimer(g_hWnd,
				 FLASH_TIMER_ID,
				 FLASH_TIMER_DURATION,
				 NULL);
	}
}

// ExitIdleMode()

void ExitIdleMode(void)
{
	// check flags
	if(g_bIdle)
	{
		// begin exclusive mode
		BeginExclusiveMode();

		// hide cursor
		HideWindowsCursor();

		// disable flash timer
		KillTimer(g_hWnd,
				  FLASH_TIMER_ID);
		
		// un-flash window
		FlashWindow(g_hWnd,
					FALSE);

		// release pause bitmap
		ReleasePauseBitmap();

		// resume audio
		ResumeAudioOutput();

		// resume application
		ApplicationResume();

		// set flag
		g_bIdle=FALSE;
	}
}

// SendCloseMessage()

void SendCloseMessage(void)
{
	// send message
	SendMessage(g_hWnd,WM_CLOSE,0,0);
}

// SwapPtrs()

void SwapPtrs(LPVOID* lplpPtr1,
 			  LPVOID* lplpPtr2)
{
	// swap pointers
	LPVOID lpTemp=(*lplpPtr2);
	(*lplpPtr2)=(*lplpPtr1);
	(*lplpPtr1)=lpTemp;
}

// AllocMem()

BOOL AllocMem(LPVOID* lplpPtr,
 			  DWORD dwSize)
{
	// allocate memory
	(*lplpPtr)=malloc(dwSize);

	// check for error
	if(!(*lplpPtr))
	{
		DisplayFatalErrorMessage("Unable to allocate memory block.");
		return(FALSE);
	}
	else
		AddMemResource;

	// ok
	return(TRUE);
}

// ExpandMem()

BOOL ExpandMem(LPVOID* lplpPtr,
 			   DWORD dwSize,
 			   DWORD dwExpAmt)
{
	// allocate new block
	LPVOID lpVoid;
	if(!AllocMem(&lpVoid,dwSize+dwExpAmt))
		return(FALSE);

	// copy memory
	CopyMemory(lpVoid,(*lplpPtr),dwSize);

	// swap pointers
	SwapPtrs(&lpVoid,lplpPtr);

	// free old block
	FreeMem(&lpVoid);

	// ok
	return(TRUE);
}

// CompressMem()

BOOL CompressMem(LPVOID* lplpPtr,
 			     DWORD dwSize)
{
	// allocate new block
	LPVOID lpVoid;
	if(!AllocMem(&lpVoid,dwSize))
		return(FALSE);

	// copy memory
	CopyMemory(lpVoid,(*lplpPtr),dwSize);

	// swap pointers
	SwapPtrs(&lpVoid,lplpPtr);

	// free old block
	FreeMem(&lpVoid);

	// ok
	return(TRUE);
}

// FreeMem()

void FreeMem(LPVOID* lplpPtr)
{
	// check pointer
	if(*lplpPtr)
	{
		// free memory
		free(*lplpPtr);
		RemoveMemResource;

		// clear pointer
		*lplpPtr=NULL;
	}
}

// GetNextTimer()

DWORD GetNextTimer(void)
{
	// check counter
	if(g_dwNumTimer==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwTimerLast,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwTimerInc,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTimer=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTimer>=g_dwMaxTimer)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwTimerLast,
					  sizeof(DWORD)*g_dwMaxTimer,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwTimerInc,
					  sizeof(DWORD)*g_dwMaxTimer,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTimer+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumTimer;

	// increment counter
	g_dwNumTimer++;

	// return index
	return(dwIndex);
}

// AddTimer()

DWORD AddTimer(DWORD dwFramesPerTick)
{
	// get return index
	DWORD dwIndex=GetNextTimer();

	// reset timer
	g_lpdwTimerLast[dwIndex]=g_dwAppTimer;

	// set increment
	g_lpdwTimerInc[dwIndex]=dwFramesPerTick;

	// return index
	return(dwIndex);
}

// UpdateTimers()

void UpdateTimers(void)
{
	// increment timer using frameskip
	g_dwAppTimer+=g_dwFrameSkip;
}

// CheckTimer()

DWORD CheckTimer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTimer)
	{
		DisplayFatalErrorMessage("Invalid timer index.",
								 "CheckTimer()",
								 FALSE);
		return(0);
	}
#endif

	// reset tick counter
	DWORD dwCount=0;
	
	// count number of ticks passed since last call
	while(g_lpdwTimerLast[dwIndex]<g_dwAppTimer)
	{
		// increment last timer
		g_lpdwTimerLast[dwIndex]+=g_lpdwTimerInc[dwIndex];

		// increment tick counter
		dwCount++;
	}

	// return number of ticks
	return(dwCount);
}

// PurgeTimers()

void PurgeTimers(void)
{
	// check counter
	if(g_dwNumTimer)
	{
		// free memory
		FreeMem((LPVOID*)&g_lpdwTimerLast);
		FreeMem((LPVOID*)&g_lpdwTimerInc);
		
		// reset counters
		g_dwNumTimer=0;
		g_dwMaxTimer=0;
	}
}

// InitRandomNumbers()

BOOL InitRandomNumbers(void)
{
	// counter
	DWORD dwCount;

	// allocate tables
	if(!AllocMem((LPVOID*)&g_lpfRand,
				 sizeof(float)*RANDOM_NUMBERS))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpnRand,
				 sizeof(int)*RANDOM_NUMBERS))
		return(FALSE);

	// seed number generator
	srand(GetTickCount());

	// create tables
	for(dwCount=0;dwCount<RANDOM_NUMBERS;dwCount++)
	{
		// set float value
		g_lpfRand[dwCount]=((float)(rand()%10000))/9999;
		
		// negate if necessary
		if(rand()%2)
			g_lpfRand[dwCount]=-g_lpfRand[dwCount];

		// set int value
		g_lpnRand[dwCount]=rand();
		
		// negate if necessary
		if(rand()%2)
			g_lpnRand[dwCount]=-g_lpnRand[dwCount];
	}

	// ok
	return(TRUE);
}

// GetRandomFloat()

float GetRandomFloat(BOOL bPrevious)
{
	// current number
	static DWORD dwCur=0;
	
	// check flag
	if(!bPrevious)
	{
		// get next number
		dwCur++;

		// prevent overflow
		if(dwCur>=RANDOM_NUMBERS)
			dwCur=0;
	}

	// return value
	return(g_lpfRand[dwCur]);
}

// GetRandomInt()

int GetRandomInt(BOOL bPrevious)
{
	// current number
	static DWORD dwCur=0;
	
	// check flag
	if(!bPrevious)
	{
		// get next number
		dwCur++;
		
		// prevent overflow
		if(dwCur>=RANDOM_NUMBERS)
			dwCur=0;
	}

	// return value
	return(g_lpnRand[dwCur]);
}

// ReleaseRandomNumbers()

void ReleaseRandomNumbers(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpfRand);
	FreeMem((LPVOID*)&g_lpnRand);
}

// VerifyClassObject()

BOOL VerifyClassObject(LPVOID lpPtr)
{
	// check pointer
	if(!lpPtr)
	{
		DisplayFatalErrorMessage("Unable to allocate class object.",
								 "Check available memory.",
								 FALSE);
		return(FALSE);
	}
	
	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// DisplayResourceUsage()

void DisplayResourceUsage(void)
{
	// display system resources
	sprintf(g_lpMsg,
			"System Resources\n\n"
			"System: %d\n"
			"Memory: %d\n"
			"Class: %d\n"
			"COM: %d\n"
			"Timers: %d\n"
			"Dialog Boxes: %d\n",
			g_dwNumWinRes,
			g_dwNumMemRes,
			g_dwNumClassRes,
			g_dwNumCOMRes,
			g_dwNumTimer,
			g_dwNumDlg);
	DisplayInfoMessage(g_lpMsg);

	// display graphical resources
	sprintf(g_lpMsg,
			"Graphical Resources\n\n"
			"Vertex Buffers: %d\n"
			"Index Buffers: %d\n"
			"Matrices: %d\n"
			"Materials: %d\n"
			"Textures: %d\n"
			"Lights: %d\n"
			"Meshes: %d\n"
			"Skin Meshes: %d\n"
			"Fonts: %d\n"
			"Bones: %d\n"
			"Poses: %d\n",
			g_dwNumVtxBuf,
			g_dwNumIdxBuf,
			g_dwNumMtx,
			g_dwNumMat,
			g_dwNumTex,
			g_dwNumLight,
			g_dwNumMesh,
			g_dwNumSkin,
			g_dwNumFont,
			g_dwNumBone,
			g_dwNumPose);
	DisplayInfoMessage(g_lpMsg);

	// display audio resources
	sprintf(g_lpMsg,
			"Audio Resources\n\n"
			"Segments: %d\n"
			"Audio Paths: %d\n",
			g_dwNumAudioSeg,
			g_dwNumAudioPath);
	DisplayInfoMessage(g_lpMsg);

	// display input resources
	sprintf(g_lpMsg,
			"Input Resources\n\n"
			"Buttons: %d\n"
			"Axes: %d\n"
			"Button Triggers: %d\n"
			"Axis Triggers: %d\n",
			g_dwNumButton,
			g_dwNumAxis,
			g_dwNumButtonTrig,
			g_dwNumAxisTrig);
	DisplayInfoMessage(g_lpMsg);
}
#endif

// ConfirmResourcesReleased()

void ConfirmResourcesReleased(void)
{
	// compute total resources
	DWORD dwTotalRes=g_dwNumWinRes+
					 g_dwNumMemRes+
					 g_dwNumClassRes+
					 g_dwNumCOMRes+
					 g_dwNumTimer+
					 g_dwNumDlg+
					 g_dwNumVtxBuf+
					 g_dwNumIdxBuf+
					 g_dwNumMtx+
					 g_dwNumMat+
					 g_dwNumTex+
					 g_dwNumLight+
					 g_dwNumMesh+
					 g_dwNumSkin+
					 g_dwNumFont+
					 g_dwNumBone+
					 g_dwNumPose+
					 g_dwNumAudioSeg+
					 g_dwNumAudioPath+
					 g_dwNumButton+
					 g_dwNumAxis+
					 g_dwNumButtonTrig+
					 g_dwNumAxisTrig;

	// if resources are left over, speak up!
	if(dwTotalRes!=0)
	{
		DisplayInfoMessage("Some resources were not able to be released.\nYour system may become unstable.");
#ifdef DEBUG_MODE
		DisplayResourceUsage();
#endif
	}
}

// OpenRegistry()

BOOL OpenRegistry(void)
{
	// key creation disposition
	DWORD dwCreationDisp;
	
	// open software key
	if(RegCreateKeyEx(HKEY_CURRENT_USER,
				      "Software",
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_ALL_ACCESS,
				      NULL,
				      &g_hSoftKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		DisplayErrorMessage("Unable to open registry key.",
							"Configuration settings may be unavailable.",
							FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// open application key
	if(RegCreateKeyEx(g_hSoftKey,
				      ApplicationName(),
				      NULL,
				      NULL,
				      REG_OPTION_NON_VOLATILE,
				      KEY_ALL_ACCESS,
				      NULL,
				      &g_hAppKey,
				      &dwCreationDisp)!=ERROR_SUCCESS)
	{
		CloseRegistry();
		DisplayErrorMessage("Unable to open registry key.",
							"Configuration settings may be unavailable.",
							FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// CloseRegistry()

void CloseRegistry(void)
{
	// close application key
	if(g_hAppKey!=INVALID_HANDLE_VALUE)
	{
		// save all values
		if(RegFlushKey(g_hAppKey)!=ERROR_SUCCESS)
			DisplayErrorMessage("Unable to save registry key.",
								"Configuration settings may not be stored.",
								FALSE);

		// close key
		if(RegCloseKey(g_hAppKey)!=ERROR_SUCCESS)
			DisplayErrorMessage("Unable to close registry key.",
								"Configuration settings may not be stored.",
								FALSE);
		else
		{
			g_hAppKey=(HKEY)INVALID_HANDLE_VALUE;
			RemoveWinResource;
		}
	}
	
	// close software key
	if(g_hSoftKey!=INVALID_HANDLE_VALUE)
	{
		// save all values
		if(RegFlushKey(g_hSoftKey)!=ERROR_SUCCESS)
			DisplayErrorMessage("Unable to save registry key.",
								"Configuration settings may not be stored.",
								FALSE);

		// close key
		if(RegCloseKey(g_hSoftKey)!=ERROR_SUCCESS)
			DisplayErrorMessage("Unable to close registry key.",
								"Configuration settings may not be stored.",
								FALSE);
		else
		{
			g_hSoftKey=(HKEY)INVALID_HANDLE_VALUE;
			RemoveWinResource;
		}
	}
}

// SetRegistryKey()

void SetRegistryKey(LPCSTR lpName,
					LPSTR lpData)
{
	// open registry
	if(OpenRegistry())
	{
		// set value
		if(RegSetValueEx(g_hAppKey,
						 lpName,
						 NULL,
						 REG_SZ,
						 (CONST LPBYTE)lpData,
						 strlen(lpData)+1)!=ERROR_SUCCESS)
		{
			DisplayErrorMessage("Unable to set registry key value.",
								"Configuration settings may not be saved.",
								FALSE);
		}
	}

	// close registry
	CloseRegistry();
}

// GetRegistryKey()

LPSTR GetRegistryKey(LPCSTR lpName)
{
	// pointer to return
	LPSTR lpData;

	// open registry
	if(OpenRegistry())
	{
		// get buffer size
		DWORD dwSize;
		DWORD dwType;
		RegQueryValueEx(g_hAppKey,
						lpName,
						NULL,
						&dwType,
						NULL,
						&dwSize);

		// check for zero or excessively large size
		if(!dwSize||
		   dwSize>128)
		{
			CloseRegistry();
			return(NULL);
		}
		
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpData,
					 dwSize))
			return(NULL);

		// read value
		if(RegQueryValueEx(g_hAppKey,
						   lpName,
						   NULL,
						   &dwType,
						   (LPBYTE)lpData,
						   &dwSize)!=ERROR_SUCCESS)
			// key does not exist
			lpData=NULL;
	}

	// close registry
	CloseRegistry();

	// return value
	return(lpData);
}

// FeeeRegistryKey()

void FreeRegistryKey(LPSTR lpData)
{
	// free previously allocated memory
	FreeMem((LPVOID*)&lpData);
}

// DeleteRegistryKey()

void DeleteRegistryKey(LPCSTR lpName)
{
	// open registry
	if(OpenRegistry())
	{
		// delete value
		RegDeleteValue(g_hAppKey,
					   lpName);
	}

	// close registry
	CloseRegistry();
}

#ifdef DEBUG_MODE
// OpenScriptCompiler()

BOOL OpenScriptCompiler(LPCSTR lpDestFile,
						LPCSTR lpSrcFile)
{
	// check handles
	if(g_hScriptFile!=INVALID_HANDLE_VALUE||
	   g_hCScriptFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple script files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open source file
	g_hScriptFile=CreateFile(lpSrcFile,
							 GENERIC_READ,
							 NULL,
							 NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL|
							 FILE_FLAG_SEQUENTIAL_SCAN,
							 NULL);

	// check for error
	if(!g_hScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script source file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// open dest file
	g_hCScriptFile=CreateFile(lpDestFile,
							  GENERIC_WRITE,
							  NULL,
							  NULL,
							  CREATE_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL|
							  FILE_FLAG_SEQUENTIAL_SCAN,
							  NULL);

	// check for error
	if(!g_hCScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script destination file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// OpenScriptCreator()

BOOL OpenScriptCreator(LPCSTR lpFile)
{
	// check handle
	if(g_hScriptFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple script files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open source file
	g_hScriptFile=CreateFile(lpFile,
							 GENERIC_WRITE,
							 NULL,
							 NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL|
							 FILE_FLAG_SEQUENTIAL_SCAN,
							 NULL);

	// check for error
	if(!g_hScriptFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open script source file.",
								 NULL,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// ReadScriptChar()

char ReadScriptChar(void)
{
	// bytes read
	DWORD dwBytesRead;

	// return value
	char cVal;

	// read byte
	ReadFile(g_hScriptFile,
			 (LPVOID)&cVal,
			 sizeof(char),
			 &dwBytesRead,
			 NULL);

	// check for error
	if(dwBytesRead!=sizeof(char))
	{
		DisplayFatalErrorMessage("Unable to read byte from script.");
		return(NULL);
	}

	// return value
	return(cVal);
}

// ReadScriptLong()

long ReadScriptLong(void)
{	
	// read string
	ReadScriptString(g_lpMsg);

	// convert string to long
	return(atol(g_lpMsg));
}

// ReadScriptFloat()

float ReadScriptFloat(void)
{
	// read string
	ReadScriptString(g_lpMsg);

	// convert string to float
	return((float)atof(g_lpMsg));
}

// ReadScriptString()

BOOL ReadScriptString(LPSTR lpStr)
{
	// character read
	char cData;
	
	// string position
	DWORD dwStrPos=0;

	// read leading bytes
	while(TRUE)
	{
		// read character
		cData=ReadScriptChar();

		// check for comment
		if(cData=='<')
			while(cData!='>')
				cData=ReadScriptChar();
		
		// check for valid characters
		if(IsValidScriptChar(cData))
			break;
	}

	// read to end of data
	while(TRUE)
	{
		// store byte
		lpStr[dwStrPos++]=cData;

		// next byte
		cData=ReadScriptChar();

		// check for comment
		if(cData=='<')
			while(cData!='>')
				cData=ReadScriptChar();
		
		// check for valid characters
		if(!IsValidScriptChar(cData))
			break;
	}

	// set string terminator
	lpStr[dwStrPos]=NULL;

	// ok
	return(TRUE);
}

// SaveScriptLong()

BOOL SaveScriptLong(long lVal)
{
	// bytes written
	DWORD dwBytesWritten;
	
	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)&lVal,
			  sizeof(long),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(long))
	{
		DisplayFatalErrorMessage("Unable to output DWORD to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SaveScriptFloat()

BOOL SaveScriptFloat(float fVal)
{
	// bytes written
	DWORD dwBytesWritten;
	
	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)&fVal,
			  sizeof(float),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(float))
	{
		DisplayFatalErrorMessage("Unable to output FLOAT to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SaveScriptString()

BOOL SaveScriptString(LPSTR lpStr)
{
	// bytes written
	DWORD dwBytesWritten;

	// bytes to write
	DWORD dwBytesToWrite=strlen(lpStr)+1;

	// output data
	WriteFile(g_hCScriptFile,
			  (LPVOID)lpStr,
			  dwBytesToWrite,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dwBytesToWrite)
	{
		DisplayFatalErrorMessage("Unable to output STRING to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// CopyScriptLong()

BOOL CopyScriptLong(void)
{
	// copy value
	return(SaveScriptLong(ReadScriptLong()));
}

// CopyScriptFloat()

BOOL CopyScriptFloat(void)
{
	// copy value
	return(SaveScriptFloat(ReadScriptFloat()));
}

// CopyScriptString()

BOOL CopyScriptString(void)
{
	// read string
	if(ReadScriptString(g_lpMsg))
	{
		// save string
		return(SaveScriptString(g_lpMsg));
	}
	else
		return(FALSE);
}

// CloseScriptCompiler()
void CloseScriptCompiler(void)
{
	// close source file
	if(!CloseHandle(g_hScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script source file.");
		return;
	}
	else
	{	
		// reset handle
		g_hScriptFile=INVALID_HANDLE_VALUE;
		
		// remove resource
		RemoveWinResource;
	}

	// close destination file
	if(!CloseHandle(g_hCScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script destination file.");
		return;
	}
	else
	{	
		// reset handle
		g_hCScriptFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}
}

// CloseScriptCreator()

void CloseScriptCreator(void)
{
	// close source file
	if(!CloseHandle(g_hScriptFile))
	{
		DisplayFatalErrorMessage("Unable to close script source file.");
		return;
	}
	else
	{	
		// reset handle
		g_hScriptFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}
}
#endif

// OpenCompiledScript()

BOOL OpenCompiledScript(LPCSTR lpFile)
{
	// check for data file reference
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// check value and load accordingly
	if(dfr.lpData)
	{
		// check memory block
		if(g_lpCScriptData)
		{
			DisplayFatalErrorMessage("Unable to open multiple script files.",
									 NULL,
									 FALSE);
			return(FALSE);
		}

		// allocate memory for data
		if(!AllocMem((LPVOID*)&g_lpCScriptData,
					 dfr.dwSize))
			return(FALSE);

		// read data
		CopyMemory(g_lpCScriptData,
				   dfr.lpData,
				   dfr.dwSize);
	}
	else
	{
		// check memory block
		if(g_lpCScriptData)
		{
			DisplayFatalErrorMessage("Unable to open multiple script files.",
									 NULL,
									 FALSE);
			return(FALSE);
		}

		// open file
		g_hCScriptFile=CreateFile(lpFile,
								  GENERIC_READ,
								  NULL,
								  NULL,
								  OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL|
								  FILE_FLAG_SEQUENTIAL_SCAN,
								  NULL);

		// check for error
		if(!g_hCScriptFile)
		{
			g_hCScriptFile=INVALID_HANDLE_VALUE;
			DisplayFatalErrorMessage("Unable to open script file.",
									 NULL,
									 FALSE);
			return(FALSE);
		}
		else
			AddWinResource;

		// get size of file
		DWORD dwHSize;
		DWORD dwSize=GetFileSize(g_hCScriptFile,
								 &dwHSize);

		// check for error
		if(!dwSize||dwHSize)
		{
			DisplayFatalErrorMessage("Unable to read script file.",
									 NULL,
									 FALSE);
			return(FALSE);
		}

		// allocate memory for data
		if(!AllocMem((LPVOID*)&g_lpCScriptData,
					 dwSize))
			return(FALSE);

		// read data
		DWORD dwBytesRead;
		ReadFile(g_hCScriptFile,
				 (LPVOID)g_lpCScriptData,
				 dwSize,
				 &dwBytesRead,
				 NULL);

		// check for error
		if(dwBytesRead!=dwSize)
		{
			DisplayFatalErrorMessage("Unable to read script data.");
			return(FALSE);
		}
		
		// close destination file
		if(!CloseHandle(g_hCScriptFile))
		{
			DisplayFatalErrorMessage("Unable to close script destination file.");
			return(FALSE);
		}
		else
		{	
			// reset handle
			g_hCScriptFile=INVALID_HANDLE_VALUE;

			// remove resource
			RemoveWinResource;
		}
	}

	// reset pointer
	g_dwCScriptDataPtr=0;

	// ok
	return(TRUE);
}

// ReadCScriptChar()

char ReadCScriptChar(void)
{
	// read value
	char* lpc=(char*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(char);
	
	// return value
	return(*lpc);
}

// ReadCScriptBytes()

void ReadCScriptBytes(LPVOID lpDest,
					  DWORD dwNumBytes)
{
	// copy data
	CopyMemory(lpDest,
			   &g_lpCScriptData[g_dwCScriptDataPtr],
			   dwNumBytes);

	// increment pointer
	g_dwCScriptDataPtr+=dwNumBytes;
}

// ReadCScriptLong()

long ReadCScriptLong(void)
{
	// read value
	long* lpl=(long*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(long);
	
	// return value
	return(*lpl);
}

// ReadCScriptFloat()

float ReadCScriptFloat(void)
{
	// read value
	float* lpf=(float*)&g_lpCScriptData[g_dwCScriptDataPtr];

	// increment pointer
	g_dwCScriptDataPtr+=sizeof(float);

	// return value
	return(*lpf);
}

// ReadCScriptString()

void ReadCScriptString(LPSTR lpStr)
{
	// string pointer
	DWORD dwStrPtr=0;
	
	// read bytes until end of string
	while(TRUE)
	{
		// read byte
		lpStr[dwStrPtr]=ReadCScriptChar();

		// check for null terminator
		if(!lpStr[dwStrPtr])
			break;

		// increment pointer
		dwStrPtr++;
	}
}

// CloseCompiledScript()

void CloseCompiledScript(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpCScriptData);
}

// EnableMouseTracking()

void EnableMouseTracking(void)
{
	// set flag
	g_bWinMouse=TRUE;
}

// DisableMouseTracking()

void DisableMouseTracking(void)
{
	// set flag
	g_bWinMouse=FALSE;
}

#ifdef DEBUG_MODE
// CreateScriptString()

BOOL CreateScriptString(LPSTR lpStr)
{
	// copy string to global & append new line
	strcpy(g_lpMsg,lpStr);
	strcat(g_lpMsg,"\r\n");
	
	// bytes written
	DWORD dwBytesWritten;

	// bytes to write
	DWORD dwBytesToWrite=strlen(g_lpMsg);

	// output data
	WriteFile(g_hScriptFile,
			  (LPVOID)g_lpMsg,
			  dwBytesToWrite,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dwBytesToWrite)
	{
		DisplayFatalErrorMessage("Unable to output STRING to script.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}
#endif

// GetNextDialog()

DWORD GetNextDialog(void)
{
	// check counter
	if(g_dwNumDlg==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpDlgData,
					 sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDlgDataPtr,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwDlgDataSize,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxDlg=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTimer>=g_dwMaxTimer)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpDlgData,
					  sizeof(LPBYTE)*g_dwMaxDlg,
					  sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDlgDataPtr,
					  sizeof(DWORD)*g_dwMaxDlg,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwDlgDataSize,
					  sizeof(DWORD)*g_dwMaxDlg,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDlg+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumDlg;

	// increment counter
	g_dwNumDlg++;

	// return index
	return(dwIndex);
}

// AddDialog()

DWORD AddDialog(short sXSize,
			    short sYSize,
				LPCSTR lpTitle)
{
	// get return index
	DWORD dwIndex=GetNextDialog();
	
	// allocate memory for template
	if(!AllocMem((LPVOID*)&g_lplpDlgData[dwIndex],
				 sizeof(BYTE)*BLOCK_ALLOC_INC))
		return(NO_OBJECT);

	// set size
	g_lpdwDlgDataSize[dwIndex]=BLOCK_ALLOC_INC;

	// reset pointer
	g_lpdwDlgDataPtr[dwIndex]=0;

	// add header
	AddDialogDWORD(dwIndex,DS_CENTER|
						   DS_MODALFRAME|
						   DS_SETFOREGROUND|
						   WS_CAPTION); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogShort(dwIndex,0); // number of items
	AddDialogCoords(dwIndex,NULL,NULL); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // menu resource
	AddDialogWORD(dwIndex,NULL); // class id
	AddDialogString(dwIndex,(LPSTR)lpTitle); // title

	// return index
	return(dwIndex);
}

// ExpandDialog()

void ExpandDialog(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "ExpandDialog()",
								 FALSE);
		return;
	}
#endif

	// expand template memory
	if(!ExpandMem((LPVOID*)&g_lplpDlgData[dwIndex],
				  sizeof(BYTE)*g_lpdwDlgDataSize[dwIndex],
				  sizeof(BYTE)*BLOCK_ALLOC_INC))
		return;

	// increment size
	g_lpdwDlgDataSize[dwIndex]+=BLOCK_ALLOC_INC;

	// ok
	return;
}

// AlignDialogWORD()

void AlignDialogWORD(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AlignDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// add byte if necessary
	if(g_lpdwDlgDataPtr[dwIndex]&1)
		AddDialogBYTE(dwIndex,NULL);
}

// AlignDialogDWORD()

void AlignDialogDWORD(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AlignDialogDWORD()",
								 FALSE);
		return;
	}
#endif

	// align to word
	AlignDialogWORD(dwIndex);

	// add word if necessary
	if(g_lpdwDlgDataPtr[dwIndex]&2)
		AddDialogWORD(dwIndex,NULL);
}

// AddDialogDWORD()

void AddDialogDWORD(DWORD dwIndex,
					DWORD dwVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogDWORD()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(DWORD))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &dwVal,
			   sizeof(DWORD));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(DWORD);
}

// AddDialogWORD()

void AddDialogWORD(DWORD dwIndex,
				   WORD wVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(WORD))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &wVal,
			   sizeof(WORD));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(WORD);
}

// AddDialogBYTE()

void AddDialogBYTE(DWORD dwIndex,
				   BYTE cVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogBYTE)",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(BYTE))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &cVal,
			   sizeof(BYTE));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(BYTE);
}

// AddDialogShort()

void AddDialogShort(DWORD dwIndex,
					short sVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogShort()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(short))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &sVal,
			   sizeof(short));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(short);
}

void AddDialogString(DWORD dwIndex,
					 LPSTR lpStr)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog string.",
								 "AddDialogWORD()",
								 FALSE);
		return;
	}
#endif

	// compute number of bytes
	DWORD dwNumBytes=(strlen(lpStr)+1)<<1;

	// expand storage if necessary
	while((g_lpdwDlgDataPtr[dwIndex]+dwNumBytes)>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// convert string
	MakeWideString(g_lpWMsg,lpStr);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   (LPVOID)g_lpWMsg,
			   dwNumBytes);

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=dwNumBytes;
}

void AddDialogPtr(DWORD dwIndex,
				  LPVOID lpPtr)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogPtr()",
								 FALSE);
		return;
	}
#endif

	// expand storage if necessary
	if((g_lpdwDlgDataPtr[dwIndex]+sizeof(LPVOID))>=g_lpdwDlgDataSize[dwIndex])
		ExpandDialog(dwIndex);

	// copy data
	CopyMemory((LPVOID)(g_lplpDlgData[dwIndex]+g_lpdwDlgDataPtr[dwIndex]),
			   &lpPtr,
			   sizeof(LPVOID));

	// increment pointer
	g_lpdwDlgDataPtr[dwIndex]+=sizeof(LPVOID);
}

int ExecuteDialog(DWORD dwIndex,
				  DLGPROC lpDlgProc)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "ExecuteDialog()",
								 FALSE);
		return(0);
	}
#endif

	// begin system output
	BOOL bFlag=BeginSystemOutput();
	
	// execute dialog box
	int nVal=DialogBoxIndirect(g_hInst,
							   (LPDLGTEMPLATE)g_lplpDlgData[dwIndex],
							   g_hWnd,
							   lpDlgProc);

	// end system output
	EndSystemOutput(bFlag);

	// return value
	return(nVal);
}

void PurgeDialogs(void)
{
	// check counter
	if(g_dwNumDlg)
	{
		// counter
		DWORD dwCount;

		// release all templates
		for(dwCount=0;dwCount<g_dwNumDlg;dwCount++)
			FreeMem((LPVOID*)&g_lplpDlgData[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lplpDlgData);
		FreeMem((LPVOID*)&g_lpdwDlgDataSize);
		FreeMem((LPVOID*)&g_lpdwDlgDataPtr);

		// reset counters
		g_dwNumDlg=0;
		g_dwMaxDlg=0;
	}
}

// AddDialogCoords()

void AddDialogCoords(DWORD dwIndex,
					 short sX,
					 short sY)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogShort()",
								 FALSE);
		return;
	}
#endif

	// add coordinates
	AddDialogShort(dwIndex,sX);
	AddDialogShort(dwIndex,sY);
}

// AddDialogControl()

void AddDialogControl(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogControl()",
								 FALSE);
		return;
	}
#endif

	// pointer to current number
	short* lpsNum=(short*)(g_lplpDlgData[dwIndex]+(sizeof(DWORD)*2));

	// increment value
	(*lpsNum)++;
}

// AddDialogPushButton()

void AddDialogPushButton(DWORD dwIndex,
						 short sXPos,
						 short sYPos,
						 short sXSize,
						 short sYSize,
						 LPCSTR lpText,
						 WORD wID,
						 BOOL bDefBtn)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogPushButton()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	if(bDefBtn)
		AddDialogDWORD(dwIndex,WS_CHILD|
							   WS_VISIBLE|
							   WS_TABSTOP|
							   BS_DEFPUSHBUTTON); // style
	else
		AddDialogDWORD(dwIndex,WS_CHILD|
							   WS_VISIBLE|
							   WS_TABSTOP|
							   BS_PUSHBUTTON); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogCheckBox()

void AddDialogCheckBox(DWORD dwIndex,
					   short sXPos,
					   short sYPos,
					   short sXSize,
					   short sYSize,
					   LPCSTR lpText,
					   WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogCheckBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   BS_AUTOCHECKBOX); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogRadioButton()

void AddDialogRadioButton(DWORD dwIndex,
						  short sXPos,
						  short sYPos,
						  short sXSize,
						  short sYSize,
						  LPCSTR lpText,
						  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogRadioButton()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   BS_AUTORADIOBUTTON); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogGroupBox()

void AddDialogGroupBox(DWORD dwIndex,
					   short sXPos,
					   short sYPos,
					   short sXSize,
					   short sYSize,
					   LPCSTR lpText)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogGroupBox()",
								 FALSE);
		return;
	}
#endif

	// add group
	AddDialogGroup(dwIndex);
	
	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   BS_GROUPBOX); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0080); // button class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogDropDownListBox()

void AddDialogDropDownListBox(DWORD dwIndex,
							  short sXPos,
							  short sYPos,
							  short sXSize,
							  short sYSize,
						      WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogDropDownListBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   CBS_SORT|
						   CBS_DROPDOWNLIST); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0085); // combo box class
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogEditControl()

void AddDialogEditControl(DWORD dwIndex,
						  short sXPos,
						  short sYPos,
						  short sXSize,
						  short sYSize,
						  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogEditControl()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   WS_BORDER|
						   ES_AUTOHSCROLL); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0081); // edit control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,""); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogListBox()

void AddDialogListBox(DWORD dwIndex,
					  short sXPos,
					  short sYPos,
					  short sXSize,
					  short sYSize,
					  WORD wID)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogListBox()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   WS_TABSTOP|
						   LBS_SORT); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,wID); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0083); // list box class
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogStaticControl()

void AddDialogStaticControl(DWORD dwIndex,
							short sXPos,
							short sYPos,
							short sXSize,
							short sYSize,
							LPCSTR lpText)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogStaticControl()",
								 FALSE);
		return;
	}
#endif

	// add control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD|
						   WS_VISIBLE|
						   SS_CENTER); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,sXPos,sYPos); // position
	AddDialogCoords(dwIndex,sXSize,sYSize); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0082); // static control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,(LPSTR)lpText); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// AddDialogGroup()

void AddDialogGroup(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumDlg)
	{
		DisplayFatalErrorMessage("Invalid dialog index.",
								 "AddDialogGroup()",
								 FALSE);
		return;
	}
#endif

	// add empty static control
	AddDialogControl(dwIndex);
	AlignDialogDWORD(dwIndex);
	AddDialogDWORD(dwIndex,WS_CHILD); // style
	AddDialogDWORD(dwIndex,NULL); // extended style
	AddDialogCoords(dwIndex,0,0); // position
	AddDialogCoords(dwIndex,0,0); // size
	AddDialogWORD(dwIndex,NULL); // identifier
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,0xFFFF); // class specifier
	AddDialogWORD(dwIndex,0x0082); // static control class
	AlignDialogWORD(dwIndex);
	AddDialogString(dwIndex,""); // text
	AlignDialogWORD(dwIndex);
	AddDialogWORD(dwIndex,NULL); // creation data
}

// SetCurrentDialogCheckBox()

BOOL SetCurrentDialogCheckBox(WORD wID,
							  BOOL bVal)
{
	// set flag
	UINT nFlag;
	if(bVal)
		nFlag=BST_CHECKED;
	else
		nFlag=BST_UNCHECKED;
	
	// set button
	if(!CheckDlgButton(g_hCurrDlg,
					   (int)wID,
					   nFlag))
	{
		DisplayErrorMessage("Unable to set dialog button state.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// GetCurrentDialogCheckBox()

BOOL GetCurrentDialogCheckBox(WORD wID)
{
	// check button
	if(IsDlgButtonChecked(g_hCurrDlg,
						  (int)wID)==BST_CHECKED)
		return(TRUE);
	else
		return(FALSE);
}

// SetCurrentDialogEditText()

BOOL SetCurrentDialogEditText(WORD wID,
							  LPSTR lpText)
{
	// set value
	if(!SetDlgItemText(g_hCurrDlg,
					   (int)wID,
					   lpText))
	{
		DisplayErrorMessage("Unable to set dialog edit control text.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// GetCurrentDialogEditText()

BOOL GetCurrentDialogEditText(WORD wID,
							  LPSTR lpText,
							  int nMaxCount)
{
	// get value
	if(!GetDlgItemText(g_hCurrDlg,
					   (int)wID,
					   lpText,
					   nMaxCount))
	{
		DisplayErrorMessage("Unable to read dialog edit control text.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetCurrentDialogEditFloat()

BOOL SetCurrentDialogEditFloat(WORD wID,
							   float fVal)
{
	// convert value to string
	sprintf(g_lpMsg,
			"%f",
			fVal);

	// set text
	return(SetCurrentDialogEditText(wID,
									g_lpMsg));
}

// GetCurrentDialogEditFloat()

float GetCurrentDialogEditFloat(WORD wID)
{
	// get text
	if(GetCurrentDialogEditText(wID,
								g_lpMsg,
								_MAX_PATH))
		return((float)atof(g_lpMsg));
	else
		return(0);
}

// SetCurrentDialogBox()

void SetCurrentDialogBox(HWND hDlg)
{
	// save value
	g_hCurrDlg=hDlg;
}

// DivideString()

BOOL DivideString(LPSTR lpStr)
{
	// counter
	DWORD dwCount;

	// get length of string
	DWORD dwLen=(DWORD)strlen(lpStr);

	// divide string at each "&"
	for(dwCount=0;dwCount<dwLen;dwCount++)
		if(lpStr[dwCount]=='&')
			lpStr[dwCount]=NULL;

	// ok
	return(TRUE);
}

// GetStringDivision()

LPSTR GetStringDivision(LPSTR lpStr,
					    DWORD dwIndex)
{
	// position pointer
	DWORD dwPos=0;
	
	// loop until specified string
	while(dwIndex)
	{
		// find next string
		dwPos+=(DWORD)(strlen(&lpStr[dwPos])+1);
		
		// decrement index
		dwIndex--;
	}

	// return pointer
	return(&lpStr[dwPos]);
}

// DisplayMessageBox()

int DisplayMessageBox(HWND hWnd,
					  LPSTR lpText,
					  LPSTR lpCaption,
					  UINT uType)
{
	// begin system output
	BOOL bFlag=BeginSystemOutput();

	// make sure message box is modal & foreground
	uType=uType|MB_APPLMODAL|MB_SETFOREGROUND;

	// display message box
	int nVal=MessageBox(hWnd,
						lpText,
						lpCaption,
						uType);

	// end system output
	EndSystemOutput(bFlag);

	// return value
	return(nVal);
}

// LockApplication()

BOOL LockApplication(HINSTANCE hInstance)
{
	// attempt to create mutex
	g_hMutex=CreateMutex(NULL,
						 TRUE,
						 ApplicationID());

	// check for error
	if(!g_hMutex)
	{
		DisplayErrorMessage("Unable to register application.");
		return(FALSE);
	}
	else
		AddWinResource;
	
	// check for previous mutex
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		DisplayErrorMessage("Unable to register duplicate applications.",
							"Close any running copies of this program.");
		UnlockApplication();
		return(FALSE);
	}

	// copy instance handle
	g_hInst=hInstance;

	// ok
	return(TRUE);
}

// UnlockApplication()

void UnlockApplication(void)
{
	// release mutex
	if(ReleaseMutex(g_hMutex))
	{
		// reset handle
		g_hMutex=NULL;

		// remove resource
		RemoveWinResource;

		// reset instance handle
		g_hInst=(HINSTANCE)INVALID_HANDLE_VALUE;
	}
	else
		DisplayErrorMessage("Unable to release application lock.",
							"Reboot may be required to run this program.");
}

// AddApplicationWindowClass()

BOOL AddApplicationWindowClass(void)
{
	// create class for this window
	WNDCLASSEX wcx;
	wcx.cbSize=sizeof(WNDCLASSEX);
	wcx.style=CS_OWNDC;
	wcx.lpfnWndProc=WindowProc;
	wcx.cbClsExtra=NULL;
	wcx.cbWndExtra=NULL;
	wcx.hInstance=g_hInst;
	if(ApplicationIcon())
	{
		wcx.hIcon=LoadIcon(g_hInst,ApplicationIcon());
		wcx.hIconSm=LoadIcon(g_hInst,ApplicationIcon());
	}
	else
	{
		wcx.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		wcx.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	}
	if(ApplicationCursor())
		wcx.hCursor=LoadCursor(g_hInst,ApplicationCursor());
	else
		wcx.hCursor=LoadCursor(NULL,IDC_ARROW);
	wcx.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName=ApplicationMenu();
	wcx.lpszClassName=ApplicationID();

	// register window class
	if(!RegisterClassEx(&wcx))
	{
		DisplayErrorMessage("Unable to register window class.",
							"Check available memory.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// RemoveApplicationWindowClass()

void RemoveApplicationWindowClass(void)
{
	// un-register class
	if(UnregisterClass(ApplicationID(),g_hInst))
		RemoveWinResource;
}

// CreateApplicationWindow()

BOOL CreateApplicationWindow(void)
{
	// add window class
	if(!AddApplicationWindowClass())
		return(NULL);

	// set system display mode
	SetSystemDisplayMode();

	// calculate window size and position
	CalculateWindowMetrics();
	
	// create application window
	g_hWnd=CreateWindowEx(NULL,
						  ApplicationID(),
						  ApplicationName(),
						  WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE,
						  g_nXPos,g_nYPos,
						  g_nXSize,g_nYSize,
						  NULL,
						  NULL,
						  g_hInst,
						  NULL);

	// check window handle
	if(!g_hWnd)
	{
		DisplayErrorMessage("Unable to create application window.",
							"Check available memory.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// DestroyApplicationWindow()

void DestroyApplicationWindow(void)
{
	// destroy window
	if(DestroyWindow(g_hWnd))
	{
		// reset handle
		g_hWnd=NULL;
		
		// remove resource
		RemoveWinResource;

		// remove window class
		RemoveApplicationWindowClass();
	}
}

// SetSystemDisplayMode()

void SetSystemDisplayMode(DWORD dwX,
						  DWORD dwY)
{
	// fill in structure
	if(dwX)
	{
		g_dmSys.dwX=dwX;
		g_dmSys.dwY=dwY;
	}
	else
	{
		g_dmSys.dwX=GetSystemMetrics(SM_CXSCREEN);
		g_dmSys.dwY=GetSystemMetrics(SM_CYSCREEN);
	}
	g_dmSys.fmt=GetSystemDisplayFormat();
	g_dmSys.bWindowed=TRUE;
	g_dmSys.bLockable=FALSE;
	g_dmSys.dwBackBufferCount=0;
}

// CalculateWindowMetrics()

void CalculateWindowMetrics(void)
{
	// get application display mode
	g_dmApp=ApplicationDisplayMode();

	// compute window size and position
	g_nXSize=g_dmApp.dwX+(GetSystemMetrics(SM_CXFIXEDFRAME)*2);
	g_nYSize=g_dmApp.dwY+(GetSystemMetrics(SM_CYFIXEDFRAME)*2)+GetSystemMetrics(SM_CYCAPTION);
	g_nXPos=(g_dmSys.dwX-g_nXSize)/2;
	g_nYPos=((g_dmSys.dwY-g_nYSize)/2)-GetSystemMetrics(SM_CYCAPTION);
}

// SetDataPath()

BOOL SetDataPath(LPCSTR lpDir)
{
	// get current directory
	if(!getcwd(g_lpMsg,_MAX_PATH-1))
	{
		DisplayErrorMessage("Unable to read current working directory.");
		return(FALSE);
	}

	// add directory to current path
	strcat(g_lpMsg,"\\");
	strcat(g_lpMsg,lpDir);

	// set directory
	if(chdir(g_lpMsg)!=0)
	{
		DisplayErrorMessage("Unable to set current working directory.");
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ProcessPendingMessages()

BOOL ProcessPendingMessages(void)
{
	// message storage
	MSG Msg;

	// retrieve message
	while(PeekMessage(&Msg,g_hWnd,NULL,NULL,PM_REMOVE))
	{
		// check for quit message
		if(Msg.message==WM_QUIT)
			return(FALSE);
		
		// process accelerator keys
		if(!TranslateAccelerator(g_hWnd,
								 g_hAccel,
								 &Msg))
		{
			// process message
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	// ok
	return(TRUE);
}

// ProcessMouseTracking()

void ProcessMouseTracking(void)
{
	// client area flag
	static BOOL bMouseWasInClient=FALSE;
	
	// check flag
	if(g_bWinMouse)
	{
		// compute delta-position values
		g_nMouseDX=g_nMouseX-g_nMousePX;
		g_nMouseDY=g_nMouseY-g_nMousePY;
		
		// store previous position values
		g_nMousePX=g_nMouseX;
		g_nMousePY=g_nMouseY;

		// check buttons
		g_bMouseBtnL=IsKeyDown(VK_LBUTTON);
		g_bMouseBtnR=IsKeyDown(VK_RBUTTON);
#ifdef DEBUG_MODE
		g_bMouseBtnS=IsKeyDown(VK_SHIFT);
		g_bMouseBtnC=IsKeyDown(VK_CONTROL);
#endif

		// set click flags
		if(!g_bMousePBtnL&&
		   g_bMouseBtnL)
			g_bMouseClkL=TRUE;
		else
			g_bMouseClkL=FALSE;
		if(!g_bMousePBtnR&&
		   g_bMouseBtnR)
			g_bMouseClkR=TRUE;
		else
			g_bMouseClkR=FALSE;
#ifdef DEBUG_MODE
		if(!g_bMousePBtnS&&
		   g_bMouseBtnS)
			g_bMouseClkS=TRUE;
		else
			g_bMouseClkS=FALSE;
		if(!g_bMousePBtnC&&
		   g_bMouseBtnC)
			g_bMouseClkC=TRUE;
		else
			g_bMouseClkC=FALSE;
#endif
		
		// store previous buttons
		g_bMousePBtnL=g_bMouseBtnL;
		g_bMousePBtnR=g_bMouseBtnR;
#ifdef DEBUG_MODE
		g_bMousePBtnS=g_bMouseBtnS;
		g_bMousePBtnC=g_bMouseBtnC;
#endif

		// check mouse against client area
		if(g_nMouseX<0||
		   g_nMouseX>=640||
		   g_nMouseY<0||
		   g_nMouseY>=480)
		{
			// check flag
			if(bMouseWasInClient)
			{
				// show cursor
				ShowWindowsCursor();

				// set flag
				bMouseWasInClient=FALSE;
			}
		}
		else
		{
			// check flag
			if(!bMouseWasInClient)
			{
				// hide cursor
				HideWindowsCursor();

				// set flag
				bMouseWasInClient=TRUE;
			}
		}
	}
}

// BeginExclusiveMode()

void BeginExclusiveMode(void)
{
	// prevent system interference
	SetThreadExecutionState(ES_SYSTEM_REQUIRED|
							ES_DISPLAY_REQUIRED|
							ES_CONTINUOUS);

	// set to high priority
	SetPriorityClass(GetCurrentProcess(),
					 HIGH_PRIORITY_CLASS);
}

// EndExclusiveMode()

void EndExclusiveMode(void)
{
	// set to idle priority
	SetPriorityClass(GetCurrentProcess(),
					 IDLE_PRIORITY_CLASS);
	
	// release execution state
	SetThreadExecutionState(ES_CONTINUOUS);
}

// MinimizeApplicationWindow()

void MinimizeApplicationWindow(void)
{
	// send minimize message
	SendMessage(g_hWnd,
				WM_SYSCOMMAND,
				SC_MINIMIZE,
				NULL);
}

#ifdef DEBUG_MODE
// IsValidScriptChar()

BOOL IsValidScriptChar(char c)
{
	// compare with all valid characters
	if(c>='A'&&
	   c<='Z')
		return(TRUE);
	if(c>='a'&&
	   c<='z')
		return(TRUE);
	if(c>='0'&&
	   c<='9')
		return(TRUE);
	if(c=='_')
		return(TRUE);
	if(c=='-')
		return(TRUE);
	if(c=='.')
		return(TRUE);
	if(c=='#')
		return(TRUE);

	// no good
	return(FALSE);
}
#endif

// BeginSystemOutput()

BOOL BeginSystemOutput(void)
{
	// return flag
	BOOL bFlag=FALSE;
	
	// ensure direct3d cooperation
	if(g_lpD3DDevice)
	{
		// check scene
		if(g_bInScene)
		{
			// set flag
			bFlag=TRUE;

			// end scene
			EndScene();
		}

		// check full-screen flag
		if(!g_pp.Windowed)
		{
			// force device to reset
			ResetRenderDevice();

			// minimize window
			MinimizeApplicationWindow();
		}
	}

	// return flag
	return(bFlag);
}

// EndSystemOutput()

void EndSystemOutput(BOOL bFlag)
{
	// make sure window is restored
	RestoreApplicationWindow();

	// reset direct3d as needed
	if(g_lpD3DDevice)
	{
		// begin scene if necessary
		if(bFlag)
			BeginScene();
	}
}

// UpdateWindowMetrics()

void UpdateWindowMetrics(void)
{
	// calculate new metrics
	CalculateWindowMetrics();

	// update window
	SetWindowPos(g_hWnd,
				 NULL,
				 g_nXPos,
				 g_nYPos,
				 g_nXSize,
				 g_nYSize,
				 SWP_NOZORDER);
}

// RestoreApplicationWindow()

void RestoreApplicationWindow(void)
{
	// send restore message
	SendMessage(g_hWnd,
				WM_SYSCOMMAND,
				SC_RESTORE,
				NULL);
}

// InitAcceleratorTable()

BOOL InitAcceleratorTable(void)
{
	// set alt+enter accelerator
	g_acAltEnter.fVirt=FALT|FVIRTKEY;
	g_acAltEnter.key=VK_RETURN;
	g_acAltEnter.cmd=IDM_TOGGLEFULLSCREEN;

	// create table
	g_hAccel=CreateAcceleratorTable(&g_acAltEnter,
									1);

	// check for error
	if(!g_hAccel)
	{
		DisplayErrorMessage("Unable to create accelerator table.");
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// ReleaseAcceleratorTable()

void ReleaseAcceleratorTable(void)
{
	// destroy table
	if(DestroyAcceleratorTable(g_hAccel))
	{
		// reset handle
		g_hAccel=(HACCEL)INVALID_HANDLE_VALUE;
		
		// remove resource
		RemoveWinResource;
	}
}

// ShowWindowsCursor()

void ShowWindowsCursor(void)
{
	// show cursor
	while(ShowCursor(TRUE)<0)
	{}

	// release mouse
	ReleaseCapture();
}

// HideWindowsCursor()

void HideWindowsCursor(void)
{
	// hide cursor
	while(ShowCursor(FALSE)>=0)
	{}

	// capture mouse
	SetCapture(g_hWnd);
}

// CapturePauseBitmap()

BOOL CapturePauseBitmap(D3DCOLOR clrFilter)
{
	// counters
	DWORD dwX,dwY;
	
	// check direct3d
	if(g_lpD3DDevice&&
	   g_dmApp.bWindowed)
	{
		// allocate memory for image
		if(!AllocMem((LPVOID*)&g_lpPauseBmpData,
					 sizeof(BYTE)*g_rClient.dwX*g_rClient.dwY*3))
			return(FALSE);

		// prepare bitmap header
		g_bih.biSize=sizeof(BITMAPINFOHEADER);
		g_bih.biWidth=(long)g_rClient.dwX;
		g_bih.biHeight=(long)g_rClient.dwY;
		g_bih.biPlanes=1;
		g_bih.biBitCount=24;
		g_bih.biCompression=BI_RGB;
		g_bih.biSizeImage=NULL;
		g_bih.biXPelsPerMeter=11811;
		g_bih.biYPelsPerMeter=11811;
		g_bih.biClrUsed=0;
		g_bih.biClrImportant=0;

		// get filter values
		float fFilterR=GetR888(clrFilter);
		float fFilterG=GetG888(clrFilter);
		float fFilterB=GetB888(clrFilter);
		float fFilterA=GetA888(clrFilter);
		fFilterR*=fFilterA;
		fFilterG*=fFilterA;
		fFilterB*=fFilterA;

		// lock back buffer
		D3DLOCKED_RECT lr=LockBackBuffer();
		LPWORD lpwData=(LPWORD)lr.pBits;
		LPDWORD lpdwData=(LPDWORD)lr.pBits;
		DWORD dwPitch=lr.Pitch;

		// set bits
		for(dwY=0;dwY<g_rClient.dwY;dwY++)
			for(dwX=0;dwX<g_rClient.dwX;dwX++)
			{
				// read color values
				WORD wC=lpwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>1))];
				DWORD dwC=lpdwData[dwX+(((g_rClient.dwY-1)-dwY)*(dwPitch>>2))];

				// color components
				float fR,fG,fB,fA;

				// check format
				switch(g_dmApp.fmt)
				{
				case(D3DFMT_R5G6B5):
					// exract components
					fR=GetR565(wC);
					fG=GetG565(wC);
					fB=GetB565(wC);
					break;
				case(D3DFMT_X1R5G5B5):
					// exract components
					fR=GetR555(wC);
					fG=GetG555(wC);
					fB=GetB555(wC);
					break;
				case(D3DFMT_X4R4G4B4):
					// exract components
					fR=GetR444(wC);
					fG=GetG444(wC);
					fB=GetB444(wC);
					break;
				case(D3DFMT_X8R8G8B8):
					// exract components
					fR=GetR888(dwC);
					fG=GetG888(dwC);
					fB=GetB888(dwC);
					break;
				}
				
				// compute grayscale value
				fA=(fR+fG+fB)/3;
				
				// filter components
				fR=fFilterR*fA;
				fG=fFilterG*fA;
				fB=fFilterB*fA;

				// set bitmap bits
				g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)  ]=(BYTE)(fB*255);
				g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+1]=(BYTE)(fG*255);
				g_lpPauseBmpData[((dwX+(dwY*g_rClient.dwX))*3)+2]=(BYTE)(fR*255);
			}

		// unlock back buffer
		UnlockBackBuffer();
	}	
	
	// redraw window
	if(!RedrawWindow(g_hWnd,
					 NULL,
					 NULL,
					 RDW_INTERNALPAINT))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleasePauseBitmap()

void ReleasePauseBitmap(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpPauseBmpData);
}

#ifdef DEBUG_MODE
// OpenDataCompiler()

BOOL OpenDataCompiler(LPCSTR lpFile)
{
	// check handle
	if(g_hDataFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple data files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open output file
	g_hDataFile=CreateFile(lpFile,
						   GENERIC_WRITE,
						   NULL,
						   NULL,
						   CREATE_ALWAYS,
						   FILE_ATTRIBUTE_NORMAL|
						   FILE_FLAG_SEQUENTIAL_SCAN,
						   NULL);

	// check for error
	if(!g_hDataFile)
	{
		g_hScriptFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to create data file.",
								 (LPSTR)lpFile,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// ok
	return(TRUE);
}

// AddDataFileRef()

DWORD AddDataFileRef(LPCSTR lpFile)
{
	// check counter
	if(g_dwNumDataFileRef==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpDataFileRefName,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDataFileRef=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumDataFileRef>=g_dwMaxDataFileRef)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpDataFileRefName,
					  sizeof(LPSTR)*g_dwMaxDataFileRef,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxDataFileRef+=BLOCK_ALLOC_INC;
	}

	// save index
	DWORD dwIndex=g_dwNumDataFileRef;

	// increment counter
	g_dwNumDataFileRef++;

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpDataFileRefName[dwIndex],
				 strlen(lpFile)+1))
		return(NO_OBJECT);

	// copy name
	strcpy(g_lplpDataFileRefName[dwIndex],
		   lpFile);
	
	// ok	
	return(dwIndex);
}

// CloseDataCompiler()

void CloseDataCompiler(void)
{
	// number of bytes
	DWORD dwBytesRead;
	DWORD dwBytesWritten;

	// counter
	DWORD dwCount;

	// data file header
	DATAFILEHEADER dfh;

	// data file pointer
	DWORD dwPos;

	// compute data file size
	dfh.dwSize=0;
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// add name
		dfh.dwSize+=strlen(g_lplpDataFileRefName[dwCount])+1;

		// add offset
		dfh.dwSize+=sizeof(DWORD);

		// add size of file
		dfh.dwSize+=sizeof(DWORD);

		// add file
		dfh.dwSize+=GetFileLength(g_lplpDataFileRefName[dwCount]);
	}

	// allocate data file memory
	if(!AllocMem((LPVOID*)&g_lpDataFile,
				 dfh.dwSize))
				 return;
	
	// add number of references
	dfh.dwNumRef=g_dwNumDataFileRef;
	
	// reset position
	dwPos=0;

	// add file names
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// copy name
		strcpy((LPSTR)&g_lpDataFile[dwPos],
			   g_lplpDataFileRefName[dwCount]);

		// increment pointer
		dwPos+=strlen(g_lplpDataFileRefName[dwCount])+1;
	}

	// save position of offset table
	LPDWORD lpOffsetTable=(LPDWORD)&g_lpDataFile[dwPos];
	dfh.dwOffsetPos=dwPos;

	// skip offset table
	dwPos+=sizeof(DWORD)*g_dwNumDataFileRef;

	// save position of size table
	LPDWORD lpSizeTable=(LPDWORD)&g_lpDataFile[dwPos];
	dfh.dwSizePos=dwPos;

	// skip size table
	dwPos+=sizeof(DWORD)*g_dwNumDataFileRef;

	// save position of data table
	dfh.dwDataPos=dwPos;

	// add files
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// set offset
		lpOffsetTable[dwCount]=dwPos;

		// set size
		lpSizeTable[dwCount]=GetFileLength(g_lplpDataFileRefName[dwCount]);

		// open file
		HANDLE hFile;
		hFile=CreateFile(g_lplpDataFileRefName[dwCount],
						 GENERIC_READ,
						 NULL,
						 NULL,
						 OPEN_EXISTING,
						 FILE_ATTRIBUTE_NORMAL|
						 FILE_FLAG_SEQUENTIAL_SCAN,
						 NULL);

		// check for error
		if(!hFile)
		{
			DisplayFatalErrorMessage("Unable to open reference file.",
									 g_lplpDataFileRefName[dwCount],
									 FALSE);
			return;
		}
		else
			AddWinResource;
		
		// read file
		ReadFile(hFile,
				 (LPVOID)&g_lpDataFile[dwPos],
				 lpSizeTable[dwCount],
				 &dwBytesRead,
				 NULL);

		// check for error
		if(dwBytesRead!=lpSizeTable[dwCount])
		{
			DisplayFatalErrorMessage("Unable to read data file.",
									 g_lplpDataFileRefName[dwCount]);
			return;
		}

		// increment pointer
		dwPos+=lpSizeTable[dwCount];

		// close file
		if(!CloseHandle(hFile))
		{
			DisplayFatalErrorMessage("Unable to reference file.",
									 g_lplpDataFileRefName[dwCount]);
			return;
		}
		else
			RemoveWinResource;
	}

	// output header
	WriteFile(g_hDataFile,
			  (LPVOID)&dfh,
			  sizeof(DATAFILEHEADER),
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=sizeof(DATAFILEHEADER))
	{
		DisplayFatalErrorMessage("Unable to output data file header.");
		return;
	}

	// output data
	WriteFile(g_hDataFile,
			  (LPVOID)g_lpDataFile,
			  dfh.dwSize,
			  &dwBytesWritten,
			  NULL);

	// check for error
	if(dwBytesWritten!=dfh.dwSize)
	{
		DisplayFatalErrorMessage("Unable to output data file.");
		return;
	}

	// free data file memory
	FreeMem((LPVOID*)&g_lpDataFile);
	
	// close data file
	if(!CloseHandle(g_hDataFile))
	{
		DisplayFatalErrorMessage("Unable to close data file.");
		return;
	}
	else
	{	
		// reset handle
		g_hDataFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}

	// free storage
	if(g_dwNumDataFileRef)
	{
		// release names
		for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
			FreeMem((LPVOID*)&g_lplpDataFileRefName[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lplpDataFileRefName);
		
		// reset counters
		g_dwNumDataFileRef=0;
		g_dwMaxDataFileRef=0;
	}
}
#endif

// OpenDataFile()

BOOL OpenDataFile(LPCSTR lpFile)
{
	// number of bytes
	DWORD dwBytesRead;

	// data file header
	DATAFILEHEADER dfh;

	// check handle
	if(g_hDataFile!=INVALID_HANDLE_VALUE)
	{
		DisplayFatalErrorMessage("Unable to open multiple data files.",
								 NULL,
								 FALSE);
		return(FALSE);
	}

	// open file
	g_hDataFile=CreateFile(lpFile,
						   GENERIC_READ,
						   NULL,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL|
						   FILE_FLAG_SEQUENTIAL_SCAN,
						   NULL);

	// check for error
	if(!g_hDataFile)
	{
		g_hDataFile=INVALID_HANDLE_VALUE;
		DisplayFatalErrorMessage("Unable to open data file.",
								 (LPSTR)lpFile,
								 FALSE);
		return(FALSE);
	}
	else
		AddWinResource;

	// read header
	ReadFile(g_hDataFile,
			 (LPVOID)&dfh,
			 sizeof(DATAFILEHEADER),
			 &dwBytesRead,
			 NULL);

	// check for error
	if(dwBytesRead!=sizeof(DATAFILEHEADER))
	{
		DisplayFatalErrorMessage("Unable to read data file header.",
								 (LPSTR)lpFile);
		return(FALSE);
	}

	// allocate data file memory
	if(!AllocMem((LPVOID*)&g_lpDataFile,
				 dfh.dwSize))
		return(FALSE);

	// read data
	ReadFile(g_hDataFile,
			 (LPVOID)g_lpDataFile,
			 dfh.dwSize,
			 &dwBytesRead,
			 NULL);

	// check for error
	if(dwBytesRead!=dfh.dwSize)
	{
		DisplayFatalErrorMessage("Unable to read data file.",
								 (LPSTR)lpFile);
		return(FALSE);
	}

	// close data file
	if(!CloseHandle(g_hDataFile))
	{
		DisplayFatalErrorMessage("Unable to close data file.");
		return(FALSE);
	}
	else
	{	
		// reset handle
		g_hDataFile=INVALID_HANDLE_VALUE;

		// remove resource
		RemoveWinResource;
	}

	// get position values
	g_dwNumDataFileRef=dfh.dwNumRef;
	g_dwDataFileOffsetPos=dfh.dwOffsetPos;
	g_dwDataFileSizePos=dfh.dwSizePos;
	g_dwDataFileDataPos=dfh.dwDataPos;

#ifdef DEBUG_MODE
	// counter
	DWORD dwCount;

	// allocate memory for used flags
	if(!AllocMem((LPVOID*)&g_lpbDataFileRefUsed,
				 sizeof(BOOL)*g_dwNumDataFileRef))
		return(FALSE);

	// reset used flags
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
		g_lpbDataFileRefUsed[dwCount]=FALSE;
#endif

	// ok
	return(TRUE);
}

// FindDataFileRef()

DATAFILEREF FindDataFileRef(LPSTR lpFile)
{
	// return value
	DATAFILEREF dfr;
	InitStruct(dfr);
	
	// counter
	DWORD dwCount;

	// check data file
	if(!g_lpDataFile)
		return(dfr);

	// reset position holder
	DWORD dwPos=0;
	
	// reset found index
	DWORD dwIndex=NO_OBJECT;
		
	// search file names
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
	{
		// compare names
		if(stricmp((LPSTR)&g_lpDataFile[dwPos],
				   lpFile)==0)
		{
			// check for duplicates
			if(dwIndex!=NO_OBJECT)
				DisplayErrorMessage("Duplicate data file references found.",
								   lpFile,
								   FALSE);

			// save index
			dwIndex=dwCount;
			
#ifdef DEBUG_MODE
			// set used flag
			g_lpbDataFileRefUsed[dwCount]=TRUE;
#endif
		}

		// increment pointer
		dwPos+=strlen((LPSTR)&g_lpDataFile[dwPos])+1;
	}

	// check index
	if(dwIndex==NO_OBJECT)
	{
		DisplayErrorMessage("Data file reference not found.",
						   lpFile,
						   FALSE);
		return(dfr);
	}

	// get pointer to offset table
	LPDWORD lpdwOffset=(LPDWORD)&g_lpDataFile[g_dwDataFileOffsetPos];

	// set data pointer
	dfr.lpData=(LPVOID)&g_lpDataFile[lpdwOffset[dwIndex]];

	// get pointer to size table
	LPDWORD lpdwSize=(LPDWORD)&g_lpDataFile[g_dwDataFileSizePos];

	// set size
	dfr.dwSize=lpdwSize[dwIndex];

	// return structure
	return(dfr);
}

// CloseDataFile()

void CloseDataFile(void)
{
#ifdef DEBUG_MODE
	// counter
	DWORD dwCount;

	// check used flags
	for(dwCount=0;dwCount<g_dwNumDataFileRef;dwCount++)
		if(!g_lpbDataFileRefUsed[dwCount])
		{
			// create message
			sprintf(g_lpMsg,
					"Data file reference #%d not used.",
					dwCount);

			// display message
			DisplayInfoMessage(g_lpMsg);
		}

	// free used flag memory
	FreeMem((LPVOID*)&g_lpbDataFileRefUsed);
#endif

	// free data file memory
	FreeMem((LPVOID*)&g_lpDataFile);

	// reset number of references
	g_dwNumDataFileRef=0;
}

// GetFileLength()

DWORD GetFileLength(LPSTR lpFile)
{
	// file attributes
	WIN32_FILE_ATTRIBUTE_DATA fad;
	
	// get file attributes
	if(!GetFileAttributesEx(lpFile,
							GetFileExInfoStandard,
							(LPVOID)&fad))
	{
		DisplayFatalErrorMessage("Unable to read file attributes.",
								 lpFile);
		return(0);
	}

	// check high size
	if(fad.nFileSizeHigh)
	{
		DisplayFatalErrorMessage("File is too large to read length.",
								 lpFile);
		return(0);
	}

	// return low size
	return(fad.nFileSizeLow);
}

// GetCurrentDialogDropDownSelection()

BOOL GetCurrentDialogDropDownSelection(DWORD dwID,
									   LPSTR lpText)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// get index
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_GETCURSEL,
									 NULL,
									 NULL);

	// read string
	if(dwIndex!=CB_ERR)
		SendMessage(hList,
					CB_GETLBTEXT,
					(WPARAM)dwIndex,
					(LPARAM)lpText);
	else
		return(FALSE);

	// ok
	return(TRUE);
}

// ResetCurrentDialogDropDownSelections()

BOOL ResetCurrentDialogDropDownSelections(DWORD dwID)
{
	// get list box handle
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// reset list box
	SendMessage(hList,
				CB_RESETCONTENT,
				NULL,
				NULL);

	// ok
	return(TRUE);
}

// AddCurrentDialogDropDownSelection()

BOOL AddCurrentDialogDropDownSelection(DWORD dwID,
									   LPSTR lpText)
{
	// get list box handle
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// add string
	if(SendMessage(hList,
				   CB_ADDSTRING,
				   NULL,
				   (LPARAM)lpText)==CB_ERR)
		return(FALSE);

	// ok
	return(TRUE);
}

// RemoveCurrentDialogDropDownSelection()

BOOL RemoveCurrentDialogDropDownSelection(DWORD dwID)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);
	
	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// get index
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_GETCURSEL,
									 NULL,
									 NULL);

	// remove item
	if(dwIndex!=CB_ERR)
	{
		// send message
		if(SendMessage(hList,
					   CB_DELETESTRING,
					   (WPARAM)dwIndex,
					   NULL)==CB_ERR)
			return(FALSE);
	}
	else
		return(FALSE);

	// ok
	return(TRUE);
}

// HighlightCurrentDialogDropDownSelection()

BOOL HighlightCurrentDialogDropDownSelection(DWORD dwID,
											 LPSTR lpText)
{
	// get handle of list box
	HWND hList=GetDlgItem(g_hCurrDlg,
						  dwID);

	// check handle
	if(!hList)
	{
		DisplayErrorMessage("Unable to read list box selection.");
		return(FALSE);
	}
	
	// find index of selection
	DWORD dwIndex=(DWORD)SendMessage(hList,
									 CB_FINDSTRING,
									 (WPARAM)-1,
									 (LPARAM)lpText);

	// check index
	if(dwIndex==CB_ERR)
		return(FALSE);

	// highlight selection
	if(SendMessage(hList,
				   CB_SETCURSEL,
				   (WPARAM)dwIndex,
				   NULL)==-1)
		return(FALSE);

	// ok
	return(TRUE);
}

// SetCurrentDialogEditLong()

BOOL SetCurrentDialogEditLong(WORD wID,
							  long lVal)
{
	// convert value to string
	sprintf(g_lpMsg,
			"%d",
			lVal);

	// set text
	return(SetCurrentDialogEditText(wID,
								    g_lpMsg));
}

// GetCurrentDialogEditLong()

long GetCurrentDialogEditLong(WORD wID)
{
	// get text
	GetCurrentDialogEditText(wID,
							 g_lpMsg,
							 _MAX_PATH);

	// return value
	return(atol(g_lpMsg));
}

// PurgeSystemObjects()

void PurgeSystemObjects(void)
{
	// purge objects
	PurgeTimers();
	PurgeDialogs();
}

// ForceImmediateExit()

void ForceImmediateExit(void)
{
	// release all allocated resources
	ReleaseAllModules();

	// release duplication lock
	UnlockApplication();

	// confirm removal of all resources
	ConfirmResourcesReleased();

	// end execution
	ExitProcess(NULL);
}

// InitializeCOM()

BOOL InitializeCOM(void)
{
	// init com
	HRESULT hr=CoInitialize(NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayErrorMessage("Unable to initialize component object model library.",
							NULL,
							FALSE);
		return(FALSE);
	}
	else
		AddCOMResource;

	// set flag
	g_bCOMReady=TRUE;

	// ok
	return(TRUE);
}

// ReleaseCOM()

void ReleaseCOM(void)
{
	// uninit com
	CoUninitialize();
	RemoveCOMResource;

	// set flag
	g_bCOMReady=FALSE;
}

// CapitalizeString()

void CapitalizeString(LPSTR lpStr)
{
	// reset current position
	DWORD dwPos=0;

	// scan string until finished
	while(lpStr[dwPos]!=0)
	{
		// get current character
		char cChar=lpStr[dwPos];

		// check character
		if(cChar>='a'&&
		   cChar<='z')
		{
			// check position
			if(dwPos==0)
			{
				// capitalize character
				lpStr[dwPos]=(cChar-'a')+'A';
			}
			else
			{
				// get previous character
				char cPrev=lpStr[dwPos-1];

				// check previous character
				if(!((cPrev>='A'&&
					  cPrev<='Z')||
					 (cPrev>='a'&&
					  cPrev<='z')))
				{
					// capitalize character
					lpStr[dwPos]=(cChar-'a')+'A';
				}
			}
		}
		
		// increment position
		dwPos++;
	}
}

// BinaryToDWORD()

DWORD BinaryToDWORD(LPSTR lpStr)
{
	// counter
	DWORD dwCount;
	
	// reset return value
	DWORD dwRet=0;

	// get length of string
	DWORD dwLen=(DWORD)(strlen(lpStr));

	// reset multiplier
	DWORD dwMul=1<<(dwLen-1);

	// compute value
	for(dwCount=0;dwCount<dwLen;dwCount++)
	{
		// check byte
		if(lpStr[dwCount]=='1')
		{
			// add multiplier to value
			dwRet|=dwMul;

			// adjust multiplier
			dwMul=dwMul>>1;
		}
		else if(lpStr[dwCount]=='0')
		{
			// adjust multiplier
			dwMul=dwMul>>1;
		}
	}

	// display value
	sprintf(g_lpMsg,"%X",dwRet);
	DisplayInfoMessage(g_lpMsg);

	// return value
	return(dwRet);
}

// eof //




