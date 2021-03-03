
// stkston.h -- Sticks & Stones header

// enumerations //

// game types
typedef enum tagGAMETYPE
{
	GT_NULL=0,
	GT_1PLAYER_A,
	GT_1PLAYER_B,
	GT_2PLAYER_A,
	GT_2PLAYER_B,
	GT_FORCE_DWORD=0xFFFFFFFF
} GAMETYPE,*LPGAMETYPE;

// play states
typedef enum tagPLAYSTATE
{
	ST_NULL=0,
	ST_PLAYGAME,
	ST_DESTROY,
	ST_COLLAPSE,
	ST_GAMEOVER,
	ST_FORCE_DWORD=0xFFFFFFFF
} PLAYSTATE,*LPPLAYSTATE;

// search path directions
typedef enum tagSEARCHPATH
{
	SP_NULL=0,
	SP_UP,
	SP_DOWN,
	SP_LEFT,
	SP_RIGHT,
	SP_FORCE_DWORD=0xFFFFFFFF
} SEARCHPATH,*LPSEARCHPATH;

// check path flags
typedef enum tagCHECKPATH
{
	CP_INVALID=0,
	CP_STOP,
	CP_CONTINUE,
	CP_FORCE_DWORD=0xFFFFFFFF
} CHECKPATH,*LPCHECKPATH;

// globals //

// playing field values
extern WORD g_wHSticks[32];
extern WORD g_wVSticks[32];
extern WORD g_wStones[32];
extern WORD g_wColHSticks[32];
extern WORD g_wColVSticks[32];
extern WORD g_wColStones[32];
// playing field extents
extern RECT g_rPlayingField[2];
// textures
extern DWORD g_dwTex[3];
// game type info
extern GAMETYPE g_gtType;
extern DWORD g_dwNumRows;
extern DWORD g_dwNumCols;
extern DWORD g_dwNumFields;
extern DWORD g_dwNumPlayers;
// standard gamepieces
extern WORD g_wStdPiece[64];
// current gamepieces
extern DWORD g_dwCurPiece[2];
extern DWORD g_dwCurXPos[2];
extern DWORD g_dwCurYPos[2];
// next gamepieces
extern DWORD g_dwNextPiece[2];
// current level
extern DWORD g_dwCurLevel[2];
// fall values
extern DWORD g_dwFallSkip[2];
extern DWORD g_dwFallVal[2];
extern BOOL g_bFastFall[2];
extern DWORD g_dwHoldFall[2];
// play state
extern PLAYSTATE g_psState[2];
// user input flags
extern BOOL g_bLeft[2];
extern BOOL g_bRight[2];
extern BOOL g_bUp[2];
extern BOOL g_bDown[2];
extern BOOL g_bBtn1[2];
extern BOOL g_bBtn2[2];
extern BOOL g_bEsc[2];
// previous user input flags
extern BOOL g_bPLeft[2];
extern BOOL g_bPRight[2];
extern BOOL g_bPUp[2];
extern BOOL g_bPDown[2];
extern BOOL g_bPBtn1[2];
extern BOOL g_bPBtn2[2];
extern BOOL g_bPEsc[2];
// user input hit flags
extern BOOL g_bLeftHit[2];
extern BOOL g_bRightHit[2];
extern BOOL g_bUpHit[2];
extern BOOL g_bDownHit[2];
extern BOOL g_bBtn1Hit[2];
extern BOOL g_bBtn2Hit[2];
extern BOOL g_bEscHit[2];
// loop detection fields
extern WORD g_wValid[16];
extern WORD g_wChecked[16];
extern WORD g_wCollapse[16];
// show next flags
extern BOOL g_bShowNext[2];

// constants //

// piece sizes
#define NOM_STONE_SIZE 20
#define NOM_STICK_SIZE 6

// masks
#define MASK_STONES 0xF000 //((WORD)BinaryToDWORD("1111000000000000"))
#define MASK_STICKS 0x0FFF //((WORD)BinaryToDWORD("0000111111111111"))
#define MASK_LEFT   0x5255 //((WORD)BinaryToDWORD("0101001001010101"))
#define MASK_RIGHT  0xA92A //((WORD)BinaryToDWORD("1010100100101010"))
#define MASK_TOP    0x31C3 //((WORD)BinaryToDWORD("0011000111000011"))
#define MASK_BOTTOM 0xCE30 //((WORD)BinaryToDWORD("1100111000110000"))

// movement flag
#define TEMPORARILY (TRUE+1)

// interference flags
#define INTF_HSTICKS ((DWORD)(1<<1))
#define INTF_VSTICKS ((DWORD)(1<<2))
#define INTF_STONES  ((DWORD)(1<<3))

// prototypes //

// object handling functions

extern BOOL InitSticksAndStones(void);

extern void ReleaseSticksAndStones(void);

extern BOOL LoadSticksAndStones(void);

extern BOOL InitStandardPieces(void);

extern void ComputePlayingFieldExtents(void);

extern void RenderSticksAndStones(void);

extern void RenderPlayingFields(void);

extern void RenderNextPieces(void);

extern BOOL AddInputSettings(void);

// game logic functions

extern void SetGameType(GAMETYPE gt);

extern void SetLevel(DWORD dwPlayer,
					 DWORD dwLevel);

extern void IncrementLevel(DWORD dwPlayer);

extern void ClearPlayingField(DWORD dwField);

extern void ResetPlayerPiece(DWORD dwPlayer);

extern DWORD GeneratePiece(void);

