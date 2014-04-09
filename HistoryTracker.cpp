#include "HistoryTracker.h"


HistoryTracker::HistoryTracker(void)
{
	clearHistory();
	delay = DELAY_TIME*2;
	head = 0;
}


HistoryTracker::~HistoryTracker(void)
{
}


bool HistoryTracker::addToHistAndCheck(double left, double right) {
	if(delay != 0) {
		delay--;
		return false;
	}
	else {
		l_history[head] = left;
		r_history[head] = right;
		head = (head+1) % HISTORY_SIZE;

		double rmin, lmin, rmax, lmax;
		rmin = lmin = rmax = lmax = 0.0;
		for(int i = 0; i < HISTORY_SIZE; ++i)
		{
			//std::cout << l_history[i] << " " ;
			if(l_history[i] == 0 || r_history[i] == 0) return false;
			
			if(l_history[i] > lmax) {
				lmax = l_history[i];
			}
			else if(l_history[i] < lmin) {
				lmin = l_history[i];
			}
			
			if(r_history[i] > rmax) {
				rmax = r_history[i];
			}
			else if(r_history[i] < rmin) {
				rmin = r_history[i];
			}
		}
		//std::cout << std::endl;

		double lDiff = abs(lmax - lmin);
		double rDiff = abs(rmax - rmin);

		double average = (lDiff + rDiff)/2.0;

		if(average > FORCE_THRESHOLD) {
			clearHistory();
			delay = DELAY_TIME;
			//std::cout<<"average: " << average << std::endl;
			return true;
		}
	}
	return false;
}

void HistoryTracker::clearHistory() {
	for(unsigned int i = 0; i < HISTORY_SIZE; ++i) {
		l_history[i] = 0;
		r_history[i] = 0;
	}
}
