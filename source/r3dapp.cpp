
// r3dapp.cpp -- application procedures

// includes //

#include"..\\engine\\r3dwin.h"
#include"stkston.h"

// globals //

// full-screen flag
BOOL g_bFullscreen=FALSE;

// functions //

// ApplicationName()

LPCSTR ApplicationName(void)
{
	// return name
	return("R3D Engine v1.0");
}

// ApplicationID()

LPCSTR ApplicationID(void)
{
	// return id
	return("75214738-90C7-41E9-916E-A014271B4B01");
}

// ApplicationIcon()

LPCSTR ApplicationIcon(void)
{
	// return icon resource
	return(NULL);
}

// ApplicationMenu()

LPCSTR ApplicationMenu(void)
{
	// return menu resource
	return(NULL);
}

// ApplicationCursor()

LPCSTR ApplicationCursor(void)
{
	// return cursor resource
	return(NULL);
}

// ApplicationDisplayMode()

DISPLAYMODE ApplicationDisplayMode(void)
{
	// prepare display mode structure
	DISPLAYMODE dm;
	dm.dwX=640;
	dm.dwY=480;
	dm.fmt=(D3DFORMAT)NULL;
	dm.bWindowed=!g_bFullscreen;
	dm.bLockable=TRUE;
	dm.dwBackBufferCount=1;
	
	// return structure
	return(dm);
}

// ApplicationDataPath()

LPCSTR ApplicationDataPath(void)
{
	// return path
	return("data");
}

// ApplicationMasterVolume()

float ApplicationMasterVolume(void)
{
	// return value
	return(1);
}

// ApplicationInit()

BOOL ApplicationInit(void)
{
	// add font
	AddFont("Courier New",16);

	// init game
	if(!InitSticksAndStones())
		return(FALSE);

	// ok
	return(TRUE);
}

// ApplicationLoop()

BOOL ApplicationLoop(void)
{
	// counter
	DWORD dwCount;
	
	// account for frameskip
	for(dwCount=0;dwCount<g_dwFrameSkip;dwCount++)
	{
		// update game
		if(!UpdateSticksAndStones())
			return(FALSE);
	}

	// render game
	RenderSticksAndStones();

	// display fps info
	char lpFPS[12];
	if(BeginText(0))
	{
		sprintf(lpFPS,"%d",g_dwFPS);
		OutputText(lpFPS,0,0);
		sprintf(lpFPS,"%d",g_dwUPS);
		OutputText(lpFPS,0,16);
		EndText();
	}

	// ok
	return(TRUE);
}

// ApplicationPause()

D3DCOLOR ApplicationPause(void)
{
	// return filter color for pause bitmap
	return(D3DCOLOR_COLORVALUE(0.25F,0.75F,1,1));
}

// ApplicationResume()

void ApplicationResume(void)
{
}

// ApplicationRelease()

void ApplicationRelease(void)
{
	// release game
	ReleaseSticksAndStones();
}

// ApplicationToggleFullScreen()

BOOL ApplicationToggleFullScreen(void)
{
	// toggle value
	g_bFullscreen=!g_bFullscreen;

	// allow update
	return(TRUE);
}

// eof //
