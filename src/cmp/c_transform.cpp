#include "cmp/c_transform.h"

using namespace Cute;

entt::entity C_Transform::get_entity() const
{
	return entity;
}

const SceneGraphTransform &C_Transform::get_local_transform() const
{
	return transform;
}

void C_Transform::set_local_transform(const SceneGraphTransform &t)
{
	invalidate_cached_parent_transform_for_children();
	transform = t;
}

void C_Transform::set_pos(const v2 pos)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos = pos;
}

void C_Transform::offset(const v2 offset)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos += offset;
}

void C_Transform::set_rotation(const float radians)
{
	invalidate_cached_parent_transform_for_children();
	transform.angle = radians;
}

SceneGraphTransform C_Transform::get_global_transform() const
{
	return get_parent_transform() * transform;
}

SceneGraphTransform C_Transform::get_parent_transform() const
{
	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform = parent ? parent->get_global_transform()
										 : SceneGraphTransform {};
	}

	return cached_parent_transform;
}

void C_Transform::add_child(C_Transform *child)
{
	assert(child->parent == nullptr);
	child->set_parent(this);
	children.add(child);
}

void C_Transform::remove_child(C_Transform *child)
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

C_Transform::~C_Transform()
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

void C_Transform::set_parent(C_Transform *p)
{
	invalidate_cached_parent_transform();
	parent = p;
}

void C_Transform::clear_parent()
{
	set_parent(nullptr);
}

void C_Transform::invalidate_cached_parent_transform()
{
	is_cached = false;
	invalidate_cached_parent_transform_for_children();
}

void C_Transform::invalidate_cached_parent_transform_for_children()
{
	for (const auto &child : children)
	{
		child->invalidate_cached_parent_transform();
	}
}

void link_scene_node_with_entity(entt::registry &reg, entt::entity e)
{
	reg.get<C_Transform>(e).entity = e;
}

void register_scene_node_callbacks(entt::registry &reg)
{
	reg.on_construct<C_Transform>().connect<&link_scene_node_with_entity>();
	reg.on_update<C_Transform>().connect<&link_scene_node_with_entity>();
}

SceneGraphTransform
operator*(const SceneGraphTransform &a, const SceneGraphTransform &b)
{
	return {a.pos + b.pos, a.angle + b.angle};
}
