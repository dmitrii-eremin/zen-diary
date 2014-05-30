var zenapi = {
	animation_duration : 200,

	clearAlerts : function()
	{
		$(".alerts-zone .alert").addClass("old-alert");
		$(".old-alert").slideUp(zenapi.animation_duration);
	}
};

$(function()
{
	var text_version = zen.getVersionString();
	var username = zen.getUsername();

	$(".app-version").html(text_version);
	$(".app-username").html(username);

	var location = window.location.href;
	if (location.indexOf("/index.html") != -1)
	{
		$("#nav-menu-index").addClass("active");
	}
});

$(".on-btn-logout").click(function(e)
{
	e.preventDefault();
	zen.logoutUser();
	window.location.href = "signin.html";
});