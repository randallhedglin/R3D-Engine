
// r3ddxg.h -- DirectX Graphics wrapper header

// flag to allow hardware-only vertex processing
//#define ALLOW_HWONLYVP

/* flexible vertex format reference:

#define D3DFVF_CUSTOMVERTEX (flag|flag|...)

D3DFVF_XYZ:		 Position (float,float,float)
D3DFVF_XYZRHW:	 Position & RHW  (float,float,float,float)
D3DFVF_XYZBn:	 Blend Weights  (1-5) (float)
D3DFVF_NORMAL:	 Normal (float,float,float)
D3DFVF_PSIZE:	 Point Size  (float)
D3DFVF_DIFFUSE:	 Diffuse (DWORD)
D3DFVF_SPECULAR: Specular  (DWORD)
D3DFVF_TEXn:	 Texture Coordinates (0-8) (float,float)

D3DFVF_TEXCOORDSIZEx(n) Texture Coordinate Size
						x=Size, n=Coordinate Index

typedef struct tagCUSTOMVERTEX
{
	[type] [var]; // based on flags
	[type] [var];
	[type] [var];
	...
} CUSTOMVERTEX,*LPCUSTOMVERTEX;

#define SET_CUSTOMVERTEX(vtx,X,Y,Z,...) { vtx.x=X; vtx.Y; vtx.z=Z; ... }

*/

// structures //

typedef struct tagRENDERMODES
{
	// number of modes
	DWORD dwNumModes;
	// pointer to modes
	D3DDISPLAYMODE* lpModes;
} RENDERMODES,*LPRENDERMODES;

typedef struct tagRESOLUTION
{
	// DWORD resolution
	DWORD dwX,dwY;
	// float resolution
	float fX,fY;
} RESOLUTION,*LPRESOLUTION;

typedef struct tagRESOLUTIONS
{
	// number of resolutions
	DWORD dwNumRes;
	// pointer to resolutions
	LPRESOLUTION lpRes;
} RESOLUTIONS,*LPRESOLUTIONS;

typedef struct tagSCALEDMATRIX
{
	// matrix
	D3DXMATRIX mtx;
	// scale factors
	float fXScale;
	float fYScale;
	float fZScale;
	// optimization
	DWORD dwDummy[5];
} SCALEDMATRIX,*LPSCALEDMATRIX;

typedef struct tagMESHSUBSETINFO
{
	// material index
	DWORD dwMat;
	// texture index
	DWORD dwTex;
} MESHSUBSETINFO,*LPMESHSUBSETINFO;

typedef struct tagLOCKEDBONEWEIGHTS
{
	// number of vertices
	DWORD dwNumVtx;
	// vertices
	LPD3DXVECTOR3 lpVtx;
	// number of bones
	DWORD dwNumBones;
	// bone weights
	float** lplpWeights;
	// vertex size
	DWORD dwVtxSize;
	// vertex buffer pointer
	float* lpfVtxBuf;
	// vertex index array
	LPDWORD lpdwVtxIndex;
} LOCKEDBONEWEIGHTS,*LPLOCKEDBONEWEIGHTS;

typedef struct tagMESHFACE
{
	// vertex indices
	DWORD dwV1;
	DWORD dwV2;
	DWORD dwV3;
} MESHFACE,*LPMESHFACE;

typedef struct tagLOCKEDMESH
{
	// number of vertices
	DWORD dwNumVtx;
	// vertices
	LPD3DXVECTOR3 lpVtx;
	// number of faces
	DWORD dwNumFaces;
	// faces
	LPMESHFACE lpFaces;
	// vertex size
	DWORD dwVtxSize;
	// vertex buffer pointer
	float* lpfVtxBuf;
	// index size
	DWORD dwIdxSize;
	// index buffer pointer
	LPBYTE lpcIdxBuf;
} LOCKEDMESH,*LPLOCKEDMESH;

typedef struct tagTFKEYMATRIX
{
	// key type
	DWORD dwKeyType;
	// number of keys
	DWORD dwNumKeys;
	// matrix
	D3DXMATRIX mtx;
} TFKEYMATRIX,*LPTFKEYMATRIX;

typedef struct tagSKYBOXINFO
{
	// vertex buffer
	DWORD dwVtxBufIndex;
	// texture
	DWORD dwTexIndex;
} SKYBOXINFO,*LPSKYBOXINFO;

// maximum polygon vertices
#define MAX_POLYGON 15

typedef struct tagPOLYGON
{
	// number of vertices
	DWORD dwNumVtx;
	// pointer to vertices
	D3DXVECTOR3 lpvVtx[MAX_POLYGON];
} POLYGON,*LPPOLYGON;

// sprite vertex type
#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

typedef struct tagSPRITEVERTEX
{
	float x,y,z;
	float tu,tv;
} SPRITEVERTEX,*LPSPRITEVERTEX;

#define SET_SPRITEVERTEX(vtx,X,Y,Z,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.tu=TU; vtx.tv=TV; }

// fader vertex type
#define D3DFVF_FADERVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

typedef struct tagFADERVERTEX
{
	float x,y,z;
	float rhw;
	DWORD diffuse;
} FADERVERTEX,*LPFADERVERTEX;

#define SET_FADERVERTEX(vtx,X,Y,Z,RHW,DIFFUSE) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.rhw=RHW; vtx.diffuse=DIFFUSE; }

// line vertex type
#ifdef DEBUG_MODE
#define D3DFVF_LINEVERTEX (D3DFVF_XYZ)

typedef struct tagLINEVERTEX
{
	float x,y,z;
} LINEVERTEX,*LPLINEVERTEX;

#define SET_LINEVERTEX(vtx,X,Y,Z) { vtx.x=X; vtx.y=Y; vtx.z=Z; }
#endif

// skybox vertex type
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

typedef struct tagSKYBOXVERTEX
{
	// position
	float x,y,z;
	// texture coordinates
	float tu,tv;
} SKYBOXVERTEX,*LPSKYBOXVERTEX;

#define SET_SKYBOXVERTEX(vtx,X,Y,Z,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.tu=TU; vtx.tv=TV; }

// billboard vertex type
#define D3DFVF_BILLBOARDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

typedef struct tagBILLBOARDVERTEX
{
	// position
	float x,y,z;
	// normal
	float nx,ny,nz;
	// texture coordinates
	float tu,tv;
} BILLBOARDVERTEX,*LPBILLBOARDVERTEX;

#define SET_BILLBOARDVERTEX(vtx,X,Y,Z,NX,NY,NZ,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.nx=NX; vtx.ny=NY; vtx.nz=NZ; vtx.tu=TU; vtx.tv=TV; }

// transition vertex type
#define D3DFVF_TRANSVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

typedef struct tagTRANSVERTEX
{
	float x,y,z;
	float tu,tv;
} TRANSVERTEX,*LPTRANSVERTEX;

#define SET_TRANSVERTEX(vtx,X,Y,Z,TU,TV) { vtx.x=X; vtx.y=Y; vtx.z=Z; vtx.tu=TU; vtx.tv=TV; }

// viewport structure
typedef struct tagVIEWPORT
{
	// position
	float x,y;
	// size
	float w,h;
	// z-clip values
	float n,f;
} VIEWPORT,*LPVIEWPORT;

// constants //

// required aspect ratio
#define ASPECT_RATIO (640.0F/480.0F)

// vertex processing flags
#define VPF_HARDWARE 1
#define VPF_SOFTWARE 2
#define VPF_MIXED    3

// no object flag
#define NO_OBJECT 0xFFFFFFFF

// alpha mask
#define MASKA_1555 0x8000
#define MASKRGB_1555 0x7FFF

// animation key matrix value
#define ANIMKEY_MATRIX 4

// point classifications
#define PLANE_ON    1
#define PLANE_FRONT 2
#define PLANE_BACK  3

// max transition data elements
#define MAX_TRANSDATA 8

