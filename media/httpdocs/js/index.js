var new_post = {

	current_note_id : 0,
	editor : {},

	in_preview : false,

	setSavedButtonState : function()
	{
		/*$("#on-btn-post").attr("class", "btn btn-success");	
		$("#on-btn-post-icon").attr("class", "icon i i-checkmark2");
		$("#on-btn-post-text").html("Сохранено");*/
	},

	setUnsavedButtonState : function()
	{
		/*$("#on-btn-post").attr("class", "btn btn-default");		
		$("#on-btn-post-icon").attr("class", "icon fa fa-save");
		$("#on-btn-post-text").html("Сохранить заметку");	*/
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

		var md_text = new_post.editor.getValue();
		var conv = new Markdown.Converter();
		var html_text = conv.makeHtml(md_text); 

		var textarea_height = $(".CodeMirror").height();
		var textarea_width = $(".CodeMirror").width();

		$("#text-preview").css("height", textarea_height + "px");
		$("#text-preview").css("width", textarea_width + "px");

		$("#text-preview").html(html_text);

		$(".CodeMirror").fadeOut(zenapi.animation_duration, function()
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
			$(".CodeMirror").fadeIn(zenapi.animation_duration);
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

	insertToEditor : function(what)
	{
		new_post.editor.replaceSelection(what);		
		new_post.editor.focus();
	},

	clearAllErrors : function()
	{
		var marks = new_post.editor.getAllMarks();
		for (var i = 0; i < marks.length; i++)
		{
			marks[i].clear();
		}
	},

	markTextAsError : function(line_from, char_from, line_to, char_to)
	{
		new_post.editor.markText({
			line : line_from,
			ch : char_from
		}, {
			line : line_to,
			ch : char_to
		}, {
			className : "spell-error"
		});
	},

	moveCursor : function(delta)
	{
		var cursor = new_post.editor.getCursor();
		new_post.editor.setCursor({
			line : cursor.line,
			ch : cursor.ch + delta
		});	
	}
};


zen_handlers.drag_n_drop = function(fname)
{		
	var mime_type = zen.getMimeType(fname);
	if (mime_type != undefined)
	{
		const image_type = "image/";
		const text_type = "text/";

		if (mime_type.length > image_type.length &&
			mime_type.substr(0, image_type.length) == image_type)
		{
			fname = "asset://zen-diary/" + zen.uriEncode(fname);

			var md_image = "![](" + fname + ")";

			new_post.insertToEditor(md_image);
		}
		else if (mime_type.length > text_type.length &&
			mime_type.substr(0, text_type.length) == text_type)
		{
			var filetext = zen.getFileContent(fname);
			if (filetext != undefined)
			{
				new_post.insertToEditor(filetext);
			}	
		}				
	}		
};

$(window).resize(function(e)
{
	var height = $(this).height();
	$(".CodeMirror").css("height", (height - 351) + "px");	
	$(".CodeMirror-gutters").css("height", (height - 351) + "px");	

	var textarea_height = $(".CodeMirror").height();
	var textarea_width = $(".CodeMirror").width();

	$("#text-preview").css("height", textarea_height + "px");
	$("#text-preview").css("width", textarea_width + "px");
});


$(document).ready(function()
{
	new_post.editor = CodeMirror.fromTextArea(document.getElementById("text"),
	{
		mode : "markdown",
		lineNumbers : zen.getShowLineNumbers(),
		theme : "default",
		extraKeys : {
			"Enter" : "newlineAndIndentContinueMarkdownList",
			"Ctrl-S" : function(cm)
			{
				$("#on-btn-qsave").click();
			}
		},
		autoCloseBrackets: zen.isAutocloseBrackets()
	});

	$(window).resize();

	var preview_width = zen.getPreviewWidth();
	$("#text-preview").css("max-width", preview_width + "px");

	var search = window.location.search;

	if (search.length > 0 && (search = search.substr(1)).length > 0)
	{
		var encrypted = zen.isNoteEncrypted(search);
		if (encrypted)
		{
			var dialog_message = zen.getTemplate("../media/httpdocs/templates/input-password.html");

			var dialog = new BootstrapDialog({
				title : "{{script.index.note-encrypted}}",
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
					
					new_post.editor.setValue(result.note);


					$("#password").val(password);
					$("#password-confirm").val(password);
					new_post.current_note_id = result.id;

					$("#page-title").html("{{ui.index.page.title.edit}}");

					$.notify("{{code.index.decrypted-successfully}}", 
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
				
				new_post.editor.setValue(result.note);

				new_post.current_note_id = result.id;

				$("#page-title").html("{{ui.index.page.title.edit}}");
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

$("#on-btn-post").click(function(e)
{
	e.preventDefault();	

	var title = $("#title").val();
	var text = new_post.editor.getValue();
	var pass = $("#password").val();
	var passconfirm = $("#password-confirm").val();		

	if (pass.length > 0 && pass != passconfirm)
	{
		$.notify("{{script.index.passwords-doesnt-match}}", 
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

		$.notify("{{script.index.note-saved}}", 
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

$(document).on("keydown", "#input-link", function(e)
{
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-btn-enter-link").click();
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

	var dialog = new BootstrapDialog({
		title : "{{script.index.markdown-syntax}}",		
		message :  $('<div></div>').load('modal/markdown-help.html'),
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

	var fname = zen.openFileDialog("{{script.index.image-files}}", "*.art; *.bm; *.bmp; *.dwg; *.dxf; *.fif; *.flo; *.fpx; *.g3; *.gif; *.ico; *.ief; *.iefs; *.jfif; *.jfif-tbnl; *.jpe; *.jpeg; *.jpg; *.jps; *.jut; *.nap; *.naplps; *.nif; *.niff; *.pbm; *.pct; *.pcx; *.pgm; *.pic; *.pict; *.png; *.pnm; *.ppm; *.qif; *.qti; *.qtif; *.ras; *.rast; *.rf; *.rgb; *.rp; *.svf; *.tif; *.tiff; *.turbot; *.wbmp; *.x-png; *.xbm; *.xif; *.xpm; *.xwd", "{{script.index.all-files}}", "*.*");
	if (fname != undefined)
	{
		fname = "asset://zen-diary/" + zen.uriEncode(fname);

		var md_image = "![](" + fname + ")";

		new_post.insertToEditor(md_image);
	}
});

$("#on-btn-youtube").click(function(e)
{
	e.preventDefault();

	var dialog_message = zen.getTemplate("../media/httpdocs/templates/input-link.html");	

	var dialog = new BootstrapDialog({
		title : "{{script.index.youtube-insert}}",
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
			var youtube_url = "http://www.youtube.com/watch?v=";
			var youtube_short_url = "http://youtu.be/";

			var index = link.lastIndexOf(youtube_url);
			if (index == -1)
			{
				index = link.lastIndexOf(youtube_short_url);
				if (index >= 0)
				{
					index = index + youtube_short_url.length;
				}
			}
			else
			{
				index = index + youtube_url.length;
			}

			if (index >= 0)			
			{
				var code = link.substr(index);

				var youtube_iframe = "<iframe width=\"420\" height=\"315\" src=\"http://www.youtube.com/embed/";

				youtube_iframe = youtube_iframe + code;

				youtube_iframe = youtube_iframe + "\" frameborder=\"0\" allowfullscreen></iframe>";

				new_post.insertToEditor(youtube_iframe);
			}
		}
	});

	dialog.open();
});

$("#on-btn-export-to-html").click(function(e)
{
	e.preventDefault();
	var text = new_post.editor.getValue();
	zenapi.exportToHtml(text);
});

$("#on-btn-spellcheck").click(function(e)
{
	e.preventDefault();	

	var lines_count = new_post.editor.lineCount();
	for (var i = 0; i < lines_count; i++)
	{
		var line = new_post.editor.getLine(i);

		var result = zen.spellCheck(line);		

		for (var j = 0; j < result.length; j++)
		{
			var mark = result[j];	

			new_post.markTextAsError(i, mark.pos, i, mark.pos + mark.len);		
		}
	}
});

$("#on-btn-clear-spellcheck").click(function(e)
{
	e.preventDefault();

	new_post.clearAllErrors();
});

$("#on-btn-link").click(function(e)
{
	e.preventDefault();
	new_post.insertToEditor("[]()");
	new_post.moveCursor(-1);	
});

$("#on-btn-h1").click(function(e)
{
	new_post.insertToEditor("# ");
});

$("#on-btn-h2").click(function(e)
{
	new_post.insertToEditor("## ");
});

$("#on-btn-h3").click(function(e)
{
	new_post.insertToEditor("### ");
});

$("#on-btn-h4").click(function(e)
{
	new_post.insertToEditor("#### ");
});

$("#on-btn-h5").click(function(e)
{
	new_post.insertToEditor("##### ");
});

$("#on-btn-h6").click(function(e)
{
	new_post.insertToEditor("###### ");
});

$("#on-btn-bold").click(function(e)
{
	new_post.insertToEditor("****");
	new_post.moveCursor(-2);
});

$("#on-btn-italic").click(function(e)
{
	new_post.insertToEditor("**");
	new_post.moveCursor(-1);
});

$("#on-btn-code").click(function(e)
{
	new_post.insertToEditor("``");
	new_post.moveCursor(-1);
});

$("#on-btn-quote").click(function(e)
{
	new_post.insertToEditor("> ");
});

$("#on-btn-ulist").click(function(e)
{
	new_post.insertToEditor("* ");
});

$("#on-btn-olist").click(function(e)
{
	new_post.insertToEditor("1. ");
});

$("#on-btn-hr").click(function(e)
{
	new_post.insertToEditor("-----");
});

$("#on-btn-save-as").click(function(e)
{
	var filename = zen.saveFileDialog("{{general.file-dialog.txt}}", "*.txt", "{{general.file-dialog.md}}", "*.md", "{{general.file-dialog.all}}", "*.*");
	if (filename != undefined)
	{
		var md_text = new_post.editor.getValue();

		md_text = zenapi.replaceAll("\n", "\r\n", md_text);

		var result = zen.setFileContent(filename, md_text);
		if (result != undefined)
		{
			$.notify("{{script.index.note-saved-as}} " + filename + ".", 
			{
				position : "top right",
				className : "success"
			});
		}
		else
		{
			$.notify("{{script.index.failed-to-export}} " + filename + ".", 
			{
				position : "top right"				
			});
		}
	}
});

$("#on-btn-open-as").click(function(e)
{
	var filename = zen.openFileDialog("{{general.file-dialog.txt}}", "*.txt", "{{general.file-dialog.md}}", "*.md", "{{general.file-dialog.all}}", "*.*");
	if (filename != undefined)
	{
		var filetext = zen.getFileContent(filename);
		if (filetext != undefined)
		{			
			new_post.insertToEditor(filetext);
		}		
	}
});