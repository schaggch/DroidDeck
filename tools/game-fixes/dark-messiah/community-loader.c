typedef void *HANDLE;
typedef HANDLE HINSTANCE;
typedef HANDLE HMODULE;
typedef void *FARPROC;
typedef unsigned int UINT;

#define WINAPI __attribute__((stdcall))
#define CDECL __attribute__((cdecl))
#define DLLIMPORT __attribute__((dllimport))

DLLIMPORT HMODULE WINAPI LoadLibraryA(const char *name);
DLLIMPORT FARPROC WINAPI GetProcAddress(HMODULE module, const char *name);
DLLIMPORT HMODULE WINAPI GetModuleHandleA(const char *name);
DLLIMPORT char *WINAPI GetCommandLineA(void);
DLLIMPORT void WINAPI ExitProcess(UINT code);
DLLIMPORT int WINAPI MessageBoxA(HANDLE window, const char *text, const char *title, UINT type);

typedef int (CDECL *LauncherMain)(HINSTANCE instance, HINSTANCE previous, char *command_line, int show);

static char *arguments_after_program(char *line) {
    int quoted = 0;
    while (*line == ' ' || *line == '\t') line++;
    if (*line == '"') {
        quoted = 1;
        line++;
    }
    while (*line && ((quoted && *line != '"') || (!quoted && *line != ' ' && *line != '\t'))) line++;
    if (quoted && *line == '"') line++;
    while (*line == ' ' || *line == '\t') line++;
    return line;
}

void CDECL mainCRTStartup(void) {
    HMODULE launcher = LoadLibraryA("bin\\wos_dmmm.dll");
    if (!launcher) {
        MessageBoxA(0, "bin\\wos_dmmm.dll could not be loaded.", "Dark Messiah Community Loader", 0x10);
        ExitProcess(2);
    }

    LauncherMain start = (LauncherMain)GetProcAddress(launcher, "LauncherMain");
    if (!start) {
        MessageBoxA(0, "LauncherMain is missing from bin\\wos_dmmm.dll.", "Dark Messiah Community Loader", 0x10);
        ExitProcess(3);
    }

    int result = start(GetModuleHandleA(0), 0, arguments_after_program(GetCommandLineA()), 10);
    ExitProcess((UINT)result);
}
