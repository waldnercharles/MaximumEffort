#pragma once
#include "common.h"
#include "geometry.h"

#include <cute.h>
#include <entt/entt.hpp>

struct SceneGraphTransform
{
	v2 pos;
	float angle;
};

SceneGraphTransform
operator*(const SceneGraphTransform &a, const SceneGraphTransform &b);

struct Transform
{
	Entity get_entity() const;

	const SceneGraphTransform &get_local_transform() const;
	SceneGraphTransform get_parent_transform() const;
	SceneGraphTransform get_global_transform() const;

	void set_local_transform(const SceneGraphTransform &t);
	void set_pos(const v2 pos);
	void offset(const v2 offset);
	void set_rotation(const float radians);

	void add_child(Transform *child);
	void remove_child(Transform *child);

	~Transform();

private:
	Entity entity = entt::null;

	Transform *parent = nullptr;
	Array<Transform *> children = {};

	SceneGraphTransform transform = {};

	mutable bool is_cached = false;
	mutable SceneGraphTransform cached_parent_transform = {};

	// Ensure components are not relocated in memory. This allows us to use regular
	// pointers pointing to them.
	static constexpr auto in_place_delete = true;

	void set_parent(Transform *p);
	void clear_parent();

	void invalidate_cached_parent_transform();
	void invalidate_cached_parent_transform_for_children();

	friend void link_scene_node_with_entity(World &, Entity);
};

void link_scene_node_with_entity(World &reg, Entity e);

void register_scene_node_callbacks(World &reg);