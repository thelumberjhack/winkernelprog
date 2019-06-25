#include <windows.h>
#include <stdio.h>

#include "..\PriorityBooster\PriorityBoosterCommon.h"

int Error(const char* message) {
	printf("%s (error=%d)\n", message, GetLastError());
	return 1;
}

int main(int argc, const char** argv) {
	if (argc < 3) {
		printf("Usage: Booster <threadid> <priority>\n");
		return 0;
	}

	// Get a handle to the device
	HANDLE hDevice = CreateFile(L"\\\\.\\PriorityBooster", GENERIC_WRITE,
		FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

	if (hDevice == INVALID_HANDLE_VALUE) {
		return Error("Failed to open device.");
	}

	ThreadData data;
	data.ThreadId = atoi(argv[1]);
	data.Priority = atoi(argv[2]);

	// Ready to call DeviceIoControl
	DWORD returned;
	BOOL success = DeviceIoControl(
		hDevice,
		IOCTL_PRIORITY_BOOSTER_SET_PRIORITY,		// Control code
		&data, sizeof(data),						// input buffer and length
		nullptr, 0,									// output ...
		&returned, 0
	);

	if (success) {
		printf("Priority changed successfully");
	}

	else {
		Error("Priority change failed!");
	}

	CloseHandle(hDevice);
}