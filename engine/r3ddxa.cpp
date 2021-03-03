
// r3ddxa.cpp -- DirectX Audio wrapper

// includes //

#include"r3dwin.h"

// initialize globals //

// audio initialized flag
BOOL g_bAudioInit=FALSE;
// performance interface
IDirectMusicPerformance8* g_lpPerformance=NULL;
// audio loader interface
IDirectMusicLoader8* g_lpAudioLoader=NULL;
// internal interfaces
IDirectMusic8* g_lpDM=NULL;
IDirectSound8* g_lpDS=NULL;
// default audiopath interface
IDirectMusicAudioPath8* g_lpDefAudioPath=NULL;
// default audiopath buffer
IDirectSoundBuffer8* g_lpDefAudioBuf=NULL;
// audio segment counters
DWORD g_dwNumAudioSeg=0;
DWORD g_dwMaxAudioSeg=0;
// audio segment data
LPBYTE* g_lplpAudioData=NULL;
IDirectMusicSegment8** g_lplpAudioSeg=NULL;
// audio master volume
float g_fAudioMasterVol=0;
// audio path counters
DWORD g_dwNumAudioPath=0;
DWORD g_dwMaxAudioPath=0;
// audio path data
IDirectMusicAudioPath8** g_lplpAudioPath=NULL;
IDirectSound3DBuffer8** g_lplpAudioBuf3D=NULL;
IDirectSoundBuffer8** g_lplpAudioBuf=NULL;
LPBOOL g_lpbAudioPath3DPrepared=NULL;
LPBOOL g_lpbAudioPath3DEnabled=NULL;
// audio listener
IDirectSound3DListener8* g_lpAudioListener=NULL;
// primary audio buffer
IDirectSoundBuffer* g_lpPrimaryBuf=NULL;
// audio master tempo
float g_fAudioMasterTempo=0;
// primary segment data
IDirectMusicSegment8* g_lpPrimarySeg=NULL;
MUSIC_TIME g_mtCurPriSegPos;
// audio distance factor
float g_fAudioDistFactor=0;
// 3d audio enabled flag
BOOL g_b3DAudioEnabled=FALSE;

// functions //

// DXAudioInit()

BOOL DXAudioInit(void)
{
	// reset initialized flag
	g_bAudioInit=FALSE;

	// init com
	if(!InitializeCOM())
		return(FALSE);

	// set initialized flag
	BOOL bInit=TRUE;

	// init performance
	if(bInit)
		if(!InitPerformance())
			bInit=FALSE;

	// init audio loader
	if(bInit)
		if(!InitAudioLoader())
			bInit=FALSE;

	// init default audiopath
	if(bInit)
		if(!InitDefAudioPath())
			bInit=FALSE;

	// init primary buffer
	if(bInit)
		if(!InitPrimaryBuffer())
			bInit=FALSE;

	// init audio listener
	if(bInit)
		if(!InitAudioListener())
			bInit=FALSE;

	// copy flag
	g_bAudioInit=bInit;

	// check flag
	if(!g_bAudioInit)
	{
		// check registry for flag
		LPSTR lpFlag=GetRegistryKey("DXNoSndMsg");
		if(!lpFlag)
		{
			// display message box
			DisplayInfoMessage("Required audio support has not been detected.\n\n"
							   "Application will run without audio.");

			// set flag
			SetRegistryKey("DXNoSndMsg",
						   "TRUE");
		}
		else
		{
			// release flag
			FreeRegistryKey(lpFlag);
		}
	}
	else
	{
		// delete existing registry flag
		DeleteRegistryKey("DXNoSndMsg");
	}
	
	// begin exclusive mode
	BeginAudioExclusiveMode();

	// reset master volume
	SetAudioMasterVolume(1);

	// reset master tempo
	SetAudioMasterTempo(1);

	// set default distance factor
	SetListenerDistanceFactor();

	// disable 3d audio
	Disable3DAudio();
	
	// ok
	return(TRUE);
}

// DXAudioRelease()

void DXAudioRelease(void)
{
	// purge objects
	PurgeAudioObjects();

	// end exclusive mode
	EndAudioExclusiveMode();

	// reset initialized flag
	g_bAudioInit=FALSE;

	// release interfaces
	ReleaseAudioListener();
	ReleasePrimaryBuffer();
	ReleaseDefAudioPath();
	ReleaseAudioLoader();
	ReleasePerformance();

	// release com
	ReleaseCOM();
}

// InitPerformance()

BOOL InitPerformance(void)
{
	// create performance
	HRESULT hr=CoCreateInstance(CLSID_DirectMusicPerformance,
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance8,
								(LPVOID*)&g_lpPerformance);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// internal pointers
	IDirectMusic* lpDM=NULL;
	IDirectSound* lpDS=NULL;

	// init performance
	hr=g_lpPerformance->InitAudio(&lpDM,
								  &lpDS,
								  g_hWnd,
								  DMUS_APATH_SHARED_STEREOPLUSREVERB,
								  64,
								  DMUS_AUDIOF_ALL,
								  NULL);

	// check for error
	if(FAILED(hr))
		return(FALSE);

	// update music interface
	hr=lpDM->QueryInterface(IID_IDirectMusic8,
							(LPVOID*)&g_lpDM);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// update sound interface
	hr=lpDS->QueryInterface(IID_IDirectSound8,
							(LPVOID*)&g_lpDS);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// ok
	return(TRUE);
}

// InitAudioLoader()

