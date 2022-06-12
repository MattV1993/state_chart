#include "state_builder.h"
#include "my_state_chart.h"


int main()
{
	auto chart = my_state_chart_builder::create();
	chart.start();

	chart.send_signal(signal::action);

	return 0;
}