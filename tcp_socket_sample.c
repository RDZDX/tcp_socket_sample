#include "tcp_socket_sample.h"

VMUINT8 *buffer;
VMUINT16 color2 = VM_COLOR_BLACK;
VMUINT16 color1 = VM_COLOR_WHITE;
VMINT sHeight;
VMINT sWidth;
VMINT xBaseline;
VMINT xHeight;
VMINT drv;
VMWCHAR cZZZX[101];
VMWCHAR cZZZQ[101];
VMCHAR zZZZ[101] = "WWp";
VMINT fontSize = VM_SMALL_FONT;
VMINT filledDsplByLines = 0;
VMBOOL flightMode = VM_FALSE;
VMBOOL startup = VM_FALSE;
VMBOOL missingConfigFile = VM_TRUE;

VMCHAR ip[100] = {};
VMCHAR portx[100] = {};
VMCHAR login[100] = {};
VMCHAR password[100] = {};
VMCHAR command[100] = {};

VMCHAR tcp_url[50] = "google.com";
VMINT tcp_port = 80;


void vm_main(void) 
{
	g_layer_hdl[0] = -1;
	vm_reg_sysevt_callback(mre_handle_sysevt);
	vm_reg_keyboard_callback(mre_handle_keyevt);
        vm_font_set_font_size(fontSize);

        sWidth = vm_graphic_get_screen_width();
        sHeight = vm_graphic_get_screen_height();
        vm_ascii_to_ucs2(cZZZQ, (strlen(zZZZ) + 1) * 2, zZZZ);
        xHeight = vm_graphic_get_string_height(cZZZQ);        // Max Font height 16
        //vm_graphic_get_character_height() = xHeight
        xBaseline = vm_graphic_get_string_baseline(cZZZQ);    // Font baseline 12
        drv = mre_get_drv();
        if (vm_sim_card_count() == 99) {flightMode = VM_TRUE;}
        checkFileExist();

}

void mre_handle_sysevt(VMINT message, VMINT param) 
{

#ifdef		SUPPORT_BG	
	switch (message) 
	{
		case VM_MSG_CREATE:
			break;
		case VM_MSG_PAINT:	
			g_layer_hdl[0] = vm_graphic_create_layer(0, 0, sWidth, sHeight, -1);
		        vm_graphic_set_clip(0, 0, sWidth, sHeight);
                        buffer = vm_graphic_get_layer_buffer(g_layer_hdl[0]);
                        vm_switch_power_saving_mode(turn_off_mode);
                        if (flightMode == VM_FALSE) {
			   mre_display_home_top_screen_ascii("Press 1: to connect");
                        } else {
                           mre_display_home_top_screen_ascii("Swich Flight mode OFF");
                        }
			break;
		case VM_MSG_HIDE:
		case VM_MSG_QUIT:
			if ( g_layer_hdl[0] != -1 )
			{
				vm_graphic_delete_layer(g_layer_hdl[0]);
				g_layer_hdl[0] = -1;
			}
			break;
	}
#else
	switch (message) 
	{
		case VM_MSG_CREATE:
		case VM_MSG_ACTIVE:
			break;
		case VM_MSG_PAINT:	
                        vm_switch_power_saving_mode(turn_off_mode);		
			break;
		case VM_MSG_INACTIVE:
		case VM_MSG_QUIT:
			if ( g_layer_hdl[0] != -1 )
				vm_graphic_delete_layer(g_layer_hdl[0]);
			break;	
	}
#endif
}

void mre_handle_keyevt(VMINT event, VMINT keycode) 
{
    if (event==VM_KEY_EVENT_UP)
	{
		switch (keycode)
		{
			case VM_KEY_NUM1 :
				{				
                    mre_display_menu_items("Select connection types:", "1. VM_TCP_APN_CMNET", "2. VM_TCP_APN_CMWAP");
                                        //draw_exit();
					vm_reg_keyboard_callback(mre_menu_handle_keyevt);
                    break;
				}
			case VM_KEY_RIGHT_SOFTKEY :
				{
				    mre_exit_application();
					break;
				}
 			default :
				{
					break;
				}					
		}
	}
}