// transition render elements
#define MAX_TRANSVERTEX   64
#define MAX_TRANSTRIANGLE 196

// transition type flags
typedef enum tagTRANSTYPE
{
	TT_NULL=0,
	TT_FADEIN,  // tex,color,duration
	TT_FADEOUT, // tex,color,duration
	TT_FLASH,   // tex1,tex2,color,duration
	TT_TWEEN,   // tex1,tex2,duration
	TT_PEEL,    // tex1,tex2,direction,duration
	TT_SWING,   // tex1,tex2,direction,duration
	TT_SCROLL,  // tex1,tex2,direction,duration
	TT_SLIDE,   // tex1,tex2,direction,duration
	TT_SQUISH,  // tex1,tex2,direction,duration
	TT_SHATTER, // tex1,tex2,duration
	TT_RANDOM,  // tex1,tex2,duration
	TT_FORCE_DWORD=0xFFFFFFFF
} TRANSTYPE,*LPTRANSTYPE;

// transition data elements
#define TD_TWEEN_TEX1 0
#define TD_TWEEN_TEX2 1
#define TD_FADE_TEX   0
#define TD_FADE_COLOR 1
#define TD_FLASH_TEX1  0
#define TD_FLASH_TEX2  1
#define TD_FLASH_COLOR 2
#define TD_PEEL_TEX1 0
#define TD_PEEL_TEX2 1
#define TD_PEEL_DIR  2
#define TD_SWING_TEX1 0
#define TD_SWING_TEX2 1
#define TD_SWING_DIR  2
#define TD_SCROLL_TEX1 0
#define TD_SCROLL_TEX2 1
#define TD_SCROLL_DIR  2
#define TD_SLIDE_TEX1 0
#define TD_SLIDE_TEX2 1
#define TD_SLIDE_DIR  2
#define TD_SQUISH_TEX1 0
#define TD_SQUISH_TEX2 1
#define TD_SQUISH_DIR  2
#define TD_SHATTER_TEX1 0
#define TD_SHATTER_TEX2 1

// transition directions
#define TDR_LTOR   1
#define TDR_RTOL   2
#define TDR_TTOB   3
#define TDR_BTOT   4

// globals //

// direct3d object
extern IDirect3D8* g_lpD3D;
// direct3d device object
extern IDirect3DDevice8* g_lpD3DDevice;
// display adapter id info
extern D3DADAPTER_IDENTIFIER8 g_lpAdapterID;
// display device capabilities
extern D3DCAPS8 g_lpDevCaps;
// windowed rendering flag
extern BOOL g_bWindowedOK;
// device presentation parameters
extern D3DPRESENT_PARAMETERS g_pp;
// list of render modes
extern RENDERMODES g_rm;
// list of resolutions
extern RESOLUTIONS g_res;
// client area
extern RESOLUTION g_rClient;
// billboard vectors
extern D3DXVECTOR3 g_vBBRight;
extern D3DXVECTOR3 g_vBBUp;
extern D3DXVECTOR3 g_vBBAhead;
// vertex processing flag
extern DWORD g_dwVPF;
// vertex buffer counters
extern DWORD g_dwNumVtxBuf;
extern DWORD g_dwMaxVtxBuf;
// vertex buffer data
extern IDirect3DVertexBuffer8** g_lplpVtxBuf;
extern LPDWORD g_lpdwFVFSize;
extern LPDWORD g_lpdwNumVtx;
extern LPDWORD g_lpdwVtxType;
// index buffer counters
extern DWORD g_dwNumIdxBuf;
extern DWORD g_dwMaxIdxBuf;
// index buffer data
extern IDirect3DIndexBuffer8** g_lplpIdxBuf;
extern LPDWORD g_lpdwPType;
extern LPDWORD g_lpdwNumPrim;
// matrix counters
extern DWORD g_dwNumMtx;
extern DWORD g_dwMaxMtx;
// matrix list
extern LPSCALEDMATRIX g_lpMtx;
// current rendering data
extern DWORD g_dwCurNumVtx;
extern DWORD g_ptCurPType;
extern DWORD g_dwCurNumPrim;
// material counters
extern DWORD g_dwNumMat;
extern DWORD g_dwMaxMat;
// material data
extern D3DMATERIAL8* g_lpMat;
extern LPSTR* g_lplpMatNames;
// texture counters
extern DWORD g_dwNumTex;
extern DWORD g_dwMaxTex;
// texture data
extern IDirect3DTexture8** g_lplpTex;
extern LPSTR* g_lplpTexNames;
// light counters
extern DWORD g_dwNumLight;
extern DWORD g_dwMaxLight;
// light data
extern D3DLIGHT8* g_lpLight;
extern LPBOOL g_lpbLightEnabled;
#ifdef DEBUG_MODE
extern LPBOOL g_lpbLightValid;
#endif
extern LPSTR* g_lplpLightNames;
// font counters
extern DWORD g_dwNumFont;
extern DWORD g_dwMaxFont;
// font data
extern HFONT* g_lphFont;
// font device context
extern HDC g_hFontDC;
// sprite rendering values
extern DWORD g_dwSpriteVtxBuf;
extern DWORD g_dwSpriteIdxBuf;
extern DWORD g_dwSpriteMat;
extern BOOL g_bSpriteWasInScene;
extern BOOL g_bSpriteWasLinearFilter;
extern BOOL g_bSpriteWasInFog;
extern BOOL g_bSpriteWasZBuffer;
// sprite matrices
extern D3DXMATRIX g_mtxSpriteProj;
extern DWORD g_dwSpriteMtx;
// identity matrix
extern D3DXMATRIX g_mtxIdentity;
// environment rendering interface
extern ID3DXRenderToSurface* g_lpRTS;
// fading vertex & index buffer
extern DWORD g_dwFadeVtxBuf;
extern DWORD g_dwFadeIdxBuf;
// z-buffer enabled flag
extern BOOL g_bZBufferEnabled;
// mesh counters
extern DWORD g_dwNumMesh;
extern DWORD g_dwMaxMesh;
// mesh data
extern ID3DXMesh** g_lplpMesh;
extern LPDWORD g_lpdwNumMeshSubsets;
extern LPMESHSUBSETINFO* g_lplpMeshSubsetInfo;
extern LPSTR* g_lplpMeshNames;
// skin mesh counters
extern DWORD g_dwNumSkin;
extern DWORD g_dwMaxSkin;
// skin mesh data
extern ID3DXSkinMesh** g_lplpSkin;
extern LPDWORD g_lpdwRenderMeshIndex;
extern LPDWORD g_lpdwOriginalMeshIndex;
extern LPDWORD g_lpdwNumSkinBones;
extern LPD3DXMATRIX* g_lplpSkinBoneMtx;
// line vertex & index buffer
#ifdef DEBUG_MODE
extern DWORD g_dwLineVtxBuf;
extern DWORD g_dwLineIdxBuf;
#endif
// invalid render values flag
extern BOOL g_bInvalidRenderValues;
// bone counters
extern DWORD g_dwNumBone;
extern DWORD g_dwMaxBone;
// bone data
extern LPDWORD g_lpdwBoneMtxIdx;
extern LPDWORD g_lpdwNumBoneChildren;
extern LPDWORD* g_lplpdwBoneChildren;
extern LPDWORD g_lpdwBonePoseIdx;
extern LPD3DXMATRIX g_lpOrigBoneMtxInv;
extern LPSTR* g_lplpBoneName;
// pose counters
extern DWORD g_dwNumPose;
extern DWORD g_dwMaxPose;
// pose data
extern LPDWORD g_lpdwNumPoseBones;
extern LPD3DXMATRIX* g_lplpPoseMtx;
// directx file interface
extern IDirectXFile* g_lpDXFile;
// skybox data
extern DWORD g_dwSkyboxIdxBuf;
extern DWORD g_dwSkyboxMat;
extern DWORD g_dwSkyboxMtx;
extern DWORD g_dwSkyboxTex;
extern SKYBOXINFO g_sbLeft;
extern SKYBOXINFO g_sbRight;
extern SKYBOXINFO g_sbFront;
extern SKYBOXINFO g_sbBack;
extern SKYBOXINFO g_sbTop;
extern SKYBOXINFO g_sbBottom;
extern float g_fSkyboxRadius;
extern D3DXVECTOR3 g_vSkyboxOffset;
// software registry key
extern HKEY g_hSoftwareKey;
// billboard data
extern DWORD g_dwBBMtxIndex;
extern DWORD g_dwBBVtxBufIdx;
extern DWORD g_dwBBIdxBufIdx;
// buffer surfaces
extern IDirect3DSurface8* g_lpBackBuffer;
extern IDirect3DSurface8* g_lpZBuffer;
// environment settings
extern BOOL g_bInScene;
extern BOOL g_bLinearFilter;
extern BOOL g_bWireframe;
extern BOOL g_bGouraud;
extern WORD g_wLPRepeat;
extern WORD g_wLPPattern;
extern BOOL g_bZBufferWrites;
extern BOOL g_bBackfaceCull;
extern BOOL g_bABlend;
extern BOOL g_bATest;
extern BOOL g_bFog;
extern D3DCOLOR g_clrFog;
extern float g_fFogStart;
extern float g_fFogEnd;
extern BOOL g_bSpecular;
extern DWORD g_dwZBias;
extern D3DCOLOR g_clrAmbient;
extern D3DXMATRIX g_mtxProj;
extern D3DXMATRIX g_mtxView;
extern VIEWPORT g_vp;
// saved matrices
extern D3DXMATRIX g_mtxRTSProj;
extern D3DXMATRIX g_mtxSprProj;
extern D3DXMATRIX g_mtxSprView;
// current render sizes
extern float g_fCurRenderX;
extern float g_fCurRenderY;
// saved render sizes
extern float g_fRTSRenderX;
extern float g_fRTSRenderY;
// transition data
extern TRANSTYPE g_ttCurTrans;
extern DWORD g_dwTransCurPos;
extern DWORD g_dwTransEndPos;
extern DWORD g_lpdwTransData[];
extern DWORD g_dwTransVtxBuf;
extern DWORD g_dwTransIdxBuf;
extern BOOL g_bTransRev;
#ifdef DEBUG_MODE
// shaded sprites flag
extern BOOL g_bShadedSprites;
// line material
extern DWORD g_dwLineMat;
#endif
// alpha texture flag
extern BOOL g_bAlphaTextures;
// mouse cursor data
extern BOOL g_bMouseCursor;
extern DWORD g_dwMouseCursorTexIdx;
extern DWORD g_dwDefMouseCursorTexIdx;
extern int g_nMouseXHotSpot;
extern int g_nMouseYHotSpot;
extern float g_fMouseXSize;
extern float g_fMouseYSize;
// shatter data
extern float* g_lpfShatterVtxTU;
extern float* g_lpfShatterVtxTV;
extern float* g_lpfShatterCenTU;
extern float* g_lpfShatterCenTV;
extern LPD3DXVECTOR3 g_lpvShatterVelocity;
extern LPDWORD g_lpdwShatterVtx1;
extern LPDWORD g_lpdwShatterVtx2;
extern LPDWORD g_lpdwShatterVtx3;
extern float* g_lpfShatterDeltaYaw;
extern float* g_lpfShatterDeltaPitch;
extern float* g_lpfShatterDeltaRoll;

