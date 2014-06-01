var new_post = {
	editor : {},

	setSavedButtonState : function()
	{
		$("#on-btn-post").attr("class", "btn btn-success");	
		$("#on-btn-post-icon").attr("class", "icon fa fa-save");
		$("#on-btn-post-text").html("Сохранено");
	},

	setUnsavedButtonState : function()
	{
		$("#on-btn-post").attr("class", "btn btn-default");		
		$("#on-btn-post-icon").attr("class", "icon fa fa-pencil");
		$("#on-btn-post-text").html("Сохранить заметку");	
	}
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

	$(editor).keydown(function(e)
	{
		new_post.setUnsavedButtonState();
		zenapi.clearAlerts();
	});
});

$(window).resize(function(e)
{
	var height = $(this).height();
	$("#text").css("height", (height - 306) + "px");
	new_post.editor.reflow();
});

$("#on-btn-post").click(function(e)
{
	e.preventDefault();

	var title = $("#title").val();
	var text = $("#text").val();
	var pass = $("#password").val();
	var passconfirm = $("#password-confirm").val();

	if (title.length == 0)
	{
		$.notify("Вы должны обязательно ввести заголовок заметки.", 
		{
			position : "top right"
		});

		$("#title").focus();
		return;
	}	

	if (pass.length > 0 && pass != passconfirm)
	{
		$.notify("Введённые вами пароли не совпадают.", 
		{
			position : "top right"
		});
		$("password-confirm").focus();
		return;
	}

	var result = zen.postNote(title, text, pass);

	if (result.success)
	{
		new_post.setSavedButtonState();
	}
	else
	{
		$.notify(result.message, 
		{
			position : "top right"
		});
	}
});

$("#title").keydown(function(e)
{
	new_post.setUnsavedButtonState();
	zenapi.clearAlerts();
});

$("#password, #password-confirm").keydown(function(e)
{
	zenapi.clearAlerts();
});