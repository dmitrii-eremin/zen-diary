$(document).ready(function()
{
	for (var i = 0; i < employees.categories.length; i++)
	{
		var object = employees.categories[i];

		var category_html = "<option value = '" + object.id + "'>" + object.title + "</option>";

		$("#edit-emp-category").append(category_html);
	}

	for (var i = 0; i < employees.employees.length; i++)
	{
		var emp = employees.employees[i];		
		if (emp.id == employees.current_employee)
		{
			$("#edit-lastname").val(emp.lastname);
			$("#edit-firstname").val(emp.firstname);
			$("#edit-post").val(emp.post);
			$("#edit-email").val(emp.email);
			$("#edit-phone").val(emp.phone);
			$("#edit-ip").val(emp.ip);
			$("#edit-comment").val(emp.comment);
			$("#edit-emp-category").val(emp.cat_id);
			break;
		}
	}
});

$("#on-cancel-edit-employee").click(function(e)
{
	e.preventDefault();
	$("#ajaxModal").click();
});


$("#on-save-edit-employee").click(function(e)
{
	e.preventDefault();

	var new_emp_info = {
		"id" : employees.current_employee,
		"firstname" : $("#edit-firstname").val(),
		"lastname" : $("#edit-lastname").val(),
		"post" : $("#edit-post").val(),
		"email" : $("#edit-email").val(),
		"phone" : $("#edit-phone").val(),
		"ip" : $("#edit-ip").val(),
		"comment" : $("#edit-comment").val(),
		"cat_id" : $("#edit-emp-category").val()
	};

	rv.request({
		func : $.post,
		url : "/api/edit-employee.lua",
		data :new_emp_info,
		failed: {
			name : ".alerts-zone",
			messages : {
				"1" : "[%settings::js::access-denied%]",				
			},
			handlers : {}		
		},
		success : function(data){

			for (var i = 0; i < employees.employees.length; i++)
			{
				if (employees.employees[i].id == new_emp_info.id)
				{
					employees.employees[i] = new_emp_info;
					break;
				}
			}

			employees.ShowEmployeeInfo(new_emp_info);
		}
	});	

	$("#ajaxModal").click();
});