
// r3ddxg.cpp -- DirectX Graphics wrapper

// includes //

#include"r3dwin.h"
#include<rmxftmpl.h>
#include<rmxfguid.h>

// initialize globals //

// direct3d object
IDirect3D8* g_lpD3D=NULL;
// direct3d device object
IDirect3DDevice8* g_lpD3DDevice=NULL;
// display adapter id info
D3DADAPTER_IDENTIFIER8 g_lpAdapterID;
// display device capabilities
D3DCAPS8 g_lpDevCaps;
// windowed rendering flag
BOOL g_bWindowedOK=FALSE;
// device presentation parameters
D3DPRESENT_PARAMETERS g_pp;
// list of render modes
RENDERMODES g_rm;
// list of resolutions
RESOLUTIONS g_res;
// client area
RESOLUTION g_rClient;
// global billboard vectors
D3DXVECTOR3 g_vBBRight=D3DXVECTOR3(1,0,0);
D3DXVECTOR3 g_vBBUp=D3DXVECTOR3(0,1,0);
D3DXVECTOR3 g_vBBAhead=D3DXVECTOR3(0,0,1);
// vertex processing flag
DWORD g_dwVPF=NULL;
// vertex buffer counters
DWORD g_dwNumVtxBuf=0;
DWORD g_dwMaxVtxBuf=0;
// vertex buffer data
IDirect3DVertexBuffer8** g_lplpVtxBuf=NULL;
LPDWORD g_lpdwFVFSize=NULL;
LPDWORD g_lpdwNumVtx=NULL;
LPDWORD g_lpdwVtxType=NULL;
// index buffer counters
DWORD g_dwNumIdxBuf=0;
DWORD g_dwMaxIdxBuf=0;
// index buffer data
IDirect3DIndexBuffer8** g_lplpIdxBuf=NULL;
LPDWORD g_lpdwPType=NULL;
LPDWORD g_lpdwNumPrim=NULL;
// matrix counters
DWORD g_dwNumMtx=0;
DWORD g_dwMaxMtx=0;
// matrix list
LPSCALEDMATRIX g_lpMtx=NULL;
// current rendering data
DWORD g_dwCurVtxShader=NULL;
DWORD g_dwCurNumVtx=0;
DWORD g_dwCurPType=NULL;
DWORD g_dwCurNumPrim=0;
// material counters
DWORD g_dwNumMat=0;
DWORD g_dwMaxMat=0;
// material data
D3DMATERIAL8* g_lpMat=NULL;
LPSTR* g_lplpMatNames=NULL;
// texture counters
DWORD g_dwNumTex=0;
DWORD g_dwMaxTex=0;
// texture data
IDirect3DTexture8** g_lplpTex=NULL;
LPSTR* g_lplpTexNames=NULL;
// light counters
DWORD g_dwNumLight=0;
DWORD g_dwMaxLight=0;
// light data
D3DLIGHT8* g_lpLight=NULL;
LPBOOL g_lpbLightEnabled=NULL;
#ifdef DEBUG_MODE
LPBOOL g_lpbLightValid=NULL;
#endif
LPSTR* g_lplpLightNames=NULL;
// font counters
DWORD g_dwNumFont=0;
DWORD g_dwMaxFont=0;
// font data
HFONT* g_lphFont=NULL;
// font device context
HDC g_hFontDC=NULL;
// sprite rendering values
DWORD g_dwSpriteVtxBuf=NO_OBJECT;
DWORD g_dwSpriteIdxBuf=NO_OBJECT;
DWORD g_dwSpriteMat=NO_OBJECT;
BOOL g_bSpriteWasInScene=FALSE;
BOOL g_bSpriteWasLinearFilter=FALSE;
BOOL g_bSpriteWasInFog=FALSE;
BOOL g_bSpriteWasZBuffer=FALSE;
// sprite matrices
D3DXMATRIX g_mtxSpriteProj;
DWORD g_dwSpriteMtx=NO_OBJECT;
// identity matrix
D3DXMATRIX g_mtxIdentity=D3DXMATRIX(1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1);
// environment rendering interface
ID3DXRenderToSurface* g_lpRTS=NULL;
// fader vertex & index buffer
DWORD g_dwFaderVtxBuf=NO_OBJECT;
DWORD g_dwFaderIdxBuf=NO_OBJECT;
// z-buffer enabled flag
BOOL g_bZBufferEnabled=TRUE;
// mesh counters
DWORD g_dwNumMesh=0;
DWORD g_dwMaxMesh=0;
// mesh data
ID3DXMesh** g_lplpMesh=NULL;
LPDWORD g_lpdwNumMeshSubsets=NULL;
LPMESHSUBSETINFO* g_lplpMeshSubsetInfo=NULL;
LPSTR* g_lplpMeshNames=NULL;
// skin mesh counters
DWORD g_dwNumSkin=0;
DWORD g_dwMaxSkin=0;
// skin mesh data
ID3DXSkinMesh** g_lplpSkin=NULL;
LPDWORD g_lpdwRenderMeshIndex=NULL;
LPDWORD g_lpdwOriginalMeshIndex=NULL;
LPDWORD g_lpdwNumSkinBones=NULL;
LPD3DXMATRIX* g_lplpSkinBoneMtx=NULL;
LPSTR* g_lplpBoneName=NULL;
// line vertex & index buffer
#ifdef DEBUG_MODE
DWORD g_dwLineVtxBuf=NO_OBJECT;
DWORD g_dwLineIdxBuf=NO_OBJECT;
#endif
// invalid render values flag
BOOL g_bInvalidRenderValues=TRUE;
// bone counters
DWORD g_dwNumBone=0;
DWORD g_dwMaxBone=0;
// bone data
LPDWORD g_lpdwBoneMtxIdx=NULL;
LPDWORD g_lpdwNumBoneChildren=NULL;
LPDWORD* g_lplpdwBoneChildren=NULL;
LPDWORD g_lpdwBonePoseIdx=NULL;
LPD3DXMATRIX g_lpOrigBoneMtxInv=NULL;
// pose counters
DWORD g_dwNumPose=0;
DWORD g_dwMaxPose=0;
// pose data
LPDWORD g_lpdwNumPoseBones=NULL;
LPD3DXMATRIX* g_lplpPoseMtx=NULL;
// directx file interface
IDirectXFile* g_lpDXFile=NULL;
// skybox data
DWORD g_dwSkyboxIdxBuf=NO_OBJECT;
DWORD g_dwSkyboxMat=NO_OBJECT;
DWORD g_dwSkyboxMtx=NO_OBJECT;
DWORD g_dwSkyboxTex=NO_OBJECT;
SKYBOXINFO g_sbLeft;
SKYBOXINFO g_sbRight;
SKYBOXINFO g_sbFront;
SKYBOXINFO g_sbBack;
SKYBOXINFO g_sbTop;
SKYBOXINFO g_sbBottom;
float g_fSkyboxRadius=0;
D3DXVECTOR3 g_vSkyboxOffset;
// billboard data
DWORD g_dwBBMtxIndex=NO_OBJECT;
DWORD g_dwBBVtxBufIdx=NO_OBJECT;
DWORD g_dwBBIdxBufIdx=NO_OBJECT;
// buffer surfaces
IDirect3DSurface8* g_lpBackBuffer=NULL;
// environment settings
BOOL g_bInScene=FALSE;
BOOL g_bLinearFilter=FALSE;
BOOL g_bWireframe=FALSE;
BOOL g_bGouraud=FALSE;
WORD g_wLPRepeat=NULL;
WORD g_wLPPattern=NULL;
BOOL g_bZBufferWrites=TRUE;
BOOL g_bBackfaceCull=1;
BOOL g_bABlend=FALSE; 
BOOL g_bATest=FALSE;
BOOL g_bFog=FALSE;
D3DCOLOR g_clrFog=NULL;
float g_fFogStart=0;
float g_fFogEnd=0;
BOOL g_bSpecular=FALSE;
DWORD g_dwZBias=0;
D3DCOLOR g_clrAmbient=NULL;
D3DXMATRIX g_mtxProj;
D3DXMATRIX g_mtxView;
VIEWPORT g_vp;
// saved matrices
D3DXMATRIX g_mtxRTSProj;
D3DXMATRIX g_mtxSprProj;
D3DXMATRIX g_mtxSprView;
// current render size
float g_fCurRenderX=0;
float g_fCurRenderY=0;
// saved render sizes
float g_fRTSRenderX=0;
float g_fRTSRenderY=0;
// transition data
TRANSTYPE g_ttCurTrans=TT_NULL;
DWORD g_dwTransCurPos=0;
DWORD g_dwTransEndPos=0;
DWORD g_lpdwTransData[MAX_TRANSDATA];
DWORD g_dwTransVtxBuf=NO_OBJECT;
DWORD g_dwTransIdxBuf=NO_OBJECT;
BOOL g_bTransRev=FALSE;
#ifdef DEBUG_MODE
// shaded sprites flag
BOOL g_bShadedSprites=FALSE;
// line material
DWORD g_dwLineMat=NO_OBJECT;
#endif
// alpha texture flag
BOOL g_bAlphaTextures=FALSE;
// mouse cursor data
BOOL g_bMouseCursor=FALSE;
DWORD g_dwMouseCursorTexIdx=NO_OBJECT;
DWORD g_dwDefMouseCursorTexIdx=NO_OBJECT;
int g_nMouseXHotSpot=0;
int g_nMouseYHotSpot=0;
float g_fMouseXSize=0;
float g_fMouseYSize=0;
// shatter data
float* g_lpfShatterVtxTU;
float* g_lpfShatterVtxTV;
float* g_lpfShatterCenTU;
float* g_lpfShatterCenTV;
LPD3DXVECTOR3 g_lpvShatterVelocity;
LPDWORD g_lpdwShatterVtx1;
LPDWORD g_lpdwShatterVtx2;
LPDWORD g_lpdwShatterVtx3;
float* g_lpfShatterDeltaYaw;
float* g_lpfShatterDeltaPitch;
float* g_lpfShatterDeltaRoll;

// functions //

// DXGraphicsInit()

BOOL DXGraphicsInit(void)
{
	// init direct3d
	if(!InitDirect3D())
		return(FALSE);

	// init d3d device
	if(!InitD3DDevice())
		return(FALSE);

	// init dxfile
	if(!InitDXFile())
		return(FALSE);

	// hide windows cursor
	HideWindowsCursor();

	// ok
	return(TRUE);
}

// DXGraphicsRelease()

void DXGraphicsRelease(void)
{
	// purge all objects
	PurgeGraphicsObjects();

	// release interfaces
	ReleaseDXFile();
	ReleaseD3DDevice();
	ReleaseDirect3D();
}

// InitRenderModes()

BOOL InitRenderModes(DWORD dwAdapterNum)
{
	// used for error checking
	HRESULT hr=NULL;
	
	// get number of modes
	g_rm.dwNumModes=g_lpD3D->GetAdapterModeCount(dwAdapterNum);

	// check for error
	if(g_rm.dwNumModes==0)
	{
		DisplayDXErrorMessage("Unable to read display adapter modes.",
							  g_lpAdapterID.Description);
		return(FALSE);
	}

	// allocate memory for modes
	if(!AllocMem((LPVOID*)&g_rm.lpModes,
				 sizeof(D3DDISPLAYMODE)*g_rm.dwNumModes))
		return(FALSE);

	// read modes
	DWORD dwCount;
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// get mode
		hr=g_lpD3D->EnumAdapterModes(dwAdapterNum,
									 dwCount,
									 &g_rm.lpModes[dwCount]);
		
		// check for error
		if(FAILED(hr))
		{
			DisplayDXErrorMessage("Unable to read adapter display mode.",
								  g_lpAdapterID.Description,
								  hr);
			return(FALSE);
		}
	}

	// purge modes with formats we don't want
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// check format
		if(!(IsFormatRGB(g_rm.lpModes[dwCount].Format)))
		{
			// invalidate mode
			g_rm.lpModes[dwCount].Width=0;
		}
	}

	// purge modes that differ only by frequency
	DWORD dwCount2;
	for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		for(dwCount2=dwCount+1;dwCount2<g_rm.dwNumModes;dwCount2++)
		{
			// compare modes
			if(g_rm.lpModes[dwCount].Width==g_rm.lpModes[dwCount2].Width&&
			   g_rm.lpModes[dwCount].Height==g_rm.lpModes[dwCount2].Height&&
			   g_rm.lpModes[dwCount].Format==g_rm.lpModes[dwCount2].Format)
			{
				// invalidate mode
				g_rm.lpModes[dwCount].Width=0;
			}
		}

	// purge modes with improper aspect ratio
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// check aspect ratio
		if(!IsNearEqual((((float)g_rm.lpModes[dwCount].Width)/((float)g_rm.lpModes[dwCount].Height)),
					    ASPECT_RATIO))
		{
			// invalidate mode
			g_rm.lpModes[dwCount].Width=0;
		}
	}
	
	// count and defragment remaining modes
	dwCount=0;
	for(dwCount2=0;dwCount2<g_rm.dwNumModes;dwCount2++)
	{
		// check mode
		if(g_rm.lpModes[dwCount2].Width!=0)
		{
			// copy mode
			CopyMemory(&g_rm.lpModes[dwCount++],
					   &g_rm.lpModes[dwCount2],
					   sizeof(D3DDISPLAYMODE));
		}
	}

	// check for empty list
	if(dwCount==0)
	{
		DisplayErrorMessage("Unable to find suitable render formats.",
							"Your display adapter does not support a required format.",
							FALSE);
		return(FALSE);
	}
	
	// resize list of modes
	g_rm.dwNumModes=dwCount;

	// compress list
	if(!CompressMem((LPVOID*)&g_rm.lpModes,
				    sizeof(D3DDISPLAYMODE)*g_rm.dwNumModes))
		return(FALSE);

	// sort list into ascending order
	while(TRUE)
	{
		// number of swaps
		DWORD dwSwaps=0;

		// swap flag
		BOOL bSwap;

		// scan through list and swap as needed
		for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		{
			// reset swap flag
			bSwap=FALSE;

			// set swap flag if needed
			if(g_rm.lpModes[dwCount].Width>g_rm.lpModes[dwCount+1].Width)
				bSwap=TRUE;
			else if(g_rm.lpModes[dwCount].Width==g_rm.lpModes[dwCount+1].Width)
				if(g_rm.lpModes[dwCount].Height>g_rm.lpModes[dwCount+1].Height)
					bSwap=TRUE;

			// check swap flag
			if(bSwap)
			{
				// increment counter
				dwSwaps++;

				// swap values
				D3DDISPLAYMODE dm=g_rm.lpModes[dwCount];
				g_rm.lpModes[dwCount]=g_rm.lpModes[dwCount+1];
				g_rm.lpModes[dwCount+1]=dm;
			}
		}

		// check swap counter
		if(dwSwaps==0)
			break;
	}

	// ok
	return(TRUE);
}

// ReleaseRenderModes()

void ReleaseRenderModes(void)
{
	// free mode memory
	FreeMem((LPVOID*)&g_rm.lpModes);

	// reset counter
	g_rm.dwNumModes=0;
}

// IsModeRenderable()

BOOL IsModeRenderable(LPDISPLAYMODE lpdm)
{
	// counter
	DWORD dwCount;

	// check modes
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// compare width
		if(g_rm.lpModes[dwCount].Width==lpdm->dwX)
		{
			// compare height
			if(g_rm.lpModes[dwCount].Height==lpdm->dwY)
			{
				// compare format
				if(g_rm.lpModes[dwCount].Format==lpdm->fmt)
				{
					// ok
					return(TRUE);
				}
			}
		}
	}

	// not found
	return(FALSE);
}

// SetValidRGBFormat()

BOOL SetValidRGBFormat(LPDISPLAYMODE lpdm)
{
	// check each useful format
	lpdm->fmt=D3DFMT_R5G6B5;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X1R5G5B5;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X4R4G4B4;
	if(IsModeRenderable(lpdm))
		return(TRUE);
	lpdm->fmt=D3DFMT_X8R8G8B8;
	if(IsModeRenderable(lpdm))
		return(TRUE);

	// none found
	return(FALSE);
}

// InitResolutions()

BOOL InitResolutions(void)
{
	// counter
	DWORD dwCount;
	
	// allocate resolutions list
	if(!AllocMem((LPVOID*)&g_res.lpRes,
				 sizeof(RESOLUTION)*g_rm.dwNumModes))
		return(FALSE);

	// copy resolutions
	for(dwCount=0;dwCount<g_rm.dwNumModes;dwCount++)
	{
		// save width & height
		g_res.lpRes[dwCount].dwX=g_rm.lpModes[dwCount].Width;
		g_res.lpRes[dwCount].dwY=g_rm.lpModes[dwCount].Height;
	}

	// purge modes that differ only by format
	DWORD dwCount2;
	for(dwCount=0;dwCount<g_rm.dwNumModes-1;dwCount++)
		for(dwCount2=dwCount+1;dwCount2<g_rm.dwNumModes;dwCount2++)
		{
			// compare formats
			if(g_res.lpRes[dwCount].dwX==g_res.lpRes[dwCount2].dwX&&
			   g_res.lpRes[dwCount].dwY==g_res.lpRes[dwCount2].dwY)
			{
				// invalidate mode
				g_res.lpRes[dwCount].dwX=0;
			}
		}

	// count and defragment remaining modes
	dwCount=0;
	for(dwCount2=0;dwCount2<g_rm.dwNumModes;dwCount2++)
	{
		// check mode
		if(g_res.lpRes[dwCount].dwX!=0)
		{
			// copy mode
			CopyMemory(&g_res.lpRes[dwCount++],
					   &g_res.lpRes[dwCount2],
					   sizeof(RESOLUTION));
		}
	}

	// check for empty list
	if(dwCount==0)
	{
		DisplayErrorMessage("Unable to find suitable resolutions.",
							"Your display adapter does not support a required format.",
							FALSE);
		return(FALSE);
	}
	
	// resize list of resolutions
	g_res.dwNumRes=dwCount;

	// compress memory
	if(!CompressMem((LPVOID*)&g_res.lpRes,
				     sizeof(RESOLUTION)*g_res.dwNumRes))
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseResolutions()

void ReleaseResolutions(void)
{
	// free resolutions memory
	FreeMem((LPVOID*)&g_res.lpRes);

	// reset counter
	g_res.dwNumRes=0;
}

// EnableZBuffer()

void EnableZBuffer(void)
{
	// enable z-buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ZENABLE,
								  D3DZB_TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable z-buffer.",NULL,hr);
#endif
	
	// set flag
	g_bZBufferEnabled=TRUE;
}

// DisableZBuffer()

void DisableZBuffer(void)
{
	// disable z-buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ZENABLE,
								  D3DZB_FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable z-buffer.",NULL,hr);
#endif
	
	// set flag
	g_bZBufferEnabled=FALSE;
}

// EnableWireframe()

void EnableWireframe(void)
{
	// enable wireframe
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FILLMODE,
								  D3DFILL_WIREFRAME);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable wireframe.",NULL,hr);
#endif

	// set flag
	g_bWireframe=TRUE;
}

// DisableWireframe()

void DisableWireframe(void)
{
	// disable wireframe
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FILLMODE,
								  D3DFILL_SOLID);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable wireframe.",NULL,hr);
#endif

	// set flag
	g_bWireframe=FALSE;
}

// EnableGouraud()

void EnableGouraud(void)
{
	// enable gouraud
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SHADEMODE,
								  D3DSHADE_GOURAUD);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable Gouraud.",NULL,hr);
#endif

	// set flag
	g_bGouraud=TRUE;
}

// DisableGouraud()

void DisableGouraud(void)
{
	// disable gouraud
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SHADEMODE,
								  D3DSHADE_FLAT);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable Gouraud.",NULL,hr);
#endif

	// set flag
	g_bGouraud=FALSE;
}

// SetLinePattern()

void SetLinePattern(WORD wRepeatFactor,
					WORD wLinePattern)
{
	// set up structure
	D3DLINEPATTERN lp;
	lp.wRepeatFactor=wRepeatFactor;
	lp.wLinePattern=wLinePattern;

#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_LINEPATTERN,
								  *((DWORD*)(&lp)));
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set line pattern.",NULL,hr);
#endif

	// save values
	g_wLPRepeat=wRepeatFactor;
	g_wLPPattern=wLinePattern;
}

// EnableZBufferWrites()

void EnableZBufferWrites(void)
{
	// enable writes
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable z-buffer writes.",NULL,hr);
#endif

	// set flag
	g_bZBufferWrites=TRUE;
}

// DisableZBufferWrites()

void DisableZBufferWrites(void)
{
	// disable writes
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable z-buffer writes.",NULL,hr);
#endif

	// set flag
	g_bZBufferWrites=FALSE;
}

// EnableBackfaceCull()

void EnableBackfaceCull(void)
{
	// enable cull
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_CCW);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable backface culling.",NULL,hr);
#endif

	// set flag
	g_bBackfaceCull=1;
}

// EnableFrontfaceCull()

void EnableFrontfaceCull(void)
{
	// enable cull
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_CW);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable frontface culling.",NULL,hr);
#endif

	// set flag
	g_bBackfaceCull=2;
}

// DisableBackfaceCull()

void DisableBackfaceCull(void)
{
	// disable cull
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_CULLMODE,
								  D3DCULL_NONE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable backface culling.",NULL,hr);
#endif

	// set flag
	g_bBackfaceCull=FALSE;
}

// EnableAlphaBlend()

void EnableAlphaBlend(void)
{
	// enable blend
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable alpha blending.",NULL,hr);
#endif

	// set flag
	g_bABlend=TRUE;
}

// DisableAlphaBlend()

void DisableAlphaBlend(void)
{
	// disable blend
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable alpha blending.",NULL,hr);
#endif

	// set flag
	g_bABlend=FALSE;
}

// EnableAlphaTest()

void EnableAlphaTest(void)
{
	// enable test
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable alpha testing.",NULL,hr);
#endif

	// set flag
	g_bATest=TRUE;
}

// DisableAlphaTest()

void DisableAlphaTest(void)
{
	// disable test
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable alpha testing.",NULL,hr);
#endif

	// set flag
	g_bATest=FALSE;
}

// EnableFog()

void EnableFog(void)
{
	// enable fog
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FOGENABLE,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable fog.",NULL,hr);
#endif

	// set flag
	g_bFog=TRUE;
}

// DisableFog()

void DisableFog(void)
{
	// disable fog
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FOGENABLE,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable fog.",NULL,hr);
#endif

	// set flag
	g_bFog=FALSE;
}

// EnableSpecular()

void EnableSpecular(void)
{
	// enable specular
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable specular highlights.",NULL,hr);
#endif

	// set flag
	g_bSpecular=TRUE;
}

// DisableSpecular()

void DisableSpecular(void)
{
	// disable specular
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable specular highlights.",NULL,hr);
#endif

	// set flag
	g_bSpecular=FALSE;
}

// SetFogColor()

void SetFogColor(D3DCOLOR clr)
{
	// set fog color
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FOGCOLOR,
								  (DWORD)clr);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set fog color.",NULL,hr);
#endif

	// save color
	g_clrFog=clr;
}

// SetFogStart()

void SetFogStart(float fVal)
{
	// set fog start
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FOGSTART,
								  *((DWORD*)(&fVal)));
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set fog starting value.",NULL,hr);
#endif

	// save value
	g_fFogStart=fVal;
}

// SetFogEnd()

void SetFogEnd(float fVal)
{
	// set fog end
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_FOGEND,
								  *((DWORD*)(&fVal)));
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set fog ending value.",NULL,hr);
#endif

	// save value
	g_fFogEnd=fVal;
}

// SetZBias()

void SetZBias(DWORD dwVal)
{
	// check for overflow
	if(dwVal>16)
		dwVal=16;

#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_ZBIAS,
								  dwVal);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set z-bias.",NULL,hr);
#endif

	// save value
	g_dwZBias=dwVal;
}

// SetAmbientLight()

void SetAmbientLight(D3DCOLOR clr)
{
	// set ambient light
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_AMBIENT,
								  (DWORD)clr);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set ambient light.",NULL,hr);
#endif

	// save value
	g_clrAmbient=clr;
}

// SetViewMatrix()

void SetViewMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// save matrix
		CopyMemory(&g_mtxView,
				   lpMtx,
				   sizeof(D3DXMATRIX));
	}
	else
	{
		// save identity matrix
		CopyMemory(&g_mtxView,
				   &g_mtxIdentity,
				   sizeof(D3DXMATRIX));
	}

	// set view matrix
	SetViewMatrixA(lpMtx);
}

// SetProjectionMatrix()

void SetProjectionMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// save specified matrix
		CopyMemory(&g_mtxProj,
				   lpMtx,
				   sizeof(D3DXMATRIX));
	}
	else
	{
		// save identity matrix
		CopyMemory(&g_mtxProj,
				   &g_mtxIdentity,
				   sizeof(D3DXMATRIX));
	}

	// set matrix
	SetProjectionMatrixA(lpMtx);
}

// SetWorldMatrix()

void SetWorldMatrix(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_WORLD,
									lpMtx);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set world matrix.",NULL,hr);
#endif
	}
	else
	{
		// set identity matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_WORLD,
									&g_mtxIdentity);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set world matrix.",NULL,hr);
#endif
	}
}

// SetBillboardVectors()

void SetBillboardVectors(LPD3DXVECTOR3 lpvAhead)
{
	// flatten view vector
	D3DXVECTOR3 vAhead;
	vAhead.x=lpvAhead->x;
	vAhead.y=0;
	vAhead.z=lpvAhead->z;

	// normalize view vector
	Normalize(&g_vBBAhead,
			  &vAhead);

	// up vector never changes, so compute right vector
	CrossProduct(&g_vBBRight,
				 &g_vBBUp,
				 &g_vBBAhead);
}

// SquareMagnitude()

float SquareMagnitude(D3DXVECTOR3 v)
{
	// use other function
	return(SquareMagnitude(&v));
}

// SquareMagnitude()

float SquareMagnitude(LPD3DXVECTOR3 lpv)
{
	// return value
	return(Squared(lpv->x)+
		   Squared(lpv->y)+
		   Squared(lpv->z));
}

// Magnitude()

float Magnitude(D3DXVECTOR3 v)
{
	// use other function
	return(Magnitude(&v));
}

// Magnitude()

float Magnitude(LPD3DXVECTOR3 lpv)
{
	// return value
	return(sqrtf(SquareMagnitude(lpv)));
}

// Normalize()

D3DXVECTOR3 Normalize(D3DXVECTOR3 v1)
{
	// normalize vector
	D3DXVECTOR3 vTemp;
	Normalize(&vTemp,&v1);
	
	// return vector
	return(vTemp);
}

// Normalize()

void Normalize(LPD3DXVECTOR3 lpv,
			   LPD3DXVECTOR3 lpv1)
{
#ifdef DEBUG_MODE
	// check pointers
	if(lpv==lpv1)
		DisplayErrorMessage("Possible undefined normal.",
							NULL,
							FALSE);

	// check magnitude
	if(IsNearZero(Magnitude(lpv1)))
		DisplayErrorMessage("Undefined normal.",
							NULL,
							FALSE);
#endif
	
	// compute normal
	(*lpv)=(*lpv1)/Magnitude(lpv1);
}

// DotProduct()

float DotProduct(D3DXVECTOR3 v1,
				 D3DXVECTOR3 v2)
{
	// use other function
	return(DotProduct(&v1,&v2));
}

// DotProduct()

float DotProduct(LPD3DXVECTOR3 lpv1,
				 LPD3DXVECTOR3 lpv2)
{
	// return value
	return((lpv1->x*lpv2->x)+
		   (lpv1->y*lpv2->y)+
		   (lpv1->z*lpv2->z));
}

// CrossProduct()

D3DXVECTOR3 CrossProduct(D3DXVECTOR3 v1,
						 D3DXVECTOR3 v2)
{
	// compute cross product
	D3DXVECTOR3 vTemp;
	CrossProduct(&vTemp,&v1,&v2);
	
	// return vector
	return(vTemp);
}

// CrossProduct()

void CrossProduct(LPD3DXVECTOR3 lpv,
				  LPD3DXVECTOR3 lpv1,
				  LPD3DXVECTOR3 lpv2)
{
#ifdef DEBUG_MODE
	// check pointers
	if(lpv==lpv1||
	   lpv==lpv2)
		DisplayErrorMessage("Possible undefined cross product.",
							NULL,
							FALSE);
#endif
	
	// compute cross product
	lpv->x=(lpv1->y*lpv2->z)-(lpv1->z*lpv2->y);
	lpv->y=(lpv1->z*lpv2->x)-(lpv1->x*lpv2->z);
	lpv->z=(lpv1->x*lpv2->y)-(lpv1->y*lpv2->x);
}

// GetNextLight()

DWORD GetNextLight(void)
{
	// check counter
	if(g_dwNumLight==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpLight,
					 sizeof(D3DLIGHT8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpbLightEnabled,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#ifdef DEBUG_MODE
		if(!AllocMem((LPVOID*)&g_lpbLightValid,
					 sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#endif
		if(!AllocMem((LPVOID*)&g_lplpLightNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxLight=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumLight>=g_dwMaxLight)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpLight,
					   sizeof(D3DLIGHT8)*g_dwMaxLight,
					   sizeof(D3DLIGHT8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpbLightEnabled,
					   sizeof(BOOL)*g_dwMaxLight,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#ifdef DEBUG_MODE
		if(!ExpandMem((LPVOID*)&g_lpbLightValid,
					   sizeof(BOOL)*g_dwMaxLight,
					   sizeof(BOOL)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
#endif
		if(!ExpandMem((LPVOID*)&g_lplpLightNames,
					   sizeof(LPSTR)*g_dwMaxLight,
					   sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxLight+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumLight;

	// increment counter
	g_dwNumLight++;

	// return index
	return(dwIndex);
}

// AddLight()

DWORD AddLight(D3DLIGHT8* lplt,
			   LPSTR lpName)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~LT%d",g_dwNumLight);
		return(AddLight(lplt,g_lpMsg));
	}

	// get return index
	DWORD dwIndex=GetNextLight();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpLightNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpLightNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// copy data
	g_lpLight[dwIndex]=(*lplt);

	// normalize direction if necessary
	if(IsNearZero(g_lpLight[dwIndex].Direction.x)||
	   IsNearZero(g_lpLight[dwIndex].Direction.y)||
	   IsNearZero(g_lpLight[dwIndex].Direction.z))
	{}
	else
		g_lpLight[dwIndex].Direction=Normalize(g_lpLight[dwIndex].Direction);
	
	// set light
	HRESULT hr=g_lpD3DDevice->SetLight(dwIndex,
									   lplt);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set light properties.",
							  NULL,
							  hr);
		return(NO_OBJECT);
	}

	// set enabled flag
	g_lpbLightEnabled[dwIndex]=FALSE;

#ifdef DEBUG_MODE
	// validate light
	g_lpbLightValid[dwIndex]=TRUE;
#endif

	// return index
	return(dwIndex);
}

// UpdateLight()

void UpdateLight(DWORD dwIndex,
				 D3DLIGHT8* lplt)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLight()",
								 FALSE);
		return;
	}
#endif

	// copy data if specified
	if(lplt)
		CopyMemory(&g_lpLight[dwIndex],
				   lplt,
				   sizeof(D3DLIGHT8));

	// update light
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetLight(dwIndex,
							&g_lpLight[dwIndex]);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to update light.",
							  NULL,
							  hr);
#endif

#ifdef DEBUG_MODE
	// validate light
	g_lpbLightValid[dwIndex]=TRUE;
#endif
}

// EnableLight()

void EnableLight(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "EnableLight()",
								 FALSE);
		return;
	}
#endif
	
	// enable light
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->LightEnable(dwIndex,
							   TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable light.",
							  NULL,
							  hr);
#endif
	
	// set flag
	g_lpbLightEnabled[dwIndex]=TRUE;
}

// DisableLight()

void DisableLight(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "DisableLight()",
								 FALSE);
		return;
	}
#endif
	
	// disable light
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->LightEnable(dwIndex,
							   FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable light.",
							  NULL,
							  hr);
#endif

	// set flag
	g_lpbLightEnabled[dwIndex]=FALSE;
}

// PurgeLights()

void PurgeLights(void)
{
	// check counter
	if(g_dwNumLight)
	{
		// counter
		DWORD dwCount;
		
		// disable and free lights
		if(g_dwNumLight)
			for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			{
				DisableLight(dwCount);
				FreeMem((LPVOID*)&g_lplpLightNames[dwCount]);
			}

		// free memory
		FreeMem((LPVOID*)&g_lpLight);
		FreeMem((LPVOID*)&g_lpbLightEnabled);
#ifdef DEBUG_MODE
		FreeMem((LPVOID*)&g_lpbLightValid);
#endif
		FreeMem((LPVOID*)&g_lplpLightNames);

		// reset counters
		g_dwNumLight=0;
		g_dwMaxLight=0;
	}
}

// EnableSVP()

void EnableSVP(void)
{
	// enable software vertex processing
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,
								  TRUE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable software vertex processing.",NULL,hr);
#endif
}

// DisableSVP()

void DisableSVP(void)
{
	// disable software vertex processing
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,
								  FALSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable software vertex processing.",NULL,hr);
#endif
}

// GetNextVertexBuffer()

DWORD GetNextVertexBuffer(void)
{
	// check counter
	if(g_dwNumVtxBuf==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lplpVtxBuf,
					 sizeof(IDirect3DVertexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwFVFSize,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumVtx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwVtxType,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxVtxBuf=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumVtxBuf>=g_dwMaxVtxBuf)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpVtxBuf,
					  sizeof(IDirect3DVertexBuffer8*)*g_dwMaxVtxBuf,
					  sizeof(IDirect3DVertexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwFVFSize,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumVtx,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwVtxType,
					  sizeof(DWORD)*g_dwMaxVtxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxVtxBuf+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumVtxBuf;

	// increment counter
	g_dwNumVtxBuf++;
	
	// return index
	return(dwIndex);
}

// AddVertexBuffer()

DWORD AddVertexBuffer(DWORD dwFVF,
					  DWORD dwFVFSize,
					  DWORD dwNumVtx,
					  BOOL bWriteOnly)
{
	// get return index
	DWORD dwIndex=GetNextVertexBuffer();

	// used for error checking
	HRESULT hr;

	// set usage flag
	DWORD dwUsage=NULL;
	if(g_dwVPF==VPF_SOFTWARE)
		dwUsage=dwUsage|D3DUSAGE_SOFTWAREPROCESSING;
	if(bWriteOnly)
		dwUsage=dwUsage|D3DUSAGE_WRITEONLY;

	// save rendering values
	g_lpdwFVFSize[dwIndex]=dwFVFSize;
	g_lpdwNumVtx[dwIndex]=dwNumVtx;
	g_lpdwVtxType[dwIndex]=dwFVF;
	
	// create vertex buffer
	hr=g_lpD3DDevice->CreateVertexBuffer(dwFVFSize*dwNumVtx,
										 dwUsage,
										 dwFVF,
										 D3DPOOL_MANAGED,
										 &g_lplpVtxBuf[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create vertex buffer.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
										 
	// return index
	return(dwIndex);
}

// LockVertexBuffer()

LPVOID LockVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "LockVertexBuffer()",
								 FALSE);
		return(NULL);
	}
#endif

	// return value
	LPVOID lpRet=NULL;

	// lock buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpVtxBuf[dwIndex]->Lock(NULL,
								NULL,
								(LPBYTE*)&lpRet,
								NULL);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to lock vertex buffer.",NULL,hr);
#endif

	// return pointer
	return(lpRet);
}

// UnlockVertexBuffer()

void UnlockVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "UnlockVertexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// unlock buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpVtxBuf[dwIndex]->Unlock();
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock vertex buffer.",NULL,hr);
#endif
}

// SetVertexBuffer()

void SetVertexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumVtxBuf)
	{
		DisplayFatalErrorMessage("Invalid vertex buffer index.",
								 "SetVertexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// save necessary values
	g_dwCurNumVtx=g_lpdwNumVtx[dwIndex];

	// set vertex shader
	SetVertexShader(g_lpdwVtxType[dwIndex]);

	// set stream source
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetStreamSource(NULL,
								   g_lplpVtxBuf[dwIndex],
								   g_lpdwFVFSize[dwIndex]);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set vertex buffer.",NULL,hr);
#endif
}

// PurgeVertexBuffers()

void PurgeVertexBuffers(void)
{
	// check counter
	if(g_dwNumVtxBuf)
	{
		// reset stream source
		if(g_lpD3DDevice)
#ifdef DEBUG_MODE
		{
			HRESULT hr=
#endif
				g_lpD3DDevice->SetStreamSource(NULL,NULL,NULL);
#ifdef DEBUG_MODE
			if(FAILED(hr))
				DisplayDXErrorMessage("Unable to reset vertex buffer.",NULL,hr);
		}
#endif

		// counter
		DWORD dwCount;

		// release all vertex buffers
		for(dwCount=0;dwCount<g_dwNumVtxBuf;dwCount++)
			ReleaseCOMPtr(g_lplpVtxBuf[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lplpVtxBuf);
		FreeMem((LPVOID*)&g_lpdwFVFSize);
		FreeMem((LPVOID*)&g_lpdwNumVtx);
		FreeMem((LPVOID*)&g_lpdwVtxType);
		
		// reset counters
		g_dwNumVtxBuf=0;
		g_dwMaxVtxBuf=0;
	}
}

// GetNextIndexBuffer()

DWORD GetNextIndexBuffer(void)
{
	// check counter
	if(g_dwNumIdxBuf==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpIdxBuf,
					 sizeof(IDirect3DIndexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwPType,
					 sizeof(D3DPRIMITIVETYPE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumPrim,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxIdxBuf=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumIdxBuf>=g_dwMaxIdxBuf)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpIdxBuf,
					  sizeof(IDirect3DIndexBuffer8*)*g_dwMaxIdxBuf,
					  sizeof(IDirect3DIndexBuffer8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwPType,
					  sizeof(D3DPRIMITIVETYPE)*g_dwMaxIdxBuf,
					  sizeof(D3DPRIMITIVETYPE)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumPrim,
					  sizeof(DWORD)*g_dwMaxIdxBuf,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxIdxBuf+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumIdxBuf;

	// increment counter
	g_dwNumIdxBuf++;

	// return index
	return(dwIndex);
}

// AddIndexBuffer()

DWORD AddIndexBuffer(DWORD dwPType,
					 DWORD dwNumPrim,
					 BOOL bWriteOnly)
{
	// check for invalid number of primitives
	if(dwNumPrim>g_lpDevCaps.MaxPrimitiveCount)
	{
		DisplayFatalErrorMessage("Unable to allocate index buffer.",
								 "Your display adapter does not support\na sufficient number of primitives.",
								 FALSE);
		return(NO_OBJECT);
	}
	
	// compute number of indices
	DWORD dwNumIdx;
	switch(dwPType)
	{
	case(D3DPT_POINTLIST):
		dwNumIdx=dwNumPrim;
		break;
	case(D3DPT_LINELIST):
		dwNumIdx=dwNumPrim*2;
		break;
	case(D3DPT_LINESTRIP):
		dwNumIdx=dwNumPrim+1;
		break;
	case(D3DPT_TRIANGLELIST):
		dwNumIdx=dwNumPrim*3;
		break;
	case(D3DPT_TRIANGLESTRIP):
		dwNumIdx=dwNumPrim+2;
		break;
	case(D3DPT_TRIANGLEFAN):
		dwNumIdx=dwNumPrim+2;
		break;
#ifdef DEBUG_MODE
	default:
		DisplayFatalErrorMessage("Invalid primitive type.",NULL,FALSE);
		return(NO_OBJECT);
#endif
	}
	
	// get return index
	DWORD dwIndex=GetNextIndexBuffer();

	// used for error checking
	HRESULT hr;

	// save rendering values
	g_lpdwPType[dwIndex]=dwPType;
	g_lpdwNumPrim[dwIndex]=dwNumPrim;

	// set usage flag
	DWORD dwUsage=NULL;
	if(bWriteOnly)
		dwUsage=dwUsage|D3DUSAGE_WRITEONLY;
	
	// create index buffer
	hr=g_lpD3DDevice->CreateIndexBuffer(sizeof(WORD)*dwNumIdx,
										dwUsage,
										D3DFMT_INDEX16,
										D3DPOOL_MANAGED,
										&g_lplpIdxBuf[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create index buffer.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;
										 
	// return index
	return(dwIndex);
}

// LockIndexBuffer()

LPWORD LockIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "LockIndexBuffer()",
								 FALSE);
		return(NULL);
	}
#endif
	
	// return value
	LPWORD lpRet=NULL;

	// lock buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpIdxBuf[dwIndex]->Lock(NULL,
								NULL,
								(LPBYTE*)&lpRet,
								NULL);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to lock index buffer.",NULL,hr);
#endif

	// return pointer
	return(lpRet);
}

// UnlockIndexBuffer()

void UnlockIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "UnlockIndexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// unlock buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpIdxBuf[dwIndex]->Unlock();
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock index buffer.",NULL,hr);
#endif
}

// SetIndexBuffer()

void SetIndexBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumIdxBuf)
	{
		DisplayFatalErrorMessage("Invalid index buffer index.",
								 "SetIndexBuffer()",
								 FALSE);
		return;
	}
#endif
	
	// set rendering values
	g_dwCurPType=g_lpdwPType[dwIndex];
	g_dwCurNumPrim=g_lpdwNumPrim[dwIndex];

	// set indices
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetIndices(g_lplpIdxBuf[dwIndex],
							  NULL);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set index buffer.",NULL,hr);
#endif
}

// PurgeIndexBuffers()

void PurgeIndexBuffers(void)
{
	// check counter
	if(g_dwNumIdxBuf)
	{
		// reset indices
		if(g_lpD3DDevice)
#ifdef DEBUG_MODE
		{
			HRESULT hr=
#endif
				g_lpD3DDevice->SetIndices(NULL,NULL);
#ifdef DEBUG_MODE
			if(FAILED(hr))
				DisplayDXErrorMessage("Unable to reset index buffer.",NULL,hr);
		}
#endif

		// counter
		DWORD dwCount;

		// release all index buffers
		for(dwCount=0;dwCount<g_dwNumIdxBuf;dwCount++)
			ReleaseCOMPtr(g_lplpIdxBuf[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lplpIdxBuf);
		FreeMem((LPVOID*)&g_lpdwPType);
		FreeMem((LPVOID*)&g_lpdwNumPrim);
		
		// reset counters
		g_dwNumIdxBuf=0;
		g_dwMaxIdxBuf=0;
	}
}

// GetNextMatrix()

DWORD GetNextMatrix(void)
{
	// check counter
	if(g_dwNumMtx==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lpMtx,
					 sizeof(SCALEDMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMtx=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMtx>=g_dwMaxMtx)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpMtx,
					  sizeof(SCALEDMATRIX)*g_dwMaxMtx,
					  sizeof(SCALEDMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxMtx+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMtx;

	// increment counter
	g_dwNumMtx++;

	// return index
	return(dwIndex);
}

// AddMatrix()

DWORD AddMatrix(LPD3DXMATRIX lpmat)
{
	// get return index
	DWORD dwIndex=GetNextMatrix();
	
	// check pointer
	if(lpmat)
		SetMatrix(dwIndex,
				  lpmat);
	else
		ResetMatrix(dwIndex);
										 
	// return index
	return(dwIndex);
}

// MatrixYaw()

void MatrixYaw(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixYaw()",
								 FALSE);
		return;
	}
#endif

	// repeat as needed
	while(fVal>1)
	{
		MatrixYaw(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixYaw(dwIndex,-1);
		fVal+=1;
	}
	
	// move ahead vector right
	MtxAheadX+=MtxRightX*fVal;
	MtxAheadY+=MtxRightY*fVal;
	MtxAheadZ+=MtxRightZ*fVal;

	// normalize ahead vector
	Normalize(&MtxAheadX,
			  &MtxAheadY,
			  &MtxAheadZ,
			  MtxAheadX,
			  MtxAheadY,
			  MtxAheadZ);
	
	// compute new right vector
	CrossProduct(&MtxRightX,
				 &MtxRightY,
				 &MtxRightZ,
				 MtxUpX,
				 MtxUpY,
				 MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ);
}

// MatrixPitch()

void MatrixPitch(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixPitch()",
								 FALSE);
		return;
	}
#endif
	
	// repeat as needed
	while(fVal>1)
	{
		MatrixPitch(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixPitch(dwIndex,-1);
		fVal+=1;
	}
	
	// move ahead vector up
	MtxAheadX+=MtxUpX*fVal;
	MtxAheadY+=MtxUpY*fVal;
	MtxAheadZ+=MtxUpZ*fVal;

	// normalize ahead vector
	Normalize(&MtxAheadX,
			  &MtxAheadY,
			  &MtxAheadZ,
			  MtxAheadX,
			  MtxAheadY,
			  MtxAheadZ);
	
	// compute new up vector
	CrossProduct(&MtxUpX,
				 &MtxUpY,
				 &MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ,
				 MtxRightX,
				 MtxRightY,
				 MtxRightZ);
}

// MatrixRoll()

void MatrixRoll(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixRoll()",
								 FALSE);
		return;
	}
#endif
	
	// repeat as needed
	while(fVal>1)
	{
		MatrixRoll(dwIndex,1);
		fVal-=1;
	}
	while(fVal<-1)
	{
		MatrixRoll(dwIndex,-1);
		fVal+=1;
	}
	
	// move right vector down
	MtxRightX-=MtxUpX*fVal;
	MtxRightY-=MtxUpY*fVal;
	MtxRightZ-=MtxUpZ*fVal;

	// normalize right vector
	Normalize(&MtxRightX,
			  &MtxRightY,
			  &MtxRightZ,
			  MtxRightX,
			  MtxRightY,
			  MtxRightZ);
	
	// compute new up vector
	CrossProduct(&MtxUpX,
				 &MtxUpY,
				 &MtxUpZ,
				 MtxAheadX,
				 MtxAheadY,
				 MtxAheadZ,
				 MtxRightX,
				 MtxRightY,
				 MtxRightZ);
}

// MatrixAhead()

void MatrixAhead(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixAhead()",
								 FALSE);
		return;
	}
#endif
	
	// move ahead
	MtxPosX+=MtxAheadX*fVal;
	MtxPosY+=MtxAheadY*fVal;
	MtxPosZ+=MtxAheadZ*fVal;
}

// MatrixBack()

void MatrixBack(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixBack()",
								 FALSE);
		return;
	}
#endif
	
	// move back
	MtxPosX-=MtxAheadX*fVal;
	MtxPosY-=MtxAheadY*fVal;
	MtxPosZ-=MtxAheadZ*fVal;
}

// MatrixLeft()

void MatrixLeft(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixLeft()",
								 FALSE);
		return;
	}
#endif
	
	// move left
	MtxPosX-=MtxRightX*fVal;
	MtxPosY-=MtxRightY*fVal;
	MtxPosZ-=MtxRightZ*fVal;
}

// MatrixRight()

void MatrixRight(DWORD dwIndex,
			     float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixRight()",
								 FALSE);
		return;
	}
#endif
	
	// move right
	MtxPosX+=MtxRightX*fVal;
	MtxPosY+=MtxRightY*fVal;
	MtxPosZ+=MtxRightZ*fVal;
}

// MatrixUp()

void MatrixUp(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixUp()",
								 FALSE);
		return;
	}
#endif
	
	// move up
	MtxPosX+=MtxUpX*fVal;
	MtxPosY+=MtxUpY*fVal;
	MtxPosZ+=MtxUpZ*fVal;
}

// MatrixDown()

void MatrixDown(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixDown()",
								 FALSE);
		return;
	}
#endif
	
	// move down
	MtxPosX-=MtxUpX*fVal;
	MtxPosY-=MtxUpY*fVal;
	MtxPosZ-=MtxUpZ*fVal;
}

// PurgeMatrices()

void PurgeMatrices(void)
{
	// check counter
	if(g_dwNumMtx)
	{
		// free memory
		FreeMem((LPVOID*)&g_lpMtx);
		
		// reset counters
		g_dwNumMtx=0;
		g_dwMaxMtx=0;
	}
}

// MatrixXScale()

void MatrixXScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixXScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale x
	g_lpMtx[dwIndex].fXScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fXScale<EPSILON)
		g_lpMtx[dwIndex].fXScale=EPSILON;
}

// MatrixYScale()

void MatrixYScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixYScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale y
	g_lpMtx[dwIndex].fYScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fYScale<EPSILON)
		g_lpMtx[dwIndex].fYScale=EPSILON;
}

// MatrixZScale()

void MatrixZScale(DWORD dwIndex,
				  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixZScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale z
	g_lpMtx[dwIndex].fZScale*=fVal;

	// verify value
	if(g_lpMtx[dwIndex].fZScale<EPSILON)
		g_lpMtx[dwIndex].fZScale=EPSILON;
}

// MatrixScale()

void MatrixScale(DWORD dwIndex,
				 float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "MatrixScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale xyz
	g_lpMtx[dwIndex].fXScale*=fVal;
	g_lpMtx[dwIndex].fYScale*=fVal;
	g_lpMtx[dwIndex].fZScale*=fVal;
}

// SetProjectionMatrix()

void SetProjectionMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetProjectionMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mProj;
	GetMatrix(dwIndex,
			  &mProj);
	
	// set matrix
	SetProjectionMatrix(&mProj);
}

// SetWorldMatrix()

void SetWorldMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetWorldMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mWorld;
	GetMatrix(dwIndex,
			  &mWorld);

	// set matrix
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTransform(D3DTS_WORLD,
								&mWorld);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set world matrix.",NULL,hr);
#endif
}

// SetViewMatrix()

void SetViewMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetViewMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// get matrix
	D3DXMATRIX mView;
	GetMatrix(dwIndex,
			  &mView);

	// set view matrix
	SetViewMatrix(&mView);
}

// SquareMagnitude()

float SquareMagnitude(float x,
					  float y,
					  float z)
{
	// return value
	return(Squared(x)+
		   Squared(y)+
		   Squared(z));
}

// Magnitude()

float Magnitude(float x,
			    float y,
			    float z)
{
	// return value
	return(sqrtf(SquareMagnitude(x,y,z)));
}

// Normalize()

void Normalize(float* lpx,
			   float* lpy,
			   float* lpz,
			   float x1,
			   float y1,
			   float z1)
{
#ifdef DEBUG_MODE
	// check magnitude
	if(IsNearZero(Magnitude(x1,y1,z1)))
		DisplayErrorMessage("Undefined normal.",
							NULL,
							FALSE);
#endif

	// compute distance
	float fDist=1/Magnitude(x1,y1,z1);

	// compute normal
	(*lpx)=x1*fDist;
	(*lpy)=y1*fDist;
	(*lpz)=z1*fDist;
}

// DotProduct()

float DotProduct(float x1,
				 float y1,
				 float z1,
				 float x2,
				 float y2,
				 float z2)
{
	// retrun value
	return((x1*x2)+
		   (y1*y2)+
		   (z1*z2));
}

// CrossProduct()

void CrossProduct(float* lpx,
				  float* lpy,
				  float* lpz,
				  float x1,
				  float y1,
				  float z1,
				  float x2,
				  float y2,
				  float z2)
{
	// return value
	(*lpx)=(y1*z2)-(z1*y2);
	(*lpy)=(z1*x2)-(x1*z2);
	(*lpz)=(x1*y2)-(y1*x2);
}

// BeginScene()

BOOL BeginScene(void)
{
	// begin scene
	HRESULT hr=g_lpD3DDevice->BeginScene();
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to begin scene.",NULL,hr);
		return(FALSE);
	}
	
	// set flag
	g_bInScene=TRUE;

	// ok
	return(TRUE);
}

// EndScene()

void EndScene(void)
{
	// end scene
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->EndScene();
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to end scene.",NULL,hr);
#endif

	// set flag
	g_bInScene=FALSE;
}

// PresentScene()

void PresentScene(void)
{
	// render mouse cursor
	RenderMouseCursor();
	
	// present back buffer
	HRESULT hr=g_lpD3DDevice->Present(NULL,NULL,NULL,NULL);

	// check result
	if(FAILED(hr))
	{
		// display error (except lost device)
		if(hr!=D3DERR_DEVICELOST)
			DisplayDXErrorMessage("Unable to present scene.",NULL,hr);
	}

#ifdef DEBUG_MODE
	// check lights
	ValidateLights();
#endif
}

// SetVertexShader()

void SetVertexShader(DWORD dwHandle)
{
	// set vertex shader
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetVertexShader(dwHandle);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set vertex shader.",NULL,hr);
#endif
}

// RenderPrimitive()

void RenderPrimitive(void)
{
	// render primitive
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)g_dwCurPType,
										0,
										g_dwCurNumVtx,
										0,
										g_dwCurNumPrim);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to render primitives.",NULL,hr);
#endif

	// validate render values
	g_bInvalidRenderValues=FALSE;
}

// ClearScreen()

void ClearScreen(D3DCOLOR clr,
				 float fZ)
{
	// clear screen to specified color
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->Clear(0,
						 NULL,
						 D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						 clr,
						 fZ,
						 NULL);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to clear screen.",NULL,hr);
#endif
}

// ClearZBuffer()

void ClearZBuffer(float fZ)
{
#ifdef DEBUG_MODE
	// clear using random color
	ClearScreen(D3DCOLOR_COLORVALUE(fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat()),
									fabsf(GetRandomFloat())));
#else
	// clear z-buffer
	g_lpD3DDevice->Clear(0,
						 NULL,
						 D3DCLEAR_ZBUFFER,
						 NULL,
						 fZ,
						 NULL);
#endif
}

// SetLight()

D3DLIGHT8 SetLight(D3DLIGHTTYPE Type,
				   float DiffuseR,
				   float DiffuseG,
				   float DiffuseB,
				   float DiffuseA,
				   float SpecularR,
				   float SpecularG,
				   float SpecularB,
				   float SpecularA,
				   float AmbientR,
				   float AmbientG,
				   float AmbientB,
				   float AmbientA,
				   float PositionX,
				   float PositionY,
				   float PositionZ,
				   float DirectionX,
				   float DirectionY,
				   float DirectionZ,
				   float Range,
				   float Falloff,
				   float Attenuation0,
				   float Attenuation1,
				   float Attenuation2,
				   float Theta,
				   float Phi)
{
	// return value
	D3DLIGHT8 lt;

	// set values
	lt.Type=Type;
	lt.Diffuse.r=DiffuseR;
	lt.Diffuse.g=DiffuseG;
	lt.Diffuse.b=DiffuseB;
	lt.Diffuse.a=DiffuseA;
	lt.Specular.r=SpecularR;
	lt.Specular.g=SpecularG;
	lt.Specular.b=SpecularB;
	lt.Specular.a=SpecularA;
	lt.Ambient.r=AmbientR;
	lt.Ambient.g=AmbientG;
	lt.Ambient.b=AmbientB;
	lt.Ambient.a=AmbientA;
	lt.Position.x=PositionX;
	lt.Position.y=PositionY;
	lt.Position.z=PositionZ;
	lt.Direction.x=DirectionX;
	lt.Direction.y=DirectionY;
	lt.Direction.z=DirectionZ;
	lt.Range=Range;
	lt.Falloff=Falloff;
	lt.Attenuation0=Attenuation0;
	lt.Attenuation1=Attenuation1;
	lt.Attenuation2=Attenuation2;
	lt.Theta=Theta;
	lt.Phi=Phi;

	// return structure
	return(lt);
}

// SetMaterial()

D3DMATERIAL8 SetMaterial(float DiffuseR,
						 float DiffuseG,
						 float DiffuseB,
						 float DiffuseA,
						 float AmbientR,
						 float AmbientG,
						 float AmbientB,
						 float AmbientA,
						 float SpecularR,
						 float SpecularG,
						 float SpecularB,
						 float SpecularA,
						 float EmissiveR,
						 float EmissiveG,
						 float EmissiveB,
						 float EmissiveA,
						 float Power)
{
	// return value
	D3DMATERIAL8 mat;

	// set up structure
	mat.Diffuse.r=DiffuseR;
	mat.Diffuse.g=DiffuseG;
	mat.Diffuse.b=DiffuseB;
	mat.Diffuse.a=DiffuseA;
	mat.Ambient.r=AmbientR;
	mat.Ambient.g=AmbientG;
	mat.Ambient.b=AmbientB;
	mat.Ambient.a=AmbientA;
	mat.Specular.r=SpecularR;
	mat.Specular.g=SpecularG;
	mat.Specular.b=SpecularB;
	mat.Specular.a=SpecularA;
	mat.Emissive.r=EmissiveR;
	mat.Emissive.g=EmissiveG;
	mat.Emissive.b=EmissiveB;
	mat.Emissive.a=EmissiveA;
	mat.Power=Power;

	// return structure
	return(mat);
}

// CopyRetangle()

void CopyRectangle(IDirect3DSurface8* lpDest,
				   IDirect3DSurface8* lpSource,
				   int nX1Src,
				   int nY1Src,
				   int nX2Src,
				   int nY2Src,
				   int nXDest,
				   int nYDest)
{
	// set up rectangle
	RECT r;
	r.left=nX1Src;
	r.right=nX2Src;
	r.top=nY1Src;
	r.bottom=nY2Src;

	// set up point
	POINT p;
	p.x=nXDest;
	p.y=nYDest;

	// copy rect
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->CopyRects(lpSource,
							 &r,
							 1,
							 lpDest,
							 &p);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to copy rectangle.",NULL,hr);
#endif
}

// CopyFrontBuffer()

void CopyFrontBuffer(IDirect3DSurface8* lpDest)
{
	// copy buffer to surface
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->GetFrontBuffer(lpDest);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to copy front buffer.",NULL,hr);
#endif
}

// CopyBackBuffer()

void CopyBackBuffer(IDirect3DSurface8** lplpDest)
{
	// copy buffer to surface
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->GetBackBuffer(0,
								 D3DBACKBUFFER_TYPE_MONO,
								 lplpDest);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to copy back buffer.",NULL,hr);
#endif
}

// CopyZBuffer()

void CopyZBuffer(IDirect3DSurface8** lplpDest)
{
	// copy buffer to surface
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->GetDepthStencilSurface(lplpDest);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to copy z-buffer.",NULL,hr);
#endif
}

// AddLight()

DWORD AddLight(D3DLIGHTTYPE Type,
			   float DiffuseR,
			   float DiffuseG,
			   float DiffuseB,
			   float DiffuseA,
			   float SpecularR,
			   float SpecularG,
			   float SpecularB,
			   float SpecularA,
			   float AmbientR,
			   float AmbientG,
			   float AmbientB,
			   float AmbientA,
			   float PositionX,
			   float PositionY,
			   float PositionZ,
			   float DirectionX,
			   float DirectionY,
			   float DirectionZ,
			   float Range,
			   float Falloff,
			   float Attenuation0,
			   float Attenuation1,
			   float Attenuation2,
			   float Theta,
			   float Phi,
			   LPSTR lpName)
{
	// set up light structure
	D3DLIGHT8 lt=SetLight(Type,
						  DiffuseR,
						  DiffuseG,
						  DiffuseB,
						  DiffuseA,
						  SpecularR,
						  SpecularG,
						  SpecularB,
						  SpecularA,
						  AmbientR,
						  AmbientG,
						  AmbientB,
						  AmbientA,
						  PositionX,
						  PositionY,
						  PositionZ,
						  DirectionX,
						  DirectionY,
						  DirectionZ,
						  Range,
						  Falloff,
						  Attenuation0,
						  Attenuation1,
						  Attenuation2,
						  Theta,
						  Phi);

	// add light
	return(AddLight(&lt,
		   lpName));
}

// AddMaterial()

DWORD AddMaterial(float DiffuseR,
				  float DiffuseG,
				  float DiffuseB,
				  float DiffuseA,
				  float AmbientR,
				  float AmbientG,
				  float AmbientB,
				  float AmbientA,
				  float SpecularR,
				  float SpecularG,
				  float SpecularB,
				  float SpecularA,
				  float EmissiveR,
				  float EmissiveG,
				  float EmissiveB,
				  float EmissiveA,
				  float Power,
				  LPSTR lpName)
{
	// set up material structure
	D3DMATERIAL8 mat=SetMaterial(DiffuseR,
								 DiffuseG,
								 DiffuseB,
								 DiffuseA,
								 AmbientR,
								 AmbientG,
								 AmbientB,
								 AmbientA,
								 SpecularR,
								 SpecularG,
								 SpecularB,
								 SpecularA,
								 EmissiveR,
								 EmissiveG,
								 EmissiveB,
								 EmissiveA,
								 Power);
	
	// add material
	return(AddMaterial(&mat,
		   lpName));
}

// GetNextMaterial()

DWORD GetNextMaterial(void)
{
	// check counter
	if(g_dwNumMat==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lpMat,
					 sizeof(D3DMATERIAL8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMatNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMat=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMat>=g_dwMaxMat)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpMat,
					  sizeof(D3DMATERIAL8)*g_dwMaxMat,
					  sizeof(D3DMATERIAL8)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMatNames,
					  sizeof(LPSTR)*g_dwMaxMat,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMat+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMat;

	// increment counter
	g_dwNumMat++;

	// return index
	return(dwIndex);
}

// AddMaterial()

DWORD AddMaterial(D3DMATERIAL8* lpmat,
				  LPSTR lpName)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~MT%d",g_dwNumMat);
		return(AddMaterial(lpmat,g_lpMsg));
	}

	// get return index
	DWORD dwIndex=GetNextMaterial();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMatNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMatNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// copy material
	g_lpMat[dwIndex]=(*lpmat);
										 
	// return index
	return(dwIndex);
}

// UpdateMaterial()

void UpdateMaterial(DWORD dwIndex,
					D3DMATERIAL8* lpmat)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterial()",
								 FALSE);
		return;
	}
#endif
	
	// copy material
	g_lpMat[dwIndex]=(*lpmat);
}

// UpdateMaterialDiffuse()

void UpdateMaterialDiffuse(DWORD dwIndex,
						   float DiffuseR,
						   float DiffuseG,
						   float DiffuseB,
						   float DiffuseA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialDiffuse()",
								 FALSE);
		return;
	}
#endif
	
	// copy diffuse
	g_lpMat[dwIndex].Diffuse.r=DiffuseR;
	g_lpMat[dwIndex].Diffuse.g=DiffuseG;
	g_lpMat[dwIndex].Diffuse.b=DiffuseB;
	g_lpMat[dwIndex].Diffuse.a=DiffuseA;
}

// UpdateMaterialAmbient()

void UpdateMaterialAmbient(DWORD dwIndex,
						   float AmbientR,
						   float AmbientG,
						   float AmbientB,
						   float AmbientA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialAmbient()",
								 FALSE);
		return;
	}
#endif
	
	// copy ambient
	g_lpMat[dwIndex].Ambient.r=AmbientR;
	g_lpMat[dwIndex].Ambient.g=AmbientG;
	g_lpMat[dwIndex].Ambient.b=AmbientB;
	g_lpMat[dwIndex].Ambient.a=AmbientA;
}

// UpdateMaterialSpecular()

void UpdateMaterialSpecular(DWORD dwIndex,
							float SpecularR,
							float SpecularG,
							float SpecularB,
							float SpecularA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialSpecular()",
								 FALSE);
		return;
	}
#endif
	
	// copy specular
	g_lpMat[dwIndex].Specular.r=SpecularR;
	g_lpMat[dwIndex].Specular.g=SpecularG;
	g_lpMat[dwIndex].Specular.b=SpecularB;
	g_lpMat[dwIndex].Specular.a=SpecularA;
}

// UpdateMaterialEmissive()

void UpdateMaterialEmissive(DWORD dwIndex,
							float EmissiveR,
							float EmissiveG,
							float EmissiveB,
							float EmissiveA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialEmissive()",
								 FALSE);
		return;
	}
#endif
	
	// copy emissive
	g_lpMat[dwIndex].Emissive.r=EmissiveR;
	g_lpMat[dwIndex].Emissive.g=EmissiveG;
	g_lpMat[dwIndex].Emissive.b=EmissiveB;
	g_lpMat[dwIndex].Emissive.a=EmissiveA;
}

// UpdateMaterialPower()

void UpdateMaterialPower(DWORD dwIndex,
						 float Power)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMat)
	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "UpdateMaterialPower()",
								 FALSE);
		return;
	}
#endif
	
	// copy power
	g_lpMat[dwIndex].Power=Power;
}

// SetMaterial()

void SetMaterial(DWORD dwIndex)
{
	// default material
	static D3DMATERIAL8 matDef={1,1,1,1, // diffuse
								1,1,1,1, // ambient
								0,0,0,1, // specular
								0,0,0,1, // emissive
								0};		 // power
	
	// check index
	if(dwIndex==NO_OBJECT)
	{
		// set default material
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetMaterial(&matDef);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set material.",
								  NULL,
								  hr);
#endif
	}
	else
	{
#ifdef DEBUG_MODE
		// verify index
		if(dwIndex>=g_dwNumMat)
		{
			DisplayFatalErrorMessage("Invalid material index.",
									 "SetMaterial()",
									 FALSE);
			return;
		}
#endif

		// set specified material
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetMaterial(&g_lpMat[dwIndex]);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set material.",
								  NULL,
								  hr);
#endif
	}
}

// PurgeMaterials()

void PurgeMaterials(void)
{
	// check counter
	if(g_dwNumMat)
	{
		// free names
		DWORD dwCount;
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
			FreeMem((LPVOID*)&g_lplpMatNames[dwCount]);
		
		// free memory
		FreeMem((LPVOID*)&g_lpMat);
		FreeMem((LPVOID*)&g_lplpMatNames);
		
		// reset counters
		g_dwNumMat=0;
		g_dwMaxMat=0;
	}
}

// UpdateLightType()

void UpdateLightType(DWORD dwIndex,
					 D3DLIGHTTYPE Type)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightType()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Type=Type;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightDiffuse()

void UpdateLightDiffuse(DWORD dwIndex,
						float DiffuseR,
						float DiffuseG,
						float DiffuseB,
						float DiffuseA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightDiffuse()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Diffuse.r=DiffuseR;
	g_lpLight[dwIndex].Diffuse.g=DiffuseG;
	g_lpLight[dwIndex].Diffuse.b=DiffuseB;
	g_lpLight[dwIndex].Diffuse.a=DiffuseA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightSpecular()

void UpdateLightSpecular(DWORD dwIndex,
						 float SpecularR,
						 float SpecularG,
						 float SpecularB,
						 float SpecularA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightSpecular()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Specular.r=SpecularR;
	g_lpLight[dwIndex].Specular.g=SpecularG;
	g_lpLight[dwIndex].Specular.b=SpecularB;
	g_lpLight[dwIndex].Specular.a=SpecularA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightAmbient()

void UpdateLightAmbient(DWORD dwIndex,
						float AmbientR,
						float AmbientG,
						float AmbientB,
						float AmbientA)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightAmbient()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Ambient.r=AmbientR;
	g_lpLight[dwIndex].Ambient.g=AmbientG;
	g_lpLight[dwIndex].Ambient.b=AmbientB;
	g_lpLight[dwIndex].Ambient.a=AmbientA;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightPosition()

void UpdateLightPosition(DWORD dwIndex,
						 float PositionX,
						 float PositionY,
						 float PositionZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightPosition()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Position.x=PositionX;
	g_lpLight[dwIndex].Position.y=PositionY;
	g_lpLight[dwIndex].Position.z=PositionZ;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightDirection()

void UpdateLightDirection(DWORD dwIndex,
						  float DirectionX,
						  float DirectionY,
						  float DirectionZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightDirection()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Direction.x=DirectionX;
	g_lpLight[dwIndex].Direction.y=DirectionY;
	g_lpLight[dwIndex].Direction.z=DirectionZ;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightRange()

void UpdateLightRange(DWORD dwIndex,
					  float Range)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightRange()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Range=Range;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightFalloff()

void UpdateLightFalloff(DWORD dwIndex,
						float Falloff)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightFalloff()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Falloff=Falloff;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightAttenuation()

void UpdateLightAttenuation(DWORD dwIndex,
							float Attenuation0,
							float Attenuation1,
							float Attenuation2)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightAttenuation()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Attenuation0=Attenuation0;
	g_lpLight[dwIndex].Attenuation1=Attenuation1;
	g_lpLight[dwIndex].Attenuation2=Attenuation2;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightTheta()

void UpdateLightTheta(DWORD dwIndex,
					  float Theta)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightTheta()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Theta=Theta;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// UpdateLightPhi()

void UpdateLightPhi(DWORD dwIndex,
					float Phi)
{
#ifdef DEBUG_MODE
	// verify light
	if(dwIndex>=g_dwNumLight)
	{
		DisplayFatalErrorMessage("Invalid light index.",
								 "UpdateLightPhi()",
								 FALSE);
		return;
	}
#endif
	
	// udpate data
	g_lpLight[dwIndex].Phi=Phi;

#ifdef DEBUG_MODE
	// invalidate light
	g_lpbLightValid[dwIndex]=FALSE;
#endif
}

// GetNextTexture()

DWORD GetNextTexture(void)
{
	// check counter
	if(g_dwNumTex==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpTex,
					 sizeof(IDirect3DTexture8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpTexNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTex=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumTex>=g_dwMaxTex)
	{
		// expand memory		
		if(!ExpandMem((LPVOID*)&g_lplpTex,
					  sizeof(IDirect3DTexture8*)*g_dwMaxTex,
					  sizeof(IDirect3DTexture8*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpTexNames,
					  sizeof(LPSTR)*g_dwMaxTex,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxTex+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumTex;

	// increment counter
	g_dwNumTex++;

	// return index
	return(dwIndex);
}

// AddTexture()

DWORD AddTexture(LPCSTR lpFile,
				 LPSTR lpName,
				 DWORD dwMipMapLevels)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~TX%d",g_dwNumTex);
		return(AddTexture(lpFile,g_lpMsg,dwMipMapLevels));
	}

	// get next texture
	DWORD dwIndex=GetNextTexture();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpTexNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);
	
	// used for error checking
	HRESULT hr;

	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// load texture
	if(dfr.lpData)
		hr=D3DXCreateTextureFromFileInMemoryEx(g_lpD3DDevice,
											   dfr.lpData,
											   dfr.dwSize,
											   D3DX_DEFAULT,
											   D3DX_DEFAULT,
											   dwMipMapLevels,
											   NULL,
											   D3DFMT_UNKNOWN,
											   D3DPOOL_MANAGED,
											   D3DX_FILTER_NONE,
											   D3DX_FILTER_BOX,
											   NULL,
											   NULL,
											   NULL,
											   &g_lplpTex[dwIndex]);
	else
		hr=D3DXCreateTextureFromFileEx(g_lpD3DDevice,
									   lpFile,
									   D3DX_DEFAULT,
									   D3DX_DEFAULT,
									   dwMipMapLevels,
									   NULL,
									   D3DFMT_UNKNOWN,
									   D3DPOOL_MANAGED,
									   D3DX_FILTER_NONE,
									   D3DX_FILTER_BOX,
									   NULL,
									   NULL,
									   NULL,
									   &g_lplpTex[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to load texture.",
								   (LPSTR)lpFile,
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// return index
	return(dwIndex);
}

// LockTexture()

D3DLOCKED_RECT LockTexture(DWORD dwIndex)
{
	// return value
	D3DLOCKED_RECT lr;
	InitStruct(lr);

#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "LockTexture()",
								 FALSE);
		return(lr);
	}
#endif
	
	// lock texture
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpTex[dwIndex]->LockRect(0,
								 &lr,
								 NULL,
								 NULL);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to lock texture.",
							  NULL,
							  hr);
#endif

	// return locked rect
	return(lr);
}
					   
// UnlockTexture()

void UnlockTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "UnlockTexture()",
								 FALSE);
		return;
	}
#endif
	
	// unlock texture
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpTex[dwIndex]->UnlockRect(0);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock texture.",
							  NULL,
							  hr);
#endif
}

// SetTexture()

void SetTexture(DWORD dwIndex)
{
	// check index
	if(dwIndex==NO_OBJECT)
	{
		// reset texture
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTexture(0,
								  NULL);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set texture.",
								  NULL,
								  hr);
#endif
	}
	else
	{
#ifdef DEBUG_MODE
		// verify index
		if(dwIndex>=g_dwNumTex)
		{
			DisplayFatalErrorMessage("Invalid texture index.",
									 "SetSkinMeshBoneMtx()",
									 FALSE);
			return;
		}
#endif

		// set texture
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTexture(0,
								  g_lplpTex[dwIndex]);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set texture.",
								  NULL,
								  hr);
#endif
	}
}

// PurgeTextures()

void PurgeTextures(void)
{
	// check counter
	if(g_dwNumTex)
	{
		// reset texture
		if(g_lpD3DDevice)
#ifdef DEBUG_MODE
		{	
			HRESULT hr=
#endif
				g_lpD3DDevice->SetTexture(0,
										  NULL);
#ifdef DEBUG_MODE
			if(FAILED(hr))
				DisplayDXErrorMessage("Unable to reset texture.",
									  NULL,
									  hr);
		}
#endif

		// counter
		DWORD dwCount;

		// release all textures and names
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
		{
			ReleaseCOMPtr(g_lplpTex[dwCount]);
			FreeMem((LPVOID*)&g_lplpTexNames[dwCount]);
		}

		// free memory
		FreeMem((LPVOID*)&g_lplpTex);
		FreeMem((LPVOID*)&g_lplpTexNames);
		
		// reset counters
		g_dwNumTex=0;
		g_dwMaxTex=0;
	}
}

// EnableLinearFilter()

void EnableLinearFilter(void)
{
	// set magnification
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MAGFILTER,
										D3DTEXF_LINEAR);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set magnification filter.",NULL,hr);
#endif

	// set minification
#ifdef DEBUG_MODE
	hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MINFILTER,
										D3DTEXF_LINEAR);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set minification filter.",NULL,hr);
#endif

	// set mipmap
#ifdef DEBUG_MODE
	hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MIPFILTER,
										D3DTEXF_LINEAR);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set mip mapping filter.",NULL,hr);
#endif

	// set flag
	g_bLinearFilter=TRUE;
}

// DisableLinearFilter()

void DisableLinearFilter(void)
{
	// set magnification
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MAGFILTER,
										D3DTEXF_POINT);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set magnification filter.",NULL,hr);
#endif
	
	// set minification
#ifdef DEBUG_MODE
	hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MINFILTER,
										D3DTEXF_POINT);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set minification filter.",NULL,hr);
#endif

	// set mipmap
#ifdef DEBUG_MODE
	hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_MIPFILTER,
										D3DTEXF_POINT);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set mip mapping filter.",NULL,hr);
#endif

	// set flag
	g_bLinearFilter=FALSE;
}

#ifdef DEBUG_MODE
// CreateDDSFromImage()

