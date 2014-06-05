#pragma once

namespace ZenDiary
{
	namespace App
	{
		class WebWindow : public Awesomium::WebViewListener::View
		{
		public:
			virtual ~WebWindow();

			WebWindow(const WebWindow &a) = delete;
			WebWindow &operator = (const WebWindow &a) = delete;

			static WebWindow *Create(const std::string &title, size_t width, size_t height, Awesomium::WebSession *session = nullptr);
			Awesomium::WebView *GetWebView();

			ZD_STATUS ToggleFullscreen();
			bool IsFullscreenMode() const;

			virtual void OnChangeTitle(Awesomium::WebView* caller,
				const Awesomium::WebString& title) override final;

			virtual void OnChangeAddressBar(Awesomium::WebView* caller,
				const Awesomium::WebURL& url) override final;

			virtual void OnChangeTooltip(Awesomium::WebView* caller,
				const Awesomium::WebString& tooltip) override final;

			virtual void OnChangeTargetURL(Awesomium::WebView* caller,
				const Awesomium::WebURL& url) override final;

			virtual void OnChangeCursor(Awesomium::WebView* caller,
				Awesomium::Cursor cursor) override final;

			virtual void OnChangeFocus(Awesomium::WebView* caller,
				Awesomium::FocusedElementType focused_type) override final;

			virtual void OnShowCreatedWebView(Awesomium::WebView* caller,
				Awesomium::WebView* new_view, const Awesomium::WebURL& opener_url,
				const Awesomium::WebURL& target_url, const Awesomium::Rect& initial_pos,
				bool is_popup) override final;

			virtual void OnAddConsoleMessage(Awesomium::WebView* caller,
				const Awesomium::WebString& message, int line_number,
				const Awesomium::WebString& source) override final;

		private:
			WebWindow(const std::string &title, size_t width, size_t height, Awesomium::WebSession *session);

			static void PlatformInit();
			static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			static const std::string m_window_class;
			static std::vector<Awesomium::WebView*> m_views;

			Awesomium::WebView *m_web_view;
			HWND m_hwnd;

			bool m_fullscreen;
			RECT m_old_window_pos;
		};
	}
}