#include <QtWidgets>

#include "kss_clock.hxx"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KSSClock clock(0.9);
	clock.showFullScreen();
	return a.exec();
}
