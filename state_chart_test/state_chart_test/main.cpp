#include "state_builder.h"
#include "my_state_chart.h"

// try_transistion_from_signal will fail because of static id - use define to calculate strings at runtime and create virtual ids?
// add different transition types - currently only have query
//    add on_finish, on_finish_delay


int main()
{
	auto chart = my_state_chart_builder::create();
	chart.start();

	chart.send_signal(signal::action);

	return 0;
}