//	keyboard.cpp

#include	<stdio.h>
#include	<string.h>
#include	<portmidi.h>
#include	"keyboard.h"

enum {
	KEYBOARD_DEV_ID		= 0,
	KEYBOARD_MAX_EVENTS = 256,
};


static PortMidiStream* midi_stream = NULL;

int find_device_id()
{
    const PmDeviceInfo *di;
    for(int i= 0; ; i++)
    {
        di= Pm_GetDeviceInfo(i);
        if(!di) break;
        if(strstr(di->name, "nanoKONTROL") && di->input)
        {
            printf("found device '%s' with interf '%s'\n", di->name, di->interf);
            return i;
        }
    }
    return -1;
}

int keyboard_init(void) {

	// open midi device
	Pm_Initialize();
    
	//~ const PmDeviceInfo* dev_info = Pm_GetDeviceInfo(KEYBOARD_DEV_ID);
    int devid= find_device_id();
	const PmDeviceInfo* dev_info = Pm_GetDeviceInfo(devid);

	if(dev_info) {
		Pm_OpenInput(&midi_stream, devid, NULL, KEYBOARD_MAX_EVENTS, NULL, NULL);
		return 1;
	}
	return 0;
}

void keyboard_kill(void) {
	if(midi_stream) {
		Pm_Close(midi_stream);
		midi_stream = NULL;
	}
	Pm_Terminate();
}


int keyboard_poll(KeyboardEvent* e) {

	if(!midi_stream) return 0;

	static PmEvent events[KEYBOARD_MAX_EVENTS];
	static int pos = 0;
	static int len = 0;

	if(pos == len) {
		pos = 0;
		len = Pm_Read(midi_stream, events, KEYBOARD_MAX_EVENTS);
		if(!len) return 0;
	}


	*e = *(KeyboardEvent*) &events[pos++].message;

	return 1;
}


