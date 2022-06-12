#include "state.h"

void state::on_signal(signal s)
{
}

state_chart& state::get_chart()
{
	return *chart;
}

state_composite& state::get_parent()
{
	return *parent;
}

void state::init(state_chart* chart, state_composite* parent)
{
	this->chart = chart;
	this->parent = parent;
}

void state_composite::on_signal(signal s)
{
	current_state->on_signal(s);
}

state_registry& state_composite::get_registry()
{
	return registry;
}

const state_creator& state_registry::get(const std::string& id)
{
	return reg.find(id)->second;
}
