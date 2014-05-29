$("#category").keydown(function(e)
{
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-add-category").click();
	}
});

$("#on-add-category").click(function(e)
{
	var cat = $("#category").val();

	if (cat.length > 0)
	{
		rv.request({
			func : $.post,
			url : "/api/add-category.lua",
			data : {
				name : cat
			},		
			success : function(data){
				var object = {
					id : data,
					title : cat
				};

				if (employees.categories == null)
				{
					employees.categories = []
				}

				employees.categories.push(object);
				employees.AddCategory(object);		
			}
			});
	}

	$("#category").val("");	
	$("#category").focus();
});	