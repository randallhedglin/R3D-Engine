
// all blocks have sticks
// smash vertical sticks
// bombs, gems, big bombs (cause full collapse)
// collapse blocks each level

// stkston.cpp -- Sticks & Stones

// includes //

#include"..\\engine\\r3dwin.h"
#include"stkston.h"

// globals //

// playing field values
WORD g_wHSticks[32];
WORD g_wVSticks[32];
WORD g_wStones[32];
WORD g_wColHSticks[32];
WORD g_wColVSticks[32];
WORD g_wColStones[32];
// playing field extents
RECT g_rPlayingField[2];
// textures
DWORD g_dwTex[3];
// game type info
GAMETYPE g_gtType=GT_NULL;
DWORD g_dwNumRows=0;
DWORD g_dwNumCols=0;
DWORD g_dwNumFields=0;
DWORD g_dwNumPlayers=0;
// standard gamepieces
WORD g_wStdPiece[64];
// current gamepieces
DWORD g_dwCurPiece[2];
DWORD g_dwCurXPos[2];
DWORD g_dwCurYPos[2];
// next gamepiece
DWORD g_dwNextPiece[2];
// current level
DWORD g_dwCurLevel[2];
// fall values
DWORD g_dwFallSkip[2];
DWORD g_dwFallVal[2];
BOOL g_bFastFall[2];
DWORD g_dwHoldFall[2];
// play state
PLAYSTATE g_psState[2];
// user input data
DWORD g_dwXAxis[2];
DWORD g_dwYAxis[2];
DWORD g_dwButton1[2];
DWORD g_dwButton2[2];
DWORD g_dwCancel[2];
// loop detection fields
WORD g_wValid[16];
WORD g_wChecked[16];
WORD g_wCollapse[16];
// show next flags
BOOL g_bShowNext[2];

// functions //

// InitSticksAndStones()

BOOL InitSticksAndStones(void)
{
	// disable z-buffer
	DisableZBuffer();

	// disable culling
	DisableBackfaceCull();
	
	// enable alpha
	EnableAlphaBlend();
	EnableAlphaTest();

	// load data
	if(!LoadSticksAndStones())
		return(FALSE);

	// add user input
	if(!AddInputSettings())
		return(FALSE);

	// ok
	return(TRUE);
}

// ReleaseSticksAndStones()

void ReleaseSticksAndStones(void)
{
}

// ClearPlayingField()

void ClearPlayingField(DWORD dwField)
{
	// clear all fields
	ZeroMemory(&g_wHSticks  [dwField<<4],sizeof(WORD)*16);
	ZeroMemory(&g_wVSticks  [dwField<<4],sizeof(WORD)*16);
	ZeroMemory(&g_wStones   [dwField<<4],sizeof(WORD)*16);
	ZeroMemory(&g_wColStones[dwField<<4],sizeof(WORD)*16);
}

// RenderSticksAndStones()
	
void RenderSticksAndStones(void)
{
	// counter
	DWORD dwCount;

	// begin scene
	if(BeginScene())
	{
		// clear screen
		ClearScreen(D3DCOLOR_RGBA(0,0,128,255));

		// begin sprites
		if(BeginSprites())
		{
			// clear playing fields
			for(dwCount=0;dwCount<g_dwNumFields;dwCount++)
				OutputSprite(g_dwTex[2],
							 ScrCoords((g_rPlayingField[dwCount].left+g_rPlayingField[dwCount].right )>>1,
									   (g_rPlayingField[dwCount].top +g_rPlayingField[dwCount].bottom)>>1),
							 ScrSize((g_rPlayingField[dwCount].right -g_rPlayingField[dwCount].left)+3,
									 (g_rPlayingField[dwCount].bottom-g_rPlayingField[dwCount].top) +3));
			
			// render playing fields
			RenderPlayingFields();

			// render next pieces
			RenderNextPieces();
			
			// end sprites
			EndSprites();
		}
		// end scene
		EndScene();
	}

	// present scene
	PresentScene();
}

// RenderPlayingFields()

