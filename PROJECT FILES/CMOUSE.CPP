/*

	cmouse.cpp
	4-2-90
	Microsoft mouse interrupt functions

	Copyright 1991
	John W. Small
	All rights reserved

	PSW / Power SoftWare
	P.O. Box 10072
	McLean, Virginia 22102 8072
	(703) 759-3838


	Works consulted:

	"Microsoft Mouse Programmer's Reference."
		Bellevue, Washington: Microsoft Press, 1989.

	"Microsoft Mouse Programmer's Reference Guide."
		Bellevue, Washington: Microsoft Press, 1986.

	Refer to above works for mouse function
	documentation.

*/


#include <dos.h>	/* union REGS, struct SREGS, */
			/*  int86(), int86x(), */
			/* geninterrupt(), enable() */
#include <cmouse.hpp>

MicrosoftMouse MM;	// Only instance allowed!

unsigned MicrosoftMouse::Xcell[] = {
	16,16,8,8,2,2,1,8,1,1,1,1,1,2,1,1,1,1,1,2};
unsigned MicrosoftMouse::Ycell[] = {
	8,8,8,8,1,1,1,8,1,1,1,1,1,1,1,1,1,1,1,1};
unsigned MicrosoftMouse::LeftTopOfs[] = {
	1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0};

MicrosoftMouse::~MicrosoftMouse()
{
	on();
	restore();
	clearAltInterrupts();
	reset();
	state = origState;
	restore();
}

/* Stack overflow check overwrites AX : do not compile with -N */
/* Do not call!  Use to cookbook your own if needed. */
#pragma argsused
void interrupt MMautoEventHandler(
	unsigned iBP,
	unsigned vertMickeys,    /* di */
	unsigned horzMickeys,    /* si */
	unsigned iDS,
	unsigned iES,
	unsigned y,              /* dx */
	unsigned x,              /* cx */
	unsigned buttonState,    /* bx */
	unsigned conditionMask,  /* ax */
	unsigned iIP,
	unsigned iCS,
	unsigned pseudoFlags)     /* not present */
{
	MM.eventTime = (*(long far *)MK_FP(0x0040,0x006C));
	enable();

	MM.eventCount++;
	MM.eventFlags = conditionMask;
	MM.leftPressed = (buttonState & MBleft)? 1 : 0;
	MM.rightPressed = (buttonState & MBright)? 1 : 0;
	MM.x = MM.physicalX(x);
	MM.y = MM.physicalY(y);
	MM.horzMickeys = horzMickeys;
	MM.vertMickeys = vertMickeys;

	if (conditionMask & MEmoved)
		MM.eventMoved++;
	if (conditionMask & MEleftPressed)  {
		MM.leftPressed = 1;
		MM.lastLeftPressX = MM.x;
		MM.lastLeftPressY = MM.y;
		MM.leftPresses++;
		MM.buttonRequested = MBleft;
		if ((MM.leftClickTime > MM.eventTime) ||
			((MM.eventTime - MM.leftClickTime)
			> MM.clickTimeOut))  {
			MM.leftClickTime = MM.eventTime;
			MM.leftClicks = 0;
		}
	}
	if (conditionMask & MEleftReleased)  {
		MM.leftPressed = 0;
		MM.lastLeftReleaseX = MM.x;
		MM.lastLeftReleaseY = MM.y;
		MM.leftReleases++;
		MM.buttonRequested = MBleft;
		if ((MM.leftClickTime > MM.eventTime) ||
			((MM.eventTime - MM.leftClickTime)
			> MM.clickTimeOut))  {
			MM.leftClickTime = MM.eventTime;
			MM.leftClicks = 0;
		}
		else
			MM.leftClicks++;
	}
	if (conditionMask & MErightPressed)  {
		MM.rightPressed = 1;
		MM.lastRightPressX = MM.x;
		MM.lastRightPressY = MM.y;
		MM.rightPresses++;
		MM.buttonRequested = MBright;
		if ((MM.rightClickTime > MM.eventTime) ||
			((MM.eventTime - MM.rightClickTime)
			> MM.clickTimeOut))  {
			MM.rightClickTime = MM.eventTime;
			MM.rightClicks = 0;
		}
	}
	if (conditionMask & MErightReleased)  {
		MM.rightPressed = 0;
		MM.lastRightReleaseX = MM.x;
		MM.lastRightReleaseY = MM.y;
		MM.rightReleases++;
		MM.buttonRequested = MBright;
		if ((MM.rightClickTime > MM.eventTime) ||
			((MM.eventTime - MM.rightClickTime)
			> MM.clickTimeOut))  {
			MM.rightClickTime = MM.eventTime;
			MM.rightClicks = 0;
		}
		else
			MM.rightClicks++;
	}

/*
	Convert from Interrupt stack frame to FAR call stack
	frame then return from FAR.  Since the mouse driver
	FAR calls this interrupt handler instead of issuing
	an interrupt call to get here, this event handler
	needs to return from a FAR call instead of returning
	from an interrupt!  A Turbo C++	interrupt function
	was used since it automatically restores DS to the
	data segment.  The mouse driver also passes
	parameters in registers instead of on the
	stack.  TC++'s interrupt keyword forces the function
	to automatically stack these registers so that they
	can be accessed.
*/

	asm mov ax,iDS       /* restore DS */
	asm mov ds,ax

	conditionMask = iBP; /* skip over parameters */
	asm add bp,OFFSET conditionMask

	asm mov sp,bp        /* exit far proc */
	asm pop bp
	asm retf
}

