
// r3ddxa.h -- DirectX Audio wrapper header

// globals //

// audio initialized flag
extern BOOL g_bAudioInit;
// performance interface
extern IDirectMusicPerformance8* g_lpPerformance;
// audio loader interface
extern IDirectMusicLoader8* g_lpAudioLoader;
// internal interfaces
extern IDirectMusic8* g_lpDM;
extern IDirectSound8* g_lpDS;
// default audiopath interface
extern IDirectMusicAudioPath8* g_lpDefAudioPath;
// default audiopath buffer
extern IDirectSoundBuffer8* g_lpDefAudioBuf;
// audio segment counters
extern DWORD g_dwNumAudioSeg;
extern DWORD g_dwMaxAudioSeg;
// audio segment data
extern LPBYTE* g_lplpAudioData;
extern IDirectMusicSegment8** g_lplpAudioSeg;
// audio master volume
extern float g_fAudioMasterVol;
// audio path counters
extern DWORD g_dwNumAudioPath;
extern DWORD g_dwMaxAudioPath;
// audio path data
extern IDirectMusicAudioPath8** g_lplpAudioPath;
extern IDirectSound3DBuffer8** g_lplpAudioBuf3D;
extern IDirectSoundBuffer8** g_lplpAudioBuf;
extern LPBOOL g_lpbAudioPath3DPrepared;
extern LPBOOL g_lpbAudioPath3DEnabled;
// audio listener
extern IDirectSound3DListener8* g_lpAudioListener;
// primary audio buffer
extern IDirectSoundBuffer* g_lpPrimaryBuf;
// audio master tempo
extern float g_fAudioMasterTempo;
// primary segment data
extern IDirectMusicSegment8* g_lpPrimarySeg;
extern MUSIC_TIME g_mtCurPriSegPos;
// audio distance factor
extern float g_fAudioDistFactor;
// 3d audio enabled flag
extern BOOL g_b3DAudioEnabled;

// constants //

// alignment flags
#define ALIGN_BEAT    (DMUS_SEGF_BEAT)
#define ALIGN_MEASURE (DMUS_SEGF_MEASURE)

// distance values
#define DIST_INCH 0.0254F // meters per inch

// prototypes //

// object handling functions

extern BOOL DXAudioInit(void);

extern void DXAudioRelease(void);

extern BOOL InitPerformance(void);

extern BOOL InitAudioLoader(LPCSTR lpPath=NULL);

extern void ReleasePerformance(void);

extern void ReleaseAudioLoader(void);

extern BOOL SetAudioSearchPath(LPCSTR lpPath=NULL);

extern void ClearAudioCache(void);

extern void PauseAudioOutput(void);

extern void ResumeAudioOutput(void);

extern BOOL InitDefAudioPath(void);

extern void ReleaseDefAudioPath(void);

extern void SetAudioMasterVolume(float fVol,
								 DWORD dwLength=0,
								 BOOL bPaths=TRUE);

extern void ResetAudioMasterVolume(void);

extern void SetAudioMasterTempo(float fTempo);

extern void ResetAudioMasterTempo(void);

extern void InvalidateAudioData(void);

extern void BeginAudioExclusiveMode(void);

extern void EndAudioExclusiveMode(void);

extern BOOL InitPrimaryBuffer(void);

extern void ReleasePrimaryBuffer(void);

extern void StartPrimaryBuffer(void);

extern void StopPrimaryBuffer(void);

extern void Enable3DAudio(void);

extern void Disable3DAudio(void);

extern void PurgeAudioObjects(void);

extern void SetAudioMasterVolumeA(float fVol,
								  DWORD dwLength=0,
								  BOOL bPaths=TRUE);

extern void SetAudioMasterTempoA(float fTempo);

extern void ActivateAllAudioPaths(void);

extern void CapturePrimaryAudioSegment(void);

extern void RestorePrimaryAudioSegment(void);

// audio segment functions

extern DWORD GetNextAudioSegment(void);

extern DWORD AddAudioSegment(LPCSTR lpFile);

extern void PlayAudioSegment(DWORD dwSegIndex,
							 DWORD dwPathIndex=NO_OBJECT,
							 DWORD dwAlign=NULL);

extern void PlayPrimaryAudioSegment(DWORD dwIndex,
									DWORD dwAlign=NULL);

extern void QueuePrimaryAudioSegment(DWORD dwIndex,
									 DWORD dwAlign=NULL);

extern void StopAudioSegment(DWORD dwIndex,
							 DWORD dwAlign=NULL);

extern void PurgeAudioSegments(void);

extern void StopAllAudioSegments(void);

extern BOOL IsAudioSegmentPlaying(DWORD dwIndex);

extern BOOL SetAudioSegmentLength(DWORD dwIndex,
								  MUSIC_TIME mtTime);

extern BOOL SetAudioSegmentRepeats(DWORD dwIndex,
								   DWORD dwRepeats=DMUS_SEG_REPEAT_INFINITE);

// audiopath functions

extern DWORD GetNextAudioPath(void);

extern DWORD AddAudioPath3D(float fMinDist,
						    float fMaxDist);

extern DWORD AddAudioPathStereo(void);

extern void PurgeAudioPaths(void);

extern void EnableAudioPath3D(DWORD dwIndex);

extern void DisableAudioPath3D(DWORD dwIndex);

