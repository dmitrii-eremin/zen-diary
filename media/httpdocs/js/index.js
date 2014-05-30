var index = {
	editor : {}
};

$(document).ready(function()
{
	index.editor = new EpicEditor({
		container : "text",
		clientSideStorage : false,
		useNativeFullscreen : true,		

		button : {
			preview : true,
			fullscreen : true,
			bar : "auto"
		},

		focusOnLoad : false,

		shortcut : {
			modifier : 18,
			fullscreen : 70,
			preview : 80
		},

		theme : {
			base : "/themes/base/epiceditor.css",
			preview : "/themes/preview/bartik.css",
			editor : "/themes/editor/epic-light.css"
		},

		string : {
			togglePreview : "Предпросмотр",
			toggleEdit : "Редактирование",
			toggleFullscreen : "Развернуть во всё окно"
		}
	}).load();
});

$(window).resize(function(e)
{
	var height = $(this).height();
	$("#text").css("height", (height - 235) + "px");
	index.editor.reflow();
});