#pragma once

#define HISTORY_SIZE 10
#define FORCE_THRESHOLD 80.0
#define DELAY_TIME 40

#include <math.h>
#include <iostream>

class HistoryTracker
{
public:
	HistoryTracker(void);
	~HistoryTracker(void);

	bool addToHistAndCheck(double left, double right);


private:
	void clearHistory();

	double l_history[HISTORY_SIZE], r_history[HISTORY_SIZE];
	int delay, head;
};