void CreateDDSFromImage(LPCSTR lpDestFile,
						LPCSTR lpSrcFile,
						BOOL bTransparent,
						BOOL bTranslucent,
						float fAlpha)
{
	// pointer to texture
	IDirect3DTexture8* lpTex=NULL;
	
	// used for error checking
	HRESULT hr;

	// set format
	D3DFORMAT fmt;
	if(bTranslucent)
		fmt=D3DFMT_A4R4G4B4;
	else
	{
		if(bTransparent)
			fmt=D3DFMT_A1R5G5B5;
		else
			fmt=D3DFMT_R5G6B5;
	}

	// load texture
	hr=D3DXCreateTextureFromFileEx(g_lpD3DDevice,
								   lpSrcFile,
								   D3DX_DEFAULT,
								   D3DX_DEFAULT,
								   1,
								   NULL,
								   fmt,
								   D3DPOOL_MANAGED,
								   D3DX_FILTER_NONE,
								   D3DX_FILTER_BOX,
								   NULL,
								   NULL,
								   NULL,
								   &lpTex);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to load texture.",
								   (LPSTR)lpSrcFile,
								   hr);
		return;
	}
	else
		AddCOMResource;

	// get surface description
	D3DSURFACE_DESC sd;
	hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read texture description.",
							  (LPSTR)lpSrcFile,
							  hr);
		return;
	}

	// check format
	if(sd.Format!=fmt)
	{
		DisplayErrorMessage("Unable to load texture into proper format.",
							(LPSTR)lpSrcFile,
							FALSE);
		return;
	}

	// add alpha
	AddAlphaToTexture(lpTex,
					  fAlpha);

	// output to DDS file
	hr=D3DXSaveTextureToFile(lpDestFile,
							 D3DXIFF_DDS,
							 lpTex,
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to output texture.",
								   (LPSTR)lpDestFile,
								   hr);
		return;
	}

	// release pointer
	ReleaseCOMPtr(lpTex);
}
#endif

// SetFont()

LOGFONT SetFont(LPSTR FaceName,
				LONG Height,
				LONG Width,
				LONG Angle,
				LONG Weight,
				BYTE Italic,
				BYTE Underline,
				BYTE Strikeout)
{
	// structure to return
	LOGFONT lf;

	// set up structure
	lf.lfHeight=Height;
	lf.lfWidth=Width;
	lf.lfEscapement=Angle;
	lf.lfOrientation=Angle;
	lf.lfWeight=Weight;
	lf.lfItalic=Italic;
	lf.lfUnderline=Underline;
	lf.lfStrikeOut=Strikeout;
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfOutPrecision=OUT_TT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=DEFAULT_PITCH|FF_DONTCARE;
	if(FaceName)
		strcpy(lf.lfFaceName,
			   FaceName);
	else
		lf.lfFaceName[0]=0;
	
	// return structure
	return(lf);
}

// AddFont()

DWORD AddFont(LPSTR FaceName,
			  LONG Height,
			  LONG Width,
			  LONG Angle,
			  LONG Weight,
			  BYTE Italic,
			  BYTE Underline,
			  BYTE Strikeout)
{
	// set up structure
	LOGFONT lf=SetFont(FaceName,
					   Height,
					   Width,
					   Angle,
					   Weight,
					   Italic,
					   Underline,
					   Strikeout);

	// add font
	return(AddFont(&lf));
}

// GetNextFont()

DWORD GetNextFont(void)
{
	// check counter
	if(g_dwNumFont==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lphFont,
					 sizeof(HFONT*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxFont=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumFont>=g_dwMaxFont)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lphFont,
					  sizeof(HFONT*)*g_dwMaxFont,
					  sizeof(HFONT*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxFont+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumFont;

	// increment counter
	g_dwNumFont++;

	// return index
	return(dwIndex);
}

// AddFont()

DWORD AddFont(LOGFONT* lplf)
{
	// get return index
	DWORD dwIndex=GetNextFont();

	// create font
	g_lphFont[dwIndex]=CreateFontIndirect(lplf);

	// check for error
	if(!g_lphFont[dwIndex])
	{
		DisplayFatalErrorMessage("Unable to load font.",
								 lplf->lfFaceName);
		return(NO_OBJECT);
	}
	else
		AddWinResource;

	// return index
	return(dwIndex);
}

// BeginText()

BOOL BeginText(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumFont)
	{
		DisplayFatalErrorMessage("Invalid font index.",
								 "BeginText()",
								 FALSE);
		return(FALSE);
	}
#endif

	// init font device context
	g_hFontDC=GetDC(g_hWnd);

	// check for error
	if(!g_hFontDC)
	{
		DisplayErrorMessage("Unable to retrieve device context.");
		return(FALSE);
	}
	else
		AddWinResource;

	// select font into device context
	SelectObject(g_hFontDC,g_lphFont[dwIndex]);

	// ok
	return(TRUE);
}

// OutputText()

void OutputText(LPSTR lpText,
			    int nX,
			    int nY,
			    COLORREF crForeground,
			    COLORREF crBackground,
			    DWORD dwFormat)
{
	// set alignment
	SetTextAlign(g_hFontDC,dwFormat);

	// set foreground color
	SetTextColor(g_hFontDC,crForeground);

	// set background color
	SetBkColor(g_hFontDC,crBackground);

	// output text
	TextOut(g_hFontDC,nX,nY,lpText,strlen(lpText));
}

// EndText()

void EndText(void)
{
	// select font out of device context
	SelectObject(g_hFontDC,NULL);

	// delete device context
	DeleteDC(g_hFontDC);
	
	// remove resource
	RemoveWinResource;
}

// PurgeFonts()

void PurgeFonts(void)
{
	// check counter
	if(g_dwNumFont)
	{
		// counter
		DWORD dwCount;

		// release all fonts
		for(dwCount=0;dwCount<g_dwNumFont;dwCount++)
		{
			DeleteObject(g_lphFont[dwCount]);
			RemoveWinResource;
		}

		// free memory
		FreeMem((LPVOID*)&g_lphFont);
		
		// reset counters
		g_dwNumFont=0;
		g_dwMaxFont=0;
	}
}

// InitSprites()

void InitSprites(void)
{
	// add material
	g_dwSpriteMat=AddMaterial(0,0,0,1,
							  0,0,0,1,
							  0,0,0,1,
							  1,1,1,1,
							  0);
	
	// add vertex buffer
	g_dwSpriteVtxBuf=AddVertexBuffer(D3DFVF_SPRITEVERTEX,
									 sizeof(SPRITEVERTEX),
									 4);

	// fill vertex buffer
	LPSPRITEVERTEX lpVtx=(LPSPRITEVERTEX)LockVertexBuffer(g_dwSpriteVtxBuf);
	SET_SPRITEVERTEX(lpVtx[0],
					 -0.5F,-0.5F,0,
					 0,1);
	SET_SPRITEVERTEX(lpVtx[1],
					 0.5F,-0.5F,0,
					 1,1);
	SET_SPRITEVERTEX(lpVtx[2],
					 -0.5F,0.5F,0,
					 0,0);
	SET_SPRITEVERTEX(lpVtx[3],
					 0.5F,0.5F,0,
					 1,0);
	UnlockVertexBuffer(g_dwSpriteVtxBuf);

	// add index buffer
	g_dwSpriteIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
									2);

	// fill index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwSpriteIdxBuf);
	lpIdx[0]=1;
	lpIdx[1]=0;
	lpIdx[2]=2;
	lpIdx[3]=3;
	UnlockIndexBuffer(g_dwSpriteIdxBuf);

	// create sprite matrix
	g_dwSpriteMtx=AddMatrix(NULL);
}

// BeginSprites()

BOOL BeginSprites(BOOL bLinear)
{
	// save projection matrix
	CopyMemory(&g_mtxSprProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));
	
	// save view matrix
	CopyMemory(&g_mtxSprView,
			   &g_mtxView,
			   sizeof(D3DXMATRIX));
	
	// reset sprite projection matrix
	ResetSpriteProjectionMatrix();
	
	// set sprite projection matrix
	SetProjectionMatrixA(&g_mtxSpriteProj);

	// set view matrix to identity
	SetViewMatrixA();

	// set vertex & index buffers
	SetVertexBuffer(g_dwSpriteVtxBuf);
	SetIndexBuffer(g_dwSpriteIdxBuf);

	// set sprite material
	SetMaterial(g_dwSpriteMat);

	// check point filter
	if(g_bLinearFilter)
	{
		// set point filter
		if(!bLinear)
			DisableLinearFilter();

		// set flag
		g_bSpriteWasLinearFilter=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasLinearFilter=FALSE;
	}
	
	// check fog
	if(g_bFog)
	{
		// disable fog		
		DisableFog();

		// set flag
		g_bSpriteWasInFog=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasInFog=FALSE;
	}
	
	// check scene
	if(g_bInScene)
	{
		// set flag
		g_bSpriteWasInScene=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasInScene=FALSE;

		// begin scene
		if(!BeginScene())
			return(FALSE);
	}
	
	// check z-buffer
	if(g_bZBufferEnabled)
	{
		// disable z-buffer
		DisableZBuffer();

		// set flag
		g_bSpriteWasZBuffer=TRUE;
	}
	else
	{
		// set flag
		g_bSpriteWasZBuffer=FALSE;
	}

	// ok
	return(TRUE);
}

// OutputSprite()

void OutputSprite(DWORD dwTexIndex,
				  float fX,
				  float fY,
				  float fXSize,
				  float fYSize,
				  float fRot)
{
#ifdef DEBUG_MODE
	// check flag
	if(g_bShadedSprites)
		DisplayInfoMessage("Unshaded sprite in shaded mode!");
#endif
	
	// update values based on render size
	fX*=g_fCurRenderX;
	fY*=g_fCurRenderY;
	fXSize*=g_fCurRenderX;
	fYSize*=g_fCurRenderY;
	
	// reset sprite matrix
	ResetMatrix(g_dwSpriteMtx);

	// translate matrix
	MatrixRight(g_dwSpriteMtx,
				fX);
	MatrixUp(g_dwSpriteMtx,
			 fY);

	// rotate matrix
	if(fRot)
		MatrixRoll(g_dwSpriteMtx,
				   fRot);

	// scale matrix
	MatrixXScale(g_dwSpriteMtx,
				 fXSize);
	MatrixYScale(g_dwSpriteMtx,
				 fYSize);

	// set world matrix
	SetWorldMatrix(g_dwSpriteMtx);

	// set texture
	SetTexture(dwTexIndex);

	// render texture
	RenderPrimitive();
}

// EndSprites()

void EndSprites(void)
{
	// end scene if necessary
	if(!g_bSpriteWasInScene)
		EndScene();

	// enable fog if necessary
	if(g_bSpriteWasInFog)
		EnableFog();
	
	// set linear filter if necessary
	if(g_bSpriteWasLinearFilter)
		EnableLinearFilter();
	
	// enable z-buffer if necessary
	if(g_bSpriteWasZBuffer)
		EnableZBuffer();
	
	// restore projection & view matrices
	SetProjectionMatrixA(&g_mtxSprProj);
	SetViewMatrixA(&g_mtxSprView);
}

// ResetMatrix()

void ResetMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "ResetMatrix()",
								 FALSE);
		return;
	}
#endif
	
	// reset matrix to identity
	CopyMemory(&g_lpMtx[dwIndex],
			   &g_mtxIdentity,
			   sizeof(D3DXMATRIX));

	// reset scale values
	g_lpMtx[dwIndex].fXScale=1;
	g_lpMtx[dwIndex].fYScale=1;
	g_lpMtx[dwIndex].fZScale=1;
}

// ResetMatrix()

void ResetMatrix(LPD3DXMATRIX lpMtx)
{
	// reset matrix to identity
	CopyMemory(lpMtx,
			   &g_mtxIdentity,
			   sizeof(D3DXMATRIX));
}

// Get2DLineIntersection()
BOOL Get2DLineIntersection(float* lpfXI,
						   float* lpfYI,
						   float fX1,
						   float fY1,
						   float fX2,
						   float fY2,
						   float fX3,
						   float fY3,
						   float fX4,
						   float fY4)
{
	// compute delta values
	float fDX=fX2-fX1;
	float fDY=fY2-fY1;

	// compute denominator value
	float fD=((fY4-fY3)*fDX)-((fX4-fX3)*fDY);
	
	// check value
	if(fD==0)
	{
		// lines are parallel
		return(FALSE);
	}
	else
	{
		// compute projection value
		float fU=(((fX4-fX3)*(fY1-fY3))-((fY4-fY3)*(fX1-fX3)))/fD;

		// compute intersection
		(*lpfXI)=fX1+(fU*fDX);
		(*lpfYI)=fY1+(fU*fDY);
	}

	// ok
	return(TRUE);
}

// AddTexture()

DWORD AddTexture(DWORD dwWidth,
				 DWORD dwHeight,
				 BOOL bTransparent,
				 BOOL bTranslucent,
				 LPSTR lpName,
				 DWORD dwMipMapLevels)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~TX%d",g_dwNumTex);
		return(AddTexture(dwWidth,dwHeight,bTransparent,bTranslucent,g_lpMsg,dwMipMapLevels));
	}

	// get next texture index
	DWORD dwIndex=GetNextTexture();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpTexNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpTexNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// set format
	D3DFORMAT fmt;
	if(bTranslucent)
		fmt=D3DFMT_A4R4G4B4;
	else
	{
		if(bTransparent)
			fmt=D3DFMT_A1R5G5B5;
		else
			fmt=D3DFMT_R5G6B5;
	}

	// clip size if necessary
	if(dwWidth>g_lpDevCaps.MaxTextureWidth)
		dwWidth=g_lpDevCaps.MaxTextureWidth;
	if(dwHeight>g_lpDevCaps.MaxTextureHeight)
		dwHeight=g_lpDevCaps.MaxTextureHeight;

	// used for error checking
	HRESULT hr;

	// create texture
	hr=g_lpD3DDevice->CreateTexture(dwWidth,
									dwHeight,
									dwMipMapLevels,
									NULL,
									fmt,
									D3DPOOL_MANAGED,
									&g_lplpTex[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create texture.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// return index
	return(dwIndex);
}

// BeginRenderToTexture()

BOOL BeginRenderToTexture(DWORD dwIndex,
						  float fClipNear,
						  float fClipFar,
						  float fAngle)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "BeginRenderToTexture()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// save projection matrix
	CopyMemory(&g_mtxRTSProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));
	
	// set projection matrix
	SetProjectionMatrixA(fClipNear,
						 fClipFar,
						 fAngle,
						 1);
	
	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=g_lplpTex[dwIndex]->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to read texture description.",
								   NULL,
								   hr);
		return(FALSE);
	}

	// save render size
	g_fRTSRenderX=g_fCurRenderX;
	g_fRTSRenderY=g_fCurRenderY;

	// set new render size
	g_fCurRenderX=(float)sd.Width;
	g_fCurRenderY=(float)sd.Height;
	
	// create surface mapper
	hr=D3DXCreateRenderToSurface(g_lpD3DDevice,
								 sd.Width,
								 sd.Height,
								 sd.Format,
								 TRUE,
								 D3DFMT_D16,
								 &g_lpRTS);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create surface mapper.",
								   "Your display adapter may not support rendering to textures.",
								   hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// get texture surface
	IDirect3DSurface8* lpSurf;
	hr=g_lplpTex[dwIndex]->GetSurfaceLevel(0,
										   &lpSurf);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to retrieve texture surface.",
								   NULL,
								   hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// prepare viewport
	D3DVIEWPORT8 vp;
	vp.X=0;
	vp.Y=0;
	vp.Width=sd.Width;
	vp.Height=sd.Height;
	vp.MinZ=0;
	vp.MaxZ=1;
	
	// begin scene
	hr=g_lpRTS->BeginScene(lpSurf,
						   &vp);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to initiate surface mapper.",
								   "Your display adapter may not support rendering to textures.",
								   hr);
		return(FALSE);
	}

	// release surface
	ReleaseCOMPtr(lpSurf);

	// set scene flag
	g_bInScene=TRUE;
	
	// ok
	return(TRUE);
}

// EndRenderToTexture()

void EndRenderToTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "EndRenderToTexture()",
								 FALSE);
		return;
	}
#endif
	
	// end scene
	HRESULT hr=g_lpRTS->EndScene();

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to terminate surface mapper.",
								   "Your display adapter may not support rendering to textures.",
								   hr);
		return;
	}
	
	// release surface mapper
	ReleaseCOMPtr(g_lpRTS);

	// restore projection matrix
	SetProjectionMatrixA(&g_mtxRTSProj);

	// restore render size
	g_fCurRenderX=g_fRTSRenderX;
	g_fCurRenderY=g_fRTSRenderY;

	// reset scene flag
	g_bInScene=FALSE;
}

#ifdef DEBUG_MODE
// CreateDDSFromTexture()

void CreateDDSFromTexture(LPCSTR lpDestFile,
						  DWORD dwIndex)
{
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "CreateDDSFromTexture()",
								 FALSE);
		return;
	}
	
	// used for error checking
	HRESULT hr;

	// output to DDS file
	hr=D3DXSaveTextureToFile(lpDestFile,
							 D3DXIFF_DDS,
							 g_lplpTex[dwIndex],
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to output texture.",
								   (LPSTR)lpDestFile,
								   hr);
		return;
	}
}
#endif

// AddAlphaToTexture()

BOOL AddAlphaToTexture(DWORD dwIndex,
					   float fAlpha)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "AddAlphaToTexture()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// add alpha
	return(AddAlphaToTexture(g_lplpTex[dwIndex],
							 fAlpha));
}

// AddAlphaToTexture()

BOOL AddAlphaToTexture(IDirect3DTexture8* lpTex,
					   float fAlpha)
{
	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read texture description.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// counters
	DWORD dwX,dwY;

	// lock texture surface
	D3DLOCKED_RECT lr;
	hr=lpTex->LockRect(0,
					   &lr,
					   NULL,
					   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to lock texture.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// get locked rect info
	DWORD dwPitch=lr.Pitch>>1;
	LPWORD lpData=(LPWORD)lr.pBits;

	// get texture size
	DWORD dwWidth=sd.Width;
	DWORD dwHeight=sd.Height;

	// check format
	switch(sd.Format)
	{
	case(D3DFMT_A4R4G4B4):
		// check alpha value
		if(fAlpha<0)
			for(dwY=0;dwY<dwHeight;dwY++)
				for(dwX=0;dwX<dwWidth;dwX++)
				{
					// time-savers
					DWORD p=dwX+(dwY*dwPitch);
					WORD c=lpData[p];

					// extract color components
					float fR=GetR444(c);
					float fG=GetG444(c);
					float fB=GetB444(c);
					
					// compute alpha
					float fA=(fR+fG+fB)/3;

					// output new value
					SetRGBA444(lpData[p],fR,fG,fB,fA);
				}
		else
			for(dwY=0;dwY<dwHeight;dwY++)
				for(dwX=0;dwX<dwWidth;dwX++)
				{
					// time-savers
					DWORD p=dwX+(dwY*dwPitch);
					WORD c=lpData[p];

					// extract color components
					float fR=GetR444(c);
					float fG=GetG444(c);
					float fB=GetB444(c);
					
					// output new value
					SetRGBA444(lpData[p],fR,fG,fB,fAlpha);
				}
		break;
	case(D3DFMT_A1R5G5B5):
		for(dwY=0;dwY<dwHeight;dwY++)
			for(dwX=0;dwX<dwWidth;dwX++)
			{
				// time-savers
				DWORD p=dwX+(dwY*dwPitch);
				WORD c=lpData[p];

				// reset alpha bit if necessary
				if(!(c&MASKRGB_1555))
					lpData[p]=0;
			}
		break;
	}

	// unlock texture surface
	hr=lpTex->UnlockRect(0);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to unlock texture.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ClearTexture()

void ClearTexture(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "ClearTexture()",
								 FALSE);
		return;
	}
#endif
	
	// clear texture
	ClearTexture(g_lplpTex[dwIndex]);
}

// ClearTexture()

void ClearTexture(IDirect3DTexture8* lpTex)
{
	// get surface description
	D3DSURFACE_DESC sd;
	HRESULT hr=lpTex->GetLevelDesc(0,&sd);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to read texture description.",
							  NULL,
							  hr);
		return;
	}

	// counters
	DWORD dwX,dwY;

	// lock texture surface
	D3DLOCKED_RECT lr;
	hr=lpTex->LockRect(0,
					   &lr,
					   NULL,
					   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to lock texture.",
							  NULL,
							  hr);
		return;
	}

	// get locked rect info
	DWORD dwPitch=lr.Pitch>>1;
	LPWORD lpData=(LPWORD)lr.pBits;

	// get texture size
	DWORD dwWidth=sd.Width;
	DWORD dwHeight=sd.Height;

	// clear all pixels	
	for(dwY=0;dwY<dwHeight;dwY++)
		for(dwX=0;dwX<dwWidth;dwX++)
			lpData[dwX+(dwY*dwPitch)]=NULL;

	// unlock texture surface
	hr=lpTex->UnlockRect(0);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to unlock texture.",
							  NULL,
							  hr);
		return;
	}
}

// InitFader()

void InitFader(void)
{
	// add vertex buffer
	g_dwFaderVtxBuf=AddVertexBuffer(D3DFVF_FADERVERTEX,
									sizeof(FADERVERTEX),
									4);

	// add index buffer
	g_dwFaderIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
								   2);

	// fill index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwFaderIdxBuf);
	lpIdx[0]=0;
	lpIdx[1]=1;
	lpIdx[2]=3;
	lpIdx[3]=2;
	UnlockIndexBuffer(g_dwFaderIdxBuf);
}

// ResetFaderVertexBuffer()

void ResetFaderVertexBuffer(D3DCOLOR clr)
{
	// lock buffer
	LPFADERVERTEX lpVtx=(LPFADERVERTEX)LockVertexBuffer(g_dwFaderVtxBuf);
	
	// set vertices
	SET_FADERVERTEX(lpVtx[0],
					0,0,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[1],
					g_fCurRenderX,0,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[2],
					0,g_fCurRenderY,1,1,
					clr);
	SET_FADERVERTEX(lpVtx[3],
					g_fCurRenderX,g_fCurRenderY,1,1,
					clr);

	// unlock buffer
	UnlockVertexBuffer(g_dwFaderVtxBuf);
}

// FadeScreen()

void FadeScreen(D3DCOLOR clr)
{
	// reset fader vertices
	ResetFaderVertexBuffer(clr);
	
	// save z-buffer state
	BOOL bZBufferEnabled=g_bZBufferEnabled;

	// disable z-buffer if necesary
	if(bZBufferEnabled)
		DisableZBuffer();
	
	// render overlay
	SetVertexBuffer(g_dwFaderVtxBuf);
	SetIndexBuffer(g_dwFaderIdxBuf);
	SetMaterial();
	SetTexture();
	RenderPrimitive();
	
	// enable z-buffer if necessary
	if(bZBufferEnabled)
		EnableZBuffer();
}

// GetNextMesh()

DWORD GetNextMesh(void)
{
	// check counter
	if(g_dwNumMesh==0)
	{
		// allocate memory		
		if(!AllocMem((LPVOID*)&g_lplpMesh,
					 sizeof(ID3DXMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumMeshSubsets,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo,
					 sizeof(LPMESHSUBSETINFO)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpMeshNames,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxMesh=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumMesh>=g_dwMaxMesh)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpMesh,
					  sizeof(ID3DXMesh*)*g_dwMaxMesh,
					  sizeof(ID3DXMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumMeshSubsets,
					  sizeof(DWORD)*g_dwMaxMesh,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMeshSubsetInfo,
					  sizeof(LPMESHSUBSETINFO)*g_dwMaxMesh,
					  sizeof(LPMESHSUBSETINFO)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpMeshNames,
					  sizeof(LPSTR)*g_dwMaxMesh,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxMesh+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumMesh;

	// increment counter
	g_dwNumMesh++;

	// return index
	return(dwIndex);
}

// AddMeshBox()

DWORD AddMeshBox(float fWidth,
				 float fHeight,
				 float fDepth,
				 DWORD dwMatIndex,
				 LPSTR lpName,
				 BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshBox(fWidth,fHeight,fDepth,dwMatIndex,g_lpMsg,bOptimize));
	}

	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create box mesh
	HRESULT hr=D3DXCreateBox(g_lpD3DDevice,
							 fWidth,
							 fHeight,
							 fDepth,
							 &g_lplpMesh[dwIndex],
							 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create box mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// OptimizeMesh()

void OptimizeMesh(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "OptimizeMesh()",
								 FALSE);
		return;
	}
#endif
	
	// used for error checking
	HRESULT hr;
	
	// get number of faces
	DWORD dwNumFaces=g_lplpMesh[dwIndex]->GetNumFaces();

	// allocate memory for adjacency
	LPDWORD lpdwAdjInfo;
	if(!AllocMem((LPVOID*)&lpdwAdjInfo,
				 sizeof(DWORD)*(dwNumFaces*3)))
		return;

	// get adjacency info
	hr=g_lplpMesh[dwIndex]->GenerateAdjacency(EPSILON,
											  lpdwAdjInfo);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to generate mesh adjacency info.",
							  "Check available memory.",
							  hr);
		FreeMem((LPVOID*)&lpdwAdjInfo);
		return;
	}
	
	// optimize mesh
	hr=g_lplpMesh[dwIndex]->OptimizeInplace(D3DXMESHOPT_COMPACT|
											D3DXMESHOPT_ATTRSORT|
											D3DXMESHOPT_VERTEXCACHE,
											lpdwAdjInfo,
											NULL,
											NULL,
											NULL);
	
	// check for attribute sort error and retry if necessary
	if(hr==D3DXERR_CANNOTATTRSORT)
		hr=g_lplpMesh[dwIndex]->OptimizeInplace(D3DXMESHOPT_COMPACT|
												D3DXMESHOPT_VERTEXCACHE,
												lpdwAdjInfo,
												NULL,
												NULL,
												NULL);
	
	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to optimize mesh.",
  							  "Check available memory.",
							  hr);
	}

	// release adjacency info
	FreeMem((LPVOID*)&lpdwAdjInfo);
}

// SetMeshSubsetInfo()

void SetMeshSubsetInfo(DWORD dwIndex,
					   DWORD dwSubset,
					   DWORD dwMatIndex,
					   DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "SetMeshSubsetInfo()",
								 FALSE);
		return;
	}
#endif
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset info
	lpMeshSubsetInfo[dwSubset].dwMat=dwMatIndex;
	lpMeshSubsetInfo[dwSubset].dwTex=dwTexIndex;
}

// AddMeshCylinder()

DWORD AddMeshCylinder(float fLowerRad,
					  float fUpperRad,
					  float fHeight,
					  DWORD dwNumSlices,
					  DWORD dwNumStacks,
					  DWORD dwMatIndex,
					  LPSTR lpName,
					  BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshCylinder(fLowerRad,fUpperRad,fHeight,dwNumSlices,dwNumStacks,dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create cylinder mesh
	HRESULT hr=D3DXCreateCylinder(g_lpD3DDevice,
								  fLowerRad,
								  fUpperRad,
								  fHeight,
								  dwNumSlices,
								  dwNumStacks,
								  &g_lplpMesh[dwIndex],
								  NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create cylinder mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshPolygon()

DWORD AddMeshPolygon(float fLengthOfSide,
					 DWORD dwNumSides,
					 DWORD dwMatIndex,
					 LPSTR lpName,
					 BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshPolygon(fLengthOfSide,dwNumSides,dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create polygon mesh
	HRESULT hr=D3DXCreatePolygon(g_lpD3DDevice,
								 fLengthOfSide,
								 dwNumSides,
								 &g_lplpMesh[dwIndex],
								 NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create polygon mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshSphere()

DWORD AddMeshSphere(float fRadius,
					DWORD dwNumSlices,
					DWORD dwNumStacks,
					DWORD dwMatIndex,
					LPSTR lpName,
					BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshSphere(fRadius,dwNumSlices,dwNumStacks,dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create sphere mesh
	HRESULT hr=D3DXCreateSphere(g_lpD3DDevice,
								fRadius,
								dwNumSlices,
								dwNumStacks,
								&g_lplpMesh[dwIndex],
								NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create sphere mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshTeapot()

DWORD AddMeshTeapot(DWORD dwMatIndex,
					LPSTR lpName,
					BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshTeapot(dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create teapot mesh
	HRESULT hr=D3DXCreateTeapot(g_lpD3DDevice,
								&g_lplpMesh[dwIndex],
								NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create teapot mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshText()

DWORD AddMeshText(DWORD dwFontIndex,
				  LPSTR lpText,
				  float fMaxDeviation,
				  float fExtrusion,
				  DWORD dwMatIndex,
				  LPSTR lpName,
				  BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshText(dwFontIndex,lpText,fMaxDeviation,fExtrusion,dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// begin text
	BeginText(dwFontIndex);
	
	// create text mesh
	HRESULT hr=D3DXCreateText(g_lpD3DDevice,
							  g_hFontDC,
							  lpText,
							  fMaxDeviation,
							  fExtrusion,
							  &g_lplpMesh[dwIndex],
							  NULL,
							  NULL);

	// end text
	EndText();

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create text mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMeshTorus()

DWORD AddMeshTorus(float fInnerRad,
				   float fOuterRad,
				   DWORD dwNumSides,
				   DWORD dwNumRings,
				   DWORD dwMatIndex,
				   LPSTR lpName,
				   BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMeshTorus(fInnerRad,fOuterRad,dwNumSides,dwNumRings,dwMatIndex,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// create torus mesh
	HRESULT hr=D3DXCreateTorus(g_lpD3DDevice,
							   fInnerRad,
							   fOuterRad,
							   dwNumSides,
							   dwNumRings,
							   &g_lplpMesh[dwIndex],
							   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create torus mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=1;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// fill in subset array
	lpMeshSubsetInfo[0].dwMat=dwMatIndex;
	lpMeshSubsetInfo[0].dwTex=NO_OBJECT;

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// AddMesh()

DWORD AddMesh(LPCSTR lpFile,
			  LPSTR lpName,
			  BOOL bOptimize)
{
	// create name if necessary
	if(!lpName)
	{
		sprintf(g_lpMsg,"~ME%d",g_dwNumMesh);
		return(AddMesh(lpFile,g_lpMsg,bOptimize));
	}
	
	// get return index
	DWORD dwIndex=GetNextMesh();

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	CopyMemory((LPVOID)g_lplpMeshNames[dwIndex],
			   (LPVOID)lpName,
			   strlen(lpName)+1);

	// set creation flags
	DWORD dwFlags=D3DXMESH_MANAGED;
	if(g_dwVPF==VPF_HARDWARE)
		dwFlags=dwFlags|D3DXMESH_USEHWONLY;
	if(g_dwVPF==VPF_SOFTWARE)
		dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

	// material buffer
	ID3DXBuffer* lpMaterials;
	DWORD dwNumMat;

	// used for error checking
	HRESULT hr;
	
	// check for data file
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// load mesh
	if(dfr.lpData)
		hr=D3DXLoadMeshFromXInMemory((LPBYTE)dfr.lpData,
									 dfr.dwSize,
									 dwFlags,
									 g_lpD3DDevice,
									 NULL,
									 &lpMaterials,
									 &dwNumMat,
									 &g_lplpMesh[dwIndex]);
	else
		hr=D3DXLoadMeshFromX((LPSTR)lpFile,
							  dwFlags,
							  g_lpD3DDevice,
							  NULL,
							  &lpMaterials,
							  &dwNumMat,
							  &g_lplpMesh[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to load mesh file.",
								   NULL,
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResources(2);

	// set number of subsets
	g_lpdwNumMeshSubsets[dwIndex]=dwNumMat;
	
	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwIndex]))
		return(NO_OBJECT);

	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwIndex];
	
	// materials buffer pointer
	LPD3DXMATERIAL lpMat=(LPD3DXMATERIAL)lpMaterials->GetBufferPointer();
	
	// fill in subset array
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumMat;dwCount++)
	{
		// update and copy material
		lpMat[dwCount].MatD3D.Ambient=lpMat[dwCount].MatD3D.Diffuse;
		lpMeshSubsetInfo[dwCount].dwMat=AddMaterial(&lpMat[dwCount].MatD3D);

		// load texture if needed
		if(lpMat[dwCount].pTextureFilename)
			lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);
		else
			lpMeshSubsetInfo[dwCount].dwTex=NO_OBJECT;
	}

	// release material buffer
	ReleaseCOMPtr(lpMaterials);

	// add normals to mesh
	AddMeshNormals(dwIndex);

	// optimize if requested
	if(bOptimize)
		OptimizeMesh(dwIndex);

	// return index
	return(dwIndex);
}

// RenderMesh()

void RenderMesh(DWORD dwMeshIndex,
				DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMeshIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "RenderMesh()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;
	
	// set world matrix
	SetWorldMatrix(dwWMtxIndex);
	
	// subset info pointer
	LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
	
	// render mesh
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		// set subset info
		SetMaterial(lpMeshSubsetInfo[dwCount].dwMat);
		SetTexture(lpMeshSubsetInfo[dwCount].dwTex);
		
		// render subset
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lplpMesh[dwMeshIndex]->DrawSubset(dwCount);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to render mesh subset.",
								  NULL,
								  hr);
#endif
	}

	// invalidate render values
	g_bInvalidRenderValues=TRUE;
}

// PurgeMeshes()

void PurgeMeshes(void)
{
	// check counter
	if(g_dwNumMesh)
	{
		// counter
		DWORD dwCount;

		// release all meshes data
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
		{
			ReleaseCOMPtr(g_lplpMesh[dwCount]);
			FreeMem((LPVOID*)&g_lplpMeshSubsetInfo[dwCount]);
			FreeMem((LPVOID*)&g_lplpMeshNames[dwCount]);
		}

		// free memory
		FreeMem((LPVOID*)&g_lplpMesh);
		FreeMem((LPVOID*)&g_lpdwNumMeshSubsets);
		FreeMem((LPVOID*)&g_lplpMeshSubsetInfo);
		FreeMem((LPVOID*)&g_lplpMeshNames);
		
		// reset counters
		g_dwNumMesh=0;
		g_dwMaxMesh=0;
	}
}

// GetMatrix()

void GetMatrix(DWORD dwIndex,
			   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "GetMatrix()",
								 FALSE);
		return;
	}
#endif

	// copy matrix
	CopyMemory(lpMtx,
			   &g_lpMtx[dwIndex],		   
			   sizeof(D3DXMATRIX));

	// scale matrix if needed
	if(!IsNearEqual(g_lpMtx[dwIndex].fXScale,1))
	{
		lpMtx->_11*=g_lpMtx[dwIndex].fXScale;
		lpMtx->_21*=g_lpMtx[dwIndex].fXScale;
		lpMtx->_31*=g_lpMtx[dwIndex].fXScale;
	}
	if(!IsNearEqual(g_lpMtx[dwIndex].fYScale,1))
	{
		lpMtx->_12*=g_lpMtx[dwIndex].fYScale;
		lpMtx->_22*=g_lpMtx[dwIndex].fYScale;
		lpMtx->_32*=g_lpMtx[dwIndex].fYScale;
	}
	if(!IsNearEqual(g_lpMtx[dwIndex].fZScale,1))
	{
		lpMtx->_13*=g_lpMtx[dwIndex].fZScale;
		lpMtx->_23*=g_lpMtx[dwIndex].fZScale;
		lpMtx->_33*=g_lpMtx[dwIndex].fZScale;
	}
}

// SetMatrix()

void SetMatrix(DWORD dwIndex,
			   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrix()",
								 FALSE);
		return;
	}
#endif

	// copy matrix
	CopyMemory(&g_lpMtx[dwIndex],
			   lpMtx,
			   sizeof(D3DXMATRIX));

	// extract scale values
	g_lpMtx[dwIndex].fXScale=Magnitude(MtxRightX,MtxRightY,MtxRightZ);
	g_lpMtx[dwIndex].fYScale=Magnitude(MtxUpX,   MtxUpY,   MtxUpZ);
	g_lpMtx[dwIndex].fZScale=Magnitude(MtxAheadX,MtxAheadY,MtxAheadZ);

	// scale matrix accordingly
	MtxRightX/=g_lpMtx[dwIndex].fXScale;
	MtxRightY/=g_lpMtx[dwIndex].fYScale;
	MtxRightZ/=g_lpMtx[dwIndex].fZScale;
	MtxUpX/=   g_lpMtx[dwIndex].fXScale;
	MtxUpY/=   g_lpMtx[dwIndex].fYScale;
	MtxUpZ/=   g_lpMtx[dwIndex].fZScale;
	MtxAheadX/=g_lpMtx[dwIndex].fXScale;
	MtxAheadY/=g_lpMtx[dwIndex].fYScale;
	MtxAheadZ/=g_lpMtx[dwIndex].fZScale;
}

// GetNextSkinMesh()

DWORD GetNextSkinMesh(void)
{
	// check counter
	if(g_dwNumSkin==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lplpSkin,
					 sizeof(ID3DXSkinMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwRenderMeshIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwOriginalMeshIndex,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumSkinBones,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpSkinBoneMtx,
					 sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxSkin=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumSkin>=g_dwMaxSkin)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lplpSkin,
					  sizeof(ID3DXSkinMesh*)*g_dwMaxSkin,
					  sizeof(ID3DXSkinMesh*)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwRenderMeshIndex,
					  sizeof(DWORD)*g_dwMaxSkin,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwOriginalMeshIndex,
					  sizeof(DWORD)*g_dwMaxSkin,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumSkinBones,
					  sizeof(DWORD)*g_dwMaxSkin,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpSkinBoneMtx,
					  sizeof(LPD3DXMATRIX)*g_dwMaxSkin,
					  sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxSkin+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumSkin;

	// increment counter
	g_dwNumSkin++;

	// return index
	return(dwIndex);
}

DWORD AddSkinMesh(DWORD dwMeshIndex,
				  DWORD dwNumBones,
				  BOOL bDefaultBones)
{
#ifdef DEBUG_MODE
	// verify mesh index
	if(dwMeshIndex>g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "AddSkinMesh()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// counter
	DWORD dwCount;

	// get return index
	DWORD dwIndex=GetNextSkinMesh();

	// save original mesh index
	g_lpdwOriginalMeshIndex[dwIndex]=dwMeshIndex;

	// create skin mesh interface
	HRESULT hr=D3DXCreateSkinMeshFromMesh(g_lplpMesh[dwMeshIndex],
										  dwNumBones,
										  &g_lplpSkin[dwIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create skinned mesh.",
								   "Check available memory.",
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set number of bones
	g_lpdwNumSkinBones[dwIndex]=dwNumBones;
	
	// allocate bone matrix array
	if(!AllocMem((LPVOID*)&g_lplpSkinBoneMtx[dwIndex],
				 sizeof(D3DXMATRIX)*g_lpdwNumSkinBones[dwIndex]))
		return(NO_OBJECT);

	// bone matrix pointer
	LPD3DXMATRIX lpBoneMtx=g_lplpSkinBoneMtx[dwIndex];
	
	// set all bone matrices to identity
	for(dwCount=0;dwCount<g_lpdwNumSkinBones[dwIndex];dwCount++)
		ResetMatrix(&lpBoneMtx[dwCount]);

	// check default bones flag
	if(bDefaultBones)
	{
		// get number of vertices
		DWORD dwNumVtx=g_lplpSkin[dwIndex]->GetNumVertices();

		// allocate vertex array
		LPDWORD lpdwVerts;
		if(!AllocMem((LPVOID*)&lpdwVerts,
					 sizeof(DWORD)*dwNumVtx))
			return(NO_OBJECT);

		// allocate influence array
		float* lpfInf;
		if(!AllocMem((LPVOID*)&lpfInf,
					 sizeof(float)*dwNumVtx))
			return(NO_OBJECT);

		// fill in vertex & influence arrays
		for(dwCount=0;dwCount<dwNumVtx;dwCount++)
		{
			lpdwVerts[dwCount]=dwCount;
			lpfInf[dwCount]=1;
		}

		// set bone influences
		hr=g_lplpSkin[dwIndex]->SetBoneInfluence(0,
												 dwNumVtx,
												 lpdwVerts,
												 lpfInf);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to set mesh bone influences.",
									   "Check available memory.",
									   hr);
			return(NO_OBJECT);
		}

		// fill in influence array
		for(dwCount=0;dwCount<dwNumVtx;dwCount++)
			lpfInf[dwCount]=0;

		// output remaining bone weights
		for(dwCount=1;dwCount<dwNumBones;dwCount++)
		{
			// set bone influences
			hr=g_lplpSkin[dwIndex]->SetBoneInfluence(dwCount,
													 dwNumVtx,
													 lpdwVerts,
													 lpfInf);

			// check for error
			if(FAILED(hr))
			{
				DisplayFatalDXErrorMessage("Unable to set mesh bone influences.",
										   "Check available memory.",
										   hr);
				return(NO_OBJECT);
			}
		}

		// release arrays
		FreeMem((LPVOID*)&lpdwVerts);
		FreeMem((LPVOID*)&lpfInf);

		// generate skin mesh
		if(!FinalizeSkinMesh(dwIndex))
			return(NO_OBJECT);
	}

	// return index
	return(dwIndex);
}

// SetSkinMeshSubsetInfo()

void SetSkinMeshSubsetInfo(DWORD dwIndex,
						   DWORD dwSubset,
						   DWORD dwMatIndex,
						   DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "SetSkinMeshSubsetInfo()",
								 FALSE);
		return;
	}
#endif
	
	// subset info pointer
	LPMESHSUBSETINFO lpSkinSubsetInfo=g_lplpMeshSubsetInfo[g_lpdwRenderMeshIndex[dwIndex]];
	
	// fill in subset info
	lpSkinSubsetInfo[dwSubset].dwMat=dwMatIndex;
	lpSkinSubsetInfo[dwSubset].dwTex=dwTexIndex;
}

// SetSkinMeshBoneMtx()

void SetSkinMeshBoneMtx(DWORD dwSkinIndex,
					    DWORD dwBoneNum,
					    DWORD dwMtxIndex)
{
	// check for reference bone
	if(dwBoneNum==NO_OBJECT)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "SetSkinMeshBoneMtx()",
								 FALSE);
		return;
	}

	// verify bone number
	if(dwBoneNum>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "SetSkinMeshBoneMtx()",
								 FALSE);
		return;
	}
#endif
	
	// bone matrix pointer
	LPD3DXMATRIX lpBoneMtx=g_lplpSkinBoneMtx[dwSkinIndex];

	// get matrix
	GetMatrix(dwMtxIndex,
			  &lpBoneMtx[dwBoneNum]);
}

// SetSkinMeshBoneMtx()

void SetSkinMeshBoneMtx(DWORD dwSkinIndex,
					    DWORD dwBoneNum,
					    LPD3DXMATRIX lpMtx)
{
	// check for reference bone
	if(dwBoneNum==NO_OBJECT)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "SetSkinMeshBoneMtx()",
								 FALSE);
		return;
	}

	// verify bone number
	if(dwBoneNum>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "SetSkinMeshBoneMtx()",
								 FALSE);
		return;
	}
#endif
	
	// bone matrix pointer
	LPD3DXMATRIX lpBoneMtx=g_lplpSkinBoneMtx[dwSkinIndex];

	// copy matrix
	CopyMemory(&lpBoneMtx[dwBoneNum],
			   lpMtx,
			   sizeof(D3DXMATRIX));
}

// RenderSkinMesh()

void RenderSkinMesh(DWORD dwSkinIndex,
					DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMesh()",
								 FALSE);
		return;
	}
