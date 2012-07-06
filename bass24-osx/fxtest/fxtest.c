/*
	BASS "DX8" effects test
	Copyright (c) 2001-2012 Un4seen Developments Ltd.
*/

#include <Carbon/Carbon.h>
#include <stdio.h>
#include <math.h>
#include "bass.h"

WindowPtr win;

DWORD chan;			// channel handle
HFX fx[4];			// 3 eq bands + reverb

// display error messages
void Error(const char *es)
{
	short i;
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	CFStringRef ces=CFStringCreateWithCString(0,mes,0);
	DialogRef alert;
	CreateStandardAlert(0,CFSTR("Error"),ces,NULL,&alert);
	RunStandardAlert(alert,NULL,&i);
	CFRelease(ces);
}

ControlRef GetControl(int id)
{
	ControlRef cref;
	ControlID cid={0,id};
	GetControlByID(win,&cid,&cref);
	return cref;
}

void SetupControlHandler(int id, DWORD event, EventHandlerProcPtr proc)
{
	EventTypeSpec etype={kEventClassControl,event};
	ControlRef cref=GetControl(id);
	InstallControlEventHandler(cref,NewEventHandlerUPP(proc),1,&etype,cref,NULL);
}
    
void UpdateFX(int b)
{
	ControlRef cref=GetControl(20+b);
	int v=GetControl32BitValue(cref);
	if (b<3) {
		BASS_DX8_PARAMEQ p;
		BASS_FXGetParameters(fx[b],&p);
		p.fGain=v;
		BASS_FXSetParameters(fx[b],&p);
	} else {
		BASS_DX8_REVERB p;
		BASS_FXGetParameters(fx[b],&p);
		p.fReverbMix=(v?log(v/20.0)*20:-96);
		BASS_FXSetParameters(fx[b],&p);
	}
}

pascal void FXEventHandler(ControlHandle control, SInt16 part)
{
	ControlID cid;
	GetControlID(control,&cid);
	UpdateFX(cid.id-20);
}

pascal OSStatus OpenEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	NavDialogRef fileDialog;
	NavDialogCreationOptions fo;
	NavGetDefaultDialogCreationOptions(&fo);
	fo.optionFlags=0;
	fo.parentWindow=win;
	NavCreateChooseFileDialog(&fo,NULL,NULL,NULL,NULL,NULL,&fileDialog);
	if (!NavDialogRun(fileDialog)) {
		NavReplyRecord r;
		if (!NavDialogGetReply(fileDialog,&r)) {
			AEKeyword k;
			FSRef fr;
			if (!AEGetNthPtr(&r.selection,1,typeFSRef,&k,NULL,&fr,sizeof(fr),NULL)) {
				char file[256];
				FSRefMakePath(&fr,(BYTE*)file,sizeof(file));
				// free both MOD and stream, it must be one of them! :)
				BASS_MusicFree(chan);
				BASS_StreamFree(chan);
				if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,BASS_SAMPLE_LOOP|BASS_SAMPLE_FLOAT))
					&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMPS|BASS_SAMPLE_LOOP|BASS_SAMPLE_FLOAT,1))) {
					// whatever it is, it ain't playable
					SetControlTitleWithCFString(inUserData,CFSTR("click here to open a file..."));
					Error("Can't play the file");
				} else {
					CFStringRef cs=CFStringCreateWithCString(0,file,kCFStringEncodingUTF8);
					SetControlTitleWithCFString(inUserData,cs);
					CFRelease(cs);
					{ // setup the effects
						BASS_DX8_PARAMEQ p;
						fx[0]=BASS_ChannelSetFX(chan,BASS_FX_DX8_PARAMEQ,0);
						fx[1]=BASS_ChannelSetFX(chan,BASS_FX_DX8_PARAMEQ,0);
						fx[2]=BASS_ChannelSetFX(chan,BASS_FX_DX8_PARAMEQ,0);
						fx[3]=BASS_ChannelSetFX(chan,BASS_FX_DX8_REVERB,1);
						p.fGain=0;
						p.fBandwidth=18;
						p.fCenter=125;
						BASS_FXSetParameters(fx[0],&p);
						p.fCenter=1000;
						BASS_FXSetParameters(fx[1],&p);
						p.fCenter=8000;
						BASS_FXSetParameters(fx[2],&p);
						UpdateFX(0);
						UpdateFX(1);
						UpdateFX(2);
						UpdateFX(3);
					}
					BASS_ChannelPlay(chan,FALSE);
				}
			}
			NavDisposeReply(&r);
		}
	}
	NavDialogDispose(fileDialog);
    return noErr;
}

int main(int argc, char* argv[])
{
	IBNibRef 		nibRef;
	OSStatus		err;
    
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		Error("An incorrect version of BASS was loaded");
		return 0;
	}

	// initialize default device
	if (!BASS_Init(-1,44100,0,NULL,NULL)) {
		Error("Can't initialize device");
		return 0;
	}

	// Create Window and stuff
	err = CreateNibReference(CFSTR("fxtest"), &nibRef);
	if (err) return err;
	err = CreateWindowFromNib(nibRef, CFSTR("Window"), &win);
	if (err) return err;
	DisposeNibReference(nibRef);

	SetupControlHandler(10,kEventControlHit,OpenEventHandler);
	SetControlAction(GetControl(20),NewControlActionUPP(FXEventHandler));
	SetControlAction(GetControl(21),NewControlActionUPP(FXEventHandler));
	SetControlAction(GetControl(22),NewControlActionUPP(FXEventHandler));
	SetControlAction(GetControl(23),NewControlActionUPP(FXEventHandler));

	ShowWindow(win);
	RunApplicationEventLoop();

	BASS_Free();

    return 0; 
}
