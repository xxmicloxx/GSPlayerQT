/*
	BASS custom looping example
	Copyright (c) 2004-2012 Un4seen Developments Ltd.
*/

#include <Carbon/Carbon.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include "bass.h"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#define WIDTH 600	// display width
#define HEIGHT 201	// height (odd number for centre line)

#pragma pack(1)
typedef struct {
	BYTE rgbRed,rgbGreen,rgbBlue,Aplha;
} RGBQUAD;
#pragma pack()

WindowPtr win=NULL;
pthread_t scanthread=0;
BOOL killscan=FALSE;

DWORD chan;
DWORD bpp;			// bytes per pixel
QWORD loop[2]={0};	// loop start & end
HSYNC lsync;		// looping sync

CGContextRef wavedc=0;
DWORD wavebuf[WIDTH*HEIGHT];
DWORD palette[HEIGHT/2+1];

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


void CALLBACK LoopSyncProc(HSYNC handle,DWORD channel,DWORD data,void *user)
{
	if (!BASS_ChannelSetPosition(channel,loop[0],BASS_POS_BYTE)) // try seeking to loop start
		BASS_ChannelSetPosition(channel,0,BASS_POS_BYTE); // failed, go to start of file instead
}

void SetLoopStart(QWORD pos)
{
	loop[0]=pos;
}

void SetLoopEnd(QWORD pos)
{
	loop[1]=pos;
	BASS_ChannelRemoveSync(chan,lsync); // remove old sync
	lsync=BASS_ChannelSetSync(chan,BASS_SYNC_POS|BASS_SYNC_MIXTIME,loop[1],LoopSyncProc,0); // set new sync
}

// scan the peaks
void *ScanPeaks(DWORD decoder)
{
	DWORD cpos=0,peak[2]={0};
	while (!killscan) {
		DWORD level=BASS_ChannelGetLevel(decoder); // scan peaks
		DWORD pos;
		if (peak[0]<LOWORD(level)) peak[0]=LOWORD(level); // set left peak
		if (peak[1]<HIWORD(level)) peak[1]=HIWORD(level); // set right peak
		if (!BASS_ChannelIsActive(decoder)) pos=-1; // reached the end
		else pos=BASS_ChannelGetPosition(decoder,BASS_POS_BYTE)/bpp;
		if (pos>cpos) {
			DWORD a;
			for (a=0;a<peak[0]*(HEIGHT/2)/32768;a++)
				wavebuf[(HEIGHT/2-1-a)*WIDTH+cpos]=palette[1+a]; // draw left peak
			for (a=0;a<peak[1]*(HEIGHT/2)/32768;a++)
				wavebuf[(HEIGHT/2+1+a)*WIDTH+cpos]=palette[1+a]; // draw right peak
			if (pos>=WIDTH) break; // gone off end of display
			cpos=pos;
			peak[0]=peak[1]=0;
		}
	}
	BASS_StreamFree(decoder); // free the decoder
	return NULL;
}

// select a file to play, and start scanning it
BOOL PlayFile()
{
	BOOL ret=FALSE;
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
				if (!(chan=BASS_StreamCreateFile(FALSE,file,0,0,0))
					&& !(chan=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_RAMP|BASS_MUSIC_POSRESET|BASS_MUSIC_PRESCAN,1))) {
					Error("Can't play file");
				} else {
					// create the bitmap
					CGColorSpaceRef colorSpace=CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
					wavedc=CGBitmapContextCreate(wavebuf,WIDTH,HEIGHT,8,WIDTH*4,colorSpace,kCGImageAlphaNoneSkipLast);
					CGColorSpaceRelease(colorSpace);
					{ // setup palette
						RGBQUAD *pal=(RGBQUAD*)palette;
						int a;
						memset(palette,0,sizeof(palette));
						for (a=1;a<=HEIGHT/2;a++) {
							pal[a].rgbRed=(255*a)/(HEIGHT/2);
							pal[a].rgbGreen=255-pal[a].rgbRed;
						}
					}
					bpp=BASS_ChannelGetLength(chan,BASS_POS_BYTE)/WIDTH; // bytes per pixel
					if (bpp<BASS_ChannelSeconds2Bytes(chan,0.02)) // minimum 20ms per pixel (BASS_ChannelGetLevel scans 20ms)
						bpp=BASS_ChannelSeconds2Bytes(chan,0.02);
					BASS_ChannelSetSync(chan,BASS_SYNC_END|BASS_SYNC_MIXTIME,0,LoopSyncProc,0); // set sync to loop at end
					BASS_ChannelPlay(chan,FALSE); // start playing
					{
						DWORD chan2=BASS_StreamCreateFile(FALSE,file,0,0,BASS_STREAM_DECODE);
						if (!chan2) chan2=BASS_MusicLoad(FALSE,file,0,0,BASS_MUSIC_DECODE,1);
						pthread_create(&scanthread,NULL,ScanPeaks,chan2); // start scanning peaks in a new thread
					}
					ret=TRUE;
				}
			}
			NavDisposeReply(&r);
		}
	}
	NavDialogDispose(fileDialog);
	return ret;
}

