#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <functional>

class state;
class state_composite;
class state_chart;

using state_ptr = std::unique_ptr<state>;
using state_composite_ptr = std::unique_ptr<state_composite>;

using state_creator = std::function<state_ptr(state_chart*, state_composite*)>;
using state_composite_creator = std::function<state_composite_ptr(state_chart*, state_composite*)>;


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

	virtual void on_signal(signal s);

    virtual std::string id() const = 0;

    template<typename State_Type>
    static state_ptr create(state_chart* chart, state_composite* parent);

protected:

	state() = default;

	state(const state&) = default;
	state(state&&) = default;

	state& operator=(const state&) = default;
	state& operator=(state&&) = default;

    state_chart& get_chart();
    state_composite& get_parent();

    void init(state_chart* chart, state_composite* parent);

private:

	state_chart* chart;
	state_composite* parent;
};

class state_registry
{
public:

    const state_creator& get(const std::string& id);

    std::map<std::string, state_creator> reg;

private:
};

class state_composite : public state
{
public:

    void on_signal(signal s) override;

    template<typename State_Composite_Type>
    static state_composite_ptr create(state_chart* chart, state_composite* parent, state_registry&& registry, const state_creator& start_state_creator);

    state_registry registry;
    state_creator start_state_creator;

protected:

    state_registry& get_registry();

private:

	state_ptr current_state;

};

template<typename State_Type>
inline state_ptr state::create(state_chart* chart, state_composite* parent)
{
    state_ptr ret = std::make_unique<State_Type>();
    ret->init(chart, parent);

    return ret;
}

template<typename State_Composite_Type>
inline state_composite_ptr state_composite::create(state_chart* chart, state_composite* parent, state_registry&& registry, const state_creator& start_state_creator)
{
    state_composite_ptr ret = std::make_unique<State_Composite_Type>();
    ret->init(chart, parent);
    ret->registry = std::move(registry);
    ret->start_state_creator = start_state_creator;

    return ret;
}