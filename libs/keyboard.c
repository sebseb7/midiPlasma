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
static PortMidiStream* midi_stream_out = NULL;

int find_device_id_in()
{
    const PmDeviceInfo *di;
    for(int i= 0; ; i++)
    {
        di= Pm_GetDeviceInfo(i);
        if(!di) break;
        if(strstr(di->name, "nanoKONTROL") && di->input)
        {
            printf("found in device '%s' with interf '%s'\n", di->name, di->interf);
            return i;
        }
    }
    return -1;
}

int find_device_id_out()
{
    const PmDeviceInfo *di;
    for(int i= 0; ; i++)
    {
        di= Pm_GetDeviceInfo(i);
        if(!di) break;
        if(strstr(di->name, "nanoKONTROL") && di->output)
        {
            printf("found out device '%s' with interf '%s'\n", di->name, di->interf);
            return i;
        }
    }
    return -1;
}

int keyboard_init(void) {

	// open midi device
	Pm_Initialize();
    
	//~ const PmDeviceInfo* dev_info = Pm_GetDeviceInfo(KEYBOARD_DEV_ID);
    int devid= find_device_id_in();
    int devid_out= find_device_id_out();
	const PmDeviceInfo* dev_info = Pm_GetDeviceInfo(devid);
	const PmDeviceInfo* dev_info_out = Pm_GetDeviceInfo(devid_out);

	if(dev_info_out) {
		Pm_OpenOutput(&midi_stream_out, devid_out, NULL, KEYBOARD_MAX_EVENTS, NULL, NULL,0);
	}
	if(dev_info) {
		Pm_OpenInput(&midi_stream, devid, NULL, KEYBOARD_MAX_EVENTS, NULL, NULL);
		return 1;
	}
	return 0;
}

void keyboard_kill(void) {
	if(midi_stream_out) {
		Pm_Close(midi_stream_out);
		midi_stream_out = NULL;
	}
	if(midi_stream) {
		Pm_Close(midi_stream);
		midi_stream = NULL;
	}
	Pm_Terminate();
}

int keyboard_send(uint8_t a,uint8_t b , uint8_t c) {

	if(!midi_stream_out) return 0;

	static PmEvent events[KEYBOARD_MAX_EVENTS];

	events[0].timestamp =  0;
	events[0].message = Pm_Message( a,b,c );

	int i = Pm_Write(midi_stream_out, events, 1);

	return i;
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



