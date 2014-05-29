(function($)
{
	$.fn.alert = function(options)
	{
		var settings = $.extend({
			"type" : "warning",
			"text" : ""			
		}, options);

		var html_data = "<div class = 'alert alert-" + settings.type + " alert-dismissable'>" + 
						"<button type = 'button' class = 'close' data-dismiss = 'alert' aria-hidden = 'true'>&times;</button>" + 
						settings.text + "</div>";

		$(this).append(html_data);
	};

})(jQuery);