void RenderPlayingFields(void)
{
	// counter
	DWORD dwCount;
	
	// counters
	DWORD dwX;
	DWORD dwY;

	// insert player pieces
	for(dwCount=0;dwCount<g_dwNumPlayers;dwCount++)
		InsertPlayerPiece(dwCount);

	// render playing fields
	for(dwCount=0;dwCount<g_dwNumFields;dwCount++)
	{
		// render horizontal sticks
		for(dwY=0;dwY<g_dwNumRows+1;dwY++)
			for(dwX=0;dwX<g_dwNumCols;dwX++)
				if(GetHStick(dwCount,dwX,dwY))
					OutputSprite(g_dwTex[0],
								 ScrCoords(g_rPlayingField[dwCount].left+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   g_rPlayingField[dwCount].top +(NOM_STICK_SIZE/2)               +(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STONE_SIZE,NOM_STICK_SIZE));

		// render vertical sticks
		for(dwY=0;dwY<g_dwNumRows;dwY++)
			for(dwX=0;dwX<g_dwNumCols+1;dwX++)
				if(GetVStick(dwCount,dwX,dwY))
					OutputSprite(g_dwTex[0],
								 ScrCoords(g_rPlayingField[dwCount].left+(NOM_STICK_SIZE/2)               +(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   g_rPlayingField[dwCount].top +(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STICK_SIZE,NOM_STONE_SIZE));

		// render stones
		for(dwY=0;dwY<g_dwNumRows;dwY++)
			for(dwX=0;dwX<g_dwNumCols;dwX++)
				if(GetStone(dwCount,dwX,dwY))
					OutputSprite(g_dwTex[1],
								 ScrCoords(g_rPlayingField[dwCount].left+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   g_rPlayingField[dwCount].top +(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STONE_SIZE,NOM_STONE_SIZE));
	}

	// remove player pieces
	for(dwCount=0;dwCount<g_dwNumPlayers;dwCount++)
		RemovePlayerPiece(dwCount);
}

// ComputePlayingFieldExtents()

void ComputePlayingFieldExtents(void)
{
	// compute width
	DWORD dwWidth=(NOM_STONE_SIZE* g_dwNumCols   )+
				  (NOM_STICK_SIZE*(g_dwNumCols+1));

	// compute height
	DWORD dwHeight=(NOM_STONE_SIZE* g_dwNumRows   )+
				   (NOM_STICK_SIZE*(g_dwNumRows+1));

	// check number of fields
	if(g_dwNumFields==1)
	{
		// compute upper-left
		g_rPlayingField[0].left=320-(dwWidth >>1);
		g_rPlayingField[0].top =240-(dwHeight>>1);

		// compute lower-right
		g_rPlayingField[0].right =g_rPlayingField[0].left+dwWidth;
		g_rPlayingField[0].bottom=g_rPlayingField[0].top +dwHeight;
	}
	else
	{
		// compute upper-left
		g_rPlayingField[0].left=160-(dwWidth >>1);
		g_rPlayingField[0].top =240-(dwHeight>>1);
		g_rPlayingField[1].left=480-(dwWidth >>1);
		g_rPlayingField[1].top =240-(dwHeight>>1);

		// compute lower-right
		g_rPlayingField[0].right =g_rPlayingField[0].left+dwWidth;
		g_rPlayingField[0].bottom=g_rPlayingField[0].top +dwHeight;
		g_rPlayingField[1].right =g_rPlayingField[1].left+dwWidth;
		g_rPlayingField[1].bottom=g_rPlayingField[1].top +dwHeight;
	}
}

// SetGameType()

void SetGameType(GAMETYPE gt)
{
	// save value
	g_gtType=gt;

	// check value
	switch(gt)
	{
	case(GT_1PLAYER_A):
		// set values
		g_dwNumRows=15;
		g_dwNumCols=8;
		g_dwNumFields=1;
		g_dwNumPlayers=1;

		// ok
		break;

	case(GT_1PLAYER_B):
		// set values
		g_dwNumRows=15;
		g_dwNumCols=8;
		g_dwNumFields=1;
		g_dwNumPlayers=1;

		// ok
		break;

	case(GT_2PLAYER_A):
		// set values
		g_dwNumRows=15;
		g_dwNumCols=12;
		g_dwNumFields=1;
		g_dwNumPlayers=2;

		// ok
		break;

	case(GT_2PLAYER_B):
		// set values
		g_dwNumRows=15;
		g_dwNumCols=8;
		g_dwNumFields=2;
		g_dwNumPlayers=2;

		// ok
		break;
	}

	// compute extents
	ComputePlayingFieldExtents();
}

// GeneratePiece()

DWORD GeneratePiece(void)
{
	// compute value
	DWORD dwVal=((DWORD)GetRandomInt())&15;
	
	// return value
	return(dwVal<<2);
}

// ResetPlayerPiece()

void ResetPlayerPiece(DWORD dwPlayer)
{
	// reset player piece
	g_dwCurPiece [dwPlayer]=GeneratePiece();
	g_dwNextPiece[dwPlayer]=GeneratePiece();
	g_dwCurXPos  [dwPlayer]=g_dwNumCols>>1;
	g_dwCurYPos  [dwPlayer]=1;
	g_bFastFall  [dwPlayer]=FALSE;
	g_dwHoldFall [dwPlayer]=60;
}

// InitStandardPieces()

BOOL InitStandardPieces(void)
{
	// set bits for each piece          "ssssvvvvvvhhhhhh"
	g_wStdPiece [0]=0x2000;//(WORD)BinaryToDWORD("0010000000000000");
	g_wStdPiece [1]=0x2000;//(WORD)BinaryToDWORD("0010000000000000");
	g_wStdPiece [2]=0x2000;//(WORD)BinaryToDWORD("0010000000000000");
	g_wStdPiece [3]=0x2000;//(WORD)BinaryToDWORD("0010000000000000");
	g_wStdPiece [4]=0x2002;//(WORD)BinaryToDWORD("0010000000000010");
	g_wStdPiece [5]=0x2100;//(WORD)BinaryToDWORD("0010000100000000");
	g_wStdPiece [6]=0x2008;//(WORD)BinaryToDWORD("0010000000001000");
	g_wStdPiece [7]=0x2080;//(WORD)BinaryToDWORD("0010000010000000");
	g_wStdPiece [8]=0x2102;//(WORD)BinaryToDWORD("0010000100000010");
	g_wStdPiece [9]=0x2108;//(WORD)BinaryToDWORD("0010000100001000");
	g_wStdPiece[10]=0x2088;//(WORD)BinaryToDWORD("0010000010001000");
	g_wStdPiece[11]=0x2082;//(WORD)BinaryToDWORD("0010000010000010");
	g_wStdPiece[12]=0x3000;//(WORD)BinaryToDWORD("0011000000000000");
	g_wStdPiece[13]=0xA000;//(WORD)BinaryToDWORD("1010000000000000");
	g_wStdPiece[14]=0x3000;//(WORD)BinaryToDWORD("0011000000000000");
	g_wStdPiece[15]=0xA000;//(WORD)BinaryToDWORD("1010000000000000");
	g_wStdPiece[16]=0x3003;//(WORD)BinaryToDWORD("0011000000000011");
	g_wStdPiece[17]=0xA900;//(WORD)BinaryToDWORD("1010100100000000");
	g_wStdPiece[18]=0x300C;//(WORD)BinaryToDWORD("0011000000001100");
	g_wStdPiece[19]=0xA480;//(WORD)BinaryToDWORD("1010010010000000");
	g_wStdPiece[20]=0x3043;//(WORD)BinaryToDWORD("0011000001000011");
	g_wStdPiece[21]=0xA902;//(WORD)BinaryToDWORD("1010100100000010");
	g_wStdPiece[22]=0x310C;//(WORD)BinaryToDWORD("0011000100001100");
	g_wStdPiece[23]=0xA4A0;//(WORD)BinaryToDWORD("1010010010100000");
	g_wStdPiece[24]=0x3103;//(WORD)BinaryToDWORD("0011000100000011");
	g_wStdPiece[25]=0xA920;//(WORD)BinaryToDWORD("1010100100100000");
	g_wStdPiece[26]=0x304C;//(WORD)BinaryToDWORD("0011000001001100");
	g_wStdPiece[27]=0xA482;//(WORD)BinaryToDWORD("1010010010000010");
	g_wStdPiece[28]=0x3083;//(WORD)BinaryToDWORD("0011000010000011");
	g_wStdPiece[29]=0xA908;//(WORD)BinaryToDWORD("1010100100001000");
	g_wStdPiece[30]=0x308C;//(WORD)BinaryToDWORD("0011000010001100");
	g_wStdPiece[31]=0xA488;//(WORD)BinaryToDWORD("1010010010001000");
	g_wStdPiece[32]=0xB000;//(WORD)BinaryToDWORD("1011000000000000");
	g_wStdPiece[33]=0xE000;//(WORD)BinaryToDWORD("1110000000000000");
	g_wStdPiece[34]=0xD000;//(WORD)BinaryToDWORD("1101000000000000");
	g_wStdPiece[35]=0x7000;//(WORD)BinaryToDWORD("0111000000000000");
	g_wStdPiece[36]=0xB903;//(WORD)BinaryToDWORD("1011100100000011");
	g_wStdPiece[37]=0xE930;//(WORD)BinaryToDWORD("1110100100110000");
	g_wStdPiece[38]=0xD270;//(WORD)BinaryToDWORD("1101001001110000");
	g_wStdPiece[39]=0x7243;//(WORD)BinaryToDWORD("0111001001000011");
	g_wStdPiece[40]=0xB464;//(WORD)BinaryToDWORD("1011010001100100");
	g_wStdPiece[41]=0xE286;//(WORD)BinaryToDWORD("1110001010000110");
	g_wStdPiece[42]=0xD889;//(WORD)BinaryToDWORD("1101100010001001");
	g_wStdPiece[43]=0x7518;//(WORD)BinaryToDWORD("0111010100011000");
	g_wStdPiece[44]=0xF000;//(WORD)BinaryToDWORD("1111000000000000");
	g_wStdPiece[45]=0xF000;//(WORD)BinaryToDWORD("1111000000000000");
	g_wStdPiece[46]=0xF000;//(WORD)BinaryToDWORD("1111000000000000");
	g_wStdPiece[47]=0xF000;//(WORD)BinaryToDWORD("1111000000000000");
	g_wStdPiece[48]=0xF003;//(WORD)BinaryToDWORD("1111000000000011");
	g_wStdPiece[49]=0xF900;//(WORD)BinaryToDWORD("1111100100000000");
	g_wStdPiece[50]=0xF030;//(WORD)BinaryToDWORD("1111000000110000");
	g_wStdPiece[51]=0xF240;//(WORD)BinaryToDWORD("1111001001000000");
	g_wStdPiece[52]=0xF243;//(WORD)BinaryToDWORD("1111001001000011");
	g_wStdPiece[53]=0xF903;//(WORD)BinaryToDWORD("1111100100000011");
	g_wStdPiece[54]=0xF930;//(WORD)BinaryToDWORD("1111100100110000");
	g_wStdPiece[55]=0xF270;//(WORD)BinaryToDWORD("1111001001110000");
	g_wStdPiece[56]=0xF483;//(WORD)BinaryToDWORD("1111010010000011");
	g_wStdPiece[57]=0xF90C;//(WORD)BinaryToDWORD("1111100100001100");
	g_wStdPiece[58]=0xF4B0;//(WORD)BinaryToDWORD("1111010010110000");
	g_wStdPiece[59]=0xF24C;//(WORD)BinaryToDWORD("1111001001001100");
	g_wStdPiece[60]=0xF48C;//(WORD)BinaryToDWORD("1111010010001100");
	g_wStdPiece[61]=0xF48C;//(WORD)BinaryToDWORD("1111010010001100");
	g_wStdPiece[62]=0xF48C;//(WORD)BinaryToDWORD("1111010010001100");
	g_wStdPiece[63]=0xF48C;//(WORD)BinaryToDWORD("1111010010001100");
	
	// ok
	return(TRUE);
}

// InsertPlayerPiece()

void InsertPlayerPiece(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos =g_dwCurXPos[dwPlayer];
	DWORD dwYPos =g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// insert horizontal sticks
	for(dwY=0;dwY<3;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdHStick(dwPiece,dwX,dwY))
				SetHStick(dwField,
						  (dwXPos-1)+dwX,
						  (dwYPos-1)+dwY);

	// insert vertical sticks
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<3;dwX++)
			if(StdVStick(dwPiece,dwX,dwY))
				SetVStick(dwField,
						  (dwXPos-1)+dwX,
						  (dwYPos-1)+dwY);

	// insert stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				SetStone(dwField,
						 (dwXPos-1)+dwX,
						 (dwYPos-1)+dwY);
}

// RemovePlayerPiece()

void RemovePlayerPiece(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos =g_dwCurXPos[dwPlayer];
	DWORD dwYPos =g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// remove horizontal sticks
	for(dwY=0;dwY<3;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdHStick(dwPiece,dwX,dwY))
				ResetHStick(dwField,
						    (dwXPos-1)+dwX,
						    (dwYPos-1)+dwY);

	// remove vertical sticks
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<3;dwX++)
			if(StdVStick(dwPiece,dwX,dwY))
				ResetVStick(dwField,
						    (dwXPos-1)+dwX,
						    (dwYPos-1)+dwY);

	// remove stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				ResetStone(dwField,
						   (dwXPos-1)+dwX,
						   (dwYPos-1)+dwY);
}