#endif
	
	// update output mesh
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lplpSkin[dwSkinIndex]->UpdateSkinnedMesh(g_lplpSkinBoneMtx[dwSkinIndex],
											   NULL,
											   g_lplpMesh[g_lpdwRenderMeshIndex[dwSkinIndex]]);
#ifdef DEBUG_MODE
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to update skinned mesh.",
							  NULL,
							  hr);
		return;
	}
#endif
	
	// render mesh
	RenderMesh(g_lpdwRenderMeshIndex[dwSkinIndex],
			   dwWMtxIndex);
}

// PurgeSkinMeshes()

void PurgeSkinMeshes(void)
{
	// check counter
	if(g_dwNumSkin)
	{
		// counter
		DWORD dwCount;

		// release all meshes & subset info arrays
		for(dwCount=0;dwCount<g_dwNumSkin;dwCount++)
		{
			ReleaseCOMPtr(g_lplpSkin[dwCount]);
			FreeMem((LPVOID*)&g_lplpSkinBoneMtx[dwCount]);
		}

		// free memory
		FreeMem((LPVOID*)&g_lplpSkin);
		FreeMem((LPVOID*)&g_lpdwRenderMeshIndex);
		FreeMem((LPVOID*)&g_lpdwOriginalMeshIndex);
		FreeMem((LPVOID*)&g_lpdwNumSkinBones);
		FreeMem((LPVOID*)&g_lplpSkinBoneMtx);
		
		// reset counters
		g_dwNumSkin=0;
		g_dwMaxSkin=0;
	}
}

// LockBoneWeightsBuffer

LPLOCKEDBONEWEIGHTS LockBoneWeightsBuffer(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "LockBoneWeightsBuffer()",
								 FALSE);
		return(NULL);
	}
#endif
	
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// pointer to return
	LPLOCKEDBONEWEIGHTS lpbw;

	// allocate bone weights structure
	if(!AllocMem((LPVOID*)&lpbw,
				 sizeof(LOCKEDBONEWEIGHTS)))
		return(NULL);

	// get number of vertices in mesh
	lpbw->dwNumVtx=g_lplpSkin[dwIndex]->GetNumVertices();

	// allocate vertex array
	if(!AllocMem((LPVOID*)&lpbw->lpVtx,
				 sizeof(D3DXVECTOR3)*lpbw->dwNumVtx))
		return(NULL);

	// get number of bones in mesh
	lpbw->dwNumBones=g_lplpSkin[dwIndex]->GetNumBones();

	// allocate bones array
	if(!AllocMem((LPVOID*)&lpbw->lplpWeights,
				 sizeof(float*)*lpbw->dwNumBones))
		return(NULL);

	// allocate weight arrays
	for(dwCount=0;dwCount<lpbw->dwNumBones;dwCount++)
		if(!AllocMem((LPVOID*)&lpbw->lplpWeights[dwCount],
					 sizeof(float)*lpbw->dwNumVtx))
			return(NULL);

	// compute size of vertex
	lpbw->dwVtxSize=ComputeVertexSize(g_lplpSkin[dwIndex]->GetFVF())/sizeof(float);

	// lock vertex buffer
	hr=g_lplpSkin[dwIndex]->LockVertexBuffer(NULL,
											 (LPBYTE*)&lpbw->lpfVtxBuf);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to lock skin mesh vertex buffer.",
								   "Check available memory.",
								   hr);
		return(NULL);
	}
	
	// read vertices
	for(dwCount=0;dwCount<lpbw->dwNumVtx;dwCount++)
	{
		lpbw->lpVtx[dwCount].x=lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)];
		lpbw->lpVtx[dwCount].y=lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)+1];
		lpbw->lpVtx[dwCount].z=lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)+2];
	}

	// allocate vertex index array
	if(!AllocMem((LPVOID*)&lpbw->lpdwVtxIndex,
				 sizeof(DWORD)*lpbw->dwNumVtx))
		return(NULL);

	// prepare vertex index array
	for(dwCount=0;dwCount<lpbw->dwNumVtx;dwCount++)
		lpbw->lpdwVtxIndex[dwCount]=dwCount;
	
	// get bone weights
	for(dwCount=0;dwCount<lpbw->dwNumBones;dwCount++)
	{
		// get weights
		hr=g_lplpSkin[dwIndex]->GetBoneInfluence(dwCount,
												 lpbw->lpdwVtxIndex,
												 lpbw->lplpWeights[dwCount]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to retrieve skin mesh bone weights.",
									   "Check available memory.",
									   hr);
			return(NULL);
		}
	}

	// return pointer
	return(lpbw);
}

// UnlockBoneWeightsBuffer()

void UnlockBoneWeightsBuffer(DWORD dwIndex,
							 LPLOCKEDBONEWEIGHTS lpbw,
							 BOOL bFinalMesh)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "UnlockBoneWeightsBuffer()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// set bone weights
	for(dwCount=0;dwCount<lpbw->dwNumBones;dwCount++)
	{
		// get weights
		hr=g_lplpSkin[dwIndex]->SetBoneInfluence(dwCount,
												 lpbw->dwNumVtx,
												 lpbw->lpdwVtxIndex,
												 lpbw->lplpWeights[dwCount]);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to set skin mesh bone weights.",
									   "Check available memory.",
									   hr);
			return;
		}
	}
	
	// replace vertices
	for(dwCount=0;dwCount<lpbw->dwNumVtx;dwCount++)
	{
		lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)]  =lpbw->lpVtx[dwCount].x;
		lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)+1]=lpbw->lpVtx[dwCount].y;
		lpbw->lpfVtxBuf[(lpbw->dwVtxSize*dwCount)+2]=lpbw->lpVtx[dwCount].z;
	}

	// unlock vertex buffer
	hr=g_lplpSkin[dwIndex]->UnlockVertexBuffer();

	// check for error
	if(FAILED(hr))
		DisplayFatalDXErrorMessage("Unable to unlock skin mesh vertex buffer.",
								   NULL,
								   hr);

	// free weight arrays
	for(dwCount=0;dwCount<lpbw->dwNumBones;dwCount++)
		FreeMem((LPVOID*)&lpbw->lplpWeights[dwCount]);

	// free memory objects
	FreeMem((LPVOID*)&lpbw->lpdwVtxIndex);
	FreeMem((LPVOID*)&lpbw->lplpWeights);
	FreeMem((LPVOID*)&lpbw->lpVtx);
	FreeMem((LPVOID*)&lpbw);

	// generate final mesh if necessary
	if(bFinalMesh)
		FinalizeSkinMesh(dwIndex);
}

// FinalizeSkinMesh()

BOOL FinalizeSkinMesh(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "FinalizeSkinMesh()",
								 FALSE);
		return(FALSE);
	}
#endif
	
	// get original mesh index
	DWORD dwMeshIndex=g_lpdwOriginalMeshIndex[dwIndex];

	// set creation flags
	DWORD dwFlags=D3DXMESH_SYSTEMMEM;
	if(g_dwVPF==VPF_HARDWARE)
		dwFlags=dwFlags|D3DXMESH_USEHWONLY;
	if(g_dwVPF==VPF_SOFTWARE)
		dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

	// get number of faces
	DWORD dwNumFaces=g_lplpMesh[dwMeshIndex]->GetNumFaces();

	// allocate memory for adjacency
	LPDWORD lpdwAdjInfo;
	if(!AllocMem((LPVOID*)&lpdwAdjInfo,
			     sizeof(DWORD)*(dwNumFaces*3)))
		return(FALSE);

	// get adjacency info
	HRESULT hr=g_lplpMesh[dwMeshIndex]->GenerateAdjacency(EPSILON,
														  lpdwAdjInfo);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to generate mesh adjacency info.",
							  "Check available memory.",
							  hr);
		return(FALSE);
	}

	// get new mesh
	dwMeshIndex=GetNextMesh();
	g_lpdwRenderMeshIndex[dwIndex]=dwMeshIndex;
	
	// generate render mesh
	hr=g_lplpSkin[dwIndex]->GenerateSkinnedMesh(dwFlags,
												EPSILON,
												lpdwAdjInfo,
												NULL,
												NULL,
												NULL,
												&g_lplpMesh[dwMeshIndex]);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to generate skinned mesh.",
								   NULL,
								   hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// free adjacency info
	FreeMem((LPVOID*)&lpdwAdjInfo);

	// set number of subsets
	g_lpdwNumMeshSubsets[dwMeshIndex]=g_lpdwNumMeshSubsets[dwIndex];

	// allocate subset array
	if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwMeshIndex],
				 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwMeshIndex]))
		return(FALSE);

	// subset info pointers
	LPMESHSUBSETINFO lpSI1=g_lplpMeshSubsetInfo[dwMeshIndex];
	LPMESHSUBSETINFO lpSI2=g_lplpMeshSubsetInfo[dwIndex];
	
	// copy subset array
	DWORD dwCount;
	for(dwCount=0;dwCount<g_lpdwNumMeshSubsets[dwMeshIndex];dwCount++)
	{
		lpSI1[dwCount].dwMat=lpSI2[dwCount].dwMat;
		lpSI1[dwCount].dwTex=lpSI2[dwCount].dwTex;
	}

	// ok
	return(TRUE);
}

#ifdef DEBUG_MODE
// InitLineRender()

void InitLineRender(void)
{
	// add line material
	g_dwLineMat=AddMaterial(0,0,0,1,
							0,0,0,1,
							0,0,0,1,
							1,1,1,1,
							0);
	
	// create line vertex buffer
	g_dwLineVtxBuf=AddVertexBuffer(D3DFVF_LINEVERTEX,
								   sizeof(LINEVERTEX),
								   2);

	// create line index buffer
	g_dwLineIdxBuf=AddIndexBuffer(D3DPT_LINELIST,
								  1);

	// fill in index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwLineIdxBuf);
	lpIdx[0]=0;
	lpIdx[1]=1;
	UnlockIndexBuffer(g_dwLineIdxBuf);
}

// RenderLine()

void RenderLine(float fX1,
				float fY1,
				float fZ1,
				float fX2,
				float fY2,
				float fZ2,
				D3DCOLOR clr)
{
	// reset world matrix
	SetWorldMatrix();
	
	// render line
	RenderTransformedLine(fX1,fY1,fZ1,fX2,fY2,fZ1,clr);
}

// RenderLine()

void RenderLine(LPD3DXVECTOR3 lpV1,
				LPD3DXVECTOR3 lpV2,
				D3DCOLOR clr)
{
	// reset world matrix
	SetWorldMatrix();
	
	// render line
	RenderTransformedLine(lpV1->x,
						  lpV1->y,
						  lpV1->z,
						  lpV2->x,
						  lpV2->y,
						  lpV2->z,
						  clr);
}

// RenderTransformedLine()

void RenderTransformedLine(float fX1,
						   float fY1,
						   float fZ1,
						   float fX2,
						   float fY2,
						   float fZ2,
						   D3DCOLOR clr)
{
	// fill in vertex buffer
	LPLINEVERTEX lpVtx=(LPLINEVERTEX)LockVertexBuffer(g_dwLineVtxBuf);
	SET_LINEVERTEX(lpVtx[0],fX1,fY1,fZ1);
	SET_LINEVERTEX(lpVtx[1],fX2,fY2,fZ2);
	UnlockVertexBuffer(g_dwLineVtxBuf);

	// render line
	SetMaterial(g_dwLineMat);
	SetTexture();
	SetVertexBuffer(g_dwLineVtxBuf);
	SetIndexBuffer(g_dwLineIdxBuf);
	RenderPrimitive();
}

// RenderTransformedLine()

void RenderTransformedLine(LPD3DXVECTOR3 lpV1,
						   LPD3DXVECTOR3 lpV2,
						   D3DCOLOR clr)
{
	// render line
	RenderTransformedLine(lpV1->x,
						  lpV1->y,
						  lpV1->z,
						  lpV2->x,
						  lpV2->y,
						  lpV2->z,
						  clr);
}
#endif

// SetViewMatrix()

void SetViewMatrix(float fPosX,
				   float fPosY,
				   float fPosZ,
				   float fTargetX,
				   float fTargetY,
				   float fTargetZ,
				   float fUpX,
				   float fUpY,
				   float fUpZ)
{
	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vTarget=D3DXVECTOR3(fTargetX,fTargetY,fTargetZ);
	D3DXVECTOR3 vUp=D3DXVECTOR3(fUpX,fUpY,fUpZ);
	
	// prepare view matrix
	D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView,
					   &vPos,
					   &vTarget,
					   &vUp);

	// set view matrix
	SetViewMatrix(&mView);
}

// SetProjectionMatrix()

void SetProjectionMatrix(float fClipNear,
						 float fClipFar,
						 float fAngle,
						 float fAspect)
{
	// prepare projection matrix
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
							   DegToRad(fAngle),
							   fAspect,
							   fClipNear,
							   fClipFar);
	
	// set projection matrix
	SetProjectionMatrix(AddMatrix(&mProj));
}

// AddBone()

DWORD AddBone(float fPosX,
			  float fPosY,
			  float fPosZ,
			  float fRightX,
			  float fRightY,
			  float fRightZ,
			  float fUpX,
			  float fUpY,
			  float fUpZ,
			  float fAheadX,
			  float fAheadY,
			  float fAheadZ,
			  LPSTR lpName,
			  DWORD dwBonePoseIdx,
			  DWORD dwNumChildren)
{
	// prepare matrix
	D3DXMATRIX mBone=D3DXMATRIX(fRightX,fRightY,fRightZ,0,
								fUpX,   fUpY,   fUpZ,   0,
								fAheadX,fAheadY,fAheadZ,0,
								fPosX,  fPosY,  fPosZ,  1);

	// add bone
	return(AddBone(&mBone,
				   lpName,		   
				   dwBonePoseIdx,
				   dwNumChildren));
}

// AddBone()

DWORD AddBone(float fPosX,
			  float fPosY,
			  float fPosZ,
			  float fTargetX,
			  float fTargetY,
			  float fTargetZ,
			  float fUpX,
			  float fUpY,
			  float fUpZ,
			  LPSTR lpName,
			  DWORD dwBonePoseIdx,
			  DWORD dwNumChildren)
{
	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vTarget=D3DXVECTOR3(fTargetX,fTargetY,fTargetZ);
	D3DXVECTOR3 vUp=D3DXVECTOR3(fUpX,fUpY,fUpZ);
	
	// prepare matrix
	D3DXMATRIX mBone;
	D3DXMatrixLookAtLH(&mBone,
					   &vPos,
					   &vTarget,
					   &vUp);
	D3DXMatrixInverse(&mBone,
					  NULL,
					  &mBone);

	// add bone
	return(AddBone(&mBone,
				   lpName,
				   dwBonePoseIdx,
				   dwNumChildren));
}

// AddBone()

DWORD AddBone(LPD3DXMATRIX lpMtx,
			  LPSTR lpName,
			  DWORD dwBonePoseIdx,
			  DWORD dwNumChildren)
{
	// add matrix
	DWORD dwIndex=AddMatrix(lpMtx);

	// add bone
	return(AddBone(dwIndex,
				   lpName,
				   dwBonePoseIdx,
				   dwNumChildren));
}

// GetNextBone()

DWORD GetNextBone(void)
{
	// check pointer
	if(g_dwNumBone==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwBoneMtxIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwNumBoneChildren,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpdwBoneChildren,
					 sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpdwBonePoseIdx,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lpOrigBoneMtxInv,
					 sizeof(D3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpBoneName,
					 sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter	
		g_dwMaxBone=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumBone>=g_dwMaxBone)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwBoneMtxIdx,
					  sizeof(DWORD)*g_dwMaxBone,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwNumBoneChildren,
					  sizeof(DWORD)*g_dwMaxBone,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpdwBoneChildren,
					  sizeof(LPDWORD)*g_dwMaxBone,
					  sizeof(LPDWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpdwBonePoseIdx,
					  sizeof(DWORD)*g_dwMaxBone,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lpOrigBoneMtxInv,
					  sizeof(D3DXMATRIX)*g_dwMaxBone,
					  sizeof(D3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpBoneName,
					  sizeof(LPSTR)*g_dwMaxBone,
					  sizeof(LPSTR)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		
		// increment counter
		g_dwMaxBone+=BLOCK_ALLOC_INC;
	}
		
	// get return index
	DWORD dwIndex=g_dwNumBone;

	// increment counter
	g_dwNumBone++;

	// return index
	return(dwIndex);
}

// AddBone()

DWORD AddBone(DWORD dwMtxIndex,
			  LPSTR lpName,
			  DWORD dwBonePoseIdx,
			  DWORD dwNumChildren)
{
#ifdef DEBUG_MODE
	// verify matrix index
	if(dwMtxIndex>g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "AddBone()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// check for name
	if(!lpName)
	{
		// prepare name
		sprintf(g_lpMsg,
				"Unknown%d",
				g_dwNumBone);

		// re-call function
		return(AddBone(dwMtxIndex,
					   g_lpMsg,
					   dwBonePoseIdx,
					   dwNumChildren));
	}

	// get return index
	DWORD dwIndex=GetNextBone();

	// save necessary values
	g_lpdwBoneMtxIdx[dwIndex]=dwMtxIndex;
	g_lpdwNumBoneChildren[dwIndex]=dwNumChildren;
	g_lpdwBonePoseIdx[dwIndex]=dwBonePoseIdx;
	
	// allocate child array if necessary
	if(dwNumChildren)
		if(!AllocMem((LPVOID*)&g_lplpdwBoneChildren[dwIndex],
					 sizeof(DWORD)*dwNumChildren))
			return(NO_OBJECT);

	// allocate memory for name
	if(!AllocMem((LPVOID*)&g_lplpBoneName[dwIndex],
				 strlen(lpName)+1))
		return(NO_OBJECT);

	// copy name
	strcpy(g_lplpBoneName[dwIndex],
		   lpName);
		
	// return index
	return(dwIndex);
}

// SetBoneChild()

void SetBoneChild(DWORD dwParentIndex,
				  DWORD dwChildNum,
				  DWORD dwChildIndex)
{
#ifdef DEBUG_MODE
	// verify indices
	if(dwParentIndex>=g_dwNumBone||
	   dwChildIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "SetBoneChild()",
								 FALSE);
		return;
	}
#endif

#ifdef DEBUG_MODE
	// verify child number
	if(dwChildNum>=g_lpdwNumBoneChildren[dwParentIndex])
	{
		DisplayFatalErrorMessage("Invalid bone child index.",
								 "SetBoneChild()",
								 FALSE);
		return;
	}
#endif
	
	// child array pointer
	LPDWORD lpChildren=g_lplpdwBoneChildren[dwParentIndex];

	// copy value
	lpChildren[dwChildNum]=dwChildIndex;
}

// AddBoneChildA()

void AddBoneChildA(DWORD dwParentIndex,
				   DWORD dwChildIndex)
{
#ifdef DEBUG_MODE
	// verify indices
	if(dwParentIndex>=g_dwNumBone||
	   dwChildIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "AddBoneChildA()",
								 FALSE);
		return;
	}
#endif
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwParentIndex];
	
	// allocate child array as needed
	if(!dwNumChildren)
	{
		if(!AllocMem((LPVOID*)&g_lplpdwBoneChildren[dwParentIndex],
					 sizeof(DWORD)))
			return;
	}
	else
	{
		if(!ExpandMem((LPVOID*)&g_lplpdwBoneChildren[dwParentIndex],
					  sizeof(DWORD)*dwNumChildren,
					  sizeof(DWORD)))
			return;
	}

	// increment number of chilren
	g_lpdwNumBoneChildren[dwParentIndex]++;

	// set bone child
	SetBoneChild(dwParentIndex,
				 dwNumChildren,
				 dwChildIndex);
}

// AddBoneChild()

DWORD AddBoneChild(DWORD dwParentIndex,
				   float fPosX,
				   float fPosY,
				   float fPosZ,
				   float fRightX,
				   float fRightY,
				   float fRightZ,
				   float fUpX,
				   float fUpY,
				   float fUpZ,
				   float fAheadX,
				   float fAheadY,
				   float fAheadZ,
				   LPSTR lpName,
				   DWORD dwBonePoseIdx,
				   DWORD dwNumChildren)
{
	// add new bone
	DWORD dwChildIndex=AddBone(fPosX,
							   fPosY,
							   fPosZ,
							   fRightX,
							   fRightY,
							   fRightZ,
							   fUpX,
							   fUpY,
							   fUpZ,
							   fAheadX,
							   fAheadY,
							   fAheadZ,
							   lpName,
							   dwBonePoseIdx,
							   dwNumChildren);

	// set bone child
	AddBoneChildA(dwParentIndex,
				  dwChildIndex);

	// return index of new bone
	return(dwChildIndex);
}

// AddBoneChild()

DWORD AddBoneChild(DWORD dwParentIndex,
				   float fPosX,
				   float fPosY,
				   float fPosZ,
				   float fTargetX,
				   float fTargetY,
				   float fTargetZ,
				   float fUpX,
				   float fUpY,
				   float fUpZ,
				   LPSTR lpName,
				   DWORD dwBonePoseIdx,
				   DWORD dwNumChildren)
{
	// add new bone
	DWORD dwChildIndex=AddBone(fPosX,
							   fPosY,
							   fPosZ,
							   fTargetX,
							   fTargetY,
							   fTargetZ,
							   fUpX,
							   fUpY,
							   fUpZ,
							   lpName,
							   dwBonePoseIdx,
							   dwNumChildren);

	// set bone child
	AddBoneChildA(dwParentIndex,
				  dwChildIndex);

	// return index of new bone
	return(dwChildIndex);
}

// AddBoneChild()

DWORD AddBoneChild(DWORD dwParentIndex,
				   LPD3DXMATRIX lpMtx,
				   LPSTR lpName,
				   DWORD dwBonePoseIdx,
				   DWORD dwNumChildren)
{
	// add new bone
	DWORD dwChildIndex=AddBone(lpMtx,
							   lpName,
							   dwBonePoseIdx,
							   dwNumChildren);

	// set bone child
	AddBoneChildA(dwParentIndex,
				  dwChildIndex);

	// return index of new bone
	return(dwChildIndex);
}

// AddBoneChild()

DWORD AddBoneChild(DWORD dwParentIndex,
				   DWORD dwMtxIndex,
				   LPSTR lpName,
				   DWORD dwBonePoseIdx,
				   DWORD dwNumChildren)
{
	// add new bone
	DWORD dwChildIndex=AddBone(dwMtxIndex,
							   lpName,
							   dwBonePoseIdx,
							   dwNumChildren);

	// add bone child
	AddBoneChildA(dwParentIndex,
				  dwChildIndex);

	// return index of new bone
	return(dwChildIndex);
}

// PurgeBones()

void PurgeBones(void)
{
	// check counter
	if(g_dwNumBone)
	{
		// counter
		DWORD dwCount;

		// release all child arrays & names
		for(dwCount=0;dwCount<g_dwNumBone;dwCount++)
		{
			if(g_lpdwNumBoneChildren[dwCount])
				FreeMem((LPVOID*)&g_lplpdwBoneChildren[dwCount]);
			FreeMem((LPVOID*)&g_lplpBoneName[dwCount]);
		}

		// free memory
		FreeMem((LPVOID*)&g_lpdwBoneMtxIdx);
		FreeMem((LPVOID*)&g_lpdwNumBoneChildren);
		FreeMem((LPVOID*)&g_lplpdwBoneChildren);
		FreeMem((LPVOID*)&g_lpdwBonePoseIdx);
		FreeMem((LPVOID*)&g_lpOrigBoneMtxInv);
		FreeMem((LPVOID*)&g_lplpBoneName);

		// reset counters
		g_dwNumBone=0;
		g_dwMaxBone=0;
	}
}

// BoneYaw()

void BoneYaw(DWORD dwIndex,
			 float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneYaw()",
								 FALSE);
		return;
	}
#endif
	
	// apply yaw
	MatrixYaw(g_lpdwBoneMtxIdx[dwIndex],
			  fVal);
}

// BonePitch()

void BonePitch(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BonePitch()",
								 FALSE);
		return;
	}
#endif
	
	// apply pitch
	MatrixPitch(g_lpdwBoneMtxIdx[dwIndex],
			    fVal);
}

// BoneRoll()

void BoneRoll(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneRoll()",
								 FALSE);
		return;
	}
#endif
	
	// apply roll
	MatrixRoll(g_lpdwBoneMtxIdx[dwIndex],
			   fVal);
}

// BoneLeft()

void BoneLeft(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneLeft()",
								 FALSE);
		return;
	}
#endif
	
	// move left
	MatrixLeft(g_lpdwBoneMtxIdx[dwIndex],
			   fVal);
}

// BoneRight()

void BoneRight(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneRight()",
								 FALSE);
		return;
	}
#endif
	
	// move right
	MatrixRight(g_lpdwBoneMtxIdx[dwIndex],
			   fVal);
}

// BoneUp()

void BoneUp(DWORD dwIndex,
		    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneUp()",
								 FALSE);
		return;
	}
#endif
	
	// move up
	MatrixUp(g_lpdwBoneMtxIdx[dwIndex],
			 fVal);
}

// BoneDown()

void BoneDown(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneDown()",
								 FALSE);
		return;
	}
#endif
	
	// move down
	MatrixDown(g_lpdwBoneMtxIdx[dwIndex],
			   fVal);
}

// BoneAhead()

void BoneAhead(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneAhead()",
								 FALSE);
		return;
	}
#endif
	
	// move ahead
	MatrixAhead(g_lpdwBoneMtxIdx[dwIndex],
			    fVal);
}

// BoneBack()

void BoneBack(DWORD dwIndex,
			  float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneBack()",
								 FALSE);
		return;
	}
#endif
	
	// move back
	MatrixBack(g_lpdwBoneMtxIdx[dwIndex],
			   fVal);
}

// BoneXScale()

void BoneXScale(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneXScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale x
	MatrixXScale(g_lpdwBoneMtxIdx[dwIndex],
			     fVal);
}

// BoneYScale()

void BoneYScale(DWORD dwIndex,
			    float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneYScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale y
	MatrixYScale(g_lpdwBoneMtxIdx[dwIndex],
			     fVal);
}

// BoneZScale()

void BoneZScale(DWORD dwIndex,
				float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneZScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale z
	MatrixZScale(g_lpdwBoneMtxIdx[dwIndex],
			     fVal);
}

// BoneScale()

void BoneScale(DWORD dwIndex,
			   float fVal)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "BoneScale()",
								 FALSE);
		return;
	}
#endif
	
	// scale xyz
	MatrixScale(g_lpdwBoneMtxIdx[dwIndex],
			    fVal);
}

// FinalizeBoneStructure()

void FinalizeBoneStructure(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FinalizeBoneStructure()",
								 FALSE);
		return;
	}
#endif
	
	// get base matrix
	D3DXMATRIX mBase;
	GetMatrix(g_lpdwBoneMtxIdx[dwIndex],
			  &mBase);

	// translate top-level bone
	FinalizeBoneChild(dwIndex,
					  &mBase);
}

// FinalizeBoneChild()

void FinalizeBoneChild(DWORD dwIndex,
					   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FinalizeBoneChild()",
								 FALSE);
		return;
	}
#endif
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwIndex];

	// translate children as parent bones
	if(dwNumChildren)
	{
		// get new base matrix
		D3DXMATRIX mBase;
		GetMatrix(g_lpdwBoneMtxIdx[dwIndex],
				  &mBase);
		
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwIndex];

		// translate each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
			FinalizeBoneChild(lpdwChildren[dwCount],
							  &mBase);
	}

	// get original bone matrix
	D3DXMATRIX mOrig;
	GetMatrix(g_lpdwBoneMtxIdx[dwIndex],
			  &mOrig);

	// save inverse of original matrix
	D3DXMatrixInverse(&g_lpOrigBoneMtxInv[dwIndex],
					  NULL,
					  &mOrig);

	// invert base matrix
	D3DXMATRIX mInv;
	D3DXMatrixInverse(&mInv,
					  NULL,
					  lpMtx);
	
	// get matrix of this bone
	D3DXMATRIX mCur;
	GetMatrix(g_lpdwBoneMtxIdx[dwIndex],
			  &mCur);
	
	// translate bone
	D3DXMATRIX mBase;
	D3DXMatrixMultiply(&mBase,
					   &mCur,
					   &mInv);

	// set new bone matrix
	SetMatrix(g_lpdwBoneMtxIdx[dwIndex],
			  &mBase);
}

#ifdef DEBUG_MODE
// RenderBoneStructure()

void RenderBoneStructure(DWORD dwBoneIndex,
						 DWORD dwWMtxIndex,
						 float fBoneLength,
						 float fBlipSize,
						 BOOL bUseZBuffer,
						 D3DCOLOR clr)
{
	// verify bone index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneStructure()",
								 FALSE);
		return;
	}
	
	// verify matrix index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderBoneStructure()",
								 FALSE);
		return;
	}
	
	// disable z-buffer
	if(!bUseZBuffer)
		DisableZBuffer();
	
	// get base matrix
	D3DXMATRIX mBase;
	GetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
			  &mBase);
	
	// render top-level bone
	RenderBoneChild(dwBoneIndex,
					dwWMtxIndex,
					&mBase,
					fBoneLength,
					fBlipSize,
					clr);
	
	// enable z-buffer
	if(!bUseZBuffer)
		EnableZBuffer();
}

// RenderBoneChild()

void RenderBoneChild(DWORD dwBoneIndex,
					 DWORD dwWMtxIndex,
					 LPD3DXMATRIX lpMtx,
					 float fBoneLength,
					 float fBlipSize,
					 D3DCOLOR clr)
{
	// verify bone index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneChild()",
								 FALSE);
		return;
	}

	// verify matrix index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderBoneChild()",
								 FALSE);
		return;
	}

	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwBoneIndex];

	// render children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwBoneIndex];

		// render each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
		{
			// get matrix of child bone
			D3DXMATRIX mChild;
			GetMatrix(g_lpdwBoneMtxIdx[lpdwChildren[dwCount]],
					  &mChild);
			

			// translate base matrix
			D3DXMATRIX mBase;
			D3DXMatrixMultiply(&mBase,
							   &mChild,
							   lpMtx);
			
			// render child
			RenderBoneChild(lpdwChildren[dwCount],
							dwWMtxIndex,
							&mBase,
							fBoneLength,
							fBlipSize,
							clr);
		}
	}

	// get world matrix
	D3DXMATRIX mWorld;
	GetMatrix(dwWMtxIndex,
			  &mWorld);

	// translate base matrix
	D3DXMATRIX mBase;
	D3DXMatrixMultiply(&mBase,
					   lpMtx,
					   &mWorld);
					   

	// render bone
	SetWorldMatrix(&mBase);
	RenderTransformedLine(0,0,-fBlipSize,
						  0,0,fBoneLength+fBlipSize,
						  clr);
	RenderTransformedLine(-fBlipSize,0,0,
						  fBlipSize,0,0,
						  clr);
	RenderTransformedLine(-fBlipSize,0,fBoneLength,
						  fBlipSize,0,fBoneLength,
						  clr);
	RenderTransformedLine(0,-fBlipSize,0,
						  0,fBlipSize,0,
						  clr);
	RenderTransformedLine(0,-fBlipSize,fBoneLength,
						  0,fBlipSize,fBoneLength,
						  clr);
}

// RenderBoneStructure()

void RenderBoneStructure(DWORD dwBoneIndex,
						 DWORD dwFontIndex)
{
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneStructure()",
								 FALSE);
		return;
	}

	// verify index
	if(dwFontIndex>=g_dwNumFont)
	{
		DisplayFatalErrorMessage("Invalid font index.",
								 "RenderBoneStructure()",
								 FALSE);
		return;
	}
	
	// reset output
	RenderBoneChildInfo();
	
	// begin text
	BeginText(dwFontIndex);
	
	// render top-level bone
	RenderBoneChild(dwBoneIndex,
					0);
	
	// end text
	EndText();
}

// RenderBoneChild()

void RenderBoneChild(DWORD dwBoneIndex,
					 DWORD dwLevel)
{
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneChild()",
								 FALSE);
		return;
	}
	
	// render child info
	RenderBoneChildInfo(g_lplpBoneName[dwBoneIndex],
						dwLevel);
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwBoneIndex];

	// render children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwBoneIndex];

		// render each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
		{
			// render child
			RenderBoneChild(lpdwChildren[dwCount],
							dwLevel+1);
		}
	}
}

