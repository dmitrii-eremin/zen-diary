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
		$.notify("Введите имя пользователя.", 
		{
			position: "top right"
		});
		$("#signup-login").focus();
		return;
	}

	if (password.length == 0)
	{
		$.notify("Введите пароль пользователя.", 
		{
			position: "top right"
		});
		$("#signup-password").focus();
		return;
	}

	if (password != password_confirm)
	{
		$.notify("Введённые вами пароли не совпадают.", 
		{
			position: "top right"
		});
		$("#signup-password").focus();
		return;
	}

	var result = zen.registerUser(login, password);
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

$("#on-btn-signin").click(function(e)
{
	e.preventDefault();

	var login = $("#signin-login").val();
	var password = $("#signin-password").val();
	
	if (login.length == 0)
	{
		$.notify("Введите имя пользователя.", 
		{
			position: "top right",			
		});

		$("#signin-login").focus();
		return;
	}

	if (password.length == 0)
	{
		$.notify("Введите пароль пользователя.", 
		{
			position: "top right",			
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
		$.notify(result.message, 
		{
			position: "top right",			
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