// prototypes //

// object handling functions

extern BOOL DXGraphicsInit(void);

extern void DXGraphicsRelease(void);

extern BOOL InitRenderModes(DWORD dwAdapterNum);

extern void ReleaseRenderModes(void);

extern BOOL InitResolutions(void);

extern void ReleaseResolutions(void);

extern BOOL IsModeRenderable(LPDISPLAYMODE lpdm);

extern BOOL SetValidRGBFormat(LPDISPLAYMODE lpdm);

extern void PurgeGraphicsObjects(void);

extern D3DLOCKED_RECT LockBackBuffer(void);

extern void UnlockBackBuffer(void);

extern void ResetGraphicsObjects(void);

extern void ResetRenderDevice(void);

extern BOOL InitDirect3D(void);

extern void ReleaseDirect3D(void);

extern BOOL GetDisplayAdapterInfo(void);

extern BOOL VerifyWindowedRendering(void);

extern BOOL SetPresentationParameters(void);

extern BOOL InitD3DDevice(void);

extern void ReleaseD3DDevice(void);

extern void UpdateClientArea(void);

extern BOOL SetAlphaMode(void);

extern BOOL SetFogMode(void);

extern BOOL SetTextureAddressingMode(void);

extern BOOL SetRenderStates(void);

extern BOOL SetVertexProcessingMode(void);

extern BOOL InitDXFile(void);

extern void ReleaseDXFile(void);

extern D3DFORMAT GetSystemDisplayFormat(void);

extern D3DLOCKED_RECT LockZBuffer(void);

extern void UnlockZBuffer(void);

// environment setting functions

extern void EnableZBuffer(void);

extern void DisableZBuffer(void);

extern void EnableWireframe(void);

extern void DisableWireframe(void);

extern void EnableGouraud(void);

extern void DisableGouraud(void);

extern void SetLinePattern(WORD wRepeatFactor=1,
						   WORD wLinePattern=0xFFFF);

extern void EnableZBufferWrites(void);

extern void DisableZBufferWrites(void);

extern void EnableBackfaceCull(void);

extern void EnableFrontfaceCull(void);

extern void DisableBackfaceCull(void);

extern void EnableAlphaBlend(void);

extern void DisableAlphaBlend(void);

extern void EnableAlphaTest(void);

extern void DisableAlphaTest(void);

extern void EnableFog(void);

extern void DisableFog(void);

extern void EnableSpecular(void);

extern void DisableSpecular(void);

extern void SetFogColor(D3DCOLOR clr);

extern void SetFogStart(float fVal);

extern void SetFogEnd(float fVal);

extern void SetZBias(DWORD dwVal);

extern void SetAmbientLight(D3DCOLOR clr);

extern void EnableSVP(void);

extern void DisableSVP(void);

extern void SetVertexShader(DWORD dwHandle);

extern void EnableLinearFilter(void);

extern void DisableLinearFilter(void);

extern void ResetRenderEnvironment(void);

extern void EnableAlphaTextures(void);

extern void DisableAlphaTextures(void);

// transformation functions

extern void SetViewMatrix(LPD3DXMATRIX lpMtx=NULL);

extern void SetViewMatrix(DWORD dwIndex);

extern void SetProjectionMatrix(LPD3DXMATRIX lpMtx=NULL);

extern void SetViewMatrixA(LPD3DXMATRIX lpMtx=NULL);

extern void SetProjectionMatrixA(LPD3DXMATRIX lpMtx=NULL);

extern void SetWorldMatrix(LPD3DXMATRIX lpMtx=NULL);

extern void SetBillboardVectors(LPD3DXVECTOR3 lpvAhead);

extern void SetCameraMatrix(DWORD dwIndex);

extern void SetProjectionMatrix(DWORD dwIndex);

extern void SetWorldMatrix(DWORD dwIndex);

extern void SetViewMatrix(float fPosX,
						  float fPosY,
						  float fPosZ,
						  float fTargetX,
						  float fTargetY,
						  float fTargetZ,
						  float fUpX=0,
						  float fUpY=1,
						  float fUpZ=0);

extern void SetProjectionMatrix(float fClipNear,
								float fClipFar,
								float fAngle=45,
								float fAspect=ASPECT_RATIO);

extern void SetProjectionMatrixA(float fClipNear,
								 float fClipFar,
								 float fAngle=45,
								 float fAspect=ASPECT_RATIO);

extern void SetDefaultProjectionMatrix(void);