extern void EnableAudioPath3DA(DWORD dwIndex);

extern void DisableAudioPath3DA(DWORD dwIndex);

extern void SetAudioPathMinDistance(DWORD dwIndex,
									float fDist);

extern void SetAudioPathMaxDistance(DWORD dwIndex,
									float fDist);

extern void SetAudioPathPosition(DWORD dwIndex,
								 LPD3DXVECTOR3 lpvPos);

extern void SetAudioPathPosition(DWORD dwIndex,
								 float fX,
								 float fY,
								 float fZ);

extern void SetAudioPathPosition(DWORD dwPathIndex,
								 DWORD dwMtxIndex);

extern void SetAudioPathOrientation(DWORD dwIndex,
									LPD3DXVECTOR3 lpvAhead);

extern void SetAudioPathOrientation(DWORD dwIndex,
								    float fX,
								    float fY,
								    float fZ);

extern void SetAudioPathOrientation(DWORD dwPathIndex,
								    DWORD dwMtxIndex);

extern void SetAudioPathProjectionCone(DWORD dwIndex,
									   DWORD dwInnerAng,
									   DWORD dwOuterAng,
									   float fOuterVol);

extern void SetAudioPathVelocity(DWORD dwIndex,
								 LPD3DXVECTOR3 lpvDir,
								 float fSpeed=1);

extern void SetAudioPathVelocity(DWORD dwIndex,
								 float fX,
								 float fY,
								 float fZ,
								 float fSpeed=1);

extern void SetAudioPathVelocity(DWORD dwPathIndex,
								 DWORD dwMtxIndex,
								 float fSpeed=1);

extern void SetAudioPathFXChorus(DWORD dwIndex=NO_OBJECT,
								 float fDepth=10,
								 float fFeedback=25,
								 float fFrequency=1.1F,
								 long lWaveform=DSFXCHORUS_WAVE_SIN,
								 float fDelay=16,
								 long lPhase=DSFXCHORUS_PHASE_90,
								 float fWetDryMix=50);

extern void SetAudioPathFXCompressor(DWORD dwIndex=NO_OBJECT,
									 float fGain=0,
									 float fAttack=10,
									 float fRelease=200,
									 float fThreshold=-20,
									 float fRatio=3,
									 float fPredelay=4);

extern void SetAudioPathFXDistortion(DWORD dwIndex=NO_OBJECT,
									 float fGain=-18,
									 float fEdge=15,
									 float fPostEQCenterFrequency=2400,
									 float fPostEQBandwidth=2400,
									 float fPreLowpassCutoff=8000);

extern void SetAudioPathFXEcho(DWORD dwIndex=NO_OBJECT,
							   float fFeedback=50,
							   float fLeftDelay=500,
							   float fRightDelay=500,
							   BOOL bSwapDelays=FALSE,
							   float fWetDryMix=50);

extern void SetAudioPathFXFlanger(DWORD dwIndex=NO_OBJECT,
								  float fDepth=100,
								  float fFeedback=-50,
								  float fFrequency=0.25F,
								  long lWaveform=DSFXFLANGER_WAVE_SIN,
								  float fDelay=2,
								  long lPhase=DSFXFLANGER_PHASE_ZERO,
								  float fWetDryMix=50);

extern void SetAudioPathFXGargle(DWORD dwIndex=NO_OBJECT,
								 DWORD dwRateHz=20,
								 DWORD dwWaveShape=DSFXGARGLE_WAVE_TRIANGLE);

extern void SetAudioPathFXReverb(DWORD dwIndex=NO_OBJECT,
								 DWORD dwType=DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT,
								 long lQuality=2);

extern void SetAudioPathFXNone(DWORD dwIndex=NO_OBJECT);

extern void SetAudioPathVolume(DWORD dwIndex=NO_OBJECT,
							   float fVol=1,
							   DWORD dwLength=0);

extern void ActivateAudioPath(DWORD dwIndex=NO_OBJECT);

extern void InactivateAudioPath(DWORD dwIndex=NO_OBJECT);

extern BOOL SetAudioPathFXInterface(DWORD dwIndex,
									REFGUID rguidObject,
									REFGUID rguidInterface,
									LPVOID* lplpObject);

// audio listener functions

extern BOOL InitAudioListener(void);

extern void ReleaseAudioListener(void);

extern void RenderAudioChanges(void);

extern void SetListenerDistanceFactor(float fDistFactor=DIST_INCH);

extern void SetListenerOrientation(LPD3DXVECTOR3 lpvAhead,
								   LPD3DXVECTOR3 lpvUp=NULL);

extern void SetListenerOrientation(float fAheadX,
								   float fAheadY,
								   float fAheadZ,
								   float fUpX=0,
								   float fUpY=1,
								   float fUpZ=0);

extern void SetListenerOrientation(DWORD dwMtxIndex);

extern void SetListenerPosition(LPD3DXVECTOR3 lpvPos);

extern void SetListenerPosition(float fX,
								float fY,
								float fZ);

extern void SetListenerPosition(DWORD dwMtxIndex);

extern void SetListenerVelocity(LPD3DXVECTOR3 lpvDir,
								float fSpeed=1);

extern void SetListenerVelocity(float fX,
								float fY,
								float fZ,
								float fSpeed=1);

extern void SetListenerVelocity(DWORD dwMtxIndex,
								float fSpeed=1);

// eof

