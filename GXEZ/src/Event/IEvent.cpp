#include "GXEZ/Event/IEvent.h"

#include <sstream>
#include <iostream>

namespace GXEZ
{
	std::string EventTypeToString(const Event::Type& type)
	{
		if (type == Event::Type::NONE)
			return ("NONE");
		else if (type == Event::Type::KEY)
			return ("KEY");
		else if (type == Event::Type::MOUSE)
			return ("MOUSE");
		else if (type == Event::Type::WINDOW)
			return ("WINDOW");
		else if (type == Event::Type::QUIT)
			return ("QUIT");
		return ("UNKNOWN");
	}

	std::string ControlKeyStateToString(const ControlKeyState& type)
	{
		if (type == ControlKeyState::NONE)
			return ("NONE");
		else if (type == ControlKeyState::PRESSED)
			return ("PRESSED");
		else if (type == ControlKeyState::RELEASED)
			return ("RELEASED");
		return ("UNKNOWN");
	}

	std::string ControlKeyWindowToString(const ControlKey& key)
	{
		if (key == ControlKey::KEY_NONE)
			return ("NONE");
		else if (key == ControlKey::WINDOW_CLOSE)
			return ("CLOSE");
		else if (key == ControlKey::WINDOW_RESIZE)
			return ("RESIZE");
		else if (key == ControlKey::WINDOW_MOVE)
			return ("MOVE");
		else if (key == ControlKey::WINDOW_FOCUS)
			return ("FOCUS");
		return ("UNKNOWN");
	}

	std::string ControlKeyMouseToString(const ControlKey& key)
	{
		if (key == ControlKey::KEY_NONE)
			return ("NONE");
		else if (key == ControlKey::MOUSE_LEFT)
			return ("LEFT");
		else if (key == ControlKey::MOUSE_RIGHT)
			return ("RIGHT");
		else if (key == ControlKey::MOUSE_SCROLL)
			return ("SCROLL");
		else if (key == ControlKey::MOUSE_MOTION)
			return ("MOTION");
		return ("UNKNOWN");
	}

	std::string Event::ToString() const
	{
		std::stringstream ss;

		ss << "Hash (" << GetHash() << ") Type (" << EventTypeToString(_def.type) << ") IdD (" << _def.idDevice << ") Key";
		if (_def.type == Event::Type::KEY)
			ss << " (" << (unsigned char)_def.key << ") (" << ControlKeyStateToString(_def.state) << ")";
		else if (_def.type == Event::Type::MOUSE)
		{
			ss << " (" << ControlKeyMouseToString(_def.key) << ") (" << ControlKeyStateToString(_def.state) << ")";
		}
		else if (_def.type == Event::Type::WINDOW)
		{
			ss << "(" << _def.idDevice << ") (" << ControlKeyWindowToString(_def.key) << ") State (" << int(_def.state) << ")";
		}
		ss << std::endl;
		return (ss.str());
	}

	Event::Event(const Type& type, const ControlKey& key, const ControlKeyState& state)
	{
		_def.type = type;
		_def.key = key;
		_def.state = state;
		_def.idDevice = 0;
		ComputeHash();
	}

	Event::Event(const Definition& def)
	{
		_def = def;
		ComputeHash();
	}

	Event::Event()
	{
		_def.Reset();
		ComputeHash();
	}

	void Event::Reset()
	{
		_def.type = Event::Type::NONE;
		_def.key = ControlKey::KEY_NONE;
		_def.state = ControlKeyState::NONE;
		_def.idDevice = 0;
		_hash = 0;
	}

	Event::Type Event::GetTypeFromControlKey(const ControlKey& key)
	{
		if (key > ControlKey::KEY_START&& key < ControlKey::KEY_END)
			return (Event::Type::KEY);
		else if (key > ControlKey::MOUSE_START&& key < ControlKey::MOUSE_END)
			return (Event::Type::MOUSE);
		else if (key > ControlKey::WINDOW_START&& key < ControlKey::WINDOW_END)
			return (Event::Type::WINDOW);
		else
			return (Event::Type::NONE);
	}

	ControlKeyState Event::GetDefaultControlKeyStateFromControlKey(const ControlKey& key)
	{
		if (key > ControlKey::KEY_START&& key < ControlKey::KEY_END)
			return (ControlKeyState::PRESSED);
		else if (key > ControlKey::MOUSE_START&& key < ControlKey::MOUSE_END)
			return (ControlKeyState::PRESSED);
		else if (key > ControlKey::WINDOW_START&& key < ControlKey::WINDOW_END)
			return (ControlKeyState::NONE);
		return (ControlKeyState::NONE);
	}

	EVENT_TYPE_HASH_ Event::GetHash() const
	{
		return (_hash);
	}

	void Event::SetDefinition(const Definition& def)
	{
		_def = def;
		ComputeHash();
	}

	const Event::Definition& Event::GetDefinition() const
	{
		return (_def);
	}

	void Event::SetType(const Type& type)
	{
		_def.type = _def.type;
		ComputeHash();
	}

	const Event::Type& Event::GetType() const
	{
		return (_def.type);
	}

	void Event::SetControlKey(const ControlKey& key)
	{
		_def.key = key;
		ComputeHash();
	}

	const ControlKey& Event::GetControlKey() const
	{
		return (_def.key);
	}

	void Event::SetControlKeyState(const ControlKeyState& state)
	{
		_def.state = state;
		ComputeHash();
	}

	const ControlKeyState& Event::GetControlKeyState() const
	{
		return (_def.state);
	}

	void Event::SetIDDevice(const uint32_t& id)
	{
		_def.idDevice = id;
		ComputeHash();
	}

	const uint32_t& Event::GetIDDevice() const
	{
		return (_def.idDevice);
	}


	void Event::ComputeHash()
	{
		_hash = (uint64_t)_def.idDevice;
		_hash << 32;
		_hash += _def.type;
		_hash += (uint64_t)_def.key << 8;
		_hash += (uint64_t)_def.state << 24;
		// std::cout << "Computed Hash (" << _hash << ") From (" << ToString() << ")" << std::endl;
	}

	void Event::Definition::Reset()
	{
		idDevice = 0;
		key = KEY_NONE;
		type = Event::Type::NONE;
		state = ControlKeyState::NONE;
	}
}