// RenderNextPieces()

void RenderNextPieces(void)
{
	// counter
	DWORD dwCount;
	
	// counters
	DWORD dwX;
	DWORD dwY;

	// position values
	DWORD dwXPos;
	DWORD dwYPos;

	// render each player's piece
	for(dwCount=0;dwCount<g_dwNumPlayers;dwCount++)
	{
		// get player values
		DWORD dwPiece=g_dwNextPiece[dwCount];

		// set position
		if(dwCount==0)
		{
			dwXPos=NOM_STICK_SIZE;
			dwYPos=NOM_STICK_SIZE;
		}
		else
		{
			dwXPos=640-((NOM_STICK_SIZE*4)+(NOM_STONE_SIZE*2));
			dwYPos=NOM_STICK_SIZE;
		}

		// render horizontal sticks
		for(dwY=0;dwY<3;dwY++)
			for(dwX=0;dwX<2;dwX++)
				if(StdHStick(dwPiece,dwX,dwY))
					OutputSprite(g_dwTex[0],
								 ScrCoords(dwXPos+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   dwYPos+(NOM_STICK_SIZE/2)               +(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STONE_SIZE,NOM_STICK_SIZE));

		// render vertical sticks
		for(dwY=0;dwY<2;dwY++)
			for(dwX=0;dwX<3;dwX++)
				if(StdVStick(dwPiece,dwX,dwY))
					OutputSprite(g_dwTex[0],
								 ScrCoords(dwXPos+(NOM_STICK_SIZE/2)               +(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   dwYPos+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STICK_SIZE,NOM_STONE_SIZE));
			
		// render stones
		for(dwY=0;dwY<2;dwY++)
			for(dwX=0;dwX<2;dwX++)
				if(StdStone(dwPiece,dwX,dwY))
					OutputSprite(g_dwTex[1],
								 ScrCoords(dwXPos+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwX*(NOM_STICK_SIZE+NOM_STONE_SIZE)),
										   dwYPos+(NOM_STONE_SIZE/2)+NOM_STICK_SIZE+(dwY*(NOM_STICK_SIZE+NOM_STONE_SIZE))),
								 ScrSize(NOM_STONE_SIZE,NOM_STONE_SIZE));
	}
}

// CanPieceMoveLeft()

BOOL CanPieceMoveLeft(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;
	
	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos=g_dwCurXPos[dwPlayer];
	DWORD dwYPos=g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// check against borders
	if(HasLeftBlocks(dwPiece))
	{
		if(dwXPos==1)
			return(FALSE);
	}
	else
	{
		if(dwXPos==0)
			return(FALSE);
	}

	// check interference
	if(DoesPieceInterfere(dwField,
						  dwXPos-1,
						  dwYPos,
						  dwPiece))
		return(FALSE);

	// check vertical sticks
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<3;dwX++)
			if(StdVStick(dwPiece,dwX,dwY))
				if(GetStone(dwField,
							(dwXPos-2)+dwX,
							(dwYPos-1)+dwY))
					return(FALSE);

	// check stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				if(GetVStick(dwField,
							 (dwXPos-1)+dwX,
							 (dwYPos-1)+dwY))
					return(FALSE);

	// ok
	return(TRUE);
}