extern BOOL UpdateSticksAndStones(void);

extern void InsertPlayerPiece(DWORD dwPlayer);

extern void RemovePlayerPiece(DWORD dwPlayer);

extern BOOL CanPieceMoveLeft(DWORD dwPlayer);

extern BOOL CanPieceMoveRight(DWORD dwPlayer);

extern DWORD CanPieceMoveDown(DWORD dwPlayer);

extern BOOL CanPieceRotateLeft(DWORD dwPlayer);

extern BOOL CanPieceRotateRight(DWORD dwPlayer);

extern DWORD DoesPieceInterfere(DWORD dwField,
							    DWORD dwXPos,
							    DWORD dwYPos,
							    DWORD dwPiece);

extern void FillStoneEdges(DWORD dwField);

extern DWORD DetectClosedLoops(DWORD dwPlayer);

extern BOOL SearchLoopPath(DWORD dwField,
						   DWORD dwX,
						   DWORD dwY);

extern CHECKPATH CheckLoopPath(DWORD dwField,
							   DWORD dwX,
							   DWORD dwY,
							   SEARCHPATH spDir);

extern void MarkLoopPath(DWORD dwField,
						 DWORD dwX,
						 DWORD dwY);

extern void SetShowNextFlag(DWORD dwPlayer,
							BOOL bFlag);

extern void UpdateCollapseValues(DWORD dwField);

// macros //

// manipulate playing field values
#define GetHStick(f,x,y)   (g_wHSticks[(y)+((f)<<4)]&(1<<(x)))
#define GetVStick(f,x,y)   (g_wVSticks[(y)+((f)<<4)]&(1<<(x)))
#define GetStone(f,x,y)    (g_wStones [(y)+((f)<<4)]&(1<<(x)))
#define SetHStick(f,x,y)   { g_wHSticks[(y)+((f)<<4)]|=(1<<(x)); }
#define SetVStick(f,x,y)   { g_wVSticks[(y)+((f)<<4)]|=(1<<(x)); }
#define SetStone(f,x,y)    { g_wStones [(y)+((f)<<4)]|=(1<<(x)); }
#define ResetHStick(f,x,y) { g_wHSticks[(y)+((f)<<4)]&=~(1<<(x)); }
#define ResetVStick(f,x,y) { g_wVSticks[(y)+((f)<<4)]&=~(1<<(x)); }
#define ResetStone(f,x,y)  { g_wStones [(y)+((f)<<4)]&=~(1<<(x)); }

// manipulate valid field values
#define GetValStone(x,y)    (g_wValid[y]&(1<<(x)))
#define SetValStone(x,y)    { g_wValid[y]|=(1<<(x)); }
#define ResetValStone(x,y)  { g_wValid[y]&=~(1<<(x)); }

// manipulate checked field values
#define GetChkStone(x,y)    (g_wChecked[y]&(1<<(x)))
#define SetChkStone(x,y)    { g_wChecked[y]|=(1<<(x)); }
#define ResetChkStone(x,y)  { g_wChecked[y]&=~(1<<(x)); }

// manipulate collapse field values
#define GetColStone(x,y)    (g_wCollapse[y]&(1<<(x)))
#define SetColStone(x,y)    { g_wCollapse[y]|=(1<<(x)); }
#define ResetColStone(x,y)  { g_wCollapse[y]&=~(1<<(x)); }

// get standard piece values
#define StdHStick(pc,x,y) (g_wStdPiece[pc]&(1<<((x)+(((y)<<1)       ))))
#define StdVStick(pc,x,y) (g_wStdPiece[pc]&(1<<((x)+(((y)<<1)+(y)+6 ))))
#define StdStone(pc,x,y)  (g_wStdPiece[pc]&(1<<((x)+(((y)<<1)    +12))))

// stone block checking
#define HasLeftBlocks(pc)   (g_wStdPiece[pc]&MASK_LEFT  )
#define HasRightBlocks(pc)  (g_wStdPiece[pc]&MASK_RIGHT )
#define HasTopBlocks(pc)    (g_wStdPiece[pc]&MASK_TOP   )
#define HasBottomBlocks(pc) (g_wStdPiece[pc]&MASK_BOTTOM)

// piece rotation
#define RotateLeft(v)  { v=(v&(~3))|((v-1)&3); }
#define RotateRight(v) { v=(v&(~3))|((v+1)&3); }

// manipulate collapse field values
#define GetColHStick(f,x,y)   (g_wColHSticks[(y)+((f)<<4)]&(1<<(x)))
#define GetColVStick(f,x,y)   (g_wColVSticks[(y)+((f)<<4)]&(1<<(x)))
#define GetColStoneA(f,x,y)   (g_wColStones [(y)+((f)<<4)]&(1<<(x)))
#define SetColHStick(f,x,y)   { g_wColHSticks[(y)+((f)<<4)]|=(1<<(x)); }
#define SetColVStick(f,x,y)   { g_wColVSticks[(y)+((f)<<4)]|=(1<<(x)); }
#define SetColStoneA(f,x,y)   { g_wColStones [(y)+((f)<<4)]|=(1<<(x)); }
#define ResetColHStick(f,x,y) { g_wColHSticks[(y)+((f)<<4)]&=~(1<<(x)); }
#define ResetColVStick(f,x,y) { g_wColVSticks[(y)+((f)<<4)]&=~(1<<(x)); }
#define ResetColStoneA(f,x,y) { g_wColStones [(y)+((f)<<4)]&=~(1<<(x)); }

// eof //