BOOL InitAudioLoader(LPCSTR lpPath)
{
	// create loader
	HRESULT hr=CoCreateInstance(CLSID_DirectMusicLoader,
								NULL,
								CLSCTX_ALL,
								IID_IDirectMusicLoader8,
								(LPVOID*)&g_lpAudioLoader);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;
	
	// set audio search path
	if(!SetAudioSearchPath(lpPath))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleasePerformance()

void ReleasePerformance(void)
{
	// close down performance
	if(g_lpPerformance)
		g_lpPerformance->CloseDown();
	
	// release internal pointers
	ReleaseCOMPtr(g_lpDM);
	ReleaseCOMPtr(g_lpDS);

	// release performance
	ReleaseCOMPtr(g_lpPerformance);
}

// ReleaseAudioLoader()

void ReleaseAudioLoader(void)
{
	// release pointer
	ReleaseCOMPtr(g_lpAudioLoader);
}

// SetAudioSearchPath()

BOOL SetAudioSearchPath(LPCSTR lpPath)
{
	// check interface
	if(!g_lpAudioLoader)
		return(FALSE);

	// check pointer
	if(lpPath)
	{
		// convert to wide
		MakeWideString(g_lpWMsg,
					   lpPath);
	}
	else
	{
		// get current directory
		if(!getcwd(g_lpMsg,_MAX_PATH-1))
		{
			DisplayErrorMessage("Unable to read current working directory.");
			return(FALSE);
		}

		// convert to wide
		MakeWideString(g_lpWMsg,
					   g_lpMsg);
	}

	// set path
	HRESULT hr=g_lpAudioLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,
												   g_lpWMsg,
												   TRUE);

	// check for error
	if(FAILED(hr))
		return(FALSE);
												  
	// ok
	return(TRUE);
}

// StopAllAudioSegments()

void StopAllAudioSegments(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop all output
		g_lpPerformance->Stop(NULL,
							  NULL,
							  0,
							  0);
	}
}

// ClearAudioCache()

void ClearAudioCache(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check interface
		if(g_lpAudioLoader)
		{
			// clear cache
			g_lpAudioLoader->ClearCache(GUID_DirectMusicAllTypes);

			// collect garbage
			g_lpAudioLoader->CollectGarbage();
		}
	}
}

// GetNextAudioSegment()

DWORD GetNextAudioSegment(void)
{
	// check counter
	if(g_dwNumAudioSeg==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpAudioData,
					 sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioSeg,
					 sizeof(IDirectMusicSegment8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioSeg=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAudioSeg>=g_dwMaxAudioSeg)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpAudioData,
					  sizeof(LPBYTE)*g_dwMaxAudioSeg,
					  sizeof(LPBYTE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioSeg,
					  sizeof(IDirectMusicSegment8*)*g_dwMaxAudioSeg,
					  sizeof(IDirectMusicSegment8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioSeg+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAudioSeg;

	// increment counter
	g_dwNumAudioSeg++;

	// return index
	return(dwIndex);
}

// AddAudioSegment()

DWORD AddAudioSegment(LPCSTR lpFile)
{
	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioSegment();

	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// check flag
	if(g_bAudioInit)
	{
		// load segment
		if(dfr.lpData)
		{
			// allocate memory for audio data
			if(!AllocMem((LPVOID*)&g_lplpAudioData[dwIndex],
						 dfr.dwSize))
				return(NO_OBJECT);

			// copy audio data
			CopyMemory((LPVOID)g_lplpAudioData[dwIndex],
					   dfr.lpData,
					   dfr.dwSize);

			// prepare object description
			DMUS_OBJECTDESC dmod;
			InitStruct(dmod);
			dmod.dwSize=sizeof(DMUS_OBJECTDESC);
			dmod.dwValidData=DMUS_OBJ_CLASS|
							 DMUS_OBJ_MEMORY;
			dmod.guidClass=CLSID_DirectMusicSegment;
			dmod.pbMemData=g_lplpAudioData[dwIndex];
			dmod.llMemLength=(LONGLONG)dfr.dwSize;

			// get object
			hr=g_lpAudioLoader->GetObject(&dmod,
										  IID_IDirectMusicSegment8,
										  (LPVOID*)&g_lplpAudioSeg[dwIndex]);
		}
		else
		{
			// reset audio data pointer
			g_lplpAudioData[dwIndex]=NULL;

			// convert filename to wide string
			MakeWideString(g_lpWMsg,
						   lpFile);
			
			// load segment
			hr=g_lpAudioLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
												   IID_IDirectMusicSegment8,
												   g_lpWMsg,
												   (LPVOID*)&g_lplpAudioSeg[dwIndex]);
		}

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to load audio segment.",
									   (LPSTR)lpFile,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// download any necessary resources
		hr=g_lplpAudioSeg[dwIndex]->Download(g_lpPerformance);
		
		// check for error
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to download audio resources.",
								  (LPSTR)lpFile,
								  hr);
	}

	// return index
	return(dwIndex);
}

// PlayAudioSegment()

void PlayAudioSegment(DWORD dwSegIndex,
					  DWORD dwPathIndex,
					  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSegIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}

	// verify index
	if(dwPathIndex>=g_dwNumAudioSeg&&
	   dwPathIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set flags
		DWORD dwFlags=DMUS_SEGF_SECONDARY|
					  dwAlign;

		// set audiopath pointer
		IDirectMusicAudioPath8* lpPath;
		if(dwPathIndex==NO_OBJECT)
			lpPath=NULL;
		else
			lpPath=g_lplpAudioPath[dwPathIndex];
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwSegIndex],
									   NULL,
									   NULL,
									   dwFlags,
									   0,
									   NULL,
									   NULL,
									   lpPath);
	}
}

// StopAudioSegment()

void StopAudioSegment(DWORD dwIndex,
					  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// stop segment
		g_lpPerformance->StopEx(g_lplpAudioSeg[dwIndex],
								0,
								dwAlign);
	}
}

// PurgeAudioSegments()

