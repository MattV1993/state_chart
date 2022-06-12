#pragma once

#include "state.h"
#include "state_chart.h"

template<typename State_Chart_Type, typename Root_Composite_State>
class state_chart_builder
{
public:

	static State_Chart_Type create();
};

template<typename State_Chart_Type, typename Root_Composite_State>
State_Chart_Type state_chart_builder<State_Chart_Type, Root_Composite_State>::state_chart_builder::create()
{
	State_Chart_Type ret;
	ret.root_creator = Root_Composite_State::create();

	return ret;
}

template<typename State_Type>
class state_builder
{
public:

	static state_creator create();

};

template<typename State_Composite_Type, typename Nested_States, typename Regions>
class state_composite_builder
{
public:

	static state_composite_creator create();
};

template<class ...T>
class state_builder_list
{
public:

	static state_registry create();
};

template<typename State_Type>
state_creator state_builder<State_Type>::state_builder::create()
{
	return [](state_chart* chart, state_composite* parent)
	{
		return state::create<State_Type>(chart, parent);
	};
}

template<typename State_Composite_Type, typename Nested_States, typename Regions>
state_composite_creator state_composite_builder<State_Composite_Type, Nested_States, Regions>::state_composite_builder::create()
{
	return [](state_chart* chart, state_composite* parent) -> state_composite_ptr
	{
		state_registry reg = Nested_States::create();
		state_creator start_state_creator;

		return state_composite::create<State_Composite_Type>(chart, parent, std::move(reg), start_state_creator);
	};
}

template<typename ...T>
state_registry state_builder_list<T...>::state_builder_list::create()
{
	state_registry ret;

	std::vector<state_creator> c;

	// Need a way of getting id for map
	// create struct which holds std::function and string?
	(c.push_back(T::create()), ...);

	for (auto& p : c)
	{
		
	}


	return ret;
}
