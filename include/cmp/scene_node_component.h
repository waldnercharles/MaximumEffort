#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct SceneNodeTransform
{
	Cute::v2 pos;
};

SceneNodeTransform
operator*(const SceneNodeTransform &a, const SceneNodeTransform &b);

struct SceneNodeComponent
{
	entt::entity get_entity() const;

	const SceneNodeTransform &get_transform() const;

	void set_transform(const SceneNodeTransform &t);

	SceneNodeTransform get_global_transform() const;

	SceneNodeTransform get_parent_transform() const;

	void add_child(SceneNodeComponent *child);

	void remove_child(SceneNodeComponent *child);

	~SceneNodeComponent();

private:
	entt::entity entity = entt::null;

	SceneNodeComponent *parent = nullptr;
	Cute::Array<SceneNodeComponent *> children = {};

	SceneNodeTransform transform = {};

	mutable bool is_cached = false;
	mutable SceneNodeTransform cached_parent_transform = {};

	// Ensure components are not relocated in memory. This allows us to use regular
	// pointers pointing to them.
	static constexpr auto in_place_delete = true;

	void set_parent(SceneNodeComponent *p);

	void clear_parent();

	void invalidate_cached_parent_transform();

	void invalidate_cached_parent_transform_for_children();

	friend void link_scene_node_with_entity(entt::registry &, entt::entity);
};

void link_scene_node_with_entity(entt::registry &reg, entt::entity e);

void register_scene_node_callbacks(entt::registry &reg);