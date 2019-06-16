/*
 * xdraw.c - Base for 1. project
 */

/*
 * Standard XToolkit and OSF/Motif include files.
 */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h> 

/*
 * Public include files for widgets used in this file.
 */
#include <Xm/MainW.h> 
#include <Xm/Form.h> 
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>

#include <Xm/MainW.h>
#include <Xm/ScrolledW.h>
#include <Xm/PanedW.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/CascadeBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <X11/Xmu/Editres.h>

/*
 * Common C library include files
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Shared variables
 */



int button_pressed = 0;		/* input state */


# define COLOR_COUNT 13

String		colorNames[COLOR_COUNT]
			= {"Black", "Brown", "Red", "Orange", "Yellow",
			"Green", "Blue", "Violet", "Dim Gray", "White",
			"Gold", "Silver","Lavender"};
XColor colors[COLOR_COUNT];

//XColor *ColorPtr[COLOR_COUNT];

int DIGIT1val = 1;
int DIGIT2val = 0;
int DIGIT3val = 0;
int MULTIPLIERval = 0;
int TOLERANCEval = 1;

XColor * backgroungColor= &colors[0];
XColor * lineColor 		= &colors[0];
XColor * resistorColor	= &colors[12];
XColor * digit1Color	= &colors[1];
XColor * digit2Color	= &colors[0];
XColor * digit3Color	= &colors[0];
XColor * multiplierColor= &colors[0];
XColor * toleranceColor	= &colors[1];



Widget	optionMenuDigit1,
		optionMenuDigit2,
		optionMenuDigit3,
		optionMenuMultiplier,
		optionMenuTolerance,
		label;


GC drawGC=0;
GC resistorGC[8];
Widget drawArea;
size_t drawAreaWidth;
size_t drawAreaHeight;



void Recalculate(){
	char result[20] = "";
	char m[] = {'\0','k','M','G'};
	
	char digit1Char = DIGIT1val + '0';
	char digit2Char = DIGIT2val + '0';
	char digit3Char = DIGIT3val + '0';
	
	
	//MULTIPLIERval
	switch (MULTIPLIERval){
		case 0://black
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = digit3Char;
			result[3] = '\0';
			break;
		case 1://brown
			result[0] = digit1Char;
			result[1] = '.';
			result[2] = digit2Char;
			result[3] = digit3Char;
			result[4] = m[1];
			result[5] = '\0';
			break;
		case 2://red
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = '.';
			result[3] = digit3Char;
			result[4] = m[1];
			result[5] = '\0';
			break;
		case 3://orange
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = digit3Char;
			result[3] = m[1];
			result[4] = '\0';
			break;
		case 4://yellow
			result[0] = digit1Char;
			result[1] = '.';
			result[2] = digit2Char;
			result[3] = digit3Char;
			result[4] = m[2];
			result[5] = '\0';
			break;
		case 5://green
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = '.';
			result[3] = digit3Char;
			result[4] = m[2];
			result[5] = '\0';
			break;
		case 6://blue
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = digit3Char;
			result[3] = m[2];
			result[4] = '\0';
			break;
		case 7://violet
			result[0] = digit1Char;
			result[1] = '.';
			result[2] = digit2Char;
			result[3] = digit3Char;
			result[4] = m[3];
			result[5] = '\0';
			break;
		case 8://gray
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = '.';
			result[3] = digit3Char;
			result[4] = m[3];
			result[5] = '\0';
			break;
		case 9://white
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = digit3Char;
			result[3] = m[3];
			result[4] = '\0';
			break;
		case 10://gold
			result[0] = digit1Char;
			result[1] = digit2Char;
			result[2] = '.';
			result[3] = digit3Char;
			result[4] = '\0';
			break;
		case 11://silver
			result[0] = digit1Char;
			result[1] = '.';
			result[2] = digit2Char;
			result[3] = digit3Char;
			result[4] = '\0';
			break;
		default:
			break;
	}
	
	
	
	strcat( result, " ohm" );
	
 //TOLERANCEval
	switch (TOLERANCEval){
		case 1://brown
			strcat( result, "\t+- 1%" );
			break;
		case 2://red
			strcat( result, "\t+- 2%" );
			break;
		case 3://orange
			strcat( result, "\t+- 3%" );
			break;
		case 4://yellow
			strcat( result, "\t+- 4%" );
			break;
		case 5://green
			strcat( result, "\t+- 0.5%" );
			break;
		case 6://blue
			strcat( result, "\t+- 0.25%" );
			break;
		case 7://violet
			strcat( result, "\t+- 0.1%" );
			break;
		case 8://gray
			strcat( result, "\t+- 0.05%" );
			break;
		case 10://gold
			strcat( result, "\t+- 5%" );
			break;
		case 11://silver
			strcat( result, "\t+- 10%" );
			break;
		default:
			break;
	}
	

	XmTextSetString (label, result);
}