MicrosoftMouse::MicrosoftMouse()
{
	unsigned char far *vec;

	vmode = 0;
	present = 0;
	buttons = 0;
	leftPressed = 0; rightPressed = 0;
	x = 0; y = 0;
	lastLeftPressX = 0; lastLeftPressY = 0;
	leftPresses = 0;
	buttonRequested = MBleft;
	lastLeftReleaseX = 0; lastLeftReleaseY = 0;
	leftReleases = 0;
	lastRightPressX = 0; lastRightPressY = 0;
	rightPresses = 0;
	lastRightReleaseX = 0; lastRightReleaseY = 0;
	rightReleases = 0;
	vertMickeys= 0; horzMickeys = 0;
	handler = (MouseHandler) 0;
	callMask = 0;
	eventMask = 0x001F;
	eventFlags = 0;
	eventCount = 0;
	eventMoved = 0;
	eventTime = 0;
	clickTimeOut = 10;
	leftClickTime = 0;
	leftClicks = 0;
	rightClickTime = 0;
	rightClicks = 0;
	origState = (void *) 0;
	state = (void *) 0;
	stateSize = 0;
	altHandler = (MouseHandler) 0;
	altCallMask = 0;
	horzPercent = 50; vertPercent = 50;
	doublePercent = 50;
	crtPage = 0;
	mouseIntrVector = (MouseDriver) 0;
	language = MLenglish;
	driverVersion = 0;
	IRQ = -1;
	typeRequired = MTunknown;
	vec = (unsigned char far *) getvect(MOUSE_INT);
	if (!vec || (*vec == 0xCF)) /* IRET */
		return;
	reset();
	driver();
	if (!present)
		return;
	save();
	origState = state;
	state = (void *) 0;
}

void MicrosoftMouse::reset()			/* MF0 */
{
	_AH = 0x0F;
	geninterrupt(0x10);
	vmode = _AL;
	_AX = 0;
	geninterrupt(MOUSE_INT);
	if (_AX) {
		buttons = _BX;
		present = 1;
	}
	else
		buttons = present = 0;
}

void MicrosoftMouse::updateStatusInfo()		/* MF3 */
{
	unsigned buttons;

	_AX = 3;
	geninterrupt(MOUSE_INT);
	buttons = _BX;
	x = _CX;
	y = _DX;
	leftPressed = (buttons & MBleft)? 1 : 0;
	rightPressed = (buttons & MBright)? 1 : 0;
	x = physicalX(x);
	y = physicalY(y);
}

void MicrosoftMouse::gotoxy()			/* MF4 */
{
	unsigned vx, vy;

	vx = virtualX(x);
	vy = virtualY(y);
	_CX = vx;
	_DX = vy;
	_AX = 4;
	geninterrupt(MOUSE_INT);
}

void MicrosoftMouse::updatePressInfo()		/* MF5 */
{
	union REGS rgs;

	rgs.x.ax = 5;
	rgs.x.bx = buttonRequested >> 1;
	(void) int86(MOUSE_INT,&rgs,&rgs);
	leftPressed = (rgs.x.ax & MBleft)? 1 : 0;
	rightPressed = (rgs.x.ax & MBright)? 1 : 0;
	if (buttonRequested == MBleft)  {
		leftPresses = rgs.x.bx;
		lastLeftPressX = physicalX(rgs.x.cx);
		lastLeftPressY = physicalY(rgs.x.dx);
	}
	else {
		rightPresses = rgs.x.bx;
		lastRightPressX = physicalX(rgs.x.cx);
		lastRightPressY = physicalY(rgs.x.dx);
	}
}