void PurgeAudioSegments(void)
{
	// counter
	DWORD dwCount;
	
	// check counter
	if(g_dwNumAudioSeg)
	{
		// stop all output
		StopAllAudioSegments();

		// check flag
		if(g_bAudioInit)
		{
			// release all internal objects
			for(dwCount=0;dwCount<g_dwNumAudioSeg;dwCount++)
			{
				// unload instrument data
				g_lplpAudioSeg[dwCount]->Unload(g_lpPerformance);
				
				// free memory
				FreeMem((LPVOID*)&g_lplpAudioData[dwCount]);
				
				// release interface
				ReleaseCOMPtr(g_lplpAudioSeg[dwCount]);
			}
		}

		// free memory objects
		FreeMem((LPVOID*)&g_lplpAudioData);
		FreeMem((LPVOID*)&g_lplpAudioSeg);
		
		// clear audio cache
		ClearAudioCache();

		// reset counters
		g_dwNumAudioSeg=0;
		g_dwMaxAudioSeg=0;
	}
}

// PlayPrimaryAudioSegment()

void PlayPrimaryAudioSegment(DWORD dwIndex,
							 DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "PlayPrimaryAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set start point
		g_lplpAudioSeg[dwIndex]->SetStartPoint(0);
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwIndex],
									   NULL,
									   NULL,
									   dwAlign,
									   0,
									   NULL,
									   NULL,
									   NULL);
	}
}

// QueuePrimaryAudioSegment()

void QueuePrimaryAudioSegment(DWORD dwIndex,
							  DWORD dwAlign)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "QueuePrimaryAudioSegment()",
								 FALSE);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set flags
		DWORD dwFlags=DMUS_SEGF_QUEUE|
					  dwAlign;
		
		// set start point
		g_lplpAudioSeg[dwIndex]->SetStartPoint(0);
		
		// play segment
		g_lpPerformance->PlaySegmentEx(g_lplpAudioSeg[dwIndex],
									   NULL,
									   NULL,
									   dwFlags,
									   0,
									   NULL,
									   NULL,
									   NULL);
	}
}

// PauseAudioOutput()

void PauseAudioOutput(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// capture primary segment
		CapturePrimaryAudioSegment();
		
		// stop all segments
		StopAllAudioSegments();
	}
}

// ResumeAudioOutput()

void ResumeAudioOutput(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// active any lost ports
		g_lpDM->Activate(TRUE);

		// activate any lost audiopaths
		ActivateAllAudioPaths();
		
		// restore primary segment
		RestorePrimaryAudioSegment();
	}
}

// IsAudioSegmentPlaying()

BOOL IsAudioSegmentPlaying(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "IsAudioSegmentPlaying()",
								 FALSE);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check segment
		if(g_lpPerformance->IsPlaying(g_lplpAudioSeg[dwIndex],
									  NULL)==S_OK)
			return(TRUE);
	}

	// not playing
	return(FALSE);
}

// SetAudioSegmentLength()

BOOL SetAudioSegmentLength(DWORD dwIndex,
						   MUSIC_TIME mtTime)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "SetAudioSegmentLength()",
								 FALSE);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set length
		g_lplpAudioSeg[dwIndex]->SetLength(mtTime);
	}

	// ok
	return(TRUE);
}

// SetAudioSegmentRepeats()

BOOL SetAudioSegmentRepeats(DWORD dwIndex,
							DWORD dwRepeats)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioSeg)
	{
		DisplayFatalErrorMessage("Invalid audio segment index.",
								 "SetAudioSegmentRepeats()",
								 FALSE);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set repeats
		g_lplpAudioSeg[dwIndex]->SetRepeats(dwRepeats);
	}

	// ok
	return(TRUE);
}

// InitDefAudioPath()

BOOL InitDefAudioPath(void)
{
	// check interface
	if(!g_lpPerformance)
		return(FALSE);

	// get audiopath
	HRESULT hr=g_lpPerformance->GetDefaultAudioPath(&g_lpDefAudioPath);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// get default audiopath buffer
	hr=g_lpDefAudioPath->GetObjectInPath(DMUS_PCHANNEL_ALL,
										 DMUS_PATH_BUFFER,
										 0,
										 GUID_NULL,
										 0,
										 IID_IDirectSoundBuffer8,
										 (LPVOID*)&g_lpDefAudioBuf);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// ok
	return(TRUE);
}

// ReleaseDefAudioPath()

void ReleaseDefAudioPath(void)
{
	// release pointers
	ReleaseCOMPtr(g_lpDefAudioPath);
	ReleaseCOMPtr(g_lpDefAudioBuf);
}

// SetAudioMasterVolume()

void SetAudioMasterVolume(float fVol,
						  DWORD dwLength,
						  BOOL bPaths)
{
	// save master volume
	g_fAudioMasterVol=fVol;

	// set master volume
	SetAudioMasterVolumeA(fVol,
						  dwLength,
						  bPaths);
}

// ResetAudioMasterVolume()

void ResetAudioMasterVolume(void)
{
	// set volume using stored value
	SetAudioMasterVolumeA(g_fAudioMasterVol);
}

// SetAudioMasterTempo()

void SetAudioMasterTempo(float fTempo)
{
	// save value
	g_fAudioMasterTempo=fTempo;

	// set tempo
	SetAudioMasterTempoA(fTempo);
}	

// ResetAudioMasterTempo()

void ResetAudioMasterTempo(void)
{
	// set tempo using stored value
	SetAudioMasterTempoA(g_fAudioMasterTempo);
}

// InvalidateAudioData()

void InvalidateAudioData(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// invalidate performance
		g_lpPerformance->Invalidate(0,
									NULL);
	}
}

// BeginAudioExclusiveMode()

void BeginAudioExclusiveMode(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// set cooperative level
		g_lpDS->SetCooperativeLevel(g_hWnd,
									DSSCL_PRIORITY);
		
		// start primary buffer
		StartPrimaryBuffer();
	}
}

// EndAudioExclusiveMode()

void EndAudioExclusiveMode(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop primary buffer
		StopPrimaryBuffer();
		
		// set cooperative level
		g_lpDS->SetCooperativeLevel(g_hWnd,
									DSSCL_NORMAL);
	}
}

