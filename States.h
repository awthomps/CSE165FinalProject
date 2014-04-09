#ifndef _STATES_H_
#define _STATES_H_

/*
#define MAIN_MENU_S 0
#define AVATAR_EDITOR_S 1
#define GAME_S 2
#define GAME_MENU_S 3
*/

//mega states
#define MAIN_MENU_S 0x00000000
#define AVATAR_EDITOR_S 0x10000000
#define GAME_S 0x20000000
#define GAME_MENU_S 0x30000000

//Masks to get mega/sub states
#define MEGA_MASK 0xF0000000
#define SUB_MASK 0x0FFFFFFF

//sub states
#define AVATAR_EDITOR_MAIN 0x10000001
#define AVATAR_EDITOR_CREATE_SHAPE 0x10000002
#define AVATAR_EDITOR_SHAPE_BOX 0x10000003
#define AVATAR_EDITOR_SHAPE_CYLINDER 0x10000004
#define AVATAR_EDITOR_SHAPE_CONE 0x10000005
#define AVATAR_EDITOR_SHAPE_SPHERE 0x10000006
#define AVATAR_EDITOR_CHOOSE_SHAPE 0x10000007
#define AVATAR_EDITOR_MODIFY_SHAPE_SIZE 0x10000008
#define AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE 0x10000009
#define AVATAR_EDITOR_MODIFY_SHAPE_COLOR 0x1000000A
#define AVATAR_EDITOR_CREATE_SHAPE_PROLOGUE 0x1000000B
#define AVATAR_EDITOR_MODIFY_SHAPE_ROTATE 0x1000000C
#define AVATAR_EDITOR_MODIFY_SHAPE_PLACE 0x1000000D
#define AVATAR_EDITOR_CHANGE_SHAPE_PROLOGUE 0x1000000E
#define AVATAR_EDITOR_ROTATE_SHAPE_PROLOGUE 0x1000000F
#define AVATAR_EDITOR_ROTATE_SHAPE 0x10000010

#endif