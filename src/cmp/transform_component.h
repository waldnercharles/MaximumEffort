#pragma once
#include "common.h"

#include <cute.h>
#include <entt/entt.hpp>

struct Transform
{
	v2 pos;
	float angle;
};

Transform operator*(const Transform &a, const Transform &b);

struct TransformComponent
{
	Entity get_entity() const;

	const Transform &get_local_transform() const;
	Transform get_world_transform() const;
	Transform get_parent_transform() const;

	void set_transform(const Transform &t);
	void set_pos(const v2 pos);
	void set_rotation(const float radians);

	void add_child(TransformComponent *child);
	void remove_child(TransformComponent *child);

	TransformComponent &operator=(const Transform &);
	TransformComponent &operator=(const v2 &);
	TransformComponent &operator+=(const v2 &pos);
	TransformComponent &operator-=(const v2 &pos);

	~TransformComponent();

private:
	Entity entity = entt::null;

	TransformComponent *parent = nullptr;
	Array<TransformComponent *> children = {};

	Transform transform = {};

	mutable bool is_cached = false;
	mutable Transform cached_parent_transform = {};

	// Ensure components are not relocated in memory. This allows us to use regular
	// pointers pointing to them.
	static constexpr auto in_place_delete = true;

	void set_parent(TransformComponent *p);
	void clear_parent();

	void invalidate_cached_parent_transform();
	void invalidate_cached_parent_transform_for_children();

	friend void link_transform_with_entity(World &world, Entity e);
};

void link_transform_with_entity(World &world, Entity e);

void register_scene_node_callbacks(World &world);