void DrawRectangle(GC drawGC, Widget w,int x1, int y1, int x2 ,int y2,XColor* backgroung, XColor* lineColor){
	
	XSetForeground(XtDisplay(w), drawGC, backgroung->pixel);
	XFillRectangle(XtDisplay(w), XtWindow(w), drawGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
	XSetForeground(XtDisplay(w), drawGC, lineColor->pixel);
	XDrawRectangle(XtDisplay(w), XtWindow(w), drawGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
  
}
void DrawResistor(GC drawGC, Widget w,size_t width,size_t height){
	DrawRectangle(drawGC, w, 0, 0, width/3.8, height,resistorColor, resistorColor);
	DrawRectangle(drawGC, w, width - (width/3.8), 0, width, height,resistorColor, resistorColor);
	
	DrawRectangle(drawGC, w, 0, height/8, width, height*7/8,resistorColor, resistorColor);
	
	//bands
	size_t bandWidth = width/15;
	size_t bandDistance = width / 10;
	size_t bandX1=(width/3.2);
	size_t bandX2=bandX1+bandDistance;
	size_t bandX3=bandX2+bandDistance;
	
	//1
	DrawRectangle(drawGC, w, width/8, 0, width/8 + bandWidth, height,digit1Color, lineColor);
	
	//2
	DrawRectangle(drawGC, w, bandX1, height/8, bandX1 + bandWidth, height*7/8,digit2Color, lineColor);
	//3
	DrawRectangle(drawGC, w, bandX2, height/8, bandX2 + bandWidth, height*7/8,digit3Color, lineColor);
	//multiplier
	DrawRectangle(drawGC, w, bandX3, height/8, bandX3 + bandWidth, height*7/8,multiplierColor, lineColor);
	
	//tolerance
	DrawRectangle(drawGC, w, width - (width/8), 0, width-(width/8 + bandWidth), height,toleranceColor, lineColor);
	
}
void Draw(GC drawGC, Widget w){
	XtVaGetValues(drawArea, XmNwidth, &drawAreaWidth, NULL);
	XtVaGetValues(drawArea, XmNheight, &drawAreaHeight, NULL);
	DrawRectangle(drawGC, w, 0, 0, (int)(long)XmNwidth, (int)(long)XmNheight, backgroungColor, backgroungColor);
	
	size_t resistorSizeWidth;
	size_t resistorSizeHeight;
	float sizeScale=4;
	if(drawAreaWidth>drawAreaHeight*sizeScale){
		resistorSizeHeight=	drawAreaHeight;
		resistorSizeWidth=	drawAreaHeight*sizeScale;
	}
	else
	{
		resistorSizeWidth=	drawAreaWidth;
		resistorSizeHeight=	drawAreaWidth/sizeScale;
	}
	DrawResistor(drawGC, w, resistorSizeWidth,resistorSizeHeight);
}

/*
 * "Expose" callback function
 */
void ExposeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
	drawGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
	
	Draw(drawGC,w);
	Recalculate();
	
}
void ResizeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
	if(drawGC)
		Draw(drawGC,w);
}
    char *fallback_resources[] = {
		"*closeQuestion.dialogTitle: Question",
		"*closeQuestion.messageString: Quit?",
		"*closeQuestion.okLabelString: Yes",
		"*closeQuestion.cancelLabelString: No",
		"*closeQuestion.messageAlignment: XmALIGNMENT_CENTER",
		NULL
	};
/*
 * "Quit" button callback function
 */
