// MIDIText.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <mmsystem.h>  /* multimedia functions (such as MIDI) for Windows */

// Variable to store the handle to the hook. 
HHOOK _hook;

// This struct contains the data received by the hook callback.
KBDLLHOOKSTRUCT kbdStruct;

// MIDI device interface for sending MIDI output
HMIDIOUT device;    
MIDIOUTCAPSW devInfos;

// Variable for retreiving active window handle
HWND hwnd;
TCHAR szWindowText[100]; // variable for window title

// Midi Message
union { unsigned long word; unsigned char data[3]; } message;

// Event that is raised when a key is pressed.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{					
		// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
		kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

		// get window title
		hwnd = GetForegroundWindow();
		GetWindowText(hwnd, szWindowText, 100);

		if (wcsstr(szWindowText, _T("Reaktor"))) 
		{
			message.data[0] = 0x90;
			message.data[1] = kbdStruct.vkCode & 0x7f;			
			message.data[2] = wParam & 0x7f;
			midiOutShortMsg(device, message.word);

			message.data[0] = 0x80;
			message.data[1] = ((kbdStruct.vkCode) >> 7) & 0x7f;
			message.data[2] = kbdStruct.scanCode & 0x7f;
			midiOutShortMsg(device, message.word);
		}
	}

	// call the next hook
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	// Set the hook and set it to use the callback function above
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, _T("Failed to install hook!"), _T("Error"), MB_ICONERROR);
	}
}

void ReleaseHook()
{
	// turn any MIDI notes currently playing:
	midiOutReset(device);

	// Remove any data in MIDI device and close the MIDI Output port
	midiOutClose(device);
	UnhookWindowsHookEx(_hook);
}

int main()
{
	// Set the hook
	SetHook();

	// Get number of devices
	unsigned int numDev = midiOutGetNumDevs();

	// Select Mididevice
	unsigned int midiPort;
	for (midiPort = 0; midiPort < numDev; midiPort++)
	{
		midiOutGetDevCapsW(midiPort, &devInfos,	36+2*MAXPNAMELEN);
		if (!(wcscmp(devInfos.szPname, _T("MIDIText"))))
			break;
	}

	// Print Device Names of Port
	printf("Connected to MIDI output port:\n");
	printf("%ws\n", devInfos.szPname);
	
	// Open the MIDI output port
	midiOutOpen(&device, midiPort, 0, 0, CALLBACK_NULL);
	
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0));
	
	return 0;
}


