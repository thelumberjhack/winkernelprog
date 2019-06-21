#include <ntddk.h>

#define DRIVER_TAG 'dcba'

UNICODE_STRING g_RegistryPath;

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	ExFreePool(g_RegistryPath.Buffer);

	KdPrint(("Sample driver Unload called\n"));
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	DriverObject->DriverUnload = SampleUnload;

	g_RegistryPath.Buffer = (WCHAR*)ExAllocatePoolWithTag(PagedPool, RegistryPath->Length, DRIVER_TAG);

	if (g_RegistryPath.Buffer == nullptr) {
		KdPrint(("Failed to allocate memory\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	g_RegistryPath.MaximumLength = RegistryPath->Length;
	RtlCopyUnicodeString(&g_RegistryPath, (PUNICODE_STRING)RegistryPath);

	// %wZ if for UNICODE_STRING objects
	KdPrint(("Copied registry path: %wZ\n", &g_RegistryPath));

	KdPrint(("Sample driver successfully initialized\n"));

	// Exercise chapter 2 - Output the Windows OS version: major, minor and build number
	RTL_OSVERSIONINFOW info;
	info.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

	NTSTATUS status = RtlGetVersion(&info);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Error occured: 0x%08X\n", status));
		return status;
	}
	KdPrint(("%d.%d.%d\n", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber));

	return STATUS_SUCCESS;
}