#include "cmp/transform.h"

Entity Transform::get_entity() const
{
	return entity;
}

const SceneGraphTransform &Transform::get_local_transform() const
{
	return transform;
}

void Transform::set_local_transform(const SceneGraphTransform &t)
{
	invalidate_cached_parent_transform_for_children();
	transform = t;
}

void Transform::set_pos(const v2 pos)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos = pos;
}

void Transform::offset(const v2 offset)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos += offset;
}

void Transform::set_rotation(const float radians)
{
	invalidate_cached_parent_transform_for_children();
	transform.angle = radians;
}

SceneGraphTransform Transform::get_global_transform() const
{
	return get_parent_transform() * transform;
}

SceneGraphTransform Transform::get_parent_transform() const
{
	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform = parent ? parent->get_global_transform()
										 : SceneGraphTransform {};
	}

	return cached_parent_transform;
}

void Transform::add_child(Transform *child)
{
	assert(child->parent == nullptr);
	child->set_parent(this);
	children.add(child);
}

void Transform::remove_child(Transform *child)
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

Transform::~Transform()
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

void Transform::set_parent(Transform *p)
{
	invalidate_cached_parent_transform();
	parent = p;
}

void Transform::clear_parent()
{
	set_parent(nullptr);
}

void Transform::invalidate_cached_parent_transform()
{
	is_cached = false;
	invalidate_cached_parent_transform_for_children();
}

void Transform::invalidate_cached_parent_transform_for_children()
{
	for (const auto &child : children)
	{
		child->invalidate_cached_parent_transform();
	}
}

void link_scene_node_with_entity(World &world, Entity e)
{
	world.get<Transform>(e).entity = e;
}

void register_scene_node_callbacks(World &world)
{
	world.on_construct<Transform>().connect<&link_scene_node_with_entity>();
	world.on_update<Transform>().connect<&link_scene_node_with_entity>();
}

SceneGraphTransform
operator*(const SceneGraphTransform &a, const SceneGraphTransform &b)
{
	return {a.pos + b.pos, a.angle + b.angle};
}