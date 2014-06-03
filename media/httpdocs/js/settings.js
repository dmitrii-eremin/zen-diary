$(document).ready(function()
{
	var login = zen.getUsername();
	$("#login").val(login);
});

$("#on-btn-save").click(function(e)
{
	e.preventDefault();

	var login = $("#login").val();
	var current_password = $("#current-password").val();
	var new_password = $("#new-password").val();
	var new_password_confirm = $("#new-password-confirm").val();

	if (login.length == 0)
	{
		$.notify("Введите имя пользователя.",
		{
			position: "top right"
		});
		$("#login").focus();
		return;
	}

	if (new_password.length > 0)	
	{
		if (new_password != new_password_confirm)
		{
			$.notify("Введённые вами новые пароли не совпадают.",
			{
				position: "top right"
			});
			$("#new-password-confirm").focus();
			return;
		}

		var result = zen.loginUser(zen.getUsername(), current_password);
		if (!result.success)
		{
			$.notify("Вы неправильно ввели текущий пароль.",
			{
				position: "top right"
			});
			$("#current-password").focus();
			return;
		}

		result = zen.changeCredits(login, new_password);
		if (result.success)
		{
			$.notify("Пароль успешно изменён.",
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
	}	
});