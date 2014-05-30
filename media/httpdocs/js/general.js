var zenapi = {
	animation_duration : 200,

	clearAlerts : function()
	{
		$(".alerts-zone .alert").addClass("old-alert");
		$(".old-alert").slideUp(zenapi.animation_duration);
	}
};