// RenderBoneChildInfo()

void RenderBoneChildInfo(LPSTR lpName,
						 DWORD dwLevel)
{
	// counter
	DWORD dwCount;
	
	// line counter
	static int nLine=0;
	
	// check for reset
	if(dwLevel==NO_OBJECT)
	{
		nLine=0;
		return;
	}

	// reset string
	strcpy(g_lpMsg," ");

	// output lines
	if(dwLevel)
		for(dwCount=0;dwCount<dwLevel;dwCount++)
			strcat(g_lpMsg,"| ");

	// output plus
	strcat(g_lpMsg,"+ ");

	// output name
	strcat(g_lpMsg,lpName);

	// render to display
	OutputText(g_lpMsg,
			   0,nLine);

	// increment line
	nLine+=16;
}
#endif

// SetCameraMatrix()

void SetCameraMatrix(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetCameraMatrix()",
								 FALSE);
		return;
	}
#endif

	// get view matrix
	D3DXMATRIX mView;
	GetMatrix(dwIndex,
			  &mView);

	// set billboard vectors
	D3DXVECTOR3 vAhead=D3DXVECTOR3(mView._31,
								   mView._32,
								   mView._33);
	SetBillboardVectors(&vAhead);

	// invert view matrix
	D3DXMATRIX mCam;
	D3DXMatrixInverse(&mCam,
					  NULL,
					  &mView);
	
	// set matrix
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTransform(D3DTS_VIEW,
								&mCam);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set camera matrix.",NULL,hr);
#endif
}

// RenderSkinMesh()

void RenderSkinMesh(DWORD dwSkinIndex,
					DWORD dwWMtxIndex,
					DWORD dwBaseBoneIndex)
{
#ifdef DEBUG_MODE
	// verify skin index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMesh()",
								 FALSE);
		return;
	}

	// verify matrix index
	if(dwWMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderSkinMesh()",
								 FALSE);
		return;
	}
#endif
	
	// prepare output mesh array
	TranslateBoneStructure(dwBaseBoneIndex,
						   dwSkinIndex);

	// render mesh
	RenderSkinMesh(dwSkinIndex,
				   dwWMtxIndex);
}

// TranslateBoneStructure()

void TranslateBoneStructure(DWORD dwBoneIndex,
							DWORD dwSkinIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "TranslateBoneStructure()",
								 FALSE);
		return;
	}

	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "TranslateBoneStructure()",
								 FALSE);
		return;
	}
#endif

	// get base matrix
	D3DXMATRIX mBase;
	GetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
			  &mBase);
	
	// translate top-level bone
	TranslateBoneChild(dwBoneIndex,
					   g_lplpSkinBoneMtx[dwSkinIndex],
					   &mBase);
}

// TranslateBoneStructure()

void TranslateBoneStructure(DWORD dwBoneIndex,
							LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "TranslateBoneStructure()",
								 FALSE);
		return;
	}
#endif

	// get base matrix
	D3DXMATRIX mBase;
	GetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
			  &mBase);
	
	// translate top-level bone
	TranslateBoneChild(dwBoneIndex,
					   lpMtx,
					   &mBase);
}

// TranslateBoneChild()

void TranslateBoneChild(DWORD dwBoneIndex,
						LPD3DXMATRIX lpMtxArray,
						LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "TranslateBoneChild()",
								 FALSE);
		return;
	}
#endif

	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwBoneIndex];

	// translate children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwBoneIndex];

		// translate each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
		{
			// get matrix of child bone
			D3DXMATRIX mChild;
			GetMatrix(g_lpdwBoneMtxIdx[lpdwChildren[dwCount]],
					  &mChild);

			// translate base matrix
			D3DXMATRIX mBase;
			D3DXMatrixMultiply(&mBase,
							   &mChild,
							   lpMtx);
			
			// translate child
			TranslateBoneChild(lpdwChildren[dwCount],
							   lpMtxArray,
							   &mBase);
		}
	}

	// check for reference bone
	if(g_lpdwBonePoseIdx[dwBoneIndex]==NO_OBJECT)
		return;
	
	// translate final matrix
	D3DXMATRIX mFinal;
	D3DXMatrixMultiply(&mFinal,
					   &g_lpOrigBoneMtxInv[dwBoneIndex],
					   lpMtx);

	// output final matrix
	CopyMemory(&lpMtxArray[g_lpdwBonePoseIdx[dwBoneIndex]],
			   &mFinal,
			   sizeof(D3DXMATRIX));
}

// AddSkinMesh()

DWORD AddSkinMesh(LPCSTR lpFile,
				  LPDWORD lpdwBoneIndex,
				  LPDWORD lpdwPoseIndex,
				  LPDWORD lpdwNumPoses)
{
	// used for error checking
	HRESULT hr;

	// check data file reference
	DATAFILEREF dfr=FindDataFileRef((LPSTR)lpFile);

	// create enumerator object
	IDirectXFileEnumObject* lpDXFileEnum;
	if(dfr.lpData)
	{
		// prepare structure
		DXFILELOADMEMORY flm;
		flm.lpMemory=dfr.lpData;
		flm.dSize=dfr.dwSize;
		
		// create enumerator
		hr=g_lpDXFile->CreateEnumObject((LPVOID)&flm,
										DXFILELOAD_FROMMEMORY,
										&lpDXFileEnum);
	}
	else
		hr=g_lpDXFile->CreateEnumObject((LPVOID)lpFile,
										DXFILELOAD_FROMFILE,
										&lpDXFileEnum);

	// check for error 
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to create DirectX file enumerator object.",
								   NULL,
								   hr);
		return(NO_OBJECT);
	}
	else
		AddCOMResource;

	// set output indices
	DWORD dwSkinIndex=NO_OBJECT;
	*lpdwBoneIndex=NO_OBJECT;
	*lpdwPoseIndex=NO_OBJECT;
	*lpdwNumPoses=0;

	// process all top-level data objects
	IDirectXFileData* lpDXFileData;
	while(SUCCEEDED(lpDXFileEnum->GetNextDataObject(&lpDXFileData)))
	{
		// add resource
		AddCOMResource;

		// process data object
		LoadSkinMeshData(lpDXFileData,
						 &dwSkinIndex,
						 lpdwBoneIndex,
						 NULL,
						 lpdwPoseIndex,
						 lpdwNumPoses);
		
		// release data object
		ReleaseCOMPtr(lpDXFileData);
	}

	// release enumerator object
	ReleaseCOMPtr(lpDXFileEnum);
	
	// finalize bone structure
	if((*lpdwBoneIndex)!=NO_OBJECT)
		FinalizeBoneStructure(*lpdwBoneIndex);

	// finalize poses
	if((*lpdwPoseIndex)!=NO_OBJECT&&
	   (*lpdwNumPoses)!=0&&
	   (*lpdwBoneIndex)!=NO_OBJECT)
		FinalizePoses((*lpdwPoseIndex),
					  (*lpdwNumPoses),
					  (*lpdwBoneIndex));

	// return index
	return(dwSkinIndex);
}

// LoadSkinMeshData()

void LoadSkinMeshData(IDirectXFileData* lpDXFileData,
					  LPDWORD lpdwSkinIndex,
					  LPDWORD lpdwBoneIndex,
					  DWORD dwParentBoneIndex,
					  LPDWORD lpdwPoseIndex,
					  LPDWORD lpdwNumPoses)
{
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;

	// flag to ignore sub-data
	BOOL bIgnoreSubs=FALSE;

	// new bone index
	DWORD dwNewBoneIndex=dwParentBoneIndex;

	// animation data
	static LPVOID lpAnimData=NULL;

	// get template type
	const GUID* lpType;
	hr=lpDXFileData->GetType(&lpType);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to read DirectX file template type.",
								   NULL,
								   hr);
		return;
	}

	// get buffer size for object name
	DWORD dwSize;
	hr=lpDXFileData->GetName(NULL,
							 &dwSize);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to read DirectX file object name length.",
								   NULL,
								   hr);
		return;
	}

	// set name as needed
	LPSTR lpName;
	if(dwSize)
	{
		// allocate buffer
		if(!AllocMem((LPVOID*)&lpName,
				     dwSize))
			return;

		// get name
		hr=lpDXFileData->GetName(lpName,
								 &dwSize);

		// check for error
		if(FAILED(hr))
		{
			DisplayFatalDXErrorMessage("Unable to read DirectX file object name.",
									   NULL,
									   hr);
			return;
		}
	}
	else
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&lpName,
				     strlen("Unknown")+1))
			return;
		
		// set default name
		strcpy(lpName,"Unknown");
	}

	// process template
	if((*lpType)==TID_D3DRMMesh)
	{
		// load only one mesh
		if((*lpdwSkinIndex)==NO_OBJECT)
		{
			// save index
			DWORD dwSkinIndex=GetNextSkinMesh();
			*lpdwSkinIndex=dwSkinIndex;

			// set original mesh index
			g_lpdwOriginalMeshIndex[dwSkinIndex]=NO_OBJECT;

			// set creation flags
			DWORD dwFlags=D3DXMESH_MANAGED;
			if(g_dwVPF==VPF_HARDWARE)
				dwFlags=dwFlags|D3DXMESH_USEHWONLY;
			if(g_dwVPF==VPF_SOFTWARE)
				dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

			// material buffer
			ID3DXBuffer* lpMaterials;
			DWORD dwNumMat;

			// adjacency buffer
			ID3DXBuffer* lpAdjacency;

			// bone buffers
			ID3DXBuffer* lpBoneNames;
			ID3DXBuffer* lpBoneTransforms;

			// load skin mesh
			hr=D3DXLoadSkinMeshFromXof(lpDXFileData,
									   dwFlags,
									   g_lpD3DDevice,
									   &lpAdjacency,
									   &lpMaterials,
									   &dwNumMat,
									   &lpBoneNames,
									   &lpBoneTransforms,
									   &g_lplpSkin[dwSkinIndex]);

			// check for error
			if(FAILED(hr))
			{
				DisplayFatalDXErrorMessage("Unable to load skinned mesh.",
										   NULL,
										   hr);
				return;
			}
			else
				AddCOMResources(5);

			// verify bone buffers
			if(!lpBoneNames||
			   !lpBoneTransforms)
			{
				DisplayFatalDXErrorMessage("Unable to find bone hierarchy in mesh.",
										   NULL,
										   hr);
				return;
			}

			// get new mesh
			DWORD dwMeshIndex=GetNextMesh();
			g_lpdwRenderMeshIndex[dwSkinIndex]=dwMeshIndex;
			
			// set new creation flags
			dwFlags=D3DXMESH_SYSTEMMEM;
			if(g_dwVPF==VPF_HARDWARE)
				dwFlags=dwFlags|D3DXMESH_USEHWONLY;
			if(g_dwVPF==VPF_SOFTWARE)
				dwFlags=dwFlags|D3DXMESH_SOFTWAREPROCESSING;

			// generate render mesh
			hr=g_lplpSkin[dwSkinIndex]->GenerateSkinnedMesh(dwFlags,
															EPSILON,
															(LPDWORD)lpAdjacency->GetBufferPointer(),
															NULL,
															NULL,
															NULL,
															&g_lplpMesh[dwMeshIndex]);

			// check for error
			if(FAILED(hr))
			{
				DisplayFatalDXErrorMessage("Unable to generate skinned mesh.",
										   NULL,
										   hr);
				return;
			}
			else
				AddCOMResource;

			// free adjacency info
			ReleaseCOMPtr(lpAdjacency);

			// set number of subsets
			g_lpdwNumMeshSubsets[dwMeshIndex]=dwNumMat;
			
			// allocate subset array
			if(!AllocMem((LPVOID*)&g_lplpMeshSubsetInfo[dwMeshIndex],
						 sizeof(MESHSUBSETINFO)*g_lpdwNumMeshSubsets[dwMeshIndex]))
				return;

			// subset info pointer
			LPMESHSUBSETINFO lpMeshSubsetInfo=g_lplpMeshSubsetInfo[dwMeshIndex];
			
			// materials buffer pointer
			LPD3DXMATERIAL lpMat=(LPD3DXMATERIAL)lpMaterials->GetBufferPointer();
			
			// fill in subset array
			for(dwCount=0;dwCount<dwNumMat;dwCount++)
			{
				// update and copy material
				lpMat[dwCount].MatD3D.Ambient=lpMat[dwCount].MatD3D.Diffuse;
				lpMeshSubsetInfo[dwCount].dwMat=AddMaterial(&lpMat[dwCount].MatD3D);

				// load texture if needed
				if(lpMat[dwCount].pTextureFilename)
					lpMeshSubsetInfo[dwCount].dwTex=AddTexture(lpMat[dwCount].pTextureFilename);
				else
					lpMeshSubsetInfo[dwCount].dwTex=NO_OBJECT;
			}

			// release material buffer
			ReleaseCOMPtr(lpMaterials);

			// add normals to mesh
			AddMeshNormals(dwMeshIndex);

			// set number of bones
			g_lpdwNumSkinBones[dwSkinIndex]=g_lplpSkin[dwSkinIndex]->GetNumBones();
			
			// allocate bone matrix array
			if(!AllocMem((LPVOID*)&g_lplpSkinBoneMtx[dwSkinIndex],
						 sizeof(D3DXMATRIX)*g_lpdwNumSkinBones[dwSkinIndex]))
				return;

			// bone matrix pointer
			LPD3DXMATRIX lpBoneMtx=g_lplpSkinBoneMtx[dwSkinIndex];
			
			// set all bone matrices to identity
			for(dwCount=0;dwCount<g_lpdwNumSkinBones[dwSkinIndex];dwCount++)
				ResetMatrix(&lpBoneMtx[dwCount]);

			// pointer to bone matrices
			LPD3DXMATRIX lpMtx=(LPD3DXMATRIX)lpBoneTransforms->GetBufferPointer();

			// pointer to bone names
			LPSTR* lpNames=(LPSTR*)lpBoneNames->GetBufferPointer();

			// set bone transforms & pose indices
			for(dwCount=0;dwCount<g_lpdwNumSkinBones[dwSkinIndex];dwCount++)
			{
				// get bone index
				DWORD dwIndex;
				dwIndex=FindBone(lpNames[dwCount],
								 *lpdwBoneIndex);

				// invert bone matrix
				D3DXMATRIX mInv;
				D3DXMatrixInverse(&mInv,
								  NULL,
								  &lpMtx[dwCount]);

				// set bone matrix
				SetMatrix(g_lpdwBoneMtxIdx[dwIndex],
						  &mInv);
				
				// set pose index
				SetBonePoseIndex(dwIndex,
								 dwCount);
			}

			// release bone buffers
			ReleaseCOMPtr(lpBoneNames);
			ReleaseCOMPtr(lpBoneTransforms);

			// create name
			sprintf(g_lpMsg,"~SK%d",dwMeshIndex);

			// allocate memory for name
			if(!AllocMem((LPVOID*)&g_lplpMeshNames[dwMeshIndex],
						 strlen(g_lpMsg)+1))
				return;

			// copy name
			CopyMemory((LPVOID)g_lplpMeshNames[dwMeshIndex],
					   (LPVOID)g_lpMsg,
					   strlen(g_lpMsg)+1);
		}
		else
		{
			DisplayFatalErrorMessage("Invalid number of meshes found in DirectX file.",
									 NULL,
									 FALSE);
			return;
		}

		// do not process any sub-data
		bIgnoreSubs=TRUE;
	}
	else if((*lpType)==TID_D3DRMFrame)
	{
		// check for animation processing
		if(lpAnimData)
		{
			// get pointer to animation key
			LPDWORD lpKey=(LPDWORD)lpAnimData;

			// check key type
			if(lpKey[0]!=ANIMKEY_MATRIX)
			{
				DisplayFatalErrorMessage("Invalid skin mesh animation key.",
										 NULL,
										 FALSE);
				return;
			}

			// see if number of poses has been set
			if(!(*lpdwNumPoses))
			{
				// save number of poses
				(*lpdwNumPoses)=lpKey[1];

				// see if skin has been loaded
				if((*lpdwSkinIndex)==NO_OBJECT)
				{
					DisplayFatalErrorMessage("Invalid skin mesh pose location.",
											 NULL,
											 FALSE);
					return;
				}

				// add initial pose
				(*lpdwPoseIndex)=AddPose(*lpdwSkinIndex);

				// add remaining poses
				if((*lpdwNumPoses)>1)
					for(dwCount=1;dwCount<(*lpdwNumPoses);dwCount++)
						AddPose(*lpdwSkinIndex);
			}
			else
			{
				// compare value
				if((*lpdwNumPoses)!=lpKey[1])
				{
					DisplayFatalErrorMessage("Invalid skin mesh pose array.",
											 NULL,
											 FALSE);
					return;
				}
			}

			// get bone index
			DWORD dwIndex=FindBone(lpName,
								   (*lpdwBoneIndex));

			// skip reference bones
			if(g_lpdwBonePoseIdx[dwIndex]!=NO_OBJECT)
			{
				// cast pointer to matrix data
				LPTFKEYMATRIX lpAnimMtx=(LPTFKEYMATRIX)&lpKey[2];

				// set pose matrices
				for(dwCount=0;dwCount<(*lpdwNumPoses);dwCount++)
					SetPoseMatrix((*lpdwPoseIndex)+dwCount,
								  g_lpdwBonePoseIdx[dwIndex],
								  &lpAnimMtx[dwCount].mtx);
			}
			
			// reset animation data pointer
			lpAnimData=NULL;
			
			// do not process any sub-data
			bIgnoreSubs=TRUE;
		}
		// do not add frames after mesh is loaded
		else if((*lpdwSkinIndex)==NO_OBJECT)
		{
			// see if top-level bone has been found
			if((*lpdwBoneIndex)==NO_OBJECT)
			{
				// add top-level bone
				dwNewBoneIndex=AddBone(AddMatrix(NULL),
									   lpName);
				*lpdwBoneIndex=dwNewBoneIndex;
			}
			else
			{
#ifdef DEBUG_MODE
				// verify index
				if(dwParentBoneIndex>=g_dwNumBone)
				{
					DisplayFatalErrorMessage("Invalid bone index.",
											 "LoadSkinMeshData()",
											 FALSE);
					return;
				}
#endif

				// add child bone
				dwNewBoneIndex=AddBoneChild(dwParentBoneIndex,
											AddMatrix(NULL),
											lpName);
			}
		}
	}
	else if((*lpType)==TID_D3DRMAnimationKey)
	{
		// animation must load after mesh is loaded
		if((*lpdwSkinIndex)==NO_OBJECT)
		{
			DisplayFatalErrorMessage("Invalid animation key position in DirectX file.",
									 NULL,
									 FALSE);
			return;
		}
		else
		{
			// load data for processing under frame
			hr=lpDXFileData->GetData(NULL,
									 &dwSize,
									 &lpAnimData);

			// check for error
			if(FAILED(hr))
			{
				DisplayFatalDXErrorMessage("Unable to read DirectX file data.",
										   NULL,
										   hr);
				return;
			}
		}
	}

	// process any child objects
	if(!bIgnoreSubs)
	{
		// get next object
		IDirectXFileObject* lpDXFileSubObj;
		while(SUCCEEDED(lpDXFileData->GetNextObject(&lpDXFileSubObj)))
		{
			// add resource
			AddCOMResource;

			// check for reference object
			IDirectXFileDataReference* lpDXFileDataRef;
			if(SUCCEEDED(lpDXFileSubObj->QueryInterface(IID_IDirectXFileDataReference,
														(LPVOID*)&lpDXFileDataRef)))
			{
				// add resource
				AddCOMResource;

				// resolve reference object
				IDirectXFileData* lpDXFileSubData;
				if(SUCCEEDED(lpDXFileDataRef->Resolve(&lpDXFileSubData)))
				{
					// add resource
					AddCOMResource;
					
					// process child object
					LoadSkinMeshData(lpDXFileSubData,
									 lpdwSkinIndex,
									 lpdwBoneIndex,
									 dwNewBoneIndex,
									 lpdwPoseIndex,
									 lpdwNumPoses);

					// release child object
					ReleaseCOMPtr(lpDXFileSubData);
				}
				
				// release reference object
				ReleaseCOMPtr(lpDXFileDataRef);
			}
			
			// check for data object
			IDirectXFileData* lpDXFileSubData;
			if(SUCCEEDED(lpDXFileSubObj->QueryInterface(IID_IDirectXFileData,
														(LPVOID*)&lpDXFileSubData)))
			{
				// add resource
				AddCOMResource;

				// process child object
				LoadSkinMeshData(lpDXFileSubData,
								 lpdwSkinIndex,
								 lpdwBoneIndex,
								 dwNewBoneIndex,
								 lpdwPoseIndex,
								 lpdwNumPoses);

				// release child object
				ReleaseCOMPtr(lpDXFileSubData);
			}
			
			// release sub-object
			ReleaseCOMPtr(lpDXFileSubObj);
		}
	}

	// free name buffer
	FreeMem((LPVOID*)&lpName);
}

// PurgeGraphicsObjects()

void PurgeGraphicsObjects(void)
{
	// purge each object
	PurgeVertexBuffers();
	PurgeIndexBuffers();
	PurgeMatrices();
	PurgeMaterials();
	PurgeTextures();
	PurgeLights();
	PurgeMeshes();
	PurgeSkinMeshes();
	PurgeFonts();
	PurgeBones();
	PurgePoses();
}

// SetBonePoseIndex()

void SetBonePoseIndex(DWORD dwBoneIndex,
					  DWORD dwPoseIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "SetBonePoseIndex()",
								 FALSE);
		return;
	}
#endif

	// copy index
	g_lpdwBonePoseIdx[dwBoneIndex]=dwPoseIndex;
}

// FindBone()

DWORD FindBone(LPCSTR lpName,
			   DWORD dwBoneIndex)
{
#ifdef DEBUG_MODE	
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FindBone()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif
	
	// output index
	DWORD dwRetIndex=NO_OBJECT;
	
	// search top-level bone
	FindBoneChild(lpName,
				  dwBoneIndex,
				  &dwRetIndex);

	// return index
	return(dwRetIndex);
}

// FindBoneChild()

void FindBoneChild(LPCSTR lpName,
				   DWORD dwBoneIndex,
	 			   LPDWORD lpdwBoneIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneChild()",
								 FALSE);
		return;
	}
#endif
	
	// see if bone has been found
	if((*lpdwBoneIndex)!=NO_OBJECT)
		return;
	
	// compare bone
	if(stricmp(lpName,
			   g_lplpBoneName[dwBoneIndex])==0)
	{
		// set index
		*lpdwBoneIndex=dwBoneIndex;

		// ok
		return;
	}
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwBoneIndex];

	// search children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwBoneIndex];

		// search each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
			FindBoneChild(lpName,
						  lpdwChildren[dwCount],
						  lpdwBoneIndex);
	}
}

// GetBoneCount()

DWORD GetBoneCount(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "GetBoneCount()",
								 FALSE);
		return(0);
	}
#endif
	
	// return value
	DWORD dwNumBones=0;
	
	// count top-level bone
	GetBoneChildCount(dwIndex,
					  &dwNumBones);

	// return count
	return(dwNumBones);
}

// GetBoneChildCount()

void GetBoneChildCount(DWORD dwIndex,
					   LPDWORD lpdwNumBones)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "GetBoneChildCount()",
								 FALSE);
		return;
	}
#endif
	
	// count this bone
	(*lpdwNumBones)++;
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwIndex];

	// count children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwIndex];

		// search each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
			GetBoneChildCount(lpdwChildren[dwCount],
							  lpdwNumBones);
	}
}


#ifdef DEBUG_MODE
// ManipulateBoneStructure()

void ManipulateBoneStructure(DWORD dwBoneIndex,
							 DWORD dwFontIndex)
{
	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "MaipulateBoneStructure()",
								 FALSE);
		return;
	}

	// verify index
	if(dwFontIndex>=g_dwNumFont)
	{
		DisplayFatalErrorMessage("Invalid font index.",
								 "MaipulateBoneStructure()",
								 FALSE);
		return;
	}

	// index currently initialized
	static DWORD dwCurIndex=NO_OBJECT;
	
	// number of bones in structure
	static DWORD dwNumBones;

	// current bone to manipulate
	static DWORD dwCurBone;
	
	// initialize if necessary
	if(dwCurIndex!=dwBoneIndex)
	{
		// save currently initialized index
		dwCurIndex=dwBoneIndex;

		// get number of bones
		dwNumBones=GetBoneCount(dwBoneIndex);

		// reset current bone
		dwCurBone=0;
	}

	// output info
	int nLine=48;
	BeginText(dwFontIndex);
	OutputText(g_lplpBoneName[dwBoneIndex+dwCurBone],0,nLine);
	nLine+=32;
	OutputText("Yaw+: <1>",0,nLine);
	nLine+=16;
	OutputText("Yaw-: <q>",0,nLine);
	nLine+=16;
	OutputText("Pitch+: <2>",0,nLine);
	nLine+=16;
	OutputText("Pitch-: <w>",0,nLine);
	nLine+=16;
	OutputText("Roll+: <3>",0,nLine);
	nLine+=16;
	OutputText("Roll-: <e>",0,nLine);
	nLine+=16;
	OutputText("XAxis+: <4>",0,nLine);
	nLine+=16;
	OutputText("XAxis-: <r>",0,nLine);
	nLine+=16;
	OutputText("YAxis+: <5>",0,nLine);
	nLine+=16;
	OutputText("YAxis-: <t>",0,nLine);
	nLine+=16;
	OutputText("ZAxis+: <6>",0,nLine);
	nLine+=16;
	OutputText("ZAxis-: <y>",0,nLine);
	nLine+=16;
	OutputText("XScale+: <7>",0,nLine);
	nLine+=16;
	OutputText("XScale-: <u>",0,nLine);
	nLine+=16;
	OutputText("YScale+: <8>",0,nLine);
	nLine+=16;
	OutputText("YScale-: <i>",0,nLine);
	nLine+=16;
	OutputText("ZScale+: <9>",0,nLine);
	nLine+=16;
	OutputText("ZScale-: <o>",0,nLine);
	nLine+=16;
	OutputText("Scale+: <0>",0,nLine);
	nLine+=16;
	OutputText("Scale-: <p>",0,nLine);
	nLine+=32;
	OutputText("Select: <enter>",0,nLine);
	nLine+=32;
	OutputText("Reset: <bksp>",0,nLine);
	nLine+=32;
	OutputText("Root: <space>",0,nLine);
	EndText();

	// select pressed flag
	static BOOL bSelect=FALSE;

	// increment values
	#define ROT_INC 0.0005F
	#define MOV_INC 0.1F
	#define SCL_INC 0.001F
	
	// current increment value
	static float fInc=0;

	// check for root button
	DWORD dwBone=dwBoneIndex;
	if(!IsKeyDown(VK_SPACE))
		dwBone+=dwCurBone;
	
	// check user input and act accordingly
	if(IsKeyDown('1'))
	{
		// yaw+
		fInc+=ROT_INC;
		BoneYaw(dwBone,fInc);
	}
	else if(IsKeyDown('Q'))
	{
		// yaw-
		fInc+=ROT_INC;
		BoneYaw(dwBone,-fInc);
	}
	else if(IsKeyDown('2'))
	{
		// pitch+
		fInc+=ROT_INC;
		BonePitch(dwBone,fInc);
	}
	else if(IsKeyDown('W'))
	{
		// pitch-
		fInc+=ROT_INC;
		BonePitch(dwBone,-fInc);
	}
	else if(IsKeyDown('3'))
	{
		// roll+
		fInc+=ROT_INC;
		BoneRoll(dwBone,fInc);
	}
	else if(IsKeyDown('E'))
	{
		// roll-
		fInc+=ROT_INC;
		BoneRoll(dwBone,-fInc);
	}
	else if(IsKeyDown('4'))
	{
		// x-axis+
		fInc+=MOV_INC;
		BoneRight(dwBone,fInc);
	}
	else if(IsKeyDown('R'))
	{
		// x-axis-
		fInc+=MOV_INC;
		BoneLeft(dwBone,fInc);
	}
	else if(IsKeyDown('5'))
	{
		// y-axis+
		fInc+=MOV_INC;
		BoneUp(dwBone,fInc);
	}
	else if(IsKeyDown('T'))
	{
		// y-axis-
		fInc+=MOV_INC;
		BoneDown(dwBone,fInc);
	}
	else if(IsKeyDown('6'))
	{
		// z-axis+
		fInc+=MOV_INC;
		BoneAhead(dwBone,fInc);
	}
	else if(IsKeyDown('Y'))
	{
		// z-axis-
		fInc+=MOV_INC;
		BoneBack(dwBone,fInc);
	}
	else if(IsKeyDown('7'))
	{
		// x-scale+
		fInc+=SCL_INC;
		BoneXScale(dwBone,1+fInc);
	}
	else if(IsKeyDown('U'))
	{
		// x-scale-
		fInc+=SCL_INC;
		BoneXScale(dwBone,1-fInc);
	}
	else if(IsKeyDown('8'))
	{
		// y-scale+
		fInc+=SCL_INC;
		BoneYScale(dwBone,1+fInc);
	}
	else if(IsKeyDown('I'))
	{
		// y-scale-
		fInc+=SCL_INC;
		BoneYScale(dwBone,1-fInc);
	}
	else if(IsKeyDown('9'))
	{
		// z-scale+
		fInc+=SCL_INC;
		BoneZScale(dwBone,1+fInc);
	}
	else if(IsKeyDown('O'))
	{
		// z-scale-
		fInc+=SCL_INC;
		BoneZScale(dwBone,1-fInc);
	}
	else if(IsKeyDown('0'))
	{
		// scale+
		fInc+=SCL_INC;
		BoneScale(dwBone,1+fInc);
	}
	else if(IsKeyDown('P'))
	{
		// scale-
		fInc+=SCL_INC;
		BoneScale(dwBone,1-fInc);
	}
	else if(IsKeyDown(VK_RETURN))
	{
		// next bone
		if(!bSelect)
		{
			bSelect=TRUE;
			dwCurBone++;
			if(dwCurBone>=dwNumBones)
				dwCurBone=0;
		}
	}
	else if(IsKeyDown(VK_BACK))
	{
		// reset structure
		ResetBoneStructure(dwBoneIndex);
	}
	else
	{
		fInc=0;
		bSelect=FALSE;
	}
}
#endif

// ResetBoneStructure()

void ResetBoneStructure(DWORD dwIndex)
{
#ifdef DEBUG_MODE	
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FindBone()",
								 FALSE);
		return;
	}
#endif
	
	// reset top-level bone
	ResetBoneChild(dwIndex);

	// re-finalize bone structure
	FinalizeBoneStructure(dwIndex);
}

// ResetBoneChild()

void ResetBoneChild(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "RenderBoneChild()",
								 FALSE);
		return;
	}
#endif
	
	// reset this bone
	D3DXMATRIX mMtx;
	D3DXMatrixInverse(&mMtx,
					  NULL,
					  &g_lpOrigBoneMtxInv[dwIndex]);
	
	// store matrix
	SetMatrix(g_lpdwBoneMtxIdx[dwIndex],
			  &mMtx);
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwIndex];

	// search children as parent bone
	if(dwNumChildren)
	{
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwIndex];

		// reset each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
			ResetBoneChild(lpdwChildren[dwCount]);
	}
}

// GetNextPose()

DWORD GetNextPose(void)
{
	// check counter
	if(g_dwNumPose==0)
	{
		// allocate memory
		if(!AllocMem((LPVOID*)&g_lpdwNumPoseBones,
					 sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!AllocMem((LPVOID*)&g_lplpPoseMtx,
					 sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxPose=BLOCK_ALLOC_INC;
	}
	else if(g_dwNumPose>=g_dwMaxPose)
	{
		// expand memory
		if(!ExpandMem((LPVOID*)&g_lpdwNumPoseBones,
					  sizeof(DWORD)*g_dwMaxPose,
					  sizeof(DWORD)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);
		if(!ExpandMem((LPVOID*)&g_lplpPoseMtx,
					  sizeof(LPD3DXMATRIX)*g_dwMaxPose,
					  sizeof(LPD3DXMATRIX)*BLOCK_ALLOC_INC))
			return(NO_OBJECT);

		// increment counter
		g_dwMaxPose+=BLOCK_ALLOC_INC;
	}

	// get return index
	DWORD dwIndex=g_dwNumPose;

	// increment counter
	g_dwNumPose++;

	// return index
	return(dwIndex);
}

// AddPose()

DWORD AddPose(DWORD dwSkinIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "AddPose()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// get return index
	DWORD dwIndex=GetNextPose();

	// get number of bones
	DWORD dwNumBones=g_lplpSkin[dwSkinIndex]->GetNumBones();

	// save number of bones
	g_lpdwNumPoseBones[dwIndex]=dwNumBones;

	// allocate pose matrix array
	if(!AllocMem((LPVOID*)&g_lplpPoseMtx[dwIndex],
				 sizeof(D3DXMATRIX)*dwNumBones))
		return(NO_OBJECT);

	// return index
	return(dwIndex);
}

// AddPose()

DWORD AddPose(DWORD dwSkinIndex,
 			  DWORD dwBoneIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "AddPose()",
								 FALSE);
		return(NO_OBJECT);
	}

	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "AddPose()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// get return index
	DWORD dwIndex=GetNextPose();

	// get number of bones
	DWORD dwNumBones=g_lplpSkin[dwSkinIndex]->GetNumBones();

	// save number of bones
	g_lpdwNumPoseBones[dwIndex]=dwNumBones;

	// allocate pose matrix array
	if(!AllocMem((LPVOID*)&g_lplpPoseMtx[dwIndex],
				 sizeof(D3DXMATRIX)*dwNumBones))
		return(NO_OBJECT);

	// translate bone structure into array
	TranslateBoneStructure(dwBoneIndex,
						   g_lplpPoseMtx[dwIndex]);

	// return index
	return(dwIndex);
}

// PurgePoses()

void PurgePoses(void)
{
	// check counter
	if(g_dwNumPose)
	{
		// counter
		DWORD dwCount;

		// release all matrix arrays
		for(dwCount=0;dwCount<g_dwNumPose;dwCount++)
			FreeMem((LPVOID*)&g_lplpPoseMtx[dwCount]);

		// free memory
		FreeMem((LPVOID*)&g_lpdwNumPoseBones);
		FreeMem((LPVOID*)&g_lplpPoseMtx);
		
		// reset counters
		g_dwNumPose=0;
		g_dwMaxPose=0;
	}
}

// SetPoseMatrix()

void SetPoseMatrix(DWORD dwPoseIndex,
				   DWORD dwMtxNum,
				   LPD3DXMATRIX lpMtx)
{
	// check for reference matrix
	if(dwMtxNum==NO_OBJECT)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "SetPoseMatrix()",
								 FALSE);
		return;
	}

	// verify index
	if(dwMtxNum>=g_lpdwNumPoseBones[dwPoseIndex])
	{
		DisplayFatalErrorMessage("Invalid pose matrix index.",
								 "SetPoseMatrix()",
								 FALSE);
		return;
	}
#endif

	// set pointer to matrix array
	LPD3DXMATRIX lpMtxArray=g_lplpPoseMtx[dwPoseIndex];

	// set matrix
	CopyMemory(&lpMtxArray[dwMtxNum],
			   lpMtx,
			   sizeof(D3DXMATRIX));
}

// SetPoseMatrix()

void SetPoseMatrix(DWORD dwPoseIndex,
				   DWORD dwMtxNum,
				   DWORD dwMtxIndex)
{
	// check for reference matrix
	if(dwMtxNum==NO_OBJECT)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "SetPoseMatrix()",
								 FALSE);
		return;
	}

	// verify index
	if(dwMtxIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetPoseMatrix()",
								 FALSE);
		return;
	}

	// verify index
	if(dwMtxNum>=g_lpdwNumPoseBones[dwPoseIndex])
	{
		DisplayFatalErrorMessage("Invalid pose matrix index.",
								 "SetPoseMatrix()",
								 FALSE);
		return;
	}
#endif

	// get matrix
	D3DXMATRIX mMtx;
	GetMatrix(dwMtxIndex,
			  &mMtx);

	// set pointer to matrix array
	LPD3DXMATRIX lpMtxArray=g_lplpPoseMtx[dwPoseIndex];
	
	// set matrix
	CopyMemory(&lpMtxArray[dwMtxNum],
			   &mMtx,
			   sizeof(D3DXMATRIX));
}

// RenderSkinMeshPose()

void RenderSkinMeshPose(DWORD dwSkinIndex,
					    DWORD dwPoseIndex,
					    DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMeshPose()",
								 FALSE);
		return;
	}

	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "RenderSkinMeshPose()",
								 FALSE);
		return;
	}
#endif

	// copy matrix array
	CopyMemory(g_lplpSkinBoneMtx[dwSkinIndex],
			   g_lplpPoseMtx[dwPoseIndex],
			   sizeof(D3DXMATRIX)*g_lpdwNumPoseBones[dwPoseIndex]);

	// render mesh
	RenderSkinMesh(dwSkinIndex,
				   dwWMtxIndex);
}

// RenderSkinMeshTween()

void RenderSkinMeshTween(DWORD dwSkinIndex,
						 DWORD dwPose1Index,
						 DWORD dwPose2Index,
						 float fTweenVal,
						 DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwSkinIndex>=g_dwNumSkin)
	{
		DisplayFatalErrorMessage("Invalid skin mesh index.",
								 "RenderSkinMeshTween()",
								 FALSE);
		return;
	}

	// verify indices
	if(dwPose1Index>=g_dwNumPose||
	   dwPose2Index>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "RenderSkinMeshTween()",
								 FALSE);
		return;
	}

	// verify index
	if(g_lpdwNumPoseBones[dwPose1Index]!=g_lpdwNumPoseBones[dwPose2Index])
	{
		DisplayFatalErrorMessage("Incompatible poses for tweening.",
								 "RenderSkinMeshTween()",
								 FALSE);
		return;
	}
