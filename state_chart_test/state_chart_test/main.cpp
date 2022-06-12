#include "state_builder.h"

class my_state : public state
{
public:

	virtual void on_hit(int damage)
	{

	}

	std::string id() const override
	{
		return "my_state";
	}
};

class my_state_composite : public state_composite
{
public:

	virtual void on_hit(int damage)
	{

	}

	std::string id() const override
	{
		return "my_composite_state";
	}
};

class state_start : public my_state_composite {};

class state_sprint : public my_state_composite {};

class state_stationary : public my_state_composite {};
class state_jog : public my_state_composite {};

class state_stationary_idle : public my_state {};
class state_stationary_attack : public my_state {};

class state_sprint_idle : public my_state {};
class state_sprint_attack : public my_state {};

class state_jog_idle : public my_state {};
class state_jog_attack : public my_state {};

class my_state_chart : public state_chart 
{
public:

	void on_hit(int damage)
	{

	}
};

using my_state_chart_builder = 

	state_chart_builder<my_state_chart,
		state_composite_builder<state_start,
			state_builder_list<
				state_composite_builder<state_stationary,
					state_builder_list<
						state_builder<state_stationary_idle>,
						state_builder<state_stationary_attack>
					>, 
					state_builder_list<>
				>,
				state_composite_builder<state_jog,
					state_builder_list<
						state_builder<state_jog_idle>,
						state_builder<state_jog_attack>
					>,
					state_builder_list<>
				>,
				state_composite_builder<state_sprint,
					state_builder_list<
						state_builder<state_sprint_idle>,
						state_builder<state_sprint_attack>
					>, 
					state_builder_list<>
				>
			>, state_builder_list<>
		>
	>;

int main()
{
	auto chart = my_state_chart_builder::create();
	chart.start();

	return 0;
}