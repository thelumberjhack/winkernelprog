#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	KdPrint(("Sample driver Unload called\n"));
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = SampleUnload;

	KdPrint(("Sample driver successfully initialized\n"));

	// Exercise chapter 2 - Output the Windows OS version: major, minor and build number
	RTL_OSVERSIONINFOW info;
	info.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

	RtlGetVersion(&info);
	KdPrint(("%d.%d.%d\n", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber));

	return STATUS_SUCCESS;
}