#include "gtk_common.h"
#include "window.h"

int main(int argc, char *argv[]) {

	window_t *win;
	gtk_init(&argc, &argv);
	
	win = create_window("MainWindow.glade");
	show_window(win);

	gtk_main();
	destroy_window(win);
	return 0;
}