#endif

	// cast float values to matrix arrays
	float* lpfPose1=(float*)g_lplpPoseMtx[dwPose1Index];
	float* lpfPose2=(float*)g_lplpPoseMtx[dwPose2Index];
	float* lpfOutput=(float*)g_lplpSkinBoneMtx[dwSkinIndex];

	// compute number to process
	DWORD dwNumFloats=g_lpdwNumPoseBones[dwPose1Index]*16;

	// process values
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumFloats;dwCount++)
	{
		// compute output value
		(*lpfOutput)=TweenVal((*lpfPose1),(*lpfPose2),(fTweenVal));

		// increment pointers
		lpfPose1++;
		lpfPose2++;
		lpfOutput++;
	}

	// render mesh
	RenderSkinMesh(dwSkinIndex,
				   dwWMtxIndex);
}

// AddPose()

DWORD AddPose(DWORD dwPose1Index,
			  DWORD dwPose2Index,
			  float fTweenVal)
{
#ifdef DEBUG_MODE
	// verify indices
	if(dwPose1Index>=g_dwNumPose||
	   dwPose2Index>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "RenderSkinMeshTween()",
								 FALSE);
		return(NO_OBJECT);
	}

	// verify index
	if(g_lpdwNumPoseBones[dwPose1Index]!=g_lpdwNumPoseBones[dwPose2Index])
	{
		DisplayFatalErrorMessage("Incompatible poses for tweening.",
								 "RenderSkinMeshTween()",
								 FALSE);
		return(NO_OBJECT);
	}
#endif

	// get return index
	DWORD dwIndex=GetNextPose();

	// get number of bones
	DWORD dwNumBones=g_lpdwNumPoseBones[dwPose1Index];

	// save number of bones
	g_lpdwNumPoseBones[dwIndex]=dwNumBones;

	// allocate pose matrix array
	if(!AllocMem((LPVOID*)&g_lplpPoseMtx[dwIndex],
				 sizeof(D3DXMATRIX)*dwNumBones))
		return(NO_OBJECT);

	// cast pointers to matrix arrays
	LPD3DXMATRIX lpPose1=(LPD3DXMATRIX)g_lplpPoseMtx[dwPose1Index];
	LPD3DXMATRIX lpPose2=(LPD3DXMATRIX)g_lplpPoseMtx[dwPose2Index];
	LPD3DXMATRIX lpOutput=(LPD3DXMATRIX)g_lplpPoseMtx[dwIndex];

	// process matrices
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumBones;dwCount++)
	{
		// compute right vector
		D3DXVECTOR3 vRight;
		vRight.x=TweenVal(lpPose1[dwCount]._11,
						  lpPose2[dwCount]._11,
						  fTweenVal);
		vRight.y=TweenVal(lpPose1[dwCount]._12,
						  lpPose2[dwCount]._12,
						  fTweenVal);
		vRight.z=TweenVal(lpPose1[dwCount]._13,
						  lpPose2[dwCount]._13,
						  fTweenVal);
		D3DXVECTOR3 vNRight;
		Normalize(&vNRight,
				  &vRight);

		// compute up vector
		D3DXVECTOR3 vUp;
		vUp.x=TweenVal(lpPose1[dwCount]._21,
					   lpPose2[dwCount]._21,
					   fTweenVal);
		vUp.y=TweenVal(lpPose1[dwCount]._22,
					   lpPose2[dwCount]._22,
					   fTweenVal);
		vUp.z=TweenVal(lpPose1[dwCount]._23,
					   lpPose2[dwCount]._23,
					   fTweenVal);
		D3DXVECTOR3 vNUp;
		Normalize(&vNUp,
				  &vUp);

		// compute ahead vector
		D3DXVECTOR3 vNAhead;
		CrossProduct(&vNAhead,
					 &vNRight,
					 &vNUp);

		// compute position vector
		D3DXVECTOR3 vPos;
		vPos.x=TweenVal(lpPose1[dwCount]._41,
						lpPose2[dwCount]._41,
						fTweenVal);
		vPos.y=TweenVal(lpPose1[dwCount]._42,
						lpPose2[dwCount]._42,
						fTweenVal);
		vPos.z=TweenVal(lpPose1[dwCount]._43,
						lpPose2[dwCount]._43,
						fTweenVal);

		// output final matrix
		lpOutput[dwCount]=D3DXMATRIX(vNRight.x,vNRight.y,vNRight.z,0,
									 vNUp.x,   vNUp.y,   vNUp.z,   0,
									 vNAhead.x,vNAhead.y,vNAhead.z,0,
									 vPos.x,   vPos.y,   vPos.z,   1);
	}

	// return index
	return(dwIndex);
}

#ifdef DEBUG_MODE
// DisplayMatrixMessage()

void DisplayMatrixMessage(LPD3DXMATRIX lpMtx)
{
	sprintf(g_lpMsg,
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f\n"
			"%f, %f, %f, %f",
			lpMtx->_11,
			lpMtx->_12,
			lpMtx->_13,
			lpMtx->_14,
			lpMtx->_21,
			lpMtx->_22,
			lpMtx->_23,
			lpMtx->_24,
			lpMtx->_31,
			lpMtx->_32,
			lpMtx->_33,
			lpMtx->_34,
			lpMtx->_41,
			lpMtx->_42,
			lpMtx->_43,
			lpMtx->_44);
	DisplayInfoMessage(g_lpMsg);
}
#endif

// FinalizePoses()

void FinalizePoses(DWORD dwPoseIndex,
				   DWORD dwNumPoses,
				   DWORD dwBoneIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "FinalizePoses()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBoneIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FinalizePoses()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;

	// finalize all poses
	for(dwCount=0;dwCount<dwNumPoses;dwCount++)
		FinalizePose(dwPoseIndex+dwCount,
					 dwBoneIndex);
}

// FinalizePose()

void FinalizePose(DWORD dwPoseIndex,
				  DWORD dwBoneIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "FinalizePose()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBoneIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FinalizePose()",
								 FALSE);
		return;
	}
#endif
	
	// get base matrix
	D3DXMATRIX mBase;
	GetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
			  &mBase);

	// translate top-level bone
	FinalizePoseChild(dwPoseIndex,
					  dwBoneIndex,
					  &mBase);

	// translate bone structure into this pose
	TranslateBoneStructure(dwBoneIndex,
						   g_lplpPoseMtx[dwPoseIndex]);
}

// FinalizePoseChild()

void FinalizePoseChild(DWORD dwPoseIndex,
					   DWORD dwBoneIndex,
					   LPD3DXMATRIX lpMtx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwPoseIndex>=g_dwNumPose)
	{
		DisplayFatalErrorMessage("Invalid pose index.",
								 "FinalizePoseChild()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBoneIndex>=g_dwNumBone)
	{
		DisplayFatalErrorMessage("Invalid bone index.",
								 "FinalizePoseChild()",
								 FALSE);
		return;
	}
#endif
	
	// get number of children
	DWORD dwNumChildren=g_lpdwNumBoneChildren[dwBoneIndex];

	// translate children as parent bones
	if(dwNumChildren)
	{
		// get new base matrix
		D3DXMATRIX mBase;
		GetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
				  &mBase);
		
		// get pointer to child index
		LPDWORD lpdwChildren=g_lplpdwBoneChildren[dwBoneIndex];

		// translate each child
		DWORD dwCount;
		for(dwCount=0;dwCount<dwNumChildren;dwCount++)
			FinalizePoseChild(dwPoseIndex,
							  lpdwChildren[dwCount],
							  &mBase);
	}

	// ignore reference bones
	if(g_lpdwBonePoseIdx[dwBoneIndex]==NO_OBJECT)
		return;
	
	// set pointer to matrix array
	LPD3DXMATRIX lpMtxArray=(LPD3DXMATRIX)g_lplpPoseMtx[dwPoseIndex];

	// set new bone matrix
	SetMatrix(g_lpdwBoneMtxIdx[dwBoneIndex],
			  &lpMtxArray[g_lpdwBonePoseIdx[dwBoneIndex]]);
}

// InitSkybox()

void InitSkybox(void)
{
	// add skybox matrix
	g_dwSkyboxMtx=AddMatrix(NULL);

	// add skybox material
	g_dwSkyboxMat=AddMaterial(0,0,0,1,
							  0,0,0,1,
							  0,0,0,1,
							  1,1,1,1,
							  0);

	// add skybox texture
	g_dwSkyboxTex=AddTexture(2,2);
	ClearTexture(g_dwSkyboxTex);

	// add skybox vertex buffers
	g_sbLeft.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										   sizeof(SKYBOXVERTEX),
										   5);
	g_sbRight.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										    sizeof(SKYBOXVERTEX),
										    5);
	g_sbFront.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										    sizeof(SKYBOXVERTEX),
										    5);
	g_sbBack.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										   sizeof(SKYBOXVERTEX),
										   5);
	g_sbTop.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										  sizeof(SKYBOXVERTEX),
										  5);
	g_sbBottom.dwVtxBufIndex=AddVertexBuffer(D3DFVF_SKYBOXVERTEX,
										     sizeof(SKYBOXVERTEX),
										     5);

	// skybox vertices
	LPSKYBOXVERTEX lpVtx;
	
	// set left skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbLeft.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,-0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],-0.707106F,0.707106F,0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],-1,0,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,-0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],-0.707106F,-0.707106F,0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbLeft.dwVtxBufIndex);

	// set right skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbRight.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],0.707106F,0.707106F,0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,-0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],1,0,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],0.707106F,-0.707106F,0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,-0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbRight.dwVtxBufIndex);

	// set front skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbFront.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],0,0,1,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbFront.dwVtxBufIndex);

	// set back skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbBack.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],0.707106F,0.707106F,-0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],-0.707106F,0.707106F,-0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],0,0,-1,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],0.707106F,-0.707106F,-0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],-0.707106F,-0.707106F,-0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbBack.dwVtxBufIndex);

	// set top skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbTop.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,0.707106F,-0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,0.707106F,-0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],0,1,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,0.707106F,0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,0.707106F,0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbTop.dwVtxBufIndex);

	// set bottom skybox vertex buffer
	lpVtx=(LPSKYBOXVERTEX)LockVertexBuffer(g_sbBottom.dwVtxBufIndex);
	SET_SKYBOXVERTEX(lpVtx[0],-0.707106F,-0.707106F,0.707106F,0.003906F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[1],0.707106F,-0.707106F,0.707106F,0.996093F,0.003906F);
	SET_SKYBOXVERTEX(lpVtx[2],0,-1,0,0.5F,0.5F);
	SET_SKYBOXVERTEX(lpVtx[3],-0.707106F,-0.707106F,-0.707106F,0.003906F,0.996093F);
	SET_SKYBOXVERTEX(lpVtx[4],0.707106F,-0.707106F,-0.707106F,0.996093F,0.996093F);
	UnlockVertexBuffer(g_sbBottom.dwVtxBufIndex);

	// add skybox index buffer
	g_dwSkyboxIdxBuf=AddIndexBuffer(D3DPT_TRIANGLEFAN,
									4);

	// set skybox index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwSkyboxIdxBuf);
	lpIdx[0]=2;
	lpIdx[1]=0;
	lpIdx[2]=1;
	lpIdx[3]=4;
	lpIdx[4]=3;
	lpIdx[5]=0;
	UnlockIndexBuffer(g_dwSkyboxIdxBuf);

	// reset skybox textures
	g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	g_sbRight.dwTexIndex=g_dwSkyboxTex;
	g_sbFront.dwTexIndex=g_dwSkyboxTex;
	g_sbBack.dwTexIndex=g_dwSkyboxTex;
	g_sbTop.dwTexIndex=g_dwSkyboxTex;
	g_sbBottom.dwTexIndex=g_dwSkyboxTex;

	// reset offset
	g_vSkyboxOffset=D3DXVECTOR3(0,0,0);

	// set default radius
	g_lpMtx[g_dwSkyboxMtx].fXScale=75;
	g_lpMtx[g_dwSkyboxMtx].fYScale=75;
	g_lpMtx[g_dwSkyboxMtx].fZScale=75;
}

// SetSkybox()

void SetSkybox(float fRadius,
			   float fOffsetX,
			   float fOffsetY,
			   float fOffsetZ,
			   DWORD dwLtTexIdx,
			   DWORD dwRtTexIdx,
			   DWORD dwFtTexIdx,
			   DWORD dwBkTexIdx,
			   DWORD dwTpTexIdx,
			   DWORD dwBtTexIdx)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwLtTexIdx>=g_dwNumTex&&
	   dwLtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwRtTexIdx>=g_dwNumTex&&
	   dwRtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwFtTexIdx>=g_dwNumTex&&
	   dwFtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBkTexIdx>=g_dwNumTex&&
	   dwBkTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTpTexIdx>=g_dwNumTex&&
	   dwTpTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}

	// verify index
	if(dwBtTexIdx>=g_dwNumTex&&
	   dwBtTexIdx!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkybox()",
								 FALSE);
		return;
	}
#endif

	// set textures
	if(dwLtTexIdx==NO_OBJECT)
		g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbLeft.dwTexIndex=dwLtTexIdx;
	if(dwRtTexIdx==NO_OBJECT)
		g_sbRight.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbRight.dwTexIndex=dwRtTexIdx;
	if(dwFtTexIdx==NO_OBJECT)
		g_sbFront.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbFront.dwTexIndex=dwFtTexIdx;
	if(dwBkTexIdx==NO_OBJECT)
		g_sbBack.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBack.dwTexIndex=dwBkTexIdx;
	if(dwTpTexIdx==NO_OBJECT)
		g_sbTop.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbTop.dwTexIndex=dwTpTexIdx;
	if(dwBtTexIdx==NO_OBJECT)
		g_sbBottom.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBottom.dwTexIndex=dwBtTexIdx;
	
	// set radius
	g_fSkyboxRadius=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fXScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fYScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fZScale=fRadius;

	// set offset
	g_vSkyboxOffset=D3DXVECTOR3(fOffsetX,fOffsetY,fOffsetZ);
}

// SetSkyboxLeft()

void SetSkyboxLeft(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxLeft()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbLeft.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbLeft.dwTexIndex=dwTexIndex;
}

// SetSkyboxRight()

void SetSkyboxRight(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxRight()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbRight.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbRight.dwTexIndex=dwTexIndex;
}

// SetSkyboxFront()

void SetSkyboxFront(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxFront()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbFront.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbFront.dwTexIndex=dwTexIndex;
}

// SetSkyboxBack()

void SetSkyboxBack(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxBack()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbBack.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBack.dwTexIndex=dwTexIndex;
}

// SetSkyboxTop()

void SetSkyboxTop(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxTop()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbTop.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbTop.dwTexIndex=dwTexIndex;
}

// SetSkyboxBottom()

void SetSkyboxBottom(DWORD dwTexIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetSkyboxBottom()",
								 FALSE);
		return;
	}
#endif
	
	// set value
	if(dwTexIndex==NO_OBJECT)
		g_sbBottom.dwTexIndex=g_dwSkyboxTex;
	else
		g_sbBottom.dwTexIndex=dwTexIndex;
}

// SetSkyboxRadius()

void SetSkyboxRadius(float fRadius)
{
	// copy radius
	g_fSkyboxRadius=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fXScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fYScale=fRadius;
	g_lpMtx[g_dwSkyboxMtx].fZScale=fRadius;
}

// RenderSkybox()

void RenderSkybox(LPD3DXVECTOR3 lpV)
{
	// render skybox
	RenderSkybox(lpV->x,
				 lpV->y,
				 lpV->z);
}

// RenderSkybox()

void RenderSkybox(DWORD dwWMtxIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwWMtxIndex>=g_dwNumMtx)

	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "RenderSkybox()",
								 FALSE);
		return;
	}
#endif
	
	// render skybox
	RenderSkybox(g_lpMtx[dwWMtxIndex].mtx._41,
				 g_lpMtx[dwWMtxIndex].mtx._42,
				 g_lpMtx[dwWMtxIndex].mtx._43);
}

// RenderSkybox()

void RenderSkybox(float fX,
				  float fY,
				  float fZ)
{
	// update skybox matrix
	g_lpMtx[g_dwSkyboxMtx].mtx._41=fX-(g_vSkyboxOffset.x*fX);
	g_lpMtx[g_dwSkyboxMtx].mtx._42=fY-(g_vSkyboxOffset.y*fY);
	g_lpMtx[g_dwSkyboxMtx].mtx._43=fZ-(g_vSkyboxOffset.z*fZ);

	// disable z-buffer writes
	BOOL bFlag=g_bZBufferWrites;
	if(bFlag)
		DisableZBufferWrites();

	// set render values
	SetWorldMatrix(g_dwSkyboxMtx);
	SetMaterial(g_dwSkyboxMat);
	SetIndexBuffer(g_dwSkyboxIdxBuf);

	// render left side
	SetVertexBuffer(g_sbLeft.dwVtxBufIndex);
	SetTexture(g_sbLeft.dwTexIndex);
	RenderPrimitive();

	// render right side
	SetVertexBuffer(g_sbRight.dwVtxBufIndex);
	SetTexture(g_sbRight.dwTexIndex);
	RenderPrimitive();

	// render front
	SetVertexBuffer(g_sbFront.dwVtxBufIndex);
	SetTexture(g_sbFront.dwTexIndex);
	RenderPrimitive();

	// render back
	SetVertexBuffer(g_sbBack.dwVtxBufIndex);
	SetTexture(g_sbBack.dwTexIndex);
	RenderPrimitive();

	// render top
	SetVertexBuffer(g_sbTop.dwVtxBufIndex);
	SetTexture(g_sbTop.dwTexIndex);
	RenderPrimitive();

	// render bottom
	SetVertexBuffer(g_sbBottom.dwVtxBufIndex);
	SetTexture(g_sbBottom.dwTexIndex);
	RenderPrimitive();

	// restore z-buffer writes
	if(bFlag)
		EnableZBufferWrites();
}

// InitBillboards()

void InitBillboards(void)
{
	// add billboard matrix
	g_dwBBMtxIndex=AddMatrix(NULL);

	// add billboard vertex buffer
	g_dwBBVtxBufIdx=AddVertexBuffer(D3DFVF_BILLBOARDVERTEX,
									sizeof(BILLBOARDVERTEX),
									4);

	// set billboard vertex buffer
	LPBILLBOARDVERTEX lpVtx=(LPBILLBOARDVERTEX)LockVertexBuffer(g_dwBBVtxBufIdx);
	SET_BILLBOARDVERTEX(lpVtx[0],
						-0.5F,-0.5F,0,
						0,0,1,
						0,1);
	SET_BILLBOARDVERTEX(lpVtx[1],
						0.5F,-0.5F,0,
						0,0,1,
						1,1);
	SET_BILLBOARDVERTEX(lpVtx[2],
						-0.5F,0.5F,0,
						0,0,1,
						0,0);
	SET_BILLBOARDVERTEX(lpVtx[3],
						0.5F,0.5F,0,
						0,0,1,
						1,0);
	UnlockVertexBuffer(g_dwBBVtxBufIdx);

	// add billboard index buffer
	g_dwBBIdxBufIdx=AddIndexBuffer(D3DPT_TRIANGLEFAN,
								   2);

	// set billboard index buffer
	LPWORD lpIdx=LockIndexBuffer(g_dwBBIdxBufIdx);
	lpIdx[0]=1;
	lpIdx[1]=0;
	lpIdx[2]=2;
	lpIdx[3]=3;
	UnlockIndexBuffer(g_dwBBIdxBufIdx);
}

// RenderBillboard()

void RenderBillboard(DWORD dwMatIndex,
					 DWORD dwTexIndex,
					 LPD3DXVECTOR3 lpvPos,
					 float fXSize,
					 float fYSize,
					 float fZSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}
#endif

	// render list of 1 billboard
	RenderBillboards(dwMatIndex,
					 dwTexIndex,
					 lpvPos,
					 1,
					 fXSize,
					 fYSize,
					 fZSize);
}

// RenderBillboards()

void RenderBillboards(DWORD dwMatIndex,
					  DWORD dwTexIndex,
					  LPD3DXVECTOR3 lpvPos,
					  DWORD dwNumPos,
					  float fXSize,
					  float fYSize,
					  float fZSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwMatIndex>=g_dwNumMat&&
	   dwMatIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid material index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}

	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)

	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderBillboards()",
								 FALSE);
		return;
	}
#endif
	
	// set render states
	SetMaterial(dwMatIndex);
	SetTexture(dwTexIndex);
	SetVertexBuffer(g_dwBBVtxBufIdx);
	SetIndexBuffer(g_dwBBIdxBufIdx);

	// prepare world matrix
	g_lpMtx[g_dwBBMtxIndex].mtx._11=g_vBBRight.x;
	g_lpMtx[g_dwBBMtxIndex].mtx._12=g_vBBRight.y;
	g_lpMtx[g_dwBBMtxIndex].mtx._13=g_vBBRight.z;
	g_lpMtx[g_dwBBMtxIndex].mtx._21=g_vBBUp.x;
	g_lpMtx[g_dwBBMtxIndex].mtx._22=g_vBBUp.y;
	g_lpMtx[g_dwBBMtxIndex].mtx._23=g_vBBUp.z;
	g_lpMtx[g_dwBBMtxIndex].mtx._31=g_vBBAhead.x;
	g_lpMtx[g_dwBBMtxIndex].mtx._32=g_vBBAhead.y;
	g_lpMtx[g_dwBBMtxIndex].mtx._33=g_vBBAhead.z;
	g_lpMtx[g_dwBBMtxIndex].fXScale=fXSize;
	g_lpMtx[g_dwBBMtxIndex].fYScale=fYSize;
	g_lpMtx[g_dwBBMtxIndex].fZScale=fZSize;

	// render each billboard
	DWORD dwCount;
	for(dwCount=0;dwCount<dwNumPos;dwCount++)
	{
		// set world matrix position
		g_lpMtx[g_dwBBMtxIndex].mtx._41=lpvPos[dwCount].x;
		g_lpMtx[g_dwBBMtxIndex].mtx._42=lpvPos[dwCount].y;
		g_lpMtx[g_dwBBMtxIndex].mtx._43=lpvPos[dwCount].z;

		// set world matrix
		SetWorldMatrix(g_dwBBMtxIndex);

		// render billboard
		RenderPrimitive();
	}
}

// AddCameraMatrix()

DWORD AddCameraMatrix(float fPosX,
					  float fPosY,
					  float fPosZ,
					  float fTargetX,
					  float fTargetY,
					  float fTargetZ,
					  float fUpX,
					  float fUpY,
					  float fUpZ)
{
	// prepare vectors
	D3DXVECTOR3 vPos=D3DXVECTOR3(fPosX,fPosY,fPosZ);
	D3DXVECTOR3 vTarget=D3DXVECTOR3(fTargetX,fTargetY,fTargetZ);
	D3DXVECTOR3 vUp=D3DXVECTOR3(fUpX,fUpY,fUpZ);
	
	// prepare view matrix
	D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView,
					   &vPos,
					   &vTarget,
					   &vUp);

	// invert view matrix
	D3DXMATRIX mCam;
	D3DXMatrixInverse(&mCam,
					  NULL,
					  &mView);

	// add matrix
	return(AddMatrix(&mCam));
}

// AddMatrix()

DWORD AddMatrix(float _11,
				float _12,
				float _13,
				float _14,
				float _21,
				float _22,
				float _23,
				float _24,
				float _31,
				float _32,
				float _33,
				float _34,
				float _41,
				float _42,
				float _43,
				float _44)
{
	// prepare matrix
	D3DXMATRIX mMtx=D3DXMATRIX(_11,_12,_13,_14,
							   _21,_22,_23,_24,
							   _31,_32,_33,_34,
							   _41,_42,_43,_44);

	// add matrix
	return(AddMatrix(&mMtx));
}

// FindLight()

DWORD FindLight(LPCSTR lpName)
{
	// counter
	DWORD dwCount;

	// search lights
	if(g_dwNumLight)
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			if(stricmp(lpName,g_lplpLightNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindMaterial()

DWORD FindMaterial(LPCSTR lpName)
{
	// counter
	DWORD dwCount;

	// search materials
	if(g_dwNumMat)
		for(dwCount=0;dwCount<g_dwNumMat;dwCount++)
			if(stricmp(lpName,g_lplpMatNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindTexture()

DWORD FindTexture(LPCSTR lpName)
{
	// counter
	DWORD dwCount;

	// search textures
	if(g_dwNumTex)
		for(dwCount=0;dwCount<g_dwNumTex;dwCount++)
			if(stricmp(lpName,g_lplpTexNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// FindMesh()

DWORD FindMesh(LPCSTR lpName)
{
	// counter
	DWORD dwCount;

	// search meshes
	if(g_dwNumMesh)
		for(dwCount=0;dwCount<g_dwNumMesh;dwCount++)
			if(stricmp(lpName,g_lplpMeshNames[dwCount])==0)
				return(dwCount);

	// not found
	return(NO_OBJECT);
}

// SetSkyboxOffset()

void SetSkyboxOffset(float fX,
					 float fY,
					 float fZ)
{
	// set offset
	g_vSkyboxOffset=D3DXVECTOR3(fX,fY,fZ);
}

// AddMeshNormals()

void AddMeshNormals(DWORD dwIndex)
{
	// used for error checking
	HRESULT hr;
	
	// get fvf flags
	DWORD dwFVF=g_lplpMesh[dwIndex]->GetFVF();

	// check for existing normals
	if(dwFVF&D3DFVF_NORMAL)
		return;

	// add normals to fvf
	dwFVF=dwFVF|D3DFVF_NORMAL;

	// get mesh creation options
	DWORD dwOptions=g_lplpMesh[dwIndex]->GetOptions();

	// create new mesh
	ID3DXMesh* lpMesh;
	hr=g_lplpMesh[dwIndex]->CloneMeshFVF(dwOptions,
										 dwFVF,
										 g_lpD3DDevice,
										 &lpMesh);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to clone mesh for normalization.",
								   "Check available memory.",
								   hr);
		return;
	}
	else
		AddCOMResource;

	// add normals
	hr=D3DXComputeNormals(lpMesh,
						  NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to generate mesh normals.",
								   NULL,
								   hr);
		return;
	}

	// replace old mesh with new one
	ReleaseCOMPtr(g_lplpMesh[dwIndex]);
	g_lplpMesh[dwIndex]=lpMesh;
}

// LockBackBuffer()

D3DLOCKED_RECT LockBackBuffer(void)
{
	// return value
	D3DLOCKED_RECT lr;
	InitStruct(lr);

	// get surface
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->GetBackBuffer(0,
								 D3DBACKBUFFER_TYPE_MONO,
								 &g_lpBackBuffer);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to retrieve back buffer surface.",
							  NULL,
							  hr);
	else
#endif
	AddCOMResource;
	
	// lock buffer
#ifdef DEBUG_MODE
	hr=
#endif
	g_lpBackBuffer->LockRect(&lr,
							 NULL,
							 D3DLOCK_NO_DIRTY_UPDATE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to lock back buffer.",
							  NULL,
							  hr);
#endif

	// return locked rect
	return(lr);
}

// UnlockBackBuffer()

void UnlockBackBuffer(void)
{
	// unlock buffer
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpBackBuffer->UnlockRect();
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to unlock back buffer.",
							  NULL,
							  hr);
#endif

	// release surface
	ReleaseCOMPtr(g_lpBackBuffer);
}

// ResetGraphicsObjects()

void ResetGraphicsObjects(void)
{
	// purge all previous objects
	PurgeGraphicsObjects();

	// init necessary objects
	InitSprites();
	InitFader();
	InitSkybox();
	InitBillboards();
	InitTransitions();
	InitMouseCursor();
#ifdef DEBUG_MODE
	InitLineRender();
#endif

	// reset skybox
	SetSkybox(50);
}

// ResetRenderDevice()

void ResetRenderDevice(void)
{
	// used for error checking
	HRESULT hr;
	
	// do nothing if device is not operational
	if(g_lpD3DDevice->TestCooperativeLevel()==D3DERR_DEVICELOST)
		return;

	// update window metrics
	UpdateWindowMetrics();

	// set presentation parameters
	if(!SetPresentationParameters())
		return;

	// reset device
	hr=g_lpD3DDevice->Reset(&g_pp);

	// check for error
	if(FAILED(hr))
		return;

	// restore surface mapper if necessary
	if(g_lpRTS)
	{
		// reset mapper
		hr=g_lpRTS->OnResetDevice();
		
		// check for error
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to reset surface mapper.",
								  NULL,
								  hr);
	}

	// update client area
	UpdateClientArea();

	// restore device settings
	ResetRenderEnvironment();

	// clear screen
	ClearScreen();
}

// InitDirect3D()

BOOL InitDirect3D(void)
{
	// create d3d interface
	g_lpD3D=Direct3DCreate8(D3D_SDK_VERSION);
	
	// check for error
	if(!g_lpD3D)
	{
		DisplayDXErrorMessage("Unable to create Direct3D interface.",
						      "Make sure you are running the current version of DirectX.");
		return(FALSE);
	}
	else
		AddCOMResource;

	// get display adapter info
	if(!GetDisplayAdapterInfo())
		return(FALSE);

	// get display mode
	g_dmSys.fmt=GetSystemDisplayFormat();

	// ok
	return(TRUE);
}

// ReleaseDirect3D()

void ReleaseDirect3D(void)
{
	// release display data
	ReleaseResolutions();
	ReleaseRenderModes();
	
	// release direct3d
	ReleaseCOMPtr(g_lpD3D);
}

// GetDisplayAdapterInfo()

BOOL GetDisplayAdapterInfo(void)
{
	// used for error checking
	HRESULT hr;
	
	// get adapter id info
	hr=g_lpD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT,
									 NULL,
									 &g_lpAdapterID);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve display adapter ID information.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// get device caps
	hr=g_lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
							  D3DDEVTYPE_HAL,
							  &g_lpDevCaps);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to retrieve display adapter capabilities.",
							  g_lpAdapterID.Description,
							  hr);
		return(FALSE);
	}

	// get available render modes
	if(!InitRenderModes(D3DADAPTER_DEFAULT))
		return(FALSE);
	
	// get available resolutions
	if(!InitResolutions())
		return(FALSE);

	// ok
	return(TRUE);
}
	
// VerifyWindowedRendering()

BOOL VerifyWindowedRendering(void)
{
	// reset windowed rendering flag
	g_bWindowedOK=TRUE;
	
	// check device for windowed rendering
	if(!(g_lpDevCaps.Caps2&D3DCAPS2_CANRENDERWINDOWED))
		g_bWindowedOK=FALSE;

	// get application display mode
	g_dmApp=ApplicationDisplayMode();
	
	// check display mode for windowed rendering
	if(!IsModeRenderable(&g_dmSys))
		g_bWindowedOK=FALSE;

	// check display resolution for windowed rendering
	if(g_dmApp.dwX>=g_dmSys.dwX&&
	   g_dmApp.dwY>=g_dmSys.dwY)
	    g_bWindowedOK=FALSE;

	// check windowed flag
	if(g_dmApp.bWindowed&&
	   !g_bWindowedOK)
	{
		// check registry for flag
		LPSTR lpFlag=GetRegistryKey("DXNoWinMsg");
		if(!lpFlag)
		{
			// display message box
			DisplayInfoMessage("Your current display settings do not allow windowed execution.\n\n"
							   "Switching to full-screen mode.");

			// set flag
			SetRegistryKey("DXNoWinMsg",
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
		DeleteRegistryKey("DXNoWinMsg");
	}

	// ok
	return(TRUE);
}

// SetPresentationParameters()

BOOL SetPresentationParameters(void)
{
	// verify windowed rendering
	if(!VerifyWindowedRendering())
		return(FALSE);

	// reset structure
	InitStruct(g_pp);

	// set presentation parameters
	if(g_dmApp.bWindowed&&
	   g_bWindowedOK)
	{
		// set display mode values
		g_dmApp.fmt=g_dmSys.fmt;
		g_dmApp.bWindowed=TRUE;
		
		// set windowed presentation parameters
		g_pp.BackBufferFormat=g_dmApp.fmt;
		g_pp.BackBufferCount=g_dmApp.dwBackBufferCount;
		g_pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		g_pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		g_pp.hDeviceWindow=g_hWnd;
		g_pp.Windowed=TRUE;
		g_pp.EnableAutoDepthStencil=TRUE;
		g_pp.AutoDepthStencilFormat=D3DFMT_D16;
		if(g_dmApp.bLockable)
			g_pp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}
	else
	{
		// set values of desired display mode
		if(!SetValidRGBFormat(&g_dmApp))
		{
			DisplayErrorMessage("Unable to find suitable render formats.",
								"Your display adapter does not support a required format.",
								FALSE);
			return(FALSE);
		}
		g_dmApp.bWindowed=FALSE;

		// full-screen parameters
		g_pp.BackBufferWidth=g_dmApp.dwX;
		g_pp.BackBufferHeight=g_dmApp.dwY;
		g_pp.BackBufferFormat=g_dmApp.fmt;
		g_pp.BackBufferCount=g_dmApp.dwBackBufferCount;
		g_pp.MultiSampleType=D3DMULTISAMPLE_NONE;
		g_pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		g_pp.hDeviceWindow=g_hWnd;
		g_pp.Windowed=FALSE;
		g_pp.EnableAutoDepthStencil=TRUE;
		g_pp.AutoDepthStencilFormat=D3DFMT_D16;
		if(g_dmApp.bLockable)
			g_pp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		g_pp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		g_pp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_ONE;
	}

	// ok
	return(TRUE);
}

// InitD3DDevice()

BOOL InitD3DDevice(void)
{
	// used for error checking
	HRESULT hr;
	
	// set presentation parameters
	if(!SetPresentationParameters())
		return(FALSE);

	// create d3d device
	DWORD dwAttempt=0;
	while(dwAttempt<3)
	{
		// set behavior & vertex processing flags
		DWORD dwBehaviorFlags;
		switch(dwAttempt)
		{
		case(0):
#ifdef ALLOW_HWONLYVP
			// set hardware processing
			dwBehaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING;
			g_dwVPF=VPF_HARDWARE;
#else
			// set mixed processing
			dwBehaviorFlags=D3DCREATE_MIXED_VERTEXPROCESSING;
			g_dwVPF=VPF_MIXED;
#endif
			break;
		case(1):
			// set mixed processing
			dwBehaviorFlags=D3DCREATE_MIXED_VERTEXPROCESSING;
			g_dwVPF=VPF_MIXED;
			break;
		case(2):
			// set software processing
			dwBehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			g_dwVPF=VPF_SOFTWARE;
			break;
		}

		// attempt to create device
		hr=g_lpD3D->CreateDevice(D3DADAPTER_DEFAULT,
								 D3DDEVTYPE_HAL,
								 g_hWnd,
								 dwBehaviorFlags,
								 &g_pp,
								 &g_lpD3DDevice);
		
		// increment attempt counter
		if(FAILED(hr))
			dwAttempt++;
		else
			dwAttempt=3;
	}

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create Direct3D device object.",
							  "Your display adapter may not support a required format.",
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// get client area
	UpdateClientArea();

	// set default viewport
	SetViewport();

	// set render states
	if(!SetRenderStates())
		return(FALSE);

	// reset graphics objects
	ResetGraphicsObjects();

	// init shatter transition
	if(!InitShatter())
		return(FALSE);

	// set default environment settings
	EnableZBuffer();
	EnableAlphaTextures();
	DisableWireframe();
	SetLinePattern(1,0xFFFF);
	EnableZBufferWrites();
	EnableBackfaceCull();
	SetZBias(0);
	DisableAlphaBlend();
	DisableAlphaTest();
	DisableFog();
	DisableSpecular();
	DisableGouraud();
	DisableLinearFilter();

	// clear screen
	ClearScreen();

	// set default matrices
	SetDefaultProjectionMatrix();
	SetDefaultViewMatrix();

	// ok
	return(TRUE);
}

// ReleaseD3DDevice()

void ReleaseD3DDevice(void)
{
	// release shatter transition
	ReleaseShatter();
	
	// release pointer
	ReleaseCOMPtr(g_lpD3DDevice);
}

// UpdateClientArea()

void UpdateClientArea(void)
{
	// check device
	if(g_lpD3DDevice)
	{
		// check full-screen flag
		if(!g_pp.Windowed)
		{
			// use current render target settings
			g_rClient.dwX=g_dmApp.dwX;
			g_rClient.dwY=g_dmApp.dwY;
			g_rClient.fX=(float)g_rClient.dwX;
			g_rClient.fY=(float)g_rClient.dwY;

			// set current render size values
			g_fCurRenderX=g_rClient.fX;
			g_fCurRenderY=g_rClient.fY;

			// ok
			return;
		}
	}

	// use system client rect
	RECT rC;
	GetClientRect(g_hWnd,&rC);

	// copy values
	g_rClient.dwX=rC.right-rC.left;
	g_rClient.dwY=rC.bottom-rC.top;
	g_rClient.fX=(float)g_rClient.dwX;
	g_rClient.fY=(float)g_rClient.dwY;

	// set current render size values
	g_fCurRenderX=g_rClient.fX;
	g_fCurRenderY=g_rClient.fY;
}

// SetAlphaMode()

BOOL SetAlphaMode(void)
{
	// used for error checking
	HRESULT hr;
	
	// set source alpha blending value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,
								     D3DBLEND_SRCALPHA);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set blending source.",
							  "Your display adapter may not support alpha blending.",
							  hr);
		return(FALSE);
	}

	// set destination alpha blending value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,
								     D3DBLEND_INVSRCALPHA);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set blending destination.",
							  "Your display adapter may not support alpha blending.",
							  hr);
		return(FALSE);
	}

	// set alpha reference value
	hr=g_lpD3DDevice->SetRenderState(D3DRS_ALPHAREF,
								     0);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha reference value.",
							  "Your display adapter may not support alpha testing.",
							  hr);
		return(FALSE);
	}

	// set alpha comparison function
	hr=g_lpD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,
								     D3DCMP_GREATER);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set alpha comparison function.",
							  "Your display adapter may not support alpha testing.",
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetFogMode()

