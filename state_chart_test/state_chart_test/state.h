#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

class state;
class state_composite;
class state_chart;
class state_regioned;

using state_ptr = std::unique_ptr<state>;
using state_composite_ptr = std::unique_ptr<state_composite>;
using state_regioned_ptr = std::unique_ptr<state_regioned>;

using state_creator_fn = std::function<state_ptr(state_chart*, state_composite*)>;

enum class signal
{
	action,
	action_stop,
	alternate_action,
	alternate_action_stop,

	forward_start,
	forward_stop,
	back_start,
	back_stop,
	left_start,
	left_stop,
	right_start,
	right_stop,
};

class state
{
public:

	virtual ~state() = default;

	virtual void on_start();

	virtual void on_signal(signal s);

	virtual void tick(float delta_seconds);

	static std::string id() { return "state"; };

	template<typename State_Type>
	static state_ptr create(state_chart* chart, state_composite* parent);

protected:

	state() = default;

	state(const state&) = default;
	state(state&&) = default;

	state& operator=(const state&) = default;
	state& operator=(state&&) = default;

	state_chart* get_chart();
	state_composite* get_parent();

	void init(state_chart* chart, state_composite* parent);

private:

	state_chart* chart;
	state_composite* parent;
};

class state_creator
{
public:

	state_ptr operator()(state_chart* chart, state_composite* parent) const;

	std::string id;
	state_creator_fn fn;
};

class state_registry
{
public:

	const state_creator& get(const std::string& id) const;
	const state_creator& get_default_state() const;

	std::unordered_map<std::string, state_creator> reg;

private:

};

class state_composite : public state
{
public:

	void on_signal(signal s) override;
	void tick(float delta_seconds) override;

	void change_state(const std::string state_id);

	template<typename State_Composite_Type>
	static state_composite_ptr create(state_chart* chart, state_composite* parent, state_registry&& registry);

	state_registry registry;

protected:

	state_registry& get_registry();

private:

	state_ptr current_state;
};

class state_regioned : public state
{
public:

	void on_signal(signal s) override;
	void tick(float delta_seconds) override;

	template<typename State_Regioned_Type>
	static state_regioned_ptr create(state_chart* chart, state_composite* parent, state_registry&& registry);

	state_registry registry;

private:

	std::vector<state_composite_ptr> regions;
};

template<typename State_Type>
inline state_ptr state::create(state_chart* chart, state_composite* parent)
{
	state_ptr ret = std::make_unique<State_Type>();
	ret->init(chart, parent);

	return ret;
}

template<typename State_Composite_Type>
inline state_composite_ptr state_composite::create(state_chart* chart, state_composite* parent, state_registry&& registry)
{
	state_composite_ptr ret = std::make_unique<State_Composite_Type>();

	ret->init(chart, parent);
	ret->registry = std::move(registry);
	ret->change_state(ret->registry.get_default_state().id);

	return ret;
}

template<typename State_Regioned_Type>
inline state_regioned_ptr state_regioned::create(state_chart* chart, state_composite* parent, state_registry&& registry)
{
	state_regioned_ptr ret = std::make_unique<State_Regioned_Type>();

	ret->init(chart, parent);
	ret->registry = std::move(registry);

	// loop through registry and create regions

	return ret;
}
