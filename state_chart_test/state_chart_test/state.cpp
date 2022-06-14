#include "state.h"

void state::on_start()
{
}

bool state::send_signal(signal s)
{
	return on_signal(s);
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

bool state::on_signal(signal s)
{
	return false;
}

bool state_composite::send_signal(signal s)
{
	bool ret = current_state->send_signal(s);

	if (ret)
	{
		return true;
	}

	return on_signal(s);
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
}

bool state_composite::on_signal(signal s)
{
	return try_transistion_from_signal(s);
}

state_registry& state_composite::get_registry()
{
	return registry;
}

bool state_composite::try_transistion_from_signal(signal s)
{
	auto new_state_id = querier.request_transition_id(current_state->id(), s);

	if (new_state_id.has_value() == false)
	{
		return false;
	}

	change_state(*new_state_id);

	return true;
}

const state_creator& state_registry::get(const std::string& id) const
{
	return reg.find(id)->second;
}

const state_creator& state_registry::get_default_state() const
{
	return reg.begin()->second;
}

void state_registry::register_state(const state_creator& creator)
{
	reg.insert(std::make_pair(creator.get_id(), creator));
}

std::vector<state_creator> state_registry::all() const
{
	std::vector<state_creator> ret;

	for (const auto& r : reg)
	{
		ret.push_back(r.second);
	}

	return ret;
}

state_creator::state_creator(const std::string& id, state_creator_fn fn)
	: id{ id }, fn{ fn }
{
}

state_ptr state_creator::operator()(state_chart* chart, state_composite* parent) const
{
	return fn(chart, parent);
}

const std::string state_creator::get_id() const
{
	return id;
}

bool state_regioned::send_signal(signal s)
{
	for (auto& r : regions)
	{
		bool ret = r->send_signal(s);

		if (ret)
		{
			return true;
		}
	}

	return on_signal(s);
}

void state_regioned::tick(float delta_seconds)
{
	for (auto& r : regions)
	{
		r->tick(delta_seconds);
	}
}

void transition_querier::register_query(const transition_query& query)
{
	queries.insert(std::make_pair(std::make_pair(query.from, query.s), query.to));
}

std::optional<std::string> transition_querier::request_transition_id(const std::string& from, signal s)
{
	auto it = queries.find(std::make_pair(from, s));

	if (it != queries.end())
	{
		return it->second;
	}

	return std::nullopt;
}

std::size_t transition_query_hasher::operator()(const std::pair<std::string, signal>& h) const
{
	return
		((std::hash<std::string>()(h.first) ^
			(std::hash<signal>()(h.second) << 1))
			>> 1);
}