extern void SetDefaultViewMatrix(void);

// misc math functions

extern float SquareMagnitude(D3DXVECTOR3 v);

extern float SquareMagnitude(LPD3DXVECTOR3 lpv);

extern float Magnitude(D3DXVECTOR3 v);

extern float Magnitude(LPD3DXVECTOR3 lpv);

extern D3DXVECTOR3 Normalize(D3DXVECTOR3 v1);

extern void Normalize(LPD3DXVECTOR3 lpv,
					  LPD3DXVECTOR3 lpv1);

extern float DotProduct(D3DXVECTOR3 v1,
						D3DXVECTOR3 v2);

extern float DotProduct(LPD3DXVECTOR3 lpv1,
					    LPD3DXVECTOR3 lpv2);

extern D3DXVECTOR3 CrossProduct(D3DXVECTOR3 v1,
							    D3DXVECTOR3 v2);

extern void CrossProduct(LPD3DXVECTOR3 lpv,
					     LPD3DXVECTOR3 lpv1,
					     LPD3DXVECTOR3 lpv2);

extern float SquareMagnitude(float x,
							 float y,
							 float z);

extern float Magnitude(float x,
					   float y,
					   float z);

extern void Normalize(float* lpx,
					  float* lpy,
					  float* lpz,
					  float x1,
					  float y1,
					  float z1);

extern float DotProduct(float x1,
					    float y1,
					    float z1,
						float x2,
					    float y2,
					    float z2);

extern void CrossProduct(float* lpx,
					     float* lpy,
					     float* lpz,
					     float x1,
					     float y1,
					     float z1,
					     float x2,
					     float y2,
					     float z2);

extern BOOL Get2DLineIntersection(float* lpfXI,
								  float* lpfYI,
								  float fX1,
								  float fY1,
								  float fX2,
								  float fY2,
								  float fX3,
								  float fY3,
								  float fX4,
								  float fY4);

extern void TransformVector(LPD3DXVECTOR3 lpv,
							LPD3DXVECTOR3 lpv1,
							LPD3DXMATRIX lpmtx);

extern BOOL GetPlaneNormal(LPD3DXVECTOR3 lpvN,
						   LPD3DXVECTOR3 lpv1,
						   LPD3DXVECTOR3 lpv2,
						   LPD3DXVECTOR3 lpv3);

extern BOOL IsVectorToLeft(LPD3DXVECTOR3 lpV,
						   LPD3DXVECTOR3 lpV1);

extern BOOL IsVectorToRight(LPD3DXVECTOR3 lpV,
						    LPD3DXVECTOR3 lpV1);

extern BOOL IsVectorInFront(LPD3DXVECTOR3 lpV,
						    LPD3DXVECTOR3 lpV1);

extern BOOL IsVectorBehind(LPD3DXVECTOR3 lpV,
						   LPD3DXVECTOR3 lpV1);

extern BOOL IsVectorAbove(LPD3DXVECTOR3 lpV,
						  LPD3DXVECTOR3 lpV1);

extern BOOL IsVectorBelow(LPD3DXVECTOR3 lpV,
						  LPD3DXVECTOR3 lpV1);

extern void ClipPolygonToPlane(LPPOLYGON lpPoly,
							   LPD3DXVECTOR3 lpv1,
							   LPD3DXVECTOR3 lpv2,
							   LPD3DXVECTOR3 lpv3);

extern BOOL DoesTriangleIntersectBox(LPD3DXVECTOR3 lpv1,
									 LPD3DXVECTOR3 lpv2,
									 LPD3DXVECTOR3 lpv3,
									 LPD3DXVECTOR3 lpvMin,
									 LPD3DXVECTOR3 lpvMax);

extern BOOL Get3DLinePlaneIntersection(LPD3DXVECTOR3 lpvI,
									   LPD3DXVECTOR3 lpvL1,
									   LPD3DXVECTOR3 lpvL2,
									   LPD3DXVECTOR3 lpvP1,
									   LPD3DXVECTOR3 lpvP2,
									   LPD3DXVECTOR3 lpvP3);

extern BOOL GetPolygonCentroid(LPD3DXVECTOR3 lpvC,
							   LPPOLYGON lpPoly);

extern BOOL IsPointInBox(LPD3DXVECTOR3 lpvP,
						 LPD3DXVECTOR3 lpvMin,
						 LPD3DXVECTOR3 lpvMax);

extern float RotFromAngle(float fAngle);

extern DWORD ClassifyPoint(LPD3DXVECTOR3 lpvPO,
						   LPD3DXVECTOR3 lpvPN,
						   LPD3DXVECTOR3 lpvP);

extern float DistanceToRayPlaneIntersection(LPD3DXVECTOR3 lpvRO,
											LPD3DXVECTOR3 lpvRN,
											LPD3DXVECTOR3 lpvPO,
											LPD3DXVECTOR3 lpvPN);

extern BOOL IsPointInTriangle(LPD3DXVECTOR3 lpvP1,
							  LPD3DXVECTOR3 lpvP2,
							  LPD3DXVECTOR3 lpvP3,
							  LPD3DXVECTOR3 lpvP);

extern BOOL Get2DBarycentricCoordinates(float fX1,
										float fY1,
									    float fX2,
										float fY2,
									    float fX3,
										float fY3,
									    float fXP,
										float fYP,
										float *lpfB1,
										float *lpfB2,
										float *lpfB3);

extern void GetNearestPointOnTriangle(LPD3DXVECTOR3 lpvP1,
									  LPD3DXVECTOR3 lpvP2,
									  LPD3DXVECTOR3 lpvP3,
									  LPD3DXVECTOR3 lpvP,
									  LPD3DXVECTOR3 lpvNP);

extern void GetNearestPointOnLine(LPD3DXVECTOR3 lpvL1,
								  LPD3DXVECTOR3 lpvL2,
								  LPD3DXVECTOR3 lpvP,
								  LPD3DXVECTOR3 lpvNP);

extern float DistanceToRaySphereIntersection(LPD3DXVECTOR3 lpvRO,
											 LPD3DXVECTOR3 lpvRN,
											 LPD3DXVECTOR3 lpvSO);

extern BOOL IsPointInSphere(LPD3DXVECTOR3 lpvP,
							LPD3DXVECTOR3 lpvSO);

// light functions

extern DWORD GetNextLight(void);

extern DWORD AddLight(D3DLIGHT8* lplt,
					  LPSTR lpName=NULL);

extern void UpdateLight(DWORD dwIndex,
						D3DLIGHT8* lplt=NULL);

extern void EnableLight(DWORD dwIndex);

extern void DisableLight(DWORD dwIndex);

extern void PurgeLights(void);

extern D3DLIGHT8 SetLight(D3DLIGHTTYPE Type,
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
						  float Phi);

extern DWORD AddLight(D3DLIGHTTYPE Type,
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
					  LPSTR lpName=NULL);

extern void UpdateLightType(DWORD dwIndex,
							D3DLIGHTTYPE Type);

extern void UpdateLightDiffuse(DWORD dwIndex,
							   float DiffuseR,
							   float DiffuseG,
							   float DiffuseB,
							   float DiffuseA);

extern void UpdateLightSpecular(DWORD dwIndex,
							    float SpecularR,
							    float SpecularG,
							    float SpecularB,
							    float SpecularA);

extern void UpdateLightAmbient(DWORD dwIndex,
							   float AmbientR,
							   float AmbientG,
							   float AmbientB,
							   float AmbientA);

extern void UpdateLightPosition(DWORD dwIndex,
							    float PositionX,
							    float PositionY,
							    float PositionZ);

extern void UpdateLightDirection(DWORD dwIndex,
							     float DirectionX,
							     float DirectionY,
							     float DirectionZ);

extern void UpdateLightRange(DWORD dwIndex,
							 float Range);

extern void UpdateLightFalloff(DWORD dwIndex,
							   float Falloff);

