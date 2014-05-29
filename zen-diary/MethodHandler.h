#pragma once


namespace ZenDiary
{
	namespace App
	{
		class MethodHandler : public Awesomium::JSMethodHandler
		{
		public:
			typedef std::pair<int, Awesomium::WebString> object_method_key_t;
			typedef std::map<object_method_key_t, JSDelegate> bound_method_map_t;
			typedef std::map<object_method_key_t, JSDelegateWithRetval> bound_method_with_retval_map_t;

		public:
			MethodHandler();
			void Bind(Awesomium::JSObject &object, const Awesomium::WebString &name, JSDelegate callback);
			void BindWithRetval(Awesomium::JSObject &object, const Awesomium::WebString &name, JSDelegateWithRetval callback);

			virtual void OnMethodCall(Awesomium::WebView* caller,
				unsigned int remote_object_id, const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args) override final;

			virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
				unsigned int remote_object_id, const Awesomium::WebString& method_name,
				const Awesomium::JSArray& args) override final;

		private:
			bound_method_map_t m_bound_methods;
			bound_method_with_retval_map_t m_bound_methods_with_retval;
		};
	}
}