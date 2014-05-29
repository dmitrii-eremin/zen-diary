$(document).ready(function()
{
	rv.request({
		func : $.get,
		url : "/api/get-categories.lua",
		data : {},
		success : function(data){
			for (var i = 0; i < data.length; i++)
			{
				var html_data = "<option value = " + data[i].id + ">" + data[i].title + "</option>";
				$("#emp-category").append(html_data);
			}	
		}
	});
});

$("#on-add-employee").click(function(e)
{
	e.preventDefault();

	var firstname = $("#firstname").val();
	var lastname = $("#lastname").val();
	var post = $("#post").val();
	var emp_cat = $("#emp-category").val();
	var email = $("#email").val();
	var phone = $("#phone").val();
	var ip = $("#ip").val();
	var comment = $("#comment").val();

	rv.request({
		func : $.post,
		url : "/api/add-employee.lua",
		data : {
			firstname : firstname,
			lastname : lastname,
			post : post,
			cat_id : emp_cat,
			email : email,
			phone : phone,
			ip : ip,
			comment : comment
		},

		success : function(data)
		{
			$("#firstname").val("");
			$("#lastname").val("");
			$("#post").val("");
			$("#emp-category").val("-1");
			$("#email").val("");
			$("#phone").val("");
			$("#ip").val("");
			$("#comment").val("");

			var emp_object = {
				cat_id : emp_cat,
				comment : comment,
				email : email,
				firstname : firstname,
				lastname : lastname,
				ip : ip,
				phone : phone,
				post : post,
				id : data
			};

			if (employees.employees == null)
			{
				employees.employees = []
			}

			employees.employees.push(emp_object);	

			employees.AddEmployee(emp_object);
		}
	});
});