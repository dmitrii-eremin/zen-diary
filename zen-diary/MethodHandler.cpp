#include "stdafx.h"
#include "MethodHandler.h"

namespace ZenDiary
{
	namespace App
	{
		MethodHandler::MethodHandler()
		{

		}

		void MethodHandler::Bind(Awesomium::JSObject &object, const Awesomium::WebString &name, JSDelegate callback)
		{
			if (object.type() == Awesomium::kJSObjectType_Local)
			{
				return;
			}

			object.SetCustomMethod(name, true);

			object_method_key_t key(object.remote_id(), name);
			m_bound_methods[key] = callback;
		}

		void MethodHandler::BindWithRetval(Awesomium::JSObject &object, const Awesomium::WebString &name, JSDelegateWithRetval callback)
		{
			if (object.type() == Awesomium::kJSObjectType_Local)
			{
				return;
			}

			object.SetCustomMethod(name, true);

			object_method_key_t key(object.remote_id(), name);
			m_bound_methods_with_retval[key] = callback;
		}

		void MethodHandler::OnMethodCall(Awesomium::WebView* caller,
			unsigned int remote_object_id, const Awesomium::WebString& method_name,
			const Awesomium::JSArray& args)
		{
			bound_method_map_t::iterator i = m_bound_methods.find(object_method_key_t(remote_object_id, method_name));
			if (i != m_bound_methods.end())
			{
				i->second(caller, args);
			}
		}

		Awesomium::JSValue MethodHandler::OnMethodCallWithReturnValue(Awesomium::WebView* caller,
			unsigned int remote_object_id, const Awesomium::WebString& method_name,
			const Awesomium::JSArray& args)
		{
			bound_method_with_retval_map_t::iterator i = m_bound_methods_with_retval.find(object_method_key_t(remote_object_id, method_name));
			if (i == m_bound_methods_with_retval.end())
			{
				return Awesomium::JSValue::Undefined();
			}

			return i->second(caller, args);
		}
	}
}