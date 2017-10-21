// kss_clock.cxx
//
// Copyright (C) 2010, 2011, 2017  Melchior FRANZ  <melchior.franz@ginzinger.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
#include <QApplication>
#include <QImage>
#include <QKeyEvent>
#include <QPainter>
#include <QTime>

#include "kss_clock.hxx"


const int TIMER_INTERVAL = 100;


class ClockPainter : public QPainter {
public:
	ClockPainter(QPaintDevice *device, int diameter);
	void drawTick(double angle, double from, double to, double width,
			const QColor &, bool shadow = false);
	void drawDisc(double width, const QColor &, bool shadow = false);
	void drawHand(double angle, double length, double width,
			const QColor &, bool disc = true);
	void drawScale(const QColor &);
};


ClockPainter::ClockPainter(QPaintDevice *device, int diameter) :
	QPainter(device)
{
	setRenderHint(QPainter::Antialiasing);
	translate(diameter / 2.0, diameter / 2.0);
	scale(diameter / 2000.0, -diameter / 2000.0);
	setPen(Qt::NoPen);
}


void ClockPainter::drawTick(double angle, double from, double to, double width,
		const QColor &color, bool shadow)
{
	save();
	rotate(90.0 - angle);

	if (shadow) {
		width += 1.0;
		setBrush(QColor(100, 100, 100));
	} else {
		setBrush(color);
	}
	drawRect(QRectF(from, -width / 2.0, to - from, width));
	restore();
}


void ClockPainter::drawDisc(double width, const QColor &color, bool shadow)
{
	if (shadow) {
		width += 1.0;
		setBrush(QColor(100, 100, 100));
	} else {
		setBrush(color);
	}
	drawEllipse(QRectF(-width, -width, 2.0 * width, 2.0 * width));
}


void ClockPainter::drawHand(double angle, double length, double width,
		const QColor &color, bool disc)
{
	if (disc)
		drawDisc(width * 1.3, color, true);
	drawTick(angle, 0.0, length, width, color, true);

	if (disc)
		drawDisc(width * 1.3, color, false);
	drawTick(angle, 0.0, length, width, color, false);

}


void ClockPainter::drawScale(const QColor &color)
{
	for (int i = 0; i < 360; i += 6)
		if (i % 30)
			drawTick(i, 920.0, 980.0, 15.0, color);
		else
			drawTick(i, 825.0, 980.0, 40.0, color);
}


//-----------------------------------------------------------------------------


KSSClock::KSSClock(float scale, QWidget *parent) :
	QWidget(parent),
	_timer(this),
	_bgndColor(Qt::black),
	_scaleColor(Qt::white),
	_hourColor(Qt::white),
	_minColor(Qt::white),
	_secColor(Qt::red),
	_scale(scale),
	_hour(-1),
	_minute(-1),
	_second(-1)
{
	Q_ASSERT(scale > 0.1);
	setWindowState(windowState() | Qt::WindowFullScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setMinimumSize(100, 100);

	QPalette p = palette();
	p.setColor(backgroundRole(), _bgndColor);
	setPalette(p);

	slotTimeout();
	connect(&_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
	_timer.start(TIMER_INTERVAL);
	show();
}


void KSSClock::slotTimeout()
{
	QTime t = QTime::currentTime();
	int s = t.second();
	if (s == _second)
		return;

	_second = _secColor != _bgndColor ? s : 0;
	_hour = t.hour();
	_minute = t.minute();
	update();
}


void KSSClock::paintEvent(QPaintEvent *)
{
	double hour_angle = _hour * 30.0 + _minute * .5 + _second / 120.0;
	double minute_angle = _minute * 6.0 + _second * .1;
	double second_angle = _second * 6.0;
	QImage clock(_diameter, _diameter, QImage::Format_RGB32);
	ClockPainter c(&clock, _diameter);
	c.fillRect(-1000, -1000, 2000, 2000, _bgndColor);

	if (_scaleColor != _bgndColor)
		c.drawScale(_scaleColor);
	if (_hourColor != _bgndColor)
		c.drawHand(hour_angle, 600.0, 55.0, _hourColor, false);
	if (_minColor != _bgndColor)
		c.drawHand(minute_angle, 900.0, 40.0, _minColor);
	if (_secColor != _bgndColor)
		c.drawHand(second_angle, 900.0, 30.0, _secColor);

	QPainter p(this);
	p.drawImage(_x, _y, clock);
	p.eraseRect(0, 0, _x, height());                                        // left ver
	p.eraseRect(_x + _diameter, 0, width(), height());                      // right ver
	p.eraseRect(_x, 0, _diameter, _y);                                      // top hor
	p.eraseRect(_x, _y + _diameter, _diameter, height() - _y - _diameter);  // bottom hor
}


void KSSClock::resizeEvent(QResizeEvent *)
{
	_diameter = qMin(width(), height()) * _scale + 0.5;
	_x = (width() - _diameter) / 2;
	_y = (height() - _diameter) / 2;
}


void KSSClock::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		qApp->quit();
		break;

	case Qt::Key_C:
	case Qt::Key_Q:
		if (event->modifiers() & Qt::ControlModifier) {
			qApp->quit();
			return;
		}
		break;

	default:
		break;
	}

	QWidget::keyPressEvent(event);
}