void DrawTimeLine(CGContextRef dc, QWORD pos, DWORD col, DWORD y)
{
	DWORD wpos=pos/bpp;
	DWORD time=BASS_ChannelBytes2Seconds(chan,pos);
	char text[10];
	sprintf(text,"%u:%02u",time/60,time%60);
	CGContextSetRGBStrokeColor(dc,(col&0xff)/255.f,((col>>8)&0xff)/255.f,((col>>16)&0xff)/255.f,1);
	CGContextSetRGBFillColor(dc,(col&0xff)/255.f,((col>>8)&0xff)/255.f,((col>>16)&0xff)/255.f,1);
	CGContextMoveToPoint(dc,wpos,0);
	CGContextAddLineToPoint(dc,wpos,HEIGHT);
	CGContextStrokePath(dc);
	CFStringRef cfs=CFStringCreateWithCString(0,text,0);
	Point p;
	short b;
	GetThemeTextDimensions(cfs,kThemeSystemFont,0,FALSE,&p,&b);
	Rect r;
	r.top=y;
	r.bottom=y+p.v;
	if (wpos>=WIDTH/2) {
		r.left=wpos-p.h;
		r.right=wpos;
	} else {
		r.left=wpos;
		r.right=wpos+p.h;
	}
	DrawThemeTextBox(cfs,kThemeSystemFont,0,FALSE,&r,0,dc);
	CFRelease(cfs);
}

static pascal OSStatus EventWindowDrawContent(EventHandlerCallRef callRef, EventRef event, void *userData)
{ // this stuff requires OSX 10.4
	CGContextRef cgc;
    QDBeginCGContext(GetWindowPort(win),&cgc);
	CGImageRef cgi=CGBitmapContextCreateImage(wavedc);
    CGRect cr=CGRectMake(0,0,WIDTH,HEIGHT);
	CGContextDrawImage(cgc,cr,cgi);
    CGImageRelease(cgi);
	DrawTimeLine(cgc,loop[0],0xffff00,12); // loop start
	DrawTimeLine(cgc,loop[1],0x00ffff,24); // loop end
	DrawTimeLine(cgc,BASS_ChannelGetPosition(chan,BASS_POS_BYTE),0xffffff,0); // current pos
    QDEndCGContext(GetWindowPort(win),&cgc);
	return noErr;
}

static pascal OSStatus EventWindowClick(EventHandlerCallRef callRef, EventRef event, void *userData)
{
	HIPoint p;
	GetEventParameter(event,kEventParamWindowMouseLocation,typeHIPoint,NULL,sizeof(p),NULL,&p);
	EventMouseButton b;
	GetEventParameter(event,kEventParamMouseButton,typeMouseButton,NULL,sizeof(b),NULL,&b);
	if (b==kEventMouseButtonPrimary) // set loop start
		SetLoopStart(p.x*bpp);
	else if (b==kEventMouseButtonSecondary) // set loop end
		SetLoopEnd(p.x*bpp);
	return noErr;
}

pascal void TimerProc(EventLoopTimerRef inTimer, void *inUserData)
{ // refresh window
	static const Rect r={0,0,HEIGHT,WIDTH};
	InvalWindowRect(win,&r);
}

int main(int argc, char* argv[])
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		Error("An incorrect version of BASS was loaded");
		return 0;
	}

	// initialize BASS
	if (!BASS_Init(-1,44100,0,win,NULL)) {
		Error("Can't initialize device");
		return 0;
	}
	if (!PlayFile()) { // start a file playing
		BASS_Free();
		return 0;
	}

	// create the window
	Rect wr;
	wr.left=200;
	wr.right=wr.left+WIDTH;
	wr.top=200;
	wr.bottom=wr.top+HEIGHT;
	CreateNewWindow(kMovableModalWindowClass,kWindowStandardHandlerAttribute,&wr,&win);
	SetWindowTitleWithCFString(win,CFSTR("BASS custom looping example (left-click to set loop start, right-click to set end)"));
	ChangeWindowAttributes(win,kWindowAsyncDragAttribute,kWindowNoAttributes);
	{
		EventTypeSpec etype={kEventClassWindow,kEventWindowDrawContent};
		InstallWindowEventHandler(win,NewEventHandlerUPP(EventWindowDrawContent),1,&etype,0,NULL);
		etype.eventKind=kEventWindowHandleContentClick;
		InstallWindowEventHandler(win,NewEventHandlerUPP(EventWindowClick),1,&etype,0,NULL);
	}

	ShowWindow(win);
	
	// setup update timer (10hz)
	EventLoopTimerRef timer;
	InstallEventLoopTimer(GetCurrentEventLoop(),kEventDurationNoWait,kEventDurationSecond/10,NewEventLoopTimerUPP(TimerProc),0,&timer);

	RunApplicationEventLoop();

	killscan=TRUE;
	pthread_join(scanthread,NULL); // wait for the scan thread
    CGContextRelease(wavedc);
	DisposeWindow(win);
	BASS_Free();
	return 0;	
}
