var zenapi = {
	animation_duration : 200,

	clearAlerts : function()
	{
		$(".alerts-zone .alert").addClass("old-alert");
		$(".old-alert").slideUp(zenapi.animation_duration);
	},

	getCorrectStr : function(num, s1, s2, s3)
	{
		var val = num % 100;
		if (val > 10 && val < 20)
		{
			return s3;
		}
		else
		{
			val = num % 10;
			if (val == 1)
			{
				return s1;
			}
			else if (val > 1 && val < 5)
			{
				return s2;
			}
			else
			{
				return s3;
			}
		}
	},

	replaceAll : function(find, replace, str)
	{
		return str.replace(new RegExp(find, "g"), replace);
	},

	exportToHtml : function(text)
	{		
		var conv = new Markdown.Converter();
		var html_text = conv.makeHtml(text); 

		html_text = zenapi.replaceAll("<br />", "<br />\r\n", html_text);

		var message = "<form><textarea class = \"form-control export-to-html-textarea\" style = \"width: 100%; height: 500px\">";
		message += html_text;
		message += "</textarea>";

		new BootstrapDialog({
			title : "{{script.general.html-code}}",
			message : message,
			type : BootstrapDialog.TYPE_DEFAULT,
			closable : true,			
		}).open();	
	}
};

$(function()
{
	var text_version = zen.getVersionString();
	var username = zen.getUsername();
	var users_count = zen.getUsersCount();

	$(".app-version").html(text_version);
	$(".app-username").html(username);

	if (users_count != undefined && users_count > 0)
	{
		$(".users-count").html(users_count);
		$(".users-count-people-word").html(			
			zenapi.getCorrectStr(users_count, "человек", "человека", "человек"));
		$(".users-count-wrapper").show();
	}

	var location = window.location.href;
	if (location.indexOf("/index.html") != -1)
	{
		$("#nav-menu-index").addClass("active");
	}
	else if (location.indexOf("/view-notes.html") != -1)
	{
		$("#nav-menu-diary").addClass("active");
	}
	else if (location.indexOf("/settings.html") != -1)
	{
		$("#nav-menu-settings").addClass("active");
	}
	else if (location.indexOf("/choose-diary.html") != -1)
	{
		$("#nav-menu-choose-diary").addClass("active");
	}
});

$(".on-btn-logout").click(function(e)
{
	e.preventDefault();
	zen.logoutUser();
	window.location.href = "signin.html";
});

$("a[data-toggle=\"ajax-modal\"]").click(function(e)
{
	e.preventDefault();
	var href = $(this).attr("href");

	var content = zen.getTemplate(href);
	

	$(".modal-background").remove();

	var modal = $("<div>").addClass("modal-background");
	$(modal).append(content);

	$("body").append(modal);

	$(modal).fadeIn(zenapi.animation_duration);
});

$(".awesomium-link").click(function(e)
{
	e.preventDefault();
	var path = $(this).attr("path");

	window.location.href = path;
});

$(document).on("click", "a", function(e)
{
	var href = $(this).attr("href");
	
	if ((href.length >= 7 && href.substr(0, 7) == "http://") ||
		(href.length >= 8 && href.substr(0, 8) == "https://"))
	{
		e.preventDefault();

		zen.shellExecute(href);
	}
});

$(document).on("click", ".modal-background", function()
{
	$(this).fadeOut(zenapi.animation_duration, function()
	{
		$(this).remove();
	});
});

$(document).on("click", ".export-to-html-textarea", function(e)
{
	$(this).select();
});

$(document).keydown(function(e)
{
	if (e.keyCode == 116)
	{
		e.preventDefault();
		window.location.reload();
	}
});

$("#on-btn-toggle-fullscreen").click(function(e)
{
	e.preventDefault();
	var result = zen.toggleFullscreen();
	if (!result.success)
	{
		$.notify(result.message, {
			position: "top right"
		});

		return;
	}	

	if (zen.isFullscreenMode())
	{
		$("#on-btn-toggle-fullscreen-icon").attr("class", "fa fa-compress");
	}
	else
	{
		$("#on-btn-toggle-fullscreen-icon").attr("class", "fa fa-expand");
	}
});