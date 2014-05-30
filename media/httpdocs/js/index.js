var new_post = {
	editor : {}
};

$(document).ready(function()
{
	new_post.editor = new EpicEditor({
		container : "text",
		clientSideStorage : false,
		useNativeFullscreen : true,		

		button : {
			preview : true,
			fullscreen : false,
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
			editor : "/themes/editor/epic-light-user.css"
		},

		string : {
			togglePreview : "Предпросмотр",
			toggleEdit : "Редактирование",
			toggleFullscreen : "Развернуть во всё окно"
		}
	}).load();

	var editor = new_post.editor.getElement("editor");

	editor.addEventListener("keydown", function(e)
	{
		$("#on-btn-post").attr("class", "btn btn-default");		

		$("#on-btn-post-icon").attr("class", "icon fa fa-pencil");

		$("#on-btn-post-text").html("Сохранить заметку");	
	});
});

$(window).resize(function(e)
{
	var height = $(this).height();
	$("#text").css("height", (height - 235) + "px");
	new_post
	.editor.reflow();
});

$("#on-btn-post").click(function(e)
{
	e.preventDefault();

	var result = {
		success : true
	};

	if (result.success)
	{
		$("#on-btn-post").attr("class", "btn btn-success");		

		$("#on-btn-post-icon").attr("class", "icon fa fa-save");

		$("#on-btn-post-text").html("Сохранено");
	}
});

$("#text").keydown(function(e)
{
	zen.alert("DF");
});