void mre_menu_handle_keyevt(VMINT event, VMINT keycode) 
{

VMCHAR tcp_portx[50];

    sprintf(tcp_portx, "%s %d", tcp_url, tcp_port);

    if (event==VM_KEY_EVENT_UP)
	{
		switch (keycode)
		{
			case VM_KEY_NUM1 :
				{				
				   //CMNET

                        filledDsplByLines = 0;
                        vertical_scrolling_text(tcp_portx);
                        vertical_scrolling_text("Connecting...");
                        mre_tcp_connection(VM_TCP_APN_CMNET);
                    	vm_reg_keyboard_callback(mre_handle_keyevt);
                   
					break;
				}
			case VM_KEY_NUM2 :
				{
					//CMWAP

                        filledDsplByLines = 0;
                        vertical_scrolling_text(tcp_portx);
                        vertical_scrolling_text("Connecting...");
                        mre_tcp_connection(VM_TCP_APN_CMWAP);
                    	vm_reg_keyboard_callback(mre_handle_keyevt);

					break;
				}

			case VM_KEY_RIGHT_SOFTKEY :
				{
				    mre_exit_application();
					break;
				}

 			default :
				{
					break;
				}					
		}
	}
}

static void mre_display_home_top_screen_ascii(VMCHAR *text)
{
	VMWCHAR string[150];	
	int x_coordinate, y_coordinate, width;

	vm_gb2312_to_ucs2(string, 150, text);
	width = vm_graphic_get_string_width(string);
	x_coordinate = (sWidth - width) / 2;
	y_coordinate = (sHeight - xHeight) / 2;
	vm_graphic_clear_layer_bg(g_layer_hdl[0]);
	vm_graphic_fill_rect(buffer, 0, 0, sWidth, sHeight, color2, color2);
	vm_graphic_textout(buffer, x_coordinate, y_coordinate, string, wstrlen(string), color1);
	//vm_graphic_flush_layer(g_layer_hdl, 1);
        draw_exit();
}

static void mre_exit_application(void)
{
	vm_graphic_delete_layer(g_layer_hdl[0]);
	g_layer_hdl[0] = -1;
	vm_exit_app();

}

static void mre_tcp_connection(VMINT apn)
{
	char   *host;
	//VMINT port = MRE_TCP_CONNECTION_PORT;
	host = vm_malloc(100);
	memset(host, 0, 100);
	//strcpy(host, url);
	strcpy(host, tcp_url);

	//connect_status = vm_tcp_connect(host, port, apn, mre_tcp_callback_application); 
	connect_status = vm_tcp_connect(host, tcp_port, apn, mre_tcp_callback_application); 

}

void mre_tcp_callback_application(VMINT handle, VMINT event)
{
	VMINT length;
	VMCHAR * layer_buffer;
	VMCHAR string[100]; //
	//VMCHAR string[]="GET http://google.com HTTP/1.1\r\nHost: google.com\r\n\r\n";
	VMINT count_bytes;
	layer_buffer = vm_malloc(500);

     sprintf(string, "%s%s%s%s%s", "GET http://", tcp_url, " HTTP/1.1\r\nHost: ", tcp_url, "\r\n\r\n"); //

     switch (event)
	{
		case VM_TCP_EVT_HOST_NOT_FOUND : 
			{
                                vertical_scrolling_text("Host not found");
                                if (filledDsplByLines != (sHeight / xHeight)) {draw_exit();}
				break;
			}
		case VM_TCP_EVT_CONNECTED : 
            { 
                length=strlen(string);
				count_bytes = vm_tcp_write(handle, string, length);
                                vertical_scrolling_text("Connected !");
				mre_create_file_for_write();
				break;
			}
		case VM_TCP_EVT_CAN_WRITE :
			{ 
			    //vm_tcp_write(handle, layer_buffer, length);
				break;
			}
		case VM_TCP_EVT_CAN_READ :
			{
				count_bytes = vm_tcp_read (handle, layer_buffer, sizeof(layer_buffer) - 1);
				if (count_bytes > 0)
				{
					mre_dump_to_file(layer_buffer, count_bytes);
                                        vertical_scrolling_text("Response dumped in file");
					memset(layer_buffer, 0, 500);
				}
				while (count_bytes > 0)
				{
					count_bytes = vm_tcp_read (handle, layer_buffer, sizeof(layer_buffer) - 1);
					mre_dump_to_file(layer_buffer, count_bytes);
                                        vertical_scrolling_text("Response dumped in file");
					memset(layer_buffer, 0, 500);
				}
                                vertical_scrolling_text("Response Read");
                                if (filledDsplByLines != (sHeight / xHeight)) {draw_exit();}
				break;
			}
		case VM_TCP_EVT_PIPE_BROKEN :
			{
                                if (filledDsplByLines != (sHeight / xHeight)) {draw_exit();}
				break;
			}
		case VM_TCP_EVT_PIPE_CLOSED :
			{
				vm_tcp_close(connect_status);
                                if (filledDsplByLines != (sHeight / xHeight)) {draw_exit();}
				break;
			}
		default:
			{
				vm_tcp_close(connect_status);
                                if (filledDsplByLines != (sHeight / xHeight)) {draw_exit();}

			}
	}
	vm_free(layer_buffer);
	layer_buffer = NULL;
}