// GetNextAudioPath()

DWORD GetNextAudioPath(void)
{
	// check counter
	if(g_dwNumAudioPath==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpAudioPath,
					 sizeof(IDirectMusicAudioPath8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioBuf3D,
					 sizeof(IDirectSound3DBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpAudioBuf,
					 sizeof(IDirectSoundBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAudioPath3DPrepared,
					 sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbAudioPath3DEnabled,
					 sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioPath=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumAudioPath>=g_dwMaxAudioPath)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpAudioPath,
					  sizeof(IDirectMusicAudioPath8*)*g_dwMaxAudioPath,
					  sizeof(IDirectMusicAudioPath8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioBuf3D,
					  sizeof(IDirectSound3DBuffer8*)*g_dwMaxAudioPath,
					  sizeof(IDirectSound3DBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpAudioBuf,
					  sizeof(IDirectSoundBuffer8*)*g_dwMaxAudioPath,
					  sizeof(IDirectSoundBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAudioPath3DPrepared,
					  sizeof(LPBOOL)*g_dwMaxAudioPath,
					  sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbAudioPath3DEnabled,
					  sizeof(LPBOOL)*g_dwMaxAudioPath,
					  sizeof(LPBOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxAudioPath+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumAudioPath;

	// increment counter
	g_dwNumAudioPath++;

	// return index
	return(dwIndex);
}

// AddAudioPath3D()

DWORD AddAudioPath3D(float fMinDist,
					 float fMaxDist)
{
	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioPath();

	// check flag
	if(g_bAudioInit)
	{
		// create audiopath
		hr=g_lpPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_3D,
													64,
													TRUE,
													&g_lplpAudioPath[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to create 3-D audio path.",
									   NULL,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// get 3d buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSound3DBuffer8,
													 (LPVOID*)&g_lplpAudioBuf3D[dwIndex]);


		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to retrieve 3-D audio buffer.",
									   NULL,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// get buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSoundBuffer8,
													 (LPVOID*)&g_lplpAudioBuf[dwIndex]);


		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to retrieve standard audio buffer.",
									   NULL,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// set flags
		g_lpbAudioPath3DPrepared[dwIndex]=TRUE;
		g_lpbAudioPath3DEnabled [dwIndex]=TRUE;
		
		// set min & max distances
		SetAudioPathMinDistance(dwIndex,fMinDist);
		SetAudioPathMaxDistance(dwIndex,fMaxDist);

		// enable 3d effects as needed
		if(g_b3DAudioEnabled)
			EnableAudioPath3DA(dwIndex);
		else
			DisableAudioPath3DA(dwIndex);
	}

	// return index
	return(dwIndex);
}

// AddAudioPathStereo()

DWORD AddAudioPathStereo(void)
{
	// used for error checking
	HRESULT hr;

	// get return index
	DWORD dwIndex=GetNextAudioPath();

	// check flag
	if(g_bAudioInit)
	{
		// create audiopath
		hr=g_lpPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_STEREO,
													64,
													TRUE,
													&g_lplpAudioPath[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to create stereo audio path.",
									   NULL,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// reset 3d buffer interface
		g_lplpAudioBuf3D[dwIndex]=NULL;

		// get buffer interface
		hr=g_lplpAudioPath[dwIndex]->GetObjectInPath(DMUS_PCHANNEL_ALL,
													 DMUS_PATH_BUFFER,
													 0,
													 GUID_NULL,
													 0,
													 IID_IDirectSoundBuffer8,
													 (LPVOID*)&g_lplpAudioBuf[dwIndex]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to retrieve standard audio buffer.",
									   NULL,
									   hr);
			return(NO_OBJECT);
		}
		else
			AddCOMResource;

		// set flag
		g_lpbAudioPath3DPrepared[dwIndex]=FALSE;
	}

	// return index
	return(dwIndex);
}

// PurgeAudioPaths()

void PurgeAudioPaths(void)
{
	// counter
	DWORD dwCount;
	
	// check counter
	if(g_dwNumAudioPath)
	{
		// check flag
		if(g_bAudioInit)
		{
			// release each audiopath
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
			{
				// inactivate audiopath
				InactivateAudioPath(dwCount);
				
				// release interfaces
				ReleaseCOMPtr(g_lplpAudioPath[dwCount]);
				ReleaseCOMPtr(g_lplpAudioBuf3D[dwCount]);
				ReleaseCOMPtr(g_lplpAudioBuf[dwCount]);
			}
		}

		// free memory objects
		FreeMem((LPVOID*)&g_lplpAudioPath);
		FreeMem((LPVOID*)&g_lplpAudioBuf3D);
		FreeMem((LPVOID*)&g_lplpAudioBuf);
		FreeMem((LPVOID*)&g_lpbAudioPath3DPrepared);
		FreeMem((LPVOID*)&g_lpbAudioPath3DEnabled);
		
		// reset counters
		g_dwNumAudioPath=0;
		g_dwMaxAudioPath=0;
	}
}

// InitPrimaryBuffer()

BOOL InitPrimaryBuffer(void)
{
	// check interface
	if(!g_lpDefAudioPath)
		return(FALSE);
	
	// used for error checking
	HRESULT hr;

	// get primary buffer
	hr=g_lpDefAudioPath->GetObjectInPath(0,
										 DMUS_PATH_PRIMARY_BUFFER,
										 0,
										 GUID_NULL,
										 0,
										 IID_IDirectSoundBuffer,
										 (LPVOID*)&g_lpPrimaryBuf);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// ok
	return(TRUE);
}

// ReleasePrimaryBuffer()

void ReleasePrimaryBuffer(void)
{
	// release interface
	ReleaseCOMPtr(g_lpPrimaryBuf);
}

// StartPrimaryBuffer()

void StartPrimaryBuffer(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// play primary buffer
		g_lpPrimaryBuf->Play(NULL,
							 NULL,
							 DSBPLAY_LOOPING);
	}
}

// StopPrimaryBuffer()

void StopPrimaryBuffer(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// stop primary buffer
		g_lpPrimaryBuf->Stop();
	}
}

// InitAudioListener()

BOOL InitAudioListener(void)
{
	// check pointer
	if(!g_lpDefAudioPath)
		return(FALSE);
	
	// used for error checking
	HRESULT hr;

	// get listener interface
	hr=g_lpDefAudioPath->GetObjectInPath(0,
										 DMUS_PATH_PRIMARY_BUFFER,
										 0,
										 GUID_NULL,
										 0,
										 IID_IDirectSound3DListener8,
										 (LPVOID*)&g_lpAudioListener);

	// check for error
	if(FAILED(hr))
		return(FALSE);
	else
		AddCOMResource;

	// ok
	return(TRUE);
}

// ReleaseAudioListener()

void ReleaseAudioListener(void)
{
	// release interface
	ReleaseCOMPtr(g_lpAudioListener);
}

// RenderAudioChanges()

void RenderAudioChanges(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// commit settings
			g_lpAudioListener->CommitDeferredSettings();
		}
	}
}

// PurgeAllAudioObjects()

void PurgeAudioObjects(void)
{
	// purge all objects
	PurgeAudioSegments();
	PurgeAudioPaths();
}

// SetAudioMasterVolumeA()

void SetAudioMasterVolumeA(float fVol,
						   DWORD dwLength,
						   BOOL bPaths)
{
	// counter
	DWORD dwCount;
	
	// check flag
	if(g_bAudioInit)
	{
		// set volume of default audiopath
		SetAudioPathVolume(NO_OBJECT,
						   fVol,
						   dwLength);

		// check audiopaths flag
		if(bPaths)
		{
			// check number of audiopaths
			if(g_dwNumAudioPath)
			{
				// set volume of each audio path
				for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
					SetAudioPathVolume(dwCount,
									   fVol,
									   dwLength);
			}
		}
	}
}

// SetAudioMasterTempoA()

void SetAudioMasterTempoA(float fTempo)
{
	// check flag
	if(g_bAudioInit)
	{
		// clip tempo if necessary
		if(fTempo<DMUS_MASTERTEMPO_MIN)
			fTempo=DMUS_MASTERTEMPO_MIN;
		else if(fTempo>DMUS_MASTERTEMPO_MAX)
			fTempo=DMUS_MASTERTEMPO_MAX;

		// set tempo
		g_lpPerformance->SetGlobalParam(GUID_PerfMasterTempo,
										(void*)&fTempo,
										sizeof(float));

		// invalidate audio data
		InvalidateAudioData();
	}
}	

// ActivateAllAudioPaths()

void ActivateAllAudioPaths(void)
{
	// counter
	DWORD dwCount;
	
	// check flag
	if(g_bAudioInit)
	{
		// activate default audiopath
		ActivateAudioPath();

		// check number of audiopaths
		if(g_dwNumAudioPath)
		{
			// activate each audio path
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				ActivateAudioPath(dwCount);
		}
	}
}

// CapturePrimaryAudioSegment()

void CapturePrimaryAudioSegment(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// get current time
		MUSIC_TIME mtCurTime;
		if(FAILED(g_lpPerformance->GetTime(NULL,
										   &mtCurTime)))
			return;
		
		// capture state of primary segment
		IDirectMusicSegmentState* lpSegState;
		if(FAILED(g_lpPerformance->GetSegmentState(&lpSegState,
												   mtCurTime)))
			return;
		else
			AddCOMResource;

		// get primary segment interface
		IDirectMusicSegment* lpSeg;
		if(FAILED(lpSegState->GetSegment(&lpSeg)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}
		else
			AddCOMResource;

		// get updated interface
		if(FAILED(lpSeg->QueryInterface(IID_IDirectMusicSegment8,
										(LPVOID*)&g_lpPrimarySeg)))
		{
			ReleaseCOMPtr(lpSeg);
			ReleaseCOMPtr(lpSegState);
			return;
		}
		else
			AddCOMResource;

		// release old interface
		ReleaseCOMPtr(lpSeg);

		// get segment length
		MUSIC_TIME mtLength;
		if(FAILED(g_lpPrimarySeg->GetLength(&mtLength)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// get segment start time
		MUSIC_TIME mtStartTime;
		if(FAILED(lpSegState->GetStartTime(&mtStartTime)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// get segment start point
		MUSIC_TIME mtStartPoint;
		if(FAILED(lpSegState->GetStartPoint(&mtStartPoint)))
		{
			ReleaseCOMPtr(lpSegState);
			return;
		}

		// release segment state
		ReleaseCOMPtr(lpSegState);

		// compute segment position
		g_mtCurPriSegPos=(mtCurTime-mtStartTime)+mtStartPoint;

		// account for loops
		while(g_mtCurPriSegPos>mtLength)
			g_mtCurPriSegPos-=mtLength;
	}
}

// RestorePrimaryAudioSegment()

void RestorePrimaryAudioSegment(void)
{
	// check flag
	if(g_bAudioInit)
	{
		// check interface
		if(g_lpPrimarySeg)
		{
			// set segment start point
			g_lpPrimarySeg->SetStartPoint(g_mtCurPriSegPos);
			
			// play primary segment
			g_lpPerformance->PlaySegmentEx(g_lpPrimarySeg,
										   NULL,
										   NULL,
										   NULL,
										   0,
										   NULL,
										   NULL,
										   NULL);
			
			// release primary segment
			ReleaseCOMPtr(g_lpPrimarySeg);
		}
	}
}

// SetListenerDistanceFactor()

void SetListenerDistanceFactor(float fDistFactor)
{
	// check flag
	if(g_bAudioInit)
	{
		// save distance factor
		g_fAudioDistFactor=fDistFactor;

		// set value
		g_lpAudioListener->SetDistanceFactor(fDistFactor,
											 DS3D_IMMEDIATE);
	}
}

// SetListenerOrientation()

void SetListenerOrientation(LPD3DXVECTOR3 lpvAhead,
							LPD3DXVECTOR3 lpvUp)
{
	// use other function
	if(lpvUp)
		SetListenerOrientation(lpvAhead->x,
							   lpvAhead->y,
							   lpvAhead->z,
							   lpvUp->x,
							   lpvUp->y,
							   lpvUp->z);
	else
		SetListenerOrientation(lpvAhead->x,
							   lpvAhead->y,
							   lpvAhead->z,
							   0,1,0);
}

// SetListenerOrientation()

void SetListenerOrientation(float fAheadX,
							float fAheadY,
							float fAheadZ,
							float fUpX,
							float fUpY,
							float fUpZ)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetOrientation(fAheadX,
										  fAheadY,
										  fAheadZ,
										  fUpX,
										  fUpY,
										  fUpZ,
										  DS3D_DEFERRED);
	}
}

// SetListenerOrientation()

void SetListenerOrientation(DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerOrientation()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetListenerOrientation(g_lpMtx[dwMtxIndex].mtx._31,
						   g_lpMtx[dwMtxIndex].mtx._32,
						   g_lpMtx[dwMtxIndex].mtx._33,
						   g_lpMtx[dwMtxIndex].mtx._21,
						   g_lpMtx[dwMtxIndex].mtx._22,
						   g_lpMtx[dwMtxIndex].mtx._23);
}

// SetListenerPosition()

void SetListenerPosition(LPD3DXVECTOR3 lpvPos)
{
	// use other function
	SetListenerPosition(lpvPos->x,
						lpvPos->y,
						lpvPos->z);
}

// SetListenerPosition()

void SetListenerPosition(float fX,
						 float fY,
						 float fZ)
{
	// check flag
	if(g_bAudioInit)
	{
		// set value
		g_lpAudioListener->SetPosition(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// SetListenerPosition()

void SetListenerPosition(DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerPosition()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetListenerPosition(g_lpMtx[dwMtxIndex].mtx._41,
						g_lpMtx[dwMtxIndex].mtx._42,
						g_lpMtx[dwMtxIndex].mtx._43);
}

// SetListenerVelocity()

void SetListenerVelocity(LPD3DXVECTOR3 lpvDir,
						 float fSpeed)
{
	// use other function
	SetListenerVelocity(lpvDir->x,
						lpvDir->y,
						lpvDir->z,
						fSpeed);
}

// SetListenerVelocity()

void SetListenerVelocity(float fX,
						 float fY,
						 float fZ,
						 float fSpeed)
{
	// check flag
	if(g_bAudioInit)
	{
		// update velocity by speed
		fX*=fSpeed;
		fY*=fSpeed;
		fZ*=fSpeed;

		// set value
		g_lpAudioListener->SetVelocity(fX,
									   fY,
									   fZ,
									   DS3D_DEFERRED);
	}
}

// SetListenerVelocity()

void SetListenerVelocity(DWORD dwMtxIndex,
						 float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetListenerVelocity()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetListenerVelocity(g_lpMtx[dwMtxIndex].mtx._31,
						g_lpMtx[dwMtxIndex].mtx._32,
						g_lpMtx[dwMtxIndex].mtx._33,
						fSpeed);
}

// Enable3DAudio()

void Enable3DAudio(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// set flag
		g_b3DAudioEnabled=TRUE;

		// check audiopath counter
		if(g_dwNumAudioPath)
		{
			// enable each audiopath
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				if(g_lpbAudioPath3DPrepared[dwCount])
					if(g_lpbAudioPath3DEnabled[dwCount])
						EnableAudioPath3DA(dwCount);
		}
	}
}

// Disable3DAudio()

void Disable3DAudio(void)
{
	// counter
	DWORD dwCount;

	// check flag
	if(g_bAudioInit)
	{
		// check audiopath counter
		if(g_dwNumAudioPath)
		{
			// disable each audiopath
			for(dwCount=0;dwCount<g_dwNumAudioPath;dwCount++)
				if(g_lpbAudioPath3DPrepared[dwCount])
					DisableAudioPath3DA(dwCount);
		}

		// set flag
		g_b3DAudioEnabled=FALSE;
	}
}

// EnableAudioPath3D()

void EnableAudioPath3D(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "EnableAudioPath3D()",
								 NULL);
		return;
	}
#endif

	// save flag
	g_lpbAudioPath3DEnabled[dwIndex]=TRUE;

	// enable audiopath
	EnableAudioPath3DA(dwIndex);
}

// DisableAudioPath3D()

void DisableAudioPath3D(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "DisableAudioPath3D()",
								 NULL);
		return;
	}
#endif

	// save flag
	g_lpbAudioPath3DEnabled[dwIndex]=FALSE;

	// disable audiopath
	DisableAudioPath3DA(dwIndex);
}

// SetAudioPathMinDistance()

void SetAudioPathMinDistance(DWORD dwIndex,
							 float fDist)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathMinDistance()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetMinDistance(fDist,
													  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathMaxDistance()

void SetAudioPathMaxDistance(DWORD dwIndex,
								 float fDist)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathMaxDistance()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetMaxDistance(fDist,
													  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwIndex,
						  LPD3DXVECTOR3 lpvPos)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathPosition(dwIndex,
						 lpvPos->x,
						 lpvPos->y,
						 lpvPos->z);
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwIndex,
						  float fX,
						  float fY,
						  float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetPosition(fX,
												   fY,
												   fZ,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathPosition()

void SetAudioPathPosition(DWORD dwPathIndex,
						  DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathPosition()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathPosition(dwPathIndex,
						 g_lpMtx[dwMtxIndex].mtx._41,
						 g_lpMtx[dwMtxIndex].mtx._42,
						 g_lpMtx[dwMtxIndex].mtx._43);
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwIndex,
							 LPD3DXVECTOR3 lpvAhead)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathOrientation(dwIndex,
							lpvAhead->x,
							lpvAhead->y,
							lpvAhead->z);
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwIndex,
							 float fX,
							 float fY,
							 float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set value
			g_lplpAudioBuf3D[dwIndex]->SetConeOrientation(fX,
														  fY,
														  fZ,
														  DS3D_DEFERRED);
		}
	}
}

// SetAudioPathOrientation()

void SetAudioPathOrientation(DWORD dwPathIndex,
							 DWORD dwMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathOrientation()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathOrientation(dwPathIndex,
						    g_lpMtx[dwMtxIndex].mtx._31,
						    g_lpMtx[dwMtxIndex].mtx._32,
						    g_lpMtx[dwMtxIndex].mtx._33);
}

// SetAudioPathProjectionCone()

void SetAudioPathProjectionCone(DWORD dwIndex,
								DWORD dwInnerAng,
								DWORD dwOuterAng,
								float fOuterVol)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathProjectionCone()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// set angles
			g_lplpAudioBuf3D[dwIndex]->SetConeAngles(dwInnerAng,
													 dwOuterAng,
													 DS3D_DEFERRED);

			// clip volume if necessary
			if(fOuterVol<0)
				fOuterVol=0;
			else if(fOuterVol>1)
				fOuterVol=1;

			// compute attenuation
			long lAtt=(long)(((float)(DSBVOLUME_MAX-DSBVOLUME_MIN))*fOuterVol);

			// set outer volume
			g_lplpAudioBuf3D[dwIndex]->SetConeOutsideVolume(lAtt,
															DS3D_DEFERRED);
		}
	}
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwIndex,
						  LPD3DXVECTOR3 lpvDir,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathVelocity(dwIndex,
						 lpvDir->x,
						 lpvDir->y,
						 lpvDir->z,
						 fSpeed);
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwIndex,
						  float fX,
						  float fY,
						  float fZ,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_lpbAudioPath3DPrepared[dwIndex])
		{
			// update velocity by speed
			fX*=fSpeed;
			fY*=fSpeed;
			fZ*=fSpeed;

			// set value
			g_lplpAudioBuf3D[dwIndex]->SetVelocity(fX,
												   fY,
												   fZ,
												   DS3D_DEFERRED);
		}
	}
}

