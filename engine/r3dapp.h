
// r3dapp.h -- application procedures header

// prototypes //

// application info functions

extern LPCSTR ApplicationName(void);

extern LPCSTR ApplicationID(void);

extern LPCSTR ApplicationIcon(void);

extern LPCSTR ApplicationMenu(void);

extern LPCSTR ApplicationCursor(void);

extern DISPLAYMODE ApplicationDisplayMode(void);

extern LPCSTR ApplicationDataPath(void);

extern float ApplicationMasterVolume(void);

// application execution functions

extern BOOL ApplicationInit(void);

extern BOOL ApplicationLoop(void);

extern D3DCOLOR ApplicationPause(void);

extern void ApplicationResume(void);

extern void ApplicationRelease(void);

extern BOOL ApplicationToggleFullScreen(void);

// eof //

