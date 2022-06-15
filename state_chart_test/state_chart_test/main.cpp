#include "state_builder.h"
#include "my_state_chart.h"

// add different transition types - currently only have query
//    add on_finish, on_finish_delay

// some states only handle actions when in state (on_signal)
// some will need to handle actions any time (poll)


int main()
{
	auto chart = my_state_chart_builder::create();
	chart.start();

	chart.send_signal(signal::action);

	return 0;
}