// SetAudioPathVelocity()

void SetAudioPathVelocity(DWORD dwPathIndex,
						  DWORD dwMtxIndex,
						  float fSpeed)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPathIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetAudioPathVelocity()",
								 NULL);
		return;
	}
#endif

	// use other function
	SetAudioPathVelocity(dwPathIndex,
						 g_lpMtx[dwMtxIndex].mtx._31,
						 g_lpMtx[dwMtxIndex].mtx._32,
						 g_lpMtx[dwMtxIndex].mtx._33,
						 fSpeed);
}

// EnableAudioPath3DA()

void EnableAudioPath3DA(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "EnableAudioPath3DA()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// check flag
			if(g_lpbAudioPath3DPrepared[dwIndex])
			{
				// enable 3d processing
				g_lplpAudioBuf3D[dwIndex]->SetMode(DS3DMODE_NORMAL,
												   DS3D_IMMEDIATE);
			}
		}
	}
}

// DisableAudioPath3DA()

void DisableAudioPath3DA(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "DisableAudioPath3DA()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// check flag
		if(g_b3DAudioEnabled)
		{
			// check flag
			if(g_lpbAudioPath3DPrepared[dwIndex])
			{
				// enable 3d processing
				g_lplpAudioBuf3D[dwIndex]->SetMode(DS3DMODE_DISABLE,
												   DS3D_IMMEDIATE);
			}
		}
	}
}

