#pragma once

class my_state : public state
{
public:

	virtual void on_hit(int damage)
	{

	}

};

class my_state_composite : public state_composite
{
public:

	virtual void on_hit(int damage)
	{

	}

};

class my_state_regioned : public state_regioned
{
public:

	virtual void on_hit(int damage)
	{

	}

};

class state_start : public my_state_regioned
{
public:

	static std::string id_s()
	{
		return "state_start";
	}
};

class state_movement : public my_state_composite
{
public:

	static std::string id_s()
	{
		return "state_movement";
	}
};

class state_sprint : public my_state_composite
{
public:

	static std::string id_s()
	{
		return "state_sprint";
	}
};

class state_stationary : public my_state_composite
{
public:

	static std::string id_s()
	{
		return "state_stationary";
	}
};

class state_jog : public my_state_composite
{
public:

	static std::string id_s()
	{
		return "state_jog";
	}
};

class state_stationary_idle : public my_state
{
public:

	static std::string id_s()
	{
		return "state_stationary_idle";
	}
};

class state_stationary_attack : public my_state
{
public:

	static std::string id_s()
	{
		return "state_stationary_attack";
	}
};

class state_sprint_idle : public my_state
{
public:

	static std::string id_s()
	{
		return "state_sprint_idle";
	}
};

class state_sprint_attack : public my_state
{
public:

	static std::string id_s()
	{
		return "state_sprint_attack";
	}
};

class state_jog_idle : public my_state
{
public:

	static std::string id_s()
	{
		return "state_jog_idle";
	}
};

class state_jog_attack : public my_state
{
public:

	static std::string id_s()
	{
		return "state_jog_attack";
	}
};

class state_block_holder : public state_composite
{
public:

	static std::string id_s()
	{
		return "state_block_holder";
	}
};

class state_block : public my_state
{
public:

	static std::string id_s()
	{
		return "state_block";
	}
};

class state_not_block : public my_state
{
public:

	void on_start() override
	{
		int a = 0;
	}

	static std::string id_s()
	{
		return "state_not_block";
	}
};

class state_zoom_holder : public state_composite
{
public:

	static std::string id_s()
	{
		return "state_zoom_holder";
	}
};

class state_zoom : public my_state
{
public:

	static std::string id_s()
	{
		return "state_zoom";
	}
};

class state_not_zoom : public my_state
{
public:

	void on_start() override
	{
		int a = 0;
	}

	static std::string id_s()
	{
		return "state_not_zoom";
	}
};


class my_state_chart : public state_chart
{
public:

	void on_hit(int damage)
	{

	}
};

using movement_transition_builder =

transition_list_builder<>;

using stationary_transition_builder = 

transition_list_builder<
	transition_query_builder<state_stationary_idle, signal::action, state_stationary_attack>
>;

using jog_transition_builder = 

transition_list_builder<
	transition_query_builder<state_jog_idle, signal::action, state_jog_attack>
>;

using sprint_transition_builder = 

transition_list_builder<
	transition_query_builder<state_sprint_idle, signal::action, state_sprint_attack>
>;

using block_transition_builder = 

transition_list_builder<
	transition_query_builder<state_not_block, signal::alternate_action, state_block>,
	transition_query_builder<state_block, signal::alternate_action_stop, state_not_block>
>;

using zoom_transition_builder = 

transition_list_builder<
	transition_query_builder<state_not_zoom, signal::alternate_action, state_zoom>,
	transition_query_builder<state_zoom, signal::alternate_action_stop, state_not_zoom>
>;

using my_movement_builder =

state_composite_builder < state_movement,
	state_list_builder<
		state_composite_builder<state_stationary,
			state_list_builder<
				state_builder<state_stationary_idle>,
				state_builder<state_stationary_attack>
			>,
			stationary_transition_builder
		>,
		state_composite_builder<state_jog,
			state_list_builder<
				state_builder<state_jog_idle>,
				state_builder<state_jog_attack>
			>,
			jog_transition_builder
		>,
		state_composite_builder<state_sprint,
			state_list_builder<
				state_builder<state_sprint_idle>,
				state_builder<state_sprint_attack>
			>,
			sprint_transition_builder
		>
	>,
	movement_transition_builder
>;

using my_block_builder =

state_composite_builder<state_block_holder,
	state_list_builder<
		state_builder<state_not_block>,
		state_builder<state_block>
	>,
	block_transition_builder
>;

using my_zoom_builder =

state_composite_builder<state_zoom_holder,
	state_list_builder<
		state_builder<state_not_zoom>,
		state_builder<state_zoom>
	>,
	zoom_transition_builder
>;

using my_state_chart_builder =

state_chart_builder<my_state_chart,
	state_regioned_builder<state_start,
		state_list_builder<
			my_movement_builder,
			my_block_builder,
			my_zoom_builder
		>
	>
>;