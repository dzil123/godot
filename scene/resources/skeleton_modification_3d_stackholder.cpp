#include "modules/tracy/include.h"
/*************************************************************************/
/*  skeleton_modification_3d_stackholder.cpp                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "scene/3d/skeleton_3d.h"
#include "scene/resources/skeleton_modification_3d.h"
#include "scene/resources/skeleton_modification_3d_stackholder.h"

bool SkeletonModification3DStackHolder::_set(const StringName &p_path, const Variant &p_value) {
	ZoneScopedS(60);
	String path = p_path;

	if (path == "held_modification_stack") {
		set_held_modification_stack(p_value);
	}
	return true;
}

bool SkeletonModification3DStackHolder::_get(const StringName &p_path, Variant &r_ret) const {
	ZoneScopedS(60);
	String path = p_path;

	if (path == "held_modification_stack") {
		r_ret = get_held_modification_stack();
	}
	return true;
}

void SkeletonModification3DStackHolder::_get_property_list(List<PropertyInfo> *p_list) const {
	ZoneScopedS(60);
	p_list->push_back(PropertyInfo(Variant::OBJECT, "held_modification_stack", PROPERTY_HINT_RESOURCE_TYPE, "SkeletonModificationStack3D", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE));
}

void SkeletonModification3DStackHolder::_execute(real_t p_delta) {
	ZoneScopedS(60);
	ERR_FAIL_COND_MSG(!stack || !is_setup || stack->skeleton == nullptr,
			"Modification is not setup and therefore cannot execute!");

	if (held_modification_stack.is_valid()) {
		held_modification_stack->execute(p_delta, execution_mode);
	}
}

void SkeletonModification3DStackHolder::_setup_modification(SkeletonModificationStack3D *p_stack) {
	ZoneScopedS(60);
	stack = p_stack;

	if (stack != nullptr) {
		is_setup = true;

		if (held_modification_stack.is_valid()) {
			held_modification_stack->set_skeleton(stack->get_skeleton());
			held_modification_stack->setup();
		}
	}
}

void SkeletonModification3DStackHolder::set_held_modification_stack(Ref<SkeletonModificationStack3D> p_held_stack) {
	ZoneScopedS(60);
	held_modification_stack = p_held_stack;

	if (is_setup && held_modification_stack.is_valid()) {
		held_modification_stack->set_skeleton(stack->get_skeleton());
		held_modification_stack->setup();
	}
}

Ref<SkeletonModificationStack3D> SkeletonModification3DStackHolder::get_held_modification_stack() const {
	ZoneScopedS(60);
	return held_modification_stack;
}

void SkeletonModification3DStackHolder::_bind_methods() {
	ZoneScopedS(60);
	ClassDB::bind_method(D_METHOD("set_held_modification_stack", "held_modification_stack"), &SkeletonModification3DStackHolder::set_held_modification_stack);
	ClassDB::bind_method(D_METHOD("get_held_modification_stack"), &SkeletonModification3DStackHolder::get_held_modification_stack);
}

SkeletonModification3DStackHolder::SkeletonModification3DStackHolder() {
	ZoneScopedS(60);
	stack = nullptr;
	is_setup = false;
	enabled = true;
}

SkeletonModification3DStackHolder::~SkeletonModification3DStackHolder() {
}
