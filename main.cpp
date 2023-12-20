#include "application.h"

int main() {
	Application& app = Application::get_instance();
	app.init();
	app.start();
	return 0;
}