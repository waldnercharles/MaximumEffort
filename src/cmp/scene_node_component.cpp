#include "cmp/scene_node_component.h"

entt::entity SceneNodeComponent::get_entity() const
{
	return entity;
}

const SceneNodeTransform &SceneNodeComponent::get_transform() const
{
	return transform;
}

void SceneNodeComponent::set_transform(const SceneNodeTransform &t)
{
	invalidate_cached_parent_transform_for_children();
	transform = t;
}

SceneNodeTransform SceneNodeComponent::get_global_transform() const
{
	return get_parent_transform() * transform;
}

SceneNodeTransform SceneNodeComponent::get_parent_transform() const
{
	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform = parent ? parent->get_global_transform()
										 : SceneNodeTransform {};
	}

	return cached_parent_transform;
}

void SceneNodeComponent::add_child(SceneNodeComponent *child)
{
	assert(child->parent == nullptr);
	child->set_parent(this);
	children.add(child);
}

void SceneNodeComponent::remove_child(SceneNodeComponent *child)
{
	assert(child->parent == this);
	int i;
	for (i = 0; i < children.count(); i++)
	{
		if (children[i] == child)
		{
			children.unordered_remove(i);
			child->clear_parent();
			return;
		}
	}

	assert(false && "Parent child relationship is broken.");
}

SceneNodeComponent::~SceneNodeComponent()
{
	if (parent)
	{
		parent->remove_child(this);
	}

	for (const auto &child : children)
	{
		child->clear_parent();
	}
}

void SceneNodeComponent::set_parent(SceneNodeComponent *p)
{
	invalidate_cached_parent_transform();
	parent = p;
}

void SceneNodeComponent::clear_parent()
{
	set_parent(nullptr);
}

void SceneNodeComponent::invalidate_cached_parent_transform()
{
	is_cached = false;
	invalidate_cached_parent_transform_for_children();
}

void SceneNodeComponent::invalidate_cached_parent_transform_for_children()
{
	for (const auto &child : children)
	{
		child->invalidate_cached_parent_transform();
	}
}

void link_scene_node_with_entity(entt::registry &reg, entt::entity e)
{
	reg.get<SceneNodeComponent>(e).entity = e;
}

void register_scene_node_callbacks(entt::registry &reg)
{
	reg.on_construct<SceneNodeComponent>()
		.connect<&link_scene_node_with_entity>();
	reg.on_update<SceneNodeComponent>().connect<&link_scene_node_with_entity>();
}

SceneNodeTransform
operator*(const SceneNodeTransform &a, const SceneNodeTransform &b)
{
	return {cf_add_v2(a.pos, b.pos)};
}
