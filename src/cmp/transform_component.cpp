#include "cmp/transform_component.h"

Entity TransformComponent::get_entity() const
{
	return entity;
}

const Transform &TransformComponent::get_local_transform() const
{
	return transform;
}

Transform TransformComponent::get_world_transform() const
{
	return get_parent_transform() * transform;
}

void TransformComponent::set_transform(const Transform &t)
{
	invalidate_cached_parent_transform_for_children();
	transform = t;
}

void TransformComponent::set_pos(const v2 pos)
{
	invalidate_cached_parent_transform_for_children();
	transform.pos = pos;
}

void TransformComponent::set_rotation(const float radians)
{
	invalidate_cached_parent_transform_for_children();
	transform.angle = radians;
}

Transform TransformComponent::get_parent_transform() const
{
	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform = parent ? parent->get_world_transform()
										 : Transform {};
	}

	return cached_parent_transform;
}

void TransformComponent::add_child(TransformComponent *child)
{
	assert(child->parent == nullptr);
	child->set_parent(this);
	children.add(child);
}

void TransformComponent::remove_child(TransformComponent *child)
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

TransformComponent::~TransformComponent()
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

void TransformComponent::set_parent(TransformComponent *p)
{
	invalidate_cached_parent_transform();
	parent = p;
}

void TransformComponent::clear_parent()
{
	set_parent(nullptr);
}

void TransformComponent::invalidate_cached_parent_transform()
{
	is_cached = false;
	invalidate_cached_parent_transform_for_children();
}

void TransformComponent::invalidate_cached_parent_transform_for_children()
{
	for (const auto &child : children)
	{
		child->invalidate_cached_parent_transform();
	}
}

TransformComponent &TransformComponent::operator=(const Transform &t)
{
	set_transform(t);
	return *this;
}

TransformComponent &TransformComponent::operator=(const v2 &p)
{
	set_pos(p);
	return *this;
}

TransformComponent &TransformComponent::operator+=(const v2 &p)
{
	set_pos(transform.pos + p);
	return *this;
}

TransformComponent &TransformComponent::operator-=(const v2 &p)
{
	set_pos(transform.pos - p);
	return *this;
}

void link_transform_with_entity(World &world, Entity e)
{
	world.get<TransformComponent>(e).entity = e;
}

void register_scene_node_callbacks(World &world)
{
	world.on_construct<TransformComponent>()
		.connect<&link_transform_with_entity>();
	world.on_update<TransformComponent>().connect<&link_transform_with_entity>(
	);
}

Transform operator*(const Transform &a, const Transform &b)
{
	return {a.pos + b.pos, a.angle + b.angle};
}