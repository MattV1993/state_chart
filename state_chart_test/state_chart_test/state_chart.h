#pragma once

#include "state.h"

class state_chart
{
public:

	void start();

	void send_signal(signal s);

	state_composite_creator root_creator;
	state_composite_ptr root;

};