extern void UpdateLightAttenuation(DWORD dwIndex,
								   float Attenuation0,
								   float Attenuation1,
								   float Attenuation2);

extern void UpdateLightTheta(DWORD dwIndex,
							 float Theta);

extern void UpdateLightPhi(DWORD dwIndex,
						   float Phi);

extern DWORD FindLight(LPCSTR lpName);

#ifdef DEBUG_MODE
extern void ValidateLights(void);
#endif

// vertex buffer functions

extern DWORD GetNextVertexBuffer(void);

extern DWORD AddVertexBuffer(DWORD dwFVF,
							 DWORD dwFVFSize,
							 DWORD dwNumVtx,
							 BOOL bWriteOnly=TRUE);

extern LPVOID LockVertexBuffer(DWORD dwIndex);

extern void UnlockVertexBuffer(DWORD dwIndex);

extern void SetVertexBuffer(DWORD dwIndex);

extern void PurgeVertexBuffers(void);

// index buffer functions

extern DWORD GetNextIndexBuffer(void);

extern DWORD AddIndexBuffer(DWORD dwPType,
							DWORD dwNumPrim,
							BOOL bWriteOnly=TRUE);

extern LPWORD LockIndexBuffer(DWORD dwIndex);

extern void UnlockIndexBuffer(DWORD dwIndex);

extern void SetIndexBuffer(DWORD dwIndex);

extern void PurgeIndexBuffers(void);

// matrix functions

extern DWORD GetNextMatrix(void);

extern DWORD AddMatrix(LPD3DXMATRIX lpmat=NULL);

extern DWORD AddMatrix(float _11,
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
					   float _44);

extern void MatrixYaw(DWORD dwIndex,
					  float fVal);

extern void MatrixPitch(DWORD dwIndex,
					    float fVal);

extern void MatrixRoll(DWORD dwIndex,
					   float fVal);

extern void MatrixAhead(DWORD dwIndex,
					   float fVal);

extern void MatrixBack(DWORD dwIndex,
					   float fVal);

extern void MatrixLeft(DWORD dwIndex,
					   float fVal);

extern void MatrixRight(DWORD dwIndex,
					    float fVal);

extern void MatrixUp(DWORD dwIndex,
					 float fVal);

extern void MatrixDown(DWORD dwIndex,
					   float fVal);

extern void MatrixXScale(DWORD dwIndex,
					     float fVal);

extern void MatrixYScale(DWORD dwIndex,
					     float fVal);

extern void MatrixZScale(DWORD dwIndex,
					     float fVal);

extern void MatrixScale(DWORD dwIndex,
					    float fVal);

extern void PurgeMatrices(void);

extern void ResetMatrix(DWORD dwIndex);

extern void ResetMatrix(LPD3DXMATRIX lpMtx);

extern void GetMatrix(DWORD dwIndex,
					  LPD3DXMATRIX lpMtx);

extern void SetMatrix(DWORD dwIndex,
					  LPD3DXMATRIX lpMtx);

#ifdef DEBUG_MODE
extern void DisplayMatrixMessage(LPD3DXMATRIX lpMtx);
#endif

extern DWORD AddCameraMatrix(float fPosX,
						     float fPosY,
						     float fPosZ,
						     float fTargetX,
						     float fTargetY,
						     float fTargetZ,
						     float fUpX=0,
						     float fUpY=1,
						     float fUpZ=0);

extern void SetMatrixPosition(DWORD dwIndex,
							  LPD3DXVECTOR3 lpvPos);

extern void SetMatrixPosition(DWORD dwIndex,
							  float fX,
							  float fY,
							  float fZ);

// basic rendering functions

extern BOOL BeginScene(void);

extern void EndScene(void);

extern void PresentScene(void);

extern void RenderPrimitive(void);

extern void ClearScreen(D3DCOLOR clr=NULL,
						float fZ=1);

extern void ClearZBuffer(float fZ=1);

extern void CopyRectangle(IDirect3DSurface8* lpDest,
						  IDirect3DSurface8* lpSource,
						  int nX1Src,
						  int nY1Src,
						  int nX2Src,
						  int nY2Src,
						  int nXDest,
						  int nYDest);

extern void CopyFrontBuffer(IDirect3DSurface8* lpDest);

extern void CopyBackBuffer(IDirect3DSurface8** lplpDest);

extern void CopyZBuffer(IDirect3DSurface8** lplpDest);

// material functions

extern D3DMATERIAL8 SetMaterial(float DiffuseR,
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
								float Power);

extern DWORD AddMaterial(float DiffuseR,
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
						 LPSTR lpName=NULL);

extern DWORD GetNextMaterial(void);

extern DWORD AddMaterial(D3DMATERIAL8* lpmat,
						 LPSTR lpName=NULL);

extern void UpdateMaterial(DWORD dwIndex,
						   D3DMATERIAL8* lpmat);

extern void UpdateMaterialDiffuse(DWORD dwIndex,
								  float DiffuseR,
								  float DiffuseG,
								  float DiffuseB,
								  float DiffuseA);

extern void UpdateMaterialAmbient(DWORD dwIndex,
								  float AmbientR,
								  float AmbientG,
								  float AmbientB,
								  float AmbientA);

extern void UpdateMaterialSpecular(DWORD dwIndex,
								   float SpecularR,
								   float SpecularG,
								   float SpecularB,
								   float SpecularA);

extern void UpdateMaterialEmissive(DWORD dwIndex,
								   float EmissiveR,
								   float EmissiveG,
								   float EmissiveB,
								   float EmissiveA);

extern void UpdateMaterialPower(DWORD dwIndex,
								float Power);

extern void SetMaterial(DWORD dwIndex=NO_OBJECT);

extern void PurgeMaterials(void);

extern DWORD FindMaterial(LPCSTR lpName);

// viewport functions

extern void SetViewport(float fX=0,
						float fY=0,
						float fWidth=1,
						float fHeight=1,
						float fZNear=0,
						float fZFar=1);

extern void ResetViewport(void);

// texture functions

extern DWORD GetNextTexture(void);

extern DWORD AddTexture(LPCSTR lpFile,
						LPSTR lpName=NULL,
						DWORD dwMipMapLevels=1);

extern D3DLOCKED_RECT LockTexture(DWORD dwIndex);
					   
extern void UnlockTexture(DWORD dwIndex);

extern void SetTexture(DWORD dwIndex=NO_OBJECT);

extern void PurgeTextures(void);

extern DWORD AddTexture(DWORD dwWidth,
						DWORD dwHeight,
						BOOL bTransparent=FALSE,
						BOOL bTranslucent=FALSE,
						LPSTR lpName=NULL,
						DWORD dwMipMapLevels=1);

extern BOOL BeginRenderToTexture(DWORD dwIndex,
								 float fClipNear,
								 float fClipFar,
								 float fAngle=45);

extern void EndRenderToTexture(DWORD dwIndex);

extern BOOL AddAlphaToTexture(DWORD dwIndex,
							  float fAlpha=-1);

extern BOOL AddAlphaToTexture(IDirect3DTexture8* lpTex,
							  float fAlpha=-1);

extern void ClearTexture(DWORD dwIndex);

extern void ClearTexture(IDirect3DTexture8* lpTex);

extern DWORD FindTexture(LPCSTR lpName);

// surface file functions

#ifdef DEBUG_MODE
extern void CreateDDSFromImage(LPCSTR lpDestFile,
							   LPCSTR lpSrcFile,
							   BOOL bTransparent=TRUE,
							   BOOL bTranslucent=FALSE,
							   float fAlpha=-1);

extern void CreateDDSFromTexture(LPCSTR lpDestFile,
								 DWORD dwIndex);
#endif

// font functions

extern LOGFONT SetFont(LPSTR FaceName,
					   LONG Height,
					   LONG Width=0,
					   LONG Angle=0,
					   LONG Weight=FW_DONTCARE,
					   BYTE Italic=FALSE,
					   BYTE Underline=FALSE,
					   BYTE Strikeout=FALSE);