// CanPieceMoveRight()

BOOL CanPieceMoveRight(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;
	
	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos=g_dwCurXPos[dwPlayer];
	DWORD dwYPos=g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// check against borders
	if(HasRightBlocks(dwPiece))
	{
		if(dwXPos==g_dwNumCols-1)
			return(FALSE);
	}
	else
	{
		if(dwXPos==g_dwNumCols)
			return(FALSE);
	}

	// check interference
	if(DoesPieceInterfere(dwField,
						  dwXPos+1,
						  dwYPos,
						  dwPiece))
		return(FALSE);

	// check vertical sticks
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<3;dwX++)
			if(StdVStick(dwPiece,dwX,dwY))
				if(GetStone(dwField,
							(dwXPos-1)+dwX,
							(dwYPos-1)+dwY))
					return(FALSE);

	// check stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				if(GetVStick(dwField,
							 dwXPos+dwX,
							 (dwYPos-1)+dwY))
					return(FALSE);

	// ok
	return(TRUE);
}

// CanPieceMoveDown()

DWORD CanPieceMoveDown(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;
	
	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos=g_dwCurXPos[dwPlayer];
	DWORD dwYPos=g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// check against borders
	if(HasBottomBlocks(dwPiece))
	{
		if(dwYPos==g_dwNumRows-1)
			return(FALSE);
	}
	else
	{
		if(dwYPos==g_dwNumRows)
			return(FALSE);
	}

	// get interference
	DWORD dwIntf=DoesPieceInterfere(dwField,
									dwXPos,
									dwYPos+1,
									dwPiece);

	// check interference
	if(dwIntf)
	{
		// check for horizontal sticks only
		if(dwIntf==INTF_HSTICKS)
			return(TEMPORARILY);
		else
			return(FALSE);
	}

	// check horizontal sticks
	for(dwY=0;dwY<3;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdHStick(dwPiece,dwX,dwY))
				if(GetStone(dwField,
							(dwXPos-1)+dwX,
							(dwYPos-1)+dwY))
					return(FALSE);

	// check stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				if(GetHStick(dwField,
							 (dwXPos-1)+dwX,
							 dwYPos+dwY))
					return(FALSE);

	// ok
	return(TRUE);
}

// CanPieceRotateLeft()

BOOL CanPieceRotateLeft(DWORD dwPlayer)
{
	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos=g_dwCurXPos[dwPlayer];
	DWORD dwYPos=g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// roatate piece left
	RotateLeft(dwPiece);

	// check against left border
	if(HasLeftBlocks(dwPiece))
		if(dwXPos==0)
			return(FALSE);

	// check against right border
	if(HasRightBlocks(dwPiece))
		if(dwXPos==g_dwNumCols)
			return(FALSE);

	// check against top border
	if(HasTopBlocks(dwPiece))
		if(dwYPos==0)
			return(FALSE);

	// check against bottom border
	if(HasBottomBlocks(dwPiece))
		if(dwYPos==g_dwNumRows)
			return(FALSE);

	// check interference
	if(DoesPieceInterfere(dwField,
						  dwXPos,
						  dwYPos,
						  dwPiece))
		return(FALSE);

	// ok
	return(TRUE);
}

// CanPieceRotateRight()

BOOL CanPieceRotateRight(DWORD dwPlayer)
{
	// get player values
	DWORD dwPiece=g_dwCurPiece[dwPlayer];
	DWORD dwXPos=g_dwCurXPos[dwPlayer];
	DWORD dwYPos=g_dwCurYPos[dwPlayer];

	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// rotate piece right
	RotateRight(dwPiece);

	// check against left border
	if(HasLeftBlocks(dwPiece))
		if(dwXPos==0)
			return(FALSE);

	// check against right border
	if(HasRightBlocks(dwPiece))
		if(dwXPos==g_dwNumCols)
			return(FALSE);

	// check against top border
	if(HasTopBlocks(dwPiece))
		if(dwYPos==0)
			return(FALSE);

	// check against bottom border
	if(HasBottomBlocks(dwPiece))
		if(dwYPos==g_dwNumRows)
			return(FALSE);

	// check interference
	if(DoesPieceInterfere(dwField,
						  dwXPos,
						  dwYPos,
						  dwPiece))
		return(FALSE);

	// ok
	return(TRUE);
}

