var new_post = {
	editor : {},

	current_note_id : 0,

	setSavedButtonState : function()
	{
		$("#on-btn-post").attr("class", "btn btn-success");	
		$("#on-btn-post-icon").attr("class", "icon i i-checkmark2");
		$("#on-btn-post-text").html("Сохранено");
	},

	setUnsavedButtonState : function()
	{
		$("#on-btn-post").attr("class", "btn btn-default");		
		$("#on-btn-post-icon").attr("class", "icon fa fa-save");
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

		if (e.keyCode == 83 && e.ctrlKey)
		{
			e.preventDefault();
			$("#on-btn-post").click();
		}
	});

	var search = window.location.search;

	if (search.length > 0 && (search = search.substr(1)).length > 0)
	{
		var encrypted = zen.isNoteEncrypted(search);
		if (encrypted)
		{
			var dialog_message = zen.getTemplate("../media/httpdocs/templates/input-password.html");

			var dialog = new BootstrapDialog({
				title : "Заметка зашифрована",
				message : dialog_message,
				type : BootstrapDialog.TYPE_DEFAULT,
				buttons : [
					{
						id : "on-btn-enter-password",
						label : "OK"
					}
				],

				onshown : function(dlg)
				{
					$("#input-note-password").focus();
				}			
			});

			dialog.realize();

			var btn_enter_pasword = dialog.getButton("on-btn-enter-password");
			btn_enter_pasword.click({}, function(e)
			{			
				var password = $("#input-note-password").val();		
				var result = zen.getNote(search, password);			
				if (result.success)
				{
					$("#title").val(result.title);
					new_post.editor.importFile(null, result.note);
					$("#password").val(password);
					$("#password-confirm").val(password);
					new_post.current_note_id = result.id;

					$("#page-title").html("Редактирование заметки");

					$.notify("Заметка расшифрована успешно.", 
					{
						position: "top right",
						className : "success"
					});
				}
				else
				{
					$.notify(result.message, 
					{
						position: "top right"
					});
				}

				dialog.close();
			});

			dialog.open();
		}
		else
		{
			var result = zen.getNote(search);			
			if (result.success)
			{
				$("#title").val(result.title);
				new_post.editor.importFile(null, result.note);
				new_post.current_note_id = result.id;

				$("#page-title").html("Редактирование заметки");
			}
			else
			{
				$.notify(result.message, 
				{
					position: "top right"
				});
			}
		}
	}
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

	var editor = new_post.editor;

	var title = $("#title").val();
	var text = editor.exportFile();
	var pass = $("#password").val();
	var passconfirm = $("#password-confirm").val();		

	if (pass.length > 0 && pass != passconfirm)
	{
		$.notify("Введённые вами пароли не совпадают.", 
		{
			position : "top right"
		});
		$("password-confirm").focus();
		return;
	}

	var result = {};

	if (new_post.current_note_id == 0)
	{
		result = zen.postNote(title, text, pass);		
	}
	else
	{
		result = zen.updateNote(new_post.current_note_id, title, text, pass);		
	}

	if (result.success)
	{
		new_post.setSavedButtonState();

		if (new_post.current_note_id == 0)
		{
			new_post.current_note_id = result.id;
		}

		$.notify("Заметка сохранена.", 
		{
			position : "top right",
			className : "success"
		});
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

$(document).on("keydown", "#input-note-password", function(e)
{
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-btn-enter-password").click();
	}
});