// SetAudioPathFXChorus()

void SetAudioPathFXChorus(DWORD dwIndex,
						  float fDepth,
						  float fFeedback,
						  float fFrequency,
						  long lWaveform,
						  float fDelay,
						  long lPhase,
						  float fWetDryMix)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXChorus()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXChorus8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_CHORUS,
								   IID_IDirectSoundFXChorus8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXChorus fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fDepth=fDepth;
			fx.fFeedback=fFeedback;
			fx.fFrequency=fFrequency;
			fx.lWaveform=lWaveform;
			fx.fDelay=fDelay;
			fx.lPhase=lPhase;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXCompressor()

void SetAudioPathFXCompressor(DWORD dwIndex,
							  float fGain,
							  float fAttack,
							  float fRelease,
							  float fThreshold,
							  float fRatio,
							  float fPredelay)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXCompressor()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXCompressor8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_COMPRESSOR,
								   IID_IDirectSoundFXCompressor8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXCompressor fx;
			fx.fGain=fGain;
			fx.fAttack=fAttack;
			fx.fRelease=fRelease;
			fx.fThreshold=fThreshold;
			fx.fRatio=fRatio;
			fx.fPredelay=fPredelay;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXDistortion()

void SetAudioPathFXDistortion(DWORD dwIndex,
							  float fGain,
							  float fEdge,
							  float fPostEQCenterFrequency,
							  float fPostEQBandwidth,
							  float fPreLowpassCutoff)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXDistortion()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXDistortion8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_DISTORTION,
								   IID_IDirectSoundFXDistortion8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXDistortion fx;
			fx.fGain=fGain;
			fx.fEdge=fEdge;
			fx.fPostEQCenterFrequency=fPostEQCenterFrequency;
			fx.fPostEQBandwidth=fPostEQBandwidth;
			fx.fPreLowpassCutoff=fPreLowpassCutoff;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXEcho()