// SetLevel()

void SetLevel(DWORD dwPlayer,
			  DWORD dwLevel)
{
	// set level
	g_dwCurLevel[dwPlayer]=dwLevel;

	// compute fall skip
	if(dwLevel>=30)
		g_dwFallSkip[dwPlayer]=0;
	else
		g_dwFallSkip[dwPlayer]=90-(dwLevel*3);
}

// IncrementLevel()

void IncrementLevel(DWORD dwPlayer)
{
	// set next level
	SetLevel(dwPlayer,
			 g_dwCurLevel[dwPlayer]+1);
}

// DoesPieceInterfere()

DWORD DoesPieceInterfere(DWORD dwField,
						 DWORD dwXPos,
						 DWORD dwYPos,
						 DWORD dwPiece)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// reset return value
	DWORD dwRet=0;

	// check horizontal sticks
	for(dwY=0;dwY<3;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdHStick(dwPiece,dwX,dwY))
				if(GetHStick(dwField,
							 (dwXPos-1)+dwX,
							 (dwYPos-1)+dwY))
					dwRet|=INTF_HSTICKS;

	// check vertical sticks
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<3;dwX++)
			if(StdVStick(dwPiece,dwX,dwY))
				if(GetVStick(dwField,
							 (dwXPos-1)+dwX,
							 (dwYPos-1)+dwY))
					dwRet|=INTF_VSTICKS;

	// check stones
	for(dwY=0;dwY<2;dwY++)
		for(dwX=0;dwX<2;dwX++)
			if(StdStone(dwPiece,dwX,dwY))
				if(GetStone(dwField,
							(dwXPos-1)+dwX,
							(dwYPos-1)+dwY))
					dwRet|=INTF_STONES;

	// return value
	return(dwRet);
}

// FillStoneEdges

void FillStoneEdges(DWORD dwField)
{
	// counters
	DWORD dwX;
	DWORD dwY;

	// check stones on vertical edges
	for(dwY=0;dwY<g_dwNumRows;dwY++)
	{
		// check left side
		if(GetStone(dwField,0,dwY))
			SetVStick(dwField,0,dwY);

		// check right side
		if(GetStone(dwField,g_dwNumCols-1,dwY))
			SetVStick(dwField,g_dwNumCols,dwY);
	}

	// check stones on horizontal edges
	for(dwX=0;dwX<g_dwNumCols;dwX++)
	{
		// check top
		if(GetStone(dwField,dwX,0))
			SetHStick(dwField,dwX,0);

		// check bottom
		if(GetStone(dwField,dwX,g_dwNumRows-1))
			SetHStick(dwField,dwX,g_dwNumRows);
	}	
}

// LoadSticksAndStonesData()

BOOL LoadSticksAndStones(void)
{
	// load textures
	g_dwTex[0]=AddTexture("red1.bmp");
	g_dwTex[1]=AddTexture("red2.bmp");
	g_dwTex[2]=AddTexture("black.bmp");

	// init gamepieces
	if(!InitStandardPieces())
		return(FALSE);	

	// ok
	return(TRUE);
}

// SetShowNextFlag()

void SetShowNextFlag(DWORD dwPlayer,
					 BOOL bFlag)
{
	// set flag
	g_bShowNext[dwPlayer]=bFlag;
}

// DetectClosedLoops()

DWORD DetectClosedLoops(DWORD dwPlayer)
{
	// counters
	DWORD dwX;
	DWORD dwY;
	
	// set field number
	DWORD dwField;
	if(g_dwNumFields==1)
		dwField=0;
	else
		dwField=dwPlayer;

	// fill stone edges
	FillStoneEdges(dwField);

	// copy playing field to valid field
	CopyMemory(g_wValid,
			   &g_wStones[dwField<<4],
			   sizeof(WORD)*16);

	// clear collapse field
	ZeroMemory(g_wCollapse,
			   sizeof(WORD)*16);

	// reset number of loops
	DWORD dwNumLoops=0;

	// scan each block
	for(dwY=0;dwY<15;dwY++)
		for(dwX=0;dwX<8;dwX++)
		{
			// clear checked field
			ZeroMemory(g_wChecked,
					   sizeof(WORD)*16);

			// search for loops
			if(SearchLoopPath(dwField,dwX,dwY))
			{
				// mark stones to collapse
				MarkLoopPath(dwField,dwX,dwY);

				// increment loop counter
				dwNumLoops++;
			}
		}

	// copy collapse field to player collapse field
	CopyMemory(&g_wColStones[dwField<<4],
			   g_wCollapse,
			   sizeof(WORD)*16);

	// return number of loops
	return(dwNumLoops);
}

// SearchLoopPath()

BOOL SearchLoopPath(DWORD dwField,
					DWORD dwX,
					DWORD dwY)
{
	// see if stone is valid
	if(!GetValStone(dwX,dwY))
		return(FALSE);
	
	// see if stone is already marked to collapse
	if(GetColStone(dwX,dwY))
		return(FALSE);

	// see if stone is already checked
	if(GetChkStone(dwX,dwY))
		return(TRUE);

	// mark stone as checked
	SetChkStone(dwX,dwY);

	// check path in all directions
	CHECKPATH cpUp   =CheckLoopPath(dwField,dwX,dwY,SP_UP);
	CHECKPATH cpDown =CheckLoopPath(dwField,dwX,dwY,SP_DOWN);
	CHECKPATH cpLeft =CheckLoopPath(dwField,dwX,dwY,SP_LEFT);
	CHECKPATH cpRight=CheckLoopPath(dwField,dwX,dwY,SP_RIGHT);
	
	// check for invalid flags
	if(cpUp   ==CP_INVALID||
	   cpDown ==CP_INVALID||
	   cpLeft ==CP_INVALID||
	   cpRight==CP_INVALID)
	{
		// path not valid
		ResetValStone(dwX,dwY);
		return(FALSE);
	}

	// check up if necessary
	if(cpUp==CP_CONTINUE)
		if(!SearchLoopPath(dwField,dwX,dwY-1))
		{
			// path not valid
			ResetValStone(dwX,dwY);
			return(FALSE);
		}

	// check down if necessary
	if(cpDown==CP_CONTINUE)
		if(!SearchLoopPath(dwField,dwX,dwY+1))
		{
			// path not valid
			ResetValStone(dwX,dwY);
			return(FALSE);
		}

	// check left if necessary
	if(cpLeft==CP_CONTINUE)
		if(!SearchLoopPath(dwField,dwX-1,dwY))
		{
			// path not valid
			ResetValStone(dwX,dwY);
			return(FALSE);
		}

	// check right if necessary
	if(cpRight==CP_CONTINUE)
		if(!SearchLoopPath(dwField,dwX+1,dwY))
		{
			// path not valid
			ResetValStone(dwX,dwY);
			return(FALSE);
		}

	// valid path found
	return(TRUE);
}

