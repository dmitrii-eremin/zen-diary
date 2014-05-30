$(document).ready(function(e)
{
	if (zen.isFirstRun())
	{
		$(".signup-form").show();		
	}
	else
	{
		$(".signin-form").show();		
	}
});

$("#on-btn-signup").click(function(e)
{
	e.preventDefault();

	var login = $("#signup-login").val();
	var password = $("#signup-password").val();
	var password_confirm = $("#signup-password-confirm").val();

	if (login.length == 0)
	{
		$(".alerts-zone").alert({
			text : "Введите имя пользователя."
		});
		$("#signup-login").focus();
		return;
	}

	if (password.length == 0)
	{
		$(".alerts-zone").alert({
			text : "Введите пароль пользователя."
		});
		$("#signup-password").focus();
		return;
	}

	if (password != password_confirm)
	{
		$(".alerts-zone").alert({
			text : "Введённые вами пароли не совпадают."
		});
		$("#signup-password").focus();
		return;
	}

	var result = zen.registerUser(login, password);
	if (result.success)
	{
			$(".alerts-zone").alert({
				text : "Вы успешно прошли регистрацию. Через несколько секунд вы войдёте в свою учётную запись.</p><p>Или нажмите <a href = \"index.html\" class = \"alert-link\">здесь</a>, если не хотите ждать.",
				type : "success"
			});

			window.setTimeout(function()
			{
				window.location.href = "index.html";
			}, 5000);
	}
	else
	{
		$(".alerts-zone").alert({
			text : result.message
		});
	}
});

$("#on-btn-signin").click(function(e)
{
	e.preventDefault();

	var login = $("#signin-login").val();
	var password = $("#signin-password").val();
	
	if (login.length == 0)
	{
		$(".alerts-zone").alert({
			text : "Введите имя пользователя."
		});
		$("#signin-login").focus();
		return;
	}

	if (password.length == 0)
	{
		$(".alerts-zone").alert({
			text : "Введите пароль пользователя."
		});
		$("#signin-password").focus();
		return;
	}
	
	var result = zen.loginUser(login, password);
	if (result.success)
	{
			window.location.href = "index.html";
	}
	else
	{
		$(".alerts-zone").alert({
			text : result.message
		});
	}
});

$("#signup-login, #signup-password, #signup-password-confirm").keydown(function(e)
{
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-btn-signup").click();
	}
	else
	{
		zenapi.clearAlerts();
	}
});

$("#signin-login, #signin-password").keydown(function(e)
{	
	if (e.keyCode == 13)
	{
		e.preventDefault();
		$("#on-btn-signin").click();
	}
	else
	{
		zenapi.clearAlerts();
	}
});