void SetAudioPathFXEcho(DWORD dwIndex,
						float fFeedback,
						float fLeftDelay,
						float fRightDelay,
						BOOL bSwapDelays,
						float fWetDryMix)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXEcho()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXEcho8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_ECHO,
								   IID_IDirectSoundFXEcho8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXEcho fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fFeedback=fFeedback;
			fx.fLeftDelay=fLeftDelay;
			fx.fRightDelay=fRightDelay;
			fx.lPanDelay=(long)bSwapDelays;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXFlanger()

void SetAudioPathFXFlanger(DWORD dwIndex,
						   float fDepth,
						   float fFeedback,
						   float fFrequency,
						   long lWaveform,
						   float fDelay,
						   long lPhase,
						   float fWetDryMix)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXFlanger()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXFlanger8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_FLANGER,
								   IID_IDirectSoundFXFlanger8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXFlanger fx;
			fx.fWetDryMix=fWetDryMix;
			fx.fDepth=fDepth;
			fx.fFeedback=fFeedback;
			fx.fFrequency=fFrequency;
			fx.lWaveform=lWaveform;
			fx.fDelay=fDelay;
			fx.lPhase=lPhase;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXGargle()

void SetAudioPathFXGargle(DWORD dwIndex,
						  DWORD dwRateHz,
						  DWORD dwWaveShape)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXGargle()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXGargle8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_GARGLE,
								   IID_IDirectSoundFXGargle8,
								   (LPVOID*)&lpFX))
		{
			// prepare effect description
			DSFXGargle fx;
			fx.dwRateHz=dwRateHz;
			fx.dwWaveShape=dwWaveShape;

			// set effect parameters
			lpFX->SetAllParameters(&fx);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXReverb()

void SetAudioPathFXReverb(DWORD dwIndex,
						  DWORD dwType,
						  long lQuality)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXReverb()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path
		InactivateAudioPath(dwIndex);

		// get effect interface
		IDirectSoundFXI3DL2Reverb8* lpFX;
		if(SetAudioPathFXInterface(dwIndex,
								   GUID_DSFX_STANDARD_I3DL2REVERB,
								   IID_IDirectSoundFXI3DL2Reverb8,
								   (LPVOID*)&lpFX))
		{
			// set type
			lpFX->SetPreset(dwType);

			// set quality
			lpFX->SetQuality(lQuality);

			// release interface
			ReleaseCOMPtr(lpFX);
		}

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathFXNone()

void SetAudioPathFXNone(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXNone()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// set pointer to buffer
		IDirectSoundBuffer8* lpBuf;
		if(dwIndex==NO_OBJECT)
			lpBuf=g_lpDefAudioBuf;
		else
			lpBuf=g_lplpAudioBuf[dwIndex];

		// disable path
		InactivateAudioPath(dwIndex);

		// set no effects
		lpBuf->SetFX(0,
					 NULL,
					 NULL);

		// enable path
		ActivateAudioPath(dwIndex);
	}
}

