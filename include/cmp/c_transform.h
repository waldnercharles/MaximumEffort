#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct SceneGraphTransform
{
	Cute::v2 pos;
	float angle;
};

SceneGraphTransform
operator*(const SceneGraphTransform &a, const SceneGraphTransform &b);

struct C_Transform
{
	entt::entity get_entity() const;

	const SceneGraphTransform &get_local_transform() const;
	SceneGraphTransform get_parent_transform() const;
	SceneGraphTransform get_global_transform() const;

	void set_local_transform(const SceneGraphTransform &t);
	void set_pos(const Cute::v2 pos);
	void set_rotation(const float radians);

	void add_child(C_Transform *child);
	void remove_child(C_Transform *child);

	~C_Transform();

private:
	entt::entity entity = entt::null;

	C_Transform *parent = nullptr;
	Cute::Array<C_Transform *> children = {};

	SceneGraphTransform transform = {};

	mutable bool is_cached = false;
	mutable SceneGraphTransform cached_parent_transform = {};

	// Ensure components are not relocated in memory. This allows us to use regular
	// pointers pointing to them.
	static constexpr auto in_place_delete = true;

	void set_parent(C_Transform *p);
	void clear_parent();

	void invalidate_cached_parent_transform();
	void invalidate_cached_parent_transform_for_children();

	friend void link_scene_node_with_entity(entt::registry &, entt::entity);
};

void link_scene_node_with_entity(entt::registry &reg, entt::entity e);

void register_scene_node_callbacks(entt::registry &reg);