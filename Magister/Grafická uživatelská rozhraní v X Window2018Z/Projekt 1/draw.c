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
	
#include <Xm/CascadeB.h>
#include <Xm/MenuShell.h>
#include <Xm/ScrolledW.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <X11/Xmu/Editres.h>
/*
 * Common C library include files
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Shared variables
 */
typedef enum { false, true } bool;
#define LINES_ALLOC_STEP	10	/* memory allocation stepping */
XSegment *lines = NULL;		/* array of line descriptors */
int maxlines = 0;		/* space allocated for max lines */
int nlines = 0;			/* current number of lines */

GC drawGC = 0;			/* GC used for final drawing */
GC inputGC = 0;			/* GC used for drawing current position */

int x1=0, y1=0, x2=0, y2=0;		/* input points */ 
int button_pressed = 0;		/* input state */

typedef enum {
  RED,
  GREEN,
  BLUE,
  PURPLE,
  YELLOW,
  CYAN
}ColorEnum;
typedef enum {
  POINT,
  LINE,
  RECTANGLE,
  ELLIPSE
}ShapeEnum;
typedef enum {
  w0,
  w1,
  w2,
  w3,
  w4,
  w5,
  w6,
  w7,
  w8,
  w9,
}WidthEnum;
WidthEnum lineWidth = w3;
int lineType = LineSolid;
ColorEnum lineColorFG = CYAN;
ColorEnum lineColorBG = GREEN;
ColorEnum fillColorFG = YELLOW;
ColorEnum fillColorBG = RED;
ShapeEnum shapeType = LINE;
bool fill=true;
typedef struct {
	int x1;
	int y1;
	int x2;
	int y2;
	int lineType;
	bool fill;
    XColor *lineFgColor;
    XColor *lineBgColor;
    XColor *fillFgColor;
    XColor *fillBgColor;
	ShapeEnum shapeType;
	GC gc;
	int lineWidth;
}DrawObject;
DrawObject * objectArray=NULL;
int countOfObjects=0;

void lineWidthCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    lineWidth = (WidthEnum) client_data;
   // puts (XtName (menu_item));
}
void lineTypeCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    long intType = ((long)client_data);
    switch(intType){
      case 0:
	lineType = LineSolid;
	break;
      case 1:
	lineType = LineDoubleDash;
	break;
      default:
	break;
    }
   // puts (XtName (menu_item));
}

void lineColorFGCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    lineColorFG = (ColorEnum) client_data;
   // puts (XtName (menu_item));
}
void lineColorBGCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    lineColorBG = (ColorEnum) client_data;
   // puts (XtName (menu_item));
}
void fillColorFGCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    fillColorFG = (ColorEnum) client_data;
   // puts (XtName (menu_item));
}
void fillCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    fill = (ColorEnum) client_data;
   // puts (XtName (menu_item));
}

void fillColorBGCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    fillColorBG = (ColorEnum) client_data;
   // puts (XtName (menu_item));
}
void shapeTypeCB (Widget menu_item, XtPointer client_data,
                XtPointer call_data)
{
    shapeType = (ShapeEnum) client_data;
   // puts (XtName (menu_item));
}
XColor	redColor,blueColor,greenColor,purpleColor,yellowColor,cyanColor;
void SetColor(XColor **Color,ColorEnum lineColorFG){

  switch(lineColorFG){
    case RED:*Color = &redColor;
      break;
    case GREEN:*Color = &greenColor;
      break;
    case BLUE: *Color = &blueColor;
      break;
    case PURPLE: *Color = &purpleColor;
      break;
    case YELLOW: *Color = &yellowColor;
      break;
    case CYAN: *Color = &cyanColor;
      break;
      
    default:
      break;
  }
}
    XColor *lineFgColor=&redColor;
    XColor *lineBgColor=&redColor;
    XColor *fillFgColor=&redColor;
    XColor *fillBgColor=&redColor;
/*
 * "InputLine" event handler
 */