void mre_dump_to_file(VMCHAR *tcp_buffer, VMINT size)
{
    VMFILE  handle;
	VMWCHAR wfile_name[30];
	VMCHAR  file_name[2 * 30];
	VMUINT length;
	VMINT write_status;

	//sprintf(file_name, "%c:\\%s", vm_get_system_driver(), "TCP.txt");
	//sprintf(file_name, "%c:\\%s", mre_get_drv(), "TCP.txt");
	sprintf(file_name, "%c:\\%s", drv, "TCP.txt");
	vm_ascii_to_ucs2(wfile_name, 100, file_name);
	handle = vm_file_open(wfile_name, MODE_APPEND, TRUE);
	if(handle < 0)
	{
                vertical_scrolling_text("File Handle Fail");
	}

	if (handle)
    {
   
	write_status =  vm_file_write(handle, tcp_buffer, size, &length);

	if (write_status < 0)
	{
	}
    }

	vm_file_close(handle);
}

static void mre_display_menu_items(VMCHAR string_1[150], VMCHAR string_2[150], VMCHAR string_3[150]) 
{

	VMWCHAR item_string[3][150];
	VMINT   x_boundary_width = 20;
	VMINT   display_string_row_height = 20;	
	VMINT   item_position_x[3], item_position_y[3], item_string_width[3];
	
	vm_gb2312_to_ucs2(item_string[0], 150, string_1);
	vm_gb2312_to_ucs2(item_string[1], 150, string_2);
	vm_gb2312_to_ucs2(item_string[2], 150, string_3);
	item_string_width[0] = vm_graphic_get_string_width(item_string[0]);
	item_string_width[1] = vm_graphic_get_string_width(item_string[1]);
	item_string_width[2] = vm_graphic_get_string_width(item_string[2]);
	item_position_x[0] = x_boundary_width;
	item_position_x[1] = x_boundary_width;
	item_position_x[2] = x_boundary_width;
	item_position_y[0] = display_string_row_height;	
	item_position_y[1] = display_string_row_height * 3 ;
	item_position_y[2] = display_string_row_height * 4 ;
	vm_graphic_clear_layer_bg(g_layer_hdl[0]);
	vm_graphic_fill_rect(buffer, 0, 0, sWidth, sHeight-20, color2, color2);
	vm_graphic_textout(buffer, item_position_x[0], item_position_y[0], item_string[0], wstrlen(item_string[0]), color1);
        vm_graphic_textout(buffer, item_position_x[1], item_position_y[1], item_string[1], wstrlen(item_string[1]), color1);
	vm_graphic_textout(buffer, item_position_x[2], item_position_y[2], item_string[2], wstrlen(item_string[2]), color1);
	//vm_graphic_flush_layer(g_layer_hdl, 1);
        draw_exit();
}

static void mre_create_file_for_write(void)
{
	VMFILE  handle;
	VMWCHAR wfile_name[30];
	VMCHAR  file_name[2 * 30];

   	//sprintf(file_name, "%c:\\%s", vm_get_system_driver(), "TCP.txt");
	//sprintf(file_name, "%c:\\%s", mre_get_drv(), "TCP.txt");
	sprintf(file_name, "%c:\\%s", drv, "TCP.txt");
	vm_ascii_to_ucs2(wfile_name, 100, file_name);
	handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, TRUE);
	if(handle < 0)
	{
                vertical_scrolling_text("File Handle Fail");
	}
	vm_file_close(handle);
}

void vertical_scrolling_text(VMSTR ascii_string)
{

    if (filledDsplByLines > sHeight || filledDsplByLines == 0) {
        filledDsplByLines = 0;
        vm_graphic_fill_rect(buffer, 0, 0, sWidth, sHeight, color2, color2);
    }

    draw_http_result_string(0, filledDsplByLines, ascii_string);
    filledDsplByLines += xHeight;

}


static void draw_http_result_string(int display_string_x_pos, int display_string_y_pos, VMSTR ascii_string)
{
	VMWSTR display_string;
	VMINT size;
	size = (strlen(ascii_string) + 1) * 2;
	display_string = vm_malloc(size);
        vm_graphic_clear_layer_bg (g_layer_hdl[0]);
	vm_ascii_to_ucs2(display_string, size,  ascii_string);
        vm_graphic_textout(buffer, display_string_x_pos, display_string_y_pos, display_string, wstrlen(display_string), color1);
	vm_graphic_flush_layer(g_layer_hdl, 1);
	vm_free(display_string);
}


