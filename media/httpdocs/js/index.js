var new_post = {

	current_note_id : 0,

	in_preview : false,

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
	},

	changePreviewButtonIcon : function()
	{
		if (new_post.in_preview)
		{
			var span = $("<span>").addClass("fa fa-code");
			$("#on-btn-preview").html(span);		
		}
		else
		{			
			var span = $("<span>").addClass("fa fa-desktop");
			$("#on-btn-preview").html(span);
		}
	},

	switchToPreview : function()
	{
		new_post.in_preview = true;

		new_post.changePreviewButtonIcon();

		var md_text = $("#text").val();
		var conv = new Markdown.Converter();
		var html_text = conv.makeHtml(md_text); 

		var textarea_height = $("#text").height() + 14;
		var textarea_width = $("#text").width() + 25;

		$("#text-preview").css("height", textarea_height + "px");
		$("#text-preview").css("width", textarea_width + "px");

		$("#text-preview").html(html_text);

		$("#text").fadeOut(zenapi.animation_duration, function()
		{
			$("#text-preview").fadeIn(zenapi.animation_duration);
		});
	},

	switchToEdit : function()
	{
		new_post.in_preview = false;

		new_post.changePreviewButtonIcon();

		$("#text-preview").fadeOut(zenapi.animation_duration, function()
		{
			$("#text").fadeIn(zenapi.animation_duration);
		});		
	},

	surroundSelection : function(object, open_tag, close_tag)
	{
		if (close_tag == undefined)
		{
			close_tag = "";
		}

		var start = $(object)[0].selectionStart;
		var end = $(object)[0].selectionEnd;	

		var old_value = $(object).val();

		var left = old_value.substring(0, start);
		var middle = old_value.substring(start, end);
		var right = old_value.substring(end);
		
		var value = left + open_tag + middle + close_tag + right;

		$(object).val(value);

		$(object).focus();

		var selection_index = start + open_tag.length;

		$(object)[0].setSelectionRange(start + open_tag.length, end + open_tag.length);
	},

	insertToEditor : function(object, what)
	{
		var start = $(object)[0].selectionStart;
		var end = $(object)[0].selectionEnd;	

		var old_value = $(object).val();

		var left = old_value.substring(0, start);
		var right = old_value.substring(end);
		
		var value = left + what + right;

		$(object).val(value);		

		if (start != end)
		{
			$(object)[0].setSelectionRange(start, start + what.length);
		}
		else
		{
			$(object)[0].setSelectionRange(start + what.length, start + what.length);
		}

		$(object).focus();
	}
};

$("#text").keydown(function(e)
{
	new_post.setUnsavedButtonState();
	zenapi.clearAlerts();

	if (e.keyCode == 83 && e.ctrlKey)
	{
		e.preventDefault();
		$("#on-btn-post").click();
	}	

	if (this.tabs_count == undefined)
	{
		this.tabs_count = 0;
	}

	var old_content_text = $("#text").val();

	var code = e.keyCode || e.which;

	if (code == 9)
	{
		e.preventDefault();	
		var start = $("#text")[0].selectionStart;
		var end = $("#text")[0].selectionEnd;

		var old_value = $("#text").val();

		var left = old_value.substring(0, start);
		var middle = old_value.substring(start, end);
		var right = old_value.substring(end);		

		var val = left + "\t" + right;

		$(this).val(val);			

		var selection_index = start + 1;

		$("#text")[0].setSelectionRange(selection_index, selection_index);
	}
	else if (code == 13)
	{	
		e.preventDefault();

		var start = $("#text")[0].selectionStart;
		var end = $("#text")[0].selectionEnd;	

		var old_value = $("#text").val();

		var left = old_value.substring(0, start);
		var middle = old_value.substring(start, end);
		var right = old_value.substring(end);			

		var lines = left.split("\n");

		this.left_prefix = "";

		if (lines.length > 0)
		{
			var line = lines[lines.length - 1];
			this.tabs_count = 0;
			for (var i = 0; i < line.length; i++)
			{
				if (line[i] == '\t')
				{
					this.tabs_count++;
				}
				else
				{
					break;
				}
			}
		}

		if (this.left_prefix.length > 0 && this.left_prefix == this.last_left_line)
		{
			var length = this.left_prefix.length;	
			left = left.substring(0, left.length - length);	
			this.tabs_count--;	
		}

		var val = left + "\n";

		var selection_index = start + 1;	

		for (var i = 0; i < this.tabs_count; i++)
		{
			val += "\t";		
			selection_index++;
		}

		if (this.left_prefix.length > 0 && this.left_prefix != this.last_left_line)
		{
			right = this.left_prefix + right;
			selection_index = selection_index + this.left_prefix.length;			
		}

		val = val + right;

		$(this).val(val);			

		$("#text")[0].setSelectionRange(selection_index, selection_index);
	}
});

$(document).ready(function()
{
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
					$("#text").val(result.note);
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
				$("#text").val(result.note);
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

$(document).keydown(function(e)
{
	if (e.keyCode == 80 && e.altKey)
	{
		$("#on-btn-preview").click();
	}
});

$(window).resize(function(e)
{
	var height = $(this).height();
	$("#text").css("height", (height - 351) + "px");	

	var textarea_height = $("#text").height() + 14;
	var textarea_width = $("#text").width() + 25;

	$("#text-preview").css("height", textarea_height + "px");
	$("#text-preview").css("width", textarea_width + "px");
});

$(window).resize();

$("#on-btn-post").click(function(e)
{
	e.preventDefault();	

	var title = $("#title").val();
	var text = $("#text").val();
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

$("#on-btn-preview").click(function(e)
{
	if (new_post.in_preview)
	{
		new_post.switchToEdit();
	}
	else
	{
		new_post.switchToPreview();
	}
});

$("#on-btn-markdown-help").click(function(e)
{
	e.preventDefault();

	var dialog_message = zen.getTemplate("../media/httpdocs/modal/markdown-help.html");

	var dialog = new BootstrapDialog({
		title : "Синтаксис markdown",
		message : dialog_message,
		type : BootstrapDialog.TYPE_DEFAULT,		
	});

	dialog.realize();	
	dialog.open();
});

$("#on-btn-qsave").click(function(e)
{
	e.preventDefault();
	$("#on-btn-post").click();
});

$("#on-btn-image").click(function(e)
{
	e.preventDefault();

	var fname = zen.openFileDialog("Все файлы (*.*)", "*.*");
	if (fname != undefined)
	{
		fname = "asset://zen-diary/" + fname;

		var md_image = "![](" + fname + ")";

		new_post.insertToEditor("#text", md_image);
	}
});

$("#on-btn-youtube").click(function(e)
{
	e.preventDefault();

	var dialog_message = zen.getTemplate("../media/httpdocs/templates/input-link.html");	

	var dialog = new BootstrapDialog({
		title : "Вставка видеоклипа",
		message : dialog_message,
		type : BootstrapDialog.TYPE_DEFAULT,
		buttons : [
			{
				id : "on-btn-enter-link",
				label : "OK"
			}
		],

		onshown : function(dlg)
		{
			$("#input-link").focus();
		}			
	});

	dialog.realize();

	var btn_enter_link = dialog.getButton("on-btn-enter-link");
	btn_enter_link.click({}, function(e)
	{			
		var link = $("#input-link").val();		
		dialog.close();

		if (link != undefined && link.length > 0)
		{
			var youtube_iframe = "<iframe source = \"";

			youtube_iframe = youtube_iframe + link;

			youtube_iframe = youtube_iframe + "\"></iframe>";

			new_post.insertToEditor("#text", youtube_iframe);
		}
	});

	dialog.open();
});