BOOL SetFogMode(void)
{
	// used for error checking
	HRESULT hr;
	
	// set fog mode
	hr=g_lpD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,
								     D3DFOG_LINEAR);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set fog blending mode.",
							  "Your display adapter may not support fog effects.",
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetTextureAddressingMode()

BOOL SetTextureAddressingMode(void)
{
	// used for error checking
	HRESULT hr;
	
	// set texture u-addressing mode
	hr=g_lpD3DDevice->SetTextureStageState(0,
										   D3DTSS_ADDRESSU,
										   D3DTADDRESS_WRAP);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set texture addressing mode.",
							  "Your display adapter may not support a required mode.",
							  hr);
		return(FALSE);
	}

	// set texture v-addressing mode
	hr=g_lpD3DDevice->SetTextureStageState(0,
										   D3DTSS_ADDRESSV,
										   D3DTADDRESS_WRAP);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to set texture addressing mode.",
							  "Your display adapter may not support a required mode.",
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// SetRenderStates()

extern BOOL SetRenderStates(void)
{
	// set alpha mode
	if(!SetAlphaMode())
		return(FALSE);

	// set fog mode
	if(!SetFogMode())
		return(FALSE);

	// set texture addressing mode
	if(!SetTextureAddressingMode())
		return(FALSE);

	// set vertex processing mode
	if(!SetVertexProcessingMode())
		return(FALSE);

	// ok
	return(TRUE);
}

// SetVertexProcessingMode()

BOOL SetVertexProcessingMode(void)
{
	// set vertex processing as needed
	switch(g_dwVPF)
	{
	case(VPF_HARDWARE):
		DisableSVP();
		break;
	case(VPF_SOFTWARE):
		EnableSVP();
		break;
	case(VPF_MIXED):
		DisableSVP();
		break;
	}

	// ok
	return(TRUE);
}

// InitDXFile()

BOOL InitDXFile(void)
{
	// used for error checking
	HRESULT hr;
	
	// create directx file interface
	hr=DirectXFileCreate(&g_lpDXFile);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to create DirectX file interface.",
							  NULL,
							  hr);
		return(FALSE);
	}
	else
		AddCOMResource;

	// register standard templates
	hr=g_lpDXFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
								     D3DRM_XTEMPLATE_BYTES);

	// check for error
	if(FAILED(hr))
	{
		DisplayDXErrorMessage("Unable to load DirectX file templates.",
							  NULL,
							  hr);
		return(FALSE);
	}

	// ok
	return(TRUE);
}

// ReleaseDXFile()

void ReleaseDXFile(void)
{
	// release pointer
	ReleaseCOMPtr(g_lpDXFile);
}

// ResetRenderEnvironment()

void ResetRenderEnvironment(void)
{
	// counter
	DWORD dwCount;
	
	// set render states
	SetRenderStates();

	// reset alpha blend
	if(g_bABlend)
		EnableAlphaBlend();
	else
		DisableAlphaBlend();

	// reset alpha test
	if(g_bATest)
		EnableAlphaTest();
	else
		DisableAlphaTest();

	// reset specular
	if(g_bSpecular)
		EnableSpecular();
	else
		DisableSpecular();

	// reset fog
	if(g_bFog)
		EnableFog();
	else
		DisableFog();

	// reset fog parameters
	SetFogColor(g_clrFog);
	SetFogStart(g_fFogStart);
	SetFogEnd(g_fFogEnd);

	// reset gouraud
	if(g_bGouraud)
		EnableGouraud();
	else
		DisableGouraud();

	// reset filtering mode
	if(g_bLinearFilter)
		EnableLinearFilter();
	else
		DisableLinearFilter();

	// reset line pattern
	SetLinePattern(g_wLPRepeat,
				   g_wLPPattern);

	// reset wireframe
	if(g_bWireframe)
		EnableWireframe();
	else
		DisableWireframe();

	// reset z-buffer
	if(g_bZBufferEnabled)
		EnableZBuffer();
	else
		DisableZBuffer();

	// reset z-buffer writes
	if(g_bZBufferWrites)
		EnableZBufferWrites();
	else
		DisableZBufferWrites();

	// reset z-bias
	SetZBias(g_dwZBias);

	// reset cull mode
	if(g_bBackfaceCull==1)
		EnableBackfaceCull();
	else if(g_bBackfaceCull==2)
		EnableFrontfaceCull();
	else
		DisableBackfaceCull();

	// reset view & projection matrices
	SetViewMatrix(&g_mtxView);
	SetProjectionMatrix(&g_mtxProj);

	// reset ambient light
	SetAmbientLight(g_clrAmbient);

	// check lights
	if(g_dwNumLight)
	{
		// restore all lights
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
		{
			// update light
			UpdateLight(dwCount,
						&g_lpLight[dwCount]);

			// enable light as needed
			if(g_lpbLightEnabled[dwCount])
				EnableLight(dwCount);
			else
				DisableLight(dwCount);
		}
	}

	// reset viewport
	ResetViewport();

	// reset fader vertex buffer
	ResetFaderVertexBuffer();

	// reset alpha textures
	if(g_bAlphaTextures)
		EnableAlphaTextures();
	else
		DisableAlphaTextures();
}

#ifdef DEBUG_MODE
// ValidateLights()

void ValidateLights(void)
{
	// counter
	DWORD dwCount;

	// check light validations
	if(g_dwNumLight)
		for(dwCount=0;dwCount<g_dwNumLight;dwCount++)
			if(g_lpbLightEnabled[dwCount]&&
			   !g_lpbLightValid[dwCount])
				DisplayErrorMessage("Warning: Light has not been validated.",
									g_lplpLightNames[dwCount],
									FALSE);
}
#endif

// ResetSpriteProjectionMatrix()

void ResetSpriteProjectionMatrix(void)
{
	// create & store matrix
	D3DXMatrixOrthoLH(&g_mtxSpriteProj,
					  g_fCurRenderX,
					  g_fCurRenderY,
					  0,1);
}
	
// ScaleRectToClient()

void ScaleRectToClient(LPRECT lpR)
{
	// scale each component
	lpR->left=  ClientCoordX(lpR->left);
	lpR->right= ClientCoordX(lpR->right);
	lpR->top=   ClientCoordY(lpR->top);
	lpR->bottom=ClientCoordY(lpR->bottom);
}

// SetViewport()

void SetViewport(float fX,
				 float fY,
				 float fWidth,
				 float fHeight,
				 float fZNear,
				 float fZFar)
{
	// save viewport values
	g_vp.x=fX;
	g_vp.y=fY;
	g_vp.w=fWidth;
	g_vp.h=fHeight;
	g_vp.n=fZNear;
	g_vp.f=fZFar;

	// set viewport structure
	D3DVIEWPORT8 vp;
	vp.X=     (DWORD)(fX*     g_fCurRenderX);
	vp.Y=     (DWORD)(fY*     g_fCurRenderY);
	vp.Width= (DWORD)(fWidth* g_fCurRenderX);
	vp.Height=(DWORD)(fHeight*g_fCurRenderY);
	vp.MinZ=fZNear;
	vp.MaxZ=fZFar;
	
	// set viewport
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetViewport(&vp);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to set viewport.",
							  NULL,
							  hr);
#endif
}

// ResetViewport()

void ResetViewport(void)
{
	// set stored viewport
	SetViewport(g_vp.x,
				g_vp.y,
				g_vp.w,
				g_vp.h,
				g_vp.n,
				g_vp.f);
}

// GetSystemDisplayFormat()

D3DFORMAT GetSystemDisplayFormat(void)
{
	// used for error checking
	HRESULT hr;
	
	// check direct3d
	if(!g_lpD3D)
		return(D3DFMT_UNKNOWN);
	
	// get adapter display mode
	D3DDISPLAYMODE dm;
	hr=g_lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
									  &dm);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to retrieve display adapter mode.",
								   g_lpAdapterID.Description,
								   hr);
		return(D3DFMT_UNKNOWN);
	}
	
	// return format
	return(dm.Format);
}

// LockMesh()

LPLOCKEDMESH LockMesh(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "LockMesh()",
								 FALSE);
		return(NULL);
	}
#endif
	
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// pointer to return
	LPLOCKEDMESH lpm;

	// allocate locked mesh structure
	if(!AllocMem((LPVOID*)&lpm,
				 sizeof(LOCKEDMESH)))
		return(NULL);

	// get number of vertices in mesh
	lpm->dwNumVtx=g_lplpMesh[dwIndex]->GetNumVertices();

	// allocate vertex array
	if(!AllocMem((LPVOID*)&lpm->lpVtx,
				 sizeof(D3DXVECTOR3)*lpm->dwNumVtx))
		return(NULL);

	// compute size of vertex
	lpm->dwVtxSize=ComputeVertexSize(g_lplpMesh[dwIndex]->GetFVF())/sizeof(float);

	// lock vertex buffer
	hr=g_lplpMesh[dwIndex]->LockVertexBuffer(NULL,
											 (LPBYTE*)&lpm->lpfVtxBuf);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to lock mesh vertex buffer.",
								   "Check available memory.",
								   hr);
		return(NULL);
	}
	
	// read vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpVtx[dwCount].x=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)];
		lpm->lpVtx[dwCount].y=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+1];
		lpm->lpVtx[dwCount].z=lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+2];
	}

	// get number of faces in mesh
	lpm->dwNumFaces=g_lplpMesh[dwIndex]->GetNumFaces();

	// get index buffer
	IDirect3DIndexBuffer8* lpidx;
	hr=g_lplpMesh[dwIndex]->GetIndexBuffer(&lpidx);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to retrieve index buffer.",
								   NULL,
								   hr);
		return(NULL);
	}

	// get buffer description
	D3DINDEXBUFFER_DESC ibd;
	hr=lpidx->GetDesc(&ibd);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to retrieve index buffer description.",
								   NULL,
								   hr);
		return(NULL);
	}

	// set index size
	if(ibd.Format==D3DFMT_INDEX16)
		lpm->dwIdxSize=sizeof(WORD);
	else if(ibd.Format==D3DFMT_INDEX32)
		lpm->dwIdxSize=sizeof(DWORD);
	else
	{
		DisplayFatalDXErrorMessage("Invalid index buffer format.",
								   NULL,
								   hr);
		return(NULL);
	}

	// lock index buffer
	hr=lpidx->Lock(0,
				   0,
				   &lpm->lpcIdxBuf,
				   NULL);

	// check for error
	if(FAILED(hr))
	{
		DisplayFatalDXErrorMessage("Unable to lock index buffer.",
								   NULL,
								   hr);
		return(NULL);
	}

	// check index size
	if(lpm->dwIdxSize==2)
	{
		// allocate memory for faces
		if(!AllocMem((LPVOID*)&lpm->lpFaces,
					 sizeof(MESHFACE)*lpm->dwNumFaces))
			return(NULL);

		// cast pointer to faces
		LPWORD lpwFaces=(LPWORD)lpm->lpcIdxBuf;

		// reset index counter
		DWORD dwIdxCount=0;
		
		// copy faces
		for(dwCount=0;dwCount<lpm->dwNumFaces;dwCount++)
		{
			lpm->lpFaces[dwCount].dwV1=(DWORD)lpwFaces[dwIdxCount++];
			lpm->lpFaces[dwCount].dwV2=(DWORD)lpwFaces[dwIdxCount++];
			lpm->lpFaces[dwCount].dwV3=(DWORD)lpwFaces[dwIdxCount++];
		}
	}
	else
		// cast pointer to faces
		lpm->lpFaces=(LPMESHFACE)lpm->lpcIdxBuf;

	// return pointer
	return(lpm);
}

// UnlockMesh()

void UnlockMesh(DWORD dwIndex,
				LPLOCKEDMESH lpm)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "UnlockMesh()",
								 FALSE);
		return;
	}
#endif
	
	// counter
	DWORD dwCount;

	// used for error checking
	HRESULT hr;
	
	// replace vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)]=lpm->lpVtx[dwCount].x;
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+1]=lpm->lpVtx[dwCount].y;
		lpm->lpfVtxBuf[(lpm->dwVtxSize*dwCount)+2]=lpm->lpVtx[dwCount].z;
	}

	// unlock vertex buffer
	hr=g_lplpMesh[dwIndex]->UnlockVertexBuffer();

	// check for error
	if(FAILED(hr))
		DisplayFatalDXErrorMessage("Unable to unlock mesh vertex buffer.",
								   NULL,
								   hr);

	// check index size
	if(lpm->dwIdxSize==2)
	{
		// cast pointer to faces
		LPWORD lpwFaces=(LPWORD)lpm->lpcIdxBuf;

		// reset index counter
		DWORD dwIdxCount=0;
		
		// replace faces
		for(dwCount=0;dwCount<lpm->dwNumFaces;dwCount++)
		{
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV1;
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV2;
			lpwFaces[dwIdxCount++]=(WORD)lpm->lpFaces[dwCount].dwV3;
		}

		// free faces memory
		FreeMem((LPVOID*)&lpm->lpFaces);
	}

	// unlock index buffer
	hr=g_lplpMesh[dwIndex]->UnlockIndexBuffer();

	// check for error
	if(FAILED(hr))
		DisplayFatalDXErrorMessage("Unable to unlock index buffer.",
								   NULL,
								   hr);

	// free memory objects
	FreeMem((LPVOID*)&lpm->lpVtx);
	FreeMem((LPVOID*)&lpm);
}

// OffsetMesh()

void OffsetMesh(DWORD dwIndex,
				float fOffsetX,
				float fOffsetY,
				float fOffsetZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "OffsetMesh()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;

	// lock mesh
	LPLOCKEDMESH lpm=LockMesh(dwIndex);

	// offset vertices
	for(dwCount=0;dwCount<lpm->dwNumVtx;dwCount++)
	{
		lpm->lpVtx[dwCount].x+=fOffsetX;
		lpm->lpVtx[dwCount].y+=fOffsetY;
		lpm->lpVtx[dwCount].z+=fOffsetZ;
	}

	// unlock mesh
	UnlockMesh(dwIndex,
			   lpm);
}

// TransformVector()

void TransformVector(LPD3DXVECTOR3 lpv,
					 LPD3DXVECTOR3 lpv1,
					 LPD3DXMATRIX lpmtx)
{
	// output vector
	D3DXVECTOR4 v;
	
	// transform vector
	D3DXVec3Transform(&v,
					  lpv1,
					  lpmtx);

	// copy values
	lpv->x=v.x;
	lpv->y=v.y;
	lpv->z=v.z;
}

// InvertMesh()

void InvertMesh(DWORD dwIndex)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMesh)
	{
		DisplayFatalErrorMessage("Invalid mesh index.",
								 "InvertMesh()",
								 FALSE);
		return;
	}
#endif

	// counter
	DWORD dwCount;
	
	// lock mesh
	LPLOCKEDMESH lm=LockMesh(dwIndex);

	// invert each face
	for(dwCount=0;dwCount<lm->dwNumFaces;dwCount++)
	{
		// swap first two indices
		DWORD dwTemp=lm->lpFaces[dwCount].dwV1;
		lm->lpFaces[dwCount].dwV1=lm->lpFaces[dwCount].dwV2;
		lm->lpFaces[dwCount].dwV2=dwTemp;
	}

	// unlock mesh
	UnlockMesh(dwIndex,
			   lm);
}

// GetPlaneNormal()

BOOL GetPlaneNormal(LPD3DXVECTOR3 lpvN,
					LPD3DXVECTOR3 lpv1,
					LPD3DXVECTOR3 lpv2,
					LPD3DXVECTOR3 lpv3)
{
	// compute vectors from point 2
	D3DXVECTOR3 vVec1=(*lpv3)-(*lpv2);
	D3DXVECTOR3 vVec2=(*lpv1)-(*lpv2);

	// compute cross product
	D3DXVECTOR3 vVec3;
	CrossProduct(&vVec3,&vVec1,&vVec2);

	// check for invalid normal
	if(IsNearZero(vVec3.x)&&
	   IsNearZero(vVec3.y)&&
	   IsNearZero(vVec3.z))
		return(FALSE);

	// normalize vector
	Normalize(lpvN,
			  &vVec3);

	// ok
	return(TRUE);
}

// IsVectorToLeft()

BOOL IsVectorToLeft(LPD3DXVECTOR3 lpV,
				    LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->x<lpV1->x)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorToRight()

BOOL IsVectorToRight(LPD3DXVECTOR3 lpV,
				     LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->x>lpV1->x)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorInFront()

BOOL IsVectorInFront(LPD3DXVECTOR3 lpV,
				     LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->z>lpV1->z)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorBehind()

BOOL IsVectorBehind(LPD3DXVECTOR3 lpV,
				    LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->z<lpV1->z)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorAbove()

BOOL IsVectorAbove(LPD3DXVECTOR3 lpV,
				   LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->y>lpV1->y)
		return(TRUE);
	else
		return(FALSE);
}

// IsVectorBelow()

BOOL IsVectorBelow(LPD3DXVECTOR3 lpV,
				   LPD3DXVECTOR3 lpV1)
{
	// check value
	if(lpV->y<lpV1->y)
		return(TRUE);
	else
		return(FALSE);
}

// ClipPolygonToPlane()

void ClipPolygonToPlane(LPPOLYGON lpPoly,
					    LPD3DXVECTOR3 lpv1,
					    LPD3DXVECTOR3 lpv2,
					    LPD3DXVECTOR3 lpv3)
{
	// do not process empty polygons
	if(lpPoly->dwNumVtx==0)
		return;
	
	// get plane normal
	D3DXVECTOR3 vN;
	if(!GetPlaneNormal(&vN,
					   lpv1,
					   lpv2,
				       lpv3))
#ifdef DEBUG_MODE
	{
		DisplayErrorMessage("Undefined plane.",
							NULL,
							FALSE);
		return;
	}
#else
		return;
#endif

	// compute d-value
	float fD=-DotProduct(&vN,
						 lpv1);

	// counters
	DWORD dwCount1;
	DWORD dwCount2;
	
	// reset output polygon
	POLYGON pgOut;
	pgOut.dwNumVtx=0;

	// set counter #1 to last vertex
	dwCount1=lpPoly->dwNumVtx-1;

	// check each edge
	for(dwCount2=0;dwCount2<lpPoly->dwNumVtx;dwCount2++)
	{		
		// get edge endpoints
		D3DXVECTOR3 v1=lpPoly->lpvVtx[dwCount1];
		D3DXVECTOR3 v2=lpPoly->lpvVtx[dwCount2];

		// compute flag of point #1
		BOOL bInside1;
		if((DotProduct(&vN,&v1)+fD)<=0)
			bInside1=TRUE;
		else
			bInside1=FALSE;

		// compute flag of point #2
		BOOL bInside2;
		if((DotProduct(&vN,&v2)+fD)<=0)
			bInside2=TRUE;
		else
			bInside2=FALSE;

		// check edge type
		if(bInside1)
		{
			if(bInside2)
			{
				// entirely inside
				pgOut.lpvVtx[pgOut.dwNumVtx++]=v2;
			}
			else
			{
				// inside to outside
				D3DXVECTOR3 vI;
				Get3DLinePlaneIntersection(&vI,
										   &v1,
										   &v2,
										   lpv1,
										   lpv2,
										   lpv3);

				// copy value
				pgOut.lpvVtx[pgOut.dwNumVtx++]=vI;
			}
		}
		else
		{
			if(bInside2)
			{
				// outside to inside
				D3DXVECTOR3 vI;
				Get3DLinePlaneIntersection(&vI,
										   &v1,
										   &v2,
										   lpv1,
										   lpv2,
										   lpv3);

				// copy values
				pgOut.lpvVtx[pgOut.dwNumVtx++]=vI;
				pgOut.lpvVtx[pgOut.dwNumVtx++]=v2;
			}
			else
			{
				// entirely outside, do nothing
			}
		}
		
		// update counter #1
		dwCount1=dwCount2;
	}

	// copy clipped polygon to original
	(*lpPoly)=pgOut;
}

// DoesTriangleIntersectBox()

BOOL DoesTriangleIntersectBox(LPD3DXVECTOR3 lpv1,
							  LPD3DXVECTOR3 lpv2,
							  LPD3DXVECTOR3 lpv3,
							  LPD3DXVECTOR3 lpvMin,
							  LPD3DXVECTOR3 lpvMax)
{
	// set up polygon
	POLYGON pgTri;
	pgTri.dwNumVtx=3;
	pgTri.lpvVtx[0]=(*lpv1);
	pgTri.lpvVtx[1]=(*lpv2);
	pgTri.lpvVtx[2]=(*lpv3);

	// set up box vertices
	D3DXVECTOR3 v1;
	v1.x=lpvMin->x;
	v1.y=lpvMin->y;
	v1.z=lpvMin->z;
	D3DXVECTOR3 v2;
	v2.x=lpvMax->x;
	v2.y=lpvMin->y;
	v2.z=lpvMin->z;
	D3DXVECTOR3 v3;
	v3.x=lpvMax->x;
	v3.y=lpvMin->y;
	v3.z=lpvMax->z;
	D3DXVECTOR3 v4;
	v4.x=lpvMin->x;
	v4.y=lpvMin->y;
	v4.z=lpvMax->z;
	D3DXVECTOR3 v5;
	v5.x=lpvMin->x;
	v5.y=lpvMax->y;
	v5.z=lpvMin->z;
	D3DXVECTOR3 v6;
	v6.x=lpvMax->x;
	v6.y=lpvMax->y;
	v6.z=lpvMin->z;
	D3DXVECTOR3 v7;
	v7.x=lpvMax->x;
	v7.y=lpvMax->y;
	v7.z=lpvMax->z;
	D3DXVECTOR3 v8;
	v8.x=lpvMin->x;
	v8.y=lpvMax->y;
	v8.z=lpvMax->z;

	// clip polygon to each plane
	ClipPolygonToPlane(&pgTri,&v1,&v4,&v8);
	ClipPolygonToPlane(&pgTri,&v2,&v7,&v3);
	ClipPolygonToPlane(&pgTri,&v1,&v5,&v6);
	ClipPolygonToPlane(&pgTri,&v8,&v4,&v3);
	ClipPolygonToPlane(&pgTri,&v1,&v2,&v3);
	ClipPolygonToPlane(&pgTri,&v5,&v8,&v7);

	// get centroid of clipped polygon
	D3DXVECTOR3 vC;
	if(GetPolygonCentroid(&vC,&pgTri))
	{
		// check for enclosure of centroid
		if(IsPointInBox(&vC,
						lpvMin,
						lpvMax))
			return(TRUE);
	}

	// no intersection found
	return(FALSE);
}

// Get3DLinePlaneIntersection()

BOOL Get3DLinePlaneIntersection(LPD3DXVECTOR3 lpvI,
								LPD3DXVECTOR3 lpvL1,
								LPD3DXVECTOR3 lpvL2,
								LPD3DXVECTOR3 lpvP1,
								LPD3DXVECTOR3 lpvP2,
								LPD3DXVECTOR3 lpvP3)
{
	// compute plane equation
	float fA=(lpvP1->y*(lpvP2->z-lpvP3->z))+
			 (lpvP2->y*(lpvP3->z-lpvP1->z))+
			 (lpvP3->y*(lpvP1->z-lpvP2->z));
	float fB=(lpvP1->z*(lpvP2->x-lpvP3->x))+
			 (lpvP2->z*(lpvP3->x-lpvP1->x))+
			 (lpvP3->z*(lpvP1->x-lpvP2->x));
	float fC=(lpvP1->x*(lpvP2->y-lpvP3->y))+
			 (lpvP2->x*(lpvP3->y-lpvP1->y))+
			 (lpvP3->x*(lpvP1->y-lpvP2->y));
	float fD=-(lpvP1->x*((lpvP2->y*lpvP3->z)-(lpvP3->y*lpvP2->z)))
			 -(lpvP2->x*((lpvP3->y*lpvP1->z)-(lpvP1->y*lpvP3->z)))
			 -(lpvP3->x*((lpvP1->y*lpvP2->z)-(lpvP2->y*lpvP1->z)));

	// compute line deltas
	float fI=lpvL2->x-lpvL1->x;
	float fJ=lpvL2->y-lpvL1->y;
	float fK=lpvL2->z-lpvL1->z;

	// compute intersect denominator
	float fU=(fA*fI)+(fB*fJ)+(fC*fK);

	// check denominator
	if(fU==0)
	{
		// line is parallel
		return(FALSE);
	}

	// compute t-value
	float fT=-((fA*lpvL1->x)+
			   (fB*lpvL1->y)+
			   (fC*lpvL1->z)+fD)/fU;
	
	// compute intersection
	lpvI->x=lpvL1->x+(fI*fT);
	lpvI->y=lpvL1->y+(fJ*fT);
	lpvI->z=lpvL1->z+(fK*fT);

	// ok
	return(TRUE);
}

// GetPolygonCentroid()

BOOL GetPolygonCentroid(LPD3DXVECTOR3 lpvC,
						LPPOLYGON lpPoly)
{
	// counter
	DWORD dwCount;

	// check number of vertices
	if(lpPoly->dwNumVtx==0)
		return(FALSE);

	// reset centroid
	(*lpvC)=D3DXVECTOR3(0,0,0);

	// add all vertices
	for(dwCount=0;dwCount<lpPoly->dwNumVtx;dwCount++)
		(*lpvC)+=lpPoly->lpvVtx[dwCount];

	// average vertices
	(*lpvC)/=((float)lpPoly->dwNumVtx);

	// ok
	return(TRUE);
}

// IsPointInBox()

BOOL IsPointInBox(LPD3DXVECTOR3 lpvP,
				  LPD3DXVECTOR3 lpvMin,
				  LPD3DXVECTOR3 lpvMax)
{
	// check point
	return(lpvMin->x<=lpvP->x&&
		   lpvMax->x>=lpvP->x&&
		   lpvMin->y<=lpvP->y&&
		   lpvMax->y>=lpvP->y&&
	       lpvMin->z<=lpvP->z&&
	       lpvMax->z>=lpvP->z);
}

// ComputeVertexSize()

DWORD ComputeVertexSize(DWORD dwFVF)
{
	// counter
	DWORD dwCount;
	
	// reset size
	DWORD dwSize=0;

	// mask xyz bits
	DWORD dwXYZ=dwFVF&((DWORD)0x0E);

	// add xyz values
	switch(dwXYZ)
	{
	case(D3DFVF_XYZ):
		// xyz
		dwSize+=sizeof(float)*3;
		break;
	case(D3DFVF_XYZRHW):
		// xyz+rhw
		dwSize+=sizeof(float)*4;
		break;
	case(D3DFVF_XYZB1):
		// xyz+(1) weight
		dwSize+=sizeof(float)*4;
		break;
	case(D3DFVF_XYZB2):
		// xyz+(2) weights
		dwSize+=sizeof(float)*5;
		break;
	case(D3DFVF_XYZB3):
		// xyz+(3) weights
		dwSize+=sizeof(float)*6;
		break;
	case(D3DFVF_XYZB4):
		// xyz+(4) weights
		dwSize+=sizeof(float)*7;
		break;
	case(D3DFVF_XYZB5):
		// xyz+(5) weights
		dwSize+=sizeof(float)*8;
		break;
	}

	// add normal
	if(dwFVF&D3DFVF_NORMAL)
		dwSize+=sizeof(float)*3;
	
	// add point size
	if(dwFVF&D3DFVF_PSIZE)
		dwSize+=sizeof(float);
	
	// add diffuse
	if(dwFVF&D3DFVF_DIFFUSE)
		dwSize+=sizeof(DWORD);
	
	// add specular
	if(dwFVF&D3DFVF_SPECULAR)
		dwSize+=sizeof(DWORD);

	// get number of texture coordinates
	DWORD dwNumTex=(dwFVF&((DWORD)0x0F00))>>8;

	// add texture coordinates
	if(dwNumTex)
	{
		// add each coordinate
		for(dwCount=0;dwCount<dwNumTex;dwCount++)
		{
			// mask size of texture coordinate
			DWORD dwTexSize=(dwFVF>>(16+(dwCount*2)))&((DWORD)0x03);

			// add texture coordinates
			switch(dwTexSize)
			{
			case(0x00):
				// (2) coordinates
				dwSize+=sizeof(float)*2;
				break;
			case(0x01):
				// (3) coordinates
				dwSize+=sizeof(float)*3;
				break;
			case(0x02):
				// (4) coordinates
				dwSize+=sizeof(float)*4;
				break;
			case(0x03):
				// (1) coordinate
				dwSize+=sizeof(float);
				break;
			}
		}
	}

	// return size
	return(dwSize);
}

#ifdef DEBUG_MODE
// DisplayRenderSettings()

void DisplayRenderSettings(void)
{
	// prepare message
	sprintf(g_lpMsg,
			"ABlend=%d\n"
			"ATest=%d\n"
			"Spec=%d\n"
			"Fog=%d\n"
			"FogClr=%d\n"
			"FogS=%f\n"
			"FogE=%f\n"
			"Amb=%d\n"
			"Lin=%d\n"
			"Gou=%d\n"
			"Wire=%d\n"
			"Pat=%d\n"
			"Rep=%d\n"
			"ZBuf=%d\n"
			"ZWri=%d\n"
			"Cull=%d\n"
			"ZBias=%d\n"
			"VPF=%d\n",
			g_bABlend,
			g_bATest,
			g_bSpecular,
			g_bFog,
			g_clrFog,
			g_fFogStart,
			g_fFogEnd,
			g_clrAmbient,
			g_bLinearFilter,
			g_bGouraud,
			g_bWireframe,
			g_wLPPattern,
			g_wLPRepeat,
			g_bZBufferEnabled,
			g_bZBufferWrites,
			g_bBackfaceCull,
			g_dwZBias,
			g_dwVPF);

	// display message
	DisplayInfoMessage(g_lpMsg);
}
#endif

// SetViewMatrixA()

void SetViewMatrixA(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_VIEW,
									lpMtx);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set view matrix.",NULL,hr);
#endif

		// invert view matrix
		D3DXMATRIX mView;
		D3DXMatrixInverse(&mView,
						  NULL,
						  lpMtx);
		
		// set billboard vectors
		D3DXVECTOR3 vAhead=D3DXVECTOR3(mView._31,
									   mView._32,
									   mView._33);
		SetBillboardVectors(&vAhead);
	}
	else
	{
		// set identity matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_VIEW,
									&g_mtxIdentity);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set view matrix.",NULL,hr);
#endif

		// set billboard vectors
		D3DXVECTOR3 vAhead=D3DXVECTOR3(0,0,-1);
		SetBillboardVectors(&vAhead);
	}
}

// SetProjectionMatrixA()

void SetProjectionMatrixA(LPD3DXMATRIX lpMtx)
{
	// check pointer
	if(lpMtx)
	{
		// set specified matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_PROJECTION,
									lpMtx);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set projection matrix.",NULL,hr);
#endif
	}
	else
	{
		// set identity matrix
#ifdef DEBUG_MODE
		HRESULT hr=
#endif
		g_lpD3DDevice->SetTransform(D3DTS_PROJECTION,
									&g_mtxIdentity);
#ifdef DEBUG_MODE
		if(FAILED(hr))
			DisplayDXErrorMessage("Unable to set projection matrix.",NULL,hr);
#endif
	}
}

// SetProjectionMatrixA()

void SetProjectionMatrixA(float fClipNear,
						  float fClipFar,
						  float fAngle,
						  float fAspect)
{
	// prepare projection matrix
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
							   DegToRad(fAngle),
							   fAspect,
							   fClipNear,
							   fClipFar);
	
	// set projection matrix
	SetProjectionMatrixA(&mProj);
}

// SetMatrixPosition()

void SetMatrixPosition(DWORD dwIndex,
					   LPD3DXVECTOR3 lpvPos)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrixPosition()",
								 FALSE);
		return;
	}
#endif

	// copy values to matrix
	g_lpMtx[dwIndex].mtx._41=lpvPos->x;
	g_lpMtx[dwIndex].mtx._42=lpvPos->y;
	g_lpMtx[dwIndex].mtx._43=lpvPos->z;
}

// SetMatrixPosition()

void SetMatrixPosition(DWORD dwIndex,
					   float fX,
					   float fY,
					   float fZ)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwIndex>=g_dwNumMtx)
	{
		DisplayFatalErrorMessage("Invalid matrix index.",
								 "SetMatrixPosition()",
								 FALSE);
		return;
	}
#endif

	// prepare vector
	D3DXVECTOR3 vPos=D3DXVECTOR3(fX,fY,fZ);

	// use other function
	SetMatrixPosition(dwIndex,&vPos);
}

// SetDefaultProjectionMatrix()

void SetDefaultProjectionMatrix(void)
{
	// set projection matrix
	SetProjectionMatrix(1,100,90,ASPECT_RATIO);
}

// SetDefaultViewMatrix()

void SetDefaultViewMatrix(void)
{
	// set view matrix
	SetViewMatrix(0,0,0,
				  0,0,1);
}

// BeginShadedSprites()

BOOL BeginShadedSprites(BOOL bLinear)
{
#ifdef DEBUG_MODE
	// set flag
	g_bShadedSprites=TRUE;
#endif

	// begin sprites
	return(BeginSprites(bLinear));
}

// OutputShadedSprite()

void OutputShadedSprite(DWORD dwTexIndex,
						D3DCOLOR clr,
						float fX,
						float fY,
						float fXSize,
						float fYSize,
						float fRot)
{
#ifdef DEBUG_MODE
	// check flag
	if(!g_bShadedSprites)
		DisplayInfoMessage("Shaded sprite in unshaded mode!");
#endif

#ifdef DEBUG_MODE
	// reset flag
	g_bShadedSprites=FALSE;
#endif

	// prepare material
	D3DMATERIAL8 mat={0,0,0,GetA888(clr),
					  0,0,0,GetA888(clr),
					  0,0,0,GetA888(clr),
					  GetR888(clr),
					  GetG888(clr),
					  GetB888(clr),
					  GetA888(clr),
					  1};
	
	// update sprite material
	UpdateMaterial(g_dwSpriteMat,
				   &mat);

	// set material
	SetMaterial(g_dwSpriteMat);
	
	// output sprite
	OutputSprite(dwTexIndex,
				 fX,fY,
				 fXSize,fYSize,
				 fRot);

#ifdef DEBUG_MODE
	// reset flag
	g_bShadedSprites=TRUE;
#endif
}

void EndShadedSprites(void)
{
#ifdef DEBUG_MODE
	// set flag
	g_bShadedSprites=FALSE;
#endif

	// prepare material
	D3DMATERIAL8 mat={0,0,0,1,
					  0,0,0,1,
					  0,0,0,1,
					  1,1,1,1,
					  1};
	
	// update sprite material
	UpdateMaterial(g_dwSpriteMat,
				   &mat);

	// end sprites
	EndSprites();
}

// EnableAlphaTextures()

void EnableAlphaTextures(void)
{
	// enable alpha textures
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_ALPHAARG1,
										D3DTA_TEXTURE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to enable alpha textures.",NULL,hr);
#endif
	
	// set flag
	g_bAlphaTextures=TRUE;
}

// DisableAlphaTextures()

void DisableAlphaTextures(void)
{
	// disable alpha textures
#ifdef DEBUG_MODE
	HRESULT hr=
#endif
	g_lpD3DDevice->SetTextureStageState(0,
										D3DTSS_ALPHAARG1,
										D3DTA_DIFFUSE);
#ifdef DEBUG_MODE
	if(FAILED(hr))
		DisplayDXErrorMessage("Unable to disable alpha textures.",NULL,hr);
#endif
	
	// set flag
	g_bAlphaTextures=FALSE;
}

// RotFromAngle()

float RotFromAngle(float fAngle)
{
	// reset return value
	float fRet=0;
	
	// clip angle if necessary
	while(fAngle<0)
		fAngle+=360;
	while(fAngle>360)
		fAngle-=360;

	// subtract 45 degree angles
	while(fAngle>45)
	{
		fRet+=1;
		fAngle-=45;
	}

	// convert remaining angle to radians
	float fRads=DegToRad(fAngle);

	// get sin & cos of angle
	float fCos=cosf(fRads);
	float fSin=sinf(fRads);

	// intersection points
	float fX;
	float fY;

	// get intersection
	Get2DLineIntersection(&fX,&fY,
						  0,0,
						  fCos,fSin,
						  1,0,
						  1,1);

	// add remaining value to return value
	fRet+=fY;

	// ok
	return(fRet);
}

// InitTransitions()

void InitTransitions(void)
{
	// add vertex buffer
	g_dwTransVtxBuf=AddVertexBuffer(D3DFVF_TRANSVERTEX,
									sizeof(TRANSVERTEX),
									MAX_TRANSVERTEX);

	// add index buffer
	g_dwTransIdxBuf=AddIndexBuffer(D3DPT_TRIANGLELIST,
								   MAX_TRANSTRIANGLE);
}

// SetReverseTransition()

void SetReverseTransition(TRANSTYPE tt,
						  DWORD dwData1,
						  DWORD dwData2,
						  DWORD dwData3,
						  DWORD dwData4)
{
	// set transition
	SetTransition(tt,
				  dwData1,
				  dwData2,
				  dwData3,
				  dwData4);

	// set reverse flag
	g_bTransRev=TRUE;
}