extern DWORD AddFont(LPSTR FaceName,
					 LONG Height,
					 LONG Width=0,
					 LONG Angle=0,
					 LONG Weight=FW_NORMAL,
					 BYTE Italic=FALSE,
					 BYTE Underline=FALSE,
					 BYTE Strikeout=FALSE);

extern DWORD GetNextFont(void);

extern DWORD AddFont(LOGFONT* lplf);

extern BOOL BeginText(DWORD dwIndex);

extern void OutputText(LPSTR lpText,
					   int nX,
					   int nY,
					   COLORREF crForeground=0x00FFFFFF,
					   COLORREF crBackground=0x00000000,
					   DWORD dwFormat=DT_LEFT|DT_TOP);

extern void EndText(void);

extern void PurgeFonts(void);

// sprite functions

extern void InitSprites(void);

extern BOOL BeginSprites(BOOL bLinear=FALSE);

extern void OutputSprite(DWORD dwTexIndex,
						 float fX,
						 float fY,
						 float fXSize,
						 float fYSize,
						 float fRot=0);

extern void EndSprites(void);

extern void ResetSpriteProjectionMatrix(void);

extern BOOL BeginShadedSprites(BOOL bLinear=FALSE);

extern void OutputShadedSprite(DWORD dwTexIndex,
							   D3DCOLOR clr,
							   float fX,
							   float fY,
							   float fXSize,
							   float fYSize,
							   float fRot=0);

extern void EndShadedSprites(void);

// fading functions

extern void InitFader(void);

extern void ResetFaderVertexBuffer(D3DCOLOR clr=0xFFFFFFFF);

extern void FadeScreen(D3DCOLOR clr);

// mesh functions

extern DWORD GetNextMesh(void);

extern DWORD AddMeshBox(float fWidth,
						float fHeight,
						float fDepth,
						DWORD dwMatIndex,
						LPSTR lpName=NULL,
						BOOL bOptimize=TRUE);

extern DWORD AddMeshCylinder(float fLowerRad,
	 						 float fUpperRad,
							 float fHeight,
	 						 DWORD dwNumSlices,
	 						 DWORD dwNumStacks,
							 DWORD dwMatIndex,
							 LPSTR lpName=NULL,
							 BOOL bOptimize=TRUE);

extern DWORD AddMeshPolygon(float fLengthOfSide,
							DWORD dwNumSides,
							DWORD dwMatIndex,
							LPSTR lpName=NULL,
							BOOL bOptimize=TRUE);

extern DWORD AddMeshSphere(float fRadius,
						   DWORD dwNumSlices,
						   DWORD dwNumStacks,
						   DWORD dwMatIndex,
						   LPSTR lpName=NULL,
						   BOOL bOptimize=TRUE);

extern DWORD AddMeshTeapot(DWORD dwMatIndex,
						   LPSTR lpName=NULL,
						   BOOL bOptimize=TRUE);

extern DWORD AddMeshText(DWORD dwFontIndex,
						 LPSTR lpText,
						 float fMaxDeviation,
						 float fExtrusion,
						 DWORD dwMatIndex,
						 LPSTR lpName=NULL,
						 BOOL bOptimize=TRUE);

extern DWORD AddMeshTorus(float fInnerRad,
						  float fOuterRad,
						  DWORD dwNumSides,
						  DWORD dwNumRings,
						  DWORD dwMatIndex,
						  LPSTR lpName=NULL,
						  BOOL bOptimize=TRUE);

extern DWORD AddMesh(LPCSTR lpFile,
					 LPSTR lpName=NULL,
					 BOOL bOptimize=TRUE);

extern void OptimizeMesh(DWORD dwIndex);

extern void SetMeshSubsetInfo(DWORD dwIndex,
							  DWORD dwSubset,
							  DWORD dwMatIndex,
							  DWORD dwTexIndex=NO_OBJECT);

extern void RenderMesh(DWORD dwMeshIndex,
					   DWORD dwWMtxIndex);

extern void PurgeMeshes(void);

extern DWORD FindMesh(LPCSTR lpName);

extern void AddMeshNormals(DWORD dwIndex);

extern void OffsetMesh(DWORD dwIndex,
					   float fOffsetX,
					   float fOffsetY,
					   float fOffsetZ);

extern LPLOCKEDMESH LockMesh(DWORD dwIndex);

extern void UnlockMesh(DWORD dwIndex,
					   LPLOCKEDMESH lpm);

extern void InvertMesh(DWORD dwIndex);

// skinned mesh functions

extern DWORD GetNextSkinMesh(void);

extern DWORD AddSkinMesh(LPCSTR lpFile,
						 LPDWORD lpdwBoneIndex,
						 LPDWORD lpdwPoseIndex,
						 LPDWORD lpdwNumPoses);

extern DWORD AddSkinMesh(DWORD dwMeshIndex,
						 DWORD dwNumBones,
						 BOOL bDefaultBones=FALSE);

extern void SetSkinMeshSubsetInfo(DWORD dwIndex,
								  DWORD dwSubset,
								  DWORD dwMatIndex,
								  DWORD dwTexIndex=NO_OBJECT);

extern void SetSkinMeshBoneMtx(DWORD dwSkinIndex,
							   DWORD dwBoneNum,
							   DWORD dwMtxIndex);

extern void SetSkinMeshBoneMtx(DWORD dwSkinIndex,
							   DWORD dwBoneNum,
							   LPD3DXMATRIX lpMtx);

extern void RenderSkinMesh(DWORD dwSkinIndex,
						   DWORD dwWMtxIndex);

extern void RenderSkinMesh(DWORD dwSkinIndex,
						   DWORD dwWMtxIndex,
						   DWORD dwBaseBoneIndex);

extern void PurgeSkinMeshes(void);

extern LPLOCKEDBONEWEIGHTS LockBoneWeightsBuffer(DWORD dwIndex);

extern void UnlockBoneWeightsBuffer(DWORD dwIndex,
								    LPLOCKEDBONEWEIGHTS lpbw,
									BOOL bFinalMesh=TRUE);

extern BOOL FinalizeSkinMesh(DWORD dwIndex);

extern void LoadSkinMeshData(IDirectXFileData* lpDXFileData,
							 LPDWORD lpdwSkinIndex,
							 LPDWORD lpdwBoneIndex,
							 DWORD dwParentBoneIndex,
							 LPDWORD lpdwPoseIndex,
							 LPDWORD lpdwNumPoses);

// line rendering functions

#ifdef DEBUG_MODE
extern void InitLineRender(void);

extern void RenderLine(float fX1,
					   float fY1,
					   float fZ1,
					   float fX2,
					   float fY2,
					   float fZ2,
					   D3DCOLOR clr=0xFFFFFFFF);

extern void RenderLine(LPD3DXVECTOR3 lpV1,
					   LPD3DXVECTOR3 lpV2,
					   D3DCOLOR clr=0xFFFFFFFF);

extern void RenderTransformedLine(float fX1,
								  float fY1,
								  float fZ1,
								  float fX2,
								  float fY2,
								  float fZ2,
								  D3DCOLOR clr=0xFFFFFFFF);

extern void RenderTransformedLine(LPD3DXVECTOR3 lpV1,
								  LPD3DXVECTOR3 lpV2,
								  D3DCOLOR clr=0xFFFFFFFF);

#endif

// bone structure functions

extern DWORD AddBone(float fPosX,
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
					 LPSTR lpName=NULL,
					 DWORD dwBonePoseIdx=NO_OBJECT,
					 DWORD dwNumChildren=0);

extern DWORD AddBone(float fPosX,
					 float fPosY,
					 float fPosZ,
					 float fTargetX,
					 float fTargetY,
					 float fTargetZ,
 					 float fUpX,
 					 float fUpY,
 					 float fUpZ,
					 LPSTR lpName=NULL,
					 DWORD dwBonePoseIdx=NO_OBJECT,
					 DWORD dwNumChildren=0);

