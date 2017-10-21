// kss_clock.hxx
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
#ifndef KSS_CLOCK_HXX
#define KSS_CLOCK_HXX

#include <QTimer>
#include <QColor>
#include <QWidget>


class KSSClock : public QWidget {
	Q_OBJECT
public:
	explicit KSSClock(float scale = 0.85, QWidget *parent = nullptr);
	virtual ~KSSClock() {}

public slots:
	void setCurrentTime() const {}

private:
	QTimer _timer;
	QColor _bgndColor;
	QColor _scaleColor;
	QColor _hourColor;
	QColor _minColor;
	QColor _secColor;
	float _scale;
	int _x;
	int _y;
	int _diameter;
	int _hour;
	int _minute;
	int _second;

private slots:
	void paintEvent(QPaintEvent *) override;
	virtual void resizeEvent(QResizeEvent *) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void slotTimeout();
};
#endif // KSS_CLOCK_HXX