// SetAudioPathVolume()

void SetAudioPathVolume(DWORD dwIndex,
						float fVol,
						DWORD dwLength)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathVolume()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// get pointer to audiopath
		IDirectMusicAudioPath8* lpPath;
		if(dwIndex==NO_OBJECT)
			lpPath=g_lpDefAudioPath;
		else
			lpPath=g_lplpAudioPath[dwIndex];

		// adjust volume by application master volume
		fVol*=ApplicationMasterVolume();
		
		// clip volume if necessary
		if(fVol<0)
			fVol=0;
		else if(fVol>1)
			fVol=1;

		// compute attenuation value
		long lAtt=((long)(fVol*9600))-9600;

		// set volume of audiopath
		lpPath->SetVolume(lAtt,
						  dwLength);
	}
}

// ActivateAudioPath()

void ActivateAudioPath(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "ActivateAudioPath()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// enable path as needed
		if(dwIndex==NO_OBJECT)
			g_lpDefAudioPath->Activate(TRUE);
		else
			g_lplpAudioPath[dwIndex]->Activate(TRUE);
	}
}

// InactivateAudioPath()

void InactivateAudioPath(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "InactivateAudioPath()",
								 NULL);
		return;
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// disable path as needed
		if(dwIndex==NO_OBJECT)
			g_lpDefAudioPath->Activate(FALSE);
		else
			g_lplpAudioPath[dwIndex]->Activate(FALSE);
	}
}

// SetAudioPathFXInterface()

BOOL SetAudioPathFXInterface(DWORD dwIndex,
							 REFGUID rguidObject,
							 REFGUID rguidInterface,
							 LPVOID* lplpObject)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumAudioPath&&
	   dwIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid audio path index.",
								 "SetAudioPathFXInterface()",
								 NULL);
		return(FALSE);
	}
#endif

	// check flag
	if(g_bAudioInit)
	{
		// get pointers to buffer
		IDirectSoundBuffer8* lpBuf;
		if(dwIndex==NO_OBJECT)
			lpBuf=g_lpDefAudioBuf;
		else
			lpBuf=g_lplpAudioBuf[dwIndex];

		// prepare effect description
		DSEFFECTDESC dsed;
		dsed.dwSize=sizeof(DSEFFECTDESC);
		dsed.dwFlags=NULL;
		dsed.guidDSFXClass=rguidObject;
		dsed.dwReserved1=NULL;
		dsed.dwReserved2=NULL;

		// result code
		DWORD dwResult;

		// used for error checking
		HRESULT hr;

		// set effect on buffer
		hr=lpBuf->SetFX(1,
						&dsed,
						&dwResult);

		// check for error
		if(FAILED(hr))
			return(FALSE);

		// get effect interface
		hr=lpBuf->GetObjectInPath(rguidObject,
								  0,
								  rguidInterface,
								  lplpObject);

		// check for error
		if(FAILED(hr))
			return(FALSE);
		else
			AddCOMResource;

		// ok
		return(TRUE);
	}

	// no good
	return(FALSE);
}

// eof //