extern DWORD AddBone(LPD3DXMATRIX lpMtx,
					 LPSTR lpName=NULL,
					 DWORD dwBonePoseIdx=NO_OBJECT,
					 DWORD dwNumChildren=0);

extern DWORD GetNextBone(void);

extern DWORD AddBone(DWORD dwMtxIndex,
					 LPSTR lpName=NULL,
					 DWORD dwBonePoseIdx=NO_OBJECT,
					 DWORD dwNumChildren=0);

extern void SetBoneChild(DWORD dwParentIndex,
						 DWORD dwChildNum,
						 DWORD dwChildIndex);

extern void AddBoneChildA(DWORD dwParentIndex,
						  DWORD dwChildIndex);

extern DWORD AddBoneChild(DWORD dwParentIndex,
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
						  LPSTR lpName=NULL,
						  DWORD dwBonePoseIdx=NO_OBJECT,
						  DWORD dwNumChildren=0);

extern DWORD AddBoneChild(DWORD dwParentIndex,
						  float fPosX,
						  float fPosY,
						  float fPosZ,
						  float fTargetX,
						  float fTargetY,
						  float fTargetZ,
 						  float fUpX,
 						  float fUpY,
 						  float fUpZ,
						  LPSTR lpName=NULL,
						  DWORD dwBonePoseIdx=NO_OBJECT,
						  DWORD dwNumChildren=0);

extern DWORD AddBoneChild(DWORD dwParentIndex,
						  LPD3DXMATRIX lpMtx,
						  LPSTR lpName=NULL,
						  DWORD dwBonePoseIdx=NO_OBJECT,
						  DWORD dwNumChildren=0);

extern DWORD AddBoneChild(DWORD dwParentIndex,
						  DWORD dwMtxIndex,
						  LPSTR lpName=NULL,
						  DWORD dwBonePoseIdx=NO_OBJECT,
						  DWORD dwNumChildren=0);

extern void PurgeBones(void);

extern void BoneYaw(DWORD dwIndex,
					float fVal);

extern void BonePitch(DWORD dwIndex,
					  float fVal);

extern void BoneRoll(DWORD dwIndex,
					 float fVal);

extern void BoneLeft(DWORD dwIndex,
					 float fVal);

extern void BoneRight(DWORD dwIndex,
					  float fVal);

extern void BoneUp(DWORD dwIndex,
				   float fVal);

extern void BoneDown(DWORD dwIndex,
					 float fVal);

extern void BoneAhead(DWORD dwIndex,
					  float fVal);

extern void BoneBack(DWORD dwIndex,
					 float fVal);

extern void BoneXScale(DWORD dwIndex,
					   float fVal);

extern void BoneYScale(DWORD dwIndex,
					   float fVal);

extern void BoneZScale(DWORD dwIndex,
					   float fVal);

extern void BoneScale(DWORD dwIndex,
					  float fVal);

extern void FinalizeBoneStructure(DWORD dwIndex);

extern void FinalizeBoneChild(DWORD dwIndex,
							  LPD3DXMATRIX lpMtx);

#ifdef DEBUG_MODE
extern void RenderBoneStructure(DWORD dwBoneIndex,
								DWORD dwWMtxIndex,
								float fBoneLength,
								float fBlipSize,
								BOOL bUseZBuffer=FALSE,
								D3DCOLOR clr=0xFFFFFFFF);

extern void RenderBoneChild(DWORD dwIndex,
							DWORD dwWMtxIndex,
							LPD3DXMATRIX lpMtx,
							float fBoneLength,
							float fBlipSize,
							D3DCOLOR clr);

extern void RenderBoneStructure(DWORD dwBoneIndex,
								DWORD dwFontIndex);

extern void RenderBoneChild(DWORD dwIndex,
							DWORD dwLevel);

extern void RenderBoneChildInfo(LPSTR lpName=NULL,
								DWORD dwLevel=NO_OBJECT);
#endif

extern void TranslateBoneStructure(DWORD dwBoneIndex,
								   DWORD dwSkinIndex);

extern void TranslateBoneStructure(DWORD dwBoneIndex,
								   LPD3DXMATRIX lpMtx);

extern void TranslateBoneChild(DWORD dwBoneIndex,
							   LPD3DXMATRIX lpMtxArray,
							   LPD3DXMATRIX lpMtx);

extern void SetBonePoseIndex(DWORD dwBoneIndex,
							 DWORD dwPoseIndex);

extern DWORD FindBone(LPCSTR lpName,
					  DWORD dwBoneIndex);

extern void FindBoneChild(LPCSTR lpName,
						  DWORD dwBoneIndex,
						  LPDWORD lpdwBoneIndex);

extern DWORD GetBoneCount(DWORD dwIndex);

extern void GetBoneChildCount(DWORD dwIndex,
							  LPDWORD lpdwNumBones);

#ifdef DEBUG_MODE
extern void ManipulateBoneStructure(DWORD dwBoneIndex,
									DWORD dwFontIndex);
#endif

extern void ResetBoneStructure(DWORD dwIndex);

extern void ResetBoneChild(DWORD dwIndex);

// pose functions

extern DWORD GetNextPose(void);

extern DWORD AddPose(DWORD dwSkinIndex);

extern DWORD AddPose(DWORD dwSkinIndex,
					 DWORD dwBoneIndex);

extern DWORD AddPose(DWORD dwPose1Index,
					 DWORD dwPose2Index,
					 float fTweenVal);

extern void SetPoseMatrix(DWORD dwPoseIndex,
						  DWORD dwMtxNum,
						  LPD3DXMATRIX lpMtx);

extern void SetPoseMatrix(DWORD dwPoseIndex,
						  DWORD dwMtxNum,
						  DWORD dwMtxIndex);

extern void RenderSkinMeshPose(DWORD dwSkinIndex,
							   DWORD dwPoseIndex,
							   DWORD dwWMtxIndex);

extern void RenderSkinMeshTween(DWORD dwSkinIndex,
							    DWORD dwPose1Index,
							    DWORD dwPose2Index,
								float fTweenVal,
							    DWORD dwWMtxIndex);

extern void PurgePoses(void);

extern void FinalizePoses(DWORD dwPoseIndex,
						  DWORD dwNumPoses,
						  DWORD dwBoneIndex);

extern void FinalizePose(DWORD dwPoseIndex,
						 DWORD dwBoneIndex);

extern void FinalizePoseChild(DWORD dwPoseIndex,
							  DWORD dwBoneIndex,
							  LPD3DXMATRIX lpMtx);

// skybox functions

extern void InitSkybox(void);

extern void SetSkybox(float fRadius,
					  float fOffsetX=0,
					  float fOffsetY=0,
					  float fOffsetZ=0,
					  DWORD dwLtTexIdx=NO_OBJECT,
					  DWORD dwRtTexIdx=NO_OBJECT,
					  DWORD dwFtTexIdx=NO_OBJECT,
					  DWORD dwBkTexIdx=NO_OBJECT,
					  DWORD dwTpTexIdx=NO_OBJECT,
					  DWORD dwBtTexIdx=NO_OBJECT);

