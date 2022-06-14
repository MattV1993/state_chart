#include "state_chart.h"

state_chart::state_chart(const state_creator& root_creator)
	: root_creator{ root_creator }
{
}

void state_chart::start()
{
	root = root_creator(this, nullptr);
}

void state_chart::send_signal(signal s)
{
	root->send_signal(s);
}

void state_chart::tick(float delta_seconds)
{
}
