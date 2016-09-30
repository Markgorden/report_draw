// Copyright (c) 1998 Matt Clay, all rights reserved.
//

#ifndef UMRESOURCES_H
#define UMRESOURCES_H

// You can change the id ranges by changing
// UM_BASE_STRING, UM_BASE_COMMAND, or
// UM_COMMON_COMMAND

#define UM_BASE_STRING					10000

#define IDS_TYPING_ACTION               UM_BASE_STRING
#define IDS_CUT_ACTION                  (UM_BASE_STRING+20)
#define IDS_PASTE_ACTION                (UM_BASE_STRING+21)
#define IDS_PASTESPECIAL_ACTION         (UM_BASE_STRING+22)
#define IDS_CLEAR_ACTION                (UM_BASE_STRING+23)
#define IDS_DELETE_ACTION               (UM_BASE_STRING+24)
#define IDS_REPLACE_ACTION				(UM_BASE_STRING+25)
#define IDS_REPLACEALL_ACTION           (UM_BASE_STRING+26)
#define IDS_STYLECHANGE_ACTION          (UM_BASE_STRING+35)
#define IDS_BOLD_ACTION                 (UM_BASE_STRING+40)
#define IDS_ITALICS_ACTION              (UM_BASE_STRING+41)
#define IDS_UNDERLINE_ACTION            (UM_BASE_STRING+42)
#define IDS_COLORCHANGE_ACTION          (UM_BASE_STRING+55)
#define IDS_DEFAULTCOLOR_ACTION         (UM_BASE_STRING+56)
#define IDS_SIZECHANGE_ACTION           (UM_BASE_STRING+80)
#define IDS_JUSTIFICATION_ACTION        (UM_BASE_STRING+95)
#define IDS_LEFTJUSTIFICATION_ACTION    (UM_BASE_STRING+100)
#define IDS_CENTERJUSTIFICATION_ACTION  (UM_BASE_STRING+101)
#define IDS_RIGHTJUSTIFICATION_ACTION   (UM_BASE_STRING+102)
#define IDS_BULLETS_ACTION              (UM_BASE_STRING+120)
#define IDS_FORMATTING_ACTION           (UM_BASE_STRING+135)
#define IDS_FONTFORMATTING_ACTION       (UM_BASE_STRING+140)
#define IDS_PARAGRAPHFORMATTING_ACTION  (UM_BASE_STRING+141)
#define IDS_TABSFORMATTING_ACTION       (UM_BASE_STRING+142)
#define IDS_DATETIME_ACTION             (UM_BASE_STRING+160)
#define IDS_INSERTOBJECT_ACTION         (UM_BASE_STRING+180)
#define IDS_MOVE_ACTION                 (UM_BASE_STRING+200)
#define IDS_DUPLICATE_ACTION            (UM_BASE_STRING+201)

#define UM_LAST_STRING					IDS_DUPLICATE_ACTION


// For multiple undo/redo by the user.

#define UM_BASE_COMMAND					45056

#define ID_UNDO_0						(UM_BASE_COMMAND+220)
#define ID_UNDO_1						(UM_BASE_COMMAND+221)
#define ID_UNDO_2						(UM_BASE_COMMAND+222)
#define ID_UNDO_3						(UM_BASE_COMMAND+223)
#define ID_UNDO_4						(UM_BASE_COMMAND+224)
#define ID_UNDO_5						(UM_BASE_COMMAND+225)
#define ID_UNDO_6						(UM_BASE_COMMAND+226)
#define ID_UNDO_7						(UM_BASE_COMMAND+227)
#define ID_UNDO_8						(UM_BASE_COMMAND+228)
#define ID_UNDO_9						(UM_BASE_COMMAND+229)
#define LAST_UNDO_ID					ID_UNDO_9

#define ID_REDO_0						(UM_BASE_COMMAND+230)
#define ID_REDO_1						(UM_BASE_COMMAND+231)
#define ID_REDO_2						(UM_BASE_COMMAND+232)
#define ID_REDO_3						(UM_BASE_COMMAND+233)
#define ID_REDO_4						(UM_BASE_COMMAND+234)
#define ID_REDO_5						(UM_BASE_COMMAND+235)
#define ID_REDO_6						(UM_BASE_COMMAND+236)
#define ID_REDO_7						(UM_BASE_COMMAND+237)
#define ID_REDO_8						(UM_BASE_COMMAND+238)
#define ID_REDO_9						(UM_BASE_COMMAND+239)
#define LAST_REDO_ID					ID_REDO_9

#define UM_LAST_COMMAND					LAST_REDO_ID

// The ClassWizard seems to define these but they
// are still considered to be user-defined commands.

#define UM_COMMON_COMMAND					32780

#ifndef ID_FORMAT_PARAGRAPH
	#define ID_FORMAT_PARAGRAPH             UM_COMMON_COMMAND
#endif

#ifndef ID_FORMAT_TABS
	#define ID_FORMAT_TABS					(UM_COMMON_COMMAND+1)
#endif

#ifndef ID_COLOR0
	#define ID_COLOR0                       (UM_COMMON_COMMAND+2)
	#define ID_COLOR1                       (UM_COMMON_COMMAND+3)
	#define ID_COLOR2                       (UM_COMMON_COMMAND+4)
	#define ID_COLOR3                       (UM_COMMON_COMMAND+5)
	#define ID_COLOR4                       (UM_COMMON_COMMAND+6)
	#define ID_COLOR5                       (UM_COMMON_COMMAND+7)
	#define ID_COLOR6                       (UM_COMMON_COMMAND+8)
	#define ID_COLOR7                       (UM_COMMON_COMMAND+9)
	#define ID_COLOR8                       (UM_COMMON_COMMAND+10)
	#define ID_COLOR9                       (UM_COMMON_COMMAND+11)
	#define ID_COLOR10                      (UM_COMMON_COMMAND+12)
	#define ID_COLOR11                      (UM_COMMON_COMMAND+13)
	#define ID_COLOR12                      (UM_COMMON_COMMAND+14)
	#define ID_COLOR13                      (UM_COMMON_COMMAND+15)
	#define ID_COLOR14                      (UM_COMMON_COMMAND+16)
	#define ID_COLOR15                      (UM_COMMON_COMMAND+17)
	#define ID_COLOR16                      (UM_COMMON_COMMAND+18)
#endif

#ifndef ID_CHAR_BOLD
	#define ID_CHAR_BOLD                    (UM_COMMON_COMMAND+19)
	#define ID_CHAR_ITALIC                  (UM_COMMON_COMMAND+20)
	#define ID_CHAR_UNDERLINE               (UM_COMMON_COMMAND+21)
#endif

#ifndef ID_PARA_LEFT
	#define ID_PARA_LEFT                    (UM_COMMON_COMMAND+23)
	#define ID_PARA_CENTER                  (UM_COMMON_COMMAND+24)
	#define ID_PARA_RIGHT                   (UM_COMMON_COMMAND+25)
#endif

#ifndef ID_PEN_PERIOD
	#define ID_PEN_PERIOD                   (UM_COMMON_COMMAND+26)
#endif

#endif
