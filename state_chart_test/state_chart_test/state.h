#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <optional>

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

	virtual bool send_signal(signal s);

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

	virtual bool on_signal(signal s);

private:

	state_chart* chart;
	state_composite* parent;
};

class state_creator
{
public:

	state_creator() = default;
	state_creator(const std::string& id, state_creator_fn fn);

	state_ptr operator()(state_chart* chart, state_composite* parent) const;

	const std::string get_id() const;

private:

	std::string id;
	state_creator_fn fn;
};

struct transition_query
{
	std::string from;
	signal s;
	std::string to;
};

inline bool operator==(const transition_query& lhs, const transition_query& rhs)
{
	return 
		(lhs.from == rhs.from) &&
		(lhs.s == rhs.s) &&
		(lhs.to == rhs.to);
}

struct transition_query_hasher
{
	std::size_t operator()(const std::pair<std::string, signal>& h) const;
};

class transition_querier
{
public:

	void register_query(const transition_query& query);

	std::optional<std::string> request_transition_id(const std::string& from, signal s);

private:

	std::unordered_map<std::pair<std::string, signal>, std::string, transition_query_hasher> queries;
};

class state_registry
{
public:

	const state_creator& get(const std::string& id) const;
	const state_creator& get_default_state() const;

	void register_state(const state_creator& creator);

	std::vector<state_creator> all() const;

private:

	std::unordered_map<std::string, state_creator> reg;
};

class state_composite : public state
{
public:

	bool send_signal(signal s) override final;

	void tick(float delta_seconds) override;

	void change_state(const std::string state_id);

	template<typename State_Composite_Type>
	static state_composite_ptr create(state_chart* chart, state_composite* parent, state_registry&& registry, transition_querier&& querier);

protected:

	bool on_signal(signal s) override;

	state_registry& get_registry();

private:

	bool try_transistion_from_signal(signal s);

	state_ptr current_state;

	state_registry registry;
	transition_querier querier;
};

class state_regioned : public state
{
public:

	bool send_signal(signal s) override final;

	void tick(float delta_seconds) override;

	template<typename State_Regioned_Type>
	static state_regioned_ptr create(state_chart* chart, state_composite* parent, state_registry&& registry);

private:

	std::vector<state_ptr> regions;
};

template<typename State_Type>
inline state_ptr state::create(state_chart* chart, state_composite* parent)
{
	state_ptr ret = std::make_unique<State_Type>();

	ret->init(chart, parent);
	ret->on_start();

	return ret;
}

template<typename State_Composite_Type>
inline state_composite_ptr state_composite::create(state_chart* chart, state_composite* parent, state_registry&& registry, transition_querier&& querier)
{
	state_composite_ptr ret = std::make_unique<State_Composite_Type>();

	ret->registry = std::move(registry);
	ret->querier = std::move(querier);

	ret->init(chart, parent);
	ret->change_state(ret->registry.get_default_state().get_id());
	ret->on_start();

	return ret;
}

template<typename State_Regioned_Type>
inline state_regioned_ptr state_regioned::create(state_chart* chart, state_composite* parent, state_registry&& registry)
{
	state_regioned_ptr ret = std::make_unique<State_Regioned_Type>();

	for (const auto& reg : registry.all())
	{
		ret->regions.push_back(reg(chart, nullptr));
	}

	ret->init(chart, parent);
	ret->on_start();


	return ret;
}
