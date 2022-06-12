#include "state.h"

void state::on_start()
{
}

void state::on_signal(signal s)
{
}

void state::tick(float delta_seconds)
{
}

state_chart* state::get_chart()
{
	return chart;
}

state_composite* state::get_parent()
{
	return parent;
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

void state_composite::tick(float delta_seconds)
{
	state::tick(delta_seconds);
	current_state->tick(delta_seconds);
}

void state_composite::change_state(const std::string state_id)
{
	auto creator = registry.get(state_id);
	current_state = creator(get_chart(), this);
	current_state->on_start();
}

state_registry& state_composite::get_registry()
{
	return registry;
}

const state_creator& state_registry::get(const std::string& id) const
{
	return reg.find(id)->second;
}

const state_creator& state_registry::get_default_state() const
{
	return reg.begin()->second;
}

state_ptr state_creator::operator()(state_chart* chart, state_composite* parent) const
{
	return fn(chart, parent);
}

void state_regioned::on_signal(signal s)
{
	for (auto& r : regions)
	{
		r->on_signal(s);
	}
}

void state_regioned::tick(float delta_seconds)
{
	for (auto& r : regions)
	{
		r->tick(delta_seconds);
	}
}