static void draw_menu(void)
{
	VMWCHAR display_string[100];
	int display_string_x_pos, display_string_y_pos, display_string_width;
	vm_gb2312_to_ucs2(display_string, 100, "Press 1  :HTTP Connection");
	display_string_width = vm_graphic_get_string_width(display_string);
	display_string_x_pos = (sWidth - display_string_width) / 2;
	display_string_y_pos = (sHeight - xHeight) / 2;
	vm_graphic_fill_rect(buffer, 0, 0, sWidth, sHeight, color1, color2);
	vm_graphic_textout(buffer, display_string_x_pos, display_string_y_pos, display_string, wstrlen(display_string), color1);
	vm_graphic_flush_layer(g_layer_hdl, 1);
}

static void draw_exit(void) {

	VMWSTR exit_str;
	int size, display_string_x_pos, display_string_y_pos;
	display_string_x_pos = sWidth - 3 * 20; 
	//display_string_y_pos = sHeight - 20; 
	display_string_y_pos = sHeight - xHeight - 1; 
	size = (strlen("Exit") + 1) * 2;
	exit_str = vm_malloc(size);
	vm_ascii_to_ucs2(exit_str, size,  "Exit");
        vm_graphic_textout_by_baseline(buffer, display_string_x_pos, display_string_y_pos, exit_str, wstrlen(exit_str), color1, xBaseline);
	vm_graphic_flush_layer(g_layer_hdl, 1);
	vm_free(exit_str);
}

VMINT mre_get_drv (void)
{
    VMINT drv;
    if ( (drv = vm_get_removable_driver ()) <0 )
    {
        drv = vm_get_system_driver ();
    }
    //g_mre_drv = drv;
    //return g_mre_drv;
    return drv;
}

void create_app_txt_path(VMWSTR text, VMSTR extt) {

    VMWCHAR fullPath[100];
    VMWCHAR wfile_extension[4];

    vm_get_exec_filename(fullPath);
    vm_ascii_to_ucs2(wfile_extension, 8, extt);
    vm_wstrncpy(text, fullPath, vm_wstrlen(fullPath) - 3);
    vm_wstrcat(text, wfile_extension);

}

void checkFileExist(void) {

    VMFILE f_read;
    VMUINT nread;
    VMWCHAR file_pathw[100];
    VMCHAR new_data[500];

    create_app_txt_path(file_pathw, "txt");

    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {

       vm_file_close(f_read);
       missingConfigFile = VM_TRUE;

    } else {

      vm_file_read(f_read, new_data, 500, &nread);
      new_data[nread] = '\0';
      vm_file_close(f_read);

      if (strlen(new_data) > 1) {

         parseText(new_data);
         missingConfigFile = VM_FALSE;

      } else {

        missingConfigFile = VM_TRUE;

      }

    }
}

VMINT parseText(VMSTR text) {

    VMCHAR vns_simbl[2] = {};
    VMCHAR nauj_strng[100] = {};
    VMINT counter = 0;
    VMINT counter1 = 0;
    VMCHAR *ptr;

    ptr = text;

    while (*ptr != '\0' || counter1 == 5) {

           if (*ptr == '\r') {ptr++;}
           if (*ptr == '\n') {

              counter = counter + 1;

              if (counter == 1) {strcpy(ip, nauj_strng);}
              if (counter == 2) {strcpy(portx, nauj_strng);}
              //if (counter == 3) {port = strtoi(nauj_strng);}
              if (counter == 3) {strcpy(login, nauj_strng);}
              if (counter == 4) {strcpy(password, nauj_strng);}
              if (counter == 5) {strcpy(command, nauj_strng);}

              counter1 = counter;

              strcpy(nauj_strng, "");
              ptr++;
           }

           sprintf(vns_simbl, "%c", *ptr);
           strcat(nauj_strng, vns_simbl);
    ptr++;

    }

    if (counter == 0) {strcpy(ip, nauj_strng);}
    if (counter == 1) {strcpy(portx, nauj_strng);}
    //if (counter == 2) {port = strtoi(nauj_strng);}
    if (counter == 2) {strcpy(login, nauj_strng);}
    if (counter == 3) {strcpy(password, nauj_strng);}
    if (counter == 4) {strcpy(command, nauj_strng);}

    //if (strlen(ip) == 1 || strlen(portx) == 0) { missingConfigFile = VM_TRUE;}

    if ( strlen(ip) != 0 ) { strcpy(tcp_url, ip); }
    if ( strlen(portx) != 0 ) { tcp_port = strtoi(portx); }

    return 0;
}