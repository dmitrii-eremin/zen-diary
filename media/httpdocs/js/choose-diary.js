var choose_diary = {
	addDiary : function(diary)
	{
		var li = $("<li>").addClass("list-group-item diary-item").attr("path", diary.filename);

		var span_right = $("<span>").addClass("pull-right");
		var span_filename = $("<span>").addClass("diary-filename").append(diary.filename);
		$(span_right).append(span_filename);

		var a_delete = $("<a>").attr("href", "javascript:void(0)").addClass("on-btn-delete-diary").
					attr("path", diary.filename).attr("name", diary.name);					
		var i_delete_icon = $("<i>").addClass("fa fa-times fa-fw m-r-xs");
		$(a_delete).append(i_delete_icon);

		$(span_right).append(a_delete);

		var a_set_diary = $("<a>").attr("href", "javascript:void(0)").addClass("on-btn-set-diary").attr("path", diary.filename);
		$(a_set_diary).append($("<div>").addClass("clear").append($("<strong>").append(diary.name)));

		$(li).append(span_right).append(a_set_diary);

		$("#diaries-list").append(li);
	}
};

$(document).ready(function(e)
{
	var db_list = zen.getDatabaseList();
	for (var i = 0; i < db_list.length; i++)
	{
		choose_diary.addDiary(db_list[i]);
	}

	var path = zen.getDatabasePath();	

	$(".diary-item").each(function()
	{
		$(this).removeClass("bg-primary");
		var item_path = $(this).attr("path");
		if (item_path == path)
		{
			$(this).addClass("bg-primary");
		}
	});
});	

$("#new-diary-name").keydown(function(e)
{
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-btn-new-diary").click();
	}
});

$(document).on("click", ".on-btn-set-diary", function(e)
{
	var path = $(this).attr("path");

	$(".diary-item").each(function()
	{
		$(this).removeClass("bg-primary");
		var item_path = $(this).attr("path");
		if (item_path == path)
		{
			$(this).addClass("bg-primary");
		}
	});

	zen.setDatabasePath(path);

	$.notify("{{script.choose-diary.default-diary-changed}}", 
	{
		position: "top right",
		className : "success"
	});
});

$(document).on("click", ".on-btn-delete-diary", function(e)
{
	e.preventDefault();

	if (zen.getDatabaseListSize() == 1)
	{
		$.notify("{{script.choose-diary.cant-delete-last-diary}}", 
		{
			position: "top right"
		});
		return;
	}

	var path = $(this).attr("path");

	if (zen.getDatabasePath() == path)
	{
		$.notify("{{script.choose-diary.failed-to-delete-active-diary}}",
		{
			pisiton: "top right"
		});
		return;
	}

	var cb = function()
	{		
		$(".diary-item").each(function()
		{
			var item_path = $(this).attr("path");
			if (item_path == path)
			{
				$(this).remove();
				zen.deleteFile(item_path);
				zen.removeItemFromDatabaseList(item_path);
			}
		});

		$.notify("{{script.choose-diary.diary-deleted}}", 
		{
			position: "top right",
			className : "success"
		});
	};

	new BootstrapDialog({
		title : "{{general.confirm}}",
		message : "{{script.choose-diary.are-you-shure-about-delete}}",
		type : BootstrapDialog.TYPE_DEFAULT,
		closable : false,
		data : {
			callback : cb
		},

		buttons : [{
			label : "{{general.cancel}}",
			action : function(dlg)
			{
				dlg.close();
			}
		},
		{
			label : "OK",
			cssClass : "btn-primary",
			action : function(dlg)
			{
				typeof dlg.getData("callback") === "function" && dlg.getData("callback")();
				dlg.close();
			}
		}
		]
	}).open();	
});

$("#on-btn-new-diary-view").click(function(e)
{
	e.preventDefault();

	var filename = zen.saveFileDialog("{{general.file-dialog.sqlite3}}", "*.sqlite3");
	if (filename != undefined)
	{
		$("#new-diary-name").val(filename);
	}
});

$("#on-btn-new-diary").click(function(e)
{
	e.preventDefault();

	var new_diary_name = $("#new-diary-name").val();
	if (new_diary_name.length == 0)
	{
		$.notify("{{script.choose-diary.input-filename}}", 
		{
			position: "top right"
		});
		$("#new-diary-name").focus();
		return;
	}

	var already_exist = false;

	$(".diary-item").each(function()
	{
		var path = $(this).attr("path");
		if (path == new_diary_name)
		{
			$.notify("{{script.choose-diary.already-exist}}", 
			{
				position: "top right"
			});

			already_exist = true;

			return false;
		}
	});

	if (already_exist)
	{
		return;
	}

	var diary_name = new_diary_name;
	var index = diary_name.lastIndexOf("\\");
	if (index == -1)
	{
		index = diary.lastIndexOf("/");
	}

	if (index != -1)
	{
		diary_name = diary_name.substr(index + 1);
	}	

	choose_diary.addDiary({
		name : diary_name,
		filename : new_diary_name
	});

	zen.addItemToDatabaseList(new_diary_name, diary_name);

	$("#new-diary-name").val("");

	$.notify("{{script.choose-diary.created-successfully}}", 
	{
		position: "top right",
		className : "success"
	});
});