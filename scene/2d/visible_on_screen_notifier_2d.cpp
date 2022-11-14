/*************************************************************************/
/*  visible_on_screen_notifier_2d.cpp                                    */
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

#include "modules/tracy/include.h"
/*************************************************************************/
/*  visible_on_screen_notifier_2d.cpp                                    */
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

#include "visible_on_screen_notifier_2d.h"

#include "scene/scene_string_names.h"

#ifdef TOOLS_ENABLED
Rect2 VisibleOnScreenNotifier2D::_edit_get_rect() const {
	ZoneScopedS(60);
	return rect;
}

bool VisibleOnScreenNotifier2D::_edit_use_rect() const {
	ZoneScopedS(60);
	return true;
}
#endif

void VisibleOnScreenNotifier2D::_visibility_enter() {
	ZoneScopedS(60);
	if (!is_inside_tree() || Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	on_screen = true;
	emit_signal(SceneStringNames::get_singleton()->screen_entered);
	_screen_enter();
}
void VisibleOnScreenNotifier2D::_visibility_exit() {
	ZoneScopedS(60);
	if (!is_inside_tree() || Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	on_screen = false;
	emit_signal(SceneStringNames::get_singleton()->screen_exited);
	_screen_exit();
}

void VisibleOnScreenNotifier2D::set_rect(const Rect2 &p_rect) {
	ZoneScopedS(60);
	rect = p_rect;
	if (is_inside_tree()) {
		RS::get_singleton()->canvas_item_set_visibility_notifier(get_canvas_item(), true, rect, callable_mp(this, &VisibleOnScreenNotifier2D::_visibility_enter), callable_mp(this, &VisibleOnScreenNotifier2D::_visibility_exit));
	}
	queue_redraw();
}

Rect2 VisibleOnScreenNotifier2D::get_rect() const {
	ZoneScopedS(60);
	return rect;
}

void VisibleOnScreenNotifier2D::_notification(int p_what) {
	ZoneScopedS(60);
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			on_screen = false;
			RS::get_singleton()->canvas_item_set_visibility_notifier(get_canvas_item(), true, rect, callable_mp(this, &VisibleOnScreenNotifier2D::_visibility_enter), callable_mp(this, &VisibleOnScreenNotifier2D::_visibility_exit));
		} break;

		case NOTIFICATION_DRAW: {
			if (Engine::get_singleton()->is_editor_hint()) {
				draw_rect(rect, Color(1, 0.5, 1, 0.2));
			}
		} break;

		case NOTIFICATION_EXIT_TREE: {
			on_screen = false;
			RS::get_singleton()->canvas_item_set_visibility_notifier(get_canvas_item(), false, Rect2(), Callable(), Callable());
		} break;
	}
}

bool VisibleOnScreenNotifier2D::is_on_screen() const {
	ZoneScopedS(60);
	return on_screen;
}

void VisibleOnScreenNotifier2D::_bind_methods() {
	ZoneScopedS(60);
	ClassDB::bind_method(D_METHOD("set_rect", "rect"), &VisibleOnScreenNotifier2D::set_rect);
	ClassDB::bind_method(D_METHOD("get_rect"), &VisibleOnScreenNotifier2D::get_rect);
	ClassDB::bind_method(D_METHOD("is_on_screen"), &VisibleOnScreenNotifier2D::is_on_screen);

	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "rect", PROPERTY_HINT_NONE, "suffix:px"), "set_rect", "get_rect");

	ADD_SIGNAL(MethodInfo("screen_entered"));
	ADD_SIGNAL(MethodInfo("screen_exited"));
}

VisibleOnScreenNotifier2D::VisibleOnScreenNotifier2D() {
	ZoneScopedS(60);
	rect = Rect2(-10, -10, 20, 20);
}

//////////////////////////////////////

void VisibleOnScreenEnabler2D::_screen_enter() {
	ZoneScopedS(60);
	_update_enable_mode(true);
}

void VisibleOnScreenEnabler2D::_screen_exit() {
	ZoneScopedS(60);
	_update_enable_mode(false);
}

void VisibleOnScreenEnabler2D::set_enable_mode(EnableMode p_mode) {
	ZoneScopedS(60);
	enable_mode = p_mode;
	if (is_inside_tree()) {
		_update_enable_mode(is_on_screen());
	}
}
VisibleOnScreenEnabler2D::EnableMode VisibleOnScreenEnabler2D::get_enable_mode() {
	ZoneScopedS(60);
	return enable_mode;
}

void VisibleOnScreenEnabler2D::set_enable_node_path(NodePath p_path) {
	ZoneScopedS(60);
	if (enable_node_path == p_path) {
		return;
	}
	enable_node_path = p_path;
	if (is_inside_tree()) {
		node_id = ObjectID();
		Node *node = get_node(enable_node_path);
		if (node) {
			node_id = node->get_instance_id();
			_update_enable_mode(is_on_screen());
		}
	}
}
NodePath VisibleOnScreenEnabler2D::get_enable_node_path() {
	ZoneScopedS(60);
	return enable_node_path;
}

void VisibleOnScreenEnabler2D::_update_enable_mode(bool p_enable) {
	ZoneScopedS(60);
	Node *node = static_cast<Node *>(ObjectDB::get_instance(node_id));
	if (node) {
		if (p_enable) {
			switch (enable_mode) {
				case ENABLE_MODE_INHERIT: {
					node->set_process_mode(PROCESS_MODE_INHERIT);
				} break;
				case ENABLE_MODE_ALWAYS: {
					node->set_process_mode(PROCESS_MODE_ALWAYS);
				} break;
				case ENABLE_MODE_WHEN_PAUSED: {
					node->set_process_mode(PROCESS_MODE_WHEN_PAUSED);
				} break;
			}
		} else {
			node->set_process_mode(PROCESS_MODE_DISABLED);
		}
	}
}
void VisibleOnScreenEnabler2D::_notification(int p_what) {
	ZoneScopedS(60);
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}

			node_id = ObjectID();
			Node *node = get_node(enable_node_path);
			if (node) {
				node_id = node->get_instance_id();
				node->set_process_mode(PROCESS_MODE_DISABLED);
			}
		} break;

		case NOTIFICATION_EXIT_TREE: {
			node_id = ObjectID();
		} break;
	}
}

void VisibleOnScreenEnabler2D::_bind_methods() {
	ZoneScopedS(60);
	ClassDB::bind_method(D_METHOD("set_enable_mode", "mode"), &VisibleOnScreenEnabler2D::set_enable_mode);
	ClassDB::bind_method(D_METHOD("get_enable_mode"), &VisibleOnScreenEnabler2D::get_enable_mode);

	ClassDB::bind_method(D_METHOD("set_enable_node_path", "path"), &VisibleOnScreenEnabler2D::set_enable_node_path);
	ClassDB::bind_method(D_METHOD("get_enable_node_path"), &VisibleOnScreenEnabler2D::get_enable_node_path);

	ADD_GROUP("Enabling", "enable_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "enable_mode", PROPERTY_HINT_ENUM, "Inherit,Always,When Paused"), "set_enable_mode", "get_enable_mode");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "enable_node_path"), "set_enable_node_path", "get_enable_node_path");

	BIND_ENUM_CONSTANT(ENABLE_MODE_INHERIT);
	BIND_ENUM_CONSTANT(ENABLE_MODE_ALWAYS);
	BIND_ENUM_CONSTANT(ENABLE_MODE_WHEN_PAUSED);
}

VisibleOnScreenEnabler2D::VisibleOnScreenEnabler2D() {
}
