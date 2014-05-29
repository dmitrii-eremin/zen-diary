#pragma once


namespace ZenDiary
{
	namespace App
	{
		class MenuHandler : public Awesomium::WebViewListener::Menu
		{
		public:
			virtual void OnShowPopupMenu(Awesomium::WebView* caller,
				const Awesomium::WebPopupMenuInfo& menu_info) override final;

			virtual void OnShowContextMenu(Awesomium::WebView* caller,
				const Awesomium::WebContextMenuInfo& menu_info) override final;
		};
	}
}