// CheckLoopPath()

CHECKPATH CheckLoopPath(DWORD dwField,
						DWORD dwX,
						DWORD dwY,
						SEARCHPATH spDir)
{
	// check direction
	switch(spDir)
	{
	// check up
	case(SP_UP):
		// check for blockage
		if(GetHStick(dwField,dwX,dwY))
			return(CP_STOP);

		// check for valid stone
		if(GetValStone(dwX,dwY-1))
			return(CP_CONTINUE);

		// ok
		break;

	// check down
	case(SP_DOWN):
		// check for blockage
		if(GetHStick(dwField,dwX,dwY+1))
			return(CP_STOP);

		// check for valid stone
		if(GetValStone(dwX,dwY+1))
			return(CP_CONTINUE);

		// ok
		break;

	// check left
	case(SP_LEFT):
		// check for blockage
		if(GetVStick(dwField,dwX,dwY))
			return(CP_STOP);

		// check for valid stone
		if(GetValStone(dwX-1,dwY))
			return(CP_CONTINUE);

		// ok
		break;

	// check right
	case(SP_RIGHT):
		// check for blockage
		if(GetVStick(dwField,dwX+1,dwY))
			return(CP_STOP);

		// check for valid stone
		if(GetValStone(dwX+1,dwY))
			return(CP_CONTINUE);

		// ok
		break;
	}
	
	// no valid paths
	return(CP_INVALID);
}

// MarkLoopPath()

void MarkLoopPath(DWORD dwField,
				  DWORD dwX,
				  DWORD dwY)
{
	// see if stone is already marked to collapse
	if(GetColStone(dwX,dwY))
		return;

	// mark stone as collapsing
	SetColStone(dwX,dwY);

	// check path in all directions
	CHECKPATH cpUp   =CheckLoopPath(dwField,dwX,dwY,SP_UP);
	CHECKPATH cpDown =CheckLoopPath(dwField,dwX,dwY,SP_DOWN);
	CHECKPATH cpLeft =CheckLoopPath(dwField,dwX,dwY,SP_LEFT);
	CHECKPATH cpRight=CheckLoopPath(dwField,dwX,dwY,SP_RIGHT);
	
	// check up if necessary
	if(cpUp==CP_CONTINUE)
		MarkLoopPath(dwField,dwX,dwY-1);

	// check down if necessary
	if(cpDown==CP_CONTINUE)
		MarkLoopPath(dwField,dwX,dwY+1);

	// check left if necessary
	if(cpLeft==CP_CONTINUE)
		MarkLoopPath(dwField,dwX-1,dwY);

	// check right if necessary
	if(cpRight==CP_CONTINUE)
		MarkLoopPath(dwField,dwX+1,dwY);
}

// AddInputSettings()

BOOL AddInputSettings(void)
{
	// purge any previous settings
	PurgeInputSettings();
	
	// add axes
	g_dwXAxis[0]=AddInputAxis();
	g_dwYAxis[0]=AddInputAxis();
	g_dwXAxis[1]=AddInputAxis();
	g_dwYAxis[1]=AddInputAxis();

	// add buttons
	g_dwButton1[0]=AddInputButton();
	g_dwButton2[0]=AddInputButton();
	g_dwCancel [0]=AddInputButton();
	g_dwButton1[1]=AddInputButton();
	g_dwButton2[1]=AddInputButton();
	g_dwCancel [1]=AddInputButton();

	// add triggers
	if(FALSE/*g_dwNumJoystick==2*/)
	{
		// add player 1
		AddInputAxisTrigger(g_dwXAxis[0],SetTriggerXAxis(1));
		AddInputAxisTrigger(g_dwYAxis[0],SetTriggerYAxis(1));
		AddInputButtonTrigger(g_dwButton1[0],SetTriggerButton(1,0));
		AddInputButtonTrigger(g_dwButton2[0],SetTriggerButton(1,1));
		AddInputButtonTrigger(g_dwCancel[0],DIK_ESCAPE);

		// add player 2
		AddInputAxisTrigger(g_dwXAxis[1],DIK_LEFT,DIK_RIGHT);
		AddInputAxisTrigger(g_dwYAxis[1],DIK_UP,DIK_DOWN);
		AddInputButtonTrigger(g_dwButton1[1],DIK_RSHIFT);
		AddInputButtonTrigger(g_dwButton2[1],DIK_RCONTROL);
		AddInputButtonTrigger(g_dwCancel[1],DIK_ESCAPE);
	}
	else
	{
		// add player 1
		AddInputAxisTrigger(g_dwXAxis[0],DIK_LEFT,DIK_RIGHT);
		AddInputAxisTrigger(g_dwYAxis[0],DIK_UP,DIK_DOWN);
		AddInputButtonTrigger(g_dwButton1[0],DIK_RSHIFT);
		AddInputButtonTrigger(g_dwButton2[0],DIK_RCONTROL);
		AddInputButtonTrigger(g_dwCancel[0],DIK_ESCAPE);

		// add player 2
		AddInputAxisTrigger(g_dwXAxis[1],DIK_S,DIK_F);
		AddInputAxisTrigger(g_dwYAxis[1],DIK_E,DIK_D);
		AddInputButtonTrigger(g_dwButton1[1],DIK_GRAVE);
		AddInputButtonTrigger(g_dwButton2[1],DIK_1);
		AddInputButtonTrigger(g_dwCancel[1],DIK_ESCAPE);
	}
	
	// ok
	return(TRUE);
}