extern void SetSkyboxLeft(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxRight(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxFront(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxBack(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxTop(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxBottom(DWORD dwTexIndex=NO_OBJECT);

extern void SetSkyboxRadius(float fRadius);

extern void RenderSkybox(LPD3DXVECTOR3 lpV);

extern void RenderSkybox(DWORD dwWMtxIndex);

extern void RenderSkybox(float fX,
						 float fY,
						 float fZ);

extern void SetSkyboxOffset(float fX,
							float fY,
							float fZ);

// billboard functions

extern void InitBillboards(void);

extern void RenderBillboard(DWORD dwMatIndex,
							DWORD dwTexIndex,
							LPD3DXVECTOR3 lpvPos,
							float fXSize,
							float fYSize,
							float fZSize);

extern void RenderBillboards(DWORD dwMatIndex,
							 DWORD dwTexIndex,
							 LPD3DXVECTOR3 lpvPos,
							 DWORD dwNumPos,
							 float fXSize,
							 float fYSize,
							 float fZSize);

// misc functions

extern void ScaleRectToClient(LPRECT lpR);

extern DWORD ComputeVertexSize(DWORD dwFVF);

#ifdef DEBUG_MODE
extern void DisplayRenderSettings(void);
#endif

// transition functions

extern void InitTransitions(void);

extern void SetTransition(TRANSTYPE tt,
						  DWORD dwData1=0,
						  DWORD dwData2=0,
						  DWORD dwData3=0,
						  DWORD dwData4=0);

extern void SetReverseTransition(TRANSTYPE tt,
								 DWORD dwData1=0,
								 DWORD dwData2=0,
								 DWORD dwData3=0,
								 DWORD dwData4=0);

extern BOOL RenderTransition(void);

extern BOOL InitShatter(void);

extern void ReleaseShatter(void);

extern void RandomizeShatter(void);

extern void RenderShatter(float fPos);

// mouse functions

extern void InitMouseCursor(void);

extern void SetMouseCursor(DWORD dwTexIndex=NO_OBJECT,
						   int nXHotSpot=-8,
						   int nYHotSpot=-8,
						   float fXSize=(16.0F/640.0F),
						   float fYSize=(16.0F/480.0F));

extern void EnableMouseCursor(DWORD dwTexIndex=NO_OBJECT,
							  int nXHotSpot=-8,
							  int nYHotSpot=-8,
							  float fXSize=(16.0F/640.0F),
							  float fYSize=(16.0F/480.0F));

extern void DisableMouseCursor(void);

extern void RenderMouseCursor(void);

// macros //

#define IsFormatRGB(a) ((a)==D3DFMT_X8R8G8B8||(a)==D3DFMT_R5G6B5||(a)==D3DFMT_X1R5G5B5||(a)==D3DFMT_X4R4G4B4)

#define GetR565(c) (((float)((((WORD)(c))&0xF800)>>11))/31)
#define GetG565(c) (((float)((((WORD)(c))&0x07E0)>>5 ))/63)
#define GetB565(c) (((float)((((WORD)(c))&0x001F)    ))/31)

#define SetR565(c,b) { c=c|(((WORD)(((float)(b))*31))<<11); }
#define SetG565(c,g) { c=c|(((WORD)(((float)(g))*63))<<5 ); }
#define SetB565(c,r) { c=c|(((WORD)(((float)(r))*31))    ); }

#define SetRGB565(c,r,g,b) { c=0; SetR565(c,r); SetG565(c,g); SetB565(c,b); }

#define GetR555(c) (((float)((((DWORD)(c))&0x7C00)>>10))/31)
#define GetG555(c) (((float)((((DWORD)(c))&0x03E0)>>5 ))/31)
#define GetB555(c) (((float)((((DWORD)(c))&0x001F)    ))/31)
#define GetA555(c) (((float)((((DWORD)(c))&0x8000)>>15))      )

#define SetR555(c,b) { c=c|(WORD)(((DWORD)(((float)(b))*31))<<10); }
#define SetG555(c,g) { c=c|(WORD)(((DWORD)(((float)(g))*31))<<5 ); }
#define SetB555(c,r) { c=c|(WORD)(((DWORD)(((float)(r))*31))    ); }
#define SetA555(c,a) { c=c|(WORD)(((DWORD)(((float)(a))      ))<<15); }

#define SetRGB555(c,r,g,b)    { c=0; SetR555(c,r); SetG555(c,g); SetB555(c,b); }
#define SetRGBA555(c,r,g,b,a) { c=0; SetR555(c,r); SetG555(c,g); SetB555(c,b); SetA555(c,a); }

#define GetR444(c) (((float)((((DWORD)(c))&0x0F00)>>8 ))/15)
#define GetG444(c) (((float)((((DWORD)(c))&0x00F0)>>4 ))/15)
#define GetB444(c) (((float)((((DWORD)(c))&0x000F)    ))/15)
#define GetA444(c) (((float)((((DWORD)(c))&0xF000)>>12))/15)

#define SetR444(c,b) { c=c|(WORD)(((DWORD)(((float)(b))*15))<<8 ); }
#define SetG444(c,g) { c=c|(WORD)(((DWORD)(((float)(g))*15))<<4 ); }
#define SetB444(c,r) { c=c|(WORD)(((DWORD)(((float)(r))*15))    ); }
#define SetA444(c,a) { c=c|(WORD)(((DWORD)(((float)(a))*15))<<12); }

#define SetRGB444(c,r,g,b)    { c=0; SetR444(c,r); SetG444(c,g); SetB444(c,b); }
#define SetRGBA444(c,r,g,b,a) { c=0; SetR444(c,r); SetG444(c,g); SetB444(c,b); SetA444(c,a); }

#define GetR888(c) (((float)((((DWORD)(c))&0x00FF0000)>>16))/255)
#define GetG888(c) (((float)((((DWORD)(c))&0x0000FF00)>>8 ))/255)
#define GetB888(c) (((float)((((DWORD)(c))&0x000000FF)    ))/255)
#define GetA888(c) (((float)((((DWORD)(c))&0xFF000000)>>24))/255)

#define SetR888(c,b) { c=c|(DWORD)(((DWORD)(((float)(b))*255))<<16); }
#define SetG888(c,g) { c=c|(DWORD)(((DWORD)(((float)(g))*255))<<8 ); }
#define SetB888(c,r) { c=c|(DWORD)(((DWORD)(((float)(r))*255))    ); }
#define SetA888(c,a) { c=c|(DWORD)(((DWORD)(((float)(a))*255))<<24); }

#define SetRGB888(c,r,g,b)    { c=0; SetR888(c,r); SetG888(c,g); SetB888(c,b); }
#define SetRGBA888(c,r,g,b,a) { c=0; SetR888(c,r); SetG888(c,g); SetB888(c,b); SetA888(c,a); }

// matrix macros
// variable "dwIndex" must contain index number
#define MtxRightX g_lpMtx[dwIndex].mtx._11
#define MtxRightY g_lpMtx[dwIndex].mtx._12
#define MtxRightZ g_lpMtx[dwIndex].mtx._13
#define MtxUpX	  g_lpMtx[dwIndex].mtx._21
#define MtxUpY    g_lpMtx[dwIndex].mtx._22
#define MtxUpZ    g_lpMtx[dwIndex].mtx._23
#define MtxAheadX g_lpMtx[dwIndex].mtx._31
#define MtxAheadY g_lpMtx[dwIndex].mtx._32
#define MtxAheadZ g_lpMtx[dwIndex].mtx._33
#define MtxPosX   g_lpMtx[dwIndex].mtx._41
#define MtxPosY   g_lpMtx[dwIndex].mtx._42
#define MtxPosZ   g_lpMtx[dwIndex].mtx._43

#define Mtx(a) ((DWORD)(a))

// screen-space scaling values
#define ScrCoordX(x)    ((((float)(x))-320)/640)
#define ScrCoordY(y)    ((240-((float)(y)))/480)
#define ScrCoords(x,y)  (ScrCoordX(x)),(ScrCoordY(y))
#define ScrSizeX(x)     (((float)(x))/640)
#define ScrSizeY(y)     (((float)(y))/480)
#define ScrSize(x,y)    (ScrSizeX(x)),(ScrSizeY(y))
#define ClientCoordX(x) ((long)(ScrSizeX(x)*g_rClient.fX))
#define ClientCoordY(y) ((long)(ScrSizeY(y)*g_rClient.fY))

// tween calculation
#define TweenVal(a,b,u) ((a)+((u)*((b)-(a))))

// eof

