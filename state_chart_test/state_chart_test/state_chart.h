#pragma once

#include "state.h"

class state_chart
{
public:

	void start();

	void send_signal(signal s);
	void tick(float delta_seconds);

	state_creator root_creator;
	state_ptr root;

};