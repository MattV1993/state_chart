#include "state_chart.h"

void state_chart::start()
{
	root = root_creator(this, nullptr);
}

void state_chart::send_signal(signal s)
{
	root->on_signal(s);
}

void state_chart::tick(float delta_seconds)
{
}