void MicrosoftMouse::updateReleaseInfo()	/* MF6 */
{
	union REGS rgs;

	rgs.x.ax = 6;
	rgs.x.bx = buttonRequested >> 1;
	(void) int86(MOUSE_INT,&rgs,&rgs);
	leftPressed = (rgs.x.ax & MBleft)? 1 : 0;
	rightPressed = (rgs.x.ax & MBright)? 1 : 0;
	if (buttonRequested == MBleft)  {
		leftReleases = rgs.x.bx;
		lastLeftReleaseX = physicalX(rgs.x.cx);
		lastLeftReleaseY = physicalY(rgs.x.dx);
	}
	else {
		rightReleases = rgs.x.bx;
		lastRightReleaseX = physicalX(rgs.x.cx);
		lastRightReleaseY = physicalY(rgs.x.dx);
	}
}

void MicrosoftMouse::trap(unsigned x1, unsigned y1,
	unsigned x2, unsigned y2)
{						/* MF7, MF8 */
	unsigned d1, d2;

	d1 = virtualX(x1);
	d2 = virtualX(x2);
	_CX = d1;
	_DX = d2;
	_AX = 7;
	geninterrupt(MOUSE_INT);
	d1 = virtualY(y1);
	d2 = virtualY(y2);
	_CX = d1;
	_DX = d2;
	_AX = 8;
	geninterrupt(MOUSE_INT);
}

void MicrosoftMouse::graphicsCursor(signed char horzHS,
	signed char vertHS,
	unsigned far *scrCurMask)               /* MF9 */
{
	unsigned h, v;

	h = ((unsigned) horzHS) * Xcell[vmode];
	v = ((unsigned) vertHS) * Ycell[vmode];
	_ES = FP_SEG(scrCurMask);
	_DX = FP_OFF(scrCurMask);
	_BX = h;
	_CX = v;
	_AX = 9;
	geninterrupt(MOUSE_INT);
}

void MicrosoftMouse::swapInterrupts()		/* MF20 */
{
	union REGS rgs;
	struct SREGS srgs;

	rgs.x.ax = 20;
	rgs.x.cx = callMask;
	rgs.x.dx = FP_OFF(handler);
	srgs.es = FP_SEG(handler);
	(void) int86x(MOUSE_INT,&rgs,&rgs,&srgs);
	callMask = rgs.x.cx;
	handler = (MouseHandler) MK_FP(srgs.es,rgs.x.dx);
}

void MicrosoftMouse::autoEventUpdate()		/* N/A  */
{
	handler = (MouseHandler) MMautoEventHandler;
	callMask = eventMask;
	swapInterrupts();
}

void MicrosoftMouse::save()			/* MF21, MF22 */
{
	union REGS rgs;
	struct SREGS srgs;

	if (state) return;
	_AX = 21;
	geninterrupt(MOUSE_INT);
	stateSize = _BX;
	if ((state = (void *) new char [stateSize])
		== (void *) 0) return;
	srgs.es = FP_SEG((void far *)state);
	rgs.x.dx = FP_OFF((void far *)state);
	rgs.x.ax = 22;
	(void) int86x(MOUSE_INT,&rgs,&rgs,&srgs);
}

void MicrosoftMouse::restore()			/* MF23 */
{
	union REGS rgs;
	struct SREGS srgs;

	if(!state) return;
	srgs.es = FP_SEG((void far *)state);
	rgs.x.dx = FP_OFF((void far *)state);
	rgs.x.ax = 23;
	(void) int86x(MOUSE_INT,&rgs,&rgs,&srgs);
	delete state;
	state = (void *) 0;
}

void MicrosoftMouse::clearAltInterrupts()	/* N/A  */
{
	altHandler = (MouseHandler) 0;
	altCallMask = MEshiftPressed
		| MEctrlPressed | MEaltPressed;
	setAltInterrupt();
	altCallMask = MEshiftPressed | MEctrlPressed;
	setAltInterrupt();
	altCallMask = MEshiftPressed | MEaltPressed;
	setAltInterrupt();
	altCallMask = MEctrlPressed | MEaltPressed;
	setAltInterrupt();
	altCallMask = MEshiftPressed;
	setAltInterrupt();
	altCallMask = MEctrlPressed;
	setAltInterrupt();
	altCallMask = MEaltPressed;
	setAltInterrupt();
}

