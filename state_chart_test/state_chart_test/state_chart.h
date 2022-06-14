#pragma once

#include "state.h"

class state_chart
{
public:

	state_chart(const state_creator& root_creator);

	void start();

	void send_signal(signal s);
	void tick(float delta_seconds);

private:

	state_creator root_creator;
	state_ptr root;

};