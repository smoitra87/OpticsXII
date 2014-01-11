/*

	cmouse.hpp
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

#ifndef CMOUSE_CPP
#define CMOUSE_CPP

typedef  void interrupt (*MouseDriver)(...);
typedef  void interrupt (*MouseHandler)(...);

#define MOUSE_INT 0x33

// mouse button designators
#define MBleft   1
#define MBright  2

// mouse events for callMask, altCallMask,
// eventMask, eventFlags
#define MEmoved		  1
#define MEleftPressed     2
#define MEleftReleased    4
#define MErightPressed    8
#define MErightReleased  16
#define MEshiftPressed   32
#define MEctrlPressed    64
#define MEaltPressed    128

// mouse types
#define MTunknown  0
#define MTbus      1
#define MTserial   2
#define MTinPort   3
#define MTps2      4
#define MThp       5

// mouse error message languages
#define MLenglish      0
#define MLfrench       1
#define MLdutch        2
#define MLgerman       3
#define MLswedish      4
#define MLfinnish      5
#define MLspanish      6
#define MLportuguese   7
#define MLitalian      8


// Do not instantiate the MicrosoftMouse class!
// Reference MM.??? instead!
// Write appropriate fields in MM and call member function
// or call function and read appropriate fields.
// Use physical screen coordinates for all values not
// the mouse virtual coordinates.  Conversion to/from
// mouse virtual coordinate is automatically handled
// by the class member functions.

class MicrosoftMouse  {

	static unsigned Xcell[];
	static unsigned Ycell[];
	static unsigned LeftTopOfs[];

	// MF0, MF33 : Video mode at reset/softReset
	// Call reset after all video mode changes!
	unsigned vmode;

	// MF21, MF22, MF23 : Used by save and restore
	void * origState, * state;
	unsigned stateSize;

	// MF31, MF32 : Used by off/on
	MouseDriver mouseIntrVector;

	// condOff virtual coordinates
	unsigned covx1, covy1, covx2, covy2;

public:
	// MF0 : Set by reset
	unsigned present;
	unsigned buttons;

	// MF3, MF5, MF6 : Button Status
	unsigned leftPressed, rightPressed;

	// MF3, MF4 : Mouse Positon
	unsigned x, y;
	// X & Y are physical, not virtual coordinates!
	// Text modes upper left corner : 1,1
	// Graphics modes upper left corner : 0,0

	// MF5 : Button Press Information
	unsigned lastLeftPressX, lastLeftPressY,
		leftPresses;
	unsigned lastRightPressX, lastRightPressY,
		rightPresses;
	// X & Y are physical, not virtual coordinates!

	// MF5, MF6 : Button Requested
	unsigned buttonRequested;

	// MF6 : Button Release Information
	unsigned lastLeftReleaseX, lastLeftReleaseY,
		leftReleases;
	unsigned lastRightReleaseX, lastRightReleaseY,
		rightReleases;
	// X & Y are physical, not virtual coordinates!

	// MF11 : Mouse motion counters
	unsigned vertMickeys, horzMickeys;

	// MF12, MF20 : set/swapInterrupt(s) mask/addr
	MouseHandler handler; // See MMautoEventHandler
	unsigned callMask;
	// Also used by autoEventUpdate.

	// Set by Mouse Event Handler.
	unsigned eventMask, eventFlags, eventCount,
		eventMoved;
	long eventTime;
	long clickTimeOut;
	long leftClickTime;
	unsigned leftClicks;
	long rightClickTime;
	unsigned rightClicks;

	// MF24, MF25 : set/getAltInterrupt mask/addr
	MouseHandler altHandler; // See MMautoEventHandler
	unsigned altCallMask;
	// MF25 can read those set by MF20!

	// MF26, MF27 : set/getSensitivity
	unsigned horzPercent, vertPercent, doublePercent;

	// MF29, MF30 : set/getCRTpage
	unsigned crtPage;

	// MF34, MF35 : Used by get/setLanguage
	unsigned language;

	// MF36 : Set by driver
	unsigned driverVersion;
	unsigned IRQ;
	unsigned typeRequired;


	MicrosoftMouse();
	~MicrosoftMouse();
	void reset();						/* MF0 */
	void show()  { _AX = 1; geninterrupt(MOUSE_INT); }	/* MF1 */
	void hide()  { _AX = 2; geninterrupt(MOUSE_INT); }      /* MF2 */
	void updateStatusInfo();				/* MF3 */
	void gotoxy();						/* MF4 */
	void updatePressInfo();					/* MF5 */
	void updateReleaseInfo();				/* MF6 */
	void trap(unsigned x1, unsigned y1,                /* MF7, MF8 */
		unsigned x2, unsigned y2);
	void graphicsCursor(signed char horzHS,			/* MF9 */
		signed char vertHS,
		unsigned far *scrCurMask);
	void softwareTextCursor(				/* MF10 */
		unsigned scrMask, unsigned curMask)
		{ _BX = 0; _CX = scrMask; _DX = curMask;
		_AX = 10; geninterrupt(MOUSE_INT); }
	void updateMotionInfo()					/* MF11 */
		{ _AX = 11; geninterrupt(MOUSE_INT);
		horzMickeys = _CX; vertMickeys = _DX; }
	void setInterrupt()  {					/* MF12 */
		_ES = FP_SEG(handler);
		_DX = FP_OFF(handler);
		_CX = callMask;
		_AX = 12;
		geninterrupt(MOUSE_INT);
	}
	void lightPenOn()                                       /* MF13 */
		{ _AX = 13; geninterrupt(MOUSE_INT); }
	void lightPenOff()					/* MF14 */
		{ _AX = 14; geninterrupt(MOUSE_INT); }
	void speed(unsigned horz, unsigned vert)                /* MF15 */
		{ _CX = horz; _DX = vert; _AX = 15;
		geninterrupt(MOUSE_INT); }
	void condOffZone(unsigned x1, unsigned y1,
		unsigned x2, unsigned y2)
	{
		covx1 = virtualX(x1);
		covy1 = virtualY(y1);
		covx2 = virtualX(x2);
		covy2 = virtualY(y2);
	}
	void condOff()                                          /* MF16 */
	{
		_DI = covy2;
		_SI = covx2;
		_DX = covy1;
		_CX = covx1;
		_AX = 16;
		geninterrupt(MOUSE_INT);
	}
	void doubleSpeedThreshold(unsigned mickeys)             /* MF19 */
		{ _DX = mickeys; _AX = 19;
		geninterrupt(MOUSE_INT); }
	void swapInterrupts();					/* MF20 */
	void autoEventUpdate();					/* N/A  */
	void save();					  /* MF21, MF22 */
	void restore();						/* MF23 */
	void setAltInterrupt()					/* MF24 */
	{
		_ES = FP_SEG(altHandler);
		_DX = FP_OFF(altHandler);
		_CX = altCallMask;
		_AX = 24;
		geninterrupt(MOUSE_INT);
	}
	void getAltInterrupt()					/* MF25 */
	{
		_CX = altCallMask;
		_AX = 25;
		geninterrupt(MOUSE_INT);
		altHandler = (MouseHandler) MK_FP(_BX,_DX);
	}
	void clearAltInterrupts();				/* N/A  */
	void setSensitivity()					/* MF26 */
		{ _DX = doublePercent; _CX = vertPercent;
		_BX = horzPercent; _AX = 26;
		geninterrupt(MOUSE_INT); }
	void getSensitivity()					/* MF27 */
		{ _AX = 27; geninterrupt(MOUSE_INT);
		horzPercent = _BX; vertPercent = _CX;
		doublePercent = _DX; }
	void setInterruptRate(unsigned rate)                    /* MF28 */
	{
		if (typeRequired != MTinPort) return;
		_BX = rate;
		_AX = 28;
		geninterrupt(MOUSE_INT);
	}
	void setCRTpage()					/* MF29 */
		{ _BX = crtPage; _AX = 29;
		geninterrupt(MOUSE_INT); }
	void getCRTpage()					/* MF30 */
		{ _AX = 30; geninterrupt(MOUSE_INT);
		crtPage = _BX; }
	void off();						/* MF31 */
	void on();						/* MF32 */
	void softReset();					/* MF33 */
	void setLanguage()					/* MF34 */
		{ _BX = language; _AX = 34;
		geninterrupt(MOUSE_INT); }
	void getLanguage()					/* MF35 */
		{ _AX = 35; geninterrupt(MOUSE_INT);
		language = _BX; }
	void driver();						/* MF36 */

	unsigned virtualX(unsigned x)  {
		return ((x - LeftTopOfs[vmode])
		* Xcell[vmode]); }
	unsigned virtualY(unsigned y)  {
		return ((y - LeftTopOfs[vmode])
		* Ycell[vmode]); }
	unsigned physicalX(unsigned x)  {
		return (x / Xcell[vmode]
		+ LeftTopOfs[vmode]); }
	unsigned physicalY(unsigned y)  {
		return (y / Ycell[vmode]
		+ LeftTopOfs[vmode]); }

};  // MicrosoftMouse
#endif

