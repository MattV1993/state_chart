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

template<typename State_Composite_Type, typename Nested_States>
class state_composite_builder
{
public:

	static state_creator create();
};

template<typename State_Regioned_Type, typename Regioned_States>
class state_regioned_builder
{
public:

	static state_creator create();
};


template<class ...T>
class state_list_builder
{
public:

	static state_registry create();
};

template<typename State_Type>
state_creator state_builder<State_Type>::state_builder::create()
{
	state_creator ret;
	ret.id = State_Type::id();

	ret.fn = [](state_chart* chart, state_composite* parent)
	{
		return state::create<State_Type>(chart, parent);
	};

	return ret;
}

template<typename State_Composite_Type, typename Nested_States>
state_creator state_composite_builder<State_Composite_Type, Nested_States>::state_composite_builder::create()
{
	state_creator ret;
	ret.id = State_Composite_Type::id();

	ret.fn = [](state_chart* chart, state_composite* parent)
	{
		state_registry reg = Nested_States::create();

		return state_composite::create<State_Composite_Type>(chart, parent, std::move(reg));
	};

	return ret;
}

template<typename State_Regioned_Type, typename Regioned_States>
state_creator state_regioned_builder<State_Regioned_Type, Regioned_States>::state_regioned_builder::create()
{
	state_creator ret;
	ret.id = State_Regioned_Type::id();

	ret.fn = [](state_chart* chart, state_composite* parent)
	{
		state_registry reg = Regioned_States::create();

		return state_regioned::create<State_Regioned_Type>(chart, parent, std::move(reg));
	};

	return ret;
}

template<typename ...T>
inline state_registry state_list_builder<T...>::state_list_builder::create()
{
	state_registry ret;
	(ret.reg.insert(std::make_pair(T::create().id, T::create())), ...);

	return ret;
}