#include <windows.h>
#include <stdio.h>
#include <stdint.h>

// Version definition
#define PAWNITEMS_VERSION "1.0.0"

// Configuration values
static char log_mode[32] = "truncate";
static FILE* g_log_file = NULL;

// Helper function to get module directory
static void get_module_directory(char* buffer, size_t buffer_size) {
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                      (LPCTSTR)get_module_directory, &hModule);
    GetModuleFileNameA(hModule, buffer, buffer_size);
    // Replace filename with directory
    char* last_backslash = strrchr(buffer, '\\');
    if (last_backslash) {
        *(last_backslash + 1) = '\0';
    }
}

// Helper function to read configuration
static void read_config(void) {
    char config_path[256];
    get_module_directory(config_path, sizeof(config_path));
    strcat_s(config_path, sizeof(config_path), "pawnitems.ini");

    // Read logging setting from ini file
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    GetPrivateProfileStringA("main", "logging", "truncate", buffer, sizeof(buffer), config_path);
    
    if (strlen(buffer) > 0) {
        strncpy_s(log_mode, sizeof(log_mode), buffer, sizeof(log_mode) - 1);
        log_mode[sizeof(log_mode) - 1] = '\0';
    }
}

// Helper function to open log file
static void open_log_file(void) {
    // Only open if logging is enabled
    if (strcmp(log_mode, "disabled") == 0) {
        return;
    }

    char log_path[256];
    get_module_directory(log_path, sizeof(log_path));
    strcat_s(log_path, sizeof(log_path), "pawnitems.log");

    const char* mode = (strcmp(log_mode, "append") == 0) ? "a" : "w";
    
    errno_t err = fopen_s(&g_log_file, log_path, mode);
    if (err != 0) {
        g_log_file = NULL;
    }
}

// Helper function to close log file
static void close_log_file(void) {
    if (g_log_file != NULL) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

// Helper function to write log
static void write_log(const char* message) {
    // Only log if logging is enabled
    if (strcmp(log_mode, "disabled") == 0) {
        return;
    }

    if (g_log_file != NULL) {
        fprintf(g_log_file, "%s\n", message);
        fflush(g_log_file);  // Ensure it's written immediately
    }
}

// DllMain - entry point for the DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Read configuration when the DLL is loaded
        read_config();
        
        // Open log file once at startup with appropriate mode
        open_log_file();
        
        // Write a log message when loaded
        write_log("pawnitems.dll loaded successfully");
        break;
        
    case DLL_PROCESS_DETACH:
        // Write message on unload and close the log
        write_log("pawnitems.dll unloaded");
        close_log_file();
        break;
    }
    return TRUE;
}