Widget closeQuestion;
void QuitCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 

    XtManageChild(closeQuestion);
}
void closeQuestionCB(Widget w, XtPointer client_data, XtPointer call_data) {
	
	if ((long)client_data) {
		
		if (drawGC != NULL) {
			XFreeGC(XtDisplay(w), drawGC);
		}
	}
	exit(0);
}
int digit1Value = 0;

void ToleranceCB (Widget w, XtPointer client_data, XtPointer call_data)
{
	TOLERANCEval = (int)(long)client_data+1;
	if(TOLERANCEval>=9)
		TOLERANCEval++;
	toleranceColor= &colors[TOLERANCEval]; // Black
	

    XtVaSetValues (optionMenuTolerance, 	XtNforeground, toleranceColor->pixel, NULL);
	if(drawGC)
		Draw(drawGC,drawArea);
	Recalculate();
}
void MultiplierCB (Widget w, XtPointer client_data, XtPointer call_data)
{
	multiplierColor= &colors[(int)(long)client_data];
	
MULTIPLIERval=(int)(long)client_data;
    XtVaSetValues (optionMenuMultiplier, 	XtNforeground, multiplierColor->pixel, NULL);
	if(drawGC)
		Draw(drawGC,drawArea);
	Recalculate();
}
void Digit1CB (Widget w, XtPointer client_data, XtPointer call_data)
{
	DIGIT1val=(int)(long)client_data+1;
	digit1Color= &colors[(int)(long)client_data+1]; //No black
    XtVaSetValues (optionMenuDigit1, 	XtNforeground, digit1Color->pixel, NULL);
	if(drawGC)
		Draw(drawGC,drawArea);
	Recalculate();
}
void Digit2CB (Widget w, XtPointer client_data, XtPointer call_data)
{ 
	DIGIT2val=(int)(long)client_data;
	digit2Color= &colors[(int)(long)client_data];
    XtVaSetValues (optionMenuDigit2, 	XtNforeground, digit2Color->pixel, NULL);
	if(drawGC)
		Draw(drawGC,drawArea);
	Recalculate();
}
void Digit3CB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
	DIGIT3val=(int)(long)client_data;
	digit3Color= &colors[(int)(long)client_data];
    XtVaSetValues (optionMenuDigit3, 	XtNforeground, digit3Color->pixel, NULL);
	if(drawGC)
		Draw(drawGC,drawArea);
	Recalculate();
}
int main(int argc, char **argv)
{
    XtAppContext app_context;
    Widget topLevel,
				mainWin, 
					frame, 
						//drawArea, 
					rowColumn,
						optionMenus,
							//optionMenuDigit1,
							//optionMenuDigit2,
							//optionMenuDigit3,
							//optionMenuMultiplier,
							//optionMenuTolerance, 
						returnValues,
						
						controlRow,
							quitBtn
	;

    /*
     * Register the default language procedure
     */
    XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

    topLevel = XtVaAppInitialize(
      &app_context,		 	/* Application context */
      "Draw",				/* Application class */
      NULL, 0,				/* command line option list */
      &argc, argv,			/* command line args */
      fallback_resources,	/* for missing app-defaults file */
	  XmNdeleteResponse, XmDO_NOTHING,
      NULL);				/* terminate varargs list */

	  //close
    closeQuestion = XmCreateQuestionDialog(topLevel, "closeQuestion", NULL, 0);
    XtVaSetValues(closeQuestion, XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
    XtUnmanageChild(XmMessageBoxGetChild(closeQuestion, XmDIALOG_HELP_BUTTON));
    XtAddCallback(closeQuestion, XmNokCallback, closeQuestionCB, (XtPointer) 1);
    
    mainWin = XtVaCreateManagedWidget(
      "mainWin",			/* widget name */
      xmMainWindowWidgetClass,		/* widget class */
      topLevel,				/* parent widget*/
      XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
      NULL);				/* terminate varargs list */

    frame = XtVaCreateManagedWidget(
      "frame",				/* widget name */
      xmFrameWidgetClass,		/* widget class */
      mainWin,				/* parent widget */
      NULL);				/* terminate varargs list */

    drawArea = XtVaCreateManagedWidget(
      "drawingArea",			/* widget name */
      xmDrawingAreaWidgetClass,		/* widget class */
      frame,				/* parent widget*/
      XmNwidth, 200,			/* set startup width */
      XmNheight, 100,			/* set startup height */
      NULL);				/* terminate varargs list */

/*
    XSelectInput(XtDisplay(drawArea), XtWindow(drawArea), 
      KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
      | Button1MotionMask );
*/
        
    rowColumn = XtVaCreateManagedWidget(
      "rowColumn",			/* widget name */
      xmRowColumnWidgetClass,		/* widget class */
      mainWin,				/* parent widget */
      NULL);				/* terminate varargs list */

	optionMenus = XtVaCreateManagedWidget(
		"control",				/* widget name */
		xmRowColumnWidgetClass,			/* widget class */
		rowColumn,				/* parent widget */
		XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
		XmNorientation, XmHORIZONTAL,		/* orientation */
		XmNpacking, XmPACK_COLUMN,		/* packing mode */
		NULL);	
	 XmString digit1,digit2,digit3,
	 Black0,Brown1,Red2,Orange3,Yellow4,
	 Green5,Blue6,Violet7,Gray8,White9;
    digit1	= XmStringCreateLocalized ("1st Digit:");
    digit2	= XmStringCreateLocalized ("2nd Digit:");
    digit3	= XmStringCreateLocalized ("3rd Digit:");
    Black0	= XmStringCreateLocalized ("0 Black");
    Brown1	= XmStringCreateLocalized ("1 Brown");
    Red2	= XmStringCreateLocalized ("2 Red");
    Orange3	= XmStringCreateLocalized ("3 Orange");
    Yellow4	= XmStringCreateLocalized ("4 Yellow");
    Green5	= XmStringCreateLocalized ("5 Green");
    Blue6	= XmStringCreateLocalized ("6 Blue");
    Violet7	= XmStringCreateLocalized ("7 Violet");
    Gray8 	= XmStringCreateLocalized ("8 Gray");
    White9 	= XmStringCreateLocalized ("9 White");
    optionMenuDigit1 = XmVaCreateSimpleOptionMenu (optionMenus,
                                "line_width_menu:", digit1, 'W', 
                                DIGIT1val-1 /*initial menu selection*/, Digit1CB,
                           //   XmVaPUSHBUTTON, Black0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, Brown1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, Red2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, Orange3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, Yellow4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, Green5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, Blue6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, Violet7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, Gray8, '8', NULL, NULL,
                                XmVaPUSHBUTTON, White9, '9', NULL, NULL,
                                NULL);
								
								
								
	optionMenuDigit2 = XmVaCreateSimpleOptionMenu (optionMenus,
                                "line_width_menu:", digit2, 'W', 
                                DIGIT2val /*initial menu selection*/, Digit2CB,
                                XmVaPUSHBUTTON, Black0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, Brown1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, Red2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, Orange3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, Yellow4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, Green5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, Blue6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, Violet7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, Gray8, '8', NULL, NULL,
                                XmVaPUSHBUTTON, White9, '9', NULL, NULL,
                                NULL);
	optionMenuDigit3= XmVaCreateSimpleOptionMenu (optionMenus,
                                "line_width_menu:", digit3, 'W', 
                                DIGIT3val /*initial menu selection*/, Digit3CB,
                                XmVaPUSHBUTTON, Black0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, Brown1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, Red2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, Orange3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, Yellow4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, Green5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, Blue6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, Violet7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, Gray8, '8', NULL, NULL,
                                XmVaPUSHBUTTON, White9, '9', NULL, NULL,
                                NULL);
	XmStringFree (digit1);
	XmStringFree (digit2);
	XmStringFree (digit3);
    XmStringFree (Black0);
    XmStringFree (Brown1);
    XmStringFree (Red2);
    XmStringFree (Orange3);
    XmStringFree (Yellow4);
    XmStringFree (Green5);
    XmStringFree (Blue6);
    XmStringFree (Violet7);
    XmStringFree (Gray8);
    XmStringFree (White9);
	
	/////////////////////////////////////////////////////////////////////////
	XmString multiplierLabel,
	
	MultiplierBlack0,
	MultiplierBrown1,
	MultiplierRed2,
	MultiplierOrange3,
	MultiplierYellow4,
	MultiplierGreen5,
	MultiplierBlue6,
	MultiplierViolet7,
	MultiplierGray8,
	MultiplierWhite9,
	MultiplierGold10,
	MultiplierSilver11;
	
    multiplierLabel	= XmStringCreateLocalized ("Multiplier:");
	
    MultiplierBlack0	= XmStringCreateLocalized ("x1 Black");
    MultiplierBrown1	= XmStringCreateLocalized ("x10 Brown");
    MultiplierRed2		= XmStringCreateLocalized ("x100 Red");
    MultiplierOrange3	= XmStringCreateLocalized ("x1k Orange");
    MultiplierYellow4	= XmStringCreateLocalized ("x10k Yellow");
    MultiplierGreen5	= XmStringCreateLocalized ("x100k Green");
    MultiplierBlue6		= XmStringCreateLocalized ("x1M Blue");
    MultiplierViolet7	= XmStringCreateLocalized ("x10M Violet");
    MultiplierGray8 	= XmStringCreateLocalized ("x100M Gray");
    MultiplierWhite9 	= XmStringCreateLocalized ("x1G White");
    MultiplierGold10 	= XmStringCreateLocalized ("/10 Gold");
    MultiplierSilver11 	= XmStringCreateLocalized ("/100 Silver");
	optionMenuMultiplier= XmVaCreateSimpleOptionMenu (optionMenus,
                                "line_width_menu:", multiplierLabel, 'W', 
                                MULTIPLIERval /*initial menu selection*/, MultiplierCB,
                                XmVaPUSHBUTTON, MultiplierBlack0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierBrown1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierRed2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierOrange3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierYellow4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierGreen5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierBlue6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierViolet7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierGray8, '8', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierWhite9, '9', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierGold10, '+', NULL, NULL,
                                XmVaPUSHBUTTON, MultiplierSilver11, '-', NULL, NULL,
                                NULL);
								
								
 XmString   toleranceLabel	= XmStringCreateLocalized ("Tolerance:"),
    toleranceBrown1	= XmStringCreateLocalized ("1% Brown"),
    toleranceRed2	= XmStringCreateLocalized ("2% Red"),
    toleranceOrange3	= XmStringCreateLocalized ("3% Orange"),
    toleranceYellow4	= XmStringCreateLocalized ("4% Yellow"),
    toleranceGreen5	= XmStringCreateLocalized ("0.5% Green"),
    toleranceBlue6	= XmStringCreateLocalized ("0.25% Blue"),
    toleranceViolet7	= XmStringCreateLocalized ("0.10% Violet"),
    toleranceGray8 	= XmStringCreateLocalized ("0.05% Gray"),
   // toleranceWhite9 	= XmStringCreateLocalized ("±% White"),
    toleranceGold10 	= XmStringCreateLocalized ("5% Gold"),
    toleranceSilver11 	= XmStringCreateLocalized ("10% Silver");
	optionMenuTolerance= XmVaCreateSimpleOptionMenu (optionMenus,
                                "line_width_menu:", toleranceLabel, 'W', 
                                TOLERANCEval-1 /*initial menu selection*/, ToleranceCB,
                                //XmVaPUSHBUTTON, toleranceBlack0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceBrown1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceRed2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceOrange3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceYellow4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceGreen5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceBlue6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceViolet7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceGray8, '8', NULL, NULL,
                                //XmVaPUSHBUTTON, toleranceWhite9, '9', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceGold10, 'G', NULL, NULL,
                                XmVaPUSHBUTTON, toleranceSilver11, 'S', NULL, NULL,
                                NULL);
    
	
	XmStringFree (multiplierLabel);
	XmStringFree (MultiplierBlack0);
	XmStringFree (MultiplierBrown1);
	XmStringFree (MultiplierRed2);
	XmStringFree (MultiplierOrange3);
	XmStringFree (MultiplierYellow4);
	XmStringFree (MultiplierGreen5);
	XmStringFree (MultiplierBlue6);
	XmStringFree (MultiplierViolet7);
	XmStringFree (MultiplierGray8);
	XmStringFree (MultiplierWhite9);
	XmStringFree (MultiplierGold10);
	XmStringFree (MultiplierSilver11);
	XmStringFree (toleranceLabel);
	XmStringFree (toleranceBrown1);
	XmStringFree (toleranceRed2);
	XmStringFree (toleranceOrange3);
	XmStringFree (toleranceYellow4);
	XmStringFree (toleranceGreen5);
	XmStringFree (toleranceBlue6);
	XmStringFree (toleranceViolet7);
	XmStringFree (toleranceGray8);
	XmStringFree (toleranceGold10);
	XmStringFree (toleranceSilver11);
	
	
	
	
	
	
	
	
	
    XtManageChild (optionMenuDigit1);
    XtManageChild (optionMenuDigit2);
    XtManageChild (optionMenuDigit3);
    XtManageChild (optionMenuMultiplier);
    XtManageChild (optionMenuTolerance);
	
	
	
	
	
	
	returnValues = XtVaCreateManagedWidget(
      "returnValues",			/* widget name */
      xmRowColumnWidgetClass,		/* widget class */
      rowColumn,				/* parent widget */
      NULL);	
	
	
	//label = XtVaCreateManagedWidget(
	//		"label", 		/* widget name */
	//		xmLabelWidgetClass,	/* widget class */
	//		returnValues, 			/* parent */
	//		NULL);
	int n = 0;
	Arg args[n];
    XtSetArg (args[n], XmNeditable, False);              n++;
    XtSetArg (args[n], XmNcursorPositionVisible, False); n++;
    XtSetArg (args[n], XmNshadowThickness, 0);           n++;
    XtSetArg (args[n], XmNhighlightThickness, 0);        n++;
    label = XmCreateText (returnValues, "text_output", args, n);
    XtManageChild (label);
//	label = XmCreateText(returnValues,"ddd",NULL);
	/////////////////////////////////////////////////////////////////
	
	
	 Colormap cmap = DefaultColormap(XtDisplay(drawArea), DefaultScreen(XtDisplay(drawArea)));
	
	size_t i=0;
	for(;i<COLOR_COUNT;i++){
		
		XAllocNamedColor(XtDisplay(drawArea), cmap, colorNames[i], &colors[i], &colors[i]);
	}
	

	
	
    XtVaSetValues (optionMenuDigit1, 	XtNforeground, digit1Color->pixel, NULL);
    XtVaSetValues (optionMenuDigit2, 	XtNforeground, digit2Color->pixel, NULL);
    XtVaSetValues (optionMenuDigit3, 	XtNforeground, digit3Color->pixel, NULL);
    XtVaSetValues (optionMenuMultiplier, 	XtNforeground, multiplierColor->pixel, NULL);
    XtVaSetValues (optionMenuTolerance, 	XtNforeground, toleranceColor->pixel, NULL);
	
	controlRow = XtVaCreateManagedWidget(
		"control",				/* widget name */
		xmRowColumnWidgetClass,			/* widget class */
		rowColumn,				/* parent widget */
		XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
		XmNorientation, XmHORIZONTAL,		/* orientation */
		XmNpacking, XmPACK_COLUMN,		/* packing mode */
		NULL);	
	
    quitBtn = XtVaCreateManagedWidget(
      "Quit",				/* widget name */
      xmPushButtonWidgetClass,		/* widget class */
      controlRow,			/* parent widget*/
      NULL);				/* terminate varargs list */

    XmMainWindowSetAreas(mainWin, NULL, rowColumn, NULL, NULL, frame);

    //XtAddCallback(drawArea, XmNinputCallback, DrawLineCB, drawArea);
    //XtAddEventHandler(drawArea, ButtonMotionMask, False, InputLineEH, NULL);
    XtAddCallback(drawArea, XmNexposeCallback, ExposeCB, drawArea);
    XtAddCallback(drawArea, XmNresizeCallback, ResizeCB, NULL);

    XtAddCallback(quitBtn, XmNactivateCallback, QuitCB, 0);

    XtRealizeWidget(topLevel);

    XtAppMainLoop(app_context);

    return 0;
}