// UpdateSticksAndStones()

BOOL UpdateSticksAndStones(void)
{
	// counter
	DWORD dwCount;

	// field number
	DWORD dwField;

	// internal data
	static DWORD dwCurRow[2];
	DWORD dwXPos;
	DWORD dwYPos;
	BOOL bContinue;
	
	// read user input
	ReadUserInput();

	// confirm game type is set
	if(g_gtType==GT_NULL)
	{
		// set game type
		SetGameType(GT_1PLAYER_A);

		// reset game states
		g_psState[0]=ST_NULL;
		g_psState[1]=ST_NULL;
	}
	
	// update each player
	for(dwCount=0;dwCount<g_dwNumPlayers;dwCount++)
	{
		// set field number
		if(g_dwNumFields==1)
			dwField=0;
		else
			dwField=dwCount;

		// check state
		switch(g_psState[dwCount])
		{
		// state not set
		case(ST_NULL):
			// set level
			SetLevel(dwCount,0);

			// set show-next flag
			SetShowNextFlag(dwCount,TRUE);
			
			// clear playing field
			ClearPlayingField(dwCount);

			// reset player piece
			ResetPlayerPiece(dwCount);

			// set state to play game
			g_psState[dwCount]=ST_PLAYGAME;

			// ok
			break;

		// general gameplay
		case(ST_PLAYGAME):
			// generate piece if necessary
			if(g_dwCurPiece[dwCount]==NO_OBJECT)
			{
				// reset piece
				g_dwCurPiece [dwCount]=g_dwNextPiece[dwCount];
				g_dwNextPiece[dwCount]=GeneratePiece();
				g_dwCurXPos  [dwCount]=g_dwNumCols>>1;
				g_dwCurYPos  [dwCount]=1;
				g_dwFallVal  [dwCount]=0;
				g_bFastFall  [dwCount]=FALSE;

				// check interference
				if(DoesPieceInterfere(dwField,
									  g_dwCurXPos[dwCount],
									  g_dwCurYPos[dwCount],
									  g_dwCurPiece[dwCount]))
				{
					// insert piece
					InsertPlayerPiece(dwCount);

					// set state to game over
					g_psState[dwCount]=ST_GAMEOVER;

					// ok
					break;
				}
			}

			// check user input (left)
			if(IsAxisMinHit(g_dwXAxis[dwCount]))
				if(CanPieceMoveLeft(dwCount))
					g_dwCurXPos[dwCount]--;

			// check user input (right)
			if(IsAxisMaxHit(g_dwXAxis[dwCount]))
				if(CanPieceMoveRight(dwCount))
					g_dwCurXPos[dwCount]++;

			// check user input (down)
			if(IsAxisMaxDown(g_dwYAxis[dwCount]))
			{
				// check for new fast-fall
				if(IsAxisMaxHit(g_dwYAxis[dwCount]))
				{
					g_bFastFall[dwCount]=TRUE;
					g_dwFallVal[dwCount]=g_dwFallSkip[dwCount];
				}
				// check for old fast-fall
				else if(g_bFastFall[dwCount])
					g_dwFallVal[dwCount]=g_dwFallSkip[dwCount];

				// reset hold-fall
				g_dwHoldFall[dwCount]=0;
			}

			// check user input (button 1)
			if(IsButtonHit(g_dwButton1[dwCount]))
				if(CanPieceRotateRight(dwCount))
					RotateRight(g_dwCurPiece[dwCount]);

			// check user input (button 2)
			if(IsButtonHit(g_dwButton2[dwCount]))
				if(CanPieceRotateLeft(dwCount))
					RotateLeft(g_dwCurPiece[dwCount]);

			// check user input (cancel)
			if(IsButtonHit(g_dwCancel[dwCount]))
				return(FALSE);

			// increment fall value as needed
			if(g_dwHoldFall[dwCount])
				g_dwHoldFall[dwCount]--;
			else
				g_dwFallVal[dwCount]++;
			
			// check fall value
			if(g_dwFallVal[dwCount]>=g_dwFallSkip[dwCount])
			{
				// reset fall value
				g_dwFallVal[dwCount]=0;
				
				// check downward movement
				DWORD dwDown=CanPieceMoveDown(dwCount);
				
				// fall if possible
				if(dwDown!=FALSE)
				{
					// move piece down
					g_dwCurYPos[dwCount]++;
				}

				// plant piece if necessary
				if(dwDown!=TRUE)
				{
					// insert piece
					InsertPlayerPiece(dwCount);

					// reset piece
					g_dwCurPiece[dwCount]=NO_OBJECT;

					// detect closed loops
					if(DetectClosedLoops(dwCount))
					{
						// reset current row
						dwCurRow[dwField]=0;
						
						// set state to destroy
						g_psState[dwCount]=ST_DESTROY;
					}
				}
			}

			// ok
			break;

		// destroy
		case(ST_DESTROY):
			// check each row
			while(dwCurRow[dwField]<g_dwNumRows)
			{
				// set y-counter to current row
				dwYPos=dwCurRow[dwField];
				
				// skip blank rows
				if(g_wColStones[dwYPos+(dwField<<4)])
				{
					// remove current row
					for(dwXPos=0;dwXPos<g_dwNumCols;dwXPos++)
						if(GetColStoneA(dwField,dwXPos,dwYPos))
						{
							// remove horizontal sticks
							ResetHStick(dwField,dwXPos,dwYPos);
							ResetHStick(dwField,dwXPos,dwYPos+1);

							// remove vertical sticks
							ResetVStick(dwField,dwXPos,  dwYPos);
							ResetVStick(dwField,dwXPos+1,dwYPos);

							// remove stone
							ResetStone(dwField,dwXPos,dwYPos);
						}
					
					// increment current row
					dwCurRow[dwField]++;

					// ok
					break;
				}
				else
				{
					// increment current row
					dwCurRow[dwField]++;
				}
			}
			
			// check current row
			if(dwCurRow[dwField]>=g_dwNumRows)
			{
				// update collapse values
				UpdateCollapseValues(dwField);

				// reset current row
				dwCurRow[dwField]=0;
				
				// set state to collapse
				g_psState[dwCount]=ST_COLLAPSE;
			}

			// ok
			break;

		// collapse
		case(ST_COLLAPSE):
			// reset continue flag
			bContinue=FALSE;

			// search for bottom-most vertical sticks
			for(dwXPos=0;dwXPos<=g_dwNumCols;dwXPos++)
				for(dwYPos=0;dwYPos<g_dwNumRows;dwYPos++)
					if(GetColVStick(dwField,dwXPos,g_dwNumRows-(dwYPos+1)))
					{
						// skip current row
						dwYPos++;
						
						// move all remaining sticks down
						while(dwYPos<g_dwNumRows)
						{
							// check sticks
							if( GetVStick(dwField,dwXPos, g_dwNumRows-(dwYPos+1)   )&&
							   !GetVStick(dwField,dwXPos,(g_dwNumRows-(dwYPos+1))+1))
							{
								// move stick
								SetVStick  (dwField,dwXPos,(g_dwNumRows-(dwYPos+1))+1);
								ResetVStick(dwField,dwXPos, g_dwNumRows-(dwYPos+1)   );

								// set flag
								bContinue=TRUE;
							}
							
							// increment y-position
							dwYPos++;
						}
					}
			
			// search for bottom-most horizontal sticks
			for(dwXPos=0;dwXPos<g_dwNumCols;dwXPos++)
				for(dwYPos=0;dwYPos<=g_dwNumRows;dwYPos++)
					if(GetColHStick(dwField,dwXPos,g_dwNumRows-dwYPos))
					{
						// skip current row
						dwYPos++;
						
						// move all remaining sticks & stones down
						while(dwYPos<=g_dwNumRows)
						{
							// check sticks
							if( GetHStick(dwField,dwXPos, g_dwNumRows-dwYPos )&&
							   !GetStone (dwField,dwXPos,(g_dwNumRows-dwYPos)))
							{
								
								// move stick
								SetHStick  (dwField,dwXPos,(g_dwNumRows-dwYPos)+1);
								ResetHStick(dwField,dwXPos, g_dwNumRows-dwYPos   );

								// set flag
								bContinue=TRUE;
							}
							
							// check stones
							if(dwYPos<g_dwNumRows)
								if( GetStone (dwField,dwXPos, g_dwNumRows-(dwYPos+1)   )&&
								   !GetHStick(dwField,dwXPos,(g_dwNumRows-(dwYPos+1))+1)&&
								   !GetStone (dwField,dwXPos,(g_dwNumRows-(dwYPos+1))+1))
								{
									// move stone
									SetStone  (dwField,dwXPos,(g_dwNumRows-(dwYPos+1))+1);
									ResetStone(dwField,dwXPos, g_dwNumRows-(dwYPos+1)   );

									// set flag
									bContinue=TRUE;
								}

							// increment y-position
							dwYPos++;
						}
					}
			
			// check continue flag
			if(!bContinue)
			{
				// detect closed loops
				if(DetectClosedLoops(dwCount))
				{
					// set state to destroy
					g_psState[dwCount]=ST_DESTROY;
				}
				else
				{
					// set state to play game
					g_psState[dwCount]=ST_PLAYGAME;
				}
			}

			// ok
			break;

		// game over
		case(ST_GAMEOVER):
			// wait for button
			if(IsButtonHit(g_dwButton1[dwCount])||
			   IsButtonHit(g_dwButton2[dwCount]))
			{
				// reset state to null
				g_psState[dwCount]=ST_NULL;
			}

			// check user cancel
			if(IsButtonHit(g_dwCancel[dwCount]))
				return(FALSE);

			// ok
			break;
		}
	}

	// ok
	return(TRUE);
}

