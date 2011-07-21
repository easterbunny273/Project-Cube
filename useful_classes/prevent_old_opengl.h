/*
 To prevent using of old gl-commands, some #defines lead to compiler errors if these functions are used
 */


#ifndef __HEADER_PREVENT_OLD_FUNCTIONS
#define __HEADER_PREVENT_OLD_FUNCTIONS

#define glLoadIdentity glLoadIdentity_NOT_ALLOWED
#define glLoadMatrix glLoadMatrix_NOT_ALLOWED
#define glMatrixMode glMatrixMode_NOT_ALLOWED
#define glMultMatrix glMultMatrix_NOT_ALLOWED

#define glTranslate glTranslate_NOT_ALLOWED
#define glRotate glRotate_NOT_ALLOWED
#define glScale glScale_NOT_ALLOWED

#define glBegin glBegin_NOT_ALLOWED
#define glEnd glEnd_NOT_ALLOWED
#define glVertex3 glVertex3_NOT_ALLOWED
#define glColor3 glColor3_NOT_ALLOWED

#define glVertexPointer glVertexPointer_NOT_ALLOWED
#define glNormalPointer glNormalPointer_NOT_ALLOWED

#define glCallList glCallList_NOT_ALLOWED

#endif
