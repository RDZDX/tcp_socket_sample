#ifndef _MRE_APP_WIZARDTEMPLATE_
#define	_MRE_APP_WIZARDTEMPLATE_

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmsock.h"
#include "string.h"
#include "stdio.h"

#define		SUPPORT_BG
#define 	MRE_LOG_ON

//VMINT		g_layer_hdl[2];
VMINT		g_layer_hdl[1];
VMINT		connect_status  = -1;

void mre_handle_sysevt(VMINT message, VMINT param);
void mre_handle_keyevt(VMINT event, VMINT keycode);
static void mre_display_home_top_screen_ascii(VMCHAR *text);
static void mre_exit_application(void);
static void mre_tcp_connection(VMINT apn);
void mre_tcp_callback_application(VMINT handle, VMINT event);
void mre_dump_to_file(VMCHAR *tcp_buffer, VMINT size);
static void mre_display_menu_items(VMCHAR string_1[150], VMCHAR string_2[150], VMCHAR string_3[150]) ;
void mre_menu_handle_keyevt(VMINT event, VMINT keycode);
static void mre_create_file_for_write(void);
void vertical_scrolling_text(VMSTR ascii_string);
static void draw_http_result_string(int display_string_x_pos, int display_string_y_pos, VMSTR ascii_string);
static void draw_menu(void);
static void draw_exit(void);
VMINT mre_get_drv (void);
void create_app_txt_path(VMWSTR text, VMSTR extt);
void checkFileExist(void);
VMINT parseText(VMSTR text);

#endif