// UpdateCollapseValues()

void UpdateCollapseValues(DWORD dwField)
{
	// counters
	DWORD dwCount;
	DWORD dwXPos;
	DWORD dwYPos;
	
	// reset stick collapse fields
	for(dwCount=0;dwCount<=g_dwNumRows;dwCount++)
	{
		// reset values
		g_wColHSticks[dwCount+(dwField<<4)]=0x0000;
		g_wColVSticks[dwCount+(dwField<<4)]=0x0000;
	}

	// update stick collapse fields
	for(dwYPos=0;dwYPos<g_dwNumRows;dwYPos++)
		for(dwXPos=0;dwXPos<g_dwNumCols;dwXPos++)
			if(GetColStoneA(dwField,dwXPos,dwYPos))
			{
				// set horizontal sticks
				SetColHStick(dwField,dwXPos,dwYPos);
				SetColHStick(dwField,dwXPos,dwYPos+1);

				// set vertical sticks
				SetColVStick(dwField,dwXPos,dwYPos);
				SetColVStick(dwField,dwXPos+1,dwYPos);
			}

	// leave only bottom-most horizontal stick
	for(dwXPos=0;dwXPos<g_dwNumCols;dwXPos++)
	{
		// reset flag
		BOOL bFlag=FALSE;

		// check each row
		for(dwYPos=0;dwYPos<=g_dwNumRows;dwYPos++)
			if(!bFlag)
			{
				// check horizontal stick
				if(GetColHStick(dwField,dwXPos,g_dwNumRows-dwYPos))
				{
					// set flag
					bFlag=TRUE;

					// set value
					SetColHStick(dwField,dwXPos,g_dwNumRows-dwYPos);
				}
			}
			else
			{
				// reset value
				ResetColHStick(dwField,dwXPos,g_dwNumRows-dwYPos);
			}
	}

	// leave only bottom-most vertical stick
	for(dwXPos=0;dwXPos<=g_dwNumCols;dwXPos++)
	{
		// reset flag
		BOOL bFlag=FALSE;

		// check each row
		for(dwYPos=0;dwYPos<g_dwNumRows;dwYPos++)
			if(!bFlag)
			{
				// set vertical stick
				if(GetColVStick(dwField,dwXPos,(g_dwNumRows-1)-dwYPos))
				{
					// set flag
					bFlag=TRUE;

					// set value
					SetColVStick(dwField,dwXPos,(g_dwNumRows-1)-dwYPos);
				}
			}
			else
			{
				// reset value
				ResetColVStick(dwField,dwXPos,(g_dwNumRows-1)-dwYPos);
			}
	}
}

// eof //
