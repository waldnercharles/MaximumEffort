#include "cmp/transform_component.h"
#include "log.h"
#include "player_component.h"
#include "weapon_component.h"

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
	//	if (!is_cached)
	{
		is_cached = true;

		cached_parent_transform =
			parent ? parent->get_world_transform() : Transform {};
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
	for (i = children.count() - 1; i >= 0; i--)
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
	for (auto child : children)
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

void on_transform_construct(World &world, Entity e)
{
	world.get<TransformComponent>(e).entity = e;
}

void on_transform_update(World &world, Entity e)
{
	world.get<TransformComponent>(e).entity = e;
}

void on_transform_destroy(World &world, Entity e)
{
	auto &t = world.get<TransformComponent>(e);

	if (t.parent)
	{
		t.parent->remove_child(&t);
	}

	for (const auto &child : t.children)
	{
		child->clear_parent();
	}
}

void register_transform_callbacks(World &world)
{
	world.on_construct<TransformComponent>().connect<&on_transform_construct>();
	world.on_update<TransformComponent>().connect<&on_transform_update>();
	world.on_destroy<TransformComponent>().connect<&on_transform_destroy>();
}