# MIDIText
Reads keyboard input and sends scancodes as MIDI messages over virtual MIDI port for use in Reaktor on Windows.

Usage:
-Install LoopMIDI and create MIDI port named "MIDIText"
-Compile "MIDIText.cpp". Set linker to link with "winmm.lib". Execute the compiled file. 
-Open Reaktor, deactivate MIDI keyboard in Options Menu, Activate the LoopMIDI Midiport input called "MIDIText" in Reaktors Audio and MIDI Settings.
-While having Reaktor as active window type some keys and obtain scancodes in Reaktor.
