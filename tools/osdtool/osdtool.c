/*
   minimOSD-ng config tool v0.1

   Implemented (so far...)
   - Font uploading

   Todo:
   - Options editor
   - OSD layout editor
   - ...

*/

#include <stdio.h>
#include <malloc.h>
#include <gtk/gtk.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#define PORT "/dev/ttyUSB0"
#define PORT_SPEED B57600




/* unclear - osd is reseting but
   this should be double checked */
void setctrlpins(int fd, int on) 
{ 
#define TIOCM_CTS       0x020 
#define TIOCM_DTR       0x002 
#define TIOCM_DSR       0x100 
#define TIOCM_RTS       0x004
	int controlbits = TIOCM_RTS | TIOCM_CTS | TIOCM_DTR | TIOCM_DSR; 
	ioctl(fd, (on ? TIOCMBIS : TIOCMBIC), &controlbits); 
}


int strbin2int(char *buf)
{
	char *r;
	unsigned char i, b;

	r = strchr(buf, '\r');
	if (r != NULL)
		*r = 0;

	if ((buf[0] != '0') && (buf[0] != '1'))
		return -1;

	if (strlen(buf) < 8)
		return -2;

	for (i = 0; i < 8; i++)
		b |= (buf[i] & 0x01) * (1 << (7-i));

	return b;
}


static void upload_font_func(GtkWidget *widget, gpointer data)
{
	GtkWidget *filew = GTK_WIDGET(data);
	const gchar *font_filename;

	char rx[4];
	char *buf;
	int fsize;

	FILE *fp;
	int sp, ret;
	struct termios tty;

	unsigned int j, k;
	int i;
	unsigned char b, csum;
	char *tok;


	font_filename = gtk_file_selection_get_filename(
			GTK_FILE_SELECTION(filew));
	g_print ("Selected filename: %s\n", font_filename);

	fp = fopen(font_filename, "r");
	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buf = malloc((fsize + 2) * sizeof(char));
	fread(buf, sizeof(char), fsize, fp);
	fclose(fp);

	buf[fsize] = '\n';
	buf[fsize+1] = 0;

	sp = open(PORT, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (sp < 0) {
		g_print("Unable to open serial port!\n");
		return;
	}

	setctrlpins(sp, 1);
	usleep(100000);
	setctrlpins(sp, 0);

	memset(&tty, 0, sizeof tty);
	if (tcgetattr(sp, &tty) != 0) {
		//error_message ("error %d from tcgetattr", errno);
		g_print("Error from tcgetattr!\n");
		return;
	}

	tty.c_lflag = 0;
	tty.c_cflag = PORT_SPEED | CS8 | CLOCAL | CREAD;
	tty.c_oflag = 0;

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 5;

	tcflush(sp, TCIFLUSH);

	if (tcsetattr (sp, TCSANOW, &tty) != 0)
		g_print("Error in tcsetattr!\n");
	
	usleep(4000000);

	printf("Entering font mode... ");
	write(sp, "\xfa\xfa\xfa\xfa", 4);

	rx[0] = ' ';
	while ((rx[2] != '\n') && (rx[1] != 'K') && (rx[0] != 'O')) {
		ret = read(sp, &rx[3], 1);
		if (ret > 0) {
			rx[0] = rx[1];
			rx[1] = rx[2];
			rx[2] = rx[3];
			printf("read '%c':'%x' ...\n", rx[3], rx[3]);
		} else {
			usleep(10000);
		}
	}

	g_print("OK\n");

	j = 0;
	k = 0;
	csum = 0;
	tok = strtok(buf, "\n");
	while (tok != NULL) {
		i = strbin2int(tok);

		if (i >= 0) {
			b = (unsigned char) i;
			write(sp, &b, 1);
			csum += b;
			j ++;
		} else {
			printf("Ignored line '%s'\n", tok);
		}

		if (j == 64) {
			while (read(sp, &b, 1) != 1) usleep(1000);
			printf("Sending character %d (checksum %d)... ",
					k, csum);
			if (k != b) {
				g_print("Error sending font: expected character index=%2x, received index=%2x\n", k, b);
				return;
			}
			while (read(sp, &b, 1) != 1) usleep(1000);
			//printf("csum_calc=%d, csum_rx=%d\n", csum, b);

			if (csum != b) {
				g_print("Error sending font: expected checksum=%2x, received checksum=%2x\n", csum, b);
				return;
			} else {
				printf("OK\n");
			}

			j = 0;
			csum = 0;
			k ++;
		}
		tok = strtok(NULL, "\n");
	}

	printf("DONE\n");

	close(sp);
	free(buf);
}


static void upload_font_cbk(GtkWidget *widget, gpointer data)
{
	GtkWidget *filew;

	filew = gtk_file_selection_new("Select font file");

	g_signal_connect(GTK_FILE_SELECTION(filew)->ok_button,
                  "clicked", G_CALLBACK(upload_font_func), filew);

	/* ensure that the dialog is destroyed after clicking a button */
	g_signal_connect_swapped(GTK_FILE_SELECTION (filew)->ok_button,
	              "clicked", G_CALLBACK (gtk_widget_destroy), filew);

	/* connect cancel btton */
	g_signal_connect_swapped(GTK_FILE_SELECTION (filew)->cancel_button,
	              "clicked", G_CALLBACK (gtk_widget_destroy), filew);

	gtk_widget_show(filew);
}


static void destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}


GtkWidget* create_menu(GtkWidget *window)
{
	GtkWidget *menu_bar, *file_menu;
	GtkWidget *menu_item;

	file_menu = gtk_menu_new();

	/* upload font */
	menu_item = gtk_menu_item_new_with_label("Upload font...");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item);
	gtk_widget_show(menu_item);

	g_signal_connect_swapped(menu_item, "activate",
			G_CALLBACK (upload_font_cbk), NULL);

	/* exit */
	menu_item = gtk_menu_item_new_with_label("Exit");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), menu_item);
	gtk_widget_show(menu_item);

	g_signal_connect_swapped(menu_item, "activate",
			G_CALLBACK (gtk_widget_destroy), window);

	/* main 'File' menu item */
	menu_item = gtk_menu_item_new_with_label("File");
	gtk_widget_show(menu_item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), file_menu);

	/* create the menu bar widget */
	menu_bar = gtk_menu_bar_new();
	gtk_menu_shell_append(GTK_MENU_SHELL (menu_bar), menu_item);

	return menu_bar;
}


int main(int argc, char *argv[])
{
	/* GtkWidget is the storage type for widgets */
	GtkWidget *window;
	GtkWidget *button;

	GtkWidget *menu_bar;
	GtkWidget *vbox;

	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (GTK_WIDGET (window), 200, 100);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	menu_bar = create_menu(window);
	gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 2);
	gtk_widget_show(menu_bar);


	g_signal_connect(window, "destroy", G_CALLBACK (destroy), NULL);

	/* exit button */
	button = gtk_button_new_with_label("Exit");

	g_signal_connect_swapped(button, "clicked",
			G_CALLBACK (gtk_widget_destroy), window);

	gtk_box_pack_end(GTK_BOX(vbox), button, TRUE, TRUE, 2);

	gtk_widget_show (button);
	gtk_widget_show (window);

	gtk_main ();
	return 0;
}

