#pragma once
#include <QtCore/qglobal.h>

class SystemInfo {
public:
	SystemInfo();
	~SystemInfo();

	static double cpuLoad(bool threaded = false, int intervalSec = 1);
	static double memPercent(bool threaded = false, int intervalSec = 1);
};