void MicrosoftMouse::off()			/* MF31 */
{
	union REGS rgs;
	struct SREGS srgs;

	if (mouseIntrVector) return;
	rgs.x.ax = 31;
	(void) int86x(MOUSE_INT,&rgs,&rgs,&srgs);
	if (rgs.x.ax == 0xFFFF) return;
	mouseIntrVector = getvect(MOUSE_INT);
	setvect(MOUSE_INT, (MouseDriver)
		MK_FP(srgs.es,rgs.x.bx));
}

void MicrosoftMouse::on()			/* MF32 */
{
	if (!mouseIntrVector) return;
	setvect(MOUSE_INT, mouseIntrVector);
	mouseIntrVector = (MouseDriver) 0;
	_AX = 32;
	geninterrupt(MOUSE_INT);
}

void MicrosoftMouse::softReset()		/* MF33 */
{
	_AX = 0x0F00;
	geninterrupt(0x10);
	vmode = _AL;
	_AX = 33;
	geninterrupt(MOUSE_INT);
	present = ((_AX == 0xFFFF) && (_BX == 2))? 1 : 0;
}

void MicrosoftMouse::driver()			/* MF36 */
{
	union REGS rgs;

	rgs.x.ax = 36;
	(void) int86(MOUSE_INT,&rgs,&rgs);
	driverVersion = rgs.x.bx;
	IRQ = rgs.h.cl;
	typeRequired = rgs.h.ch;
}


// To use cmouse in your application
// comment out //#define TEST_CMOUSE_CPP
#define TEST_CMOUSE_CPP
#ifdef TEST_CMOUSE_CPP

#include <conio.h>	/* clrscr(), gotoxy(), cprintf() */
#include <stdlib.h>
#include <graphics.h>

char *mice[] = {
	"Unknown", "Bus", "Serial",
	"InPort", "PS2", "HP"
};

void MouseReport()
{
	(void) cprintf("\r\nMouse Driver Version : %d.%d%d",
		MM.driverVersion >> 8,
		(MM.driverVersion & 0x00F0) >> 4,
		(MM.driverVersion & 0x000F));
	(void) cprintf("\r\nRequires : %s Mouse",mice[MM.typeRequired]);
	if (MM.present)
		(void) cprintf("\r\nMouse has %2d buttons.",MM.buttons);
	else
		(void) cprintf("\r\nMouse not connected!");
	(void) cprintf("\r\n\nMouse cursor @ (%3d,%3d)",MM.x,MM.y);
	(void) cprintf("    Left  : %s",MM.leftPressed? "Down":"Up  ");
	(void) cprintf("    Right : %s",MM.rightPressed? "Down":"Up  ");
	(void) cprintf("\r\n%s button requested",
		(MM.buttonRequested == MBleft)?
		"Left " : "Right");
	(void) cprintf("\r\nLeft:   Presses  : %3d  @ (%3d,%3d)   ",
		MM.leftPresses, MM.lastLeftPressX,
		MM.lastLeftPressY);
	(void) cprintf("Releases : %3d  @ (%3d,%3d)",
		MM.leftReleases, MM.lastLeftReleaseX,
		MM.lastLeftReleaseY);
	(void) cprintf("\r\nRight:  Presses  : %3d  @ (%3d,%3d)   ",
		MM.rightPresses, MM.lastRightPressX,
		MM.lastRightPressY);
	(void) cprintf("Releases : %3d  @ (%3d,%3d)",
		MM.rightReleases, MM.lastRightReleaseX,
		MM.lastRightReleaseY);
	(void) cprintf("\r\nMickeys :  Horz  : %10u   Vert  : %10u",
		MM.horzMickeys, MM.vertMickeys);
	(void) cprintf("\r\nEvent   :  Count : %10u   Time  : %10lu   Flags : %10u",
		MM.eventCount, MM.eventTime, MM.eventFlags);
	(void) cprintf("\r\nClicks  :  Left  : %10u   Right : %10u",
		MM.leftClicks, MM.rightClicks);
	(void) cprintf("\r\n@ time  :  Left  : %10lu   Right : %10lu",
		MM.leftClickTime, MM.rightClickTime);
}


#endif