/* ARGSUSED */
void InputLineEH(Widget w, XtPointer client_data, XEvent *event, Boolean *cont)
{
    Pixel    fg, bg;
    SetColor(&lineFgColor,lineColorFG);
    SetColor(&lineBgColor,lineColorBG);
    SetColor(&fillFgColor,fillColorFG);
    SetColor(&fillBgColor,fillColorBG); 
    
    
    if (button_pressed) {
		if (!inputGC) {
			inputGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
		}
			XSetFunction(XtDisplay(w), inputGC, GXxor);
			XSetPlaneMask(XtDisplay(w), inputGC, ~0);
			XtVaGetValues(w, XmNforeground, &fg, XmNbackground, &bg, NULL);
			
	XSetForeground(XtDisplay(w), inputGC, bg ^ lineFgColor->pixel);
	XSetBackground(XtDisplay(w), inputGC, bg ^ lineBgColor->pixel);
	XSetLineAttributes(XtDisplay(w), inputGC, lineWidth, lineType, CapButt, JoinMiter);
		if (button_pressed > 1) {
			/* erase previous position */
			switch (shapeType) {
				case LINE:
					XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
					break;
				case RECTANGLE:
				
					if(fill)
					{
						XSetForeground(XtDisplay(w), inputGC, bg ^ fillFgColor->pixel);
						XFillRectangle(XtDisplay(w), XtWindow(w), inputGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
						XSetForeground(XtDisplay(w), inputGC, bg ^ lineFgColor->pixel);
					}
					XDrawRectangle(XtDisplay(w), XtWindow(w), inputGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
					break;
				case ELLIPSE:
					if(fill)
					{
						XSetForeground(XtDisplay(w), inputGC, bg ^ fillFgColor->pixel);
						XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
						XSetForeground(XtDisplay(w), inputGC, bg ^ lineFgColor->pixel);
					}
					XDrawArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
					break;
				default:
					if(lineWidth== 0){
					  XDrawPoint(XtDisplay(w), XtWindow(w), inputGC, x2, y2);
					}
					else
					{
					  XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - lineWidth, y1 - lineWidth, lineWidth * 2, lineWidth * 2, 0, 360 * 64);
				    }
					break;
			}
		} else {
			/* remember first MotionNotify */
			button_pressed = 2;
		}

		x2 = event->xmotion.x;
		y2 = event->xmotion.y;
		switch(shapeType){
			case POINT:
				if(lineWidth== 0){
					XDrawPoint(XtDisplay(w), XtWindow(w), inputGC, x2, y2);
				}
				else
				{
					XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - lineWidth, y1 - lineWidth, lineWidth * 2, lineWidth * 2, 0, 360 * 64);
				}
			
				break;
			case LINE: 
				XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
				break;
			case RECTANGLE:
				if(fill)
				{
					XSetForeground(XtDisplay(w), inputGC, bg ^ fillFgColor->pixel);
					XFillRectangle(XtDisplay(w), XtWindow(w), inputGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
					XSetForeground(XtDisplay(w), inputGC, bg ^ lineFgColor->pixel);
				}
				XDrawRectangle(XtDisplay(w), XtWindow(w), inputGC, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
				break;
			case ELLIPSE:
				if(fill)
				{
					XSetForeground(XtDisplay(w), inputGC, bg ^ fillFgColor->pixel);
					XFillArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
					XSetForeground(XtDisplay(w), inputGC, bg ^ lineFgColor->pixel);
				}
				XDrawArc(XtDisplay(w), XtWindow(w), inputGC, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
				break;
			default:
		      
				break;
		   
		}
    }
}

/*
 * "DrawLine" callback function
 */
void DrawLineCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    SetColor(&lineFgColor,lineColorFG);
    SetColor(&lineBgColor,lineColorBG);
    SetColor(&fillFgColor,fillColorFG);
    SetColor(&fillBgColor,fillColorBG); 
    //Arg al[4];
    //int ac;
   // XGCValues v;
    XmDrawingAreaCallbackStruct *d = (XmDrawingAreaCallbackStruct*) call_data;

    switch (d->event->type) {
	case ButtonPress:
	    if (d->event->xbutton.button == Button1) {
		button_pressed = 1;
		x1 = d->event->xbutton.x;
		y1 = d->event->xbutton.y;
	    }
	    break;

	case ButtonRelease:
	    /*if (d->event->xbutton.button == Button1) {
		if (++nlines > maxlines) {
		    maxlines += LINES_ALLOC_STEP;
		    lines = (XSegment*) XtRealloc((char*)lines,
		      (Cardinal)(sizeof(XSegment) * maxlines));
		}

		lines[nlines - 1].x1 = x1;
		lines[nlines - 1].y1 = y1;
		lines[nlines - 1].x2 = d->event->xbutton.x;
		lines[nlines - 1].y2 = d->event->xbutton.y;

		button_pressed = 0;

		if (!drawGC) {
		    ac = 0;
		    XtSetArg(al[ac], XmNforeground, &v.foreground); ac++;
		    XtGetValues(w, al, ac);
		    drawGC = XCreateGC(XtDisplay(w), XtWindow(w),
			GCForeground, &v);
		}
		XDrawLine(XtDisplay(w), XtWindow(w), drawGC,
		  x1, y1, d->event->xbutton.x, d->event->xbutton.y);
	    }*/
		button_pressed = 0;
		if(countOfObjects%LINES_ALLOC_STEP==0)
			objectArray = (DrawObject*) realloc(objectArray,sizeof(DrawObject)*(countOfObjects+LINES_ALLOC_STEP));
		
		
		DrawObject drawObject;
		drawObject.x1=x1;
		drawObject.y1=y1;
		drawObject.x2=d->event->xbutton.x;
		drawObject.y2=d->event->xbutton.y;
		drawObject.lineType=lineType;
		drawObject.lineFgColor=lineFgColor;
		drawObject.lineBgColor=lineBgColor;
		drawObject.fillFgColor=fillFgColor;
		drawObject.fillBgColor=fillBgColor;
		drawObject.fill=fill;
		drawObject.shapeType=shapeType;
		drawObject.lineWidth=lineWidth;
		
		drawObject.gc = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
		XSetForeground(XtDisplay(w), drawObject.gc, lineFgColor->pixel);
		XSetBackground(XtDisplay(w), drawObject.gc, lineBgColor->pixel);
		
		XSetLineAttributes(XtDisplay(w), drawObject.gc, lineWidth, lineType, CapButt, JoinMiter);
		
		
		objectArray[countOfObjects]=drawObject;
		
		
		
		countOfObjects++;
		XClearArea(XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True);
	    break;
    }
}

/*
 * "Expose" callback function
 */
/* ARGSUSED */
void ExposeCB(Widget w, XtPointer client_data, XtPointer call_data)
{
	int i=0;
	for(;i<countOfObjects;i++){
		int x1=objectArray[i].x1;
		int y1=objectArray[i].y1;
		int x2=objectArray[i].x2;
		int y2=objectArray[i].y2;
		switch (objectArray[i].shapeType) {
			case LINE:
				XDrawLine(XtDisplay(w), XtWindow(w), objectArray[i].gc, x1, y1, x2, y2);
				break;
			case RECTANGLE:
				if (objectArray[i].fill) {
					XSetForeground(XtDisplay(w), objectArray[i].gc, objectArray[i].fillFgColor->pixel);
					XFillRectangle(XtDisplay(w), XtWindow(w), objectArray[i].gc, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
					XSetForeground(XtDisplay(w), objectArray[i].gc, objectArray[i].lineFgColor->pixel);
				}
				XDrawRectangle(XtDisplay(w), XtWindow(w), objectArray[i].gc, (x1 < x2) ? x1 : x2, (y1 < y2) ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
				break;
			case ELLIPSE:
				if (objectArray[i].fill) {
					XSetForeground(XtDisplay(w), objectArray[i].gc, objectArray[i].fillFgColor->pixel);
					XFillArc(XtDisplay(w), XtWindow(w), objectArray[i].gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
					XSetForeground(XtDisplay(w), objectArray[i].gc, objectArray[i].lineFgColor->pixel);
				}
				XDrawArc(XtDisplay(w), XtWindow(w), objectArray[i].gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
				break;
			case POINT:
			
				if(objectArray[i].lineWidth== 0){
					  
						XDrawPoint(XtDisplay(w), XtWindow(w), objectArray[i].gc, x2, y2);
					}
					else
					{
					  XFillArc(XtDisplay(w), XtWindow(w), objectArray[i].gc, x1 - objectArray[i].lineWidth, y1 - objectArray[i].lineWidth, objectArray[i].lineWidth * 2, objectArray[i].lineWidth * 2, 0, 360 * 64);
				    }
				
				break;
			default:
				break;
		}
	}
	
    /*if (nlines <= 0)
	return;
    if (!drawGC)
	drawGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
    XDrawSegments(XtDisplay(w), XtWindow(w), drawGC, lines, nlines);*/
}
void Clear(Widget w){
	
	size_t i = 0;
	for (; i < countOfObjects; i++) {
			XFreeGC(XtDisplay(w), objectArray[i].gc);
		}
		countOfObjects=0;
	if(objectArray!=NULL)
		free(objectArray);
	objectArray=NULL;
}
/*
 * "Clear" button callback function
 */
/* ARGSUSED */
void ClearCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
    Widget wcd = (Widget) client_data;
	Clear(w);
    XClearWindow(XtDisplay(wcd), XtWindow(wcd));
	
}

/*
 * "Quit" button callback function
 */
/* ARGSUSED */
Widget closeQuestion;
void QuitCB(Widget w, XtPointer client_data, XtPointer call_data)
{ 
    XtManageChild(closeQuestion);
}
void closeQuestionCB(Widget w, XtPointer client_data, XtPointer call_data) {
	Clear(w);
	
	if ((long)client_data) {
		
		if (inputGC != NULL) {
			//XFreeGC(XtDisplay(w), inputGC);
		}
	}
	exit(0);
}
int main(int argc, char **argv)
{
  
  
  
    XtAppContext app_context;
    Widget 
    topLevel, 
		mainWin, 
			frame,
				drawArea,
			menu,
				style,
					option_menu_shape,
					option_menu_line_type,
					option_menu_fill,
				option_menu_width,
				colors,
					option_menu_lineFG_color,
					option_menu_lineBG_color,
					option_menu_fillFG_color,
					option_menu_fillBG_color,
					quitBtn, 
				clearBtn;

    /*
     * Register the default language procedure
     */
    XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

    char *fallback_resources[] = {
		"*closeQuestion.dialogTitle: Question",
		"*closeQuestion.messageString: Quit?",
		"*closeQuestion.okLabelString: Yes",
		"*closeQuestion.cancelLabelString: No",
		"*closeQuestion.messageAlignment: XmALIGNMENT_CENTER",
		NULL
	};
    topLevel = XtVaAppInitialize(
      &app_context,		 	/* Application context */
      "Draw",				/* Application class */
      NULL, 0,				/* command line option list */
      &argc, argv,			/* command line args */
      fallback_resources,		/* for missing app-defaults file */
      //XmNminWidth, 1610,		/* set startup width */
      //XmNminHeight, 100,		/* set startup height */
      XmNdeleteResponse, XmDO_NOTHING,
      NULL				/* terminate varargs list */);

    //close
    closeQuestion = XmCreateQuestionDialog(topLevel, "closeQuestion", NULL, 0);
    XtVaSetValues(closeQuestion, XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
    XtUnmanageChild(XmMessageBoxGetChild(closeQuestion, XmDIALOG_HELP_BUTTON));
    XtAddCallback(closeQuestion, XmNokCallback, closeQuestionCB, (XtPointer) 1);
    
    Atom wm_delete = XInternAtom(XtDisplay(topLevel), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(topLevel, wm_delete, QuitCB, NULL);
    XmActivateWMProtocol(topLevel, wm_delete);
    
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
      XmNwidth, 500,			/* set startup width */
      XmNheight, 400,			/* set startup height */
      NULL);				/* terminate varargs list */


//Settings Menu
	  //Menu
 //   rowColumn = XtVaCreateManagedWidget(
  //    "rowColumn",			/* widget name */
 //     xmRowColumnWidgetClass,		/* widget class */
  //    mainWin,				/* parent widget */
 //     XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
 //     XmNorientation, XmHORIZONTAL,	/* orientation */
  //    XmNpacking, XmPACK_COLUMN,	/* packing mode */
  //    NULL);				/* terminate varargs list */

	menu = XtVaCreateManagedWidget(
		"menu",			/* widget name */
		xmRowColumnWidgetClass,			/* widget class */
		mainWin,				/* parent widget */
		NULL);		

	style = XtVaCreateManagedWidget(
		"style",				/* widget name */
		xmRowColumnWidgetClass,			/* widget class */
		menu,				/* parent widget */
		XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
		XmNorientation, XmHORIZONTAL,		/* orientation */
		XmNpacking, XmPACK_COLUMN,		/* packing mode */
		NULL);	
    
	colors = XtVaCreateManagedWidget(
		"colors",				/* widget name */
		xmRowColumnWidgetClass,			/* widget class */
		menu,				/* parent widget */
		XmNentryAlignment, XmALIGNMENT_CENTER,	/* alignment */
		XmNorientation, XmHORIZONTAL,		/* orientation */
		XmNpacking, XmPACK_COLUMN,		/* packing mode */
		NULL);	

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	XmString       draw_shape, point, line, square, ellipse;
    draw_shape = XmStringCreateLocalized ("Shape:");
    point = XmStringCreateLocalized ("Point");
    line = XmStringCreateLocalized ("Line");
    square = XmStringCreateLocalized ("Square");
    ellipse = XmStringCreateLocalized ("Ellipse");
    option_menu_shape = XmVaCreateSimpleOptionMenu (style,
                                "shape_menu", draw_shape, 'S', 
                                shapeType /*initial menu selection*/, shapeTypeCB,
                                XmVaPUSHBUTTON, point, 'P', NULL, NULL,
                                XmVaPUSHBUTTON, line, 'L', NULL, NULL,
                                XmVaPUSHBUTTON, square, 'Q', NULL, NULL,
                                XmVaPUSHBUTTON, ellipse, 'E', NULL, NULL,
                                NULL);


    XmStringFree (point);
    XmStringFree (line);
    XmStringFree (square);
    XmStringFree (ellipse);
    XmStringFree (draw_shape);
    XtManageChild (option_menu_shape);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

    

    XmString draw_width,Width0,Width1,Width2,Width3,
	Width4,Width5,Width6,Width7,Width8,Width9;
    draw_width = XmStringCreateLocalized ("Line width:");
    Width0 = XmStringCreateLocalized ("0");
    Width1 = XmStringCreateLocalized ("1");
    Width2 = XmStringCreateLocalized ("2");
    Width3 = XmStringCreateLocalized ("3");
    Width4 = XmStringCreateLocalized ("4");
    Width5 = XmStringCreateLocalized ("5");
    Width6 = XmStringCreateLocalized ("6");
    Width7 = XmStringCreateLocalized ("7");
    Width8 = XmStringCreateLocalized ("8");
    Width9 = XmStringCreateLocalized ("9");
    option_menu_width = XmVaCreateSimpleOptionMenu (menu,
                                "line_width_menu:", draw_width, 'W', 
                                lineWidth /*initial menu selection*/, lineWidthCB,
                                XmVaPUSHBUTTON, Width0, '0', NULL, NULL,
                                XmVaPUSHBUTTON, Width1, '1', NULL, NULL,
                                XmVaPUSHBUTTON, Width2, '2', NULL, NULL,
                                XmVaPUSHBUTTON, Width3, '3', NULL, NULL,
                                XmVaPUSHBUTTON, Width4, '4', NULL, NULL,
                                XmVaPUSHBUTTON, Width5, '5', NULL, NULL,
                                XmVaPUSHBUTTON, Width6, '6', NULL, NULL,
                                XmVaPUSHBUTTON, Width7, '7', NULL, NULL,
                                XmVaPUSHBUTTON, Width8, '8', NULL, NULL,
                                XmVaPUSHBUTTON, Width9, '9', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_width);
    XmStringFree (Width0);
    XmStringFree (Width1);
    XmStringFree (Width2);
    XmStringFree (Width3);
    XmStringFree (Width4);
    XmStringFree (Width5);
    XmStringFree (Width6);
    XmStringFree (Width7);
    XmStringFree (Width8);
    XmStringFree (Width9);
    XtManageChild (option_menu_width);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    
    XmString draw_FGcolor,ColorRed,ColorGreen,ColorBlue,ColorPurple,ColorCyan,ColorYellow;
    draw_FGcolor = XmStringCreateLocalized ("Line FG color:");
    ColorRed = XmStringCreateLocalized ("Red");
    ColorGreen = XmStringCreateLocalized ("Green");
    ColorBlue = XmStringCreateLocalized ("Blue");
    ColorPurple = XmStringCreateLocalized ("Purple");
    ColorYellow = XmStringCreateLocalized ("Yellow");
    ColorCyan = XmStringCreateLocalized ("Cyan");
    option_menu_lineFG_color = XmVaCreateSimpleOptionMenu (colors,
                                "linefg_color_menu:", draw_FGcolor, 'C', 
                                lineColorFG /*initial menu selection*/, lineColorFGCB,
                                XmVaPUSHBUTTON, ColorRed, 'R', NULL, NULL,
                                XmVaPUSHBUTTON, ColorGreen, 'G', NULL, NULL,
                                XmVaPUSHBUTTON, ColorBlue, 'B', NULL, NULL,
                                XmVaPUSHBUTTON, ColorPurple, 'P', NULL, NULL,
                                XmVaPUSHBUTTON, ColorYellow, 'Y', NULL, NULL,
                                XmVaPUSHBUTTON, ColorCyan, 'C', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_FGcolor);
    XtManageChild (option_menu_lineFG_color);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    
    XmString draw_BGcolor;
    draw_BGcolor = XmStringCreateLocalized ("Line BG color:");
    option_menu_lineBG_color = XmVaCreateSimpleOptionMenu (colors,
                                "linebg_color_menu:", draw_BGcolor, 'C', 
                                lineColorBG /*initial menu selection*/, lineColorBGCB,
                                XmVaPUSHBUTTON, ColorRed, 'R', NULL, NULL,
                                XmVaPUSHBUTTON, ColorGreen, 'G', NULL, NULL,
                                XmVaPUSHBUTTON, ColorBlue, 'B', NULL, NULL,
                                XmVaPUSHBUTTON, ColorPurple, 'P', NULL, NULL,
                                XmVaPUSHBUTTON, ColorYellow, 'Y', NULL, NULL,
                                XmVaPUSHBUTTON, ColorCyan, 'C', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_BGcolor);
    XtManageChild (option_menu_lineBG_color);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    XmString draw_fillFG_color;
    draw_fillFG_color = XmStringCreateLocalized ("Fill FG color:");
    option_menu_fillFG_color = XmVaCreateSimpleOptionMenu (colors,
                                "fillfg_color_menu:", draw_fillFG_color, 'F', 
                                fillColorFG /*initial menu selection*/, fillColorFGCB,
                                XmVaPUSHBUTTON, ColorRed, 'R', NULL, NULL,
                                XmVaPUSHBUTTON, ColorGreen, 'G', NULL, NULL,
                                XmVaPUSHBUTTON, ColorBlue, 'B', NULL, NULL,
                                XmVaPUSHBUTTON, ColorPurple, 'P', NULL, NULL,
                                XmVaPUSHBUTTON, ColorYellow, 'Y', NULL, NULL,
                                XmVaPUSHBUTTON, ColorCyan, 'C', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_fillFG_color);
    XtManageChild (option_menu_fillFG_color);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    XmString draw_fillBG_color;
    draw_fillBG_color = XmStringCreateLocalized ("Fill BG color:");
    option_menu_fillBG_color = XmVaCreateSimpleOptionMenu (colors,
                                "fillbg_color_menu:", draw_fillBG_color, 'F', 
                                fillColorBG /*initial menu selection*/, fillColorBGCB,
                                XmVaPUSHBUTTON, ColorRed, 'R', NULL, NULL,
                                XmVaPUSHBUTTON, ColorGreen, 'G', NULL, NULL,
                                XmVaPUSHBUTTON, ColorBlue, 'B', NULL, NULL,
                                XmVaPUSHBUTTON, ColorPurple, 'P', NULL, NULL,
                                XmVaPUSHBUTTON, ColorYellow, 'Y', NULL, NULL,
                                XmVaPUSHBUTTON, ColorCyan, 'C', NULL, NULL,
                                NULL);
    
    XmStringFree (ColorRed);
    XmStringFree (ColorGreen);
    XmStringFree (ColorBlue);
    XmStringFree (ColorPurple);
    XmStringFree (ColorYellow);
    XmStringFree (ColorCyan);
    XmStringFree (draw_fillBG_color);
    XtManageChild (option_menu_fillBG_color);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    XmString draw_line_type,lineSolid,lineDoubleDash;
    draw_line_type = XmStringCreateLocalized ("Line type:");
    lineSolid = XmStringCreateLocalized ("Solid");
    lineDoubleDash = XmStringCreateLocalized ("Double Dash");
    option_menu_line_type = XmVaCreateSimpleOptionMenu (style,
                                "line_type_menu:", draw_line_type, 'T', 
                                lineType /*initial menu selection*/, lineTypeCB,
                                XmVaPUSHBUTTON, lineSolid, 'S', NULL, NULL,
                                XmVaPUSHBUTTON, lineDoubleDash, 'D', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_line_type);
    XmStringFree (lineSolid);
    XmStringFree (lineDoubleDash);
    XtManageChild (option_menu_line_type);
	///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
    XmString draw_fill,FillYES,FilNO;
    draw_fill = XmStringCreateLocalized ("Fill:");
    FillYES = XmStringCreateLocalized ("Yes");
    FilNO = XmStringCreateLocalized ("No");
    option_menu_fill = XmVaCreateSimpleOptionMenu (style,
                                "line_type_menu:", draw_fill, 'F', 
                                fill /*initial menu selection*/, fillCB,
                                XmVaPUSHBUTTON, FilNO, 'N', NULL, NULL,
                                XmVaPUSHBUTTON, FillYES, 'Y', NULL, NULL,
                                NULL);
    
    XmStringFree (draw_fill);
    XmStringFree (FillYES);
    XmStringFree (FilNO);
    XtManageChild (option_menu_fill);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
    XSelectInput(XtDisplay(drawArea), XtWindow(drawArea), 
      KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
      | Button1MotionMask );
*/
        
    clearBtn = XtVaCreateManagedWidget(
      "Clear",				/* widget name */
      xmPushButtonWidgetClass,		/* widget class */
      menu,			/* parent widget*/
      NULL);				/* terminate varargs list */

     
     quitBtn = XtVaCreateManagedWidget(
      "Quit",				/* widget name */
      xmPushButtonWidgetClass,		/* widget class */
      menu,			/* parent widget*/
      NULL);

     Colormap cmap = DefaultColormap(XtDisplay(drawArea), DefaultScreen(XtDisplay(drawArea)));
	if (
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "red", &redColor, &redColor) ||
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "green", &greenColor, &greenColor) ||
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "blue", &blueColor, &blueColor) ||
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "purple", &purpleColor, &purpleColor) ||
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "yellow", &yellowColor, &yellowColor) ||
		!XAllocNamedColor(XtDisplay(drawArea), cmap, "cyan", &cyanColor, &cyanColor)
	) {
		fprintf(stderr, "Can't alloc color!\n");
		return 1;
	}
	
	

    XmMainWindowSetAreas(mainWin, NULL, menu, NULL, NULL, frame);

    XtAddCallback(drawArea, XmNinputCallback, DrawLineCB, drawArea);
    XtAddEventHandler(drawArea, ButtonMotionMask, False, InputLineEH, NULL);
    XtAddCallback(drawArea, XmNexposeCallback, ExposeCB, drawArea);
    XtAddCallback(drawArea, XmNresizeCallback, ExposeCB, drawArea);

    XtAddCallback(clearBtn, XmNactivateCallback, ClearCB, drawArea);
    XtAddCallback(quitBtn, XmNactivateCallback, QuitCB, 0);

    XtRealizeWidget(topLevel);

    XtAppMainLoop(app_context);

    return 0;
}