// SetTransition()

void SetTransition(TRANSTYPE tt,
				   DWORD dwData1,
				   DWORD dwData2,
				   DWORD dwData3,
				   DWORD dwData4)
{
	// internal variables
	LPWORD lpIdx;
	DWORD dwX;
	DWORD dwY;
	BOOL bRev;

	// save transition type
	g_ttCurTrans=tt;

	// reset reverse flag
	g_bTransRev=FALSE;

	// check transition type for general settings
	switch(tt)
	{
	// tex,color
	case(TT_FADEIN):
	case(TT_FADEOUT):
		// copy texture index
		g_lpdwTransData[TD_FADE_TEX]=dwData1;

		// copy color value
		g_lpdwTransData[TD_FADE_COLOR]=dwData2;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData3;

		// ok
		break;

	// tex1,tex2,color
	case(TT_FLASH):
		// copy texture indices
		g_lpdwTransData[TD_FLASH_TEX1]=dwData1;
		g_lpdwTransData[TD_FLASH_TEX2]=dwData2;

		// copy color
		g_lpdwTransData[TD_FLASH_COLOR]=dwData3;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData4;

		// ok
		break;

	// tex1,tex2
	case(TT_TWEEN):
	case(TT_SHATTER):
		// copy texture indices
		g_lpdwTransData[TD_TWEEN_TEX1]=dwData1;
		g_lpdwTransData[TD_TWEEN_TEX2]=dwData2;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData3;

		// ok
		break;

	// tex1,tex2,direction
	case(TT_PEEL):
	case(TT_SWING):
	case(TT_SCROLL):
	case(TT_SQUISH):
	case(TT_SLIDE):
		// copy texture indices
		g_lpdwTransData[TD_PEEL_TEX1]=dwData1;
		g_lpdwTransData[TD_PEEL_TEX2]=dwData2;

		// copy direction
		g_lpdwTransData[TD_PEEL_DIR]=dwData3;

		// copy timing values
		g_dwTransCurPos=0;
		g_dwTransEndPos=dwData4;

		// ok
		break;

	// random
	case(TT_RANDOM):
		// create random trasition
		switch(abs(GetRandomInt())%5)
		{
		case(0): tt=TT_PEEL;   break;
		case(1): tt=TT_SWING;  break;
		case(2): tt=TT_SCROLL; break;
		case(3): tt=TT_SLIDE;  break;
		case(4): tt=TT_SQUISH; break;
		}

		// set reverse flag
		if(GetRandomInt()%2)
			bRev=TRUE;
		else
			bRev=FALSE;

		// get random direction
		dwData4=(DWORD)((abs(GetRandomInt())%4)+1);
		
		// set new transition
		if(bRev)
			SetTransition(tt,
						  dwData1,
						  dwData2,
						  dwData4,
						  dwData3);
		else
			SetReverseTransition(tt,
								 dwData2,
								 dwData1,
								 dwData4,
								 dwData3);

		// ok
		return;
	}

	// check transition type for index buffer
	switch(tt)
	{
	// hi-res, 2-sided mesh
	case(TT_PEEL):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		for(dwY=0;dwY<7;dwY++)
			for(dwX=0;dwX<7;dwX++)
			{
				// compute positions
				WORD wVtxPos=(WORD)(dwX+(dwY*8));
				WORD wIdxPos=(WORD)(dwX+(dwY*7))*12;

				// set indices
				lpIdx[wIdxPos   ]=wVtxPos+1;
				lpIdx[wIdxPos+1 ]=wVtxPos;
				lpIdx[wIdxPos+2 ]=wVtxPos+8;
				lpIdx[wIdxPos+3 ]=wVtxPos+1;
				lpIdx[wIdxPos+4 ]=wVtxPos+8;
				lpIdx[wIdxPos+5 ]=wVtxPos+9;
				lpIdx[wIdxPos+6 ]=wVtxPos;
				lpIdx[wIdxPos+7 ]=wVtxPos+1;
				lpIdx[wIdxPos+8 ]=wVtxPos+8;
				lpIdx[wIdxPos+9 ]=wVtxPos+8;
				lpIdx[wIdxPos+10]=wVtxPos+1;
				lpIdx[wIdxPos+11]=wVtxPos+9;
			}
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=196;

		// ok
		break;

	// low-res, 1-sided mesh
	case(TT_SWING):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		lpIdx[0]=2;
		lpIdx[1]=1;
		lpIdx[2]=0;
		lpIdx[3]=1;
		lpIdx[4]=2;
		lpIdx[5]=3;
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=2;

		// ok
		break;

	// shatter mesh
	case(TT_SHATTER):
		// prepare index buffer
		lpIdx=LockIndexBuffer(g_dwTransIdxBuf);
		for(dwX=0;dwX<20;dwX++)
		{
			// compute positions
			WORD wVtxPos=(WORD)(dwX*3);
			WORD wIdxPos=(WORD)(dwX*6);

			// set indices
			lpIdx[wIdxPos  ]=wVtxPos;
			lpIdx[wIdxPos+1]=wVtxPos+1;
			lpIdx[wIdxPos+2]=wVtxPos+2;
			lpIdx[wIdxPos+3]=wVtxPos+1;
			lpIdx[wIdxPos+4]=wVtxPos;
			lpIdx[wIdxPos+5]=wVtxPos+2;
		}
		UnlockIndexBuffer(g_dwTransIdxBuf);

		// set number of primitives
		g_lpdwNumPrim[g_dwTransIdxBuf]=40;

		// randomize shatter
		RandomizeShatter();

		// ok
		break;
	}
}

// RenderTransition()

BOOL RenderTransition(void)
{
	// internal variables
	float fAlpha;
	BOOL bAlphaTextures;
	LPTRANSVERTEX lpVtx;
	DWORD dwX;
	DWORD dwY;
	D3DXMATRIX mProj;
	float fTU;
	float fTV;
	float fX;
	float fY;
	float fZ;
	float fAngle;
	float fCurveX[8];
	float fCurveY[8];
	float fXPos1;
	float fYPos1;
	float fXSize1;
	float fYSize1;
	float fXPos2;
	float fYPos2;
	float fXSize2;
	float fYSize2;

	// reset continue flag
	BOOL bContinue=TRUE;
	
	// update timer
	g_dwTransCurPos+=g_dwFrameSkip;

	// check for overflow
	if(g_dwTransCurPos>g_dwTransEndPos)
	{
		// clip current position
		g_dwTransCurPos=g_dwTransEndPos;
		
		// transition complete
		bContinue=FALSE;
	}

	// compute transition position
	float fPos=((float)(g_dwTransCurPos))/((float)(g_dwTransEndPos));

	// reverse position if necessary
	if(g_bTransRev)
		fPos=1-fPos;

	// clear screen
	ClearScreen();

	// begin scene
	if(!BeginScene())
		return(FALSE);

	// check transition type
	switch(g_ttCurTrans)
	{
	// no transition set
	case(TT_NULL):
		// transition complete
		bContinue=FALSE;

		// ok
		break;

	// fade in
	case(TT_FADEIN):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture
			OutputSprite(g_lpdwTransData[TD_FADE_TEX],
	 					 ScrCoords(320,240),
						 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetG888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetB888(g_lpdwTransData[TD_FADE_COLOR]),
									   1-fPos));

		// ok
		break;

	// fade out
	case(TT_FADEOUT):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture
			OutputSprite(g_lpdwTransData[TD_FADE_TEX],
	 					 ScrCoords(320,240),
						 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetG888(g_lpdwTransData[TD_FADE_COLOR]),
									   GetB888(g_lpdwTransData[TD_FADE_COLOR]),
									   fPos));
		// ok
		break;

	// flash
	case(TT_FLASH):
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render appropriate texture
			if(fPos<0.5F)
				OutputSprite(g_lpdwTransData[TD_FLASH_TEX1],
	 						 ScrCoords(320,240),
							 ScrSize(640,480));
			else
				OutputSprite(g_lpdwTransData[TD_FLASH_TEX2],
	 						 ScrCoords(320,240),
							 ScrSize(640,480));
			
			// end sprites
			EndSprites();
		}

		// compute alpha
		fAlpha=1-(fabsf(fPos-0.5F)*2);
		
		// fade screen
		FadeScreen(D3DCOLOR_COLORVALUE(GetR888(g_lpdwTransData[TD_FLASH_COLOR]),
									   GetG888(g_lpdwTransData[TD_FLASH_COLOR]),
									   GetB888(g_lpdwTransData[TD_FLASH_COLOR]),
									   fAlpha));
		// ok
		break;

	// tween
	case(TT_TWEEN):
		// copy alpha textures flag
		bAlphaTextures=g_bAlphaTextures;

		// check alpha textures flag
		if(bAlphaTextures)
			DisableAlphaTextures();
		
		// begin sprites
		if(BeginShadedSprites(TRUE))
		{
			// render texture 1
			OutputShadedSprite(g_lpdwTransData[TD_TWEEN_TEX1],
							   D3DCOLOR_COLORVALUE(1,1,1,1),
	 						   ScrCoords(320,240),
							   ScrSize(640,480));
			
			// render texture 2
			OutputShadedSprite(g_lpdwTransData[TD_TWEEN_TEX2],
							   D3DCOLOR_COLORVALUE(1,1,1,fPos),
	 						   ScrCoords(320,240),
							   ScrSize(640,480));

			// end sprites
			EndShadedSprites();
		}

		// check alpha textures flag
		if(bAlphaTextures)
			EnableAlphaTextures();
		
		// ok
		break;

	// peel
	case(TT_PEEL):
		// compute angle
		fAngle=fPos*0.4F;

		// compute curve values
		fCurveX[0]=0;
		fCurveY[0]=0;
		for(dwX=1;dwX<8;dwX++)
		{
			fCurveX[dwX]=fCurveX[dwX-1]+(cosf(fAngle*((float)(dwX)))/7);
			fCurveY[dwX]=fCurveY[dwX-1]+(sinf(fAngle*((float)(dwX)))/7);
		}

		// prepare vertex buffer
		lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);
		for(dwY=0;dwY<8;dwY++)
			for(dwX=0;dwX<8;dwX++)
			{
				// compute texture coordinates
				fTU=((float)(dwX))/7;
				fTV=((float)(dwY))/7;
				
				// compute position based on direction
				switch(g_lpdwTransData[TD_PEEL_DIR])
				{
				case(TDR_LTOR):
					fX=-((fCurveX[7-dwX]-(fPos*0.15F))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fCurveY[7-dwX]*640);
					break;
				case(TDR_RTOL):
					fX=((fCurveX[dwX]-(fPos*0.15F))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fCurveY[dwX]*640);
					break;
				case(TDR_BTOT):
					fX=(fTU-0.5F)*640;
					fY=-((fCurveX[7-dwY]-(fPos*0.15F))-0.5F)*480;
					fZ=579.5F-(fCurveY[7-dwY]*480);
					break;
				case(TDR_TTOB):
					fX=(fTU-0.5F)*640;
					fY=((fCurveX[dwY]-(fPos*0.15F))-0.5F)*480;
					fZ=579.5F-(fCurveY[dwY]*480);
					break;
				}

				// set vertex
				SET_TRANSVERTEX(lpVtx[dwX+(dwY*8)],
								fX,fY,fZ,
								fTU,1-fTV);
			}
		UnlockVertexBuffer(g_dwTransVtxBuf);

		// point for future reference
		RenderTransition1:;

		// begin sprites
		if(BeginSprites(TRUE))
		{
			// output texture 2
			OutputSprite(g_lpdwTransData[TD_PEEL_TEX2],
						 ScrCoords(320,240),
						 ScrSize(640,480));

			// end sprites
			EndSprites();
		}
		
		// save projection matrix
		CopyMemory(&mProj,
				   &g_mtxProj,
				   sizeof(D3DXMATRIX));
		

		// output texture 1
		SetWorldMatrix();
		SetViewMatrix();
		SetProjectionMatrixA(1,1000,45);
		SetVertexBuffer(g_dwTransVtxBuf);
		SetIndexBuffer(g_dwTransIdxBuf);
		SetMaterial(g_dwSpriteMat);
		SetTexture(g_lpdwTransData[TD_PEEL_TEX1]);
		RenderPrimitive();

		// reset projection matrix
		SetProjectionMatrixA(&mProj);
		
		// ok
		break;

	// swing
	case(TT_SWING):
		// compute angle
		fAngle=fPos*PI*0.5F;

		// prepare vertex buffer
		lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);
		for(dwY=0;dwY<2;dwY++)
			for(dwX=0;dwX<2;dwX++)
			{
				// compute texture coordinates
				fTU=(float)(dwX);
				fTV=(float)(dwY);
				
				// compute position based on direction
				switch(g_lpdwTransData[TD_PEEL_DIR])
				{
				case(TDR_LTOR):
					fX=-(((1-fTU)*cosf(fAngle))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-((1-fTU)*sinf(fAngle)*640);
					break;
				case(TDR_RTOL):
					fX=((fTU*cosf(fAngle))-0.5F)*640;
					fY=(fTV-0.5F)*480;
					fZ=579.5F-(fTU*sinf(fAngle)*640);
					break;
				case(TDR_BTOT):
					fX=(fTU-0.5F)*640;
					fY=-(((1-fTV)*cosf(fAngle))-0.5F)*480;
					fZ=579.5F-((1-fTV)*sinf(fAngle)*480);
					break;
				case(TDR_TTOB):
					fX=(fTU-0.5F)*640;
					fY=((fTV*cosf(fAngle))-0.5F)*480;
					fZ=579.5F-(fTV*sinf(fAngle)*480);
					break;
				}

				// set vertex
				SET_TRANSVERTEX(lpVtx[dwX+(dwY*2)],
								fX,fY,fZ,
								fTU,1-fTV);
			}
		UnlockVertexBuffer(g_dwTransVtxBuf);

		// jump to previous code
		goto RenderTransition1;

	// scroll
	case(TT_SCROLL):
		// set texture sizes
		fXSize1=1;
		fYSize1=1;
		fXSize2=1;
		fYSize2=1;

		// check direction
		switch(g_lpdwTransData[TD_SCROLL_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos;
			fYPos1=0;
			fXPos2=fPos-1;
			fYPos2=0;
			break;
		case(TDR_RTOL):
			fXPos1=(1-fPos)-1;
			fYPos1=0;
			fXPos2=1-fPos;
			fYPos2=0;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos;
			fXPos2=0;
			fYPos2=fPos-1;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=(1-fPos)-1;
			fXPos2=0;
			fYPos2=(1-fPos);
			break;
		}

		// point for future reference
		RenderTransition2:;
				
		// begin sprites
		if(BeginSprites(TRUE))
		{
			// render texture 1
			OutputSprite(g_lpdwTransData[TD_SCROLL_TEX1],
	 					 fXPos1,fYPos1,
						 fXSize1,fYSize1);
			
			// render texture 2
			OutputSprite(g_lpdwTransData[TD_SCROLL_TEX2],
	 					 fXPos2,fYPos2,
						 fXSize2,fYSize2);
			
			// end sprites
			EndSprites();
		}

		// ok
		break;

	// squish
	case(TT_SQUISH):
		// set texture sizes
		fXSize2=1;
		fYSize2=1;

		// check direction
		switch(g_lpdwTransData[TD_SQUISH_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=fPos-1;
			fYPos2=0;
			break;
		case(TDR_RTOL):
			fXPos1=((1-fPos)-1)*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=1-fPos;
			fYPos2=0;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=fPos-1;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=((1-fPos)-1)*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(1-fPos);
			break;
		}

		// jump to previous code
		goto RenderTransition2;

	// slide
	case(TT_SLIDE):
		// check direction
		switch(g_lpdwTransData[TD_SLIDE_DIR])
		{
		case(TDR_LTOR):
			fXPos1=fPos*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=(fPos-1)*0.5F;
			fYPos2=0;
			fXSize2=fPos;
			fYSize2=1;
			break;
		case(TDR_RTOL):
			fXPos1=((1-fPos)-1)*0.5F;
			fYPos1=0;
			fXSize1=1-fPos;
			fYSize1=1;
			fXPos2=(1-fPos)*0.5F;
			fYPos2=0;
			fXSize2=fPos;
			fYSize2=1;
			break;
		case(TDR_BTOT):
			fXPos1=0;
			fYPos1=fPos*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(fPos-1)*0.5F;
			fXSize2=1;
			fYSize2=fPos;
			break;
		case(TDR_TTOB):
			fXPos1=0;
			fYPos1=((1-fPos)-1)*0.5F;
			fXSize1=1;
			fYSize1=1-fPos;
			fXPos2=0;
			fYPos2=(1-fPos)*0.5F;
			fXSize2=1;
			fYSize2=fPos;
			break;
		}

		// jump to previous code
		goto RenderTransition2;

	// shatter
	case(TT_SHATTER):
		// render shatter
		RenderShatter(fPos);

		// ok
		break;
	}

	// end scene
	EndScene();

	// present scene
	PresentScene();

	// ok
	return(bContinue);
}

// InitMouseCursor()

void InitMouseCursor(void)
{
	// create texture
	g_dwDefMouseCursorTexIdx=AddTexture(4,4,
										TRUE);
	
	// lock texture
	D3DLOCKED_RECT lr=LockTexture(g_dwDefMouseCursorTexIdx);

	// get texture surface data
	LPWORD lpData=(LPWORD)lr.pBits;
	DWORD dwPitch=lr.Pitch>>1;

	// set bits
	lpData[0+(0*dwPitch)]=0xFFFF;
	lpData[1+(0*dwPitch)]=0xFFFF;
	lpData[2+(0*dwPitch)]=0xFFFF;
	lpData[3+(0*dwPitch)]=0x0000;
	lpData[0+(1*dwPitch)]=0xFFFF;
	lpData[1+(1*dwPitch)]=0xFFFF;
	lpData[2+(1*dwPitch)]=0x0000;
	lpData[3+(1*dwPitch)]=0x0000;
	lpData[0+(2*dwPitch)]=0xFFFF;
	lpData[1+(2*dwPitch)]=0x0000;
	lpData[2+(2*dwPitch)]=0xFFFF;
	lpData[3+(2*dwPitch)]=0x0000;
	lpData[0+(3*dwPitch)]=0x0000;
	lpData[1+(3*dwPitch)]=0x0000;
	lpData[2+(3*dwPitch)]=0x0000;
	lpData[3+(3*dwPitch)]=0xFFFF;

	// unlock texture
	UnlockTexture(g_dwDefMouseCursorTexIdx);
}

// SetMouseCursor()

void SetMouseCursor(DWORD dwTexIndex,
					int nXHotSpot,
					int nYHotSpot,
					float fXSize,
					float fYSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "SetMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// store values
	if(dwTexIndex==NO_OBJECT)
		g_dwMouseCursorTexIdx=g_dwDefMouseCursorTexIdx;
	else
		g_dwMouseCursorTexIdx=dwTexIndex;
	g_nMouseXHotSpot=nXHotSpot;
	g_nMouseYHotSpot=nYHotSpot;
	g_fMouseXSize=fXSize;
	g_fMouseYSize=fYSize;
}

// EnableMouseCursor()

void EnableMouseCursor(DWORD dwTexIndex,
					   int nXHotSpot,
					   int nYHotSpot,
					   float fXSize,
					   float fYSize)
{
#ifdef DEBUG_MODE
	// verify index
	if(dwTexIndex>=g_dwNumTex&&
	   dwTexIndex!=NO_OBJECT)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "EnableMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// set cursor
	SetMouseCursor(dwTexIndex,
				   nXHotSpot,
				   nYHotSpot,
				   fXSize,
				   fYSize);

	// set flag
	g_bMouseCursor=TRUE;
}

// DisableMouseCursor()

void DisableMouseCursor(void)
{
	// set flag
	g_bMouseCursor=FALSE;
}

// RenderMouseCursor()

void RenderMouseCursor(void)
{
	// check flags
	if(!g_bWinMouse)
		return;
	if(!g_bMouseCursor)
		return;

#ifdef DEBUG_MODE
	// verify index
	if(g_dwMouseCursorTexIdx>=g_dwNumTex)
	{
		DisplayFatalErrorMessage("Invalid texture index.",
								 "RenderMouseCursor()",
								 FALSE);
		return;
	}
#endif

	// begin sprites
	if(BeginSprites())
	{
		// output sprite
		OutputSprite(g_dwMouseCursorTexIdx,
					 ScrCoords(g_nMouseX-g_nMouseXHotSpot,
							   g_nMouseY-g_nMouseYHotSpot),
					 g_fMouseXSize,
					 g_fMouseYSize);
		
		// end sprites
		EndSprites();	
	}
}

// InitShatter()

BOOL InitShatter(void)
{
	// allocate memory
	if(!AllocMem((LPVOID*)&g_lpfShatterVtxTU,
				 sizeof(float)*17))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterVtxTV,
				 sizeof(float)*17))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterCenTU,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterCenTV,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpvShatterVelocity,
				 sizeof(D3DXVECTOR3)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx1,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx2,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpdwShatterVtx3,
				 sizeof(DWORD)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaYaw,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaPitch,
				 sizeof(float)*20))
		return(FALSE);
	if(!AllocMem((LPVOID*)&g_lpfShatterDeltaRoll,
				 sizeof(float)*20))
		return(FALSE);

	// macro for setting vertex indices
	#define SET_SHATTER_INDICES(v,v1,v2,v3) { g_lpdwShatterVtx1[v]=v1; g_lpdwShatterVtx2[v]=v2; g_lpdwShatterVtx3[v]=v3; }
	
	// init vertex indices
	SET_SHATTER_INDICES(0, 0, 1, 5);
	SET_SHATTER_INDICES(1, 5, 1, 2);
	SET_SHATTER_INDICES(2, 5, 2, 6);
	SET_SHATTER_INDICES(3, 2, 3, 6);
	SET_SHATTER_INDICES(4, 6, 3, 4);
	SET_SHATTER_INDICES(5, 7, 5, 0);
	SET_SHATTER_INDICES(6, 6, 9, 4);
	SET_SHATTER_INDICES(7, 7, 5, 10);
	SET_SHATTER_INDICES(8, 6, 9, 11);
	SET_SHATTER_INDICES(9, 5, 8, 6);
	SET_SHATTER_INDICES(10,6, 8, 11);
	SET_SHATTER_INDICES(11,11,8, 10);
	SET_SHATTER_INDICES(12,10,5, 8);
	SET_SHATTER_INDICES(13,7, 12,10);
	SET_SHATTER_INDICES(14,11,9, 16);
	SET_SHATTER_INDICES(15,12,13,10);
	SET_SHATTER_INDICES(16,13,14,10);
	SET_SHATTER_INDICES(17,14,10,11);
	SET_SHATTER_INDICES(18,14,15,11);
	SET_SHATTER_INDICES(19,15,16,11);
	
	// ok
	return(TRUE);
}

// ReleaseShatter()

void ReleaseShatter(void)
{
	// free memory
	FreeMem((LPVOID*)&g_lpfShatterVtxTU);
	FreeMem((LPVOID*)&g_lpfShatterVtxTV);
	FreeMem((LPVOID*)&g_lpfShatterCenTU);
	FreeMem((LPVOID*)&g_lpfShatterCenTV);
	FreeMem((LPVOID*)&g_lpvShatterVelocity);
	FreeMem((LPVOID*)&g_lpdwShatterVtx1);
	FreeMem((LPVOID*)&g_lpdwShatterVtx2);
	FreeMem((LPVOID*)&g_lpdwShatterVtx3);
	FreeMem((LPVOID*)&g_lpfShatterDeltaYaw);
	FreeMem((LPVOID*)&g_lpfShatterDeltaPitch);
	FreeMem((LPVOID*)&g_lpfShatterDeltaRoll);
}

// RandomizeShatter()

void RandomizeShatter(void)
{
	// counter
	DWORD dwCount;
	
	// macro for setting vertices
	#define SET_SHATTER_VERTEX(vtx,tu,tv) { g_lpfShatterVtxTU[vtx]=tu; g_lpfShatterVtxTV[vtx]=tv; }
	
	// create vertex array
	SET_SHATTER_VERTEX(0, 0.000F                          ,0.000F);
	SET_SHATTER_VERTEX(1, 0.250F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(2, 0.500F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(3, 0.750F+(GetRandomFloat()*0.112F),0.000F);
	SET_SHATTER_VERTEX(4, 1.000F                          ,0.000F);
	SET_SHATTER_VERTEX(5, 0.250F+(GetRandomFloat()*0.150F),0.333F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(6, 0.750F+(GetRandomFloat()*0.150F),0.333F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(7, 0.000F                          ,0.500F+(GetRandomFloat()*0.450F));
	SET_SHATTER_VERTEX(8, 0.500F+(GetRandomFloat()*0.075F),0.500F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(9, 1.000F                          ,0.500F+(GetRandomFloat()*0.450F));
	SET_SHATTER_VERTEX(10,0.250F+(GetRandomFloat()*0.150F),0.666F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(11,0.750F+(GetRandomFloat()*0.150F),0.666F+(GetRandomFloat()*0.150F));
	SET_SHATTER_VERTEX(12,0.000F                          ,1.000F);
	SET_SHATTER_VERTEX(13,0.250F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(14,0.500F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(15,0.750F+(GetRandomFloat()*0.112F),1.000F);
	SET_SHATTER_VERTEX(16,1.000F                          ,1.000F);

	// process each triangle
	for(dwCount=0;dwCount<20;dwCount++)
	{
		// compute centroid
		g_lpfShatterCenTU[dwCount]=(g_lpfShatterVtxTU[g_lpdwShatterVtx1[dwCount]]+
									g_lpfShatterVtxTU[g_lpdwShatterVtx2[dwCount]]+
									g_lpfShatterVtxTU[g_lpdwShatterVtx3[dwCount]])/3;
		g_lpfShatterCenTV[dwCount]=(g_lpfShatterVtxTV[g_lpdwShatterVtx1[dwCount]]+
									g_lpfShatterVtxTV[g_lpdwShatterVtx2[dwCount]]+
									g_lpfShatterVtxTV[g_lpdwShatterVtx3[dwCount]])/3;

		// create random velocity
		g_lpvShatterVelocity[dwCount]=D3DXVECTOR3(GetRandomFloat()*128,
												  -(640+(GetRandomFloat()*48)),
												  GetRandomFloat()*128);

		// create random orientation values
		g_lpfShatterDeltaYaw[dwCount]=GetRandomFloat()*PI*1.5F;
		g_lpfShatterDeltaPitch[dwCount]=GetRandomFloat()*PI*1.5F;
		g_lpfShatterDeltaRoll[dwCount]=GetRandomFloat()*PI*1.5F;
	}
}

// RenderShatter()

void RenderShatter(float fPos)
{
	// counters
	DWORD dwCount1;
	DWORD dwCount2;

	// save position
	float fOrigPos=fPos;
	
	// update vertex buffer
	LPTRANSVERTEX lpVtx=(LPTRANSVERTEX)LockVertexBuffer(g_dwTransVtxBuf);
	for(dwCount1=0;dwCount1<20;dwCount1++)
	{
		// adjust position for this piece
		float fPos=(fOrigPos*2)-((float)(dwCount1)/20);

		// check for overflow
		if(fPos<0)
			fPos=0;
		
		// adjust position to follow curve
		fPos=1-sqrtf(1-Squared(fPos));
		
		// get centroid
		float fCenTU=g_lpfShatterCenTU[dwCount1];
		float fCenTV=g_lpfShatterCenTV[dwCount1];
		
		// create rotation matrix
		D3DXMATRIX mTri;
		D3DXMatrixRotationYawPitchRoll(&mTri,
									   g_lpfShatterDeltaYaw[dwCount1]*fPos,
									   g_lpfShatterDeltaPitch[dwCount1]*fPos,
									   g_lpfShatterDeltaRoll[dwCount1]*fPos);

		// insert position into matrix
		mTri._41=((fCenTU-0.5F)*640)+(g_lpvShatterVelocity[dwCount1].x*fPos);
		mTri._42=((fCenTV-0.5F)*480)+(g_lpvShatterVelocity[dwCount1].y*fPos);
		mTri._43=579.5F             +(g_lpvShatterVelocity[dwCount1].z*fPos);
		
		// output each vertex
		for(dwCount2=0;dwCount2<3;dwCount2++)
		{
			// vertex coordinates
			float fTU;
			float fTV;
			
			// read vertex coordinates
			switch(dwCount2)
			{
			case(0):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx1[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx1[dwCount1]];
				break;
			case(1):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx2[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx2[dwCount1]];
				break;
			case(2):
				fTU=g_lpfShatterVtxTU[g_lpdwShatterVtx3[dwCount1]];
				fTV=g_lpfShatterVtxTV[g_lpdwShatterVtx3[dwCount1]];
				break;
			}

			// compute vector
			D3DXVECTOR3 vVec;
			vVec.x=(fTU-fCenTU)*640;
			vVec.y=(fTV-fCenTV)*480;
			vVec.z=0;

			// transform vector into vertex
			D3DXVECTOR3 vVtx;
			TransformVector(&vVtx,
							&vVec,
							&mTri);

			// output vertex
			SET_TRANSVERTEX(lpVtx[(dwCount1*3)+dwCount2],
							vVtx.x,
							vVtx.y,
							vVtx.z,
							fTU,
							1-fTV);
		}
	}
	UnlockVertexBuffer(g_dwTransVtxBuf);

	// begin sprites
	if(BeginSprites(TRUE))
	{
		// output texture 2
		OutputSprite(g_lpdwTransData[TD_SHATTER_TEX2],
					 ScrCoords(320,240),
					 ScrSize(640,480));

		// end sprites
		EndSprites();
	}
	
	// save projection matrix
	D3DXMATRIX mProj;
	CopyMemory(&mProj,
			   &g_mtxProj,
			   sizeof(D3DXMATRIX));

	// output texture 1
	SetWorldMatrix();
	SetViewMatrix();
	SetProjectionMatrixA(1,1000,45);
	SetVertexBuffer(g_dwTransVtxBuf);
	SetIndexBuffer(g_dwTransIdxBuf);
	SetMaterial(g_dwSpriteMat);
	SetTexture(g_lpdwTransData[TD_SHATTER_TEX1]);
	RenderPrimitive();

	// reset projection matrix
	SetProjectionMatrixA(&mProj);
}

// ClassifyPoint()

DWORD ClassifyPoint(LPD3DXVECTOR3 lpvPO,
					LPD3DXVECTOR3 lpvPN,
					LPD3DXVECTOR3 lpvP)
{
	// compute d-vector
	D3DXVECTOR3 vD=(*lpvPO)-(*lpvP);

	// compute d-value
	float fD=DotProduct(&vD,lpvPN);

	// check d-value
	if(fD<0)
		// in front
		return(PLANE_FRONT);
	else if(fD>0)
		// in back
		return(PLANE_BACK);
	else
		// on plane
		return(PLANE_ON);
}

// DistanceToRayPlaneIntersection()

float DistanceToRayPlaneIntersection(LPD3DXVECTOR3 lpvRO,
									 LPD3DXVECTOR3 lpvRN,
									 LPD3DXVECTOR3 lpvPO,
									 LPD3DXVECTOR3 lpvPN)
{
	// compute v-value
	float fV=DotProduct(lpvPN,
						lpvRN);
	
	// check v-value
	if(fV==0)
		// no intersection possible
		return(-1);

	// compute d-value
	float fD=-DotProduct(lpvPN,
						 lpvPO);

	// compute u-value
	float fU=DotProduct(lpvPN,
						lpvRO)+fD;
	
	// compute distance
	return(-(fU/fV));
}

// Get2DBarycentricCoordinates()

BOOL Get2DBarycentricCoordinates(float fX1,
								 float fY1,
								 float fX2,
								 float fY2,
								 float fX3,
								 float fY3,
								 float fXP,
								 float fYP,
								 float *lpfB1,
								 float *lpfB2,
								 float *lpfB3)
{
	// compute denominator
	float fB0=((fX2-fX1)*(fY3-fY1))-
			  ((fX3-fX1)*(fY2-fY1));

	// check for zero-area polygon
	if(fB0==0)
		// no good
		return(FALSE);
	else
	{
		// compute reciprocal
		fB0=1/fB0;

		// compute coordinates
		(*lpfB1)=(((fX2-fXP)*(fY3-fYP))-
			      ((fX3-fXP)*(fY2-fYP)))*fB0;
		(*lpfB2)=(((fX3-fXP)*(fY1-fYP))-
				  ((fX1-fXP)*(fY3-fYP)))*fB0;
		(*lpfB3)=1.0F-(*lpfB1)-(*lpfB2);
		
		// ok
		return(TRUE);
	}
}
									
// IsPointInTriangle()

BOOL IsPointInTriangle(LPD3DXVECTOR3 lpvP1,
					   LPD3DXVECTOR3 lpvP2,
					   LPD3DXVECTOR3 lpvP3,
					   LPD3DXVECTOR3 lpvP)
{
	// barycentric coordinates
	float fB1;
	float fB2;
	float fB3;

	// check projection on X-Y plane
	if(!Get2DBarycentricCoordinates(lpvP1->x,lpvP1->y,
									lpvP2->x,lpvP2->y,
									lpvP3->x,lpvP3->y,
									lpvP->x, lpvP->y,
									&fB1,&fB2,&fB3))
	{
		// check projection on X-Z plane
		if(!Get2DBarycentricCoordinates(lpvP1->x,lpvP1->z,
										lpvP2->x,lpvP2->z,
										lpvP3->x,lpvP3->z,
										lpvP->x, lpvP->z,
										&fB1,&fB2,&fB3))
		{
			// get projection on Y-Z plane
			if(!Get2DBarycentricCoordinates(lpvP1->y,lpvP1->z,
											lpvP2->y,lpvP2->z,
											lpvP3->y,lpvP3->z,
											lpvP->y, lpvP->z,
											&fB1,&fB2,&fB3))
			{
				DisplayErrorMessage("Undefined barycentric coordinates.",
									NULL,
									FALSE);
				return(FALSE);
			}
		}
	}

	// check coordinates
	if(fB1>=0&&
	   fB2>=0&&
	   fB3>=0)
		// ok
		return(TRUE);
	else
		// no good
		return(FALSE);
}
									 
// GetNearestPointOnLine()

void GetNearestPointOnLine(LPD3DXVECTOR3 lpvL1,
						   LPD3DXVECTOR3 lpvL2,
						   LPD3DXVECTOR3 lpvP,
						   LPD3DXVECTOR3 lpvNP)

{
	// compute vectors
	D3DXVECTOR3 vC=(*lpvP)-(*lpvL1);
	D3DXVECTOR3 vV=(*lpvL2)-(*lpvL1);

	// compute d-value
	float fD=Magnitude(vV);

	// normalize v-vector
	vV/=fD;
	
	// compute t-value
	float fT=DotProduct(&vV,
						&vC);
	
	// check t-value
	if(fT<=0)
		// use point #1
		(*lpvNP)=(*lpvL1);
	else if(fT>=fD)
		// use point #2
		(*lpvNP)=(*lpvL2);
	else
		// use point between 1 and 2
		(*lpvNP)=(*lpvL1)+(vV*fT);
}

// GetNearestPointOnTriangle()

void GetNearestPointOnTriangle(LPD3DXVECTOR3 lpvP1,
							   LPD3DXVECTOR3 lpvP2,
							   LPD3DXVECTOR3 lpvP3,
							   LPD3DXVECTOR3 lpvP,
							   LPD3DXVECTOR3 lpvNP)
{
	// nearest points on lines
	D3DXVECTOR3 vV1;
	D3DXVECTOR3 vV2;
	D3DXVECTOR3 vV3;
	
	// get closest points on lines
	GetNearestPointOnLine(lpvP1,lpvP2,lpvP,&vV1);
	GetNearestPointOnLine(lpvP2,lpvP3,lpvP,&vV2);
	GetNearestPointOnLine(lpvP3,lpvP1,lpvP,&vV3);

	// compute vectors to points
	D3DXVECTOR3 vPV1=(*lpvP)-vV1;
	D3DXVECTOR3 vPV2=(*lpvP)-vV2;
	D3DXVECTOR3 vPV3=(*lpvP)-vV3;

	// compute square distances
	float fD1=SquareMagnitude(&vPV1);
	float fD2=SquareMagnitude(&vPV2);
	float fD3=SquareMagnitude(&vPV3);

	// find lowest distance
	if(fD1<fD2)
	{
		if(fD1<fD3)
			// use point #1
			(*lpvNP)=vV1;
		else
			// use point #3
			(*lpvNP)=vV3;
	}
	else
	{
		if(fD2<fD3)
			// use point #2
			(*lpvNP)=vV2;
		else
			// use point #3
			(*lpvNP)=vV3;
	}
}

// DistanceToRaySphereIntersection()

float DistanceToRaySphereIntersection(LPD3DXVECTOR3 lpvRO,
									  LPD3DXVECTOR3 lpvRN,
									  LPD3DXVECTOR3 lpvSO)
{
	// compute q-vector
	D3DXVECTOR3 vQ=(*lpvSO)-(*lpvRO);

	// compute equation values
	float fC=Magnitude(&vQ);
	float fV=DotProduct(&vQ,
						lpvRN);
	float fD=1-(Squared(fC)-Squared(fV));
	
	// check d-value
	if(fD<0)
		// no intersection
		return(-1);
	else
		// compute distance to nearest point
		return(fV-sqrtf(fD));
}

// IsPointInSphere()

BOOL IsPointInSphere(LPD3DXVECTOR3 lpvP,
					 LPD3DXVECTOR3 lpvSO)
{
	// get vector between points
	D3DXVECTOR3 vVec=(*lpvP)-(*lpvSO);
	
	// check square length
	if(SquareMagnitude(&vVec)<=1)
		return(TRUE);
	else
		return(FALSE);
}

// eof

